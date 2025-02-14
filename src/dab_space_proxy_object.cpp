/** \file dab_space_proxy_object.cpp
*/

#include "dab_space_proxy_object.h"

using namespace dab;
using namespace dab::space;

SpaceProxyObject::SpaceProxyObject()
: mSpaceObject(nullptr)
, mNeighborGroup(nullptr)
{}

SpaceProxyObject::SpaceProxyObject(SpaceObject* pSpaceObject, NeighborGroup* pNeighborGroup)
: mSpaceObject(pSpaceObject)
, mNeighborGroup(pNeighborGroup)
{}

SpaceProxyObject::~SpaceProxyObject()
{}

SpaceProxyObject::operator std::string() const
{
    std::stringstream stream;
    
    stream << mSpaceObject;
    
	return stream.str();
}

std::string
SpaceProxyObject::info(int pPropagationLevel) const
{
    std::stringstream stream;
    
    stream << mSpaceObject->info(pPropagationLevel);

	return stream.str();
}