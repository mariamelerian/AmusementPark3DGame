#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>






#define _HAS_STD_TO_STRING 0
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

//timer for screen
int elapsedTime = 0;
const int totalTime = 60;  // 30 seconds

int AnimationElapsedTime = 0;
bool AnimationOn = false;



//if x between 0.8 and 1.1     IF Z BETWEEN -0.25 and -0.3

float PlayerX = 0;//least possible x==-0.1 max x=1.1
float PlayerY = -0.285;
float PlayerZ = 1;
float PlayerRotationAngle;

float TreeX = 0.1;
float TreeY = 0;
float TreeZ = 0.25;
int TreeDirection = 1;  // Initial direction: 1 for increasing, -1 for decreasing
float TreeScaleFactor = 1.0;  // Initial scale factor


float cX = -0.27;
float TeapotX = -0.27;
float TeapotY = 0;
float TeapotZ = 0.4;
float TeapotScaleFactor = 1.0;  // Initial scale factor
int Teapotdirection = 1;  // Initial direction: 1 for increasing, -1 for decreasing


float FerrisWheelX = -2.45;
float FerrisWheelY = -0.05;
float FerrisWheelZ = -1.9;
float FerrisWheelScaleFactor = 1.0;  // Initial scale factor
int FerrisWheeldirection = 1;  // Initial direction: 1 for increasing, -1 for decreasing

bool ferrisWheelRotating = false;
float ferrisWheelRotationAngle = 0.0;
const float ferrisWheelRotationSpeed = 1.0; // Adjust the rotation speed as needed
int timerInterval = 1000;  // Adjust the interval as needed


float TeddyBearX = 1.23;
float TeddyBearY = -0.1;
float TeddyBearZ = 0;
float TeddyBearscaleFactor = 1.0;  // Initial scale factor
int TeddyBeardirection = 1;  // Initial direction: 1 for increasing, -1 for decreasing

float FountainX = 0.27;
float FountainY = 0;
float FountainZ = 0;
int FountainDirection = 1;  // Initial direction: 1 for increasing, -1 for decreasing
float FountainScaleFactor = 1.0;  // Initial scale factor


float CashierX = 0.43;
float CashierY = 0;
float CashierZ = 0.4;
int CashierDirection = 1;  // Initial direction: 1 for increasing, -1 for decreasing
float CashierScaleFactor = 1.0;  // Initial scale factor

bool GameOverWin = false;
bool GameOverLost = false;
bool DetectedCollision = false;
bool TimesUp = false;






bool changeColors = true;  // Variable to toggle colors


class Vector3f {
public:
	float x, y, z;


	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;
	Camera(float eyeX = 0.6f, float eyeY = 0.5f, float eyeZ = 2.55f, float centerX = 0.6f, float centerY = 0.6f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;


void DisplayGameOverLostPopup() {
	glClearColor(1.0, 0.0, 0.0, 1.0); // Set background color to red
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0); // Set text color to black

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1, 1, -1, 1); // Set the orthographic projection

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRasterPos2f(-0.6, 0); // Text position
	std::string GameLostText = "Game Over, You Lost!";

	// Use a bold font, like GLUT_BITMAP_HELVETICA_18 for bold
	for (char c : GameLostText) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

	glFlush();
}

void DisplayGameOverWinPopup() {
	glClearColor(0.0, 1.0, 0.0, 1.0); // Set background color to green
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0); // Set text color to red

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1, 1, -1, 1); // Set the orthographic projection

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRasterPos2f(-0.6, 0); // Text position
	std::string GameWinText = "Game Over, You Win!";

	for (char c : GameWinText) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

	glFlush();
}




void Collisions() {
	if (PlayerX >= 0.8 && PlayerX <= 1.1 && PlayerZ >= -0.3 && PlayerZ <= -0.25) {
		// Collision occurred
		printf("Collision Detected!\n");
		DetectedCollision = true;
		// You can add any logic or actions to be taken upon collision
	}
}



// Timer callback function for the game
void timerCallback(int value) {
	elapsedTime++;
	if (elapsedTime >= totalTime && !GameOverWin) {
		std::cout << "Time is up! Displaying Game Over, You Lost! screen.\n";
		TimesUp = true;
		//DisplayGameOverLostPopup();
		return;  // Do not proceed to destroy the window
	}

	// Set the timer for the next frame
	glutTimerFunc(1000, timerCallback, 0);  // 1000 milliseconds for 1 second
}


