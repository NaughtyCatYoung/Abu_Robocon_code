import serial

ser = serial.Serial(
    port = '/dev/ttyUSB0',
    baudrate = 115200,
    timeout = 10,
    xonxoff = False
)

val = ser.write('getData\n'.encode(encoding = 'ascii', errors = 'strict'))
print("Bytes written: ", val)