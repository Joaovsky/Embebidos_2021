# -*- coding: utf-8 -*-
from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import pyqtSignal, QThread
from PyQt5 import QtWidgets,uic
from PyQt5.QtGui import QPixmap
#from picamera import PiCamera
from serial.tools import list_ports
import serial
import time
import sys
import shutil
import threading

mode=0
ref=0
actual_lux=0
actual_hum=0
actual_temp=0
setpoint=0

#pix= QPixmap("../app_python/images/icons/temperature.png")

#usb_serial = serial.Serial('/dev/ttyACM0',115200)

class Ui(QtWidgets.QMainWindow):
    mode=0
    def __init__(self):
        self.pix= QPixmap("../app_python/images/icons/temperature.png")
        super(Ui,self).__init__()
        uic.loadUi('mainwindow.ui',self)
        self.more_btn.clicked.connect(self.btn_more_pressed)
        self.less_btn.clicked.connect(self.btn_less_pressed)
        self.auto_btn.clicked.connect(self.btn_auto_pressed)
        self.take_btn.clicked.connect(self.btn_take_pressed)
        self.ref_light_slider.valueChanged.connect(self.slider_ref_changed)
        self.show()
    def btn_more_pressed(self):
        global setpoint
        setpoint +=1
        if setpoint >= 27:
            setpoint =27
        self.setpoint_val.setNum(setpoint)
        usb_serial.write(str.encode('T'+str(setpoint)+'\r\n'))       
    def btn_less_pressed(self):
        global setpoint
        setpoint -=1
        if setpoint <= 15:
            setpoint =15
        self.setpoint_val.setNum(setpoint)
        usb_serial.write(str.encode('T'+str(setpoint)+'\r\n'))     
    def btn_auto_pressed(self):
        global mode
        if mode ==1:
            mode=0
            self.auto_btn.setStyleSheet("border:1px solid rgb(200,200,200)")
        else:
            mode=1
            self.auto_btn.setStyleSheet("border:2px solid rgb(25,25,25)")
        usb_serial.write(str.encode('LM'+str(mode)+'\r\n'))
    def btn_take_pressed(self):
        self.Sign_show.setPixmap(self.pix)       
    def slider_ref_changed(self,value):
        global ref
        ref=value
        usb_serial.write(str.encode('L'+str(ref)+'\r\n'))
        
        
class MyTask1(QThread):
    #capture image 0.1 s (deamon)
    done_signal = pyqtSignal(str)
    def __init__(self):
        QThread.__init__(self)
    def run(self):
        Cam = PiCamera()
        while True:
            QThread.msleep(10)
            Cam.capture('test.jpg')
            self.done_signal.emit('Foto')

class MyTask2(QThread):
    #read imputs from stm
    done_signal = pyqtSignal(str)
    def __init__(self):
        QThread.__init__(self)
    def run(self):
        # Do some work here
        global ref
        global setpoint
        global actual_lux
        global actual_hum
        global actual_temp
        while True:
            while usb_serial.inWaiting()>0:
                readed=usb_serial.read(usb_serial.inWaiting())
                if b'RT' in readed:
                    if readed[2] >47 and readed[2] <58 and readed[3] >47 and readed[3] <58:
                        setpoint = chr(readed[2])+chr(readed[3])
                        win.setpoint_val.setNum(int(setpoint))
                elif b'T' in readed:
                    if readed[1] >47 and readed[1] <58 and readed[2] >47 and readed[2] <58:
                        actual_temp = chr(readed[2])+chr(readed[3])
                        win.actual_val.setNum(int(actual_temp))
                if b'RL' in readed:
                    if len(readed)>5: 
                        ref = 100
                    elif len(readed)==5:
                        ref = chr(readed[2])+chr(readed[3])
                    elif len(readed)<5:
                        ref = chr(readed[2])
                    win.ref_light_slider.setValue(int(ref))
                elif b'ML' in readed:
                    mode = chr(readed[2])
                    if int(mode) ==1:
                        win.auto_btn.setStyleSheet("border:2px solid rgb(25,25,25)")
                    else:
                        win.auto_btn.setStyleSheet("border:1px solid rgb(200,200,200)")
                elif b'L' in readed:
                    if len(readed)>4: 
                        actual_lux = 100
                    elif len(readed)==4:
                        actual_lux = chr(readed[1])+chr(readed[2])
                    elif len(readed)<4:
                        actual_lux = chr(readed[2])
                        
                elif b'H' in readed:
                    if len(readed)>4: 
                        actual_hum = 100
                    elif len(readed)==4:
                        actual_hum = chr(readed[1])+chr(readed[2])
                    elif len(readed)<4:
                        actual_hum = chr(readed[1])
                        win.humidity_val.setNum(int(actual_hum))
            QThread.msleep(10)
            #self.done_signal.emit('Task2 ends')
        
class MyTask3(QThread):
    #deteçao de sinal/obstaculo
    done_signal = pyqtSignal(str)
    def __init__(self):
        QThread.__init__(self)
    def run(self):
        # Do some work here
        
        if True:
            self.done_signal.emit('Signal')
        if True:
            self.done_signal.emit('Obstacle')
        else:
            QThread.msleep(10)


def process_done_signal(result):
    pix= QPixmap("../app_python/test.jpg")
    if result == 'Foto': #se já tirou foto->avaliar se é sinal/ obstaculo
        #task2.done_signal.connect(process_done_signal)
        #task2.start()
        shutil.copy2('../app_python/test.jpg','../app_python/foto.jpg')
        win.Sign_show.setPixmap(pix)
        #print(result)
        


if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = Ui()

    task1 = MyTask1()
    task2 = MyTask2()
    task3 = MyTask3()
    #print(list_ports.comports())

    task1.done_signal.connect(process_done_signal)
    task1.start()
    #task2.done_signal.connect(process_done_signal)
    task2.start()
    #task3.done_signal.connect(process_done_signal)
    #task3.start()
    app.exec_()
    while 1:
        time.sleep(0.001)
        #print(usb_serial.readline())

    sys.exit()
