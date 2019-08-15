#include<stdio.h>
#include<bits/c++io.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))


double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double k=80;
double b=80;

double angle;
double x_pos=100;
double y_pos=100;
double z_pos=200;


struct point
{
    double x,y,z;
};

point u,r,l;

point calc(point n,point m,double angle)
{

    double x=n.x;
    double y=n.y;
    double z=n.z;
    double xx=m.x;
    double yy=m.y;
    double zz=m.z;

    double cs=cos(angle);
    double xs=x*cs;
    double ys=y*cs;
    double zs=z*cs;


    double ss=sin(angle);
    double xxs=xx*ss;
    double yys=yy*ss;
    double zzs=zz*ss;

    point p= {xs+xxs,ys+yys,zs+zzs};
    return p;

}


void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glColor3f(1.0, 0, 0);
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glColor3f(0, 0, 1);
            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glColor3f(0, 1.0, 0);
            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
    }
}

void drawSquarexy(double a)
{
    //glColor3f(1.0,0.0,0.0);
    for(a=a;a>=0;a=a-.2){
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }
    glEnd();}
}

void drawSquareyz(double a)
{
    //glColor3f(1.0,0.0,0.0);
    for(a=a;a>=0;a=a-.2){
    glBegin(GL_QUADS);
    {
        glVertex3f( 0,a, a);
        glVertex3f( 0,a,-a);
        glVertex3f(0,-a,-a);
        glVertex3f(0,-a, a);
    }
    glEnd();}
}

void drawSquarezx(double a)
{
    //glColor3f(1.0,0.0,0.0);
    for(a=a;a>=0;a=a-.2){
    glBegin(GL_QUADS);
    {
        glVertex3f( a,0, a);
        glVertex3f( a,0,-a);
        glVertex3f(-a,0,-a);
        glVertex3f(-a,0, a);
    }
    glEnd();}
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0; i<segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void zzdrawCircle(double radius,double h,int segments)
{
    segments=25;
    if(h<=0)
        return;
    int i;
    for(double p=h;p>=-h;p=p-.3)
    {


    struct point points[28];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*(pi/2));
        points[i].y=radius*sin(((double)i/(double)segments)*(pi/2));
    }
    //draw segments using generated points
    for(i=0; i<segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x,points[i].y,p);
            glVertex3f(points[i+1].x,points[i+1].y,p);
        }
        glEnd();
    }}
}




void drawSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }
            glEnd();
        }
    }
}

void zdrawSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*(pi/2));
            points[i][j].y=r*sin(((double)j/(double)slices)*(pi/2));
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        glColor3f(1,0,0);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere

            }
            glEnd();
        }
    }
}

void drawzz()
{


 zzdrawCircle(50,50,50);

}

