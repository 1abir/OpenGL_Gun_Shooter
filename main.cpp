#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <glut.h>

#include <vector>

#include <custom/camera.h>

using namespace std;

#define BIG_RADIUS 15
#define SMALL_RADIUS 5
#define RANGE 50
#define STEP 5
#define CILYNDER_LENGTH 50
#define PLANE_SIDE 50
#define PLANE_OFFSET (BIG_RADIUS + SMALL_RADIUS + CILYNDER_LENGTH * 2)

Point3D axis, all_axis, cily_axis, plane_normal;
vector<Point3D> intersections;

float rotateCylinderAngle = 0;
float rotate_all_angle = 0;

const float rotate_all_range = RANGE;

float rotate_hemi_angle = 0;
const float rotate_hemi_range = RANGE;

float rotate_cily_angle = 0;
const float rotate_cily_range = RANGE;

double cameraHeight;
double cameraAngle;
int drawgrid=1;
int drawaxes=1;
double angle;

struct point
{
	double x, y, z;
};

void drawAxes()
{
	if (drawaxes == 1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f(100, 0, 0);
			glVertex3f(-100, 0, 0);

			glVertex3f(0, -100, 0);
			glVertex3f(0, 100, 0);

			glVertex3f(0, 0, 100);
			glVertex3f(0, 0, -100);
		}
		glEnd();
	}
}

void drawGrid()
{
	int i;
	if (drawgrid == 1)
	{
		glColor3f(0.6, 0.6, 0.6); //grey
		glBegin(GL_LINES);
		{
			for (i = -8; i <= 8; i++)
			{

				if (i == 0)
					continue; //SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i * 10, -90, 0);
				glVertex3f(i * 10, 90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i * 10, 0);
				glVertex3f(90, i * 10, 0);
			}
		}
		glEnd();
	}
}

void drawSquare(double a)
{
	//glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);
	{
		glVertex3f(a, a, 2);
		glVertex3f(a, -a, 2);
		glVertex3f(-a, -a, 2);
		glVertex3f(-a, a, 2);
	}
	glEnd();
}

void drawCircle(double radius, int segments)
{
	int i;
	struct point points[100];
	glColor3f(1,0,0);
	//generate points
	for (i = 0; i <= segments; i++)
	{
		points[i].x = radius * cos(((double)i / (double)segments) * 2 * pi);
		points[i].y = radius * sin(((double)i / (double)segments) * 2 * pi);
	}
	//draw segments using generated points
	for (i = 0; i < segments; i++)
	{
		glBegin(GL_LINES);
		{
			glVertex3f(points[i].x, points[i].y, 0);
			glVertex3f(points[i + 1].x, points[i + 1].y, 0);
		}
		glEnd();
	}
}

void drawCone(double radius, double height, int segments)
{
	int i;
	double shade;
	struct point points[100];
	//generate points
	for (i = 0; i <= segments; i++)
	{
		points[i].x = radius * cos(((double)i / (double)segments) * 2 * pi);
		points[i].y = radius * sin(((double)i / (double)segments) * 2 * pi);
	}
	//draw triangles using generated points
	for (i = 0; i < segments; i++)
	{
		//create shading effect
		if (i < segments / 2)
			shade = 2 * (double)i / (double)segments;
		else
			shade = 2 * (1.0 - (double)i / (double)segments);
		glColor3f(shade, shade, shade);

		glBegin(GL_TRIANGLES);
		{
			glVertex3f(0, 0, height);
			glVertex3f(points[i].x, points[i].y, 0);
			glVertex3f(points[i + 1].x, points[i + 1].y, 0);
		}
		glEnd();
	}
}

void drawHemisphere(double radius, int slices, int stacks, int upper_hemisphere)
{
	struct point points[100][100];
	int i, j, shade;
	double h, r;
	//generate points
	for (i = 0; i <= stacks; i++)
	{
		h = radius * sin(((double)i / (double)stacks) * (pi / 2));
		r = radius * cos(((double)i / (double)stacks) * (pi / 2));
		for (j = 0; j <= slices; j++)
		{
			points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
			points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
			points[i][j].z = h;
		}
	}
	//draw quads using generated points
	for (i = 0; i < stacks; i++)
	{
		for (j = 0; j < slices; j++)
		{
			if (j % 2)
				shade = 0;
			else
				shade = 1;
			glColor3f(shade, shade, shade);
			glBegin(GL_QUADS);
			{
				//upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, upper_hemisphere * points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, upper_hemisphere * points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, upper_hemisphere * points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, upper_hemisphere * points[i + 1][j].z);
			}
			glEnd();
		}
	}
}

