# -*- coding: utf-8 -*-

import sys
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *

global sec
sec = 0
class WorkThread(QThread):
    trigger = pyqtSignal()
    def __init__(self):
        super(WorkThread, self).__init__()
    def run(self):
        for i in range(20000000000):
            pass
        self.trigger.emit()

def countTime():
    global sec
    sec += 1
    lcdNumber.display(sec)

def work():
    timer.start(1000)
    workThread.start()
    workThread.trigger.connect(timeStop)

def timeStop():
    timer.stop()
    print("End-of-run time",lcdNumber.value())
    global sec
    sec = 0


if __name__ == "__main__":
    app = QApplication(sys.argv)
    top = QWidget()
    top.resize(300,120)
    layout = QVBoxLayout(top)
    lcdNumber = QLCDNumber()
    layout.addWidget(lcdNumber)
    button = QPushButton("test")
    layout.addWidget(button)

    timer = QTimer()
    workThread = WorkThread()
    button.clicked.connect(work)
    timer.timeout.connect(countTime)
    top.show()
    sys.exit(app.exec_())