void wallTimer(int value) {
	changeColors = !changeColors;  // Toggle colors
	glutPostRedisplay();  // Trigger a redraw
	glutTimerFunc(1000, wallTimer, 0);  // Set timer for the next interval (1000 seconds)
	// Update the Ferris wheel rotation angle
	if (ferrisWheelRotating) {
		ferrisWheelRotationAngle += ferrisWheelRotationSpeed;
		glutPostRedisplay();
	}
}


void TeddyBearTimerFunction(int value) {
	static int elapsedTime = 0;



	// Change the direction every 1000 ms
	if (elapsedTime % 1000 == 0) {
		TeddyBeardirection *= -1;
	}

	// Adjust the scale factor based on the direction
	TeddyBearscaleFactor += 0.002 * TeddyBeardirection;

	// Redraw the teddy bear
	glutPostRedisplay();

	// Set the timer for the next frame
	glutTimerFunc(16, TeddyBearTimerFunction, 0);  // 16 milliseconds for 60 fps

	// Increment the elapsed time
	elapsedTime += 16;
}


void AnimationTimerFunction(int value) {

	if (AnimationOn) {
		static int AnimationElapsedTime = 0;


		// Change the direction every 1000 ms
		if (AnimationElapsedTime % 1000 == 0) {
			TreeDirection *= -1;
			FountainDirection *= -1;
			CashierDirection *= -1;
			FerrisWheeldirection *= -1;
			Teapotdirection *= -1;
		}


		// Adjust the scale factor based on the direction
		TreeScaleFactor += 0.001 * TreeDirection;
		FountainScaleFactor += 0.001 * FountainDirection;
		CashierScaleFactor += 0.001 * CashierDirection;
		FerrisWheelScaleFactor += 0.001 * FerrisWheeldirection;
		TeapotScaleFactor += 0.001 * Teapotdirection;

		// Redraw the teddy bear
		glutPostRedisplay();

		// Set the timer for the next frame
		glutTimerFunc(16, AnimationTimerFunction, 0);  // 16 milliseconds for 60 fps

		// Increment the elapsed time
		AnimationElapsedTime += 16;
	}


}

void drawCylinder(double radius, double height, int slices, int stacks) {
	GLUquadric* quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);

	gluCylinder(quadric, radius, radius, height, slices, stacks);

	gluDeleteQuadric(quadric);
}

void drawFilledCylinder(GLfloat radius, GLfloat height, GLint slices, GLint stacks) {
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);  // Filled style

	// Draw the filled cylinder
	gluCylinder(quadric, radius, radius, height, slices, stacks);

	// Draw the top disk to close the cylinder
	gluDisk(quadric, 0.0, radius, slices, stacks);

	// Move to the bottom to draw the bottom disk
	glTranslatef(0.0, 0.0, -height);

	//// Draw the bottom disk to close the cylinder
	//gluDisk(quadric, 0.0, radius, slices, stacks);

	gluDeleteQuadric(quadric);

}


void drawTorus(double outerRadius, double innerRadius, int sides, int rings) {
	glColor3f(0.6, 0.6, 0.5); // Set color for the torus (red)

	// Draw the wireframe torus
	glutWireTorus(innerRadius, outerRadius, sides, rings);

	// Draw a sphere in the center to create the appearance of a solid torus
	glColor3f(0.6, 0.6, 0.5); // Set color for the sphere (red)
	glutSolidSphere(innerRadius, sides, rings);
}
void drawWall(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();

}

void drawStripedWall(double thickness) {
	glPushMatrix();
	glTranslated(0, -0.01, 0);

	for (int i = 0; i < 9; ++i) {
		// Alternate between red and white stripes
		if ((i % 2 == 0 && changeColors) || (i % 2 != 0 && !changeColors)) {
			glColor3f(1.0, 0.0, 0.0);  // Red color
		}
		else {
			glColor3f(1.0, 1.0, 1.0);  // White color
		}

		// Draw a single stripe
		glPushMatrix();
		glTranslated(0.05, 0.54 * thickness, 0.5);
		glScaled(1.0 / 9.0, thickness, 1.0);
		//glScaled(1.1, 1.1, 1.1);
		glutSolidCube(1);
		glPopMatrix();

		// Move to the next stripe without any gap
		glTranslatef(1.0 / 9.0, 0.0, 0.0);
	}

	// Draw a red cone
	glColor3f(0.9, 0.0, 0.0);  // Red color
	glPushMatrix();
	glTranslated(0, 0, 0.97);  // Adjust position
	//glRotated(-270, 1, 0, 0);  // Rotate cone to face upwards
	glutSolidCone(0.05, 0.1, 30, 30);  // Draw cone
	glPopMatrix();

	// Draw a white sphere on top of the cone
	glColor3f(1.0, 1.0, 1.0);  // White color
	glPushMatrix();
	glTranslated(0, 0, 1.07);  // Adjust position
	glutSolidSphere(0.03, 30, 30);  // Draw sphere
	glPopMatrix();
	glPopMatrix();

	// Draw a red cone
	glColor3f(0.9, 0.0, 0.0);  // Red color
	glPushMatrix();
	glTranslated(0, 0, 0.97);  // Adjust position
	//glRotated(-270, 1, 0, 0);  // Rotate cone to face upwards
	glutSolidCone(0.05, 0.1, 30, 30);  // Draw cone
	glPopMatrix();

	// Draw a white sphere on top of the cone
	glColor3f(1.0, 1.0, 1.0);  // White color
	glPushMatrix();
	glTranslated(0, 0, 1.07);  // Adjust position
	glutSolidSphere(0.03, 30, 30);  // Draw sphere
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}


