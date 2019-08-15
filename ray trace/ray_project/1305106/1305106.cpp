#include<stdio.h>
#include<bits/c++io.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))
#include <bits/stdc++.h>
#include <math.h>
#include "bitmap_image.hpp"

int recursion_level=3;

using namespace std;

//double col[4];
double sub_col[4];

double Window_width= 500, Window_height=500;

double image_width;

void capture();
class Object;
class Vector3;
vector <Object*>  objects;
vector <Vector3> lights;


class Vector3
{
public:
    double x,y,z;
    Vector3() {}
    Vector3(double x,double y,double z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    void pro()
    {
        cout<<this->x<<" "<<this->y<<" "<<this->z<<endl;
    }
    void normalize()
    {
        double tt = x*x+y*y+z*z;
        tt=sqrt(tt);

        this->x =  this->x/tt;
        this->y =  this->y/tt;
        this->z =  this->z/tt;
    }
};



class Ray
{
public:
    Vector3 start;
    Vector3 dir;

    Ray() {}
    Ray(Vector3 x, Vector3 y)
    {
        start = x;
        dir  = y;
    }


};

class Object
{
public:

    Vector3 reference_point;
    double height, width, length;
    int Shine;
    double color[3];
    double co_efficients[4];



    Object() { }

    virtual void draw() = 0;

    virtual double getIntersectingT(Ray r) = 0;


    virtual double intersect(Ray r,  int level,double *current_color) = 0;



    void setDimension(double x, double y, double z)
    {
        this->height = x;
        this->width = y;
        this->length = z;
    }

    void setColor(double x, double y, double z)
    {
        this->color[0] = x;
        this->color[1] = y;
        this->color[2] = z;
    }
    void setShine(int x)
    {
        this->Shine = x;
    }
    void setCoEfficients(double x, double y, double z, double v)
    {
        this->co_efficients[0] = x;
        this->co_efficients[1] = y;
        this->co_efficients[2] = z;
        this->co_efficients[3] = v;
    }
};
void drawSphere(double radius,int slices,int stacks,Vector3 vv );

class Sphere: public Object
{
public:
    Sphere(Vector3 Center, double Radius)
    {
        this->reference_point = Center;
        this->length = Radius;
    }
    double getIntersectingT(Ray r)
    {




        double a = 1;

        Vector3 pp(r.start.x-reference_point.x,r.start.y-reference_point.y,r.start.z-reference_point.z);


        double b = r.dir.x*pp.x + r.dir.y*pp.y + r.dir.z*pp.z;
        b = b*2;
        double c = pp.x*pp.x + pp.y*pp.y + pp.z*pp.z - (length*length);
        double d = b*b - 4*a*c;

        if(d<0)
            return -1;
        d= sqrt(d);





        double l1 = (-b + d)/(2*a);
        double l2 = (-b - d)/(2*a);

        if(l1<=l2)
            return l1;
        else
            return l2;

}

    void setColorAt(double *col)
    {


        col[0] =  color[0]*co_efficients[0];
        col[1] =  color[1]*co_efficients[0];
        col[2] =  color[2]*co_efficients[0];
    }



    Vector3 getNormal(Vector3 intersectionPoint)
    {
        Vector3 norm(intersectionPoint.x-reference_point.x,intersectionPoint.y-reference_point.y,intersectionPoint.z-reference_point.z);
        norm.normalize();
        return norm;
    }

    Vector3 getReflection(Ray ray, Vector3 intersectionPoint, Vector3 normal)
    {


        double t = ray.dir.x*normal.x+ray.dir.y*normal.y+ray.dir.z*normal.z;

        Vector3 sec(normal.x*t*2,normal.y*t*2,normal.z*t*2 );

        Vector3 ans(ray.dir.x-sec.x,ray.dir.y-sec.y,ray.dir.z-sec.z);

        ans.normalize();

        return ans;
    }

    double calc_phong(Vector3 reflection,Ray r,int Shine)
    {
        double temp = reflection.x*r.dir.x+reflection.y*r.dir.y+reflection.z*r.dir.z;
        return pow(temp, Shine);
    }

    double calc_lam(Vector3 l, Vector3 n)
    {
        return l.x*n.x +l.y*n.y +l.z*n.z ;
    }

