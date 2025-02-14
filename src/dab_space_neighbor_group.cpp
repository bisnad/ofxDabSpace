/** \file dab_space_neighbor_group.cpp
*/

#include "dab_space_neighbor_group.h"
#include "dab_space.h"
#include "dab_space_object.h"
#include "dab_space_neighbor_relation.h"
#include "dab_space_neighbor_group_alg.h"

using namespace dab;
using namespace dab::space;

NeighborGroup::NeighborGroup()
: mSpaceObject(nullptr)
, mSpace(nullptr)
, mNeighborGroupAlg(NULL)
{}

NeighborGroup::NeighborGroup(SpaceObject* pSpaceObject, Space* pSpace, bool pVisible)
: mSpaceObject(pSpaceObject)
, mSpace(pSpace)
, mVisible(pVisible)
, mNeighborGroupAlg(nullptr)
{}

NeighborGroup::NeighborGroup(SpaceObject* pSpaceObject, Space* pSpace, bool pVisible, NeighborGroupAlg* pNeighborGroupAlg)
: mSpaceObject(pSpaceObject)
, mSpace(pSpace)
, mVisible(pVisible)
, mNeighborGroupAlg(pNeighborGroupAlg)
{
	if( mNeighborGroupAlg != nullptr) mNeighborGroupAlg->setNeighborGroup(this);
}

NeighborGroup::~NeighborGroup()
{
	delete mNeighborGroupAlg;
}

SpaceObject*
NeighborGroup::spaceObject()
{
	return mSpaceObject;
}

const SpaceObject*
NeighborGroup::spaceObject() const
{
	return mSpaceObject;
}

const std::string&
NeighborGroup::name() const
{
	return mSpace->name();
}

Space*
NeighborGroup::space()
{
	return mSpace;
}

bool
NeighborGroup::visible() const
{
	return mVisible;
}

void
NeighborGroup::setVisible(bool pVisible)
{
	mVisible = pVisible;
}

bool
NeighborGroup::canHaveNeighbors() const
{
	return ( mNeighborGroupAlg != nullptr && mNeighborGroupAlg->maxNeighborCount() != 0 ) ? true : false;
}

NeighborGroupAlg*
NeighborGroup::neighborGroupAlg()
{
	return mNeighborGroupAlg;
}

void
NeighborGroup::setNeighborGroupAlg(NeighborGroupAlg* pNeighborGroupAlg)
{
	if(mNeighborGroupAlg != nullptr) delete mNeighborGroupAlg;
	mNeighborGroupAlg = pNeighborGroupAlg;
	if( mNeighborGroupAlg != nullptr) mNeighborGroupAlg->setNeighborGroup(this);
}

