#ifndef LIGHTBASE_H
#define LIGHTBASE_H

#include <memory>
#include "../linAlgModule/qbVector.h"
#include "../ray.hpp"
#include "../primitives/objectbase.hpp"
namespace waRT {
    class LightBase {
        public:
            LightBase();
            virtual ~LightBase();
            virtual bool ComputeIllumination( const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                              const std::vector<std::shared_ptr<waRT::ObjectBase>> &objectList,
                                              const std::shared_ptr<waRT::ObjectBase> &currentObject,
                                              qbVector<double> &color, double &intensity);
        public:
            qbVector<double> m_color    {3};
            qbVector<double> m_location {3};
            double           m_instensity;
    };
}
#endif