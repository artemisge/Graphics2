#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#define M_PI 3.14159265358979323846
using namespace std;

// constantly changing angle
GLfloat angle = 0, mult = 1;

// AEM:3374 mod 3 = 2
GLfloat a = 8, b = 90, xyz[3] = {1, 2, 6};

void myinit()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST); // to see back of the cube, has to do with 3Dness
	glClearColor(0, 0, 0, 1); 
	glOrtho(-30.0, 30.0, -30.0, 30.0, 0, b+20); // αν ειναι αρνητικά, πάει πίσω από τον παρατηρητή
	// nearVal, farVal documentation
    //Specify the distances to the nearer and farther depth clipping planes. These values are negative if the plane is to be behind the viewer. 

	glMatrixMode(GL_MODELVIEW); // peirazoume ton xoro kai ta simeia tora, oxi tin kamera

}

float step = 0.05;
void spinResize() {
	angle += 0.45;
	if (angle > 360) {
		angle = 0;
	}
	//printf("%.2f ", mult);
	glutPostRedisplay();
	mult += step;
	if (mult >= 2.99) {
		step = -0.05;
	}
	if (mult <= 1.01) {
		step = 0.05;
	}
}

// sxhmatizei thn mia pleura tou kubou apo ta 4 vertices pou dinoume os orisma
void cubeFace(GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[]){
	glBegin(GL_POLYGON);
		// Vertex: 3D, float, array
		glVertex3fv(V0); 
		glVertex3fv(V1);
		glVertex3fv(V2);
		glVertex3fv(V3);
	glEnd(); 
}

GLfloat initialSquare[4][3] = {	{-1.0, 1.0, 1},
								{1.0, 1.0, 1},
								{1.0, -1.0, 1},
								{-1.0, -1.0, 1}};

int menuoption = 0; // default rotate cube center
void display() 
{	
	// Δημιουργία τετραγώνου πλευράς 2 πάνω στο επίπεδο Z=1
	glNewList(1, GL_COMPILE);
		cubeFace(initialSquare[0], initialSquare[1], initialSquare[2], initialSquare[3]);
	glEndList();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch(menuoption) {
		case 0:
			// cube's center is on (0, 0, b)
			// rotate around b-center
			glLoadIdentity();
			glTranslatef(0, 0, -b);
			glScalef(a/2*mult, a/2*mult, 1);
			glRotatef(angle, xyz[0], xyz[1], xyz[2]);
			break;
		case 1:
			// rotate around -8*b/10
			glLoadIdentity();	
			glTranslatef(0, 0, -8*b/10);
			
			glRotatef(angle, xyz[0], xyz[1], xyz[2]);
			glTranslatef(0, 0, -20); // aktina peristrofhs
			glScalef(a/2*mult, a/2*mult, a/2*mult);

			// -8*b/10 point, to see it visually
			glPushMatrix();
				glLoadIdentity();
				glColor3f(1,1,1);
				glPointSize(10);
				glBegin(GL_POINTS);
					// Vertex: 3D, float, array
					glVertex3f(0, 0, -8*b/10); 
				glEnd(); 
			glPopMatrix();
			break;
		}
	

	// πλευρά 1
	glPushMatrix();
		glColor3f(1,0,0);
		glCallList(1);
	glPopMatrix();

	// πλευρά 2
	glPushMatrix();
		glColor3f(1,1,0);
		glRotatef(90, 0, 1, 0); // 90moires guro apo ton y
		glCallList(1);
	glPopMatrix();

	// πλευρά 3
	glPushMatrix();
		glColor3f(0,1,0);
		glRotatef(-90, 0, 1, 0); // 90moires guro apo ton y
		glCallList(1);
	glPopMatrix();

	// πλευρά 4
	glPushMatrix();
		glColor3f(0,0,1);
		glRotatef(90, 1, 0, 0); // 90moires guro apo ton y
		glCallList(1);
	glPopMatrix();

	// πλευρά 5
	glPushMatrix();
		glColor3f(0,1,1);
		glRotatef(-90, 1, 0, 0); // 90moires guro apo ton y
		glCallList(1);
	glPopMatrix();

	// πλευρά 6
	glPushMatrix();
		glColor3f(1,0,1);
		glRotatef(-180, 1, 0, 0); // 180moires guro apo ton y
		glCallList(1);
	glPopMatrix();

	glutSwapBuffers();
	//glFlush(); /* clear buffers */
}

void menu(int option) {
	switch (option) {
        case 0:
			menuoption = 0;
			break;
        case 1:
			menuoption = 1;
			break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 
	glutInitWindowSize(800, 800);				
	glutInitWindowPosition(50, 150);				
	glutCreateWindow("Cube spin");			 
	myinit();									
	glutDisplayFunc(display);
	glutIdleFunc(spinResize);	// called after display is finished	

	glutCreateMenu(menu);
	glutAddMenuEntry("Rotate center", 0);
    glutAddMenuEntry("Rotate vector", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop(); /* enter event loop */
	return 0;
}

//ModelView matrix is the concatenation of Model matrix and 
//View Matrix. View Matrix defines the position(location and orientation) 
//of the camera, while model matrix defines the frame’s position 
//of the primitives you are going to draw.
//Projection matrix defines the characteristics of your 
//camera, such as clip planes, field of view, projection method etc.