void drawHorizontalStripedWall(double thickness) {
	glPushMatrix();
	glTranslated(-0.01, 0, 0);
	for (int i = 0; i < 9; ++i) {
		// Alternate between red and white stripes
		if ((i % 2 == 0 && changeColors) || (i % 2 != 0 && !changeColors)) {
			glColor3f(1.0, 1.0, 1.0);  // White color
		}
		else {
			glColor3f(1.0, 0.0, 0.0);  // Red color
		}

		// Draw a single stripe
		glPushMatrix();
		glTranslated(0.5, 0.5 * thickness, 0.087);
		glScaled(0.954, thickness, 1.0 / 9.0);
		glutSolidCube(1);
		glPopMatrix();



		// Move to the next stripe without any gap
		glTranslatef(0.0, 0.0, 1.0 / 9.0);
	}
	//glTranslated(-2, -0.5, -1.5);
	// Draw a red cone
	glColor3f(0.9, 0.0, 0.0);  // Red color
	glPushMatrix();
	glTranslated(0.95, 0, 0);  // Adjust position
	glRotated(-270, 0, 1, 0);  // Rotate cone to face upwards
	glutSolidCone(0.05, 0.1, 30, 30);  // Draw cone
	glPopMatrix();

	// Draw a white sphere on top of the cone
	glColor3f(1.0, 1.0, 1.0);  // White color
	glPushMatrix();
	glTranslated(1.05, 0, 0);  // Adjust position
	glutSolidSphere(0.03, 30, 30);  // Draw sphere
	glPopMatrix();
	glPopMatrix();
}

