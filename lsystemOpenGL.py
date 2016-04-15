from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import math

speed = 0.25 # this is 250 milli seconds, set the animation speed as required.

class Turtle:

	def __init__(self,posx,posy,angle = 0):
		self.posx = posx
		self.posy = posy
		self.angle = angle
		self.oldx = posx
		self.oldy = posy
		self.pen_state_down = True

	def forward(self,length):
		self.oldx = self.posx
		self.oldy = self.posy
		self.posx = self.posx + round(length * math.cos(math.radians(self.angle)))
		self.posy = self.posy + round(length * math.sin(math.radians(self.angle)))
		draw_line(self)
	
	def backward(self,length):
		self.oldx = self.posx
		self.oldy = self.posy
		self.posx = self.posx - round(length * math.cos(math.radians(self.angle)))
		self.posy = self.posy - round(length * math.sin(math.radians(self.angle)))
		draw_line(self)

	def right(self,angle):
		self.angle = self.angle - angle
		#print 'ANGLE = ', self.angle

	def left(self,angle):
		self.angle = self.angle + angle
		#print 'ANGLE = ', self.angle

	def seth(self,angle):
		self.angle = angle

	def goto(self,position):
		self.posx,self.posy = position
		self.oldx,self.oldy = position

	def setpos(self,posx,posy):
		self.goto((posx,posy))

	def penup(self):
		self.pen_state_down = False

	def pendown(self):
		self.pen_state_down = True

	def pos(self):
		return (self.posx,self.posy)

	def heading(self):
		return self.angle



class Lsystem:
	def __init__(self,filename = None):
		self.name = ''
		self.variables = []
		self.constants = []
		self.axiom = ''
		self.rules = []
		self.angle = None
		self.length = 2
		self.generation = []
		self.depth = 3
		if filename is not None:
			self.file_number = filenames.index(filename)
		else:
			self.file_number = 3 
	
	def __repr__(self):
		print 'name:',self.name
		print 'variables:',self.variables
		print 'constants:',self.constants
		print 'axiom:',self.axiom
		print 'rules:',self.rules
		print 'angle:',self.angle

	def get_input(self,filename):
		file1 = open(filename,'r')
		lines = file1.readlines()
		self.name = filename.split('.')[0]
		self.variables = []
		self.constants = []
		self.axiom = ''
		self.rules = []
		self.angle = None
		self.generation = []		
		for line in lines:
			split_line = line.split()
			if (split_line[0] == 'variables:'):
				self.variables = split_line[1:]
			elif (split_line[0] == 'constants:'):
				if split_line[1] != 'none':
					self.constants = split_line[1:]
			elif (split_line[0] in ['axiom:','start:']):
				self.axiom = split_line[1]
			elif (split_line[0] == 'rules:'):
				for elem in split_line[1:]:
					elem = elem[1:-1]
					a,b = elem.split('->')
					self.rules.append((a,b))
			elif (split_line[0] == 'angle:'):
				self.angle = int(split_line[1])
			else:
				print 'Error: Unknown line:',line


	def generate(self,number_of_generations):
		self.generation.append(self.axiom)
		for i in range(number_of_generations):
			self.generation.append(self.expand(self.generation[i]))

	def expand(self,string):
		expanded_string = ''
		for character in string:
			if character in self.constants:
				expanded_string += character
			elif character in self.variables:
				for rule in self.rules:
					if rule[0] == character:
						expanded_string += rule[1]
						break
			else:
				print 'Error: Unknown character',character,' in string', string
		return expanded_string	

	def print_generations(self):
		for i in range(len(self.generation)):
			print '[',i,']',self.generation[i]

	def get_generation(self,generation_number):
		self.generate(generation_number)
		return self.generation[generation_number - 1]

	def draw(self):
		code = self.get_generation(self.depth)
		length = self.length
		angle = self.angle
		stack = []
		tt = Turtle(0,0,0)
		tt.penup()
		#tt.setpos(-100,200)
		tt.seth(90)
		tt.pendown()
		for codebit in code:
			if codebit in ['F','A','B']:
				tt.forward(length)
			elif codebit == '+':
				tt.right(angle)
			elif codebit == '-':
				tt.left(angle)
			elif codebit == '[':
				stack.append((tt.pos(),tt.heading()))
			elif codebit == ']':
				position,heading = stack.pop()
				tt.penup()
				tt.goto(position)
				tt.seth(heading)
				tt.pendown()
			else:
				pass
	def depthincr(self):
		self.depth += 1

	def depthdecr(self):
		if self.depth != 0:
			self.depth -= 1

	def lengthincr(self):
		self.length *= 2

	def lengthdecr(self):
		if self.length != 1:
			self.length = int(self.length / 2)		

	def fileincr(self):
		if self.file_number < len(filenames) - 1:
			self.file_number += 1
		self.length = 2
		self.depth = 3

	def filedecr(self):
		if self.file_number != 0:
			self.file_number -= 1
		self.length = 2
		self.depth = 3

	def get_file_number(self):
		return self.file_number

	def get_depth(self):
		return self.depth

	def get_name(self):
		return self.name
