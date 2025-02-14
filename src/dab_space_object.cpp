/** \file dab_space_object.cpp
*/

#include "dab_space_object.h"
#include "dab_space.h"
#include "dab_space_neighbors.h"
#include "dab_space_neighbor_group.h"
#include "dab_space_neighbor_group_alg.h"

using namespace dab;
using namespace dab::space;

unsigned int SpaceObject::sObjectCount = 0;

SpaceObject::SpaceObject()
: mDim(0)
, mPosition(mDim)
, mId(sObjectCount++)
{
	mNeighbors = new Neighbors(this);
}

SpaceObject::SpaceObject(unsigned int pDim)
: mDim(pDim)
, mPosition(mDim)
, mId(sObjectCount++)
{
	mNeighbors = new Neighbors(this);
}

SpaceObject::SpaceObject(const Eigen::VectorXf& pPosition)
: mDim(pPosition.rows())
, mPosition(pPosition)
, mId(sObjectCount++)
{
	mNeighbors = new Neighbors(this);
}

SpaceObject::SpaceObject(const SpaceObject& pSpaceObject)
: mDim(pSpaceObject.mDim)
, mPosition(pSpaceObject.mPosition)
, mId(sObjectCount++)
{
	mNeighbors = new Neighbors(this);
    
	// add space object to the same spaces as the original space object
	unsigned int nGC = pSpaceObject.mNeighbors->neighborGroupCount();

	for(unsigned int i=0; i<nGC; ++i)
	{
		NeighborGroup* nG = pSpaceObject.mNeighbors->neighborGroup(i);
		Space* space = nG->space();
		bool vis = nG->visible();
		NeighborGroupAlg* neighborGroupAlg;
		if(nG->canHaveNeighbors() == true) neighborGroupAlg = new NeighborGroupAlg( *( nG->neighborGroupAlg() ) );
		else neighborGroupAlg = nullptr;
		
		space->addObject(this, vis, neighborGroupAlg);
	}
}

SpaceObject::~SpaceObject()
{
	delete mNeighbors;
}

unsigned int
SpaceObject::id() const
{
	return mId;
}

unsigned int
SpaceObject::dim() const
{
	return mDim;
}

Eigen::VectorXf&
SpaceObject::position()
{
	return mPosition;
}

const Eigen::VectorXf&
SpaceObject::position() const
{
	return mPosition;
}

void
SpaceObject::setPosition(const Eigen::VectorXf& pPosition) throw (Exception)
{
    if(pPosition.rows() != mDim) throw Exception("SPACE ERROR: dimension of supplied position " + std::to_string(pPosition.rows()) + " does not match required dimension of space object " + std::to_string(mDim), __FILE__, __FUNCTION__, __LINE__ );
	
	mPosition = pPosition;
}

void
SpaceObject::changePosition(const Eigen::VectorXf& pPosition) throw (Exception)
{
    if(pPosition.rows() != mDim) throw Exception("SPACE ERROR: dimension of supplied position " + std::to_string(pPosition.rows()), __FILE__, __FUNCTION__, __LINE__);
	
	mPosition += pPosition;
}

bool
SpaceObject::checkNeighborGroup(const std::string& pSpaceName) const
{
	return mNeighbors->checkNeighborGroup(pSpaceName);
}

