/*
    The `ObjectBase` class serves as the foundation for all objects in the ray tracing engine. It defines basic functionality and an interface for derived objects to implement specific behaviors, such as intersection tests and transformations. This class is crucial for representing 3D objects in the ray tracing process. Here's an overview of its components:

    1. **Constructor and Destructor**:
       - The default constructor `ObjectBase()` initializes an object with no specific properties.
       - The destructor `~ObjectBase()` cleans up any resources allocated by the object, though it performs no specific operations in this base class.

    2. **Intersection Test (`TestIntersection`)**:
       - This virtual function is designed to be overridden by derived classes that represent specific types of 3D objects (e.g., spheres, planes, cubes). 
       - The purpose of this method is to determine whether a ray intersects the object, and if so, to compute the intersection point, surface normal, and local color at the intersection.
       - **Parameters**:
         - `castRay`: The ray being tested for intersection with the object.
         - `intPoint`: A vector that will hold the intersection point if an intersection is found.
         - `localNormal`: A vector that will store the surface normal at the intersection point.
         - `localColor`: A vector to store the color at the intersection point.
       - **Return Value**: 
         The method returns `false` by default, indicating that no intersection is detected. Derived classes will override this method with specific intersection logic based on the object's geometry.

    3. **Transformation Matrix (`SetTransformMatrix`)**:
       - This method allows the transformation matrix to be set for the object. The transformation matrix defines how the object is positioned, rotated, and scaled in 3D space.
       - **Parameter**:
         - `transformMatrix`: An object of type `waRT::GTform` representing the forward and backward transformation matrices for the object.
       - This transformation matrix is used to transform rays and intersection points between local object space and world space, enabling proper intersection testing and rendering.

    4. **Floating Point Comparison (`CloseEnough`)**:
       - The `CloseEnough` method is a utility function used to compare two floating-point numbers with a small tolerance (`EPSILON`) to account for the precision errors inherent in floating-point arithmetic.
       - **Parameters**:
         - `f1` and `f2`: The two floating-point numbers to be compared.
       - **Return Value**: 
         Returns `true` if the absolute difference between the two numbers is less than `EPSILON`, and `false` otherwise.
       - This method is essential in intersection testing and other computations where floating-point precision errors could cause incorrect results.

    5. **Summary**:
       - The `ObjectBase` class provides a basic interface for 3D objects in the ray tracing engine. It includes a method for testing ray-object intersections, a way to apply transformations to objects, and a utility for floating-point comparisons.
       - The `TestIntersection` method is designed to be overridden by derived classes that implement specific geometry (e.g., spheres, planes). This allows for flexibility in adding new object types to the ray tracing engine.
       - The `SetTransformMatrix` method ensures that each object can be transformed in 3D space, which is essential for realistic scene construction.
       - The `CloseEnough` function helps mitigate floating-point errors, ensuring stable and accurate intersection tests and other computations.
*/

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

