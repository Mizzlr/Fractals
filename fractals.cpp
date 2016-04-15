#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#define WIDTH 1100
#define HEIGHT 600

struct Point2
{
	float x;
	float y;
};

struct Line{
	struct Point2 entry;
	struct Point2 exit;
};
struct Line_data
{
	struct Line parent;
	struct Line children[10];
	struct Line outline[10];
	int number_of_children;
	int number_of_outline;
} line_data;

struct Line first_generation_outlines[100];
struct Line second_generation_outlines[1000];
struct Line third_generation_outlines[10000];

float start_line[2];
float stop_line[2];
void display();
char drawing_mode = 'A';

void mouse(int button, int state, int Mx, int My) 
{
	static int first = 0;
	printf("Mx = %d, My = %d\n",Mx,My);
	if(first == 0)
	{
		line_data.number_of_children = 0;
		line_data.number_of_outline = 0;
	}
	if(first == 2)
		drawing_mode = 'B';
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			start_line[0] = (GLfloat) Mx;
			start_line[1] = (GLfloat) (HEIGHT - My);
		}
		if(state == GLUT_UP)
		{	
			stop_line[0] = (GLfloat) Mx;
			stop_line[1] = (GLfloat) (HEIGHT - My);
			display();
		}
		if(first == 0){
			line_data.parent.entry.x = start_line[0];
			line_data.parent.entry.y = start_line[1];
		}
		if(first == 1){
			line_data.parent.exit.x = stop_line[0];
			line_data.parent.exit.y = stop_line[1];
		}
		if(first > 1 && first % 2 == 0 && drawing_mode == 'B')// entry point of a child
		{
			line_data.children[line_data.number_of_children].entry.x = start_line[0];
			line_data.children[line_data.number_of_children].entry.y = start_line[1];
		}
		if(first > 1 && first % 2 == 1 && drawing_mode == 'B')// exit point of a child
		{
			line_data.children[line_data.number_of_children].exit.x = stop_line[0];
			line_data.children[line_data.number_of_children].exit.y = stop_line[1];
			line_data.number_of_children ++;
		}
		if(first > 1 && first % 2 == 0 && drawing_mode == 'C')// entry point of a outline
		{
			line_data.outline[line_data.number_of_outline].entry.x = start_line[0];
			line_data.outline[line_data.number_of_outline].entry.y = start_line[1];
		}
		if(first > 1 && first % 2 == 1 && drawing_mode == 'C')// exit point of a outline
		{
			line_data.outline[line_data.number_of_outline].exit.x = stop_line[0];
			line_data.outline[line_data.number_of_outline].exit.y = stop_line[1];
			line_data.number_of_outline ++;
		}
	}	
	first++;
}
void myinit()
{

	glViewport(0, 0,WIDTH,HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity( );
	gluOrtho2D(0,WIDTH - 1,0,HEIGHT - 1);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}
void display()
{
	static int change_of_drawing_mode = 1;
	printf("In Display, ");
	printf("start (%f,%f) stop (%f,%f)\n" ,start_line[0],start_line[1],stop_line[0],stop_line[1]);

	if(drawing_mode == 'A')
		glLineWidth(4);
	else if(drawing_mode == 'B')
		glLineWidth(2);
	else if(drawing_mode == 'C' && change_of_drawing_mode){
		glLineWidth(3);
		glClear(GL_COLOR_BUFFER_BIT);
		change_of_drawing_mode = 0;
	}

	glBegin(GL_LINES);
	if(drawing_mode == 'A')
		glColor3f(1,0,0);
	else if(drawing_mode == 'B')
		glColor3f(1,1,0);
	else if(drawing_mode == 'C')
		glColor3f(0,1,0);
	else glColor3f(0.5,0,1);
	
	glVertex3f(start_line[0],start_line[1],0);
	glVertex3f(stop_line[0],stop_line[1],0);
	
	glEnd();
	glFlush();
	glutSwapBuffers();
}

void print_the_input_line_data()
{
	printf("Number_of_children = %d \n",line_data.number_of_children);
	printf("Number_of_outline = %d \n",line_data.number_of_outline);
	printf("Parent: (%f,%f) (%f,%f)\n",line_data.parent.entry.x,line_data.parent.entry.y,line_data.parent.exit.x,line_data.parent.exit.y);
	for(int child = 0; child < line_data.number_of_children; child ++)
		printf("Child %d: (%f,%f) (%f,%f)\n",child,line_data.children[child].entry.x,line_data.children[child].entry.y,line_data.children[child].exit.x,line_data.children[child].exit.y);
	for(int outline = 0; outline < line_data.number_of_outline; outline ++)
		printf("Outline %d: (%f,%f) (%f,%f)\n",outline,line_data.outline[outline].entry.x,line_data.outline[outline].entry.y,line_data.outline[outline].exit.x,line_data.outline[outline].exit.y);
}

struct Line translate(struct Line line, struct Point2 point)
{
	struct Line translated_line = line;
	translated_line.entry.x += point.x;
	translated_line.entry.y += point.y;
	translated_line.exit.x += point.x;
	translated_line.exit.y += point.y;
	return translated_line;
}

struct Line rotate(struct Line line, float cos_theta,int plane)
{
	struct Line rotated_line;
	float sin_theta = sqrt(1 - cos_theta * cos_theta);
	if(plane) sin_theta = - sin_theta; // change the sign in the lower plane
	rotated_line.entry.x = cos_theta * line.entry.x - sin_theta * line.entry.y;
	rotated_line.entry.y = sin_theta * line.entry.x + cos_theta * line.entry.y;
	rotated_line.exit.x = cos_theta * line.exit.x - sin_theta * line.exit.y;
	rotated_line.exit.y = sin_theta * line.exit.x + cos_theta * line.exit.y;
	return rotated_line;
}

struct Line scale(struct Line line, float scaling_factor)
{
	struct Line scaled_line;
	scaled_line.entry.x = line.entry.x * scaling_factor;
	scaled_line.entry.y = line.entry.y * scaling_factor;
	scaled_line.exit.x = line.exit.x * scaling_factor;
	scaled_line.exit.y = line.exit.y * scaling_factor;
	return scaled_line;
}

float magnitude(struct Point2 vector)
{
	float mag;
	mag = vector.x * vector.x + vector.y * vector.y;
	mag = sqrt(mag);
	return mag;
}

float compute_cos_theta(struct Line line1, struct Line line2)
{ 
	struct Point2 vector1;
	struct Point2 vector2;

	vector1.x = line1.exit.x - line1.entry.x;
	vector1.y = line1.exit.y - line1.entry.y;
	vector2.x = line2.exit.x - line2.entry.x;
	vector2.y = line2.exit.y - line2.entry.y;

	float dot_product,cos_theta;
	dot_product = vector1.x * vector2.x + vector1.y * vector2.y;
	cos_theta = dot_product / (magnitude(vector1) * magnitude(vector2));
	printf("angle theta = %f \n", acos(cos_theta) * 180 / 3.14159);
	return cos_theta;
}

int compute_plane(struct Line line)
{
	if(line.entry.y > line.exit.y)
		return 1;
	else 
		return 0;
}


float compute_scaling_factor(struct Line src_line, struct Line dest_line)
{
	struct Point2 src_vector;
	struct Point2 dest_vector;

	src_vector.x = src_line.exit.x - src_line.entry.x;
	src_vector.y = src_line.exit.y - src_line.entry.y;
	dest_vector.x = dest_line.exit.x - dest_line.entry.x;
	dest_vector.y = dest_line.exit.y - dest_line.entry.y;

	float scaling_factor;
	scaling_factor = magnitude(dest_vector) / magnitude(src_vector);
	return scaling_factor;
}

void draw_line(struct Line line)
{
	glBegin(GL_LINES);
		glVertex2f(line.entry.x,line.entry.y);
		glVertex2f(line.exit.x,line.exit.y);
	glEnd();
}

void draw_lines(struct Line *lines, int number_of_lines)
{
	int line_index = 0;
	for(;line_index < number_of_lines; line_index++)
		draw_line(lines[line_index]);
}


void compute_children_for(struct Line outline, struct Line outline_children[])
{
	float cos_theta;
	float scaling_factor;

	cos_theta = compute_cos_theta(line_data.parent,outline);
	scaling_factor = compute_scaling_factor(line_data.parent,outline);


	struct Point2 origin_to_parent_entry;
	struct Point2 origin_to_outline_entry;

	origin_to_parent_entry.x = - line_data.parent.entry.x;
	origin_to_parent_entry.y = - line_data.parent.entry.y;
	origin_to_outline_entry.x = outline.entry.x;
	origin_to_outline_entry.y = outline.entry.y;

	int child;
	int plane = compute_plane(outline);

	for(child = 0; child < line_data.number_of_children ; child ++ )
	{
		outline_children[child] = translate(line_data.children[child],origin_to_parent_entry);
		outline_children[child] = scale(outline_children[child],scaling_factor);
		outline_children[child] = rotate(outline_children[child],cos_theta,plane);
		outline_children[child] = translate(outline_children[child],origin_to_outline_entry);
	}
	//return outline_children;
}

void process_the_line_data(int generations)
{
	print_the_input_line_data();
	int outline_index,child_index;
	struct Line outline_children[10];
	int noutline = line_data.number_of_outline;
	int nchild = line_data.number_of_children;
	
	for(outline_index = 0; outline_index < noutline ; outline_index ++)
	{
		compute_children_for(line_data.outline[outline_index],outline_children);
		for(child_index = 0; child_index < nchild; child_index ++)
			first_generation_outlines[outline_index * noutline + child_index] = outline_children[child_index];
	}

	for(outline_index = 0; outline_index < noutline * nchild ; outline_index ++)
	{
		compute_children_for(first_generation_outlines[outline_index],outline_children);
		for(child_index = 0; child_index < nchild; child_index ++)
			second_generation_outlines[outline_index * noutline * nchild + child_index] = outline_children[child_index];
	}

	for(outline_index = 0; outline_index < noutline * nchild * nchild; outline_index ++)
	{
		compute_children_for(second_generation_outlines[outline_index],outline_children);
		for(child_index = 0; child_index < nchild; child_index ++)
			third_generation_outlines[outline_index * noutline * nchild * nchild + child_index] = outline_children[child_index];
	}

	switch(generations)
	{
		case 1: 
			glClear(GL_COLOR_BUFFER_BIT);
			draw_lines(first_generation_outlines,noutline * nchild);
			break;
		case 2:
			glClear(GL_COLOR_BUFFER_BIT);
			draw_lines(second_generation_outlines,noutline * nchild * nchild);
			break;
		case 3:
			glClear(GL_COLOR_BUFFER_BIT);
			draw_lines(third_generation_outlines,noutline * nchild * nchild * nchild);
			break;
	}
	display();
} 
void keyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
	if (key == 'c') drawing_mode = 'C';
	if (key == '1' || key == '2' || key == '3') process_the_line_data((int)(key - '0'));
	if (key == 'p') print_the_input_line_data();
}
int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH,HEIGHT);
	printf("Initialized window of size %d x %d",WIDTH,HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glPointSize(50);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Fractals");
	myinit();
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display); 
	glutMainLoop();
	return 0;
}
