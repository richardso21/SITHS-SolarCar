import serial
import serial.tools.list_ports

# ports = list(serial.tools.list_ports.comports())
# for p in ports:
#     print(p)
ser = serial.Serial('/dev/cu.usbserial-01E0569D', 9600)

print(ser.name)

# for i in range(10):
while True:
    line = ser.readline()
    print(line.decode('UTF-8').strip())