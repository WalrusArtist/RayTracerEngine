#include "objectsphere.hpp"
#include <cmath>

waRT::ObjSphere::ObjSphere(){}
waRT::ObjSphere::~ObjSphere(){}

bool waRT::ObjSphere::TestIntersection(const waRT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) {
	waRT::Ray bckRay = m_transformMatrix.Apply(castRay, waRT::BCKTFORM);
	qbVector<double> vhat = bckRay.m_lab;
	vhat.Normalize();
	double b = 2.0 * qbVector<double>::dot(bckRay.m_point1, vhat);
	double c = qbVector<double>::dot(bckRay.m_point1, bckRay.m_point1) - 1.0;
	double intTest = (b*b) - 4.0 * c;
	qbVector<double> poi;
	
	if (intTest > 0.0) {
		double numSQRT = sqrtf(intTest);
		double t1 = (-b + numSQRT) / 2.0;
		double t2 = (-b - numSQRT) / 2.0;

		if ((t1 < 0.0) || (t2 < 0.0)) {
			return false;
		} else {
			if (t1 < t2) {
				poi = bckRay.m_point1 + (vhat * t1);
			} else {
				poi = bckRay.m_point1 + (vhat * t2);
			}
			intPoint = m_transformMatrix.Apply(poi, waRT::FWDTFORM);
			qbVector<double> objOrigin = qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}};
			qbVector<double> newObjOrigin = m_transformMatrix.Apply(objOrigin, waRT::FWDTFORM);
			localNormal = intPoint - newObjOrigin;
			localNormal.Normalize();
			localColor = m_baseColor;
		}
		return true;
	} else {
		return false;
	}
}