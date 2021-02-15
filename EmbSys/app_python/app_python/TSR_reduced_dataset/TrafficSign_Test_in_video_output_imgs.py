import cv2 as cv
import tensorflow as tf
import numpy as np
import time
from tensorflow.python.client import device_lib
import datetime
from PIL import Image
#img = Image.open('collision_avoidance.png')
gpu_options = tf.compat.v1.GPUOptions(per_process_gpu_memory_fraction=0.5)
sess = tf.compat.v1.Session(config=tf.compat.v1.ConfigProto(gpu_options=gpu_options))

start_time = time.time() # start time of the loop

img1 = Image.open('30.png')
img2 = Image.open('no_entry.png')
img3 = Image.open('roundabout_mandatory.png')
img4 = Image.open('STOP.png')

print(device_lib.list_local_devices())
print("Num GPUs Available: ", len(tf.config.experimental.list_physical_devices('GPU')))
print("GPU device: ", tf.test.gpu_device_name())

#from keras.models import load_model, Model
#from keras import backend as K

#sess = tf.Session()
#K.set_session(sess)
#..... train your model

#############################################
frameWidth= 640         # CAMERA RESOLUTION
frameHeight = 480
brightness = 180
threshold = 0.9         # PROBABLITY THRESHOLD
font = cv.FONT_HERSHEY_SIMPLEX
##############################################

# SETUP THE VIDEO CAMERA
cap = cv.VideoCapture('tsr_2.mp4')
cap.set(3, frameWidth)
cap.set(4, frameHeight)
cap.set(10, brightness)
fps_start_time = datetime.datetime.now()
fps=0
total_frames=0

# initialize the list of class labels MobileNet SSD was trained to detect
CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat",
           "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
           "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
           "sofa", "train", "tvmonitor"]

gpu_options = tf.compat.v1.GPUOptions(per_process_gpu_memory_fraction=0.7)
sess = tf.compat.v1.Session(config=tf.compat.v1.ConfigProto(gpu_options=gpu_options))
#img = Image.open('collision_avoidance.png')
triggered = 0

# load our serialized model from disk
print("Load MobileNetSSD model")

#prototxt_path = "MobileNetSSD_deploy.prototxt"
#model_path = "MobileNetSSD_deploy.caffemodel"
#file_video = "cutcartest.mp4"

t3 = time.time()
#net = cv.dnn.readNetFromCaffe(prototxt_path, model_path)

# IMPORT THE TRAINED MODEL
# pickle_in=open("model_trained.p","rb")  ## rb = READ BYTE
# model=pickle.load(pickle_in)
model = tf.keras.models.load_model(r'./trained_model')

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
def getCalssName(classNo):
    if   classNo == 0: return 'Speed Limit 30 km/h'
    elif classNo == 1: return 'Stop'
    elif classNo == 2: return 'No entry'
    elif classNo == 3: return 'Bumpy road'
    elif classNo == 4: return 'Roundabout mandatory'
    elif classNo == 5: return 'Speed Limit 50 km/h'
    elif classNo == 6: return 'No passing'

while True:
    # READ IMAGE
    success, imgOrignal = cap.read()
    # PROCESS IMAGE
    img = np.asarray(imgOrignal)
    img = cv.resize(img, (32, 32))
    img = preprocessing(img)
    #cv.imshow('Processed Image', img)
    img = img.reshape(1, 32, 32, 1)
    cv.putText(imgOrignal, "CLASS: ", (20, 35), font, 0.75, (0, 0, 255), 2, cv.LINE_AA)
    cv.putText(imgOrignal, "PROBABILITY: ", (20, 75), font, 0.75, (0, 0, 255), 2, cv.LINE_AA)
    cv.putText(imgOrignal, "FPS: ", (20, 105), font, 0.75, (0, 0, 255),2, cv.LINE_AA)
    # PREDICT IMAGE
    predictions = model.predict(img)
    classIndex = model.predict_classes(img)
    probabilityValue =np.amax(predictions)
    #print("FPS: ", 1.0 / (time.time() - start_time))  # FPS = 1 / time to process loop
    if probabilityValue > threshold:
        #rint(getCalssName(classIndex))
        cv.putText(imgOrignal,str(classIndex)+" "+str(getCalssName(classIndex)), (120, 35), font, 0.75, (0, 0, 255), 2, cv.LINE_AA)
        cv.putText(imgOrignal, str(round(probabilityValue*100,2) )+"%", (180, 75), font, 0.75, (0, 0, 255), 2, cv.LINE_AA)
        cv.putText(imgOrignal, str(1.0 / (time.time() - start_time)), (240, 105), font, 0.75, (0, 0, 255), 2, cv.LINE_AA)
        img1.show()

        #cv.imshow("Result", imgOrignal)
        #cv.waitKey(0)
    #if cv.waitKey(1) and 0xFF == ord('q'):
        #break

