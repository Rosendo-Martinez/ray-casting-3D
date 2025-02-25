#ifndef RAY_H
#define RAY_H

#include <cassert>
#include <iostream>
#include <Vector3f.h>

using namespace std;


// Ray class mostly copied from Peter Shirley and Keith Morley
class Ray
{
public:
    Ray( const Vector3f& orig, const Vector3f& dir );
    Ray( const Ray& r );

    const Vector3f& getOrigin() const;
    const Vector3f& getDirection() const;
    Vector3f pointAtParameter( float t ) const;

private:
    // don't use this constructor
    Ray();

    Vector3f origin;
    Vector3f direction;
};


inline ostream& operator << ( ostream& os, const Ray& r);


#endif // RAY_H