void drawTableLeg(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawJackPart() {
	glPushMatrix();
	glScaled(0.2, 0.2, 1.0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 1.2);
	glutSolidSphere(0.2, 15, 15);
	glTranslated(0, 0, -2.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();
}
void drawJack() {
	glPushMatrix();
	drawJackPart();
	glRotated(90.0, 0, 1, 0);
	drawJackPart();
	glRotated(90.0, 1, 0, 0);
	drawJackPart();
	glPopMatrix();
}
void drawTable(double topWid, double topThick, double legThick, double legLen) {
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
}



void drawTree() {

	glScalef(TreeScaleFactor, TreeScaleFactor, TreeScaleFactor);



	//adjust size and location
	glTranslated(TreeX, TreeY, TreeZ);
	glScalef(0.20f, 0.35f, 0.20f);
	glTranslated(0.20, 0.35, 0.45);

	// Draw tree trunk
	glColor3f(0.54f, 0.27f, 0.07f); // Brown color for trunk
	glPushMatrix();
	glScaled(0.1, 0.5, 0.1); // Adjust the dimensions based on your preference
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw a circle underneath the cone
	glColor3f(0.0f, 0.5f, 0.0f); // Green color for leaves
	glPushMatrix();
	glTranslated(0.0, 0.5, 0.0); // Move to the top of the trunk
	glutSolidSphere(0.4, 10, 10); // Adjust the dimensions based on your preference
	glPopMatrix();

	// Draw tree leaves (a simple cone as representation)
	glColor3f(0.0f, 0.5f, 0.0f); // Green color for leaves
	glPushMatrix();
	glTranslated(0.0, 0.6, 0.0); // Move to the top of the trunk
	glRotated(-90.0, 1, 0, 0); // Rotate the cone to point upwards
	glutSolidCone(0.4, 0.8, 10, 10); // Adjust the dimensions based on your preference
	glPopMatrix();
	//glFlush();
}



void drawTeapotRide() {

	glTranslatef(TeapotX, TeapotY, TeapotZ);
	glScalef(TeapotScaleFactor, TeapotScaleFactor, TeapotScaleFactor);
	float offset = 0.10;

	// Large black flattened sphere
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0); // Black color for the large flattened sphere
	glTranslated(0.6, 0.02, 0.5);
	glScaled(0.5, 0.01, 0.5); // Adjust the dimensions based on your preference
	glutSolidSphere(0.9, 50, 50);
	glPopMatrix();

	glTranslated(0, -0.05, 0);

	// Center teapot with a smaller white flattened sphere
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0); // White color for the smaller flattened sphere
	glTranslated(0.6, 0.09, 0.5);
	glScaled(0.5, 0.01, 0.5);
	glutSolidSphere(0.1, 50, 50); // Adjust the diameter as needed
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6, 0.1, 0.5);
	glScaled(0.5, 0.5, 0.5);
	glRotated(30, 0, 1, 0);
	glColor3f(1, 1, 0.3);
	glutSolidTeapot(0.08);
	glPopMatrix();

	// Teapots around the center with smaller white flattened spheres
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0); // White color for the smaller flattened sphere
	glTranslated(0.6 + offset, 0.09, 0.5 + offset);
	glScaled(0.5, 0.01, 0.5);
	glutSolidSphere(0.1, 50, 50); // Adjust the diameter as needed
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6 + offset, 0.1, 0.5 + offset);
	glScaled(0.5, 0.5, 0.5);
	glRotated(30, 0, 1, 0);
	glColor3f(0.7, 1, 0.4);
	glutSolidTeapot(0.08);
	glPopMatrix();

	// Teapot at (0.6 + offset, 0.1, 0.5 - offset)
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0); // White color for the smaller flattened sphere
	glTranslated(0.6 + offset, 0.09, 0.5 - offset);
	glScaled(0.5, 0.01, 0.5);
	glutSolidSphere(0.1, 50, 50); // Adjust the diameter as needed
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6 + offset, 0.1, 0.5 - offset);
	glScaled(0.5, 0.5, 0.5);
	glRotated(30, 0, 1, 0);
	glColor3f(0.8, 0.6, 0.9);
	glutSolidTeapot(0.08);
	glPopMatrix();

	// Teapot at (0.6 - offset, 0.1, 0.5 + offset)
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0); // White color for the smaller flattened sphere
	glTranslated(0.6 - offset, 0.09, 0.5 + offset);
	glScaled(0.5, 0.01, 0.5);
	glutSolidSphere(0.1, 50, 50); // Adjust the diameter as needed
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6 - offset, 0.1, 0.5 + offset);
	glScaled(0.5, 0.5, 0.5);
	glRotated(30, 0, 1, 0);
	glColor3f(1, 0.7, 0.8);
	glutSolidTeapot(0.08);
	glPopMatrix();

	// Teapot at (0.6 - offset, 0.1, 0.5 - offset)
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0); // White color for the smaller flattened sphere
	glTranslated(0.6 - offset, 0.09, 0.5 - offset);
	glScaled(0.5, 0.01, 0.5);
	glutSolidSphere(0.1, 50, 50); // Adjust the diameter as needed
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6 - offset, 0.1, 0.5 - offset);
	glScaled(0.5, 0.5, 0.5);
	glRotated(30, 0, 1, 0);
	glColor3f(0.5, 0.8, 0.9);
	glutSolidTeapot(0.08);
	glPopMatrix();
}



