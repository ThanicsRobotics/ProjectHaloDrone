import spidev
import time

spi = spidev.SpiDev()
spi.open(0, 0)

try:
    while True:
        # spi.writebytes([0x24])
        # spi.writebytes([0x00])
        # time.sleep(.003)
        # spi.writebytes([0x80])
        #resp = spi.xfer([0x00])
        start = [0x24, 0x00]
        data = [0x80, 0x00, 0x82, 0x00, 0x84, 0x00, 0x86, 0x00, 0x00]
        coefficients = [0x88, 0x00, 0x8A, 0x00, 0x8C, 0x00, 0x8E,
                        0x00, 0x90, 0x00, 0x92, 0x00, 0x94, 0x00, 0x96, 0x00, 0x00]
        resp = spi.xfer(coefficients)
        print(resp)
        time.sleep(0.5)
        spi.writebytes(start)
        time.sleep(0.003)
        resp2 = spi.xfer(data)
        print(resp2)
        time.sleep(2)
finally:
    spi.close()
