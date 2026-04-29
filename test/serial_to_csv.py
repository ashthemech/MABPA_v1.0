import serial
import csv

#open serial port, set baud rate, set a timeout
ser = serial.Serial('COM3', 115200, timeout=1)
with open('outputTest10Hz.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['Time (ms)', 'Sensor Value']) #write the header

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()  #read line from serial
            if line:
                data = line.split(',')  #split the data into two columns (time, sensor value)
                if len(data) == 2:  #ensure the line has two values
                    writer.writerow(data)  #write the data into CSV
