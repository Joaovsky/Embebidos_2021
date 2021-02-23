import cv2 as cv
import sys
from PyQt5 import QtWidgets, QtCore ,uic
from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import QTimer,QDateTime
from PyQt5.QtCore import pyqtSignal, QThread
from PyQt5.QtGui import QPixmap

import queue
#from picamera import PiCamera
import serial
import time
import sys
import tensorflow as tf
import math
import numpy as np
#from PIL import Image
import os

mode = 0
ref = 0
actual_lux = 0
actual_hum = 0
actual_temp = 0
setpoint = 20
counter1 = 0
counter2 = 0

#usb_serial = serial.Serial('/dev/ttyACM0',115200)

prototxt_path = "MobileNetSSD_deploy.prototxt"
model_path = "MobileNetSSD_deploy.caffemodel"
model = tf.keras.models.load_model(r'./trained_model')
label_array = ['Stop', 'Keep Right', 'Yield']

net = cv.dnn.readNetFromCaffe(prototxt_path, model_path)

#############################################
frameWidth = 640         # CAMERA RESOLUTION
frameHeight = 480
brightness = 180
threshold = 0.95         # PROBABLITY THRESHOLD
font = cv.FONT_HERSHEY_SIMPLEX
##############################################

def grayscale(img):
    img = cv.cvtColor(img,cv.COLOR_BGR2GRAY)
    return img
def equalize(img):
    img = cv.equalizeHist(img)
    return img
def preprocessing(img):
    img = grayscale(img)
    img = equalize(img)
    img = img/255
    return img
def getCalssName(classNo):
    if   classNo == 0: return label_array[0]
    elif classNo == 1: return label_array[1]
    elif classNo == 2: return label_array[2]

class Ui(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui,self).__init__()
        uic.loadUi('mainwindow.ui',self)
        self.more_btn.clicked.connect(self.btn_more_pressed)
        self.less_btn.clicked.connect(self.btn_less_pressed)
        self.auto_btn.clicked.connect(self.btn_auto_pressed)
        self.ref_light_slider.valueChanged.connect(self.slider_ref_changed)
        self.setWindowFlag(QtCore.Qt.FramelessWindowHint)
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

class capture_img_thread(QThread):
    #capture image
    done_signal = pyqtSignal(str)
    def __init__(self):
        #self.Cam = PiCamera()
        #self.Cam.resolution =(640,480)
        self.img0 = cv.VideoCapture(0)
        self.img0.set(3, frameWidth)
        self.img0.set(4, frameHeight)
        self.img0.set(10, brightness)
        QThread.__init__(self)
    def run(self):
        print("Entrada: Tirar foto")
        success, cap2 = self.img0.read()
        q_signs.put(cap2)
        q_cars.put(cap2)
        self.done_signal.emit('Foto')
        print("Saida: Tirar foto")

class receive_from_stm_thread(QThread):
    #read imputs from stm
    def __init__(self):
        QThread.__init__(self)
    def run(self):
        print("Entrada: Receive from STM")

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
            #QThread.msleep(50)
        print("Saida: Receive from STM")

class detect_cars_thread(QThread):
    #Obstacle Detection
    done_signal = pyqtSignal(str)
    def __init__(self):
        self.CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat",
           "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
           "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
           "sofa", "train", "tvmonitor"]
        #self.timer.singleShot(5000, lambda: win.CollisionWarningLabel.setPixmap(self.pix))
        QThread.__init__(self)
    def run(self):
        print("Entrada: Verificar se existe carro")
        #self.timer = QTimer()
        img = q_cars.get()
        rgb = cv.cvtColor(img, cv.COLOR_BGR2RGB)
        (H, W) = img.shape[:2]
        # convert the frame to a blob and pass the blob through the
        # network and obtain the detections
        blob = cv.dnn.blobFromImage(img, size=(300, 300), ddepth=cv.CV_8U)
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
                idx = int(detections[0, 0, i, 1])
                if self.CLASSES[idx] != "car":
                    #print (self.CLASSES[idx])
                    self.done_signal.emit('NOP')
                    break
                box = detections[0, 0, i, 3:7] * np.array([W, H, W, H])
                (startX, startY, endX, endY) = box.astype("int")
                    #find screen dimensions
                if ((endX - startX)>0.3*W)and((0.3*W)<((endX+startX)/2)<(0.7*W)):
                    print("COLLISION WARNING")
                    pix = QPixmap('../app_python/signs/collision_avoidance.png')
                    win.CollisionWarningLabel.setPixmap(pix)
                    win.collision_label.setText('Collision Warning')
                    os.system(f'aplay ../app_python/signs/sound.wav')
                    self.done_signal.emit('Obstacle')
        else:
            self.done_signal.emit('NOP')
        print("Saida: Verificar se existe carros")

class detect_signals_thread(QThread):
    #Signal Detection
    done_signal = pyqtSignal(str)
    def __init__(self):
        self.Threshold = 0.95
        QThread.__init__(self)
    def run(self):
        print("Entrada: Verificar se existe sinais")
        # READ IMAGE
        global model
        img0 = q_signs.get()
        print("Dps retirar queue")
        img = np.asarray(img0)
        img = cv.resize(img, (32, 32))
        img = preprocessing(img)
        img = img.reshape(1, 32, 32, 1)
        # PREDICT IMAGE
        predictions = model.predict(img)
        classIndex = model.predict_classes(img)
        probabilityValue = np.amax(predictions)
        print("Dps inferencia")
        if probabilityValue > self.Threshold:
            print("predicted")
            print(probabilityValue)
            signs_path = '../app_python/signs/' + str(classIndex)[1] + '/img.png'
            audio_path = f'aplay ../app_python/signs/' + str(classIndex)[1] + '/sound.wav'
            win.Sign_label.setText(label_array[int(str(classIndex)[1])])
            pix = QPixmap(signs_path)
            win.Sign_show.setPixmap(pix)
            os.system(audio_path)
            self.done_signal.emit('Sign')
        else:
            self.done_signal.emit('NOP')
        print("Saida: Verificar se existe sinais")

def clean_sign_windows():
    win.Sign_show.clear()
    win.Sign_label.clear()

def clean_car_windows():
    win.collision_label.clear()
    win.CollisionWarningLabel.clear()

def process_done_signal(result):
    # se já tirou foto->avaliar se é sinal/ obstaculo
    if result == 'Foto':

        detect_signals.start()
        detect_vehicles.start()

    elif result == 'Sign':

        cleartrafficsign = QTimer()
        cleartrafficsign.singleShot(2000, clean_sign_windows)
        img_capture.start()

    elif result == 'Obstacle':

        clearcarsign = QTimer()
        clearcarsign.singleShot(2000, clean_car_windows)
        img_capture.start()

    elif result == 'NOP':
        img_capture.start()

if __name__ == '__main__':

    #Application Start
    app = QApplication(sys.argv)
    win = Ui()

    #Queues Initialization
    q_signs = queue.Queue()
    q_cars = queue.Queue()

    #Camera Capture Task
    img_capture = capture_img_thread()
    img_capture.done_signal.connect(process_done_signal)

    #Receive from STM Task
    receive_from_stm = receive_from_stm_thread()

    #Detect vehicles Task
    detect_vehicles = detect_cars_thread()
    detect_vehicles.done_signal.connect(process_done_signal)

    #Detect Transit Signals Task
    detect_signals = detect_signals_thread()
    detect_signals.done_signal.connect(process_done_signal)

    #Tasks initialization
    img_capture.start()
    receive_from_stm.start()

    while True:
        app.exec_()
        
    cv.destroyAllWindows()
    sys.exit()