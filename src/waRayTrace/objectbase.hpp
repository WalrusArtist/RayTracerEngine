#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "./linAlgModule/qbVector.h"
#include "ray.hpp"
#include "gtfm.hpp"

namespace waRT {
    class ObjectBase {
    public:
        ObjectBase();
        virtual ~ObjectBase();
        virtual bool TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor);
        void SetTransformMatrix(const waRT::GTform &transformMatrix);
        bool CloseEnough(const double f1, const double f2);
    public:
        qbVector<double> m_baseColor{3};
        waRT::GTform m_transformMatrix;
    };
}
#endif