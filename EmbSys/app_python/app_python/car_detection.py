import time
import cv2 as cv
import numpy as np
import math
import tensorflow as tf

gpu_options = tf.compat.v1.GPUOptions(per_process_gpu_memory_fraction=0.7)
sess = tf.compat.v1.Session(config=tf.compat.v1.ConfigProto(gpu_options=gpu_options))

print("Num GPUs Available: ", len(tf.config.experimental.list_physical_devices('GPU')))

print("GPU device: ", tf.test.gpu_device_name())

# load our serialized model from disk
print("Load MobileNetSSD model")

#prototxt_path = r'D:\mobileNET_CAR_det\MobileNet-SSD\vov\MobileNetSSD_deploy.prototxt'
#model_path = r'D:\mobileNET_CAR_det\MobileNet-SSD\voc\MobileNetSSD_deploy.caffemodel'

prototxt_path = "MobileNetSSD_deploy.prototxt"
model_path = "MobileNetSSD_deploy.caffemodel"
#file_video = "production ID_4791734.mp4"
file_video = "cutcartest.mp4"


#MobileNetSSD_deploy.prototxt
#MobileNetSSD_deploy.caffemodel
# initialize the list of class labels MobileNet SSD was trained to detect
CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat",
           "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
           "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
           "sofa", "train", "tvmonitor"]

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
            # extract the index of the class label from the
            # detections list
            idx = int(detections[0, 0, i, 1])
            # if the class label is not a car, ignore it
            if CLASSES[idx] != "car":
                continue
            # compute the (x, y)-coordinates of the bounding box
            # for the object
            box = detections[0, 0, i, 3:7] * np.array([W, H, W, H])
            (startX, startY, endX, endY) = box.astype("int")

            cv.rectangle(next_frame, (startX, startY), (endX, endY), (0, 255, 0), 3)

    return next_frame


def VehicheDetection_UsingMobileNetSSD(filename):
    cap = cv.VideoCapture(filename)

    # Write output file
    frame_width = int(cap.get(cv.CAP_PROP_FRAME_WIDTH))
    #frame_width = int(cap.get(3))

    frame_height = int(cap.get(cv.CAP_PROP_FRAME_HEIGHT))
    #frame_height = int(cap.get(4))

    # Define the codec and create VideoWriter object
    fps = 20
    size = (int(frame_width), int(frame_height))
    fourcc = cv.VideoWriter_fourcc('m', 'p', '4', 'v')
    out = cv.VideoWriter()
    success = out.open('car_det_test.mp4', fourcc, fps, size, True)

    frame_count = 0

    # start timer
    t1 = time.time()

    while True:
        ret, next_frame = cap.read()  # Reads the next video frame into memory

        if ret == False: break

        frame_count += 1
        next_frame = process_frame_MobileNetSSD(next_frame)

        # write frame
        out.write(next_frame)

    # end timer
    t2 = time.time()

    # calculate FPS
    fps = str(float(frame_count / float(t2 - t1))) + ' FPS'

    print("/MobileNetSSD Car Detector")
    print("Frames processed: {}".format(frame_count))
    print("Elapsed time: {:.2f}".format(float(t2 - t1)))
    print("FPS: {}".format(fps))

    cap.release()
    cv.destroyAllWindows()
    out.release()

VehicheDetection_UsingMobileNetSSD(file_video)
