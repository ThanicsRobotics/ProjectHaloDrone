import serial

port = serial.Serial(port="/dev/ttyAMA0", baudrate=9600, timeout=1)
ack = 0
connected = True

try:
    while connected:
        bit_rec = port.read()
        print "Recieved:", bit_rec

except KeyboardInterrupt:
    port.close()
