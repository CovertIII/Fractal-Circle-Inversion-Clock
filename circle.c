#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "vector2.h"

vector2 movin = {0,0};

int db = 0;

void circle_object(float pos_x, float pos_y, float size) {
	glPushMatrix();
	glTranslatef(pos_x,pos_y,0);
	float counter;
	glBegin(GL_LINE_LOOP);
	for (counter = 0; counter <= 2*3.14159; counter = counter + 3.14159/12) {
		glVertex3f ((size)*cos(counter), (size)*sin(counter), 0.0);
	}
    glEnd();
	glPopMatrix();
}

typedef struct{
	vector2 p;
	double r;
	double th;
} circle;

// https://users.math.yale.edu/public_html/People/frame/Fractals/CircInvFrac/InvFormulas/InvFormulas1.html
vector2 invertpt(circle _c, vector2 _pt){
	vector2 rpt;
	double s = _c.r*_c.r / v2SPow(v2Sub(_pt, _c.p));
	rpt = v2Add(_c.p, v2sMul(s, v2Sub(_pt, _c.p)));
	return rpt;
};

circle c[8];
int circle_num = 9;

int lastFrameTime = 0;


int init(void){
	int i;
	for(i=0; i<4; i++){
		c[i].p.x = sqrt(2)*cos(i*M_PI/2.0f);
		c[i].p.y = sqrt(2)*sin(i*M_PI/2.0f);
		c[i].r   = 1.0f;
	}
	c[4].p.x = (sqrt(2) - 1)/2.0f;
	c[4].p.y = 0;
	c[4].r = 0.075;
	c[4].th = 0;

	c[5].p.x = (sqrt(2) - 1)-0.1f;
	c[5].p.y = 0;
	c[5].r = 0.06f;
	c[5].th = 0;

	c[6].p.x = (sqrt(2) - 1)-0.05f;
	c[6].p.y = 0;
	c[6].r = 0.05f;
	c[6].th = 0;

	c[7].p.x = 0;
	c[7].p.y = 0;
	c[7].r = 0.15f;
	c[7].th = 0;

	c[8].p.x = 0;
	c[8].p.y = 0;
	c[8].r = 0.06f;
	c[8].th = 0;

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);*/
}


void processNormalKeys(unsigned char key, int xx, int yy) {
	if(key == 'g'){
		db = db ? 0 : 1;
	}

}

void releaseNormalKeys(unsigned char key, int xx, int yy) {

}


void numbers(void){
	int now = glutGet(GLUT_ELAPSED_TIME);
    int elapsedMilliseconds = now - lastFrameTime;
    float elapsedTime = elapsedMilliseconds / 1000.0f;
    lastFrameTime = now;
	float h = elapsedTime;

	time_t rawtime;
	struct tm * timeinfo;
	struct timeval tv;

	gettimeofday(&tv, NULL);


	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	c[6].th = -(double)timeinfo->tm_sec/60.0f * 2 * M_PI  + M_PI_2 - (double)tv.tv_usec/(double)10000000.0f;
	c[6].p.x = ((sqrt(2) - 1)-0.05f)*cos(c[6].th);
	c[6].p.y = ((sqrt(2) - 1)-0.05f)*sin(c[6].th);

	c[5].th = -((double)timeinfo->tm_min + (double)timeinfo->tm_sec/60.0f)/60.0f*2*M_PI + M_PI_2;
	c[5].p.x = ((sqrt(2) - 1)-0.06f)*cos(c[5].th);
	c[5].p.y = ((sqrt(2) - 1)-0.06f)*sin(c[5].th);

	c[8].th = -((double)timeinfo->tm_min + (double)timeinfo->tm_sec/60.0f)/60.0f*2*M_PI + M_PI_2;
	c[8].p.x = ((sqrt(2) - 1)-2*0.06f-0.06f)*cos(c[5].th);
	c[8].p.y = ((sqrt(2) - 1)-2*0.06f-0.06f)*sin(c[5].th);

	c[4].th = -((double)timeinfo->tm_hour + (double)timeinfo->tm_min/60.0f)/12.0f*2*M_PI + M_PI_2;
	c[4].p.x = (0.15+0.075)*cos(c[4].th);
	c[4].p.y = (0.15+0.075)*sin(c[4].th);
}

void display(void) {
	int i,k;
	numbers();
	//-----This is the stuff involved with drawing the screen----//
	glClearColor (0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glColor3f(1,1,1);

	if(db){
		for(i=0; i<circle_num; i++){
			circle_object(c[i].p.x,c[i].p.y,c[i].r);
		}
	}

	vector2 pt;
	pt.x = -sqrt(2);
	pt.y = sqrt(2);
	int circle = rand()%circle_num;
	int tmp;

	glBegin(GL_POINTS);
	glVertex3f (pt.x, pt.y, 0.0);
	glEnd();

	for(i<0; i<10000; i++){
		pt = invertpt(c[circle], pt);
		tmp = rand()%circle_num;
		while(tmp == circle){
			tmp = rand()%circle_num;
		}
		circle = tmp;
		glBegin(GL_POINTS);
		glVertex3f (pt.x, pt.y, 0.0);
		glEnd();
	}
	glPopMatrix();

	glutSwapBuffers();

}

void pressKey(int key, int xx, int yy) {

}

void releaseKey(int key, int xx, int yy) {

}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

	double ratio = glutGet(GLUT_WINDOW_WIDTH)/(double)glutGet(GLUT_WINDOW_HEIGHT);
	//vector2 w = {(sqrt(2)-1)*2,(sqrt(2)-1)*2};
	vector2 w = {2,2};
	double wx = ratio * w.y;
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-wx/2.0f, wx/2.0f, -w.y/2.0f, w.y/2.0f);
    glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);

	glutCreateWindow("Tron Clone Client");

	init();

	glutIgnoreKeyRepeat(1);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(releaseNormalKeys);
	glutMainLoop();
	return EXIT_SUCCESS;
}