float
NeighborGroup::neighborRadius() const throw (Exception)
{
    if(mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object cannot have neighbors", __FILE__, __FUNCTION__, __LINE__);

	return mNeighborGroupAlg->neighborRadius();
}

unsigned int
NeighborGroup::neighborCount() const
{
	return mNeighborRelations.size();
}

unsigned int
NeighborGroup::maxNeighborCount() const throw (Exception)
{
	if(mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object cannot have neighbors", __FILE__, __FUNCTION__, __LINE__);
    
	return mNeighborGroupAlg->maxNeighborCount();
}

bool
NeighborGroup::replaceNeighborMode() const throw (Exception)
{
	if(mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object cannot have neighbors", __FILE__, __FUNCTION__, __LINE__);
    
	return mNeighborGroupAlg->replaceNeighborMode();
}

bool
NeighborGroup::full() const throw (Exception)
{
    if(mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object cannot have neighbors", __FILE__, __FUNCTION__, __LINE__);
    
	return mNeighborGroupAlg->full();
}

void
NeighborGroup::setNeighborRadius(float pNeighborRadius) throw (Exception)
{
    if(mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object cannot have neighbors", __FILE__, __FUNCTION__, __LINE__);
    
	mNeighborGroupAlg->setNeighborRadius(pNeighborRadius);
}

SpaceObject*
NeighborGroup::neighbor(unsigned int pIndex) throw (Exception)
{
    if(pIndex >= mNeighborRelations.size()) throw Exception("SPACE ERROR: index " + std::to_string(pIndex) + " exceeds number of neighbors " + std::to_string(mNeighborRelations.size()), __FILE__, __FUNCTION__, __LINE__);
    
	return mNeighborRelations[pIndex]->neighbor();
}

const SpaceObject*
NeighborGroup::neighbor(unsigned int pIndex) const throw (Exception)
{
    if(pIndex >= mNeighborRelations.size()) throw Exception("SPACE ERROR: index " + std::to_string(pIndex) + " exceeds number of neighbors " + std::to_string(mNeighborRelations.size()), __FILE__, __FUNCTION__, __LINE__);
    
	return mNeighborRelations[pIndex]->neighbor();
}

const Eigen::VectorXf&
NeighborGroup::value(unsigned int pIndex) const throw (Exception)
{
    if(pIndex >= mNeighborRelations.size()) throw Exception("SPACE ERROR: index " + std::to_string(pIndex) + " exceeds number of neighbors " + std::to_string(mNeighborRelations.size()), __FILE__, __FUNCTION__, __LINE__);
    
	return mNeighborRelations[pIndex]->value();
}

const Eigen::VectorXf&
NeighborGroup::direction(unsigned int pIndex) const throw (Exception)
{
    if(pIndex >= mNeighborRelations.size()) throw Exception("SPACE ERROR: index " + std::to_string(pIndex) + " exceeds number of neighbors " + std::to_string(mNeighborRelations.size()), __FILE__, __FUNCTION__, __LINE__);
    
	return mNeighborRelations[pIndex]->direction();
}

float
NeighborGroup::distance(unsigned int pIndex) const throw (Exception)
{
    if(pIndex >= mNeighborRelations.size()) throw Exception("SPACE ERROR: index " + std::to_string(pIndex) + " exceeds number of neighbors " + std::to_string(mNeighborRelations.size()), __FILE__, __FUNCTION__, __LINE__);
	
	return mNeighborRelations[pIndex]->distance();
}

std::vector<SpaceNeighborRelation*>&
NeighborGroup::neighborRelations()
{
	return mNeighborRelations;
}

SpaceNeighborRelation*
NeighborGroup::neighborRelation(unsigned int pIndex) throw (Exception)
{
    if(pIndex >= mNeighborRelations.size()) throw Exception("SPACE ERROR: index " + std::to_string(pIndex) + " exceeds number of neighbors " + std::to_string(mNeighborRelations.size()), __FILE__, __FUNCTION__, __LINE__);
	
	return mNeighborRelations[pIndex];
}

const SpaceNeighborRelation*
NeighborGroup::neighborRelation(unsigned int pIndex) const throw (Exception)
{
    if(pIndex >= mNeighborRelations.size()) throw Exception("SPACE ERROR: index " + std::to_string(pIndex) + " exceeds number of neighbors " + std::to_string(mNeighborRelations.size()), __FILE__, __FUNCTION__, __LINE__);
	
	return mNeighborRelations[pIndex];
}

SpaceNeighborRelation*
NeighborGroup::operator[](unsigned int pIndex) throw (Exception)
{
    if(pIndex >= mNeighborRelations.size()) throw Exception("SPACE ERROR: index " + std::to_string(pIndex) + " exceeds number of neighbors " + std::to_string(mNeighborRelations.size()), __FILE__, __FUNCTION__, __LINE__);
	
	return mNeighborRelations[pIndex];
}

bool
NeighborGroup::addNeighbor(SpaceObject* pNeighborObject) throw (Exception)
{
    if(mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object cannot have neighbors", __FILE__, __FUNCTION__, __LINE__);
    
	return mNeighborGroupAlg->createNeighbor(mSpaceObject, pNeighborObject);
}

bool
NeighborGroup::addNeighbor(SpaceObject* pNeighborObject, float pDistance, const Eigen::VectorXf& pDirection) throw (Exception)
{
    if(mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object cannot have neighbors", __FILE__, __FUNCTION__, __LINE__);
    
	return mNeighborGroupAlg->createNeighbor(mSpaceObject, pNeighborObject, pDistance, pDirection);
}

void
NeighborGroup::removeNeighbor(SpaceObject* pNeighborObject) throw (Exception)
{
    if(mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object cannot have neighbors", __FILE__, __FUNCTION__, __LINE__);
    
	mNeighborGroupAlg->removeNeighbor(pNeighborObject);
}

void
NeighborGroup::removeNeighbor(unsigned int pNeighborIndex) throw (Exception)
{
    if(mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object cannot have neighbors", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mNeighborGroupAlg->removeNeighbor(pNeighborIndex);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to remove neighbor at index " + std::to_string(pNeighborIndex), __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
NeighborGroup::removeNeighbors() throw (Exception)
{
    if(mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object cannot have neighbors", __FILE__, __FUNCTION__, __LINE__);
    
	mNeighborGroupAlg->removeNeighbors();
}


NeighborGroup::operator std::string() const
{
    std::stringstream stream;
    
	if(mNeighborGroupAlg == nullptr)
    {
        stream << "empty NeighborGroup\n";
    }
	else
	{
		unsigned int neighborCount = mNeighborRelations.size();
        
        stream << "neighborCount: " << neighborCount << "\n";
    }
	
	return stream.str();
}

std::string
NeighborGroup::info(int pPropagationLevel) const
{
    std::stringstream stream;
    
	if(mNeighborGroupAlg == nullptr)
    {
        stream << "empty NeighborGroup\n";
    }
	else
	{
		unsigned int neighborCount = mNeighborRelations.size();
        
        stream << "neighborCount: " << neighborCount << "\n";
        
		if(pPropagationLevel != 0)
		{
            stream << "NeighborListAlg: ";
            stream << mNeighborGroupAlg->info(pPropagationLevel - 1) << "\n";

			if(neighborCount > 0)
			{
                stream << "Neighbors\n";

				for(unsigned int i=0; i<neighborCount; ++i)
				{
                    stream << mNeighborRelations[i]->info(pPropagationLevel - 1) << "\n";
				}
			}
		}
	}
	
	return stream.str();
}