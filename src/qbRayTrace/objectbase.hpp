#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "./qbLinAlg/qbVector.h"
#include "ray.hpp"
#include "gtfm.hpp"

namespace qbRT
{
    class ObjectBase
    {
    public:
        ObjectBase();
        virtual ~ObjectBase();

        virtual bool TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor);

        void SetTransformMatrix(const qbRT::GTform &transformMatrix);

        // func to test whether two float point numbers are close to be being equal.
        bool CloseEnough(const double f1, const double f2);

    public:
        qbVector<double> m_baseColor{3};

        qbRT::GTform m_transformMatrix;
    };
}
#endif