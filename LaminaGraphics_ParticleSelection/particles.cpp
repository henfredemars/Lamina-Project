
#include "particles.h"

Particles::Particles(float xin, float yin, float zin, float radiusin) {
    x = xin;
    y = yin;
    z = zin;
    radius = radiusin;
}

float Particles::getX() {
    return x;
}
float Particles::getY() {
    return y;
}
float Particles::getZ() {
    return z;
}
float Particles::getRadius() {
    return radius;
}
