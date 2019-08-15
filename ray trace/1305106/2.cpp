#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

/*
struct point
{
	double x,y,z;
};

point u,r,l;

point calc(point n,point m,double angle)
{

    double x=n.x;double y=n.y;double z=n.z;double xx=m.x;double yy=m.y;double zz=m.z;

    double cs=cos(angle);
    double xs=x*cs;double ys=y*cs; double zs=z*cs;


      double ss=sin(angle);
    double xxs=xx*ss;double yys=yy*ss; double zzs=zz*ss;

     point p={xs+xxs,ys+yys,zs+zzs};
     return p;

}

*/



struct direction
{

  double x,y;
};

direction calc(direction n,direction m,double angle)
{

    double x=n.x;double y=n.y;
    double xx=m.x;double yy=m.y;

    double cs=cos(angle);
    double xs=x*cs;double ys=y*cs;
    //double zs=z*cs;


      double ss=sin(angle);
    double xxs=xx*ss;double yys=yy*ss;
    //double zzs=zz*ss;

     direction p={xs+xxs,ys+yys};
     return p;

}

direction adi(direction a,direction b)
{
    direction z={a.x+b.x,a.y+b.y};
    return z;

}

direction cng(direction d,double a,double b)
{

    double x,y,z;
    double c=d.x*a+d.y*b;
    x=d.x-2*c*a;
    y=d.y-2*c*b;
    direction t={x,y};
    return t;
}

direction d,p;

direction norm(direction p)
{

    direction t={p.y,-p.x};
    return t;
}

/*void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}*/

void drawbox()
{

    glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,100,0);
			glVertex3f(100,-100,0);

			glVertex3f(-100,-100,0);
			glVertex3f(-100,100,0);

			glVertex3f(-100,100,0);
			glVertex3f(100,100,0);

			glVertex3f(-100,-100,0);
			glVertex3f(100,-100,0);
		}glEnd();

}

void drawline()
{
    direction e=adi(p,d);
     e=adi(e,d);
      e=adi(e,d);
    direction uu=adi(e,d);
    direction ulta={d.y,-d.x};

    //glColor3f(1.0, 1.0, 0);
		glBegin(GL_LINES);{
			glVertex3f( p.x,p.y,0);
			glVertex3f(e.x,e.y,0);
		}glEnd();


		glBegin(GL_TRIANGLES);
        {
            glVertex3f(uu.x,uu.y,0);
			glVertex3f(e.x+ulta.x,e.y+ulta.y,0);
			glVertex3f(e.x-ulta.x,e.y-ulta.y,0);
        }glEnd();


}

void drawSS()
{
    //glColor3f(1,0,0);
   drawline();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			d=calc(d,norm(d),.1);
			break;
		case GLUT_KEY_UP:		// up arrow key
			d=calc(d,norm(d),-.1);
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
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


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
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
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
    drawbox();
//	drawAxes();
	//drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

	//codes for any changes in Models, Camera

	 direction e=adi(p,d);
     e=adi(e,d);
      e=adi(e,d);
    direction uu=adi(e,d);

	if(uu.x>100)
       d=cng(d,-1,0);
    else if(uu.x<-100)
        d=cng(d,1,0);
    else if(uu.y>100)
        d=cng(d,0,-1);
    else if(uu.y<-100)
        d=cng(d,0,1);

    p.x=p.x+(d.x)/300;p.y=p.y+(d.y)/300;

	glutPostRedisplay();
}

void init(){

    d={5,0};
	p={50,50};
	//codes for initialization
	//drawgrid=0;
	//drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;



	//clear the screen
	glClearColor(0,0,0,0);



	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
