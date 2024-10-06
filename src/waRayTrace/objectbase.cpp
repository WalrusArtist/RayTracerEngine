#include "objectbase.hpp"
#include <math.h>
#define EPSILON 1e-21f;

waRT::ObjectBase::ObjectBase(){}
waRT::ObjectBase::~ObjectBase(){}

bool waRT::ObjectBase::TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) {
    return false;
}

void waRT::ObjectBase::SetTransformMatrix(const waRT::GTform &transformMatrix) {
	m_transformMatrix = transformMatrix;
}

bool waRT::ObjectBase::CloseEnough(const double f1, const double f2) {
    return fabs(f1-f2) < EPSILON;
}

