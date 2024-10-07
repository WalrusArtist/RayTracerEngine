/*
    This class `Camera` defines the behavior and properties of a camera in a ray tracing engine. 
    It is responsible for determining the viewpoint, orientation, and the projection of rays from the camera into the 3D scene. 
    Here is a breakdown of the code:

    1. **Camera Construction (Constructor)**:
       The constructor initializes the camera's key attributes, including its position (`m_cameraPosition`), the direction it's facing (`m_cameraLookAt`), the up direction (`m_cameraUp`), the distance between the camera and its projection plane (`m_cameraLength`), the size of the horizontal plane (`m_cameraHorzSize`), and the aspect ratio (`m_cameraAspectRatio`). These values are essential for defining the camera's orientation and projection geometry.

    2. **Setters and Getters**:
       The class provides setter methods to modify camera attributes:
       - `SetPosition()`, `SetLookAt()`, `SetUp()` update the camera's position, where it is looking, and the up vector, respectively.
       - `SetLength()`, `SetHorzSize()`, `SetAspect()` set the length (distance to the projection screen), the horizontal screen size, and the aspect ratio, respectively.
       
       It also includes getter methods to retrieve these values. The getters for `GetU()`, `GetV()`, and `GetScreenCenter()` provide access to the camera's internal projection screen vectors and its center, which are updated after computing camera geometry.

    3. **Camera Geometry Calculation (`UpdateCameraGeometry`)**:
       This method recalculates the camera's orientation and projection plane geometry based on the updated position, look-at direction, and up vector. It calculates:
       - `m_alignmentVector`: The normalized vector that points from the camera's position to where it is looking (the alignment direction).
       - `m_projectionScreenU`: The horizontal vector of the projection screen, calculated using a cross-product between the alignment vector and the camera's up vector.
       - `m_projectionScreenV`: The vertical vector of the projection screen, calculated by crossing the projection screen's horizontal vector (`m_projectionScreenU`) with the alignment vector.
       - `m_projectionScreenCentre`: The center point of the projection screen in 3D space, calculated as the camera position plus the alignment vector scaled by the camera length.
       - The method also scales `m_projectionScreenU` and `m_projectionScreenV` by the horizontal size and adjusts for the aspect ratio to ensure proper projection scaling.

    4. **Ray Generation (`GenerateRay`)**:
       This method generates a ray that originates from the camera and passes through a given point on the projection screen. It takes two parameters `proScreenX` and `proScreenY`, which represent normalized coordinates on the projection screen. The steps are as follows:
       - `screenWorldPart1`: Computes the world coordinate along the horizontal direction of the projection screen based on `proScreenX`.
       - `screenWorldCoordinate`: Adds the vertical displacement (using `proScreenY`) to find the final world coordinates of the point on the projection screen.
       - The ray (`cameraRay`) is then created:
           - `m_point1`: The ray's origin is set to the camera's position.
           - `m_point2`: The ray's destination is set to the computed screen world coordinate.
           - `m_lab`: The direction of the ray is the difference between the screen world coordinate and the camera's position.

       This function allows the camera to cast rays into the scene based on screen coordinates, which is essential for ray tracing as it projects rays from the camera into the 3D world.

    Summary:
    - The `Camera` class is critical in defining how rays are generated from a camera's viewpoint into a 3D scene.
    - It handles orientation, projection geometry, and ray generation, all of which are fundamental for correctly tracing rays in a ray tracing engine.
*/

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