void drawSS()
{


    glPushMatrix();
    {

     glTranslatef(0,0,k);
     drawSquarexy(b);
    }
    glPopMatrix();

    glPushMatrix();
    {

     glTranslatef(0,0,-k);
     drawSquarexy(b);
    }
    glPopMatrix();


     glPushMatrix();
    {

     glTranslatef(k,0,0);
     drawSquareyz(b);
    }
    glPopMatrix();

    glPushMatrix();
    {

     glTranslatef(-k,0,0);
     drawSquareyz(b);
    }
    glPopMatrix();


    glPushMatrix();
    {

     glTranslatef(0,k,0);
     drawSquarezx(b);
    }
    glPopMatrix();

    glPushMatrix();
    {

     glTranslatef(0,-k,0);
     drawSquarezx(b);
    }
    glPopMatrix();

    glPushMatrix();
    {

    glTranslatef(0,0,b);
     glTranslatef(0,b,0);
    glTranslatef(b,0,0);
     glRotatef(0,0,1,0);
     zdrawSphere(k-b,40,40);
    }
    glPopMatrix();
    glPushMatrix();
    {

    glTranslatef(0,0,b);
     glTranslatef(0,b,0);
    glTranslatef(-b,0,0);
     glRotatef(-90,0,1,0);
     zdrawSphere(k-b,40,40);
    }
    glPopMatrix();

    glPushMatrix();
    {

    glTranslatef(0,0,-b);
     glTranslatef(0,b,0);
    glTranslatef(-b,0,0);
     glRotatef(-180,0,1,0);
     zdrawSphere(k-b,40,40);
    }
    glPopMatrix();

    glPushMatrix();
    {

    glTranslatef(0,0,-b);
     glTranslatef(0,b,0);
    glTranslatef(b,0,0);
     glRotatef(-270,0,1,0);
     zdrawSphere(k-b,40,40);
    }
    glPopMatrix();



    ///////////// lower part



    glPushMatrix();
    {

    glTranslatef(0,0,b);
     glTranslatef(0,-b,0);
    glTranslatef(b,0,0);
     glRotatef(0,0,1,0);
     glRotatef(-90,0,0,1);
     zdrawSphere(k-b,40,40);
    }
    glPopMatrix();
    glPushMatrix();
    {

    glTranslatef(0,0,b);
     glTranslatef(0,-b,0);
    glTranslatef(-b,0,0);
     glRotatef(-90,0,1,0);
     glRotatef(-90,0,0,1);
     zdrawSphere(k-b,40,40);
    }
    glPopMatrix();

    glPushMatrix();
    {

    glTranslatef(0,0,-b);
     glTranslatef(0,-b,0);
    glTranslatef(-b,0,0);
     glRotatef(-180,0,1,0);
     glRotatef(-90,0,0,1);
     zdrawSphere(k-b,40,40);
    }
    glPopMatrix();

    glPushMatrix();
    {glTranslatef(0,0,-b);
     glTranslatef(0,-b,0);
    glTranslatef(b,0,0);
     glRotatef(-270,0,1,0);
     glRotatef(-90,0,0,1);
     zdrawSphere(k-b,40,40);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0,b,0);
    glTranslatef(b,0,0);
     zzdrawCircle(k-b,b,90);
    }
    glPopMatrix();


    glPushMatrix();
    {
        glTranslatef(0,-b,0);
    glTranslatef(b,0,0);
    glRotatef(-90,0,0,1);
     zzdrawCircle(k-b,b,90);
    }
    glPopMatrix();


     glPushMatrix();
    {
        glTranslatef(0,b,0);
    glTranslatef(-b,0,0);
    glRotatef(90,0,0,1);
     zzdrawCircle(k-b,b,90);
    }
    glPopMatrix();


       glPushMatrix();
    {
        glTranslatef(0,-b,0);
    glTranslatef(-b,0,0);
    glRotatef(180,0,0,1);
     zzdrawCircle(k-b,b,90);
    }
    glPopMatrix();


     glPushMatrix();
    {
        glTranslatef(0,0,b);
    glTranslatef(0,b,0);
    glRotatef(-90,0,1,0);
     zzdrawCircle(k-b,b,90);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0,0,b);
    glTranslatef(0,-b,0);
     glRotatef(90,1,0,0);
    glRotatef(-90,0,1,0);
     zzdrawCircle(k-b,b,90);
    }
    glPopMatrix();


    glPushMatrix();
    {
        glTranslatef(0,0,-b);
    glTranslatef(0,-b,0);
    glRotatef(-180,1,0,0);
     glRotatef(-90,0,1,0);

     zzdrawCircle(k-b,b,90);
    }
    glPopMatrix();


     glPushMatrix();
    {
        glTranslatef(0,0,-b);
    glTranslatef(0,-b,0);
     glRotatef(90,1,0,0);
    glRotatef(-270,1,0,0);
    glRotatef(90,0,1,0);
     zzdrawCircle(k-b,b,90);
    }
    glPopMatrix();


     glPushMatrix();
    {

   // glTranslatef(0,0,b);
     glTranslatef(b,0,0);
    glTranslatef(0,0,b);
     glRotatef(90,1,0,0);
      zzdrawCircle(k-b,b,90);
    }glPopMatrix();

    glPushMatrix();
    {

   // glTranslatef(0,0,b);
     glTranslatef(-b,0,0);
    glTranslatef(0,0,b);
    glRotatef(-90,0,1,0);
     glRotatef(90,1,0,0);
      zzdrawCircle(k-b,b,90);
    }glPopMatrix();

    glPushMatrix();
    {

   // glTranslatef(0,0,b);
     glTranslatef(-b,0,0);
    glTranslatef(0,0,-b);
    glRotatef(-180,0,1,0);
     glRotatef(90,1,0,0);
      zzdrawCircle(k-b,b,90);
    }glPopMatrix();


     glPushMatrix();
    {

   // glTranslatef(0,0,b);
     glTranslatef(b,0,0);
    glTranslatef(0,0,-b);
    glRotatef(-270,0,1,0);
     glRotatef(90,1,0,0);
      zzdrawCircle(k-b,b,90);
    }glPopMatrix();

    /*glPushMatrix();
    {

    glTranslatef(0,0,20);
     glTranslatef(0,20,0);
    glTranslatef(20,0,0);
     glRotatef(0,0,1,0);
     zdrawSphere(50,40,40);
    }
    glPopMatrix();

     glPushMatrix();
    {

    glTranslatef(0,0,20);
     glTranslatef(0,20,0);
    glTranslatef(-20,0,0);
     glRotatef(-90,0,1,0);
     zdrawSphere(50,40,40);
    }
    glPopMatrix();

    /*glPushMatrix();
    {
       glScalef(1,.1,1);
      glColor3f(1,1,0);
    glRotatef(angle,0,0,1);
    glTranslatef(0,-200,0);
    //glRotatef(angle,0,0,1);
    glTranslatef(0,-100,0);
    glutWireSphere(50,100, 100);
    }
    glPopMatrix();*/
}

