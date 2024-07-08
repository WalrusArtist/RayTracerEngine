#include "pointlight.hpp"

qbRT::PointLight::PointLight()
{
    m_color = qbVector<double>{std::vector<double> {1.0, 1.0, 1.0}};
    m_intensity = 1.0;
}

qbRT::PointLight::~PointLight()
{

}


bool qbRT::PointLight::ComputeIllumination( const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                            const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
                                            const std::shared_ptr<qbRT::ObjectBase> &currentObject,
                                            qbVector<double> &color, double &intensity)
{
    qbVector<double> lightDir = (m_location - intPoint).Normalized();

    qbVector<double> startPoint = intPoint;

    //compute angle betwen localnorm and lightray. Assume localnorm is a unit vector
    double angle = acos(qbVector<double>::dot(localNormal, lightDir));

    // if away from light, daaaark
    double PI_BY_TWO_APROX = 1.5708;
    if (angle > PI_BY_TWO_APROX)
    {
        // daaark
        color = m_color;
        intensity = 0.0;
        return false;
    }
    else
    {
        color = m_color;
        intensity = m_intensity * (1.0 - (angle / PI_BY_TWO_APROX));
        return true;
    }

}