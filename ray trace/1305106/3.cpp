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
double angle1=0,angle2=0,angle3=0,angle4=0,angle5=0;
double x_pos=0;
double ag=0;

double y_pos=0;

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

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,2);
        glVertex3f( a,-a,2);
        glVertex3f(-a,-a,2);
        glVertex3f(-a, a,2);
    }
    glEnd();
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

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0; i<segments; i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
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


void drawSS()
{
    glPushMatrix();
    {

        glColor3f(1,1,1);
        glRotatef(ag,1,0,0);
         glRotatef(angle1,0,0,1);
        glTranslatef(0,-100,0);
        glScalef(.1,1,.1);
        glutWireSphere(100,4, 10);
    }
    glPopMatrix();

    glPushMatrix();
    {

        glColor3f(1,1,1);
        glRotatef(ag,1,0,0);
        glRotatef(angle1,0,0,1);
        glTranslatef(0,-200,0);
        glRotatef(angle2,0,0,1);
        glTranslatef(0,-80,0);
        glScalef(.05,1,.05);
        glutWireSphere(80,4, 10);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glColor3f(1,1,1);
        glRotatef(ag,1,0,0);
        glRotatef(angle1,0,0,1);
        glTranslatef(0,-200,0);
        glRotatef(angle2,0,0,1);
        glTranslatef(0,-160,0);
        glRotatef(angle3,0,1,0);
      double x1=0,x2=0,y=0;
        for(int i=1; i<=100; i++)
        {



                glBegin(GL_TRIANGLES);
                {
                    glColor3f(1,1,1);
                    glVertex3f(0,0,0);
                    glVertex3f(x1,y,0);
                    glVertex3f(x2,y,0);
                }
                glEnd();

                x1-=.2;x2+=.2;y-=.4;

        }


    }
    glPopMatrix();

    glPushMatrix();
    {

        glColor3f(1,1,1);
        glRotatef(ag,1,0,0);
        glRotatef(angle1,0,0,1);
        glTranslatef(0,-200,0);
        glRotatef(angle2,0,0,1);
        glTranslatef(0,-160,0);
        glRotatef(angle3,0,1,0);
        glTranslatef(-20,0,0);
        glTranslatef(0,-40,0);
        glRotatef(angle4,0,1,0);
        glTranslatef(0,-30,0);
        glScalef(.2,1,.2);
        glutWireSphere(30,5, 5);
    }
    glPopMatrix();

    glPushMatrix();
    {

        glColor3f(1,1,1);
        glRotatef(ag,1,0,0);
        glRotatef(angle1,0,0,1);
        glTranslatef(0,-200,0);
        glRotatef(angle2,0,0,1);
        glTranslatef(0,-160,0);
        glRotatef(angle3,0,1,0);
        glTranslatef(20,0,0);
        glTranslatef(0,-40,0);
        glRotatef(angle5,1,0,0);
        glTranslatef(0,-30,0);
        glScalef(.2,1,.2);
        glutWireSphere(30,5, 5);
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
    {
        angle1+=.7;
    }
    break;
    case '2':
    {
        angle1-=.7;
    }
    break;
    case '3':
    {
        angle2+=.7;
    }
    break;
    case '4':
    {
        angle2-=.7;
    }
    break;
    case '5':
    {
        angle3+=.7;
    }
    break;
    case '6':
    {
        angle3-=.7;
    }
    break;
     case '7':
    {
        angle4+=.7;
    }
    break;
    case '8':
    {
        angle4-=.7;
    }
    break;
    case '9':
    {
        angle5+=.7;
    }
    break;
    case '0':
    {
        angle5-=.7;
    }
    break;
    case 'q':
    {
        ag-=.7;
    }
    break;
    case 'w':
    {
        ag+=.7;
    }
    break;
    case 'u':
    {
        point temp_up=calc(u,l,.05);
        point temp_for=calc(l,u,-.05);
        u=temp_up;
        l=temp_for;
    }
    break;
    case 'n':
    {
        point temp_up=calc(u,l,-.05);
        point temp_for=calc(l,u,.05);
        u=temp_up;
        l=temp_for;
    }
    break;
    case 'k':
    {
        point temp_rt=calc(r,l,-.05);
        point temp_for=calc(l,r,.05);
        r=temp_rt;
        l=temp_for;
    }
    break;
    case 'h':
    {
        point temp_rt=calc(r,l,.05);
        point temp_for=calc(l,r,-.05);
        r=temp_rt;
        l=temp_for;
    }
    break;
    case 'o':
    {
        point temp_rt=calc(u,r,-.05);
        point temp_for=calc(r,u,.05);
        u=temp_rt;
        r=temp_for;
    }
    break;
    case 'p':
    {
        point temp_rt=calc(u,r,.05);
        point temp_for=calc(r,u,-.05);
        u=temp_rt;
        r=temp_for;
    }
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
