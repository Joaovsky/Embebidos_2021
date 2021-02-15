import numpy as np
import cv2
import pickle
import tensorflow as tf
from tensorflow import keras
import numpy as np
import time
from tensorflow.python.client import device_lib
import datetime

def main():
    gpu_options = tf.compat.v1.GPUOptions(per_process_gpu_memory_fraction=1)
    sess = tf.compat.v1.Session(config=tf.compat.v1.ConfigProto(gpu_options=gpu_options))

    print("Num GPUs Available: ", len(tf.config.experimental.list_physical_devices('GPU')))

    print("GPU device: ", tf.test.gpu_device_name())

    font = cv2.FONT_HERSHEY_SIMPLEX

    cap = cv2.VideoCapture(0)

    fps_start_time = datetime.datetime.now()
    fps = 0
    total_frames = 0

    while True:
        ret, frame = cap.read()
        total_frames = total_frames+1

        fps_end_time = datetime.datetime.now()
        time_diff = fps_end_time - fps_start_time
        if time_diff.seconds == 0:
            fps = 0.0
        else:
            fps = total_frames/time_diff.seconds

        fps_text = "FPS: {:.2f}".format(fps)

        cv2.putText(frame, fps_text, (5,30), font, 1, (0, 0, 255), 1, cv2.LINE_AA)

        cv2.imshow("Application", frame)
        key = cv2.waitKey(1)
        if key == ord('q'):
            break

    cv2.destroyALLWindows()

main()










import numpy as np
import cv2
import pickle
#############################################
frameWidth= 640         # CAMERA RESOLUTION
frameHeight = 480
brightness = 180
threshold = 0.75         # PROBABLITY THRESHOLD
font = cv2.FONT_HERSHEY_SIMPLEX
##############################################
# SETUP THE VIDEO CAMERA
cap = cv2.VideoCapture(0)
cap.set(3, frameWidth)
cap.set(4, frameHeight)
cap.set(10, brightness)
# IMPORT THE TRANNIED MODEL
pickle_in=open("model_trained.p","rb")  ## rb = READ BYTE
model=pickle.load(pickle_in)
def grayscale(img):
img = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
return img
def equalize(img):
img =cv2.equalizeHist(img)
return img
def preprocessing(img):
img = grayscale(img)
img = equalize(img)
img = img/255
return img
def getCalssName(classNo):
if   classNo == 0: return 'Speed Limit 20 km/h'
elif classNo == 1: return 'Speed Limit 30 km/h'
elif classNo == 2: return 'Speed Limit 50 km/h'
elif classNo == 3: return 'Speed Limit 60 km/h'
elif classNo == 4: return 'Speed Limit 70 km/h'
elif classNo == 5: return 'Speed Limit 80 km/h'
elif classNo == 6: return 'End of Speed Limit 80 km/h'
elif classNo == 7: return 'Speed Limit 100 km/h'
elif classNo == 8: return 'Speed Limit 120 km/h'
elif classNo == 9: return 'No passing'
elif classNo == 10: return 'No passing for vechiles over 3.5 metric tons'
elif classNo == 11: return 'Right-of-way at the next intersection'
elif classNo == 12: return 'Priority road'
elif classNo == 13: return 'Yield'
elif classNo == 14: return 'Stop'
elif classNo == 15: return 'No vechiles'
elif classNo == 16: return 'Vechiles over 3.5 metric tons prohibited'
elif classNo == 17: return 'No entry'
elif classNo == 18: return 'General caution'
elif classNo == 19: return 'Dangerous curve to the left'
elif classNo == 20: return 'Dangerous curve to the right'
elif classNo == 21: return 'Double curve'
elif classNo == 22: return 'Bumpy road'
elif classNo == 23: return 'Slippery road'
elif classNo == 24: return 'Road narrows on the right'
elif classNo == 25: return 'Road work'
elif classNo == 26: return 'Traffic signals'
elif classNo == 27: return 'Pedestrians'
elif classNo == 28: return 'Children crossing'
elif classNo == 29: return 'Bicycles crossing'
elif classNo == 30: return 'Beware of ice/snow'
elif classNo == 31: return 'Wild animals crossing'
elif classNo == 32: return 'End of all speed and passing limits'
elif classNo == 33: return 'Turn right ahead'
elif classNo == 34: return 'Turn left ahead'
elif classNo == 35: return 'Ahead only'
elif classNo == 36: return 'Go straight or right'
elif classNo == 37: return 'Go straight or left'
elif classNo == 38: return 'Keep right'
elif classNo == 39: return 'Keep left'
elif classNo == 40: return 'Roundabout mandatory'
elif classNo == 41: return 'End of no passing'
elif classNo == 42: return 'End of no passing by vechiles over 3.5 metric tons'
while True:
# READ IMAGE
success, imgOrignal = cap.read()
# PROCESS IMAGE
img = np.asarray(imgOrignal)
img = cv2.resize(img, (32, 32))
img = preprocessing(img)
cv2.imshow("Processed Image", img)
img = img.reshape(1, 32, 32, 1)
cv2.putText(imgOrignal, "CLASS: " , (20, 35), font, 0.75, (0, 0, 255), 2, cv2.LINE_AA)
cv2.putText(imgOrignal, "PROBABILITY: ", (20, 75), font, 0.75, (0, 0, 255), 2, cv2.LINE_AA)
# PREDICT IMAGE
predictions = model.predict(img)
classIndex = model.predict_classes(img)
probabilityValue =np.amax(predictions)
if probabilityValue > threshold:
#print(getCalssName(classIndex))
cv2.putText(imgOrignal,str(classIndex)+" "+str(getCalssName(classIndex)), (120, 35), font, 0.75, (0, 0, 255), 2, cv2.LINE_AA)
cv2.putText(imgOrignal, str(round(probabilityValue*100,2) )+"%", (180, 75), font, 0.75, (0, 0, 255), 2, cv2.LINE_AA)
cv2.imshow("Result", imgOrignal)
if cv2.waitKey(1) and 0xFF == ord('q'):
break










