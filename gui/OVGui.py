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
                if 'ST' in p.description]
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

    # charts to update
    update_plot = pyqtSignal(float, float, str)

    @pyqtSlot()
    def __init__(self):
        super(CommTask, self).__init__()
        self.connected = False
        self.debugTest = False
        self.mcu_serial = None

    def run(self):
        while True:
            if self.connected:
                # Read Line, decode and emit
                line = self.mcu_serial.readline().decode('utf-8')
                print(line)
                self.process_msg(line)
                self.isready_status.emit(True)
                # line = "New Msg"
                # time.sleep(0.05)

            elif self.debugTest:
                line = "No Connection"
                self.newline_signal.emit(line)
                self.isready_status.emit(False)
                time.sleep(0.5)

    def process_msg(self, json_line):
        json_doc = json.loads(json_line)
        print(json_doc["T"])
        if(json_doc["S"] == "SystemStatus"):
            # print(type(json_doc['P']))
            self.update_plot.emit(json_doc["T"]/1000,json_doc['P'][0],"Pressure")
        # self.newline_signal.emit(json_doc)
        # self.update_plot.emit(json_doc["T"]/1000,json_doc["FlowSp"],"Flow1")
        # self.update_plot.emit(json_doc["T"]/1000,json_doc["FlowSp"],"Flow2")
        # self.update_plot.emit(json_doc["T"]/1000,json_doc["RateSp"],"Volume")
        # self.update_plot.emit(json_doc["T"]/1000,json_doc["IESp"],"Pressure")
        # self.update_plot.emit(json_doc["T"]/1000,json_doc["FlowSp"],"PEEP")


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
        available_port = check_usb_available()
        print("Connecting Device, port: " + available_port)
        if available_port :  
            self.comm_task.mcu_serial = serial.Serial(available_port, 115200, timeout = 5)

            if self.comm_task.mcu_serial:
                
                self.comm_task.mcu_serial.flushInput()
                # self.comm_task.mcu_serial.flushOutput()
                time.sleep(0.1)
                self.comm_task.mcu_serial.read(5) # Read first line as well
                print("readbytes")
                self.comm_task.mcu_serial.readline() # Read first line as well
                self.comm_task.mcu_serial.readline() # Read first line as well
                self.comm_task.mcu_serial.readline() # Read first line as well
                self.comm_task.connected = True
                self.connection_status.emit(True)
            else :
                print("Error Connecting to " + available_port + " Port")

    def disconnect_device(self):   
        if self.comm_task.mcu_serial.isOpen():
            self.comm_task.connected = False 
            time.sleep(0.5)
            self.comm_task.mcu_serial.flushInput()
            self.comm_task.mcu_serial.flushOutput()
            self.comm_task.mcu_serial.close() 
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

    #Connect Plot Update Signals
    pygui.comm_task.update_plot.connect(rootObject.updatePlot) 

    if not engine.rootObjects():
        sys.exit(-1)

    sys.exit(app.exec_())