import serial
import csv

# Open serial port (adjust the port name accordingly, e.g., COM3 for Windows or /dev/ttyUSB0 for Linux/Mac)
ser = serial.Serial('COM3', 115200, timeout=1)  # Replace `COM3 with your serial port
with open('outputTest10Hz.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['Time (ms)', 'Sensor Value'])  # Write the header

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()  # Read line from serial
            if line:
                data = line.split(',')  # Split the data into two columns (time, sensor value)
                if len(data) == 2:  # Ensure the line has two values
                    writer.writerow(data)  # Write the data into CSV
