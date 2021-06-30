# readSerial.py reads the serial port to the RX arduino as it receives
# radio transmissions from TX (on car). It stores each instance of recorded
# voltage, current, and speed data onto a sqlite database for a local grafana
# instance to display on a dashboard.

import serial
import serial.tools.list_ports
import sqlite3

# establish serial connection
print("=== PORTS: ===")
ports = []
for n, i in enumerate(serial.tools.list_ports.comports()):
    print(f'{n} - {i}')
    ports.append(i.device)
print()
sel = int(input("Please select Arduino COM / port to listen (enter #): "))
ser = serial.Serial(ports[sel])

# setup connection w sqlite database
# con = sqlite3.connect("testData.db")
# db = con.cursor()
# db.execute('''CREATE TABLE IF NOT EXISTS testData (
#     timestamp INTEGER PRIMARY KEY,
#     speed REAL)''')
con = sqlite3.connect("testData.db")
db = con.cursor()
db.execute('''
CREATE TABLE IF NOT EXISTS carData (
    timestamp INTEGER PRIMARY KEY,
    speed INTEGER,
    battery_voltage REAL,
    aux_voltage REAL,
    motor_current REAL,
    array_current REAL,
    temp INTEGER)''')

try:
    while True:
        # read lora data from arduino
        dataBatch = ser.readline().decode('utf-8')
        # split each snapshot in n-second long batch
        dataRaw = dataBatch.strip()[:-1].split('|')

        # for all snapshots in batch
        for data in dataRaw:
            dataSp = data.split(';')
            print(dataSp)

            if len(dataSp) < 2:
                continue
            # insert into database
            # db.execute('INSERT INTO testData(timestamp, speed) VALUES (?, ?)',
            #            dataSp)
            db.execute('INSERT INTO carData VALUES (?, ?, ?, ?, ?, ?, ?)',
                        dataSp)
            con.commit()


except KeyboardInterrupt:  # excute before termination via Ctrl-C
    print("Closing connections...")
    ser.close()
    con.close()
