#include "lightbase.hpp"

waRT::LightBase::LightBase(){}
waRT::LightBase::~LightBase(){}

bool waRT::LightBase::ComputeIllumination( const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                    const std::vector<std::shared_ptr<waRT::ObjectBase>> &objectList,
                                    const std::shared_ptr<waRT::ObjectBase> &currentObject,
                                    qbVector<double> &color, double &intensity) 
                                    {return false;}