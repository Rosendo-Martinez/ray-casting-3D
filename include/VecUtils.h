#ifndef VEC_UTILS_H
#define VEC_UTILS_H

#include <vecmath.h>


class VecUtils
{
public:

    static Vector3f min( const Vector3f& b, const Vector3f& c );

    static Vector3f max( const Vector3f& b, const Vector3f& c );

    static Vector3f clamp( const Vector3f& data, float low = 0, float high = 1 );

    // transforms a 3D point using a matrix, returning a 3D point
    static Vector3f transformPoint( const Matrix4f& mat, const Vector3f& point );

    // transform a 3D directino using a matrix, returning a direction
    // This function *does not* take the inverse tranpose for you.
    static Vector3f transformDirection( const Matrix4f& mat, const Vector3f& dir );
};


#endif // VEC_UTILS_H