void drawInverseHemisphere(double radius, int slices, int stacks, int upper_hemisphere)
{
	struct point points[100][100];
	int i, j, shade;
	double h, r;
	//generate points
	for (i = 0; i <= stacks; i++)
	{
		h = radius * sin(((double)i / (double)stacks) * (pi / 2));
		r = radius * cos(((double)i / (double)stacks) * (pi / 2));
		r = 2*radius - r;
		for (j = 0; j <= slices; j++)
		{
			points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
			points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
			points[i][j].z = h;
		}
	}
	//draw quads using generated points
	for (i = 0; i < stacks; i++)
	{
		for (j = 0; j < slices; j++)
		{
			if (j % 2)
				shade = 0;
			else
				shade = 1;
			glColor3f(shade, shade, shade);
			glBegin(GL_QUADS);
			{
				//upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, upper_hemisphere * points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, upper_hemisphere * points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, upper_hemisphere * points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, upper_hemisphere * points[i + 1][j].z);
			}
			glEnd();
		}
	}
}

void drawCylinder(float radius, float height, int segments)
{
	int i;
	float shade;
	Point3D points[100];
	
	for (i = 0; i <= segments; i++)
	{
		points[i].x = radius * cos(((double)i / (double)segments) * pi * 2);
		points[i].y = radius * sin(((double)i / (double)segments) * pi * 2);
	}
	glColor3f(0, 1.0, 0);
	
	for (i = 0; i < segments; i++)
	{
		if (i % 2)
			shade = 0;
		else
			shade = 1;
		glColor3f(shade, shade, shade);

		glBegin(GL_QUADS);
		{
			glVertex3f(points[i].x, points[i].y, 0);
			glVertex3f(points[i + 1].x, points[i + 1].y, 0);
			glVertex3f(points[i + 1].x, points[i + 1].y, height);
			glVertex3f(points[i].x, points[i].y, height);
		}
		glEnd();
	}
}

void drawPlane(float a, float offset)
{
	a = abs(a);
	glColor3f(0.3, .3, .3);
	glBegin(GL_QUADS);
	glVertex3f(a, a, offset);
	glVertex3f(-a, a, offset);
	glVertex3f(-a, -a, offset);
	glVertex3f(a, -a, offset);
	glEnd();
}

void drawPoint(float a,float b, float offset)
{
	glColor3f(1,0,0);
	float width_by_4 = 1;
	glBegin(GL_QUADS);
	glVertex3f(a+width_by_4, b+width_by_4, offset);
	glVertex3f(a-width_by_4, b+width_by_4, offset);
	glVertex3f(a+width_by_4, b-width_by_4, offset);
	glVertex3f(a-width_by_4, b-width_by_4, offset);
	glEnd();
}

void drawBulletMarks()
{
	// glPushMatrix();
	// {
	// 	glTranslatef(0, 0, PLANE_OFFSET);
		glColor3f(1, 0, 0);

		for (auto i = intersections.begin(); i < intersections.end(); i++)
		{
			Point3D temp = *i;
			drawPoint(temp.x,temp.y,temp.z-2);
		}
	// }
	// glPopMatrix();
}

Point3D rp = Point3D(0.0, 0.0, 0.0);
Point3D pp = Point3D(0.0, 0.0, PLANE_OFFSET);

void trigger()
{
	Point3D rv = axis;
	Point3D ip = intersectPoint(rv, rp, plane_normal, pp);
	if ((ip.x <= PLANE_SIDE && ip.x >= -1*PLANE_SIDE && ip.y <= PLANE_SIDE && ip.y >= -1*PLANE_SIDE && ip.z == PLANE_OFFSET))
	{
		cout << ip << endl;
		intersections.push_back(ip);
	}
}

void drawGun()
{


	drawPlane(PLANE_SIDE, PLANE_OFFSET);
	drawBulletMarks();

	glRotatef(rotate_all_angle, all_axis.x, all_axis.y, all_axis.z);

	drawHemisphere(BIG_RADIUS, 24, 20, -1);

	glRotatef(rotate_hemi_angle, cily_axis.x, cily_axis.y, cily_axis.z);

	drawHemisphere(BIG_RADIUS, 24, 20, 1);

	glTranslatef(0, 0, BIG_RADIUS + SMALL_RADIUS);

	glRotatef(rotate_cily_angle, cily_axis.x, cily_axis.y, cily_axis.z);

	glRotatef(rotateCylinderAngle, 0, 0, 1);

	drawHemisphere(SMALL_RADIUS, 20, 10, -1);
	drawCylinder(SMALL_RADIUS, CILYNDER_LENGTH, 20);
	glTranslatef(0, 0, CILYNDER_LENGTH);
	drawInverseHemisphere(SMALL_RADIUS, 20, 10, 1);

}

