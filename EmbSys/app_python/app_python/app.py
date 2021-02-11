# -*- coding: utf-8 -*-
import cv2 as cv
import sys
from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import QTimer,QDateTime
from PyQt5.QtCore import pyqtSignal, QThread
from PyQt5 import QtWidgets,uic
from PyQt5.QtGui import QPixmap
from PyQt5 import QtWidgets, QtCore
from timeit import Timer
#from picamera import PiCamera
#from serial.tools import list_ports
import serial
import time
import sys
import shutil
import threading
import tensorflow as tf
import math
import numpy as np
import time
from PIL import Image
#from playsound import playsound
import os

mode=0
ref=0
actual_lux=0
actual_hum=0
actual_temp=0
setpoint=0
triggered = 0

prototxt_path = "MobileNetSSD_deploy.prototxt"
model_path = "MobileNetSSD_deploy.caffemodel"
file_video = "cutcartest.mp4"
img = Image.open('../app_python/images/signs/collision_avoidance.png')
net = cv.dnn.readNetFromCaffe(prototxt_path, model_path)


def process_frame_MobileNetSSD(next_frame):
    rgb = cv.cvtColor(next_frame, cv.COLOR_BGR2RGB)
    (H, W) = next_frame.shape[:2]
    # convert the frame to a blob and pass the blob through the
    # network and obtain the detections
    blob = cv.dnn.blobFromImage(next_frame, size=(300, 300), ddepth=cv.CV_8U)
    net.setInput(blob, scalefactor=1.0 / 127.5, mean=[127.5, 127.5, 127.5])
    detections = net.forward()
    # loop over the detections
    for i in np.arange(0, detections.shape[2]):
        # extract the confidence (i.e., probability) associated
        # with the prediction
        confidence = detections[0, 0, i, 2]
        # filter out weak detections by ensuring the `confidence`
        # is greater than the minimum confidence
        if confidence > 0.7:
            box = detections[0, 0, i, 3:7] * np.array([W, H, W, H])
            (startX, startY, endX, endY) = box.astype("int")
            #find screen dimensions
            if ((endX - startX)>0.3*W)and((0.3*W)<((endX+startX)/2)<(0.7*W)):
                pix = QPixmap('../app_python/images/signs/collision_avoidance.png')
                win.CollisionWarningLabel.setPixmap(pix)
                win.Sign_show.clear()
                timer.start()
    return next_frame

def VehicheDetection_UsingMobileNetSSD(filename):
    cap = cv.VideoCapture(filename)
    # Write output file
    #frame_width = int(cap.get(cv.CAP_PROP_FRAME_WIDTH))
    #frame_height = int(cap.get(cv.CAP_PROP_FRAME_HEIGHT))
    # Define the codec and create VideoWriter object
    #fps = 20
    #size = (int(frame_width), int(frame_height))
    #fourcc = cv.VideoWriter_fourcc('m', 'p', '4', 'v')
    #out = cv.VideoWriter()
    #success = out.open('car_det_test.mp4', fourcc, fps, size, True)
    #frame_count = 0
    # start timer
    #t1 = time.time()
    while True:
        ret, next_frame = cap.read()  # Reads the next video frame into memory
        if ret == False: break
        #frame_count += 1
        next_frame = process_frame_MobileNetSSD(next_frame)
        # write frame
        #out.write(next_frame)
        #key = cv.waitKey(50)
        #if key == 27:  # Hit ESC key to stop
            #break
    # end timer
    #t2 = time.time()
    # calculate FPS
    #fps = str(float(frame_count / float(t2 - t1))) + ' FPS'
    #print("/MobileNetSSD Car Detector")
    #print("Frames processed: {}".format(frame_count))
    #print("Elapsed time: {:.2f}".format(float(t2 - t1)))
    #print("FPS: {}".format(fps))
    #cap.release()
    #cv.destroyAllWindows()
    #out.release()

class Ui(QtWidgets.QMainWindow):
    mode=0
    def __init__(self):
        self.pix= QPixmap("../app_python/images/icons/temperature.png")
        super(Ui,self).__init__()
        uic.loadUi('mainwindow.ui',self)
        self.more_btn.clicked.connect(self.btn_more_pressed)
        self.less_btn.clicked.connect(self.btn_less_pressed)
        self.auto_btn.clicked.connect(self.btn_auto_pressed)
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
        #Cam = PiCamera()
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
    #deteçao de obstaculo
    done_signal = pyqtSignal(str)
    def __init__(self):
        QThread.__init__(self)
    def run(self):
        # Do some work here
        VehicheDetection_UsingMobileNetSSD(file_video)
        if True:
            self.done_signal.emit('Signal')
        if True:
            self.done_signal.emit('Obstacle')
        else:
            QThread.msleep(10)

class MyTask4(QThread):
    #deteçao de sinal
    done_signal = pyqtSignal(str)
    def __init__(self):
        QThread.__init__(self)
    def run(self):
        # Do some work here
        #VehicheDetection_UsingMobileNetSSD(file_video)
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



class Timer1(QThread):
    #deteçao de sinal
    done_signal = pyqtSignal(str)
    def __init__(self):
        self.var = 0
        QThread.__init__(self)
    def run(self):
        # Do some work here
        while True:
            if self.var==2:
                self.var = 0
                win.CollisionWarningLabel.clear()
            self.var += 1
            QThread.msleep(1000)
"""   
        if True:
            self.done_signal.emit('Signal')
        if True:
            self.done_signal.emit('Obstacle')
        else:
            QThread.msleep(10)
"""

if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = Ui()

    #signal
    #signal startTimer();
    #signal
    #signal stopTimer();

    task1 = MyTask1()
    task2 = MyTask2()
    task3 = MyTask3()
    timer = Timer1()
    #task4 = MyTask4()
    #print(list_ports.comports())
    task1.done_signal.connect(process_done_signal)
    task1.start()
    #task2.done_signal.connect(process_done_signal)
    task2.start()
    task3.done_signal.connect(process_done_signal)
    task3.start()
    #task4.done_signal.connect(process_done_signal)
    #task4.start()
    app.exec_()
    while 1:
        time.sleep(0.001)
        #print(usb_serial.readline())

    sys.exit()
