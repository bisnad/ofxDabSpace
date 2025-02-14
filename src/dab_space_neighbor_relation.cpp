/** \file dab_space_neighbor_relation.cpp
*/

#include "dab_space_neighbor_relation.h"
#include "dab_space_object.h"

using namespace dab;
using namespace dab::space;

SpaceNeighborRelation::SpaceNeighborRelation()
: mObject(nullptr)
, mNeighborObject(nullptr)
, mValue(1)
, mDirection(1)
, mDistance(0.0)
{}

SpaceNeighborRelation::SpaceNeighborRelation(unsigned int pDim)
: mObject(nullptr)
, mNeighborObject(nullptr)
, mValue(pDim)
, mDirection(pDim)
, mDistance(0.0)
{}

SpaceNeighborRelation::SpaceNeighborRelation(SpaceObject* pObject, SpaceObject* pNeighborObject) throw (Exception)
: mObject(pObject)
, mNeighborObject(pNeighborObject)
, mValue(mNeighborObject->position() - mObject->position())
, mDirection(mValue)
, mDistance(mDirection.norm())
{
    if(mObject == mNeighborObject) throw Exception("SPACE ERROR: space object and neighbor can't refer to one and the same object", __FILE__, __FUNCTION__, __LINE__);
    if(mObject->dim() != mNeighborObject->dim()) throw Exception("SPACE ERROR: space object and neighbor must have identical dimension", __FILE__, __FUNCTION__, __LINE__);
}

SpaceNeighborRelation::SpaceNeighborRelation(SpaceObject* pObject, SpaceObject* pNeighborObject, float pDistance, const Eigen::VectorXf& pDirection) throw (Exception)
: mObject(pObject)
, mNeighborObject(pNeighborObject)
, mValue(pDirection)
, mDirection(pDirection)
, mDistance(pDistance)
{
    if(mObject == mNeighborObject) throw Exception("SPACE ERROR: space object and neighbor can't refer to one and the same object", __FILE__, __FUNCTION__, __LINE__);
    if(mObject->dim() != mNeighborObject->dim()) throw Exception("SPACE ERROR: space object and neighbor must have identical dimension", __FILE__, __FUNCTION__, __LINE__);
    if(mDirection.rows() != mNeighborObject->dim()) throw Exception("SPACE ERROR: direction and neighbor must have identical dimension", __FILE__, __FUNCTION__, __LINE__);
}

SpaceNeighborRelation::SpaceNeighborRelation(SpaceObject* pObject, SpaceObject* pNeighborObject, const Eigen::VectorXf& pValue, const Eigen::VectorXf& pDirection, float pDistance) throw (Exception)
: mObject(pObject)
, mNeighborObject(pNeighborObject)
, mValue(pValue)
, mDirection(pDirection)
, mDistance(pDistance)
{
    if(mObject == mNeighborObject) throw Exception("SPACE ERROR: space object and neighbor can't refer to one and the same object", __FILE__, __FUNCTION__, __LINE__);
    if(mObject->dim() != mNeighborObject->dim()) throw Exception("SPACE ERROR: space object and neighbor must have identical dimension", __FILE__, __FUNCTION__, __LINE__);
    if(mDirection.rows() != mNeighborObject->dim()) throw Exception("SPACE ERROR: direction and neighbor must have identical dimension", __FILE__, __FUNCTION__, __LINE__);
}

SpaceNeighborRelation::~SpaceNeighborRelation()
{}

SpaceObject*
SpaceNeighborRelation::neighbor()
{
	return mNeighborObject;
}

const Eigen::VectorXf&
SpaceNeighborRelation::value() const
{
	return mValue;
}

const Eigen::VectorXf&
SpaceNeighborRelation::direction() const
{
	return mDirection;
}

float
SpaceNeighborRelation::distance() const
{
	return mDistance;
}

void
SpaceNeighborRelation::setNeighbor(SpaceObject* pNeighborObject) throw (Exception)
{
    if(mObject == pNeighborObject) throw Exception("SPACE ERROR: space object and neighbor can't refer to one and the same object", __FILE__, __FUNCTION__, __LINE__);
    if(mObject->dim() != pNeighborObject->dim()) throw Exception("SPACE ERROR: space object and neighbor must have identical dimension", __FILE__, __FUNCTION__, __LINE__);
	
	mNeighborObject = pNeighborObject;
}

void
SpaceNeighborRelation::setValue(const Eigen::VectorXf& pValue) throw (Exception)
{
    if(mValue.rows() != pValue.rows()) throw Exception("SPACE ERROR: wrong dimension mismatch", __FILE__, __FUNCTION__, __LINE__);
	
	mValue = pValue;
}

void
SpaceNeighborRelation::setDirection(const Eigen::VectorXf& pDirection) throw (Exception)
{
    if(mDirection.rows() != pDirection.rows()) throw Exception("SPACE ERROR: wrong dimension mismatch", __FILE__, __FUNCTION__, __LINE__);
	
	mDirection = pDirection;
}

void
SpaceNeighborRelation::setDistance(float pDistance)
{
	mDistance = pDistance;
}

void
SpaceNeighborRelation::set(SpaceObject* pObject, SpaceObject* pNeighborObject) throw (Exception)
{
    if(pObject == pNeighborObject) throw Exception("SPACE ERROR: space object and neighbor can't refer to one and the same object", __FILE__, __FUNCTION__, __LINE__);
    if(pObject->dim() != pNeighborObject->dim()) throw Exception("SPACE ERROR: space object and neighbor must have identical dimension", __FILE__, __FUNCTION__, __LINE__);
    if(pObject->dim() != mValue.rows()) throw Exception("SPACE ERROR: space object and value must have identical dimension", __FILE__, __FUNCTION__, __LINE__);
    if(pObject->dim() != mDirection.rows()) throw Exception("SPACE ERROR: space object and direction must have identical dimension", __FILE__, __FUNCTION__, __LINE__);
	
	mObject = pObject;
	mNeighborObject = pNeighborObject;
	mValue = mNeighborObject->position() - mObject->position();
	mDirection = mValue;
	mDistance = mDirection.norm();
}

std::string
SpaceNeighborRelation::info(int pPropagationLevel) const
{
    std::stringstream stream;
    
    stream << "object: " << mObject->info(pPropagationLevel - 1) << "\n";
    stream << "neighbor: " << mNeighborObject->info(pPropagationLevel - 1) << "\n";
    stream << "value: " << mValue << "\n";
    stream << "direction: " << mDirection << "\n";
    stream << "distance: " << mDistance << "\n";
	
	return stream.str();
}


SpaceNeighborRelation::operator std::string() const
{
    std::stringstream stream;
    
    stream << "object: " << *mObject << "\n";
    stream << "neighbor: " << *mNeighborObject << "\n";
    stream << "value: " << mValue << "\n";
    stream << "direction: " << mDirection << "\n";
    stream << "distance: " << mDistance << "\n";
	
	return stream.str();
}
