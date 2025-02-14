/** \file dab_space_neighbor_group_alg.cpp
*/

#include "dab_space_neighbor_group_alg.h"
#include "dab_space_object.h"
#include "dab_space_neighbor_relation.h"
#include "dab_space_neighbor_group.h"
#include "dab_space.h"

using namespace dab;
using namespace dab::space;


float NeighborGroupAlg::sNeighborRadius = 5.0;
int NeighborGroupAlg::sMaxNeighborCount = 10;
bool NeighborGroupAlg::sReplaceNeighborMode = false;

NeighborGroupAlg::NeighborGroupAlg()
: mNeighborGroup(nullptr)
, mNeighborRadius(sNeighborRadius)
, mMaxNeighborCount(sMaxNeighborCount)
, mReplaceNeighborMode(sReplaceNeighborMode)
{}

NeighborGroupAlg::NeighborGroupAlg(float pNeighborRadius, int pMaxNeighborCount, bool pReplaceNeighborMode)
: mNeighborGroup(nullptr)
, mNeighborRadius(pNeighborRadius)
, mMaxNeighborCount(pMaxNeighborCount)
, mReplaceNeighborMode(pReplaceNeighborMode)
{}

NeighborGroupAlg::NeighborGroupAlg(NeighborGroupAlg& pNeighborGroupAlg)
: mNeighborGroup(nullptr)
, mNeighborRadius(pNeighborGroupAlg.mNeighborRadius)
, mMaxNeighborCount(pNeighborGroupAlg.mMaxNeighborCount)
, mReplaceNeighborMode(pNeighborGroupAlg.mReplaceNeighborMode)
{}

NeighborGroupAlg::~NeighborGroupAlg()
{}

void
NeighborGroupAlg::setNeighborGroup(NeighborGroup* pNeighborGroup)
{
	mNeighborGroup = pNeighborGroup;
    mNeighborDirection.resize(mNeighborGroup->space()->dim(), 1);
}

float
NeighborGroupAlg::neighborRadius() const
{
	return mNeighborRadius;
}

int
NeighborGroupAlg::maxNeighborCount() const
{
	return mMaxNeighborCount;
}

bool
NeighborGroupAlg::replaceNeighborMode() const
{
	return mReplaceNeighborMode;
}

bool
NeighborGroupAlg::full() const
{
	if(mMaxNeighborCount > -1 && mReplaceNeighborMode == false && mNeighborGroup->neighborCount() >= mMaxNeighborCount) return true;
	return false;
}

void
NeighborGroupAlg::setNeighborRadius(float pNeighborRadius)
{
	mNeighborRadius = pNeighborRadius;
}

void 
NeighborGroupAlg::setMaxNeighborCount(int pMaxNeighborCount)
{
	mMaxNeighborCount = pMaxNeighborCount;
}

bool
NeighborGroupAlg::createNeighbor(SpaceObject* pObject1, SpaceObject* pObject2)
{
    //	std::cout << "NeighborListAlg::createNeighbor(SpaceObject* pObject1, SpaceObject* pObject2)\n";
    //	std::cout << "object1 pos " << pObject1->position() << " object2 pos " << pObject2->position() << "\n";
	
	// check whether more neighbors can be added
	if(mMaxNeighborCount == 0) return false; // maximum number of neighbors exceeded
	
    std::vector<SpaceNeighborRelation*>& neighborRelations = mNeighborGroup->mNeighborRelations;
	int neighborCount = neighborRelations.size();
	
	if(mMaxNeighborCount > 0 && mMaxNeighborCount <= neighborCount && mReplaceNeighborMode == false) return false; // maximum number of neighbors exceeded and existing neighbors cannot be replaced
	
	// calculate distance and direction
	Eigen::VectorXf& object1Position = pObject1->position();
	Eigen::VectorXf& object2Position = pObject2->position();
	unsigned int dim = mNeighborDirection.rows();
	float neighborDistance = 0;
	for(unsigned int d=0; d<dim; ++d)
	{
		mNeighborDirection[d] = object2Position[d] - object1Position[d];
		neighborDistance += mNeighborDirection[d] * mNeighborDirection[d];
	}
	neighborDistance = sqrt( neighborDistance );
	
    //	std::cout << "neighborDist " << neighborDistance << "\n";
	
	// check whether neighbor is outside visibility radius
	if(mNeighborRadius >= 0.0 && mNeighborRadius < neighborDistance ) return false;
    
	// check if both neighbor list is full and neighbors can be replaced but all existing neigbors are closer than new neighbor
    // TODO: check if this doesn't cause an out of bounds error for mMaxNeighborCount = 0
	if( mMaxNeighborCount <= neighborCount && mReplaceNeighborMode == true && neighborRelations.back()->distance() < neighborDistance ) return false;
    
	// create new neighbor relation
	SpaceNeighborRelation* neighborRelation = new SpaceNeighborRelation(pObject1, pObject2, neighborDistance, mNeighborDirection );
	
	//std::cout << "insert neighborRelation " << *neighborRelation << "\n";
	//std::cout << "neighborRelation list before insertion " << *mNeighborList << "\n";
    
	// first neigbhor
	if(neighborCount == 0)
	{
		//std::cout << "first insert\n";
        
		neighborRelations.push_back(neighborRelation);
		return true;
	}
	
	// closest neighbor
	if(neighborRelations.front()->distance() >= neighborDistance)
	{
		//std::cout << "closest insert\n";
        
		neighborRelations.insert(neighborRelations.begin(), neighborRelation);
	}
	else // normal insert
	{
		//std::cout << "normal insert\n";
        
		unsigned int neighborCount = neighborRelations.size();
		unsigned int insertionIndex = neighborCount;
		
		for(unsigned int i=neighborCount - 1; i>=0; --i)
		{
			if(neighborRelations[i]->distance() > neighborDistance) insertionIndex = i;
			else break;
		}
		
		neighborRelations.insert(neighborRelations.begin() + insertionIndex, neighborRelation);
	}
	
	neighborCount++;
	
	// check if we need to get rid of last neighor
	if(mMaxNeighborCount != -1 && neighborCount > mMaxNeighborCount)
	{
		SpaceNeighborRelation* lastRelation = neighborRelations.back();
		
		if(lastRelation == neighborRelation)
		{
			delete lastRelation;
			neighborRelations.pop_back();
			
			return false;
		}
		
		delete lastRelation;
		neighborRelations.pop_back();
		
		//std::cout << "remove last neighbor\n";
	}
	
	//std::cout << "neighbor list after insertion " << *mNeighborList << "\n";
	
	return true;
}

