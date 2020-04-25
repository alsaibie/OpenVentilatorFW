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
    usb_com_port = 'COM8'
    
    return usb_com_port

# @pyqtSlot()

class CommTask(QObject):
    def __init__(self, parent=None):
        QObject.__init__(self, parent)
        self.connected = False
        self.debugTest = False
        self.mcu_serial = None
    # Background thread communicate with mcu and relay messages
    commend_signal = pyqtSignal()  # Notify GUI that comm port has closed
    newline_signal = pyqtSignal(str)  # Send new line to GUI
    isready_status = pyqtSignal(bool)
    # charts to update
    update_plot = pyqtSignal(float, float, str)
    update_values = pyqtSignal(float, str)
    flush_plots = pyqtSignal()

    def run(self):
        while True:
            if self.connected:
                # Read Line, decode and emit
                line = []
                try:
                    line = self.mcu_serial.readline().decode('utf-8')
                except :
                    print("Error reading new serial line")
                if line:
                    # print(line)
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
        try:
            json_doc = json.loads(json_line)
            if(json_doc["S"] == "SystemStatus"):
                # self.newline_signal.emit(json_doc)
                t = json_doc["T"]/1000
                
                self.update_values.emit(t, "uptime")

                self.update_plot.emit(t,json_doc['P'][0],"Pressure")
                self.update_values.emit(json_doc['P'][0], "minPressure")
                self.update_values.emit(json_doc['P'][2], "maxPressure")

                self.update_plot.emit(t, json_doc['V'][0], "Volume" )
                self.update_values.emit(json_doc['V'][0], "volume")
                
            if(json_doc["S"] == "OpStatus"):
                self.update_values.emit(json_doc['OpMode'], "opMode")
                self.update_values.emit(json_doc['OpState'], "opState")

        except:
            print("Invalid JSON string")       
            # print(json_line)
        
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
                # self.comm_task.mcu_serial.read(5) # Read first line as well
                # print("readbytes")
                # self.comm_task.mcu_serial.readline() # Read first line as well
                # self.comm_task.mcu_serial.readline() # Read first line as well
                # self.comm_task.mcu_serial.readline() # Read first line as well
                self.comm_task.flush_plots.emit()
                time.sleep(0.2)
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
        if self.comm_task.connected:
            json_msg = {} 
            json_msg["bC"] = which
            json_msg["state"] = state
            line_msg = json.dumps(json_msg) + '\n'
            self.comm_task.mcu_serial.write(line_msg.encode('utf-8'))
            self.comm_task.mcu_serial.flushOutput()

    @pyqtSlot(str, bool)
    def sendModeCommand(self, which, mode):
        if self.comm_task.connected:
            json_msg = {} 
            json_msg["mC"] = which
            json_msg["mode"] = mode
            line_msg = json.dumps(json_msg) + '\n'
            self.comm_task.mcu_serial.write(line_msg.encode('utf-8'))
            self.comm_task.mcu_serial.flushOutput()

    @pyqtSlot(str, int)
    def sendIntegerCommand(self, which, val):
        if self.comm_task.connected:
            json_msg = {} 
            json_msg["iC"] = which
            json_msg["val"] = int(val)
            line_msg = json.dumps(json_msg) + '\n'
            self.comm_task.mcu_serial.write(line_msg.encode('utf-8'))
            self.comm_task.mcu_serial.flushOutput()

    @pyqtSlot(str, float)
    def sendFloatCommand(self, which, val):
        if self.comm_task.connected:
            json_msg = {} 
            json_msg["fC"] = which
            json_msg["val"] = val
            line_msg = json.dumps(json_msg) + '\n'
            self.comm_task.mcu_serial.write(line_msg.encode('utf-8'))
            self.comm_task.mcu_serial.flushOutput()
    
    @pyqtSlot(str, float)
    def sendParameterCommand(self, which, val):
        if self.comm_task.connected:
            json_msg = {} 
            json_msg["pC"] = which
            json_msg["val"] = val
            line_msg = json.dumps(json_msg) + '\n'
            self.comm_task.mcu_serial.write(line_msg.encode('utf-8'))
            self.comm_task.mcu_serial.flushOutput()

    # @pyqtSlot(QXYSeries, int)
    # def update_series(self, series, len_data):
    #     self.xpressure += 1
    #     # Get new data pt
    #     y = np.sin(self.xpressure/5) + random.uniform(-.1, .1)
    #     point = QPointF(self.xpressure, y)
    #     if(len(series) > len_data + 5):
    #         series.remove(0)
    #     series.append(point)

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
    pygui.comm_task.update_values.connect(rootObject.updateValuesRow)
    pygui.comm_task.flush_plots.connect(rootObject.flushPlots)

    if not engine.rootObjects():
        sys.exit(-1)

    sys.exit(app.exec_())