void keyboardListener(unsigned char key, int x, int y)
{
	switch (key)
	{

	case '1':
		rotate_left(3 * UNIT);
		break;
	case '2':
		rotate_right(3 * UNIT);
		break;
	case '3':
		rotate_up(3 * UNIT);
		break;
	case '4':
		rotate_down(3 * UNIT);
		break;
	case '5':
		tilt(3 * UNIT);
		break;
	case '6':
		tilt(-3 * UNIT);
		break;
	case 'd':
		rotateCylinderAngle += STEP;
		break;
	case 'f':
		rotateCylinderAngle -= STEP;
		break;
	case 'q':
		if (rotate_all_angle < rotate_all_range)
		{
			rotate_all_angle += STEP;
			axis = axis.rotate(STEP, all_axis);
		}
		break;
	case 'w':
		if (rotate_all_angle > -1 * rotate_all_range)
		{
			rotate_all_angle -= STEP;
			axis = axis.rotate(-1*STEP, all_axis);
		}
		break;
	case 'e':
		if (rotate_hemi_angle < rotate_hemi_range)
		{
			rotate_hemi_angle += STEP;
			axis = axis.rotate(STEP, cily_axis);
		}
		break;
	case 'r':
		if (rotate_hemi_angle > -1 * rotate_hemi_range)
		{
			rotate_hemi_angle -= STEP;
			axis = axis.rotate(-1*STEP, cily_axis);
		}
		break;
	case 'a':
		if (rotate_cily_angle < rotate_cily_range)
		{
			rotate_cily_angle += STEP;
			axis = axis.rotate(STEP, cily_axis);
		}
		break;
	case 's':
		if (rotate_cily_angle > -1 * rotate_cily_range)
		{
			rotate_cily_angle -= STEP;
			axis = axis.rotate(-1*STEP, cily_axis);
		}
		break;
	default:
		break;
	}
}

void specialKeyListener(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN: //down arrow key
		backward(UNIT);
		break;
	case GLUT_KEY_UP: // up arrow key
		forward(UNIT);
		break;

	case GLUT_KEY_RIGHT:
		right(UNIT);
		break;
	case GLUT_KEY_LEFT:
		left(UNIT);
		break;

	case GLUT_KEY_PAGE_UP:
		up(UNIT);
		break;
	case GLUT_KEY_PAGE_DOWN:
		down(UNIT);
		break;
	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:
		break;
	case GLUT_KEY_END:
		break;
	default:
		break;
	}
}

void mouseListener(int button, int state, int x, int y)
{ //x, y is the x-y of the screen (2D)
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{ // 2 times?? in ONE click? -- solution is checking DOWN or UP
			drawaxes = 1 - drawaxes;
			trigger();
		}
		break;

	case GLUT_RIGHT_BUTTON:
		//........
		if (state == GLUT_DOWN)
			drawgrid = 1-drawgrid;
		break;

	case GLUT_MIDDLE_BUTTON:
		//........
		break;

	default:
		break;
	}
}

void display()
{

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0); //color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	// gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	// gluLookAt(0,0,200,	0,0,0,	0,1,0);

	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

	//glColor3f(1,0,0);
	//drawSquare(10);

	// drawSS();

	//drawCircle(30,24);

	//drawCone(20,50,24);

	//drawSphere(30,24,20);

	// drawHemisphere(30,24,20,1);

	// drawCylinder(20,100,23);

	drawGun();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate()
{
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init()
{
	//codes for initialization
	drawgrid = 1;
	drawaxes = 1;
	cameraHeight = 150.0;
	cameraAngle = 1.0;
	angle = 0;

	//clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80, 1, 1, 1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

	// l = Point3D(-1 / sqrt(2), -1 / sqrt(2), 0);
	// r = Point3D(-1 / sqrt(2), 1 / sqrt(2), 0);
	// u = Point3D(0, 0, 1);
	l = Point3D(1,0,0);
	r = Point3D(0,0,1);
	u = r.cross(l); // 0,-1,0

	// cout<<(u.cross(r)) << l.cross(u)<<endl;

	pos = Point3D(-100, 0, 0);

	Point3D arbitrary(1, 0, 0);
	axis = Point3D(0, 0, 1);
	all_axis = axis.cross(arbitrary).normalized();
	cily_axis = axis.cross(all_axis).normalized();
	plane_normal = axis;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); //Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST); //enable Depth Testing

	glutDisplayFunc(display); //display callback function
	glutIdleFunc(animate);	  //what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop(); //The main loop of OpenGL

	return 0;
}