bool
NeighborGroupAlg::createNeighbor(SpaceObject* pObject1, SpaceObject* pObject2, float pDistance, const Eigen::VectorXf& pDirection)
{
	//std::cout << "NeighborListAlg::createNeighbor(SpaceObject* pObject1, SpaceObject* pObject2, real pDistance, const math::Vector<real>& pDirection)\n";
    
	if(mMaxNeighborCount == 0) return false;
    
    std::vector<SpaceNeighborRelation*>& neighborRelations = mNeighborGroup->mNeighborRelations;
    
	SpaceNeighborRelation* neighborRelation = new SpaceNeighborRelation(pObject1, pObject2, pDistance, pDirection);
	
	// neighbor outside visibility radius
	if(mNeighborRadius >= 0.0 && mNeighborRadius < pDistance)
	{
		delete neighborRelation;
		return false;
	}
	
	// neighborlist full
	if(mMaxNeighborCount != -1 && neighborRelations.size() >= mMaxNeighborCount)
	{
		// neighborlist non replacing
		if(mReplaceNeighborMode == false)
		{
			delete neighborRelation;
			return false;
		}
		else
		{
			// new neighbor is further away than all other neighbors
			if(neighborRelations.back()->distance() < pDistance)
			{
				delete neighborRelation;
				return false;
			}
		}
	}
    
	// insert new neighborRelation
	//std::cout << "insert neighborRelation " << *neighborRelation << "\n";
	//std::cout << "neighborRelation list before insertion " << *mNeighborList << "\n";
    
	// first neigbhor
	if(neighborRelations.size() == 0)
	{
		neighborRelations.push_back(neighborRelation);
		return true;
	}
	
	// closest neighbor
	if(neighborRelations.front()->distance() >= pDistance)
	{
		neighborRelations.insert(neighborRelations.begin(), neighborRelation);
	}
	else // normal insert
	{
		unsigned int neighborCount = neighborRelations.size();
		unsigned int insertionIndex = neighborCount;
		
		for(unsigned int i=neighborCount - 1; i>=0; --i)
		{
			if(neighborRelations[i]->distance() > pDistance) insertionIndex = i;
			else break;
		}
		
		neighborRelations.insert(neighborRelations.begin() + insertionIndex, neighborRelation);
	}
	
	// check if we need to get rid of last neighor
	if(mMaxNeighborCount != -1 && neighborRelations.size() > mMaxNeighborCount)
	{
		SpaceNeighborRelation* lastRelation = neighborRelations.back();
		
		if(lastRelation == neighborRelation)
		{
			delete lastRelation;
			neighborRelations.pop_back();
			
			return false;
		}
		
		delete lastRelation;
		neighborRelations.pop_back();
	}
	
	//std::cout << "neighbor list after insertion " << *mNeighborList << "\n";
	
	return true;
}