filenames = [
'bushytree.txt',
'conifer.txt',
'crystal.txt',
'dragoncurve.txt',
'gospercurve.txt',
'hexagonalkolam.txt',
'hilbertcurve.txt',
'kochcurve.txt',
'kochcurve2.txt',
'kochcurve3.txt',
'mangokolam.txt',
'peanocurve.txt',
'peanogosper.txt',
'penrosetiling.txt',
'plant.txt',
'plant2.txt',
'pythagoriantree.txt',
'quadsnowflake.txt',
'sierpenskitriangle.txt',
'sierpenskitriangle2.txt',
'tiles.txt',
'twiggytree.txt']


def draw_line(tt):
	#print 'TURTLE START = ',(tt.oldx,tt.oldy)
	#print 'TURTLE  STOP = ',(tt.posx,tt.posy)
	glBegin(GL_LINES)
	glVertex2f(tt.oldx,tt.oldy)
	glVertex2f(tt.posx,tt.posy)
	glEnd()
	glFlush()
	glutSwapBuffers()               
	#time.sleep(speed)


def keyboard(key,x,y):
	# if key == 'd':
	# 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) # clear the screen
	# 	lsystem = Lsystem()
	# 	lsystem.get_input(sys.argv[1])
	# 	lsystem
	# 	print 'Code',lsystem.get_generation(int(sys.argv[2]))
	# 	lsystem.draw()
	pass
	
def special(key,x,y):
	if key == GLUT_KEY_RIGHT: 
		lsystem.depthincr()
	elif key == GLUT_KEY_LEFT:
		lsystem.depthdecr()
	elif key == GLUT_KEY_UP:
		lsystem.lengthincr()
	elif key == GLUT_KEY_DOWN:
		lsystem.lengthdecr()
	elif key == GLUT_KEY_F2:
		lsystem.fileincr()
	elif key == GLUT_KEY_F1:
		lsystem.filedecr()
	else:
		pass
	#print 'file_number',lsystem.get_file_number()
	lsystem.get_input(filenames[lsystem.get_file_number()])
	lsystem
	print 'Code ',lsystem.get_name(),'[',lsystem.get_depth(),'] :',lsystem.get_generation(lsystem.get_depth())
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) # clear the screen
	lsystem.draw()

def draw(): 
	pass     # ondraw is called all the time
                              

def myinit():
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
	gluOrtho2D(-400,400,-400,400)
	glMatrixMode(GL_MODELVIEW)
	glLoadIdentity()
	
def main():
	window = 0                                             # glut window number
	width, height = 800, 800                               # window size
	glutInit()                                             # initialize glut
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH)
	glutInitWindowSize(width, height)                      # set window siz
	glutInitWindowPosition(200, 200)                           # set window position
	window = glutCreateWindow("L-Systems and Fractals")         # create window with title
	glutDisplayFunc(draw)     
	glutKeyboardFunc(keyboard)      
	glutSpecialFunc(special)	
	glutIdleFunc(draw)              
	myinit()
	glutMainLoop() # start everything

lsystem = Lsystem()
if __name__ == '__main__':
	main()
