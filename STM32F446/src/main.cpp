#include <mbed.h>
//#include <BufferedSerial.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define AUTH_KEY 0xF9
#define GYRO_CAL 0x04
#define STM32_ARM_TEST 0x9F
#define STM32_ARM_CONF 0x0A
#define MOTOR_TEST 0x0F
#define NO_MOTORS 0x0E

//Communication Pins
I2C i2c(PB_9,PB_8);                         //sda,scl
//Serial pc(USBTX, USBRX);                    //tx,rx
Serial radio(PB_6, PB_7);
SPISlave spi(PA_7, PA_6, PA_5, PA_4);       //mosi, miso, sclk, ssel
Timer onTime;

//PWM Motor Pins
DigitalOut motor1(PC_5);
DigitalOut motor2(PC_4);
DigitalOut motor3(PB_0);
DigitalOut motor4(PB_1);

//Radio Serial Buffer
char radioBuffer[128];

//RF Protocol Coefficients
const int throttleCoefficient = 3;
const int rollCoefficient = 4;
const int pitchCoefficient = 5;
const int yawCoefficient = 6;
bool authenticated = false;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PID gain and limit settings
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float pid_p_gain_roll = 3.8;                //Gain setting for the roll P-controller
float pid_i_gain_roll = 0.01;               //Gain setting for the roll I-controller
float pid_d_gain_roll = 20.0;               //Gain setting for the roll D-controller
int pid_max_roll = 400;                     //Maximum output of the PID-controller (+/-)

float pid_p_gain_pitch = pid_p_gain_roll;   //Gain setting for the pitch P-controller.
float pid_i_gain_pitch = pid_i_gain_roll;   //Gain setting for the pitch I-controller.
float pid_d_gain_pitch = pid_d_gain_roll;   //Gain setting for the pitch D-controller.
int pid_max_pitch = pid_max_roll;           //Maximum output of the PID-controller (+/-)

float pid_p_gain_yaw = 9.0;                 //Gain setting for the pitch P-controller. //4.0
float pid_i_gain_yaw = 0.1;                 //Gain setting for the pitch I-controller. //0.02
float pid_d_gain_yaw = 0.0;                 //Gain setting for the pitch D-controller.
int pid_max_yaw = 400;                      //Maximum output of the PID-controller (+/-)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Declaring global variables
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t last_channel_1, last_channel_2, last_channel_3, last_channel_4;
uint8_t highByte, lowByte;
volatile int receiver_input_roll, receiver_input_pitch, receiver_input_throttle, receiver_input_yaw;
volatile int mod_receiver_input_throttle;
int counter_channel_1, counter_channel_2, counter_channel_3, counter_channel_4, loop_counter;
int esc_1, esc_2, esc_3, esc_4;
int throttle; 
//battery_voltage;
int cal_int, start, gyro_address;
int receiver_input[5];
int temperature;
int acc_axis[4], gyro_axis[4];
float roll_level_adjust, pitch_level_adjust;

long acc_x, acc_y, acc_z, acc_total_vector;
unsigned long timer_channel_1, timer_channel_2, timer_channel_3, timer_channel_4, esc_timer, esc_loop_timer;
unsigned long timer_1, timer_2, timer_3, timer_4, current_time;
unsigned long loop_timer;
double gyro_pitch, gyro_roll, gyro_yaw;
double gyro_axis_cal[4];
float pid_error_temp;
float pid_i_mem_roll, pid_roll_setpoint, gyro_roll_input, pid_output_roll, pid_last_roll_d_error;
float pid_i_mem_pitch, pid_pitch_setpoint, gyro_pitch_input, pid_output_pitch, pid_last_pitch_d_error;
float pid_i_mem_yaw, pid_yaw_setpoint, gyro_yaw_input, pid_output_yaw, pid_last_yaw_d_error;
float angle_roll_acc, angle_pitch_acc, angle_pitch, angle_roll;
bool gyro_angles_set;
bool coFlag = false;
int coefficient;
int count = 0;
bool wordStart = false;
bool wordEnd = false;

//Sends all motor PWM signals LOW
void motors_off() {
  motor1 = 0;
  motor2 = 0;
  motor3 = 0;
  motor4 = 0;
}

//Sends all motor PWM signals HIGH
void motors_on() {
  motor1 = 1;
  motor2 = 1;
  motor3 = 1;
  motor4 = 1;
}