bool
NeighborGroupAlg::createNeighbor(SpaceObject* pObject1, SpaceObject* pObject2, const Eigen::VectorXf& pValue, const Eigen::VectorXf& pDirection, float pDistance)
{
	//std::cout << "NeighborGroupAlg::createNeighbor(SpaceObject* pObject1, SpaceObject* pObject2, const math::Vector<real>& pValue, const math::Vector<real>& pDirection, pDistance)\n";
    
	if(mMaxNeighborCount == 0) return false;
    
    std::vector<SpaceNeighborRelation*>& neighborRelations = mNeighborGroup->mNeighborRelations;
    
	SpaceNeighborRelation* neighborRelation = new SpaceNeighborRelation(pObject1, pObject2, pValue, pDirection, pDistance);
	
	// neighbor outside visibility radius
	if(mNeighborRadius >= 0.0 && mNeighborRadius < pDistance)
	{
		delete neighborRelation;
		return false;
	}
	
	// neighborlist full
	if(mMaxNeighborCount != -1 && neighborRelations.size() >= mMaxNeighborCount)
	{
		// neighborlist non replacing
		if(mReplaceNeighborMode == false)
		{
			delete neighborRelation;
			return false;
		}
		else
		{
			// new neighbor is further away than all other neighbors
			if(neighborRelations.back()->distance() < pDistance)
			{
				delete neighborRelation;
				return false;
			}
		}
	}
    
	// insert new neighborRelation
	//std::cout << "insert neighborRelation " << *neighborRelation << "\n";
	//std::cout << "neighborRelation list before insertion " << *mNeighborList << "\n";
    
	// first neigbhor
	if(neighborRelations.size() == 0)
	{
		neighborRelations.push_back(neighborRelation);
		return true;
	}
	
	// closest neighbor
	if(neighborRelations.front()->distance() >= pDistance)
	{
		neighborRelations.insert(neighborRelations.begin(), neighborRelation);
	}
	else // normal insert
	{
		unsigned int neighborCount = neighborRelations.size();
		unsigned int insertionIndex = neighborCount;
		
		for(unsigned int i=neighborCount - 1; i>=0; --i)
		{
			if(neighborRelations[i]->distance() > pDistance) insertionIndex = i;
			else break;
		}
		
		neighborRelations.insert(neighborRelations.begin() + insertionIndex, neighborRelation);
	}
	
	// check if we need to get rid of last neighor
	if(mMaxNeighborCount != -1 && neighborRelations.size() > mMaxNeighborCount)
	{
		SpaceNeighborRelation* lastRelation = neighborRelations.back();
		
		if(lastRelation == neighborRelation)
		{
			delete lastRelation;
			neighborRelations.pop_back();
			
			return false;
		}
		
		delete lastRelation;
		neighborRelations.pop_back();
	}
	
	//std::cout << "neighbor list after insertion " << *mNeighborList << "\n";
	
	return true;
}

void
NeighborGroupAlg::removeNeighbor(SpaceObject* pNeighborObject)
{
    std::vector< SpaceNeighborRelation* >& neighborRelations = mNeighborGroup->mNeighborRelations;
	int neighborCount = neighborRelations.size();
    
	for(int i=neighborCount - 1; i>=0; --i)
	{
		SpaceNeighborRelation* neighborRelation = neighborRelations[i];
		
		if( neighborRelation->neighbor() == pNeighborObject )
		{
			neighborRelations.erase(neighborRelations.begin() + i);
			delete neighborRelation;
		}
	}
}

void
NeighborGroupAlg::removeNeighbor(unsigned int pNeighborIndex)
{
    std::vector< SpaceNeighborRelation* >& neighborRelations = mNeighborGroup->mNeighborRelations;
	int neighborCount = neighborRelations.size();
	
	if(pNeighborIndex < neighborCount)
	{
		SpaceNeighborRelation* neighborRelation = neighborRelations[pNeighborIndex];
		neighborRelations.erase(neighborRelations.begin() + pNeighborIndex);
		delete neighborRelation;
	}
}

void
NeighborGroupAlg::removeNeighbors()
{
    std::vector< SpaceNeighborRelation* >& neighborRelations = mNeighborGroup->mNeighborRelations;
	unsigned int neighborCount = neighborRelations.size();
	
	for(unsigned int i=0; i<neighborCount; ++i) delete neighborRelations[i];
	neighborRelations.clear();
}

NeighborGroupAlg::operator std::string() const
{
    std::stringstream stream;
    
    stream << "neighborRadius " << mNeighborRadius << " maxNeighborCount " << mMaxNeighborCount << " replaceNeighborMode " << mReplaceNeighborMode << "\n";
    
	return stream.str();
}

std::string
NeighborGroupAlg::info(int pPropagationLevel) const
{
    std::stringstream stream;
    
    stream << "neighborRadius " << mNeighborRadius << " maxNeighborCount " << mMaxNeighborCount << " replaceNeighborMode " << mReplaceNeighborMode << "\n";
    
	return stream.str();
}