void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {


    case '1':
			{point temp_up=calc(u,l,.02);
			point temp_for=calc(l,u,-.02);
			u=temp_up;
			l=temp_for;}
			break;
         case '2':
			{point temp_up=calc(u,l,-.02);
			point temp_for=calc(l,u,.02);
			u=temp_up;
			l=temp_for;}
			break;
			case '3':
			{point temp_rt=calc(r,l,-.02);
			point temp_for=calc(l,r,.02);
			r=temp_rt;
			l=temp_for;}
			break;
         case '4':
			{point temp_rt=calc(r,l,.02);
			point temp_for=calc(l,r,-.02);
			r=temp_rt;
			l=temp_for;}
			break;
			case '5':
			{point temp_rt=calc(u,r,-.02);
			point temp_for=calc(r,u,.02);
			u=temp_rt;
			r=temp_for;}
			break;
         case '6':
			{point temp_rt=calc(u,r,.02);
			point temp_for=calc(r,u,-.02);
			u=temp_rt;
			r=temp_for;}
			break;
    default:
        break;
    }
}


void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
		    {
                y_pos-=l.y;
                x_pos-=l.x;
                z_pos-=l.z;
            }
			break;
		case GLUT_KEY_UP:		// up arrow key
			 {
                y_pos+=l.y;
                x_pos+=l.x;
                z_pos+=l.z;
            }
			break;

		case GLUT_KEY_RIGHT:
			{
                y_pos+=r.y;
                x_pos+=r.x;
                z_pos+=r.z;
            }
			break;
		case GLUT_KEY_LEFT:
			{
                y_pos=y_pos-r.y;
                x_pos=x_pos-r.x;
                z_pos= z_pos-r.z;
            }
			break;

		case GLUT_KEY_PAGE_UP:
		    {
                y_pos+=u.y;
                x_pos+=u.x;
                z_pos+=u.z;
            }
			break;

			case GLUT_KEY_PAGE_DOWN:
		    {
                y_pos-=u.y;
                x_pos-=u.x;
                z_pos-=u.z;
            }
			break;

    case GLUT_KEY_HOME:
        {
            if(b+.5<=k)
                b=b+.5;
        }
        break;
    case GLUT_KEY_END:
        {
             if(b-.5>=0)
                b=b-.5;
        }
        break;

    default:
        break;
    }
}


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
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



void display()
{

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
    gluLookAt(x_pos,y_pos,z_pos,x_pos+l.x,y_pos+l.y,z_pos+l.z,	u.x,u.y,u.z);


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

    //drawSS();
    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

    //drawSphere(30,24,20);




    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    //angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawgrid=0;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    //angle=0;
    l= {0,0,-1};
    r= {1,0,0};
    u= {0,1,0};

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
    gluPerspective(110,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(1000, 1000);
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