void readline() {
  //Read character incoming on serial bus
  char thisChar = radio.getc();
  //pc.printf("%c\r\n", thisChar);

  //Check if this character is the end of message
  if (thisChar == '\n') {
    wordEnd = true;
    radioBuffer[count] = '\0';
    count = 0;
    return;
  }

  //If we just finished a message, start a new one in the buffer
  else if (wordEnd == true) {
    radioBuffer[count] = thisChar;
    count++;
    wordEnd = false;
    return;
  }

  //Assign the next character to the current buffer
  else {
    radioBuffer[count] = thisChar;
    count++;
    return;
  }
}

void updateTelemetry(int data, int myCoefficient) {
  //Figuring out which coefficient the data corresponds to, and setting it
  switch (myCoefficient) {
    case throttleCoefficient:
      receiver_input_throttle = data;
      break;
    case rollCoefficient:
      receiver_input_roll = data;
      break;
    case pitchCoefficient:
      receiver_input_pitch = data;
      break;
    case yawCoefficient:
      receiver_input_yaw = data;
      break;
    default:
      break;
  }
}

void rxInterrupt() {
  readline();                                                             //Read data from serial bus 
  if (wordEnd == true) {                                                  //If we have finished a message
    int data = (int)strtol(radioBuffer, NULL, 10);                             //Convert hex data to decimal
    //pc.printf("%d\r\n", data);
    if (coFlag == true && data > 999) {                                   //If we have a coefficient and data for PWM is valid
      updateTelemetry(data, coefficient);                                 //Update the input values
      coFlag = false;
    }
    else {
      if (data < 10) {                                                    //If data is less than 10, it is a coefficient
        coFlag = true;
      }
      coefficient = data;
    }
    memset(radioBuffer,0,sizeof(radioBuffer));
  }
  else {
    return;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Subroutine for reading the gyro
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gyro_signalen() {
  char cmd[14];
  cmd[0] = 0x2D;
  i2c.write(gyro_address, cmd, 1);                                       //Write to first data I2C register
  i2c.read(gyro_address, cmd, 14);                                       //Incremental I2C read

  acc_axis[1] = (short)(cmd[0]<<8|cmd[1]);                               //Add the low and high byte to the acc_x variable.
  acc_axis[2] = (short)(cmd[2]<<8|cmd[3]);                               //Add the low and high byte to the acc_y variable.
  acc_axis[3] = (short)(cmd[4]<<8|cmd[5]);                               //Add the low and high byte to the acc_z variable.
  gyro_axis[1] = (short)(cmd[6]<<8|cmd[7]);                               //Add the low and high byte to the temperature variable.
  gyro_axis[2] = (short)(cmd[8]<<8|cmd[9]);                              //Read high and low part of the angular data. x
  gyro_axis[3] = (short)(cmd[10]<<8|cmd[11]);                            //Read high and low part of the angular data. y
  temperature = (short)(cmd[12]<<8|cmd[13]);                            //Read high and low part of the angular data. z
  memset(cmd,0,sizeof(cmd));
  
  //Only compensate after the calibration.
  if (cal_int == 2000) {
    gyro_axis[1] -= gyro_axis_cal[1];                                    
    gyro_axis[2] -= gyro_axis_cal[2];                                    
    gyro_axis[3] -= gyro_axis_cal[3];                                    
  }

  //Assign and invert appropriate gyro and acc axis
  gyro_roll = gyro_axis[2];                                            
  gyro_pitch = gyro_axis[1];                    
  gyro_yaw = gyro_axis[3] * -1;                       
  acc_x = acc_axis[1];                           
  acc_y = acc_axis[2];                           
  acc_z = acc_axis[3] * -1;
}

void calculate_angles() {
  //65.5 = 1 deg/sec in gyro scale
  gyro_roll_input = (gyro_roll_input * 0.7) + ((gyro_roll / 65.5) * 0.3);   //Gyro pid input is deg/sec.
  gyro_pitch_input = (gyro_pitch_input * 0.7) + ((gyro_pitch / 65.5) * 0.3);//Gyro pid input is deg/sec.
  gyro_yaw_input = (gyro_yaw_input * 0.7) + ((gyro_yaw / 65.5) * 0.3);      //Gyro pid input is deg/sec.
  
  //Gyro angle calculations
  //0.0000611 = (0.004 / 65.5)
  angle_pitch += gyro_pitch * 0.0000611;                                    //Calculate the traveled pitch angle and add this to the angle_pitch variable.
  angle_roll += gyro_roll * 0.0000611;                                      //Calculate the traveled roll angle and add this to the angle_roll variable.

  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
  angle_pitch -= angle_roll * sin(gyro_yaw * 0.000001066);                  //If the IMU has yawed transfer the roll angle to the pitch angle.
  angle_roll += angle_pitch * sin(gyro_yaw * 0.000001066);                  //If the IMU has yawed transfer the pitch angle to the roll angle.

  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));       //Calculate the total accelerometer vector.
  
  if (abs(acc_y) < acc_total_vector) {                                      //Prevent the asin function to produce a NaN
    angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;          //Calculate the pitch angle.
  }

  if (abs(acc_x) < acc_total_vector) {                                      //Prevent the asin function to produce a NaN
    angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;          //Calculate the roll angle.
  }
  
  //Place the MPU-6050 spirit level and note the values in the following two lines for calibration.
  angle_pitch_acc -= 0.0;                                                   //Accelerometer calibration value for pitch.
  angle_roll_acc -= 0.0;                                                    //Accelerometer calibration value for roll.
  
  angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;            //Correct the drift of the gyro pitch angle with the accelerometer pitch angle.
  angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;               //Correct the drift of the gyro roll angle with the accelerometer roll angle.
  
  pitch_level_adjust = angle_pitch * 12;                                    //Calculate the pitch angle correction
  roll_level_adjust = angle_roll * 12;                                      //Calculate the roll angle correction                         
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Subroutine for calculating PID outputs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void calculate_pid() {
  //Roll calculations
  pid_error_temp = gyro_roll_input - pid_roll_setpoint;
  pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
  if(pid_i_mem_roll > pid_max_roll)pid_i_mem_roll = pid_max_roll;
  else if(pid_i_mem_roll < pid_max_roll * -1)pid_i_mem_roll = pid_max_roll * -1;

  pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error);
  if(pid_output_roll > pid_max_roll)pid_output_roll = pid_max_roll;
  else if(pid_output_roll < pid_max_roll * -1)pid_output_roll = pid_max_roll * -1;

  pid_last_roll_d_error = pid_error_temp;

  //Pitch calculations
  pid_error_temp = gyro_pitch_input - pid_pitch_setpoint;
  pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
  if(pid_i_mem_pitch > pid_max_pitch)pid_i_mem_pitch = pid_max_pitch;
  else if(pid_i_mem_pitch < pid_max_pitch * -1)pid_i_mem_pitch = pid_max_pitch * -1;

  pid_output_pitch = pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch + pid_d_gain_pitch * (pid_error_temp - pid_last_pitch_d_error);
  if(pid_output_pitch > pid_max_pitch)pid_output_pitch = pid_max_pitch;
  else if(pid_output_pitch < pid_max_pitch * -1)pid_output_pitch = pid_max_pitch * -1;

  pid_last_pitch_d_error = pid_error_temp;

  //Yaw calculations
  pid_error_temp = gyro_yaw_input - pid_yaw_setpoint;
  pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
  if(pid_i_mem_yaw > pid_max_yaw)pid_i_mem_yaw = pid_max_yaw;
  else if(pid_i_mem_yaw < pid_max_yaw * -1)pid_i_mem_yaw = pid_max_yaw * -1;

  pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw + pid_d_gain_yaw * (pid_error_temp - pid_last_yaw_d_error);
  if(pid_output_yaw > pid_max_yaw)pid_output_yaw = pid_max_yaw;
  else if(pid_output_yaw < pid_max_yaw * -1)pid_output_yaw = pid_max_yaw * -1;

  pid_last_yaw_d_error = pid_error_temp;
}

