#include "QuadraticFormula.h"

float Quadratic::discriminant(float a, float b, float c)
{
    return (b * b) - (4.0f * a * c);
}

// Quadratic formula: one root
float Quadratic::root(float a, float  b)
{
    return -b / (2.0f * a);
}

// Quadratic formula: right root
float Quadratic::rootRight(float a, float b, float discriminantSqrt)
{
    return (-b + discriminantSqrt) / (2.0f * a);
}

// Quadratic formula: left root
float Quadratic::rootLeft(float a, float b, float discriminantSqrt)
{
    return (-b - discriminantSqrt) / (2.0f * a);
}