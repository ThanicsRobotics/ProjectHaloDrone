from smbus import SMBus

bus = SMBus(1)

adr = 0x33
setup = 0x82

bus.write_byte(adr, setup)