void set_gyro_registers(){
  char cmd[2];
  
  //Changing register bank
  cmd[0] = 0x7F;                                                                //We want to write to the REG_BANK_SEL register (7F hex)
  cmd[1] = 0x00;                                                                //Set the register bits as 00000000 to select USER BANK 0
  i2c.write(gyro_address, cmd, 2);
  memset(cmd,0,sizeof(cmd));

  cmd[0] = 0x06;                                                                //We want to write to the PWR_MGMT_1 register (06 hex)
  cmd[1] = 0x01;                                                                //Set the register bits as 00000000 to activate the gyro
  i2c.write(gyro_address, cmd, 2);
  memset(cmd,0,sizeof(cmd));

  //Changing register bank
  cmd[0] = 0x7F;                                                                //We want to write to the REG_BANK_SEL register (7F hex)
  cmd[1] = 0x20;                                                                //Set the register bits as 00100000 to select USER BANK 2
  i2c.write(gyro_address, cmd, 2);
  memset(cmd,0,sizeof(cmd));

  cmd[0] = 0x01;                                                                //We want to write to the GYRO_CONFIG_1 register (01 hex)
  cmd[1] = 0x23;                                                                //Set the register bits as 00100011 (500dps full scale)
  i2c.write(gyro_address, cmd, 2);
  memset(cmd,0,sizeof(cmd));
  
  cmd[0] = 0x14;                                                                //We want to write to the ACCEL_CONFIG register (1C hex)
  cmd[1] = 0x25;                                                                //Set the register bits as 00100101 (+/- 8g full scale range)
  i2c.write(gyro_address, cmd, 2);
  memset(cmd,0,sizeof(cmd));

  //Changing register bank
  cmd[0] = 0x7F;                                                                //We want to write to the REG_BANK_SEL register (7F hex)
  cmd[1] = 0x00;                                                                //Set the register bits as 00000000 to select USER BANK 0
  i2c.write(gyro_address, cmd, 2);
  memset(cmd,0,sizeof(cmd));
}

