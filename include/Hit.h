#ifndef HIT_H
#define HIT_H

#include <vecmath.h>
#include "Ray.h"
#include <float.h>


class Material;


class Hit
{
public:
    // constructors
    Hit();
    Hit(float _t, Material* m, const Vector3f& n);
    Hit(const Hit& h);

    // destructor
    ~Hit();

    float getT() const;
    Material* getMaterial() const;
    const Vector3f& getNormal() const;
    void set(float _t, Material* m, const Vector3f& n, bool _hasTex = false);
	void setTexCoord(const Vector2f & coord);

    /**
     * Why should hit have hasTex?
     * shouldn't that be determined by wether the material has valid texture?
     */
	bool hasTex;
	Vector2f texCoord;
private:
	float t;
    Material* material;
    Vector3f normal;
};


inline ostream& operator << ( ostream &os, const Hit& h);


#endif // HIT_H