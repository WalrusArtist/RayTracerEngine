#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "lightbase.hpp"

namespace waRT {
    class PointLight : public LightBase {
        public:
            PointLight();

            virtual ~PointLight() override;

            virtual bool ComputeIllumination( const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                    const std::vector<std::shared_ptr<waRT::ObjectBase>> &objectList,
                                    const std::shared_ptr<waRT::ObjectBase> &currentObject,
                                    qbVector<double> &color, double &intensity);

        public:
            qbVector<double> m_color;
            double m_intensity;
    };
}

#endif