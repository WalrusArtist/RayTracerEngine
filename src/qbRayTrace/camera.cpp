#include "camera.hpp"
#include "ray.hpp"
#include <math.h>

qbRT::Camera::Camera()
{
    // const
    m_cameraPosition = qbVector<double>{std::vector<double>{0.0, -10.0, 0, 0}};
    m_cameraLookAt = qbVector<double>{std::vector<double>{0.0, -10.0, 0, 0}};
    m_cameraUp = qbVector<double>{std::vector<double>{0.0, -10.0, 0, 0}};
    m_cameraLength = 1.0;
    m_cameraHorzSize = 1.0;
    m_cameraAspectRatio = 1.0;
}

// SETTERS

void qbRT::Camera::SetPosition(const qbVector<double> &newPosition)
{
    m_cameraPosition = newPosition;
}

void qbRT::Camera::SetLookAt(const qbVector<double> &newLookAt)
{
    m_cameraLookAt = newLookAt;
}

void qbRT::Camera::SetUp(const qbVector<double> &upVector)
{
    m_cameraUp = upVector;
}

void qbRT::Camera::SetLength(double newLength)
{
    m_cameraLength = newLength;
}

void qbRT::Camera::SetHorzSize(double newHorzSize)
{
    m_cameraHorzSize = newHorzSize;
}

void qbRT::Camera::SetAspect(double newAspect)
{
    m_cameraAspectRatio = newAspect;
}

// GETTERS

qbVector<double> qbRT::Camera::GetPosition()
{
    return m_cameraPosition;
}

qbVector<double> qbRT::Camera::GetLookAt()
{
    return m_cameraLookAt;
}

qbVector<double> qbRT::Camera::GetUp()
{
    return m_cameraUp;
}

double qbRT::Camera::GetLength()
{
    return m_cameraLength;
}

double qbRT::Camera::GetHorzSize()
{
    return m_cameraHorzSize;
}

double qbRT::Camera::GetAspect()
{
    return m_cameraAspectRatio;
}

qbVector<double> qbRT::Camera::GetU()
{
    return m_projectionScreenU;
}

qbVector<double> qbRT::Camera::GetV()
{
    return m_projectionScreenV;
}

qbVector<double> qbRT::Camera::GetScreenCenter()
{
    return m_projectionScreenCentre;
}

// compute camera geom
void qbRT::Camera::UpdateCameraGeometry()
{
    // compute vector from camera to lookat pos
    m_alignmentVector = m_cameraLookAt - m_cameraPosition;
    m_alignmentVector.Normalize();

    // compute U and V vectors
    m_projectionScreenU = qbVector<double>::cross(m_alignmentVector, m_cameraUp);
    m_projectionScreenU.Normalize();
    m_projectionScreenV = qbVector<double>::cross(m_projectionScreenU, m_alignmentVector);

    // compute the pos of the cente point
    m_projectionScreenCentre = m_cameraPosition + (m_cameraLength * m_alignmentVector);

    // modify the U and V to match aspect ratio
    m_projectionScreenU = m_projectionScreenU * m_cameraHorzSize;
    m_projectionScreenV = m_projectionScreenV * (m_cameraHorzSize / m_cameraAspectRatio);
}

bool qbRT::Camera::GenerateRay(float proScreenX, float proScreenY, qbRT::Ray &cameraRay)
{
    // compute the loc of the screen in world cordinates
    qbVector<double> screenWorldPart1 = m_projectionScreenCentre + (m_projectionScreenU * proScreenX);
    qbVector<double> screenWorldCoordinate = screenWorldPart1 + (m_projectionScreenV * proScreenY);

    // use this point along the camera position to compute the ray
    cameraRay.m_point1 = m_cameraPosition;
    cameraRay.m_point2 = screenWorldCoordinate;
    cameraRay.m_lab    = screenWorldCoordinate - m_cameraPosition;
    return true;
}