void testMotor(DigitalOut motor) {
  int start = onTime.read_ms();
  while(onTime.read_ms() - start < 1000) {
    motor = 1;
    wait(.001);
    motor = 0;
    wait(.003);
  }
  start = onTime.read_ms();
  while(onTime.read_ms() - start < 5000) {
    motor = 1;
    wait(.0012);
    motor = 0;
    wait(.0028);
  }
}

void testMotors() {
  testMotor(motor1);
  testMotor(motor2);
  testMotor(motor3);
  testMotor(motor4);
}

void authRasPiCM3() {
  //Load authentication key into SPI buffer
  spi.reply(0x01);
  authenticated = false;

  //Stay in this loop until the flight controller (STM32) has made contact with the Raspberry Pi
  while (authenticated == false) {
    motors_on();                                                              //Set motor PWM signals high
    wait(.001);                                                               //Wait 1000us
    motors_off();                                                             //Set motor PWM signals low
    int start = onTime.read_us();
    while(onTime.read_us() - start < 3000) {
      if (spi.receive()) {
        int response = spi.read();
        if (response == 0x01) spi.reply(AUTH_KEY);
        if (response == AUTH_KEY) authenticated = true;
      }
    }
    //wait(.003);                                                               //Wait 3 milliseconds before the next loop
  }
}

