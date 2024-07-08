#ifndef GTFM_H
#define GTFM_H

#include "ray.hpp"
#include "./qbLinAlg/qbVector.h"
#include "./qbLinAlg/qbMatrix.h"

namespace qbRT
{
    constexpr bool FWDTFORM = true;
    constexpr bool BCKTFORM = false;

    class GTform
    {
        public:
            GTform();
            ~GTform();

            // matrices pair
            GTform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck);

            // funk to set translation, rotation and scaling
            void SetTransform(
                const qbVector<double> &translation,
                const qbVector<double> &rotation,
                const qbVector<double> &scale
            );

            // funks to return the transform matrices
            qbMatrix2<double> GetForward();
            qbMatrix2<double> GetBackward();

            // funk to apply transform
            qbRT::Ray Apply(const qbRT::Ray &inputRay, bool dirFlag);
            qbVector<double> Apply(const qbVector<double> &inputVector, bool firFlag);
            friend GTform operator* (const qbRT::GTform &lhs, const qbRT::GTform &rhs);
            GTform operator=(const GTform &rhs);
            void PrintMatrix(bool dirFlag);
            static void PrintVector(const qbVector<double> vector);

        private:
            void PrintVector(const qbMatrix2<double> &matrix);

        private:
            qbMatrix2<double> m_fwdfm {4,4};
            qbMatrix2<double> m_bckfm {4,4};
    };
}