    double intersect(Ray r,  int level,double *col)
    {

        double t = getIntersectingT( r);

        if (t<=0)
            return -1;
        if(level==0)
            return t;










        Vector3 intersectionPoint(r.start.x+r.dir.x*t,r.start.y+r.dir.y*t,r.start.z+r.dir.z*t);



        setColorAt(col);

        Vector3 normal=getNormal(intersectionPoint);

        Vector3 reflection=getReflection(r, intersectionPoint, normal);

        for (int lt=0; lt<lights.size(); lt++)
        {
            Vector3 d(lights[lt].x-intersectionPoint.x,lights[lt].y-intersectionPoint.y,lights[lt].z-intersectionPoint.z);

            double dist = d.x*d.x + d.y*d.y + d.z*d.z;
            dist = sqrt(dist);
            d.normalize();

            Vector3 start(intersectionPoint.x+d.x,intersectionPoint.y+d.y,intersectionPoint.z+d.z);

            Ray L(start,d);

            if(normal.x*d.x+normal.y*d.y+normal.z*d.z <0)
                continue;



            int i;
            for (i=0; i<objects.size(); i++)
            {
                double t = objects[i]->getIntersectingT(L);

                if(t <=0 )
                {
                    continue;
                }

                if(t>dist)
                    continue;

                break;
            }

            if(i==objects.size())
            {







                double lam = calc_lam(L.dir, normal);
                if(lam<0)
                    lam = 0;

                double phong = calc_phong(reflection,r,Shine);
                if(phong<0)
                    phong = 0;


                for (int k=0; k<3; k++)
                {
                    col[k] += 1 * lam * co_efficients[1] * color[k];
                    col[k] += 1 * phong * co_efficients[2] * color[k];
                }

            }

            if(level<recursion_level)
            {


                Vector3 st(intersectionPoint.x+reflection.x,intersectionPoint.y+reflection.y,intersectionPoint.z+reflection.z);

                Ray ry(st, reflection);



                int nearest = -1;
                double dist_ref = 1000000000;

                double ref_col[4];


                for(int id=0; id<objects.size(); id++)
                {


                    ref_col[0]=0;
                    ref_col[1]=0;
                    ref_col[2]=0;
                    double temp =  objects[id]->intersect(ry, 0, ref_col);

                    if(temp<dist_ref && temp>=0)
                    {
                        dist_ref = temp;
                        nearest = id;

                    }
                }

                if(nearest!=-1)
                {
                    ref_col[0]=0;
                    ref_col[1]=0;
                    ref_col[2]=0;
                    double temp =  objects[nearest]->intersect(ry,level+1, ref_col);

                    for (int g=0; g<3; g++)
                    {
                        col[g]+=ref_col[g]*co_efficients[3];
                    }

                }
            }








        }

        for (int k=0; k<3; k++)
        {
            col[k]=min(col[k],1.0);
            col[k]=max(col[k],0.0);
        }


        return t;
    }

    void draw()
    {

        glPushMatrix();
        {

            glTranslatef(reference_point.x, reference_point.y, reference_point.z);
            Vector3 vv(color[0],color[1],color[2]);

            drawSphere(length, 40, 40,vv);

        }
        glPopMatrix();


    }

};

class Triangle: public Object
{
    public:
    Vector3 corners[3];


    Triangle(Vector3 a, Vector3 b, Vector3 c)
    {

        corners[0]=a;
        corners[1]=b;
        corners[2]=c;
    }

    void draw()
    {

        glPushMatrix();
        {





            glColor3f(color[0],color[1],color[2]);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(corners[0].x, corners[0].y, corners[0].z);
                glVertex3f(corners[1].x, corners[1].y, corners[1].z);
                glVertex3f(corners[2].x, corners[2].y, corners[2].z);
            }
            glEnd();



        }
        glPopMatrix();


    }



    void setColorAt(double *col,Vector3 intersectionPoint)
    {

        col[0] =  color[0]*co_efficients[0];
        col[1] =  color[1]*co_efficients[0];
        col[2] =  color[2]*co_efficients[0];
    }


