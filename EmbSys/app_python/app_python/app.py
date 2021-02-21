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
import os
from tensorflow.python.client import device_lib
import datetime

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
img = Image.open('../app_python/signs/collision_avoidance.png')
net = cv.dnn.readNetFromCaffe(prototxt_path, model_path)

img1 = cv.imread('30_test.png')
img2 = cv.imread('no_entry_test.png')
img3 = cv.imread('roundabout_mandatory_test.png')
img4 = cv.imread('stop_test.png')

img_array = [img1,img2,img3,img4]
img_index = 0

img_vid1 = cv.imread('vid00001.png')
img_vid2 = cv.imread('vid00003.png')
img_vid3 = cv.imread('vid00005.png')
img_vid4 = cv.imread('vid00007.png')
img_vid5 = cv.imread('vid00009.png')
img_vid6 = cv.imread('vid00011.png')
img_vid7 = cv.imread('vid00013.png')
img_vid8 = cv.imread('vid00015.png')
img_vid9 = cv.imread('vid00017.png')
img_vid10 = cv.imread('vid00019.png')
img_vid11 = cv.imread('vid00021.png')
img_vid12 = cv.imread('vid00023.png')

img_vid_array = [img_vid1,img_vid2,img_vid3,img_vid4,img_vid5,img_vid6,img_vid7,img_vid8,img_vid9,img_vid10,img_vid11,img_vid12]
img_vid_index = 0

#############################################
frameWidth= 640         # CAMERA RESOLUTION
frameHeight = 480
brightness = 180
threshold = 0.9         # PROBABLITY THRESHOLD
font = cv.FONT_HERSHEY_SIMPLEX
##############################################

model = tf.keras.models.load_model(r'./trained_model')

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
                pix = QPixmap('../app_python/signs/collision_avoidance.png')
                win.CollisionWarningLabel.setPixmap(pix)
                win.collision_label.setText('Collision Warning')
                os.system(f'aplay ../app_python/signs/sound.wav')
                win.Sign_show.clear()
                win.Sign_label.clear()
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

label_array = ['Stop', 'Keep Right', 'Yield']

def grayscale(img):
    img = cv.cvtColor(img,cv.COLOR_BGR2GRAY)
    return img
def equalize(img):
    img =cv.equalizeHist(img)
    return img
def preprocessing(img):
    img = grayscale(img)
    img = equalize(img)
    img = img/255
    return img
#def getCalssName(classNo):
    if   classNo == 0: return label_array[0]
    elif classNo == 1: return label_array[1]
    elif classNo == 2: return label_array[2]

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
        global img_vid_array
        global img_vid_index
        while True:
            process_frame_MobileNetSSD(img_vid_array[img_vid_index])
            img_vid_index = (img_vid_index + 1) % len(img_vid_array)
            QThread.msleep(1000)

def process_signs_frame_CNN(self):
    # PROCESS IMAGE
    cap2 = img_array[img_index]
    img = np.asarray(cap2)
    img = cv.resize(img, (32, 32))
    img = preprocessing(img)
    img = img.reshape(1, 32, 32, 1)
    # PREDICT IMAGE
    predictions = model.predict(img)
    classIndex = model.predict_classes(img)
    probabilityValue = np.amax(predictions)
    if probabilityValue > threshold:
        signs_path = '../app_python/signs/' + str(classIndex)[1] + '/img.png'
        audio_path = f'aplay ../app_python/signs/' + str(classIndex)[1] + '/sound.wav'
        win.Sign_label.setText(label_array[int(str(classIndex)[1])])
        pix = QPixmap(signs_path)
        win.Sign_show.setPixmap(pix)
        os.system(audio_path)

def process_done_signal(result):
    pix= QPixmap("../app_python/test.jpg")
    if result == 'Foto': #se já tirou foto->avaliar se é sinal/ obstaculo
        shutil.copy2('../app_python/test.jpg','../app_python/foto.jpg')
        win.Sign_show.setPixmap(pix)

class MyTask4(QThread):
    #deteçao de sinal
    done_signal = pyqtSignal(str)
    def __init__(self):
        QThread.__init__(self)
    def run(self):
        # READ IMAGE
        global img_array
        global img_index
        while True:
            process_signs_frame_CNN(self)
            img_index = (img_index + 1) % len(img_array)
            QThread.msleep(1000)

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
                win.collision_label.clear()
            self.var += 1
            QThread.msleep(1000)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = Ui()
    task1 = MyTask1()
    task2 = MyTask2()
    task3 = MyTask3()
    task4 = MyTask4()
    timer = Timer1()
    task1.start()
    task2.start()
    task3.start()
    task4.start()
    app.exec_()
    while 1:
        time.sleep(0.001)
    sys.exit()