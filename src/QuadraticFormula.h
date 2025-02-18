#ifndef QUADRATIC_FORMULA_H
#define QUADRATIC_FORMULA_H

namespace Quadratic
{
    // Quadratic formula: discriminant
    float discriminant(float a, float b, float c);

    // Quadratic formula: one root
    float root(float a, float  b);

    // Quadratic formula: right root
    float rootRight(float a, float b, float discriminantSqrt);

    // Quadratic formula: left root
    float rootLeft(float a, float b, float discriminantSqrt);
}

#endif