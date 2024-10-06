#ifndef GTFM_H
#define GTFM_H

#include "./qbLinAlg/qbVector.h"
#include "./qbLinAlg/qbMatrix.h"
#include "ray.hpp"

namespace waRT {
	constexpr bool FWDTFORM = true;
	constexpr bool BCKTFORM = false;

	class GTform {
		public:
			GTform();
			~GTform();
			GTform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck);
			void SetTransform(const qbVector<double> &translation,
						      const qbVector<double> &rotation,
						      const qbVector<double> &scale);
			qbMatrix2<double> GetForward();
			qbMatrix2<double> GetBackward();			
			waRT::Ray Apply(const waRT::Ray &inputRay, bool dirFlag);
			qbVector<double> Apply(const qbVector<double> &inputVector, bool dirFlag);
			friend GTform operator* (const waRT::GTform &lhs, const waRT::GTform &rhs);
			GTform operator= (const GTform &rhs);
			void PrintMatrix(bool dirFlag);
			static void PrintVector(const qbVector<double> &vector);
		private:
			void Print(const qbMatrix2<double> &matrix);
		private:
			qbMatrix2<double> m_fwdtfm {4, 4};
			qbMatrix2<double> m_bcktfm {4, 4};
	};
}

#endif