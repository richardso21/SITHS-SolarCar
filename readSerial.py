# readSerial.py reads the serial port to the RX arduino as it receives
# radio transmissions from TX (on car). It stores each instance of recorded
# voltage, current, and speed data onto a sqlite database for a local grafana
# instance to display on a dashboard.

import serial
import serial.tools.list_ports
import sqlite3
from datetime import datetime

# establish serial connection
print("=== PORTS: ===")
for i in serial.tools.list_ports.comports():
    print(i)
print()
ser = serial.Serial(input("Please print Arduino COM / port to listen: "))

# setup connection w sqlite database
con = sqlite3.connect("carData.db")
db = con.cursor()
db.execute('''
CREATE TABLE IF NOT EXISTS carData (
    timestamp INTEGER PRIMARY KEY,
    speed REAL,
    battery_voltage REAL,
    aux_voltage REAL,
    motor_current REAL,
    array_current REAL)''')

try:
    while True:
        # read lora data from arduino and get only the data via str split
        dataRaw = ser.readline()
        data = dataRaw.split(',')[2]

        # timestamp, speed, battery (V), aux (V), motor (I), array(I)
        dataSp = list(map(int, data.split(';')))
        ts = dataSp[0]
        # convert into unix timestamp (POSIX)
        dataSp[0] = datetime(2000 + ts[4:6], ts[2:4],
                            ts[0:2], ts[6:8], ts[8:10], ts[10:12]).timestamp()

        # insert into database after getting values
        db.execute('INSERT INTO carData VALUES (?, ?, ?, ?, ?, ?)',
                    dataSp)


except KeyboardInterrupt:  # excute before termination via Ctrl-C
    ser.close()
    con.close()