int main() {
  //Configure communications
  onTime.start();                                                             //Start loop timer
  i2c.frequency(400000);                                                      //I2C Frequency set to 400kHz
  radio.baud(9600);                                                           //Serial Radio baud rate at 9600bps
  radio.attach(&rxInterrupt);
  
  //Setup the spi for 16 bit data, mode 0 and 1.5MHz clock rate
  spi.format(16,0);
  spi.frequency(3000000);
  
  authRasPiCM3();

  //Load SPI buffer with dummy byte
  spi.reply(0x03);

  start = 0;                                                                  //Set start back to zero
  gyro_address = 0x69<<1;                                                     //Store the gyro address

  set_gyro_registers();                                                       //Set the specific gyro registers
  
  //Let's take multiple gyro data samples so we can determine the average gyro offset (calibration).
  cal_int = 0;
  while (cal_int < 2000) {                            //Take 2000 readings for calibration.
    //We don't want the esc's to be beeping annoyingly. So let's give them a 1000us puls while calibrating the gyro.
    motors_on();                                                              //Set motor PWM signals high
    wait(.001);                                                               //Wait 1000us
    motors_off();                                                             //Set motor PWM signals low
    
    int start = onTime.read_us();
    while ((onTime.read_us() - start < 3000) && cal_int < 2000) {
      gyro_signalen();                                                          //Read the gyro output.
      gyro_axis_cal[1] += gyro_axis[1];                                         //Add roll value to gyro_roll_cal.
      gyro_axis_cal[2] += gyro_axis[2];                                         //Add pitch value to gyro_pitch_cal.
      gyro_axis_cal[3] += gyro_axis[3];                                         //Add yaw value to gyro_yaw_cal.
      cal_int += 1;
    }
    //wait(.003);                                                               //Wait 3 milliseconds before the next loop
  }

  //Now that we have 2000 measures, we need to divide by 2000 to get the average gyro offset.
  gyro_axis_cal[1] /= 2000;                                                   //Divide the roll total by 2000.
  gyro_axis_cal[2] /= 2000;                                                   //Divide the pitch total by 2000.
  gyro_axis_cal[3] /= 2000;                                                   //Divide the yaw total by 2000.
  
 //Let's take multiple gyro data samples so we can determine the average gyro offset (calibration).
  // for (cal_int = 0; cal_int < 2000 ; cal_int ++){                           //Take 2000 readings for calibration.
  //   gyro_signalen();                                                        //Read the gyro output.
  //   gyro_axis_cal[1] += gyro_axis[1];                                       //Add roll value to gyro_roll_cal.
  //   gyro_axis_cal[2] += gyro_axis[2];                                       //Add pitch value to gyro_pitch_cal.
  //   gyro_axis_cal[3] += gyro_axis[3];                                       //Add yaw value to gyro_yaw_cal.
  //   //We don't want the esc's to be beeping annoyingly. So let's give them a 1000us puls while calibrating the gyro.
  //   motors_on();
  //   wait(.001);
  //   motors_off();
  //   wait(.003);
  // }

  // //Now that we have 2000 measures, we need to devide by 2000 to get the average gyro offset.
  // gyro_axis_cal[1] /= 2000;                                                 //Divide the roll total by 2000.
  // gyro_axis_cal[2] /= 2000;                                                 //Divide the pitch total by 2000.
  // gyro_axis_cal[3] /= 2000;                                                 //Divide the yaw total by 2000.

  spi.reply(GYRO_CAL);

  //Wait until the receiver is active and the throttle is set to the lower position.
  bool armed = false;
  bool noMotors = false;
  while((receiver_input_throttle < 990 || receiver_input_throttle > 1020 || receiver_input_yaw < 1400) && !armed) {
    //We don't want the ESCs to be beeping annoyingly. So let's give them a 1000us pulse while calibrating the gyro.
    motors_on();                                                              //Set motor PWM signals high
    wait(.001);                                                               //Wait 1000us
    motors_off();                                                             //Set motor PWM signals low
    int start = onTime.read_us();
    while (onTime.read_us() - start < 3000) {
      if (spi.receive()) {
        int data = spi.read();
        if (data == STM32_ARM_TEST) spi.reply(STM32_ARM_CONF);
        if (data == STM32_ARM_CONF) armed = true;
        if (data == MOTOR_TEST) {
          testMotors();
          spi.reply(STM32_ARM_CONF);
          armed = true;
          break;
        }
        if (data == NO_MOTORS) {
          spi.reply(STM32_ARM_CONF);
          noMotors = true;
          armed = true;
          break;
        }
      }
    }
  }
  start = 0;                                                                  //Set start back to 0.

  spi.reply(0x05);

  //calculate_angles();
  loop_timer = onTime.read_us();                                              //First timer reading (starting main loop)
  
  //Infinite PID Loop
  while(1) {
    calculate_angles();
    //For starting the motors: throttle low and yaw left (step 1)
    if((receiver_input_throttle < 1050 && receiver_input_yaw < 1050 && receiver_input_yaw > 990) || armed) {
      start = 2;

      angle_pitch = angle_pitch_acc;                                         //Set the gyro pitch angle equal to the accelerometer pitch angle when the quadcopter is started.
      angle_roll = angle_roll_acc;                                           //Set the gyro roll angle equal to the accelerometer roll angle when the quadcopter is started.
      gyro_angles_set = true;                                                //Set the IMU started flag.
      
      //Reset the PID controllers for a bumpless start.
      pid_i_mem_roll = 0;
      pid_last_roll_d_error = 0;
      pid_i_mem_pitch = 0;
      pid_last_pitch_d_error = 0;
      pid_i_mem_yaw = 0;
      pid_last_yaw_d_error = 0;

      armed = false;
    }
    if (noMotors) start = 1;
    // //When yaw stick is back in the center position start the motors (step 2)
    // if ((start == 1 && receiver_input_throttle < 1050 && receiver_input_yaw > 1450) || armed) {
    //   start = 2;
    //   angle_pitch = angle_pitch_acc;                                         //Set the gyro pitch angle equal to the accelerometer pitch angle when the quadcopter is started.
    //   angle_roll = angle_roll_acc;                                           //Set the gyro roll angle equal to the accelerometer roll angle when the quadcopter is started.
    //   gyro_angles_set = true;                                                //Set the IMU started flag.
      
    //   //Reset the PID controllers for a bumpless start.
    //   pid_i_mem_roll = 0;
    //   pid_last_roll_d_error = 0;
    //   pid_i_mem_pitch = 0;
    //   pid_last_pitch_d_error = 0;
    //   pid_i_mem_yaw = 0;
    //   pid_last_yaw_d_error = 0;

    //   armed = false;
    // }

    //Stopping the motors: throttle low and yaw right.
    if (start == 2 && receiver_input_throttle < 1050 && receiver_input_yaw > 1950) {
      start = 0;
    }
    
    //start = 2;
    // receiver_input_roll = 1500;
    // receiver_input_pitch = 1500;
    // receiver_input_yaw = 1500;

    //The PID set point in degrees per second is determined by the roll receiver input.
    //In the case of dividing by 3 the max roll rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
    pid_roll_setpoint = 0;
    
    //We need a little dead band of 16us for better results.
    if (receiver_input_roll > 1508) {
      pid_roll_setpoint = receiver_input_roll - 1508;
    }
    else if (receiver_input_roll < 1492) {
      pid_roll_setpoint = receiver_input_roll - 1492;
    }

    pid_roll_setpoint -= roll_level_adjust;                                   //Subtract the angle correction from the standardized receiver roll input value.
    pid_roll_setpoint /= 3.0;                                                 //Divide the setpoint for the PID roll controller by 3 to get angles in degrees.
  
  
    //The PID set point in degrees per second is determined by the pitch receiver input.
    //In the case of dividing by 3 the max pitch rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
    pid_pitch_setpoint = 0;
    //We need a little dead band of 16us for better results.
    if (receiver_input_pitch > 1508) pid_pitch_setpoint = receiver_input_pitch - 1508;
    else if (receiver_input_pitch < 1492) pid_pitch_setpoint = receiver_input_pitch - 1492;
  
    pid_pitch_setpoint -= pitch_level_adjust;                                  //Subtract the angle correction from the standardized receiver pitch input value.
    pid_pitch_setpoint /= 3.0;                                                 //Divide the setpoint for the PID pitch controller by 3 to get angles in degrees.
  
    //The PID set point in degrees per second is determined by the yaw receiver input.
    //In the case of deviding by 3 the max yaw rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
    pid_yaw_setpoint = 0;
    //We need a little dead band of 16us for better results.
    if (receiver_input_throttle > 1050) { //Do not yaw when turning off the motors.
      if (receiver_input_yaw > 1508) pid_yaw_setpoint = (receiver_input_yaw - 1508)/3.0;
      else if (receiver_input_yaw < 1492) pid_yaw_setpoint = (receiver_input_yaw - 1492)/3.0;
    }
    
    calculate_pid();                                                          //PID inputs are known. So we can calculate the pid output.

    throttle = mod_receiver_input_throttle;                                   //We need the throttle signal as a base signal, and add PID altitude control factor
    //throttle = 1500;

    if (start == 2) {                                                          //The motors are started.
      if (throttle > 1800) throttle = 1800;                                   //We need some room to keep full control at full throttle.
      esc_1 = throttle - pid_output_pitch + pid_output_roll - pid_output_yaw; //Calculate the pulse for esc 1 (front-right - CCW)
      esc_2 = throttle + pid_output_pitch + pid_output_roll + pid_output_yaw; //Calculate the pulse for esc 2 (rear-right - CW)
      esc_3 = throttle + pid_output_pitch - pid_output_roll - pid_output_yaw; //Calculate the pulse for esc 3 (rear-left - CCW)
      esc_4 = throttle - pid_output_pitch - pid_output_roll + pid_output_yaw; //Calculate the pulse for esc 4 (front-left - CW)

      // esc_1 = 1500; //Calculate the pulse for esc 1 (front-right - CCW)
      // esc_2 = 1500; //Calculate the pulse for esc 2 (rear-right - CW)
      // esc_3 = 1500; //Calculate the pulse for esc 3 (rear-left - CCW)
      // esc_4 = 1500; //Calculate the pulse for esc 4 (front-left - CW)

      if (esc_1 < 1100) esc_1 = 1100;                                         //Keep the motors running.
      if (esc_2 < 1100) esc_2 = 1100;                                         //Keep the motors running.
      if (esc_3 < 1100) esc_3 = 1100;                                         //Keep the motors running.
      if (esc_4 < 1100) esc_4 = 1100;                                         //Keep the motors running.

      if (esc_1 < 1010) esc_1 = 1000;                                         //Keep the motors running.
      if (esc_2 < 1010) esc_2 = 1000;                                         //Keep the motors running.
      if (esc_3 < 1010) esc_3 = 1000;                                         //Keep the motors running.
      if (esc_4 < 1010) esc_4 = 1000;                                         //Keep the motors running.
      
      if (esc_1 > 2000) esc_1 = 2000;                                           //Limit the esc-1 pulse to 2000us.
      if (esc_2 > 2000) esc_2 = 2000;                                           //Limit the esc-2 pulse to 2000us.
      if (esc_3 > 2000) esc_3 = 2000;                                           //Limit the esc-3 pulse to 2000us.
      if (esc_4 > 2000) esc_4 = 2000;                                           //Limit the esc-4 pulse to 2000us.  
    }
    else {
      esc_1 = 1000;                                                           //If start is not 2 keep a 1000us pulse for esc-1.
      esc_2 = 1000;                                                           //If start is not 2 keep a 1000us pulse for esc-2.
      esc_3 = 1000;                                                           //If start is not 2 keep a 1000us pulse for esc-3.
      esc_4 = 1000;                                                           //If start is not 2 keep a 1000us pulse for esc-4.
    }
    
    // We wait until 4000us are passed.
    if ((onTime.read_us() - loop_timer < 4000)) {
      if (spi.receive()) {
        short int data = spi.read();
        if (data >= 0 && data <= 1000) {
          mod_receiver_input_throttle = data + 1000;
        }
      }
    }
    //spi.reply(((signed char)angle_pitch << 8) | ((signed char)angle_roll & 0xFF));
    while (onTime.read_us() - loop_timer < 4000);
    loop_timer = onTime.read_us();                                            //Set the timer for the next loop.

    //RISING EDGE of PWM motor pulses (start of loop)
    motors_on();
    
    timer_channel_1 = esc_1 + loop_timer;                                     //Calculate the time of the falling edge of the esc-1 pulse.
    timer_channel_2 = esc_2 + loop_timer;                                     //Calculate the time of the falling edge of the esc-2 pulse.
    timer_channel_3 = esc_3 + loop_timer;                                     //Calculate the time of the falling edge of the esc-3 pulse.
    timer_channel_4 = esc_4 + loop_timer;                                     //Calculate the time of the falling edge of the esc-4 pulse.
    
    //There is always 1000us of spare time. So let's do something useful that is very time consuming.
    //Get the current gyro and receiver data and scale it to degrees per second for the pid calculations.
    
    gyro_signalen();
    //calculate_angles();

    //CLOCK SPEED TEST
    //spi.reply(SystemCoreClock/1000000);
    
    //FALLING EDGES of PWM motor pulses
    while (motor1 == 1 || motor2 == 1 || motor3 == 1 || motor4 == 1) {        //Stay in this loop until all motor PWM signals are low
      esc_loop_timer = onTime.read_us();                                      //Read the current time.
      if(timer_channel_1 <= esc_loop_timer) motor1 = 0;                        //Set digital output 7 to low if the time is expired.
      if(timer_channel_2 <= esc_loop_timer) motor2 = 0;                        //Set digital output 6 to low if the time is expired.
      if(timer_channel_3 <= esc_loop_timer) motor3 = 0;                        //Set digital output 5 to low if the time is expired.
      if(timer_channel_4 <= esc_loop_timer) motor4 = 0;                        //Set digital output 4 to low if the time is expired.
    }
  }
}