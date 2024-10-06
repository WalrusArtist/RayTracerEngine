#include "camera.hpp"
#include "ray.hpp"
#include <math.h>

waRT::Camera::Camera() {
    // const
    m_cameraPosition = qbVector<double>{std::vector<double>{0.0, -10.0, 0, 0}};
    m_cameraLookAt   = qbVector<double>{std::vector<double>{0.0, -10.0, 0, 0}};
    m_cameraUp       = qbVector<double>{std::vector<double>{0.0, -10.0, 0, 0}};
    m_cameraLength      = 1.0;
    m_cameraHorzSize    = 1.0;
    m_cameraAspectRatio = 1.0;
}

// SETTERS
void waRT::Camera::SetPosition(const qbVector<double> &newPosition) { m_cameraPosition = newPosition;}
void waRT::Camera::SetLookAt(const qbVector<double> &newLookAt)     { m_cameraLookAt = newLookAt;}
void waRT::Camera::SetUp(const qbVector<double> &upVector)          { m_cameraUp = upVector;}
void waRT::Camera::SetLength(double newLength)                      { m_cameraLength = newLength;}
void waRT::Camera::SetHorzSize(double newHorzSize)                  { m_cameraHorzSize = newHorzSize;}
void waRT::Camera::SetAspect(double newAspect)                      { m_cameraAspectRatio = newAspect;}

// GETTERS
qbVector<double> waRT::Camera::GetPosition()     { return m_cameraPosition;}
qbVector<double> waRT::Camera::GetLookAt()       { return m_cameraLookAt;}
qbVector<double> waRT::Camera::GetUp()           { return m_cameraUp;}
double waRT::Camera::GetLength()                 { return m_cameraLength;}
double waRT::Camera::GetHorzSize()               { return m_cameraHorzSize;}
double waRT::Camera::GetAspect()                 { return m_cameraAspectRatio;}
qbVector<double> waRT::Camera::GetU()            { return m_projectionScreenU;}
qbVector<double> waRT::Camera::GetV()            { return m_projectionScreenV;}
qbVector<double> waRT::Camera::GetScreenCenter() { return m_projectionScreenCentre;}

// compute camera geom
void waRT::Camera::UpdateCameraGeometry() {
    m_alignmentVector        = m_cameraLookAt - m_cameraPosition;
    m_alignmentVector.Normalize();
    m_projectionScreenU      = qbVector<double>::cross(m_alignmentVector, m_cameraUp);
    m_projectionScreenU.Normalize();
    m_projectionScreenV      = qbVector<double>::cross(m_projectionScreenU, m_alignmentVector);
    m_projectionScreenCentre = m_cameraPosition + (m_cameraLength * m_alignmentVector);
    m_projectionScreenU      = m_projectionScreenU * m_cameraHorzSize;
    m_projectionScreenV      = m_projectionScreenV * (m_cameraHorzSize / m_cameraAspectRatio);
}

bool waRT::Camera::GenerateRay(float proScreenX, float proScreenY, waRT::Ray &cameraRay) {
    qbVector<double> screenWorldPart1      = m_projectionScreenCentre + (m_projectionScreenU * proScreenX);
    qbVector<double> screenWorldCoordinate = screenWorldPart1 + (m_projectionScreenV * proScreenY);
    cameraRay.m_point1 = m_cameraPosition;
    cameraRay.m_point2 = screenWorldCoordinate;
    cameraRay.m_lab    = screenWorldCoordinate - m_cameraPosition;
    return true;
}