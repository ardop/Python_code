import sys 
import os
import thread
from PyQt4.QtGui import *
from PyQt4.QtCore import *


#Default angles
lDefaultAngles = [137, 26, 105, 180, 90]
rDefaultAngles = [53,150,105,0,90]
hDefaultAngles = [90, 90]

#Angles to be written into the file
lAngles = [0, 0, 0, 0, 0]
rAngles = [0, 0, 0, 0, 0]
hAngles = [0, 0]

defaultTarget = []

#Target values
target = [0, 0, 0]


class Motor_Interface(QDialog):
    
    def __init__(self, parent = None):
        
		super(Motor_Interface,self).__init__(parent)
		
		targetLabel = QLabel('Target ')
		xLabel = QLabel('x ')
		yLabel = QLabel('y ')
		zLabel = QLabel('z ')
		
		#Initializing widgets
		
		self.targetButton = QPushButton('Move')
		
		self.xTargetText = QLineEdit()
		self.xTargetText.setFixedSize(80, 20)
		self.yTargetText = QLineEdit()
		self.yTargetText.setFixedSize(80, 20)
		self.zTargetText = QLineEdit()
		self.zTargetText.setFixedSize(80, 20)
		
		
		
		#Grid and widget locations
		
		grid = QGridLayout()

		grid.addWidget(targetLabel, 0, 0)
		grid.addWidget(xLabel, 1, 0)
		grid.addWidget(yLabel, 2, 0)
		grid.addWidget(zLabel, 3, 0)
		grid.addWidget(self.xTargetText, 1, 1)
		grid.addWidget(self.yTargetText, 2, 1)
		grid.addWidget(self.zTargetText, 3, 1)
		grid.addWidget(self.targetButton, 4, 0)
		

		
		
		self.connect(self.targetButton, SIGNAL("clicked()"), self.calc_ik_angles)
		
		
		self.setLayout(grid)
		self.setWindowTitle("Inverse Kinematics")
		self.setGeometry(500,300,400,100)

    def calc_ik_angles(self):
		
		global target
		
		target[0] = str(self.xTargetText.text())
		target[1] = str(self.yTargetText.text())
		target[2] = str(self.zTargetText.text())
		
		target = map(float, target)
		
		file = open('ik_angles.txt', 'wb')
		for t in target:
			file.write(str(t) + '\n')
		
		
		file.close()
		
		print "Written"
		
def cpp_code():
	
	#os.system('gnome-terminal')
	#os.system('gnome-terminal -e cd ../build && echo ubuntu | sudo -S ./ik_interface && cd ../code')
	#os.system("gnome-terminal -e --working-directory=/home/ubuntu/ardop/kinematics/build -e 'sudo ./ik_interface'")
	os.system('xterm -hold -e "cd ../build && echo ubuntu | sudo -S ./ik_interface && cd ../code"')
		

def ui_code():
	app = QApplication(sys.argv)
	motor = Motor_Interface()
	motor.show()
	app.exec_()
	
	       
try:
	thread.start_new_thread(cpp_code, ())
	thread.start_new_thread(ui_code, ())
	
except:
	print 'Failed'
	
while 1:
	pass
	
	
	   



