#include "objectsphere.hpp"
#include <cmath>

qbRT::ObjSphere::ObjSphere()
{

}

qbRT::ObjSphere::~ObjSphere()
{

}

bool qbRT::ObjSphere::TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor)
{
    // compute a b and c
    qbVector<double> vhat = castRay.m_lab;
    vhat.Normalize();
    // a=1.0

    double b = 2.0 * qbVector<double>::dot(castRay.m_point1, vhat);
    double c = qbVector<double>::dot(castRay.m_point1, castRay.m_point1) - 1.0;
    double intTest = (b*b) - 4.0 * c;

    if (intTest > 0.0)
    {
        return true;
    }
    else
    {
        return false;
    }
}