void drawFerriswheel() {
	glPushMatrix();
	glTranslatef(FerrisWheelX, FerrisWheelY, FerrisWheelZ);
	glScaled(FerrisWheelScaleFactor, FerrisWheelScaleFactor, FerrisWheelScaleFactor);
	glScalef(5, 7, 5);
	glPushMatrix();
	glTranslated(0.6, 0.1, 0.5);
	glScaled(0.5, 0.5, 0.5);

	// Draw a grey torus
	drawTorus(0.08, 0.013, 30, 30);

	glPopMatrix();

	// Draw support structures as filled cuboids
	glPushMatrix();
	glTranslatef(0.6, 0.056, 0.47);  // Shift down
	glRotated(40.0, 1, 0, 0);  // Adjust tilt
	glScaled(0.003, 0.095, 0.005);  // Adjust dimensions
	glColor3f(0.0, 0.0, 0.0);  // Set color to black
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, 0.050, 0.51);  // Shift down
	glRotated(180.0, 1, 0, 0);  // Rotate
	glScaled(0.003, 0.095, 0.005);  // Adjust dimensions
	glColor3f(0.0, 0.0, 0.0);  // Set color to black
	glutSolidCube(1.0);
	glPopMatrix();


	//rotation
	 // Rotate the Ferris wheel based on the global angle
	glRotatef(ferrisWheelRotationAngle, 0.0, 1.0, 0.0);

	// Draw white spheres as baskets/seats
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(0.5, 0, 0.5);
	glColor3f(1.0, 1.0, 1.0);  // Set color to white
	glPushMatrix();
	glTranslatef(0.6, 0.19, 0.51);  // Adjust position
	glutSolidSphere(0.02, 30, 30);  // Draw first sphere
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.8, 0.19, 0.51);  // Adjust position
	glutSolidSphere(0.02, 30, 30);  // Draw second sphere
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.75, 0.28, 0.51);  // Adjust position
	glutSolidSphere(0.02, 30, 30);  // Draw third sphere
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.65, 0.28, 0.51);  // Adjust position
	glutSolidSphere(0.02, 30, 30);  // Draw fourth sphere
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.75, 0.12, 0.51);  // Adjust position
	glutSolidSphere(0.02, 30, 30);  // Draw fifth sphere
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.65, 0.12, 0.51);  // Adjust position
	glutSolidSphere(0.02, 30, 30);  // Draw sixth sphere
	glPopMatrix();

	// Wireframe cuboid to connect the first sphere
	glPushMatrix();
	glTranslatef(0.7, 0.2, 0.51);  // Adjust position
	glRotatef(90, 1, 0, 0);
	glRotatef(30, 0, 1, 0);
	glScaled(0.01, 0.001, 0.135);  // Adjust dimensions 
	glutWireCube(1.0);
	glPopMatrix();

	// Wireframe cuboid to connect the second sphere
	glPushMatrix();
	glTranslatef(0.7, 0.2, 0.51);  // Adjust position
	glRotatef(90, 1, 0, 0);
	glRotatef(-30, 0, 1, 0);
	glScaled(0.01, 0.001, 0.135);  // Adjust dimensions 
	glutWireCube(1.0);
	glPopMatrix();

	// Wireframe cuboid to connect the third sphere
	glPushMatrix();
	glTranslatef(0.7, 0.2, 0.51);  // Adjust position
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScaled(0.01, 0.001, 0.135);  // Adjust dimensions 
	glutWireCube(1.0);
	glPopMatrix();



	glPopMatrix();
}


