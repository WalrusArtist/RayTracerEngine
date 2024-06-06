#include "objectsphere.hpp"
#include <cmath>

qbRT::ObjSphere::ObjSphere()
{

}

qbRT::ObjSphere::~ObjSphere()
{

}

bool qbRT::ObjSphere::TestIntersection(const qbRT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor)
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
        double numSQRT = sqrtf(intTest);
        double t1 = (-b + numSQRT) / 2.0;
        double t2 = (-b - numSQRT) / 2.0;

        // basically ignore if t1 or t2 is negative, because it means it's behind the camera
        if ((t1 < 0.0) || (t2 < 0.0))
        {
            return false;
        }
        else
        {
            if (t1 < t2)
            {
                intPoint = castRay.m_point1 + (vhat * t1);
            }
            else
            {
                intPoint = castRay.m_point1 + (vhat * t2);
            }
        }
    return true;
    }
    else
    {
        return false;
    }
}