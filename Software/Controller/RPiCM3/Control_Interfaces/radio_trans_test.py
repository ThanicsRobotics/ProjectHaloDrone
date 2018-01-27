import time
import serial

port = serial.Serial(port="/dev/ttyAMA0", baudrate=9600)
ack = 0
connected = True

while connected:
    port.write((str(ack)+"\r\n").encode())
    print ("Sent: "+str(ack))
    ack += 1
    time.sleep(1)