void drawTeddyBear() {
	glPushMatrix();

	//animation

	glScalef(TeddyBearscaleFactor, TeddyBearscaleFactor, TeddyBearscaleFactor);
	//adjust size and location

	glTranslatef(TeddyBearX, TeddyBearY, TeddyBearZ);
	glScalef(0.5, 0.5, 0.5);



	// Draw brown oval for the body
	glColor3f(0.6, 0.3, 0.0);  // Brown color
	glPushMatrix();
	glTranslatef(0.0, 0.5, 0.0);  // Adjust position
	glScalef(0.6, 1.0, 0.2);  // Adjust dimensions
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	// Draw peach oval for the body
	glColor3f(0.9, 0.8, 0.5);  // Peach color
	glPushMatrix();
	glTranslatef(0.0, 0.5, 0.1);  // Adjust position
	glScalef(0.35, 0.55, 0.15);  // Adjust dimensions
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	// Draw brown spheres for limbs
	glColor3f(0.6, 0.3, 0.0);  // Brown color
	glScalef(1.3, 1.3, 1.3);

	// Left arm
	glPushMatrix();
	glTranslatef(-0.185, 0.5, 0.0);
	glutSolidSphere(0.1, 30, 30);
	glPopMatrix();

	// Right arm
	glPushMatrix();
	glTranslatef(0.185, 0.5, 0.0);
	glutSolidSphere(0.1, 30, 30);
	glPopMatrix();

	// Left leg
	glPushMatrix();
	glTranslatef(-0.1, 0.25, 0.1);
	glutSolidSphere(0.1, 30, 30);
	glPopMatrix();

	// Right leg
	glPushMatrix();
	glTranslatef(0.1, 0.25, 0.1);
	glutSolidSphere(0.1, 30, 30);
	glPopMatrix();

	// Draw brown sphere for the head
	glColor3f(0.6, 0.3, 0.0);  // Brown color
	glPushMatrix();
	glTranslatef(0.0, 0.65, 0.0);  // Head position
	glutSolidSphere(0.13, 30, 30);
	glPopMatrix();

	// Draw brown ears
	glColor3f(0.6, 0.3, 0.0);  // Brown color
	// Left ear
	glPushMatrix();
	glTranslatef(-0.13, 0.75, 0.0);
	glRotatef(30.0, 0, 1, 0);
	glutSolidSphere(0.07, 30, 30);
	glPopMatrix();

	// Right ear
	glPushMatrix();
	glTranslatef(0.13, 0.75, 0.0);
	glRotatef(-30.0, 0, 1, 0);
	glutSolidSphere(0.07, 30, 30);
	glPopMatrix();

	// Draw peach circle on top of ears
	glColor3f(0.9, 0.8, 0.5);  // Peach color
	glScalef(0.65, 0.65, 0.65);

	// Left ear
	glPushMatrix();
	glTranslatef(-0.19, 1.15, 0.05);
	glRotatef(30.0, 0, 1, 0);
	glutSolidSphere(0.07, 30, 30);
	glPopMatrix();

	// Right ear
	glPushMatrix();
	glTranslatef(0.19, 1.15, 0.05);
	glRotatef(-30.0, 0, 1, 0);
	glutSolidSphere(0.07, 30, 30);
	glPopMatrix();

	glTranslatef(0, 0.15, 0.1);
	glScalef(1.3, 1.3, 1.3);
	// Draw small black eyes
	glTranslatef(0, 0, -0.05);
	glColor3f(0.0, 0.0, 0.0);  // Black color
	glPushMatrix();
	glTranslatef(-0.05, 0.7, 0.1);  // Left eye position
	glutSolidSphere(0.02, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.05, 0.7, 0.1);  // Right eye position
	glutSolidSphere(0.02, 30, 30);
	glPopMatrix();

	// Draw small black nose
	glPushMatrix();
	glTranslatef(0.0, 0.65, 0.15);  // Nose position
	glutSolidSphere(0.015, 30, 30);
	glPopMatrix();

	glPopMatrix();
}



void drawCashier() {
	glTranslatef(CashierX, CashierY, CashierZ);
	glScalef(CashierScaleFactor, CashierScaleFactor, CashierScaleFactor);
	glScalef(0.7, 0.7, 0.7);

	glPushMatrix();

	//draw table
	glTranslatef(1.25, 0, 1);
	glRotated(90, 0, 1, 0);
	drawTable(0.2, 0.01, .01, 0.2);
	glPopMatrix();

	// Draw the dark grey cube on top
	glPushMatrix();
	glTranslatef(1.25, 0.25, 1);  // Adjust position above the table
	glScalef(0.1, 0.1, 0.1);  // Adjust dimensions
	glColor3f(0.2, 0.2, 0.2);  // Dark grey color
	glutSolidCube(1);
	glPopMatrix();

	//draw smaller lighter grey box on top
	 // Draw the smaller light grey cube on top
	glPushMatrix();
	glTranslatef(1.25, 0.3, 1);  // Adjust position above the dark grey cube
	glScalef(0.05, 0.05, 0.05);  // Adjust dimensions
	glColor3f(0.7, 0.7, 0.7);  // Light grey color
	glutSolidCube(1);
	glPopMatrix();


	glPopMatrix();


}


void drawFountain() {

	glTranslatef(FountainX, FountainY, FountainZ);

	glScalef(FountainScaleFactor, FountainScaleFactor, FountainScaleFactor);

	glScalef(0.7, 0.7, 0.7);
	//first layer
	// Draw the grey cylinder as the base of the fountain
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(0.9, 0.1, 1.0);  // Adjust position


	glRotatef(90, 1, 0, 0);
	drawCylinder(0.2, 0.1, 15, 10);  // Draw a grey cylinder
	glPopMatrix();

	// Draw the blue filled cylinder inside
	glPushMatrix();
	glTranslatef(0.9, 0.1, 1.0);  // Adjust position (slightly above the outer cylinder)
	glColor3f(0.0, 0.0, 1.0);  // Blue color

	glRotatef(90, 1, 0, 0);
	drawFilledCylinder(0.195, 0.075, 30, 10);  // Draw the blue filled cylinder
	glPopMatrix();

	// Draw the grey cylinder as the middle of the fountain
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(0.9, 0.3, 1.0);  // Adjust position


	glRotatef(90, 1, 0, 0);
	drawCylinder(0.035, 0.2, 30, 10);  // Draw a grey cylinder
	glPopMatrix();

	//second layer
	glScaled(0.7, 0.7, 0.7);
	glTranslated(0.4, 0.3, 0.4);


	// Draw the grey cylinder as the base of the fountain
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(0.9, 0.1, 1.0);  // Adjust position


	glRotatef(90, 1, 0, 0);
	drawCylinder(0.2, 0.1, 10, 10);  // Draw a grey cylinder
	glPopMatrix();

	// Draw the blue filled cylinder inside
	glPushMatrix();
	glTranslatef(0.9, 0.1, 1.0);  // Adjust position (slightly above the outer cylinder)
	glColor3f(0.0, 0.0, 1.0);  // Blue color

	glRotatef(90, 1, 0, 0);
	drawFilledCylinder(0.195, 0.075, 30, 10);  // Draw the blue filled cylinder
	glPopMatrix();



}

