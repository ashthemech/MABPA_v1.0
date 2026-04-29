import pyqtgraph as pg
from PyQt5 import QtWidgets, QtCore
import serial
import time as pytime  #avoid conflict with "time" variable

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        #serial setup
        self.ser = serial.Serial("COM3", 115200)
        self.start_time = pytime.time()

        #data storage
        self.times = []
        self.voltages = []

        #setup plotting window
        self.plot_graph = pg.PlotWidget()
        self.setCentralWidget(self.plot_graph)
        self.plot_graph.setBackground("w")
        pen = pg.mkPen(color=(0, 0, 0), width=2)
        self.plot_graph.setTitle("Voltage vs Time of Bicep Muscle Sensor", color="black", size="20pt")
        styles = {"color": "black", "font-size": "18px"}
        self.plot_graph.setLabel("left", "Voltage [V]", **styles)
        self.plot_graph.setLabel("bottom", "Time (s)", **styles)
        self.plot_graph.showGrid(x=True, y=True)
        self.curve = self.plot_graph.plot([], [], pen=pen)

       # Timer to update the graph
        self.timer = QtCore.QTimer()
        self.timer.setInterval(1)  # Update every 1 ms
        self.timer.timeout.connect(self.update_plot)
        self.timer.start()

    def update_plot(self):
        while self.ser.in_waiting:
            try:
                line = self.ser.readline().decode('utf-8').strip()
                if line.startswith(">sEMG1:"):
                    voltage = float(line.split(":")[1])
                    elapsed = pytime.time() - self.start_time
                    self.times.append(elapsed)
                    self.voltages.append(voltage)

                    # Limit points to improve performance
                    if len(self.times) > 1000:
                        self.times = self.times[-1000:]
                        self.voltages = self.voltages[-1000:]

                    self.curve.setData(self.times, self.voltages)
            except Exception as e:
                print("Error reading data:", e)

app = QtWidgets.QApplication([])
main = MainWindow()
main.show()
app.exec()