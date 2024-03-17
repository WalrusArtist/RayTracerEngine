#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "./qbLinAlg/qbVector.h"
#include "ray.hpp"

namespace qbRT
{
    class ObjectBase
    {
    public:
        ObjectBase();
        virtual ~ObjectBase();

        virtual bool TestIntersections(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor);

        // func to test whether two float point numbers are close to be being equal.
        bool CloseEnough(const double f1, const double f2);

        public:
        qbVector<double> m_basecolor(3);
    };
}
#endif