import pygame
import os
import copy

# Centering the window
os.environ['SDL_VIDEO_CENTERED'] = '1'

# Defining colors
BLACK = (0,0,0)
WHITE = (255, 255, 255)
APP_BACKGROUND = (99, 226, 103)
EYE_BASE = (20, 20, 20)
EYE_CIRCLE_HOVER = (60, 60, 60)
EYE_CIRCLE_CLICKED = (233, 3, 3)
TEXT = (30, 30, 30)

# mouse coordinates
mousex = mousey = -1

# Coordinates of the centres of all the circles in both eyes. They are calculated using
# the rectangle coordinates. They are used to draw the hollow and filled circles
left_cir_coord = [(80+20*(j+1), 115+20*(i+1)) for i in xrange(10) for j in xrange(14)]
right_cir_coord = [(420+20*(j+1), 115+20*(i+1)) for i in xrange(10) for j in xrange(14)]

# Two lists to store the hover state of all the circles. The value corresponding to 
# a particular circle coordinate is 1 if the mouse is hovering over it
left_hover_state = [0]*len(left_cir_coord)
right_hover_state = [0]*len(right_cir_coord)

# Two lists to store the clicked state of all the circles. The value corresponding to 
# a particular cirlce coordinate is 1 if that circle is activated by clicking
left_clicked_state = [0]*len(left_cir_coord)
right_clicked_state = [0]*len(right_cir_coord)


# lists to store the final coordinates
left_coord = []
right_coord = []

def mirror_left_to_right():

	right_clicked_state = []
	for i in xrange(len(left_clicked_state)):

		if left_clicked_state[i]==1:
			pass

			# Mirror to the right
			



def calculate_hover_state(x, y):
	# calculating the hover status and updating the status for left and right eyes
	k=0
	for (i, j) in left_cir_coord:
		d = ((x-i)**2+(y-j)**2)**0.5
		if d<=9:
			left_hover_state[k] = 1
		else:
			left_hover_state[k] = 0
		k+=1
	k=0
	for (i, j) in right_cir_coord:
		d = ((x-i)**2+(y-j)**2)**0.5
		if d<=9:
			right_hover_state[k] = 1
		else:
			right_hover_state[k] = 0
		k+=1

def calculate_clicked_state(x, y):
	# Calculating the clicked status for left and right eyes
	k=0
	for (i, j) in left_cir_coord:
		d = ((x-i)**2+(y-j)**2)**0.5
		if d<=9:
			# alternate between 1 and 0
			left_clicked_state[k] = 1 - left_clicked_state[k]

		k+=1
	k=0
	for (i, j) in right_cir_coord:
		d = ((x-i)**2+(y-j)**2)**0.5
		if d<=9:
			# alternate between 1 and 0
			right_clicked_state[k] = 1 - right_clicked_state[k]

		k+=1

def calculate_coord():

	# Getting the indices for which the state is 1
	left_active = [i for i in range(len(left_clicked_state)) if left_clicked_state[i]==1]
	right_active = [i for i in range(len(right_clicked_state)) if right_clicked_state[i]==1]

	# Now we can calculate the coordinates of the pixels by mapping indices to pixels
	left = []
	right = []

	for i in left_active:
		x = 1+(i%14)
		y = 10-(i/14)
		left.append(x)
		left.append(y)

	for i in right_active:
		x = 1+(i%14)
		y = 10-(i/14)
		right.append(x)
		right.append(y)

	# returning
	return (left, right)

def display_coord():

	global left_coord
	global right_coord

	# Converting for string representation and printing
	left_coord = ", ".join(map(str, left_coord))
	right_coord = ", ".join(map(str, right_coord))
	left_coord = '{'+left_coord+'}'
	right_coord = '{'+right_coord+'}'
	print('Left Eye Pattern:')
	print(left_coord)
	print('Right Eye Pattern:')
	print(right_coord)




# size of the screen
size = [800, 450]
screen = pygame.display.set_mode(size)
pygame.display.set_caption("Pattern Generator v1.2")

pygame.init()

instruction_font = pygame.font.SysFont("courier", 15)
instruction_text1 = instruction_font.render("ENTER: DISPLAY COORDINATES", True, TEXT)
instruction_text2 = instruction_font.render("c+r: COPY TO RIGHT", True, TEXT)
instruction_text3 = instruction_font.render("c+l: COPY TO LEFT", True, TEXT)
instruction_text4 = instruction_font.render("ctrl+r: CLEAR RIGHT", True, TEXT)
instruction_text5 = instruction_font.render("ctrl+l: CLEAR LEFT", True, TEXT)