    Vector3 cross_mul(Vector3 a,Vector3 b)
    {
        double vect_A[3],vect_B[3];

        vect_A[0]=a.x;
        vect_A[1]=a.y;
        vect_A[2]=a.z;

        vect_B[0]=b.x;
        vect_B[1]=b.y;
        vect_B[2]=b.z;

        double x= vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
        double y= vect_A[0] * vect_B[2] - vect_A[2] * vect_B[0];
        double z= vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];

        Vector3 t(x,-y,z);
        return t;
    }

    Vector3 getNormal(Vector3 intersectionPoint)
    {


        Vector3 x(corners[1].x-corners[0].x,corners[1].y-corners[0].y,corners[1].z-corners[0].z);
        Vector3 y(corners[2].x-corners[0].x,corners[2].y-corners[0].y,corners[2].z-corners[0].z);

        Vector3 ans = cross_mul(x,y);

        ans.normalize();


        return ans;
    }

    double getIntersectingT(Ray r)
    {




            double a,f,u,v;


            double eps = 0.0000001;

            Vector3 e1(corners[1].x - corners[0].x,corners[1].y - corners[0].y,corners[1].z - corners[0].z);
            Vector3 e2(corners[2].x - corners[0].x,corners[2].y - corners[0].y,corners[2].z - corners[0].z);



            Vector3 h = cross_mul(r.dir, e2);



            a = e1.x*h.x + e1.y*h.y + e1.z*h.z ;

            if (a > -eps && a < eps)
                return -1;

            f = 1/a;



            Vector3 s(r.start.x-corners[0].x,r.start.y-corners[0].y,r.start.z-corners[0].z);

            u = f *(s.x*h.x+s.y*h.y+s.z*h.z);

            if (u < 0.0 || u > 1.0)
                return -1;




            Vector3 q = cross_mul(s,e1);

            v = f * (r.dir.x*q.x+r.dir.y*q.y+r.dir.z*q.z);
            if (v < 0.0 || u + v > 1.0)
                return -1;

            double t = f * (e2.x*q.x+e2.y*q.y+e2.z*q.z);
            if (t >eps)
            {

                return t;
            }
            else
                return -1;




    }

    Vector3 getReflection(Ray ray, Vector3 intersectionPoint, Vector3 normal)
    {


        double t = ray.dir.x*normal.x+ray.dir.y*normal.y+ray.dir.z*normal.z;

        Vector3 sec(normal.x*t*2,normal.y*t*2,normal.z*t*2 );

        Vector3 ans(ray.dir.x-sec.x,ray.dir.y-sec.y,ray.dir.z-sec.z);

        ans.normalize();

        return ans;
    }

    double calc_phong(Vector3 reflection,Ray r,int Shine)
    {


        double temp = reflection.x*r.dir.x+reflection.y*r.dir.y+reflection.z*r.dir.z;
        return pow(temp, Shine);
    }

    double calc_lam(Vector3 l, Vector3 n)
    {
        return l.x*n.x +l.y*n.y +l.z*n.z ;
    }

    double intersect(Ray r,  int level,double *col)
    {



        double t = getIntersectingT( r);

        if (t<=0)
            return -1;
        if(level==0)
            return t;

        Vector3 intersect_point(r.start.x+t*r.dir.x,r.start.y+t*r.dir.y,r.start.z+t*r.dir.z);











        Vector3 intersectionPoint(r.start.x+r.dir.x*t,r.start.y+r.dir.y*t,r.start.z+r.dir.z*t);



        setColorAt(col,intersectionPoint);



        Vector3 normal=getNormal(intersectionPoint);

        Vector3 reflection=getReflection(r, intersectionPoint, normal);

        for (int lt=0; lt<lights.size(); lt++)
        {
            Vector3 d(lights[lt].x-intersectionPoint.x,lights[lt].y-intersectionPoint.y,lights[lt].z-intersectionPoint.z);

            double dist = d.x*d.x + d.y*d.y + d.z*d.z;
            dist = sqrt(dist);
            d.normalize();

            Vector3 start(intersectionPoint.x+d.x,intersectionPoint.y+d.y,intersectionPoint.z+d.z);

            Ray L(start,d);

            if(normal.x*d.x+normal.y*d.y+normal.z*d.z <0)
                continue;



            int i;
            for (i=0; i<objects.size(); i++)
            {
                double t = objects[i]->getIntersectingT(L);

                if(t <=0 )
                {
                    continue;
                }

                if(t>dist)
                    continue;

                break;
            }

            if(i==objects.size())
            {







                double lam = calc_lam(L.dir, normal);
                if(lam<0)
                    lam = 0;

                double phong = calc_phong(reflection,r,Shine);
                if(phong<0)
                    phong = 0;


                for (int k=0; k<3; k++)
                {
                    col[k] += 1 * lam * co_efficients[1] * color[k];
                    col[k] += 1 * phong * co_efficients[2] * color[k];
                }

            }

            if(level<recursion_level)
            {


                Vector3 st(intersectionPoint.x+reflection.x,intersectionPoint.y+reflection.y,intersectionPoint.z+reflection.z);

                Ray ry(st, reflection);



                int nearest = -1;
                double dist_ref = 1000000000;

                double ref_col[4];


                for(int id=0; id<objects.size(); id++)
                {


                    ref_col[0]=0;
                    ref_col[1]=0;
                    ref_col[2]=0;
                    double temp =  objects[id]->intersect(ry, 0, ref_col);

                    if(temp<dist_ref && temp>=0)
                    {
                        dist_ref = temp;
                        nearest = id;

                    }
                }

                if(nearest!=-1)
                {
                    ref_col[0]=0;
                    ref_col[1]=0;
                    ref_col[2]=0;
                    double temp =  objects[nearest]->intersect(ry,level+1, ref_col);

                    for (int g=0; g<3; g++)
                    {
                        col[g]+=ref_col[g]*co_efficients[3];
                    }

                }
            }








        }

        for (int k=0; k<3; k++)
        {
            col[k]=min(col[k],1.0);
            col[k]=max(col[k],0.0);
        }


        return t;
    }
};

