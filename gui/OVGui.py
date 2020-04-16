import os
import sys
import warnings

from PyQt5.QtCore import QObject, QUrl, QThread, pyqtSignal, QPointF, pyqtSlot, pyqtProperty
from PyQt5.QtWidgets import QApplication
from PyQt5.QtQml import QQmlApplicationEngine
from PyQt5.QtChart import QXYSeries
import serial
import serial.tools.list_ports
import random
import resources
import numpy as np
import json
import time
warnings.simplefilter('always', UserWarning)
def check_usb_available():

    usb_com_port = None
    usb_ports = None
    usb_ports = [p.device for p in serial.tools.list_ports.comports()
                if 'USB' in p.description]
    if not usb_ports:
        warnings.warn("No Ports Found")
    elif len(usb_ports) > 1:
        warnings.warn("More than one USB Port Found, please specify port name")
    else:
        usb_com_port = usb_ports[0]
    
    return usb_com_port

# @pyqtSlot()


class CommTask(QObject):
    def __init__(self, parent=None):
        QObject.__init__(self, parent)
    # Background thread communicate with mcu and relay messages
    commend_signal = pyqtSignal()  # Notify GUI that comm port has closed
    newline_signal = pyqtSignal(str)  # Send new line to GUI
    isready_status = pyqtSignal(bool)

    @pyqtSlot()
    def __init__(self):
        super(CommTask, self).__init__()
        self.connected = False
        self.debugTest = True
        self.mcu_serial = None

    def run(self):
        while True:
            if self.connected:
                # Read Line, decode and emit
                # line = ser.readline().decode('utf-8')
                # print(line)
                line = "New Msg"
                time.sleep(0.5)
                self.isready_status.emit(True)
                self.newline_signal.emit(line)
            elif self.debugTest:
                line = "No Connection"
                self.newline_signal.emit(line)
                self.isready_status.emit(False)
                time.sleep(0.5)

class PYGUI(QObject):
    connection_status = pyqtSignal(bool)

    def __init__(self, parent=None):
        QObject.__init__(self, parent)
        self.xflow = 0.0
        self.xvolume = 0.0
        self.xpressure = 0.0
        self.xpeep = 0.0
        self.comm_task = CommTask()
        self.thread = None
        self.thread = QThread()
        self.comm_task.moveToThread(self.thread)
        self.thread.started.connect(self.comm_task.run)
        self.thread.start()


    def connect_device(self):
        print("Connecting Device")
        available_port = check_usb_available()
        
        if available_port :  # TODO: replace with if serial connection is successful
            mcu_serial = serial.Serial(available_port, 115200)
            if mcu_serial:
                self.comm_task.connected = True
                self.connection_status.emit(True)
            else :
                print("Error Connecting to " + available_port + " Port")

    def disconnect_device(self):
        if True:  # TODO: replace with if serial disconnection is successful
            self.comm_task.connected = False
            self.connection_status.emit(False)

    @pyqtSlot(str, bool)
    def sendBinaryCommand(self, which, state):
        print(which + " " + str(state))

    @pyqtSlot(str, int)
    def sendIntegerCommand(self, which, val):
        print(which + " " + str(val))

    @pyqtSlot(str, float)
    def sendFloatCommand(self, which, val):
        print(which + " " + str(val))

    @pyqtSlot()
    def test(self):
        print('Hello')

    @pyqtSlot(QXYSeries, int)
    def update_pressuredata_series(self, series, len_data):
        self.xpressure += 1
        # Get new data pt
        y = np.sin(self.xpressure/5) + random.uniform(-.1, .1)
        point = QPointF(self.xpressure, y)
        if(len(series) > len_data + 5):
            series.remove(0)
        series.append(point)

    @pyqtSlot(QXYSeries, QXYSeries, int)
    def update_flowdata_series(self, series1, series2, len_data):
        self.xflow += 1
        # Get new data pt
        y1 = np.sin(self.xflow/5) + random.uniform(-1, 1)
        y2 = np.sin(self.xflow/5)
        point1 = QPointF(self.xflow, y1)
        point2 = QPointF(self.xflow, y2)
        if(len(series1) > len_data + 5):
            series1.remove(0)
            series2.remove(0)
        series1.append(point1)
        series2.append(point2)

    @pyqtSlot(QXYSeries, int)
    def update_volumedata_series(self, series, len_data):
        self.xvolume += 1
        # Get new data pt
        y = np.sin(self.xvolume/3) + random.uniform(-.5, .5)
        point = QPointF(self.xvolume, y)
        if(len(series) > len_data + 5):
            series.remove(0)
        series.append(point)

    @pyqtSlot(QXYSeries, int)
    def update_peepdata_series(self, series, len_data):
        self.xpeep += 1
        # Get new data pt
        y = np.sin(self.xpeep/1) + random.uniform(-.2, .2)
        point = QPointF(self.xpeep, y)
        if(len(series) > len_data + 5):
            series.remove(0)
        series.append(point)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    pygui = PYGUI()
    engine = QQmlApplicationEngine()
    os.environ['QT_QUICK_CONTROLS_STYLE'] = 'Material'

    context = engine.rootContext()

    context.setContextProperty("pygui", pygui)
    engine.load(QUrl('qrc:/qml/OVGui.qml'))

    # Connect Signals
    rootObject = engine.rootObjects()[0]
    rootObject.connectDevice.connect(pygui.connect_device)
    rootObject.disconnectDevice.connect(pygui.disconnect_device)

    pygui.connection_status.connect(rootObject.connectionStatus)
    pygui.comm_task.newline_signal.connect(rootObject.incomingLine)
    pygui.comm_task.isready_status.connect(rootObject.isReadyStatus)
    if not engine.rootObjects():
        sys.exit(-1)

    sys.exit(app.exec_())
