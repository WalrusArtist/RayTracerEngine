/*
    The `ObjSphere` class is a specific implementation of a 3D sphere object in the ray tracing engine. It inherits from the `ObjectBase` class and provides a concrete implementation of the `TestIntersection` method, which determines whether a ray intersects the sphere. The sphere is defined with its center at the origin in local space and a radius of 1 unit. This class handles ray-sphere intersection tests in both local and world space using transformation matrices. Below is an explanation of the key components:

    1. **Constructor and Destructor**:
       - The default constructor `ObjSphere()` initializes the sphere object with no specific properties.
       - The destructor `~ObjSphere()` ensures proper cleanup of the sphere object, though it doesn't perform any specific operations.

    2. **Intersection Test (`TestIntersection`)**:
       - This method provides the logic for determining if a given ray intersects the sphere. If an intersection occurs, it calculates the intersection point, the surface normal at that point, and the local color.
       
       - **Parameters**:
         - `castRay`: The ray being tested for intersection with the sphere.
         - `intPoint`: A vector that will store the intersection point in world space if an intersection occurs.
         - `localNormal`: A vector that will store the surface normal at the intersection point in world space.
         - `localColor`: A vector to store the color of the sphere at the intersection point.

       - **Local Space Conversion**:
         The input ray (`castRay`) is transformed into the sphere's local object space using the object's transformation matrix (`m_transformMatrix.Apply(castRay, waRT::BCKTFORM)`). This allows for testing the intersection with a unit sphere centered at the origin in local space.

       - **Ray-Sphere Intersection**:
         In local space, the sphere has a radius of 1. The method solves the quadratic equation for ray-sphere intersection:
         - **b**: Represents the projection of the ray's origin onto the normalized direction vector (`vhat`).
         - **c**: Represents the squared distance from the ray's origin to the sphere, minus the radius squared (which is 1).
         - **intTest**: The discriminant of the quadratic equation, used to determine whether the ray intersects the sphere. If `intTest` is greater than zero, the ray intersects the sphere at two points (t1 and t2). Otherwise, no intersection occurs.

       - **Intersection Point Calculation**:
         If two valid intersection points are found (both `t1` and `t2` are positive), the closer of the two (`t1` or `t2`) is selected as the valid intersection point.
         - The intersection point is calculated in local space (`poi = bckRay.m_point1 + (vhat * t1)`), then transformed back into world space using the forward transformation (`m_transformMatrix.Apply(poi, waRT::FWDTFORM)`).

       - **Surface Normal**:
         The surface normal is computed by subtracting the sphere's transformed origin from the intersection point, and then normalizing the result. This gives the direction of the surface normal in world space.

       - **Color**:
         The sphere's base color (`m_baseColor`) is assigned to `localColor`.

       - **Return Value**:
         The method returns `true` if a valid intersection is found and all the relevant data (intersection point, normal, color) has been computed. Otherwise, it returns `false`.

    3. **Summary**:
       - The `ObjSphere` class implements the intersection test for a unit sphere in 3D space, with the ability to handle arbitrary transformations (position, scale, rotation) via transformation matrices.
       - The class handles both local space (unit sphere at the origin) and world space (transformed object in the scene) intersection testing.
       - It calculates and provides the intersection point, surface normal, and color at the intersection point, which are crucial for shading and rendering the object in the ray tracing engine.
*/

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