class Floor: public Object
{
public:


    Floor(double FloorWidth, double TileWidth)
    {

        Vector3 rf(-FloorWidth/2, -FloorWidth/2,  0);
        reference_point = rf;
        length = TileWidth;
    }

    void draw()
    {

        glPushMatrix();
        {



            int tot_tiles = (-reference_point.x*2)/length;

            int seq = 0;

            int cnt = 0;
            int x_count;
            int y_count;

            for (double x_cord = reference_point.x, x_count=0; x_count<=tot_tiles; x_cord+= length, x_count++)
                for (double y_cord = reference_point.y, y_count=0; y_count<=tot_tiles; y_cord+= length, y_count++)
                {
                    int p = x_count;
                    int q =y_count;
                    if ((p+q)%2 == 0)
                    {
                        glColor3f(0, 0, 0);
                    }
                    else
                    {
                        glColor3f(1, 1, 1);
                    }

                    glBegin(GL_QUADS);
                    {
                        glVertex3f(x_cord, y_cord, reference_point.z);
                        glVertex3f(x_cord+length, y_cord, reference_point.z);
                        glVertex3f(x_cord+length, y_cord+length, reference_point.z);
                        glVertex3f(x_cord, y_cord+length, reference_point.z);

                    }
                    glEnd();

                }

        }
        glPopMatrix();


    }



    void setColorAt(double *col,Vector3 intersectionPoint)
    {


        int mod_ck = (int)((intersectionPoint.x-(reference_point.x))/length )+ (int)((intersectionPoint.y-(reference_point.y))/length);








        if((mod_ck%2)==1)
        {


            col[0] =  0;
            col[1] =  0;
            col[2] =  0;
        }
        else
        {
            col[0] =  co_efficients[0];
            col[1] =  co_efficients[0];
            col[2] =  co_efficients[0];

      }


    }

    void getColorAt(Vector3 intersectionPoint)
    {

    }

    Vector3 getNormal(Vector3 intersectionPoint)
    {
        Vector3 norm(0,0,1);
        return norm;
    }

    double getIntersectingT(Ray r)
    {



        if(r.dir.z == 0)
            return -1;

        return -(r.start.z/r.dir.z);






    }

    Vector3 getReflection(Ray ray, Vector3 intersectionPoint, Vector3 normal)
    {


        double t = ray.dir.x*normal.x+ray.dir.y*normal.y+ray.dir.z*normal.z;

        Vector3 sec(normal.x*t*2,normal.y*t*2,normal.z*t*2 );

        Vector3 ans(ray.dir.x-sec.x,ray.dir.y-sec.y,ray.dir.z-sec.z);

        ans.normalize();

        return ans;
    }

