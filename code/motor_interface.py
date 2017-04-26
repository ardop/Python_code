import sys 
import os
import thread
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import subprocess
import os
import atexit

#Default angles
lDefaultAngles = [90, 0, 90, 90, 90, 40]
rDefaultAngles = [90, 0, 90, 90, 90, 40]
hDefaultAngles = [90, 90]

#Angles to be written into the file
lAngles = [0, 0, 0, 0, 0, 0]
rAngles = [0, 0, 0, 0, 0, 0]
hAngles = [0, 0]

#os.system('gnome-terminal & disown')
#os.system('gnome-terminal --window-with-profile=karun -e echo ubuntu | sudo -S ../build/mic')
#os.system('cd ../build && sudo ./mic')

#p = subprocess.Popen('sudo ../build/test', shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
#for line in p.stdout.readlines():
	#print line,   

class Motor_Interface(QDialog):
    
    def __init__(self, parent = None):
        
        super(Motor_Interface,self).__init__(parent)


        leftLabel = QLabel('Left Arm ')
        rightLabel = QLabel('Right Arm ')
        headLabel = QLabel('Head')

        #Initializing widgets

        self.setButton = QPushButton('Set Default Values')
        self.aButton = QPushButton('Move')

		
        self.lShoulderText = QLineEdit()
        self.lShoulderText.setFixedSize(80, 20)
        self.lFlapText = QLineEdit()
        self.lFlapText.setFixedSize(80, 20)
        self.lRotText = QLineEdit()
        self.lRotText.setFixedSize(80, 20)
        self.lElbowText = QLineEdit()
        self.lElbowText.setFixedSize(80, 20)
        self.lHandText = QLineEdit()
        self.lHandText.setFixedSize(80, 20)
        self.lGripperText = QLineEdit()
        self.lGripperText.setFixedSize(80, 20)

        self.rShoulderText = QLineEdit()
        self.rShoulderText.setFixedSize(80, 20)
        self.rFlapText = QLineEdit()
        self.rFlapText.setFixedSize(80, 20)
        self.rRotText = QLineEdit()
        self.rRotText.setFixedSize(80, 20)
        self.rElbowText = QLineEdit()
        self.rElbowText.setFixedSize(80, 20)
        self.rHandText = QLineEdit()
        self.rHandText.setFixedSize(80, 20)
        self.rGripperText = QLineEdit()
        self.rGripperText.setFixedSize(80, 20)

        self.vHead = QLineEdit()
        self.vHead.setFixedSize(80, 20)
        self.hHead = QLineEdit()
        self.hHead.setFixedSize(80, 20)

        #Grid and widget locations
        
        grid = QGridLayout()

        grid.addWidget(leftLabel, 0, 0)
        grid.addWidget(rightLabel, 0, 2)

        grid.addWidget(self.lShoulderText, 1, 0)
        grid.addWidget(self.lFlapText, 2, 0)
        grid.addWidget(self.lRotText, 3, 0)
        grid.addWidget(self.lElbowText, 4, 0)
        grid.addWidget(self.lHandText, 5, 0)
        grid.addWidget(self.lGripperText, 6, 0)

        grid.addWidget(self.rShoulderText, 1, 2)
        grid.addWidget(self.rFlapText, 2, 2)
        grid.addWidget(self.rRotText, 3, 2)
        grid.addWidget(self.rElbowText, 4, 2)
        grid.addWidget(self.rHandText, 5, 2)
        grid.addWidget(self.rGripperText, 6, 2)


        grid.addWidget(headLabel, 8, 1)
        grid.addWidget(self.vHead, 9, 1)
        grid.addWidget(self.hHead, 10, 1)

        grid.addWidget(self.aButton, 12, 0)
        grid.addWidget(self.setButton, 12, 2)

        self.connect(self.aButton, SIGNAL("clicked()"), self.write_angles)
        self.connect(self.setButton, SIGNAL("clicked()"), self.set_angles)
        
        
        self.setLayout(grid)
        self.setWindowTitle("Motor Control Interface")
        self.setGeometry(500,300,400,100)

    def write_angles(self):
        
        lAngles[0] = str(self.lShoulderText.text())
        lAngles[1] = str(self.lFlapText.text())
        lAngles[2] = str(self.lRotText.text())
        lAngles[3] = str(self.lElbowText.text())
        lAngles[4] = str(self.lHandText.text())
        lAngles[5] = str(self.lGripperText.text())

        rAngles[0] = str(self.rShoulderText.text())
        rAngles[1] = str(self.rFlapText.text())
        rAngles[2] = str(self.rRotText.text())
        rAngles[3] = str(self.rElbowText.text())
        rAngles[4] = str(self.rHandText.text())
        rAngles[5] = str(self.rGripperText.text())

        hAngles[0] = str(self.vHead.text())
        hAngles[1] = str(self.hHead.text())

        for i, angle in enumerate(lAngles):

            if angle=='':
                lAngles[i] = int(lDefaultAngles[i])
            else:
                lAngles[i] = int(lAngles[i])

        for i, angle in enumerate(rAngles):

            if angle=='':
                rAngles[i] = int(rDefaultAngles[i])
            else:
                rAngles[i] = int(rAngles[i])

        for i, angle in enumerate(hAngles):

            if angle=='':
                hAngles[i] = int(hDefaultAngles[i])
            else:
                hAngles[i] = int(hAngles[i])


		file = open('angles.txt', 'wb')
		for angle in rAngles:
			file.write(str(angle) + '\n')
        for angle in lAngles:
            file.write(str(angle) + '\n')
        for angle in hAngles:
            file.write(str(angle) + '\n')

        file.close()
        
        print "Moved"


    def set_angles(self):
        
        self.lShoulderText.setText(str(lDefaultAngles[0]))
        self.lFlapText.setText(str(lDefaultAngles[1]))
        self.lRotText.setText(str(lDefaultAngles[2]))
        self.lElbowText.setText(str(lDefaultAngles[3]))
        self.lHandText.setText(str(lDefaultAngles[4]))
        self.lGripperText.setText(str(lDefaultAngles[5]))

        self.rShoulderText.setText(str(rDefaultAngles[0]))
        self.rFlapText.setText(str(rDefaultAngles[1]))
        self.rRotText.setText(str(rDefaultAngles[2]))
        self.rElbowText.setText(str(rDefaultAngles[3]))
        self.rHandText.setText(str(rDefaultAngles[4]))
        self.rGripperText.setText(str(rDefaultAngles[5]))

        self.vHead.setText(str(hDefaultAngles[0]))
        self.hHead.setText(str(hDefaultAngles[1]))


def cpp_code():
	
	#os.system('gnome-terminal')
	#os.system('gnome-terminal -e cd ../build && echo ubuntu | sudo -S ./ik_interface && cd ../code')
	#os.system("gnome-terminal -e --working-directory=/home/ubuntu/ardop/kinematics/build -e 'sudo ./ik_interface'")
	os.system('xterm -hold -e "cd ../build && echo ubuntu | sudo -S ./m_interface && cd ../code"')
		

def ui_code():
	
	app = QApplication(sys.argv)
	motor = Motor_Interface()
	motor.show()
	app.exec_()
	# code to execute right before exiting
	sys.exit()
	
	       
try:
	
	thread.start_new_thread(cpp_code, ())
	thread.start_new_thread(ui_code, ())
	
except:
	print 'Failed'
	
while 1:
	pass
	
