#ifndef __camera_h__
#define __camera_h__


#define UNIT 5

#include <math.h>
#include<custom/vectors.h>


FlynnVector3 u,l,r,pos;

void up(float unit){
    pos += u*unit;
}
void down(float unit){
    pos -= u*unit;
}
void forward(float unit){
    pos += l*unit;
}
void backward(float unit){
    pos -= l*unit;
}
void left(float unit){
    pos -= r*unit;
}
void right(float unit){
    pos += r*unit;
}

void rotate_left(float angle){
    l = l.rotate(angle,u);
    r = l.cross(u);
}
void rotate_right(float angle){
    l = l.rotate(-angle,u);
    r = l.cross(u);
}
void rotate_up(float angle){
    l = l.rotate(angle,r);
    u = r.cross(l);
}
void rotate_down(float angle){
    l = l.rotate(-angle,r);
    u = r.cross(l);
}
void tilt(float angle){
    u = u.rotate(angle,l);
    r = l.cross(u);
}



#endif