    double calc_phong(Vector3 reflection,Ray r,int Shine)
    {


        double temp = reflection.x*r.dir.x+reflection.y*r.dir.y+reflection.z*r.dir.z;
        return pow(temp, Shine);
    }

    double calc_lam(Vector3 l, Vector3 n)
    {
        return l.x*n.x +l.y*n.y +l.z*n.z ;
    }

    bool not_within(Vector3 intersect_point)
    {
        if(reference_point.x>intersect_point.x || -reference_point.x<intersect_point.x)
            return true;
        if(reference_point.y>intersect_point.y || -reference_point.y<intersect_point.y)
            return true;

        return false;
    }

    double intersect(Ray r,  int level,double *col)
    {



        double t = getIntersectingT( r);

        if (t<=0)
            return -1;
        if(level==0)
            return t;

        Vector3 intersect_point(r.start.x+t*r.dir.x,r.start.y+t*r.dir.y,r.start.z+t*r.dir.z);


        if(not_within(intersect_point))
        {

            return -1;
        }











        Vector3 intersectionPoint(r.start.x+r.dir.x*t,r.start.y+r.dir.y*t,r.start.z+r.dir.z*t);



        setColorAt(col,intersectionPoint);



        Vector3 normal=getNormal(intersectionPoint);

        Vector3 reflection=getReflection(r, intersectionPoint, normal);

        for (int lt=0; lt<lights.size(); lt++)
        {
            Vector3 d(lights[lt].x-intersectionPoint.x,lights[lt].y-intersectionPoint.y,lights[lt].z-intersectionPoint.z);

            double dist = d.x*d.x + d.y*d.y + d.z*d.z;
            dist = sqrt(dist);
            d.normalize();

            Vector3 start(intersectionPoint.x+d.x,intersectionPoint.y+d.y,intersectionPoint.z+d.z);

            Ray L(start,d);

            if(normal.x*d.x+normal.y*d.y+normal.z*d.z <0)
                continue;



            int i;
            for (i=0; i<objects.size(); i++)
            {
                double t = objects[i]->getIntersectingT(L);

                if(t <=0 )
                {
                    continue;
                }

                if(t>dist)
                    continue;

                break;
            }

            if(i==objects.size())
            {







                double lam = calc_lam(L.dir, normal);
                if(lam<0)
                    lam = 0;

                double phong = calc_phong(reflection,r,Shine);
                if(phong<0)
                    phong = 0;


                for (int k=0; k<3; k++)
                {
                    col[k] += 1 * lam * co_efficients[1] * color[k];
                    col[k] += 1 * phong * co_efficients[2] * color[k];
                }

            }

            if(level<recursion_level)
            {


                Vector3 st(intersectionPoint.x+reflection.x,intersectionPoint.y+reflection.y,intersectionPoint.z+reflection.z);

                Ray ry(st, reflection);



                int nearest = -1;
                double dist_ref = 1000000000;

                double ref_col[4];


                for(int id=0; id<objects.size(); id++)
                {


                    ref_col[0]=0;
                    ref_col[1]=0;
                    ref_col[2]=0;
                    double temp =  objects[id]->intersect(ry, 0, ref_col);

                    if(temp<dist_ref && temp>=0)
                    {
                        dist_ref = temp;
                        nearest = id;

                    }
                }

                if(nearest!=-1)
                {
                    ref_col[0]=0;
                    ref_col[1]=0;
                    ref_col[2]=0;
                    double temp =  objects[nearest]->intersect(ry,level+1, ref_col);

                    for (int g=0; g<3; g++)
                    {
                        col[g]+=ref_col[g]*co_efficients[3];
                    }

                }
            }








        }

        for (int k=0; k<3; k++)
        {
            col[k]=min(col[k],1.0);
            col[k]=max(col[k],0.0);
        }


        return t;
    }
};




double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;


double angle;



struct point
{
    double x,y,z;
};

point u,r,l,pos;

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
        glColor3f(0.6, 0.6, 0.6);
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









