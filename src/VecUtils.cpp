#include "VecUtils.h"


Vector3f VecUtils::min( const Vector3f& b, const Vector3f& c )
{
    Vector3f out;

    for( int i = 0; i < 3; ++i )
    {
        out[ i ] = ( b[i] < c[i] ) ? b[i] : c[i];
    }

    return out;
}


Vector3f VecUtils::max( const Vector3f& b, const Vector3f& c )
{
    Vector3f out;

    for( int i = 0; i < 3; ++i )
    {
        out[ i ] = ( b[i] > c[i] ) ? b[i] : c[i];
    }

    return out;
}


Vector3f VecUtils::clamp( const Vector3f& data, float low, float high)
{
    Vector3f out = data;
    for( int i = 0; i < 3; ++i )
    {
        if( out[ i ] < low )
        {
            out[ i ] = low;
        }
        if( out[ i ] > high )
        {
            out[ i ] = high;
        }
    }

    return out;
}


// transforms a 3D point using a matrix, returning a 3D point
Vector3f VecUtils::transformPoint( const Matrix4f& mat, const Vector3f& point )
{
    return ( mat * Vector4f( point, 1 ) ).xyz();
}


// transform a 3D directino using a matrix, returning a direction
// This function *does not* take the inverse tranpose for you.
Vector3f VecUtils::transformDirection( const Matrix4f& mat, const Vector3f& dir )
{
    return ( mat * Vector4f( dir, 0 ) ).xyz();
}