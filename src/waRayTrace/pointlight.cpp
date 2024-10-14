/*
    The `PointLight` class represents a point light source in the ray tracing engine. A point light emits light uniformly in all directions from a specific location in 3D space. This class calculates the illumination at a point of intersection based on the light's position, intensity, and the normal at the intersection point. Below is an explanation of the main components:

    1. **Constructor and Destructor**:
       - The default constructor `PointLight()` initializes the light's color to white (`{1.0, 1.0, 1.0}`) and sets the light's intensity to 1.0. The color and intensity determine the appearance and strength of the light when it interacts with objects in the scene.
       - The destructor `~PointLight()` does not perform any special cleanup actions.

    2. **Illumination Computation (`ComputeIllumination`)**:
       - This method calculates the amount of light received at a given point of intersection on an object and determines how much of the light contributes to the shading at that point.

       - **Parameters**:
         - `intPoint`: The intersection point on the object where the light is being evaluated.
         - `localNormal`: The surface normal at the intersection point, used to compute the angle of incidence of the light.
         - `objectList`: A list of all objects in the scene, used for checking potential shadows or occlusion.
         - `currentObject`: The object that is currently being evaluated for shading (typically the object that the ray intersects).
         - `color`: A vector that stores the computed light color at the intersection point.
         - `intensity`: A double that stores the computed light intensity at the intersection point.

       - **Light Direction**:
         The direction of the light (`lightDir`) is calculated as the normalized vector from the intersection point (`intPoint`) to the light's location (`m_location`).

       - **Angle of Incidence**:
         The angle between the surface normal at the intersection point and the light direction is computed using the dot product between `localNormal` and `lightDir`. This angle is used to determine how much light the surface receives. If the angle exceeds 90 degrees (i.e., the light is behind the surface), the surface is in shadow and does not receive any light.

       - **Shading Logic**:
         - If the computed angle is greater than 90 degrees (approximately `1.5708` radians, stored as `PI_BY_TWO_APROX`), the surface is facing away from the light, resulting in zero illumination (intensity = 0). In this case, the object appears dark.
         - Otherwise, the light's intensity is computed as a fraction based on the angle. The intensity decreases as the angle increases, simulating the falloff of light as it strikes the surface at a more oblique angle. The light's color is also applied to the surface, contributing to its appearance.

       - **Return Value**:
         - The method returns `true` if the surface is illuminated (i.e., the light strikes the surface at an angle less than 90 degrees). In this case, both `color` and `intensity` are updated with the computed values.
         - If the surface is in shadow or facing away from the light, the method returns `false`, indicating no illumination, and the `color` remains as the light's base color while `intensity` is set to zero.

    3. **Summary**:
       - The `PointLight` class models a point light source in the scene and computes how it interacts with objects based on their surface normals and the angle of incidence of the light.
       - The method `ComputeIllumination` determines whether a surface point is lit or in shadow, and calculates the color and intensity of the light that contributes to shading the object.
       - This class is essential for implementing basic lighting and shading models in the ray tracing engine, such as Lambertian reflection, by computing how much light reaches a surface and at what intensity.
*/

#include "pointlight.hpp"

waRT::PointLight::PointLight() {
    m_color = qbVector<double>{std::vector<double> {1.0, 1.0, 1.0}};
    m_intensity = 1.0;
}

waRT::PointLight::~PointLight() {}

bool waRT::PointLight::ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                           const std::vector<std::shared_ptr<waRT::ObjectBase>> &objectList,
                                           const std::shared_ptr<waRT::ObjectBase> &currentObject,
                                           qbVector<double> &color, double &intensity) {
    qbVector<double> lightDir   = (m_location - intPoint).Normalized();
    qbVector<double> startPoint = intPoint;

    waRT::Ray lightRay(startPoint, startPoint + lightDir);
    qbVector<double> poi       {3};
    qbVector<double> poiNormal {3};
    qbVector<double> poiColor  {3};
    bool validInt = false;
    for (auto sceneObject : objectList) {
        if (sceneObject != currentObject) {
            validInt = sceneObject -> TestIntersection(lightRay, poi, poiNormal, poiColor);
        }
        if (validInt)
            break;
    }
    if (!validInt) {
        double angle = acos(qbVector<double>::dot(localNormal, lightDir));
        if (angle > 1.5708) {
            color     = m_color;
            intensity = 0.0;
            return false;
        } else {
            color     = m_color;
            intensity = m_intensity * (1.0 - (angle / 1.5708));
            return true;
        }
    } else {
      color     = m_color;
      intensity = 0.0;
      return false;
    }
}