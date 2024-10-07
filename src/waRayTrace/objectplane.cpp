/* 
    This code defines a class `ObjectPlane` for representing a 2D plane object in a 3D space, used within a ray tracing engine. 
    The `TestIntersection` function checks whether a given ray intersects the plane and calculates the intersection point, 
    local normal, and local color if an intersection occurs. Here's a breakdown of the logic:

    1. **Ray Transformation**: 
       The incoming ray (`castRay`) is first transformed from global space to the object's local space using the 
       `m_transformMatrix.Apply` function. The transformed ray is stored in `bckRay`. The direction vector of the ray is normalized 
       for consistent calculations.
       
    2. **Intersection Test**: 
       The function checks if the ray is parallel to the plane by examining the Z component of the direction vector `k`. 
       If the Z component is zero (or close to zero), the ray is parallel and will not intersect the plane, and the function 
       returns false.

    3. **Plane Intersection Calculation**: 
       If the ray is not parallel, the function calculates the intersection point using parameter `t`, which is the distance along the ray to the intersection. 
       The `u` and `v` values represent the coordinates of this intersection point in the plane's local space. 
       
       - If `u` and `v` are within the bounds of the plane (assumed to be centered at the origin and spanning from -1 to 1 in both X and Y directions), 
         an intersection is confirmed. Otherwise, the function returns false.

    4. **Intersection Point and Normal Calculation**: 
       The intersection point is calculated in local space and transformed back into global space using the transformation matrix. 
       The function then calculates the local normal of the plane, which is constant and points in the negative Z direction. 
       The normal vector is also transformed back into global space.

    5. **Output**: 
       If an intersection is found, the function sets the following output parameters:
       - `intPoint`: The intersection point in global coordinates.
       - `localNormal`: The surface normal at the intersection point, transformed into global space.
       - `localColor`: The base color of the plane, used for shading.
       
       If an intersection is found, the function returns `true`, otherwise, it returns `false`.

    Summary:
    - This class method is essential for detecting ray-plane intersections in a ray tracing context. 
    - The function handles ray transformation, intersection testing, and calculates geometric details needed for shading.
*/

#include "objectplane.hpp"
#include  <cmath>

waRT::ObjectPlane::ObjectPlane()  {}
waRT::ObjectPlane::~ObjectPlane() {}

bool waRT::ObjectPlane::TestIntersection(const waRT::Ray &castRay, qbVector<double> &intPoint,
                                         qbVector<double> &localNormal, qbVector<double> &localColor) {
    waRT::Ray bckRay = m_transformMatrix.Apply(castRay, waRT::BCKTFORM);
    qbVector<double> k = bckRay.m_lab;
    k.Normalize();

    if (!CloseEnough(k.GetElement(2), 0.0)) { // GetElement(2) = z
        double t = bckRay.m_point1.GetElement(2) / -k.GetElement(2);

        if (t > 0.0) {
            double u = bckRay.m_point1.GetElement(0) + (k.GetElement(0) * t);
            double v = bckRay.m_point1.GetElement(1) + (k.GetElement(1) * t);

            if ((abs(u) < 1.0) && (abs(v) < 1.0)) {
                qbVector<double> poi = bckRay.m_point1 + t * k;
                intPoint = m_transformMatrix.Apply(poi, waRT::FWDTFORM);

                qbVector<double> localOrigin {std::vector<double>  {0.0, 0.0, 0.0}};
                qbVector<double> normalVector {std::vector<double> {0.0, 0.0, -1.0}};
                qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, waRT::FWDTFORM);
                localNormal = m_transformMatrix.Apply(normalVector, waRT::FWDTFORM) - globalOrigin;

                localColor = m_baseColor;
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    return false;
}