app_done = False # variable to stop the application loop
clock = pygame.time.Clock() # managing screen update time

def main_draw(screen):
	screen.fill(APP_BACKGROUND)
	draw_base(screen)
	draw_circles(screen)
	draw_hover(screen)
	draw_clicked(screen)

def draw_base(screen):
	# Base rectangles (both eyes) and the instruction text
	pygame.draw.rect(screen, EYE_BASE, [80, 115, 300, 220])
	pygame.draw.rect(screen, EYE_BASE, [420, 115, 300, 220])
	screen.blit(instruction_text1, (90, 10))
	screen.blit(instruction_text2, (90, 25))
	screen.blit(instruction_text3, (90, 40))
	screen.blit(instruction_text4, (90, 55))
	screen.blit(instruction_text5, (90, 70))

def draw_circles(screen):
	# left eye
	for (i, j) in left_cir_coord:
		pygame.draw.circle(screen, EYE_CIRCLE_HOVER, (i, j), 9, 1 )
	# right eye
	for (i, j) in right_cir_coord:
		pygame.draw.circle(screen, EYE_CIRCLE_HOVER, (i, j), 9, 1 )

def draw_hover(screen):
	# Drawing solid grey cirlces for hover
	k = 0
	for i in left_hover_state:
		if i==1:
			(x, y) = left_cir_coord[k]
			pygame.draw.circle(screen, EYE_CIRCLE_HOVER, (x, y), 9)
		k+=1
	k=0
	for i in right_hover_state:
		if i==1:
			(x, y) = right_cir_coord[k]
			pygame.draw.circle(screen, EYE_CIRCLE_HOVER, (x, y), 9)
		k+=1

def draw_clicked(screen):
	# Drawing colored solid circles for clicked
	k = 0
	for i in left_clicked_state:
		if i==1:
			(x, y) = left_cir_coord[k]
			pygame.draw.circle(screen, EYE_CIRCLE_CLICKED, (x, y), 9)
		k+=1
	k=0
	for i in right_clicked_state:
		if i==1:
			(x, y) = right_cir_coord[k]
			pygame.draw.circle(screen, EYE_CIRCLE_CLICKED, (x, y), 9)
		k+=1


# Mouse hold state for drag selecting
mouseHold = False

while not app_done:

	# Obtaining mouse coordinates
	mousex, mousey = pygame.mouse.get_pos();

	mouseClick = False

	# Getting list of all keys pressed for multi key events
	keys = pygame.key.get_pressed()

	# Event processing --------------------------------------------------------------
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			app_done = True # Quits the app on the next iteration
		if event.type == pygame.MOUSEBUTTONDOWN:
			mouseHold = True
			mouseClick = True
		if event.type == pygame.MOUSEBUTTONUP:
			# We calculate the coordinates of the clicked circles
			mouseHold = False
			(left_coord, right_coord) = calculate_coord()



		if event.type==pygame.KEYDOWN:  
		    if event.key == pygame.K_RETURN:

		    	# PRESS ENTER TO DISPLAY THE CHARACTERS
		        display_coord()

		if keys[pygame.K_c] and keys[pygame.K_r] and sum(left_clicked_state)!=0:
			#Copy pattern to the right
			right_clicked_state = copy.copy(left_clicked_state)

		if keys[pygame.K_c] and keys[pygame.K_l] and sum(right_clicked_state)!=0:
			#Copy pattern to the left
			left_clicked_state = copy.copy(right_clicked_state)

		if keys[pygame.K_LCTRL] and keys[pygame.K_r]:
			#Clear right eye
			right_clicked_state = [0]*len(right_cir_coord)

		if keys[pygame.K_LCTRL] and keys[pygame.K_l]:
			#clear left eye
			left_clicked_state = [0]*len(left_cir_coord)








	# We calculate the hover states by calling the function
	calculate_hover_state(mousex, mousey)

	# If there is a mouse click event, we update the clicked state
	if mouseClick:
		calculate_clicked_state(mousex, mousey)

	main_draw(screen)
	pygame.display.flip()
	clock.tick(60)


pygame.quit()