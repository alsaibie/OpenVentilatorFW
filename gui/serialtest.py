import serial, serial.tools.list_ports
import warnings

usb_com_port = None
usb_ports = None
usb_ports = [p.device for p in serial.tools.list_ports.comports()
                if 'ST' in p.description]
print(usb_ports)
if not usb_ports:
    warnings.warn("No Ports Found")
elif len(usb_ports) > 1:
    warnings.warn("More than one USB Port Found, please specify port name")
else:
    usb_com_port = usb_ports[0]

