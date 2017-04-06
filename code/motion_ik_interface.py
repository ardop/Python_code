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

#Point values
point = [0, 0, 0]

point_history = []

selected_motion = ""


class Motion_IK_Interface(QDialog):
    
	def __init__(self, parent = None):
	    
		super(Motion_IK_Interface,self).__init__(parent)
		
		pointLabel = QLabel('Point Coordinates')
		motionLabel = QLabel('Motion Type ')
		xLabel = QLabel('x ')
		yLabel = QLabel('y ')
		zLabel = QLabel('z ')
		
		#Initializing widgets
		
		self.addPointButton = QPushButton('Add Point')

		self.selectMotion = QComboBox()
		self.selectMotion.addItems(['Linear', 'Quadratic'])

		self.pointHistoryScroll = QScrollArea()
		self.pointHistoryScroll.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
	 	self.pointHistoryScroll.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
	 	self.pointHistoryScroll.setWidgetResizable(True)
	 	self.pointHistoryString = QLabel('')
		self.xTargetText = QLineEdit()
		self.xTargetText.setFixedSize(100, 20)
		self.yTargetText = QLineEdit()
		self.yTargetText.setFixedSize(100, 20)
		self.zTargetText = QLineEdit()
		self.zTargetText.setFixedSize(100, 20)

		self.pointHistoryScroll.setWidget(self.pointHistoryString)
		
		#Grid and widget locations
		
		grid = QGridLayout()

		grid.addWidget(motionLabel, 0, 0)
		grid.addWidget(self.selectMotion, 0, 1)
		grid.addWidget(pointLabel, 1, 0)
		grid.addWidget(xLabel, 2, 0)
		grid.addWidget(yLabel, 3, 0)
		grid.addWidget(zLabel, 4, 0)
		grid.addWidget(self.xTargetText, 2, 1)
		grid.addWidget(self.yTargetText, 3, 1)
		grid.addWidget(self.zTargetText, 4, 1)
		grid.addWidget(self.addPointButton, 5, 0)
		grid.addWidget(self.pointHistoryScroll, 6, 0)

		self.connect(self.addPointButton, SIGNAL("clicked()"), self.goToPoint)	
		# self.selectMotion.currentIndexChanged.connect(self.selectMotionChange)

		self.setLayout(grid)
		self.setWindowTitle("Motion Inverse Kinematics")
		self.setGeometry(500,300,400,100)

	def selectMotionChange(self, i):
		
		selected_motion = self.selectMotion.currentText()
		print selected_motion



	def goToPoint(self):
		
		global point
		
		point[0] = str(self.xTargetText.text())
		point[1] = str(self.yTargetText.text())
		point[2] = str(self.zTargetText.text())

		if(point[0]=="" or point[1] == "" or point[2] == ""):

			print 'Please enter the point values'

		else:
		
			point = map(float, point)
			
			file = open('motion_ik_angles.txt', 'wb')
			for t in point:
				file.write(str(t) + '\n')
			
			
			file.close()
			
			print "Written"

			#Updating point history
			point_history.append("(" + str(point[0]) + ", " + str(point[1]) + ", " + str(point[2]) + ")")
			point_history_string = " --> ".join(map(str, point_history))
			
			# set in label
			self.pointHistoryString.setText(point_history_string)




		
def cpp_code():
	
	#os.system('gnome-terminal')
	#os.system('gnome-terminal -e cd ../build && echo ubuntu | sudo -S ./ik_interface && cd ../code')
	#os.system("gnome-terminal -e --working-directory=/home/ubuntu/ardop/kinematics/build -e 'sudo ./ik_interface'")
	os.system('xterm -hold -e "cd ../build && echo ubuntu | sudo -S ./ik_interface && cd ../code"')
		

def ui_code():
	app = QApplication(sys.argv)
	motor = Motion_IK_Interface()
	motor.show()
	app.exec_()
	
	       
try:
	# thread.start_new_thread(cpp_code, ())
	thread.start_new_thread(ui_code, ())
	
except:
	print 'Failed'
	
while 1:
	pass
	
	
	   