void drawSphere(double radius,int slices,int stacks,Vector3 vv)
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
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

        glColor3f(vv.x,vv.y,vv.z);
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


    for(int i=0; i<objects.size(); i++)
    {


        objects[i]->draw();


    }

    for(int i=0; i<lights.size(); i++)
    {

        glPushMatrix();
        {

            glTranslatef(lights[i].x, lights[i].y, lights[i].z);
            Vector3 vv(1,0,1);

            drawSphere(4, 40, 40,vv);

        }
        glPopMatrix();





    }







}


point f3(point vect, point perp, int dir)
{
    double c = cos(pi/180);
    double s = dir * sin(pi/180);
    point p;
    p.x = c * vect.x + s * perp.x;
    p.y = c * vect.y + s * perp.y;
    p.z = c * vect.z + s * perp.z;
    c = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    p.x /= c;
    p.y /= c;
    p.z /= c;
    return p;
}

void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {



       case '1': {
            point l1 = f3(l, r, -1);
            r = f3(r, l, 1);
            l = l1;
            break;
		}
		case '2': {
            point l1 = f3(l, r, 1);
            r = f3(r, l, -1);
            l = l1;
            break;
		}

		case '3': {
            point u1 = f3(u, l, -1);
            l = f3(l, u, 1);
            u = u1;
            break;
        }
        case '4': {
            point u1 = f3(u, l, 1);
            l = f3(l, u, -1);
            u = u1;
            break;
        }
        case '5': {
            point r1 = f3(r, u, -1);
            u = f3(u, r, 1);
            r = r1;
            break;
        }
        case '6':{
            point r1 = f3(r, u, 1);
            u = f3(u, r, -1);
            r = r1;
            break;
        }
    case '0':
        capture();
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
                pos.y-=l.y;
                pos.x-=l.x;
                pos.z-=l.z;
            }
			break;
		case GLUT_KEY_UP:		// up arrow key
			 {
                pos.y+=l.y;
                pos.x+=l.x;
                pos.z+=l.z;
            }
			break;

		case GLUT_KEY_RIGHT:
			{
                pos.y+=r.y;
                pos.x+=r.x;
                pos.z+=r.z;
            }
			break;
		case GLUT_KEY_LEFT:
			{
                pos.y=pos.y-r.y;
                pos.x=pos.x-r.x;
                pos.z= pos.z-r.z;
            }
			break;

		case GLUT_KEY_PAGE_UP:
		    {
                pos.y+=u.y;
                pos.x+=u.x;
                pos.z+=u.z;
            }
			break;

			case GLUT_KEY_PAGE_DOWN:
		    {
                pos.y-=u.y;
                pos.x-=u.x;
                pos.z-=u.z;
            }
			break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        // if(sphr<60){sphr+=2;sqr-=2;cylr+=2;cylh-=4;sphz-=2;cylz-=2;}
        break;
    case GLUT_KEY_END:
        // if(sphr>0){sphr-=2;sqr+=2;cylr-=2;cylh+=4;sphz+=2;cylz+=2;}
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

    //ialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);



    glMatrixMode(GL_MODELVIEW);



    drawAxes();
    drawGrid();


    drawSS();


    glutSwapBuffers();
}


