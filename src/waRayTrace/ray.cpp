/*
    The `Ray` class represents a mathematical ray in 3D space, which is fundamental in ray tracing for determining how light rays interact with objects in the scene. A ray is defined by two points: an origin (`m_point1`) and a direction (`m_point2`), and it forms the basis for casting rays from the camera or light sources to detect intersections with objects.

    1. **Constructor**:
       - The default constructor `Ray()` initializes the ray's origin (`m_point1`) to the point `(0.0, 0.0, 0.0)` and the direction endpoint (`m_point2`) to `(0.0, 0.0, 1.0)`. This means the default ray points in the positive z-direction.
       - The direction of the ray (`m_lab`) is computed as the difference between `m_point2` and `m_point1`. This direction vector is often referred to as the ray's "line of action."

       - The parameterized constructor `Ray(const qbVector<double> &point1, const qbVector<double> &point2)` allows the user to define a custom ray by providing two vectors:
         - `point1`: The starting point (origin) of the ray.
         - `point2`: The direction endpoint of the ray.
         After setting these points, the constructor calculates the direction vector (`m_lab`) as the difference between the provided points, representing the ray's trajectory in space.

    2. **Line of Action (`m_lab`)**:
       - The `m_lab` vector is an essential property of the ray because it defines the ray's direction and magnitude. This vector is used to test intersections with objects in the scene by determining if and where the ray intersects with object geometries such as spheres or planes.
       - The `m_lab` vector is calculated as `m_point2 - m_point1`, which gives the direction in which the ray travels.

    3. **Getters**:
       - The class provides two getter methods for accessing the ray's origin and direction endpoint:
         - `GetPoint1() const`: Returns the origin point of the ray (`m_point1`).
         - `GetPoint2() const`: Returns the direction endpoint of the ray (`m_point2`).
       These methods are useful when other components of the ray tracing engine, such as intersection tests, need to retrieve the ray's defining points.

    4. **Usage in Ray Tracing**:
       - Rays are cast from the camera through each pixel on the image plane and are traced through the scene to detect intersections with objects. When an intersection is found, the ray's direction and origin are used to calculate the position of the hit point, surface normals, and shading effects based on the light sources.
       - In addition to casting rays from the camera, rays can also be traced from light sources to determine shadows or reflective rays for handling specular effects.

    5. **Summary**:
       - The `Ray` class is a fundamental building block of the ray tracing engine. It represents a directed line in 3D space with an origin and a direction endpoint.
       - The class provides essential operations such as constructing a ray from two points and retrieving the ray's defining points via getter methods.
       - The direction vector (`m_lab`) computed in both constructors is crucial for determining how the ray interacts with objects, which is key to implementing the core ray tracing algorithm.
*/

#include "ray.hpp"

waRT::Ray::Ray() {
    m_point1 = qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}};
    m_point2 = qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}};
    m_lab = m_point2 - m_point1;
}

waRT::Ray::Ray(const qbVector<double> &point1, const qbVector<double> &point2) {
    m_point1 = point1;
    m_point2 = point2;
    m_lab = m_point2 - m_point1;
}

// GETTERS
qbVector<double> waRT::Ray::GetPoint1() const { return m_point1;}
qbVector<double> waRT::Ray::GetPoint2() const { return m_point2;}