/** \file dab_space_proxy_object.h
*/

#ifndef _dab_space_proxy_object_h_
#define _dab_space_proxy_object_h_

#include <iostream>
#include <Eigen/Dense>
#include "dab_exception.h"
#include "dab_space_object.h"
#include "dab_space_neighbor_group.h"
#include "dab_space_neighbor_group_alg.h"

namespace dab
{

namespace space
{

class NeighborGroup;

class SpaceProxyObject
{
public:
    SpaceProxyObject(SpaceObject* pSpaceObject, NeighborGroup* pNeighborGroup);
    ~SpaceProxyObject();
    
    inline SpaceObject* spaceObject();
    
    inline const SpaceObject* spaceObject() const;
    
    inline NeighborGroup* neighborGroup();
    
    inline const NeighborGroup* neighborGroup() const;
    
    /**
     \brief return space object dimension
     \return space object dimension
     */
    inline unsigned int dim() const;
    
    /**
     \brief return space object position
     \return space object position
     */
    inline Eigen::VectorXf& position();
    
    /**
     \brief check whether the space object is visible or not
     \return true: object is visible, false: otherwise
     
     a visible object can be a neighbor for another object in the same space, an invisible can't
     */
    inline bool visible() const;
    
    /**
     \brief check whether this object can have neighbors
     \return true: object can have neighbors, false: object can't have neighbors
     */
    inline bool canHaveNeighbors() const;
    
    /**
     \brief returns true if no more neighbors can be added to neighbor list
     \exception Exception object can't have neighbors
     */
    inline bool neighborListFull() const throw (Exception);
    
    /**
     \brief return neighbor radius
     \return neighbor radius
     \exception Exception object can't have neighbors
     */
    inline float neighborRadius() const throw (Exception);
    
    /**
     \brief return maxmimum number of neighbors
     \return return maxmimum number of neighbors
     \exception Exception object can't have neighbors
     */
    inline int maxNeighborCount() const throw (Exception);
    
    /**
     \brief remove neighbors
     */
    inline void removeNeighbors() throw (Exception);
    
    /**
     \brief add neighbor object to neighbor list
     \param pNeighborObject neighbor space object
     \exception Exception neighbor object could not be added
     
     creates new neighbor relation
     */
    inline bool addNeighbor(SpaceObject* pNeighborObject) throw (Exception);
    
    /**
     \brief add neighbor object to neighbor list
     \param pNeighborObject neighbor space object
     \param pDistance distance
     \param pDirection direction
     \exception Exception neighbor object could not be added
     
     creates new neighbor relation
     */
    inline bool addNeighbor(SpaceObject* pNeighborObject, float pDistance, const Eigen::VectorXf& pDirection) throw (Exception);
    
    /**
     \brief add neighbor object to neighbor list
     \param pNeighborObject neighbor space object
     \param pValue value
     \param pDirection direction
     \param pDistance distance
     \exception Exception neighbor object could not be added
     
     creates new neighbor relation
     */
    inline bool addNeighbor(SpaceObject* pNeighborObject, const Eigen::VectorXf& pValue, const Eigen::VectorXf& pDirection, float pDistance) throw (Exception);
    
    /**
     \brief obtain textual space proxy object information
     */
    virtual operator std::string() const;
    
    /**
     \brief obtain textual space proxy object information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
	virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual space proxy object info
     \param pOstream output text stream
     \param pSpaceProxyObject space proxy object
     */
    friend std::ostream& operator << ( std::ostream& pOstream, const SpaceProxyObject& pSpaceProxyObject )
    {
        pOstream << std::string(pSpaceProxyObject);
        
        return pOstream;
    };
    
protected:
    SpaceProxyObject();
    
    SpaceObject* mSpaceObject;
    NeighborGroup* mNeighborGroup;
    
};

SpaceObject*
SpaceProxyObject::spaceObject()
{
    return mSpaceObject;
}

const SpaceObject*
SpaceProxyObject::spaceObject() const
{
    return mSpaceObject;
}

NeighborGroup*
SpaceProxyObject::neighborGroup()
{
    return mNeighborGroup;
}

const NeighborGroup*
SpaceProxyObject::neighborGroup() const
{
    return mNeighborGroup;
}

unsigned int
SpaceProxyObject::dim() const
{
    return mSpaceObject->mDim;
}

Eigen::VectorXf&
SpaceProxyObject::position()
{
    return mSpaceObject->mPosition;
}

bool
SpaceProxyObject::visible() const
{
    return mNeighborGroup->mVisible;
}

bool
SpaceProxyObject::canHaveNeighbors() const
{
    return (mNeighborGroup->mNeighborGroupAlg != nullptr && mNeighborGroup->canHaveNeighbors() ) ? true : false;
}

bool
SpaceProxyObject::neighborListFull() const throw (Exception)
{
    if(mNeighborGroup->mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object can't have neighbors", __FILE__, __FUNCTION__, __LINE__);
    return mNeighborGroup->mNeighborGroupAlg->full();
}

float
SpaceProxyObject::neighborRadius() const throw (Exception)
{
    if(mNeighborGroup->mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object can't have neighbors", __FILE__, __FUNCTION__, __LINE__);
    return mNeighborGroup->mNeighborGroupAlg->neighborRadius();
}

int
SpaceProxyObject::maxNeighborCount() const throw (Exception)
{
    if(mNeighborGroup->mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object can't have neighbors", __FILE__, __FUNCTION__, __LINE__);
    return mNeighborGroup->mNeighborGroupAlg->maxNeighborCount();
}

void 
SpaceProxyObject::removeNeighbors() throw (Exception)
{
    if(mNeighborGroup->mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object can't have neighbors", __FILE__, __FUNCTION__, __LINE__);
    mNeighborGroup->mNeighborGroupAlg->removeNeighbors();
}

bool
SpaceProxyObject::addNeighbor(SpaceObject* pNeighborObject) throw (Exception)
{
    if(mNeighborGroup->mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object can't have neighbors", __FILE__, __FUNCTION__, __LINE__);
    return mNeighborGroup->mNeighborGroupAlg->createNeighbor(mSpaceObject, pNeighborObject);
}

bool
SpaceProxyObject::addNeighbor(SpaceObject* pNeighborObject, float pDistance, const Eigen::VectorXf& pDirection) throw (Exception)
{
    if(mNeighborGroup->mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object can't have neighbors", __FILE__, __FUNCTION__, __LINE__);
    return mNeighborGroup->mNeighborGroupAlg->createNeighbor(mSpaceObject, pNeighborObject, pDistance, pDirection);	
}

bool 
SpaceProxyObject::addNeighbor(SpaceObject* pNeighborObject, const Eigen::VectorXf& pValue, const Eigen::VectorXf& pDirection, float pDistance) throw (Exception)
{
    if(mNeighborGroup->mNeighborGroupAlg == nullptr) throw Exception("SPACE ERROR: object can't have neighbors", __FILE__, __FUNCTION__, __LINE__);
    return mNeighborGroup->mNeighborGroupAlg->createNeighbor(mSpaceObject, pNeighborObject, pValue, pDirection, pDistance);	
}

};
    
};

#endif
