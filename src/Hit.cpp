#include "Hit.h"


// Hit ------------------------------------------------------------


// constructors
Hit::Hit()
{
    material = NULL;
    t = FLT_MAX;
    hasTex=false;
}


Hit::Hit( float _t, Material* m, const Vector3f& n )
{ 
    t = _t;
    material = m;
    normal = n;
    hasTex=false;
}


Hit::Hit( const Hit& h )
{ 
    t = h.t; 
    material = h.material; 
    normal = h.normal;
    hasTex=h.hasTex;
}

// destructor
Hit::~Hit()
{

}


float Hit::getT() const
{
    return t;
}


Material* Hit::getMaterial() const
{
    return material;
}


const Vector3f& Hit::getNormal() const
{
    return normal;
}


void Hit::set(float _t, Material* m, const Vector3f& n, bool _hasTex)
{
    t = _t;
    material = m;
    normal = n;
    this->hasTex = _hasTex;
}


void Hit::setTexCoord(const Vector2f & coord){
    texCoord = coord;
}


// Helper functions ------------------------------------------------------------------------


inline ostream& operator << ( ostream &os, const Hit& h)
{
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}