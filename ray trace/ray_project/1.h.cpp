/***#include<stdio.h>
#include<bits/c++io.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
extern vector <Object> objects;
extern vector <Vector3> lights;***/



class Vector3
{
public:
    double x,y,z;
    point() {}
    point(double x,double y,double z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    void pro()
    {
        //cout<<this->x<<" "<<this->y<<endl;
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
    virtual void draw() {}

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

class Sphere: public Object
{
public:
    Sphere(Vector3 Center, double Radius)
    {
        this->reference_point = Center;
        this->length = Radius;
    }

    void draw()
    {
        //write codes for drawing sphere
    }

};