void animate()
{

    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawgrid=0;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;


    l= {1,0,0};
    r= {0,-1,0};
    u= {0,0,1};
    pos.x=0;
    pos.y=-100;
    pos.z=10;
    u.x=0;
    u.y=0;
    u.z=1;
    l.x=0;
    l.y=1;
    l.z=0;
    r.x=1;
    r.y=0;
    r.z=0;

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

Vector3 scalar_mul(Vector3 a,double val)
{
    Vector3 t(a.x*val,a.y*val,a.z*val);
    return t;
}

Vector3 addi(Vector3 a,Vector3 b)
{
    Vector3 temp(a.x+b.x,a.y+b.y,a.z+b.z);
    return temp;
}

void loadTestData()
{

    image_width = 2000;



    Object *temp;
    Vector3 ct(0,0,10);
    temp=new Sphere(ct, 10);
    temp->setColor(1,.5,1);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);

    objects.push_back(temp);

    Object *temp1;

    Vector3 ct1(0,30,10);
    temp1=new Sphere(ct1, 15); // Center(0,0,10), Radius 10
    temp1->setColor(.5,1,.5);
    temp1->setCoEfficients(0.4,0.2,0.2,0.2);
    temp1->setShine(1);

    objects.push_back(temp1);

    Object *temp11;
    Vector3 ct11(-35,10,40);
    temp11=new Sphere(ct11, 10); // Center(0,0,10), Radius 10
    temp11->setColor(.2,.3,.7);
    temp11->setCoEfficients(0.4,0.2,0.2,0.2);
    temp11->setShine(1);

    objects.push_back(temp11);


    Object *temp13;

    Vector3 ct33(30,30,30);
    temp13=new Sphere(ct33, 20); // Center(0,0,10), Radius 10
    temp13->setColor(.8,.8,.2);
    temp13->setCoEfficients(0.4,0.2,0.2,0.2);
    temp13->setShine(1);

    objects.push_back(temp13);

    Object *tri;

    Vector3 p1(-50,60,20);
    Vector3 p2(-50,40,20);
    Vector3 p3(0,30,40);
    tri=new Triangle(p1,p2,p3);
    tri->setColor(.2,.3,.9);
    tri->setCoEfficients(0.4,0.2,0.2,0.2);
    tri->setShine(1);

    objects.push_back(tri);



    Vector3 light3(0,0,100);
    lights.push_back(light3);


    Vector3 light1(0,100,0);
    lights.push_back(light1);


    Vector3 light22(0,100,200);
    lights.push_back(light22);




    Object *temp21;
    temp21=new Floor(1000, 20);
    temp21->setCoEfficients(0.4,0.2,0.2,0.2);
    temp21->setShine(1);
    objects.push_back(temp21);


}

void capture()
{

    bitmap_image image(image_width, image_width);

    for(int i=0; i<image_width; i++)
    {
        for(int j=0; j<image_width; j++)
        {
            image.set_pixel(i,j,0,0,0);
        }
    }



    double plane_distance= (500/2)/tan(110*pi/360);


    Vector3 look(l.x, l.y, l.z);
    Vector3 up(u.x, u.y, u.z);
    Vector3 right(r.x, r.y, r.z);

    look.normalize();up.normalize();right.normalize();


    Vector3 topleft;
    topleft.x= pos.x + l.x*plane_distance -r.x*Window_width/2+ u.x*Window_height/2;
    topleft.y= pos.y + l.y*plane_distance -r.y*Window_width/2+ u.y*Window_height/2;
    topleft.z= pos.z + l.z*plane_distance -r.z*Window_width/2+ u.z*Window_height/2;

    double du=Window_width/image_width;
    double dv=Window_height/image_width;



    for (int i=0; i<image_width; i++)
        for (int j=0; j<image_width; j++)
        {


            Vector3 cor;



            cor.x = topleft.x + r.x*j*du - u.x*i*dv;
            cor.y = topleft.y + r.y*j*du - u.y*i*dv;
            cor.z = topleft.z + r.z*j*du - u.z*i*dv;


            Vector3  d(cor.x-pos.x, cor.y-pos.y, cor.z-pos.z);

            double tt = d.x*d.x + d.y*d.y + d.z*d.z;
            tt=sqrt(tt);

            d.x = d.x/tt;
            d.y = d.y/tt;
            d.z = d.z/tt;

            Vector3 hh(pos.x,pos.y,pos.z);
            Ray ray(hh, d);

            int nearest = -1;
            double dist_ref = 1000000000;

            double col[4];


            for(int id=0; id<objects.size(); id++)
            {


                col[0]=0;
                col[1]=0;
                col[2]=0;
                double temp =  objects[id]->getIntersectingT(ray);

                if(temp<dist_ref && temp>=0)
                {
                    dist_ref = temp;
                    nearest = id;

                }
            }

            if(nearest!=-1)
            {
                col[0]=0;
                col[1]=0;
                col[2]=0;
                double temp =  objects[nearest]->intersect(ray,1, col);

                image.set_pixel(j,i,col[0]*255,col[1]*255,col[2]*255);


            }
        }

    cout<<" image save"<<endl;




    image.save_image("2.bmp");

}

int main(int argc, char **argv)
{

    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    loadTestData();



    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
