#include <mbed.h>
#include <Servo.h>
#include <BufferedSerial.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

DigitalOut led(LED1);
I2C i2c(D14,D15); //sda,scl
Serial pc(USBTX, USBRX); //tx,rx
Serial device(D10, D2);
Timer onTime;

//PWM Motor Pins
DigitalOut motor1(D3);
DigitalOut motor2(D9);
DigitalOut motor3(D5);
DigitalOut motor4(D6);

//Device Serial Buffer
char buffer[128];

//RF Protocol Coefficients
const int throttleCoefficient = 3;
const int rollCoefficient = 4;
const int pitchCoefficient = 5;
const int yawCoefficient = 6;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PID gain and limit settings
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float pid_p_gain_roll = 3.8;               //Gain setting for the roll P-controller
float pid_i_gain_roll = 0.01;              //Gain setting for the roll I-controller
float pid_d_gain_roll = 20.0;              //Gain setting for the roll D-controller
int pid_max_roll = 400;                    //Maximum output of the PID-controller (+/-)

float pid_p_gain_pitch = pid_p_gain_roll;  //Gain setting for the pitch P-controller.
float pid_i_gain_pitch = pid_i_gain_roll;  //Gain setting for the pitch I-controller.
float pid_d_gain_pitch = pid_d_gain_roll;  //Gain setting for the pitch D-controller.
int pid_max_pitch = pid_max_roll;          //Maximum output of the PID-controller (+/-)

float pid_p_gain_yaw = 9.0;                //Gain setting for the pitch P-controller. //4.0
float pid_i_gain_yaw = 0.1;               //Gain setting for the pitch I-controller. //0.02
float pid_d_gain_yaw = 0.0;                //Gain setting for the pitch D-controller.
int pid_max_yaw = 400;                     //Maximum output of the PID-controller (+/-)

bool auto_level = true;                 //Auto level on (true) or off (false)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Declaring global variables
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t last_channel_1, last_channel_2, last_channel_3, last_channel_4;
//uint8_t eeprom_data[36];
uint8_t highByte, lowByte;
//volatile int receiver_input_roll, receiver_input_pitch, receiver_input_throttle, receiver_input_yaw;
volatile int receiver_input_roll, receiver_input_pitch, receiver_input_throttle, receiver_input_yaw;
int counter_channel_1, counter_channel_2, counter_channel_3, counter_channel_4, loop_counter;
int esc_1, esc_2, esc_3, esc_4;
int throttle, battery_voltage;
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

void motors_off() {
  motor1 = 0;
  motor2 = 0;
  motor3 = 0;
  motor4 = 0;
}

void motors_on() {
  motor1 = 1;
  motor2 = 1;
  motor3 = 1;
  motor4 = 1;
}

void readline() {
  char thisChar = device.getc();
  //pc.printf("%c\r\n", thisChar);
  
  // while(thisChar != '\n') {
  //     buffer[count] = thisChar;
  //     thisChar = device.getc();
  //     count++;
  // }
  // buffer[count] = '\0';
  if (thisChar == '\n') {
    wordEnd = true;
    buffer[count] = '\0';
    count = 0;
    return;
  }
  else if (wordEnd == true) {
    buffer[count] = thisChar;
    count++;
    wordEnd = false;
    return;
  }
  else {
    buffer[count] = thisChar;
    count++;
    return;
  }
}

void updateTelemetry(int data, int myCoefficient) {
  //int coefficient = (int)strtol(buffer, NULL, 16);
  //pc.printf("Throttle: %d\r\n", coefficient);
  //pc.printf("Data: %d ", data);
  //pc.printf("Co: %d\r\n", myCoefficient);
  switch (myCoefficient) {
    case throttleCoefficient:
      //readline();
      //sscanf(buffer, "%d", &receiver_input_throttle);
      receiver_input_throttle = data;
      //pc.printf("Updated Throttle\r\n");
      break;
    case rollCoefficient:
      //readline();
      //sscanf(buffer, "%d", &receiver_input_roll);
      receiver_input_roll = data;
      //pc.printf("Updated Roll\r\n");
      break;
    case pitchCoefficient:
      //readline();
      //sscanf(buffer, "%d", &receiver_input_pitch);
      receiver_input_pitch = data;
      break;
    case yawCoefficient:
      //readline();
      //sscanf(buffer, "%d", &receiver_input_yaw);
      receiver_input_yaw = data;
      break;
    default:
      break;
  }
}

