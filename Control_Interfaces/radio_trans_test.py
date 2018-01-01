import time
import serial

port = serial.Serial(port="/dev/ttyS0", baudrate=9600)
ack = 0
connected = True

while connected:
    port.write(str(ack))
    print "Sent:", ack
    ack += 1
    time.sleep(1)
