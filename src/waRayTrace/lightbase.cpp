/*
    The `LightBase` class serves as a base class for handling lighting computations in a ray tracing engine. It defines the basic interface for light objects, providing a foundation for different types of lights (such as point lights, directional lights, etc.) to be implemented and extended. Here’s an overview of the class:

    1. **Constructor and Destructor**:
       - The default constructor `LightBase()` initializes a generic light object without any specific properties. This can be extended by derived classes to add more specialized attributes, such as position, direction, or color.
       - The destructor `~LightBase()` ensures proper cleanup of resources, though it doesn't perform any specific operations in this base class.

    2. **Illumination Calculation (`ComputeIllumination`)**:
       This is a virtual function designed to be overridden by derived classes that implement specific light types. The purpose of this method is to compute the lighting effects at a given intersection point on an object in the scene.
       
       - **Parameters**:
         - `intPoint`: The point of intersection on the object where the light interacts.
         - `localNormal`: The surface normal at the intersection point, used to compute how light interacts with the surface.
         - `objectList`: A list of all objects in the scene, passed here to handle potential occlusion (shadow casting) and other object interactions with light.
         - `currentObject`: The object currently being illuminated (the one that the intersection point belongs to).
         - `color`: A vector that will be populated with the light's contribution to the color at the intersection point.
         - `intensity`: A double that will store the intensity of the light at the intersection point.
       
       - **Return Value**: 
         The function returns a boolean value. In the base class, it always returns `false`, indicating that no actual illumination calculation is performed here. Derived classes are expected to override this method and provide specific implementations for various lighting models.

    3. **Summary**:
       - The `LightBase` class provides a foundation for creating different types of light sources in a ray tracing engine. It defines an interface for calculating the illumination (color and intensity) at a given point in the scene.
       - While the `ComputeIllumination` function is defined in this base class, it serves as a placeholder and returns `false` by default. Derived light classes will override this method to implement actual lighting behavior based on the light type.
       - This structure allows for flexibility in the ray tracing engine, enabling the easy addition of different lighting models and behavior by extending this base class.
*/

#include "lightbase.hpp"

waRT::LightBase::LightBase(){}
waRT::LightBase::~LightBase(){}

bool waRT::LightBase::ComputeIllumination( const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                    const std::vector<std::shared_ptr<waRT::ObjectBase>> &objectList,
                                    const std::shared_ptr<waRT::ObjectBase> &currentObject,
                                    qbVector<double> &color, double &intensity) 
                                    {return false;}