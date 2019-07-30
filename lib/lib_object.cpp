#include "lib_object.h"

namespace sdl2_lib
{
Object::Object(double w, double h, double angle)
{
    Reset(w, h, angle);
}
void Object::Reset(double w, double h, double angle)
{
    //ctor
    this->base_w = w;
    this->base_h = h;
    this->w = w;
    this->h = h;
    this->base_ang = angle;
}

Object::~Object()
{
    //dtor
}

void Object::setCenter(bool value)
{
    int dif = value ? 1 : 0;
    // compute the diference between now and then
    // int dif = (value == true) - (isCentered == true);
    // Off -> On (is = false, value = true) = 1
    // Off -> Off or On -> On = 0
    // On -> Off (is = true, value = false) = -1

    // Let's just apply it ;)
    deltax = - (dif * w) / 2;
    deltay = - (dif * h) / 2;
}

void Object::updateSize()
{
    w = base_w * zoomx;
    h = base_h * zoomy;
}
void Object::setSize(double w, double h)
{
    this->w = w;
    this->h = h;
}
void Object::setZoom(double Zoomx, double Zoomy)
{
    zoomx = Zoomx;
    zoomy = Zoomy;
    updateSize();
}

void Object::setPos(double x, double y)
{
    posx = x;
    posy = y;
}

void Object::setAng(double ang)
{
    this->ang = ang;
}

void Object::drawAt(double posx, double posy, double ang, Flip FLIP)
{
    internal_draw(posx + deltax, posy + deltay, ang, FLIP);
}
void Object::draw(Flip FLIP)
{
    internal_draw(posx + deltax, posy + deltay, base_ang + ang, FLIP);
}
void Object::draw(int ox, int oy, Flip FLIP)
{
    internal_draw(posx + deltax, posy + deltay, ox , oy, base_ang + ang, FLIP);
}

} // namespace sdl2_lib
