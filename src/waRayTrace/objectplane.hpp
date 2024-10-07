#ifndef OBJECTPLANE_H
#define OBJECTPLANE_H

#include "objectbase.hpp"
#include "gtfm.hpp"

namespace waRT {
    class ObjectPlane : public ObjectBase {
        public:
            ObjectPlane();
            virtual ~ObjectPlane() override;
            virtual bool TestIntersection(const waRT::Ray &castRay, qbVector<double> &intPoint,
                                          qbVector<double> &localNormal, qbVector<double> &localColor) override;
        private:
    };
}
#endif