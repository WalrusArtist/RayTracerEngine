#ifndef OBJECTSPHERE_H
#define OBJECTSPHERE_H

#include "objectbase.hpp"

namespace qbRT
{
    class ObjSphere : public ObjectBase
    {
        public:
            ObjSphere();

            virtual ~ObjSphere() override;

            virtual bool TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor);

        private:
        

    };
}
#endif