void drawPerson() {

	glTranslated(PlayerX, PlayerY, PlayerZ); //make it x y z and use them in movement
	glTranslated(0.20, 0.42, 0.35);
	glRotated(PlayerRotationAngle, 0, 1, 0);



	// Draw head
	glPushMatrix();
	glTranslated(0.0, 0.05, 0.0); // Slightly higher position for the head
	glColor3f(1.0f, 0.87f, 0.68f); // Light skin tone for the head
	glutSolidSphere(0.05, 15, 15);
	glPopMatrix();

	// Draw body
	glPushMatrix();
	glTranslated(0.0, -0.05, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f); // Red color for body
	glScaled(0.05, 0.1, 0.02); // Adjust the dimensions based on your preference
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw arms
	   // Draw arms using cylinders
	glPushMatrix();
	glTranslated(-0.065, -0.03, 0.0);
	glColor3f(1.0f, 0.87f, 0.68f); // Light skin tone for the arm
	glRotated(90.0, 0, 1, 0); // Rotate the cylinder to align with the arm
	GLUquadricObj* quadric = gluNewQuadric();
	gluCylinder(quadric, 0.01, 0.01, 0.04, 15, 15); // Adjust the dimensions based on your preference
	gluDeleteQuadric(quadric);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.025, -0.03, 0.0);
	glColor3f(1.0f, 0.87f, 0.68f); // Light skin tone for the arm
	glRotated(90.0, 0, 1, 0); // Rotate the cylinder to align with the arm
	quadric = gluNewQuadric();
	gluCylinder(quadric, 0.01, 0.01, 0.04, 15, 15); // Adjust the dimensions based on your preference
	gluDeleteQuadric(quadric);
	glPopMatrix();

	// Draw legs
	glPushMatrix();
	glTranslated(-0.015, -0.1, 0.0);
	glColor3f(0.0f, 0.0f, 1.0f); // Blue color for legs
	glScaled(0.02, 0.01, 0.02); // Adjust the dimensions based on your preference
	glutSolidSphere(1.0, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.015, -0.1, 0.0);
	glColor3f(0.0f, 0.0f, 1.0f); // Blue color for legs
	glScaled(0.02, 0.01, 0.02); // Adjust the dimensions based on your preference
	glutSolidSphere(1.0, 15, 15);
	glPopMatrix();

	// Draw eyes
	glPushMatrix();
	glTranslated(0.01, 0.06, 0.04); // Adjust eye position based on head position
	glColor3f(0.0f, 0.0f, 0.0f); // Black color for eyes
	glutSolidSphere(0.01, 10, 10);
	glTranslated(-0.02, 0.0, 0.0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	//draw mouth
	glPushMatrix();
	glTranslated(0.00, 0.04, 0.05); // Adjust eye position based on head position
	glColor3f(0.0f, 0.0f, 0.0f); // Black color for eyes
	glutSolidSphere(0.01, 15, 15);
	glPopMatrix();
	//glFlush();


}