void rxInterrupt() {
  // if (start == 2) {
  //   if (onTime.read_us() - loop_timer > 3500) {
  //     //pc.printf("fail\r\n");
  //     return;
  //   }
  // }
  readline();
  if (wordEnd == true) {
    int data = (int)strtol(buffer, NULL, 10);
    //pc.printf("%d\r\n", data);
    if (coFlag == true && data > 999) {
      updateTelemetry(data, coefficient);
      coFlag = false;
    }
    else  {
      if (data < 10) {
        coFlag = true;
      }
      coefficient = data;
    }
    memset(buffer,0,sizeof(buffer));
  }
  else {
    return;
  }
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Subroutine for reading the gyro
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gyro_signalen(){
  //pc.printf("Gyro Signal Reading\r\n");
  //Read the MPU-6050
  //if(eeprom_data[31] == 1){
    //Wire.beginTransmission(gyro_address);                                   //Start communication with the gyro.
    //Wire.write(0x3B);                                                       //Start reading @ register 43h and auto increment with every read.
    //Wire.endTransmission();                                                 //End the transmission.
    //Wire.requestFrom(gyro_address,14);                                      //Request 14 bytes from the gyro.
    
    //receiver_input_roll = convert_receiver_channel(1);                 //Convert the actual receiver signals for pitch to the standard 1000 - 2000us.
    //receiver_input_pitch = convert_receiver_channel(2);                 //Convert the actual receiver signals for roll to the standard 1000 - 2000us.
    //receiver_input_throttle = convert_receiver_channel(3);                 //Convert the actual receiver signals for throttle to the standard 1000 - 2000us.
    //receiver_input_yaw = convert_receiver_channel(4);                 //Convert the actual receiver signals for yaw to the standard 1000 - 2000us.
    
    //while(Wire.available() < 14);                                           //Wait until the 14 bytes are received.
    char cmd[14];
    cmd[0] = 0x3B;
    i2c.write(gyro_address, cmd, 1);
    //char sensorData[14];
    i2c.read(gyro_address, cmd, 14);
    acc_axis[1] = (short)(cmd[0]<<8|cmd[1]);                               //Add the low and high byte to the acc_x variable.
    acc_axis[2] = (short)(cmd[2]<<8|cmd[3]);                               //Add the low and high byte to the acc_y variable.
    acc_axis[3] = (short)(cmd[4]<<8|cmd[5]);                               //Add the low and high byte to the acc_z variable.
    temperature = (short)(cmd[6]<<8|cmd[7]);                               //Add the low and high byte to the temperature variable.
    gyro_axis[1] = (short)(cmd[8]<<8|cmd[9]);                              //Read high and low part of the angular data. x
    gyro_axis[2] = (short)(cmd[10]<<8|cmd[11]);                              //Read high and low part of the angular data. y
    gyro_axis[3] = (short)(cmd[12]<<8|cmd[13]);                              //Read high and low part of the angular data. z
    memset(cmd,0,sizeof(cmd));
  //}
  //pc.printf("Acc Axis: %d,", gyro_axis[2]);
  //pc.printf("%d\r\n", gyro_axis[2]);
  //pc.printf("%d\r\n", acc_axis[3]);
  //wait(.4);
  if(cal_int == 2000){
    gyro_axis[1] -= gyro_axis_cal[1];                                       //Only compensate after the calibration.
    gyro_axis[2] -= gyro_axis_cal[2];                                       //Only compensate after the calibration.
    gyro_axis[3] -= gyro_axis_cal[3];                                       //Only compensate after the calibration.
  }
  //pc.printf("%d\r\n", gyro_axis[2]);
  //NEED TO COME BACK HERE AND CHECK TO SEE IF THE GYRO VALUES NEED TO BE REVERSED********************************************************************************************************

   gyro_roll = gyro_axis[2];                      //Set gyro_roll to the correct axis that was stored in the EEPROM.
   //if(eeprom_data[28] & 0b10000000)gyro_roll *= -1;                          //Invert gyro_roll if the MSB of EEPROM bit 28 is set.
   gyro_pitch = gyro_axis[1];                     //Set gyro_pitch to the correct axis that was stored in the EEPROM.
   //if(eeprom_data[29] & 0b10000000)gyro_pitch *= -1;                         //Invert gyro_pitch if the MSB of EEPROM bit 29 is set.
   gyro_yaw = gyro_axis[3] * -1;                       //Set gyro_yaw to the correct axis that was stored in the EEPROM.
   //if(eeprom_data[30] & 0b10000000)gyro_yaw *= -1;                           //Invert gyro_yaw if the MSB of EEPROM bit 30 is set.

   acc_x = acc_axis[1];                           //Set acc_x to the correct axis that was stored in the EEPROM.
   //if(eeprom_data[29] & 0b10000000)acc_x *= -1;                              //Invert acc_x if the MSB of EEPROM bit 29 is set.
   acc_y = acc_axis[2];                           //Set acc_y to the correct axis that was stored in the EEPROM.
   //if(eeprom_data[28] & 0b10000000)acc_y *= -1;                              //Invert acc_y if the MSB of EEPROM bit 28 is set.
   acc_z = acc_axis[3] * -1;                           //Set acc_z to the correct axis that was stored in the EEPROM.
   //if(eeprom_data[30] & 0b10000000)acc_z *= -1;                              //Invert acc_z if the MSB of EEPROM bit 30 is set.
   //pc.printf("%d\r\n", gyro_roll);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Subroutine for calculating pid outputs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The PID controllers are explained in part 5 of the YMFC-3D video session:
//https://youtu.be/JBvnB0279-Q 
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

//This part converts the actual receiver signals to a standardized 1000 – 1500 – 2000 microsecond value.
//The stored data in the EEPROM is used.
// int convert_receiver_channel(byte function){
//   byte channel, reverse;                                                       //First we declare some local variables
//   int low, center, high, actual;
//   int difference;

//   channel = eeprom_data[function + 23] & 0b00000111;                           //What channel corresponds with the specific function
//   if(eeprom_data[function + 23] & 0b10000000)reverse = 1;                      //Reverse channel when most significant bit is set
//   else reverse = 0;                                                            //If the most significant is not set there is no reverse

//   actual = receiver_input[channel];                                            //Read the actual receiver value for the corresponding function
//   low = (eeprom_data[channel * 2 + 15] << 8) | eeprom_data[channel * 2 + 14];  //Store the low value for the specific receiver input channel
//   center = (eeprom_data[channel * 2 - 1] << 8) | eeprom_data[channel * 2 - 2]; //Store the center value for the specific receiver input channel
//   high = (eeprom_data[channel * 2 + 7] << 8) | eeprom_data[channel * 2 + 6];   //Store the high value for the specific receiver input channel

//   if(actual < center){                                                         //The actual receiver value is lower than the center value
//     if(actual < low)actual = low;                                              //Limit the lowest value to the value that was detected during setup
//     difference = ((long)(center - actual) * (long)500) / (center - low);       //Calculate and scale the actual value to a 1000 - 2000us value
//     if(reverse == 1)return 1500 + difference;                                  //If the channel is reversed
//     else return 1500 - difference;                                             //If the channel is not reversed
//   }
//   else if(actual > center){                                                                        //The actual receiver value is higher than the center value
//     if(actual > high)actual = high;                                            //Limit the lowest value to the value that was detected during setup
//     difference = ((long)(actual - center) * (long)500) / (high - center);      //Calculate and scale the actual value to a 1000 - 2000us value
//     if(reverse == 1)return 1500 - difference;                                  //If the channel is reversed
//     else return 1500 + difference;                                             //If the channel is not reversed
//   }
//   else return 1500;
// }

 void set_gyro_registers(){
    pc.printf("Set Gyro Registers\r\n");
//   //Setup the MPU-6050
//   if(eeprom_data[31] == 1){
    // Wire.beginTransmission(gyro_address);                                      //Start communication with the address found during search.
    // Wire.write(0x6B);                                                          //We want to write to the PWR_MGMT_1 register (6B hex)
    // Wire.write(0x00);                                                          //Set the register bits as 00000000 to activate the gyro
    // Wire.endTransmission();                                                    //End the transmission with the gyro.
    char cmd[2];
    cmd[0] = 0x6B;
    //i2c.write(gyro_address, cmd, 1, true);
    cmd[1] = 0x00;
    i2c.write(gyro_address, cmd, 2);
    memset(cmd,0,sizeof(cmd));
    cmd[0] = 0x6B;
    i2c.write(gyro_address, cmd, 1);
    i2c.read(gyro_address, cmd, 1);
    pc.printf("PWR_MGMT_1: %d\r\n", cmd[0]);
    // Wire.beginTransmission(gyro_address);                                      //Start communication with the address found during search.
    // Wire.write(0x1B);                                                          //We want to write to the GYRO_CONFIG register (1B hex)
    // Wire.write(0x08);                                                          //Set the register bits as 00001000 (500dps full scale)
    // Wire.endTransmission();                                                    //End the transmission with the gyro
    //char pwrRegister[1] = {0x6B};
    cmd[0] = 0x1B;
    //i2c.write(gyro_address, cmd, 1, true);
    cmd[1] = 0x08;
    i2c.write(gyro_address, cmd, 2);
    i2c.write(gyro_address, cmd, 1);
    i2c.read(gyro_address, cmd, 1);
    pc.printf("GYRO_CONFIG: %d\r\n", cmd[0]);
    // Wire.beginTransmission(gyro_address);                                      //Start communication with the address found during search.
    // Wire.write(0x1C);                                                          //We want to write to the ACCEL_CONFIG register (1A hex)
    // Wire.write(0x10);                                                          //Set the register bits as 00010000 (+/- 8g full scale range)
    // Wire.endTransmission();                                                    //End the transmission with the gyro
    cmd[0] = 0x1C;
    //i2c.write(gyro_address, cmd, 1, true);
    cmd[1] = 0x10;    
    i2c.write(gyro_address, cmd, 2);
    i2c.write(gyro_address, cmd, 1);
    i2c.read(gyro_address, cmd, 1);
    pc.printf("ACCEL_CONFIG: %d\r\n", cmd[0]);

    // //Let's perform a random register check to see if the values are written correct
    // Wire.beginTransmission(gyro_address);                                      //Start communication with the address found during search
    // Wire.write(0x1B);                                                          //Start reading @ register 0x1B
    // Wire.endTransmission();                                                    //End the transmission
    // Wire.requestFrom(gyro_address, 1);                                         //Request 1 bytes from the gyro
    // while(Wire.available() < 1);                                               //Wait until the 6 bytes are received
    // if(Wire.read() != 0x08){                                                   //Check if the value is 0x08
    //   //digitalWrite(12,HIGH);                                                   //Turn on the warning led
    //   while(1)delay(10);                                                       //Stay in this loop for ever
    // }

    // Wire.beginTransmission(gyro_address);                                      //Start communication with the address found during search
    // Wire.write(0x1A);                                                          //We want to write to the CONFIG register (1A hex)
    // Wire.write(0x03);                                                          //Set the register bits as 00000011 (Set Digital Low Pass Filter to ~43Hz)
    // Wire.endTransmission();                                                    //End the transmission with the gyro    
    cmd[0] = 0x1A;    
    //i2c.write(gyro_address, cmd, 1, true);
    cmd[1] = 0x03;    
    i2c.write(gyro_address, cmd, 2);
    i2c.write(gyro_address, cmd, 1);
    i2c.read(gyro_address, cmd, 1);
    pc.printf("CONFIG: %d\r\n", cmd[0]);

  //}  
 }

int main() {
    //Configure communications
    pc.printf("Program Start\r\n");
    onTime.start();
    i2c.frequency(400000);
    pc.baud(9600);
    device.baud(9600);
    device.attach(&rxInterrupt);

    //Copy the EEPROM data for fast access data.
    //for(start = 0; start <= 35; start++)eeprom_data[start] = EEPROM.read(start);
    start = 0;                                                                //Set start back to zero.
    gyro_address = 0x68<<1;                                           //Store the gyro address in the variable.
    
    // while(1) {
    //   __disable_irq();
    //   pc.printf("Throttle: %d\r\n", receiver_input_throttle);
    //   // pc.printf("Roll: %d\r\n", receiver_input_roll);
    //   // pc.printf("Pitch: %d\r\n", receiver_input_pitch);
    //   // pc.printf("Yaw: %d\r\n", receiver_input_yaw);
    //   // //wait(.0005);
    //   __enable_irq();
    //   wait(.0001);
    // }

    //Serial Control Throttle test
    // motors_standby();
    // wait(2);

    // while (1) {
    //   float reps = 500;
      
    //   float percent = ((float)receiver_input_throttle)/reps;
    //   motor1 = percent;
    //   motor2 = percent;
    //   motor3 = percent;
    //   motor4 = percent;
    //   //pc.printf("Percent: %f\r\n", percent);
    //   //wait(.0002);
    // }

    /* //Motor Test
    motor1 = 0.0;
    motor2 = 0.0;
    motor3 = 0.0;
    motor4 = 0.0;
    wait(2);
    pc.printf("Starting motor\r\n");
    motor1 = 0.5;
    motor2 = 0.5;
    motor3 = 0.5;
    motor4 = 0.5;
    wait(3);
    pc.printf("Stopping motor\r\n");
    motor1 = 0.0;
    motor2 = 0.0;
    motor3 = 0.0;
    motor4 = 0.0;*/

  
    //Wire.begin();                                                             //Start the I2C as master.
  
    //TWBR = 12;                                                                //Set the I2C clock speed to 400kHz.
  
    //Arduino (Atmega) pins default to inputs, so they don't need to be explicitly declared as inputs.
    //DDRD |= B11110000;                                                        //Configure digital port 4, 5, 6, and 7 as output.
    //pinMode(7,OUTPUT);
    //pinMode(13,INPUT);
    //Use the led on the Arduino for startup indication.
    //digitalWrite(12,HIGH);                                                    //Turn on the warning led.
    led = 1;
    
    //Check the EEPROM signature to make sure that the setup program is executed.
    //while(eeprom_data[33] != 'J' || eeprom_data[34] != 'M' || eeprom_data[35] != 'B')delay(10);
  
    //The flight controller needs the MPU-6050 with gyro and accelerometer
    //If setup is completed without MPU-6050 stop the flight controller program  
    //if(eeprom_data[31] == 2 || eeprom_data[31] == 3)delay(10);
  
    set_gyro_registers();                                                     //Set the specific gyro registers.
    pc.printf("Done registers\r\n");
    for (cal_int = 0; cal_int < 1250 ; cal_int ++){                           //Wait 5 seconds before continuing.
      motors_on();                                                     //Set digital port 4, 5, 6 and 7 high.
      wait(.001);                                                       //Wait 1000us.
      motors_off();                                                     //Set digital port 4, 5, 6 and 7 low.
      wait(.003);                                                //Wait 3000us.
    }

    // motors_standby();
    // wait(5);
    
    //Let's take multiple gyro data samples so we can determine the average gyro offset (calibration).
    for (cal_int = 0; cal_int < 2000 ; cal_int ++){                           //Take 2000 readings for calibration.
    //   if(cal_int % 15 == 0)digitalWrite(12, !digitalRead(12));                //Change the led status to indicate calibration.
     gyro_signalen();                                                        //Read the gyro output.
     gyro_axis_cal[1] += gyro_axis[1];                                       //Add roll value to gyro_roll_cal.
     gyro_axis_cal[2] += gyro_axis[2];                                       //Add pitch value to gyro_pitch_cal.
     gyro_axis_cal[3] += gyro_axis[3];                                       //Add yaw value to gyro_yaw_cal.
    //   //We don't want the esc's to be beeping annoyingly. So let's give them a 1000us puls while calibrating the gyro.
     //motors_standby();
     motors_on();                                                     //Set digital port 4, 5, 6 and 7 high.
     wait(.001);                                                       //Wait 1000us.
     motors_off();                                                     //Set digital port 4, 5, 6 and 7 low.
     wait(.003);                                                              //Wait 3 milliseconds before the next loop.
      //pc.printf("%d\r\n", cal_int);
    }
    //Serial.println("Gyro calibrated");
    //Now that we have 2000 measures, we need to divide by 2000 to get the average gyro offset.
    gyro_axis_cal[1] /= 2000;                                                 //Divide the roll total by 2000.
    gyro_axis_cal[2] /= 2000;                                                 //Divide the pitch total by 2000.
    gyro_axis_cal[3] /= 2000;                                                 //Divide the yaw total by 2000.
    /*
    PCICR |= (1 << PCIE0);                                                    // set PCIE0 to enable PCMSK0 scan
    PCMSK0 |= (1 << PCINT2);                                                  // set PCINT2 (digital input 10) to trigger an interrupt on state change
    PCMSK0 |= (1 << PCINT3);                                                  // set PCINT3 (digital input 11)to trigger an interrupt on state change
    PCMSK0 |= (1 << PCINT4);                                                  // set PCINT4 (digital input 12)to trigger an interrupt on state change
    PCMSK0 |= (1 << PCINT5);                                                  // set PCINT5 (digital input 13)to trigger an interrupt on state change
    */
    //Wait until the receiver is active and the throttle is set to the lower position.
    pc.printf("Waiting for arming...\r\n");
    
    while(receiver_input_throttle < 990 || receiver_input_throttle > 1020 || receiver_input_yaw < 1400){
      //receiver_input_throttle = convert_receiver_channel(3);                 //Convert the actual receiver signals for throttle to the standard 1000 - 2000us
      //receiver_input_yaw = convert_receiver_channel(4);                 //Convert the actual receiver signals for yaw to the standard 1000 - 2000us
      //Serial.println(receiver_input_throttle);
      //Serial.println(receiver_input_yaw);
      start ++;                                                               //While waiting increment start whith every loop.
      //We don't want the esc's to be beeping annoyingly. So let's give them a 1000us puls while waiting for the receiver inputs.
      motors_on();                                                     //Set digital port 4, 5, 6 and 7 high.
      wait(.001);                                                       //Wait 1000us.
      motors_off();                                                     //Set digital port 4, 5, 6 and 7 low.
      wait(.003);                                                              //Wait 3 milliseconds before the next loop.
      //motors_standby();
      if(start == 125){                                                       //Every 125 loops (500ms).
        //digitalWrite(12, !digitalRead(12));                                   //Change the led status.
        led = !led;
        start = 0;                                                            //Start again at 0.
      }
    }
    start = 0;                                                                //Set start back to 0.
    pc.printf("Ready\r\n");
    //Load the battery voltage to the battery_voltage variable.
    //65 is the voltage compensation for the diode.
    //12.6V equals ~5V @ Analog 0.
    //12.6V equals 1023 analogRead(0).
    //1260 / 1023 = 1.2317.
    //The variable battery_voltage holds 1050 if the battery voltage is 10.5V.
    //battery_voltage = (analogRead(0) + 65) * 1.2317;
    int battery_voltage = 1260;
    
    //loop_timer = micros();                                                    //Set the timer for the next loop.
    
    loop_timer = onTime.read_us(); 
    //When everything is done, turn off the led.
    //digitalWrite(12,LOW);                                                     //Turn off the warning led.

    while(1) {
        //void loop(){
            //Serial.println("Main program start");
           //pc.printf("%d\r\n", receiver_input_yaw);
           if(start == 2){
            //pc.printf("%f\r\n", angle_roll);
           //  Serial.print(gyro_roll,0);
           //  Serial.print(";");
           //  Serial.print(gyro_pitch,0);
           //  Serial.print(";");
           //  Serial.println(gyro_yaw,0);
             }
           
           //65.5 = 1 deg/sec (check the datasheet of the MPU-6050 for more information).
           gyro_roll_input = (gyro_roll_input * 0.7) + ((gyro_roll / 65.5) * 0.3);   //Gyro pid input is deg/sec.
           gyro_pitch_input = (gyro_pitch_input * 0.7) + ((gyro_pitch / 65.5) * 0.3);//Gyro pid input is deg/sec.
           gyro_yaw_input = (gyro_yaw_input * 0.7) + ((gyro_yaw / 65.5) * 0.3);      //Gyro pid input is deg/sec.
         
         
           ////////////////////////////////////////////////////////////////////////////////////////////////////
           //This is the added IMU code from the videos:
           //https://youtu.be/4BoIE8YQwM8
           //https://youtu.be/j-kE0AMEWy4
           ////////////////////////////////////////////////////////////////////////////////////////////////////
           
           //Gyro angle calculations
           //0.0000611 = 1 / (250Hz / 65.5)
           angle_pitch += gyro_pitch * 0.0000611;                                    //Calculate the traveled pitch angle and add this to the angle_pitch variable.
           angle_roll += gyro_roll * 0.0000611;                                      //Calculate the traveled roll angle and add this to the angle_roll variable.
         
           //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
           angle_pitch -= angle_roll * sin(gyro_yaw * 0.000001066);                  //If the IMU has yawed transfer the roll angle to the pitch angle.
           angle_roll += angle_pitch * sin(gyro_yaw * 0.000001066);                  //If the IMU has yawed transfer the pitch angle to the roll angle.
           //pc.printf("%f\r\n", angle_roll);
           //Accelerometer angle calculations
           acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));       //Calculate the total accelerometer vector.
           
           if(abs(acc_y) < acc_total_vector){                                        //Prevent the asin function to produce a NaN
             angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;          //Calculate the pitch angle.
           }
           if(abs(acc_x) < acc_total_vector){                                        //Prevent the asin function to produce a NaN
             angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;          //Calculate the roll angle.
           }
           
           //Place the MPU-6050 spirit level and note the values in the following two lines for calibration.
           angle_pitch_acc -= 0.0;                                                   //Accelerometer calibration value for pitch.
           angle_roll_acc -= 0.0;                                                    //Accelerometer calibration value for roll.
           
           angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;            //Correct the drift of the gyro pitch angle with the accelerometer pitch angle.
           angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;               //Correct the drift of the gyro roll angle with the accelerometer roll angle.
           
           pitch_level_adjust = angle_pitch * 12;                                    //Calculate the pitch angle correction
           roll_level_adjust = angle_roll * 12;                                      //Calculate the roll angle correction
         
           if(!auto_level){                                                          //If the quadcopter is not in auto-level mode
             pitch_level_adjust = 0;                                                 //Set the pitch angle correction to zero.
             roll_level_adjust = 0;                                                  //Set the roll angle correcion to zero.
           }
           //pc.printf("%d\r\n", receiver_input_yaw);
         
           //For starting the motors: throttle low and yaw left (step 1).
           if(receiver_input_throttle < 1050 && receiver_input_yaw < 1050 && receiver_input_yaw > 990) {
             start = 1;
             pc.printf("Armed\r\n");
           }
           //When yaw stick is back in the center position start the motors (step 2).
           if(start == 1 && receiver_input_throttle < 1050 && receiver_input_yaw > 1450){
             start = 2;
         
             angle_pitch = angle_pitch_acc;                                          //Set the gyro pitch angle equal to the accelerometer pitch angle when the quadcopter is started.
             angle_roll = angle_roll_acc;                                            //Set the gyro roll angle equal to the accelerometer roll angle when the quadcopter is started.
             gyro_angles_set = true;                                                 //Set the IMU started flag.
             
             //Reset the PID controllers for a bumpless start.
             pid_i_mem_roll = 0;
             pid_last_roll_d_error = 0;
             pid_i_mem_pitch = 0;
             pid_last_pitch_d_error = 0;
             pid_i_mem_yaw = 0;
             pid_last_yaw_d_error = 0;
           }
           //Stopping the motors: throttle low and yaw right.
           if(start == 2 && receiver_input_throttle < 1050 && receiver_input_yaw > 1950)start = 0;
           //pc.printf("%f\r\n", angle_roll);
           //The PID set point in degrees per second is determined by the roll receiver input.
           //In the case of dividing by 3 the max roll rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
           pid_roll_setpoint = 0;
           //We need a little dead band of 16us for better results.
           if(receiver_input_roll > 1508)pid_roll_setpoint = receiver_input_roll - 1508;
           else if(receiver_input_roll < 1492)pid_roll_setpoint = receiver_input_roll - 1492;
         
           pid_roll_setpoint -= roll_level_adjust;                                   //Subtract the angle correction from the standardized receiver roll input value.
           pid_roll_setpoint /= 3.0;                                                 //Divide the setpoint for the PID roll controller by 3 to get angles in degrees.
         
         
           //The PID set point in degrees per second is determined by the pitch receiver input.
           //In the case of dividing by 3 the max pitch rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
           pid_pitch_setpoint = 0;
           //We need a little dead band of 16us for better results.
           if(receiver_input_pitch > 1508)pid_pitch_setpoint = receiver_input_pitch - 1508;
           else if(receiver_input_pitch < 1492)pid_pitch_setpoint = receiver_input_pitch - 1492;
         
           pid_pitch_setpoint -= pitch_level_adjust;                                  //Subtract the angle correction from the standardized receiver pitch input value.
           pid_pitch_setpoint /= 3.0;                                                 //Divide the setpoint for the PID pitch controller by 3 to get angles in degrees.
         
           //The PID set point in degrees per second is determined by the yaw receiver input.
           //In the case of deviding by 3 the max yaw rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
           pid_yaw_setpoint = 0;
           //We need a little dead band of 16us for better results.
           if(receiver_input_throttle > 1050){ //Do not yaw when turning off the motors.
             if(receiver_input_yaw > 1508)pid_yaw_setpoint = (receiver_input_yaw - 1508)/3.0;
             else if(receiver_input_yaw < 1492)pid_yaw_setpoint = (receiver_input_yaw - 1492)/3.0;
           }
           
           calculate_pid();                                                            //PID inputs are known. So we can calculate the pid output.
         
           //The battery voltage is needed for compensation.
           //A complementary filter is used to reduce noise.
           //0.09853 = 0.08 * 1.2317.
           //battery_voltage = battery_voltage * 0.92 + (analogRead(0) + 65) * 0.09853;
           battery_voltage = 1260;
           
           //Turn on the led if battery voltage is to low.
           //if(battery_voltage < 1000 && battery_voltage > 600)digitalWrite(12, HIGH);
         
         
           throttle = receiver_input_throttle;                                      //We need the throttle signal as a base signal.
          //  pc.printf("start:%d\r\n", start);
          //  pc.printf("throttle:%d\r\n", throttle);
          //  pc.printf("roll:%d\r\n", receiver_input_roll);
          //  pc.printf("pitch:%d\r\n", receiver_input_pitch);
          //  pc.printf("yaw:%d\r\n", receiver_input_yaw);
          //  wait(1);


           if (start == 2){                                                          //The motors are started.
             //pc.printf("hi %d\r\n", throttle);
             if (throttle > 1800) throttle = 1800;                                   //We need some room to keep full control at full throttle.
             esc_1 = throttle - pid_output_pitch + pid_output_roll - pid_output_yaw; //Calculate the pulse for esc 1 (front-right - CCW)
             esc_2 = throttle + pid_output_pitch + pid_output_roll + pid_output_yaw; //Calculate the pulse for esc 2 (rear-right - CW)
             esc_3 = throttle + pid_output_pitch - pid_output_roll - pid_output_yaw; //Calculate the pulse for esc 3 (rear-left - CCW)
             esc_4 = throttle - pid_output_pitch - pid_output_roll + pid_output_yaw; //Calculate the pulse for esc 4 (front-left - CW)
             //pc.printf("%d\r\n", esc_1);
             if (battery_voltage < 1240 && battery_voltage > 800){                   //Is the battery connected?
               esc_1 += esc_1 * ((1240 - battery_voltage)/(float)3500);              //Compensate the esc-1 pulse for voltage drop.
               esc_2 += esc_2 * ((1240 - battery_voltage)/(float)3500);              //Compensate the esc-2 pulse for voltage drop.
               esc_3 += esc_3 * ((1240 - battery_voltage)/(float)3500);              //Compensate the esc-3 pulse for voltage drop.
               esc_4 += esc_4 * ((1240 - battery_voltage)/(float)3500);              //Compensate the esc-4 pulse for voltage drop.
             } 
         
             if (esc_1 < 1100) esc_1 = 1100;                                         //Keep the motors running.
             if (esc_2 < 1100) esc_2 = 1100;                                         //Keep the motors running.
             if (esc_3 < 1100) esc_3 = 1100;                                         //Keep the motors running.
             if (esc_4 < 1100) esc_4 = 1100;                                         //Keep the motors running.
         
             if(esc_1 > 2000)esc_1 = 2000;                                           //Limit the esc-1 pulse to 2000us.
             if(esc_2 > 2000)esc_2 = 2000;                                           //Limit the esc-2 pulse to 2000us.
             if(esc_3 > 2000)esc_3 = 2000;                                           //Limit the esc-3 pulse to 2000us.
             if(esc_4 > 2000)esc_4 = 2000;                                           //Limit the esc-4 pulse to 2000us.  
           }
         
           else{
             esc_1 = 1000;                                                           //If start is not 2 keep a 1000us pulse for esc-1.
             esc_2 = 1000;                                                           //If start is not 2 keep a 1000us pulse for esc-2.
             esc_3 = 1000;                                                           //If start is not 2 keep a 1000us pulse for esc-3.
             esc_4 = 1000;                                                           //If start is not 2 keep a 1000us pulse for esc-4.
           }
         
           ////////////////////////////////////////////////////////////////////////////////////////////////////
           //Creating the pulses for the ESC's is explained in this video:
           //https://youtu.be/fqEkVcqxtU8
           ////////////////////////////////////////////////////////////////////////////////////////////////////
         
           //! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
           //Because of the angle calculation the loop time is getting very important. If the loop time is 
           //longer or shorter than 4000us the angle calculation is off. If you modify the code make sure 
           //that the loop time is still 4000us and no longer! More information can be found on 
           //the Q&A page: 
           //! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
             
           //if(micros() - loop_timer > 4050)digitalWrite(12, HIGH);                   //Turn on the LED if the loop time exceeds 4050us.
           
           //All the information for controlling the motor's is available.
           //The refresh rate is 250Hz. That means the esc's need there pulse every 4ms.
           
           //We wait until 4000us are passed.
           //int loopTime = onTime.read_us() - loop_timer;
           while(onTime.read_us() - loop_timer < 4000) {
              //do stuff thats not flight
              //pc.printf("%f  -  ", motor2.read());
              //pc.printf("%d\r\n", (int)angle_roll);
              
           }
           //pc.printf("%d\r\n", onTime.read_us() - loop_timer);
          //  if (loopTime > 4000) {
          //    pc.printf("Loop Overtime\r\n");
          //    wait(1);
          //  }
           //                     
           loop_timer = onTime.read_us();                                                   //Set the timer for the next loop.
           
          //  motor1 = ((float)esc_1-1000)/1000;
          //  motor2 = ((float)esc_2-1000)/1000;
          //  motor3 = ((float)esc_3-1000)/1000;
          //  motor4 = ((float)esc_4-1000)/1000;

          //  PORTD |= B11110000;                                                       //Set digital outputs 4,5,6 and 7 high.
           //__disable_irq();
           motors_on();
           //
           timer_channel_1 = esc_1 + loop_timer;                                     //Calculate the time of the falling edge of the esc-1 pulse.
           timer_channel_2 = esc_2 + loop_timer;                                     //Calculate the time of the falling edge of the esc-2 pulse.
           timer_channel_3 = esc_3 + loop_timer;                                     //Calculate the time of the falling edge of the esc-3 pulse.
           timer_channel_4 = esc_4 + loop_timer;                                     //Calculate the time of the falling edge of the esc-4 pulse.
           
           //There is always 1000us of spare time. So let's do something useful that is very time consuming.
           //Get the current gyro and receiver data and scale it to degrees per second for the pid calculations.
           gyro_signalen();
             
           while(motor1 == 1 || motor2 == 1 || motor3 == 1 || motor4 == 1) {                                                       //Stay in this loop until output 4,5,6 and 7 are low.
             esc_loop_timer = onTime.read_us();                                              //Read the current time.
             if(timer_channel_1 <= esc_loop_timer)motor1 = 0;                //Set digital output 7 to low if the time is expired.
             if(timer_channel_2 <= esc_loop_timer)motor2 = 0;                //Set digital output 6 to low if the time is expired.
             if(timer_channel_3 <= esc_loop_timer)motor3 = 0;                //Set digital output 5 to low if the time is expired.
             if(timer_channel_4 <= esc_loop_timer)motor4 = 0;                //Set digital output 4 to low if the time is expired.
           }
           //__enable_irq();
         }
    //}
}


  

  