void
SpaceObject::addNeighborGroup(NeighborGroup* pNeighborGroup) throw (Exception)
{
    try
    {
        mNeighbors->addNeighborGroup(pNeighborGroup);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to add neighbour group", __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
SpaceObject::removeNeighborGroup(NeighborGroup* pNeighborGroup) throw (Exception)
{
    try
    {
       mNeighbors->removeNeighborGroup(pNeighborGroup);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to remove neighbour group", __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
SpaceObject::visible(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighbors->visible(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get visibility of neighbour group in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
SpaceObject::setVisible(bool pVisible)
{
    mNeighbors->setVisible(pVisible);
}

void
SpaceObject::setVisible(const std::string& pSpaceName, bool pVisible)
{
    try
    {
        mNeighbors->setVisible(pSpaceName, pVisible);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to set visibility of neighbour group in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

Neighbors*
SpaceObject::neighbors()
{
	return mNeighbors;
}

unsigned int
SpaceObject::neighborGroupIndex(const std::string& pSpaceName) throw (Exception)
{
    try
    {
        return mNeighbors->neighborGroupIndex(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get index of neighbour group in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

NeighborGroup*
SpaceObject::neighborGroup(unsigned int pIndex) throw (Exception)
{
    try
    {
        return mNeighbors->neighborGroup(pIndex);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbour group at index " + std::to_string(pIndex), __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

NeighborGroup*
SpaceObject::neighborGroup(const std::string& pSpaceName) throw (Exception)
{
    try
    {
        return mNeighbors->neighborGroup(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbour group for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

Space*
SpaceObject::neighborSpace(const std::string& pSpaceName) throw (Exception)
{
    try
    {
        return mNeighbors->neighborGroup(pSpaceName)->space();
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbour space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
SpaceObject::canHaveNeighbors(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighbors->canHaveNeighbors(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to check neighbors for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

unsigned int
SpaceObject::maxNeighborCount(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighbors->maxNeighborCount(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get maximum neighbor count for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

unsigned int
SpaceObject::neighborCount(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighbors->neighborCount(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor count for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

float
SpaceObject::neighborRadius(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighbors->neighborRadius(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor radius for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
SpaceObject::replaceNeighborMode(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighbors->replaceNeighborMode(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to replace neighbor mode for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

SpaceObject*
SpaceObject::neighbor(const std::string& pSpaceName, unsigned int pIndex) throw (Exception)
{
    try
    {
        return mNeighbors->neighbor(pSpaceName, pIndex);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor at index " + std::to_string(pIndex) + " in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

SpaceNeighborRelation*
SpaceObject::neighborRelation(const std::string& pSpaceName, unsigned int pIndex) throw (Exception)
{
    try
    {
        return mNeighbors->neighborRelation(pSpaceName, pIndex);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor relation at index " + std::to_string(pIndex) + " in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
SpaceObject::neighborListFull(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighbors->full(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to check if neighbor list is full for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
SpaceObject::addNeighbor(const std::string& pSpaceName, SpaceObject* pNeighborObject) throw (Exception)
{
    try
    {
        return mNeighbors->addNeighbor(pSpaceName, pNeighborObject);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to add neighbor for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
SpaceObject::addNeighbor(const std::string& pSpaceName, SpaceObject* pNeighborObject, float pDistance, const Eigen::VectorXf& pDirection) throw(dab::Exception)
{
    try
    {
        return mNeighbors->addNeighbor(pSpaceName, pNeighborObject, pDistance, pDirection);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to add neighbor for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
SpaceObject::removeNeighbors()
{
	mNeighbors->removeNeighbors();
}

void
SpaceObject::removeNeighbors(const std::string& pSpaceName) throw(dab::Exception)
{
    try
    {
        mNeighbors->removeNeighbors(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to remove neighbors for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

SpaceObject::operator std::string() const
{
    std::stringstream stream;
    
    stream << "SpaceObject\n";
    stream << "Position [";

	for(unsigned int i=0; i<mDim; ++i)
    {
        stream << " " << mPosition[i];
    }
	stream << " ]\n";
    
    stream << *mNeighbors << "\n";
	
	return stream.str();
}

std::string
SpaceObject::info(int pPropagationLevel) const
{
    std::stringstream stream;
    
    stream << "SpaceObject\n";
    stream << "Position [";
    
	for(unsigned int i=0; i<mDim; ++i)
    {
        stream << " " << mPosition[i];
    }
	stream << " ]\n";
    
    stream << mNeighbors->info(pPropagationLevel - 1) << "\n";
	
	return stream.str();
}