/** \file dab_space_neighbors.cpp
*/

#include "dab_space_neighbors.h"
#include "dab_space_object.h"
#include "dab_space_neighbor_relation.h"
#include "dab_space_neighbor_group.h"
#include "dab_space.h"

using namespace dab;
using namespace dab::space;

Neighbors::Neighbors()
: mSpaceObject(nullptr)
{}

Neighbors::Neighbors(SpaceObject* pSpaceObject)
: mSpaceObject(pSpaceObject)
{}

Neighbors::~Neighbors()
{
	unsigned int neighborGroupCount = mNeighborGroups.size();
	
	if(neighborGroupCount > 0)
	{
		for(int i=neighborGroupCount - 1; i>=0; --i)
		{
			NeighborGroup* neighborGroup = mNeighborGroups[i];
			Space* space = neighborGroup->space();
			space->removeObject(mSpaceObject);
		}
		mNeighborGroups.clear();
	}
}

unsigned int
Neighbors::neighborGroupCount() const
{
	return mNeighborGroups.size();
}

bool
Neighbors::checkNeighborGroup(const std::string& pSpaceName)
{
	return mNeighborGroups.contains(pSpaceName);
}

unsigned int
Neighbors::neighborGroupIndex(const std::string& pSpaceName) throw (Exception)
{
	try
	{
		return mNeighborGroups.index(pSpaceName);
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: neighbor group index not found for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
	}
}

NeighborGroup*
Neighbors::neighborGroup(unsigned int pIndex) throw (Exception)
{
    if(pIndex >= mNeighborGroups.size()) throw Exception("SPACE ERROR: index " + std::to_string(pIndex) + " exceeds number of neighbor groups " + std::to_string(mNeighborGroups.size()), __FILE__, __FUNCTION__, __LINE__);
	
	return mNeighborGroups[pIndex];
}

NeighborGroup*
Neighbors::neighborGroup(const std::string& pSpaceName) throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName];
    }
    catch (Exception&  e)
    {
        e += Exception("SPACE ERROR: neighborgroup not found for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
Neighbors::addNeighborGroup(NeighborGroup* pNeighborGroup) throw (Exception)
{
    const std::string& spaceName = pNeighborGroup->space()->name();
    
    try
    {
        mNeighborGroups.add( spaceName, pNeighborGroup );
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to add neighbor group for space " + spaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
Neighbors::removeNeighborGroup(NeighborGroup* pNeighborGroup) throw (Exception)
{
    const std::string& spaceName = pNeighborGroup->space()->name();
    
    try
    {
        mNeighborGroups.remove(spaceName);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to remove neighbor group for space " + spaceName , __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
Neighbors::canHaveNeighbors(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->canHaveNeighbors();
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to test for neighbors for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
Neighbors::visible(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->visible();
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to test for visibility for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
Neighbors::setVisible(bool pVisible)
{
	unsigned int neighborGroupCount = mNeighborGroups.size();
	for(unsigned int i=0; i<neighborGroupCount; ++i) mNeighborGroups[i]->setVisible(pVisible);
}

void
Neighbors::setVisible(const std::string& pSpaceName, bool pVisible) throw (Exception)
{
    try
    {
        mNeighborGroups[pSpaceName]->setVisible(pVisible);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to set visibility for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

float
Neighbors::neighborRadius(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->neighborRadius();
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor radius for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
Neighbors::replaceNeighborMode(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->replaceNeighborMode();
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to replace neighbor mode for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

unsigned int
Neighbors::maxNeighborCount(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->maxNeighborCount();
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get maximum neighbor count for space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

unsigned int
Neighbors::neighborCount(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->neighborCount();
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor count in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
Neighbors::full(const std::string& pSpaceName) const throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->full();
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to test if neighbours contains maximum number of neighbors in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

SpaceObject*
Neighbors::neighbor(const std::string& pSpaceName, unsigned int pIndex) throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->neighbor(pIndex);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor " + std::to_string(pIndex) + " in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

const Eigen::VectorXf&
Neighbors::direction(const std::string& pSpaceName, unsigned int pIndex) const throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->direction(pIndex);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor " + std::to_string(pIndex) + " direction in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

float
Neighbors::distance(const std::string& pSpaceName, unsigned int pIndex) const throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->distance(pIndex);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor" + std::to_string(pIndex) + " distance in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

std::vector<SpaceNeighborRelation*>&
Neighbors::neighborRelations(const std::string& pSpaceName) throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->neighborRelations();
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor relations in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

SpaceNeighborRelation*
Neighbors::neighborRelation(const std::string& pSpaceName, unsigned int pIndex) throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->neighborRelation(pIndex);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get neighbor relation " + std::to_string(pIndex) + " in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
Neighbors::addNeighbor(const std::string& pSpaceName, SpaceObject* pNeighborObject) throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->addNeighbor(pNeighborObject);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to add neighbor to space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

bool
Neighbors::addNeighbor(const std::string& pSpaceName, SpaceObject* pNeighborObject, float pDistance, const Eigen::VectorXf& pDirection) throw (Exception)
{
    try
    {
        return mNeighborGroups[pSpaceName]->addNeighbor(pNeighborObject, pDistance, pDirection);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to add neighbor to space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
Neighbors::removeNeighbors()
{
	unsigned int neighborGroupCount = mNeighborGroups.size();
	for(unsigned int i=0; i<neighborGroupCount; ++i) mNeighborGroups[i]->removeNeighbors();
}

void
Neighbors::removeNeighbors(const std::string& pSpaceName) throw (Exception)
{
    try
    {
        mNeighborGroups[pSpaceName]->removeNeighbors();
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to remove neighbors in space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

Neighbors::operator std::string() const
{
	std::stringstream stream;
    
    stream << "Neighbors\n";
    
	unsigned int neighborGroupCount = mNeighborGroups.size();
	for(unsigned int i=0; i<neighborGroupCount; ++i)
	{
        stream << "neighbor collection for space : " + mNeighborGroups.key(i) + "\n";
        stream << *mNeighborGroups[i] << "\n";
	}
	
	return stream.str();
}

std::string
Neighbors::info(int pPropagationLevel) const
{
	std::stringstream stream;
    
    stream << "Neighbors\n";
    
	unsigned int neighborGroupCount = mNeighborGroups.size();
	for(unsigned int i=0; i<neighborGroupCount; ++i)
	{
        stream << "neighbor collection for space : " + mNeighborGroups.key(i) + "\n";
        stream << mNeighborGroups[i]->info(pPropagationLevel - 1) << "\n";
	}
	
	return stream.str();
}