void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	if (DetectedCollision) {
		DisplayGameOverWinPopup();
		GameOverWin = true;
	}
	else if (TimesUp && !GameOverWin) {
		DisplayGameOverLostPopup();
	}
	else {



		glPopMatrix();
		glPushMatrix();
		drawPerson();
		glPopMatrix();

		glPushMatrix();
		drawTeapotRide();
		glPopMatrix();

		glPushMatrix();
		drawFerriswheel();
		glPopMatrix();

		if (!DetectedCollision) {
			glPushMatrix();
			drawTeddyBear();  // translations for the tree are enclosed in a separate matrix block
			glPopMatrix();
		}


		glPushMatrix();
		drawCashier();
		glPopMatrix();

		glPushMatrix();
		drawFountain();
		glPopMatrix();

		glPushMatrix();
		drawTree();  // translations for the tree are enclosed in a separate matrix block
		glPopMatrix();


		glPushMatrix();
		glTranslatef(1.2, 0, 0.3);
		glScalef(0.6, 0.6, 0.6);
		drawTree();  // translations for the tree are enclosed in a separate matrix block
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 0, 0.3);
		glScalef(0.55, 0.55, 0.55);
		drawTree();  // translations for the tree are enclosed in a separate matrix block
		glPopMatrix();



		glPopMatrix();
		glPushMatrix();
		/*glTranslated(0.4, 0.0, 0.4);
		drawTable(0.6, 0.02, 0.02, 0.3);*/
		glPopMatrix();
		glScaled(0.7, 0.7, 0.7);
		//glColor3f(0.54f, 0.81f, 0.94f); // Set to baby blue (lower red and green, high blue)
		glScaled(2, 2, 2);


		drawWall(0.02);
		glPushMatrix();
		glRotated(90, 0, 0, 1.0);
		//glColor3f(1.0f, 0.5f, 1.0f); // Set to pink (high red and blue, low green)





		glTranslated(0, -0.01, 0);
		drawHorizontalStripedWall(0.025);
		glPopMatrix();
		glPushMatrix();
		glRotated(-90, 1.0, 0.0, 0.0);
		//glRotated(-90, 1.0, 0.0, 0.0);
		drawStripedWall(0.02);
		glPopMatrix();

		// Right Wall (similar to the left wall but on the right side)
		glPushMatrix();
		glTranslated(1.0, 0.0, 0.0);  // Translate to the right side
		glRotated(90, 0, 0, 1.0);  // Rotate by 90 degrees around the Z-axis
		//glColor3f(1.0f, 0.5f, 1.0f);  // Set the color for the wall
		drawHorizontalStripedWall(0.025);  // Draw the wall on the right side
		glPopMatrix();




		//glTranslated(20,0,40);
		//drawWall(0.02);
		//glPushMatrix();
		//glRotated(90, 0, 0, 1.0);
		//glColor3f(1.0f, 0.5f, 1.0f); // Set to pink (high red and blue, low green)

	}
	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);

	case 'W':
		Collisions();

		if (PlayerZ <= 1.05) {
			PlayerZ += d; // Move character forward along the Z-axis
			PlayerRotationAngle = 0;
		}

		break;
	case 'S':
		Collisions();

		if (PlayerZ >= -0.3) {
			PlayerZ -= d; // Move character backward along the Z-axis
			PlayerRotationAngle = 180;


		}
		break;
	case 'A':
		Collisions();


		//least possible x==-0.1 max x=1.1

		if (PlayerX >= -0.1) {
			PlayerX -= d; // Move character left along the X-axis
			//PlayerRotationAngle = -45;
			PlayerRotationAngle = 270; // Rotate to the left when moving left
		}
		break;
	case 'D':
		Collisions();

		if (PlayerX <= 1.1) {
			PlayerX += d; // Move character right along the X-axis
			//PlayerRotationAngle = 90;
			PlayerRotationAngle = 90; // Rotate to the left when moving left



		}
		break;

	case 'f':

		// Adjust the camera to a front view
		camera = Camera(0.6f, 0.5f, 2.55f, 0.6f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
		break;
	case 't':
		// Adjust the camera to a top view
		camera = Camera(0.5f, 2.8f, 1.3f, 0.5f, 0.5f, 0.45f, 0.0f, 1.4f, 0.5f);
		break;
	case 'g':
		// Adjust the camera to a side view
		camera = Camera(2.8f, 0.9f, 0.8f, 0.3f, 0.7f, 0.8f, 0.0f, 1.0f, 0.0f);
		break;


	case 'x':
		// Toggle animation state
		AnimationOn = !AnimationOn;

		if (AnimationOn) {
			// Start or restart the animation timer
			glutTimerFunc(0, AnimationTimerFunction, 0);
		}




		break;
	}

	glutPostRedisplay();
}



void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Lab 5");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	// Set the timer to start immediately and repeat every second
	glutTimerFunc(0, timerCallback, 0);
	glutTimerFunc(timerInterval, wallTimer, 0);
	//glutTimerFunc(0, wallTimer, 0);  // Set the timer to start immediately
	glutTimerFunc(0, TeddyBearTimerFunction, 0);
	glutTimerFunc(0, AnimationTimerFunction, 0);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);




	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
