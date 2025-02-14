/** \file dab_space_neighbor_group_alg.h
*/

#ifndef _dab_space_neighbor_group_alg_h_
#define _dab_space_neighbor_group_alg_h_

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "dab_exception.h"

namespace dab
{

namespace space
{

class SpaceObject;
class SpaceNeighborRelation;
class NeighborGroup;

class NeighborGroupAlg
{
public:
    /**
     \brief create neighbor algorithm
     */
    NeighborGroupAlg();
    
    /**
     \brief create neighbor algorithm
     \param pNeighborRadius radius within which neighbors are searched
     \param pMaxNeighborCount maximum number of neighbors in neighbor list if >= 0 (-1: no limit)
     \param pReplaceNeighborMode replace more distant neighbors with closer neighbors if true
     */
    NeighborGroupAlg(float pNeighborRadius, int pMaxNeighborCount, bool pReplaceNeighborMode);
    
    /**
     \brief copy constructor
     \param pNeighborGroupAlg neighbor algorithm to copy
     */
    NeighborGroupAlg(NeighborGroupAlg& pNeighborGroupAlg);
    
    /**
     \brief destructor
     */
    virtual ~NeighborGroupAlg();
    
    /**
     \brief pass pointer to neigbhbor group
     \param pNeighborGroup neighbor group
     */
    void setNeighborGroup(NeighborGroup* pNeighborGroup);
    
    /**
     \brief returns neighbor search radius
     \return neighbor search radius
     */
    float neighborRadius() const;
    
    /**
     \brief return maximum number of neighbors
     \return maximum number of neighbors
     */
    int maxNeighborCount() const;
    
    /**
     \brief return neighbor replacement mode
     \return neighbor replacement mode
     */
    bool replaceNeighborMode() const;
    
    /**
     \brief check whether more neighbors can be stored
     */
    virtual bool full() const;
    
    /**
     \brief set neighbor radius
     \param pNeighborRadius neighbor radius
     */
    void setNeighborRadius(float pNeighborRadius);

	/**
	\brief set maximum number of neighbors
	\param pMaxNeighborCount maximum number of neighbors
	*/
	void setMaxNeighborCount(int pMaxNeighborCount);
    
    /**
     \brief add Neighbor
     \param pObject1 the space object a neighbor will be added to
     \param pObject2 the neighbor space object
     \return whether neighbor has been created or not
     */
    virtual bool createNeighbor(SpaceObject* pObject1, SpaceObject* pObject2);
    
    /**
     \brief add Neighbor
     \param pObject1 the space object a neighbor will be added to
     \param pObject2 the neighbor space object
     \param pDistance distance
     \param pDirection direction
     \return whether neighbor has been created or not
     */
    virtual bool createNeighbor(SpaceObject* pObject1, SpaceObject* pObject2, float pDistance, const Eigen::VectorXf& pDirection);
    
    /**
     \brief add Neighbor
     \param pObject1 the space object a neighbor will be added to
     \param pObject2 the neighbor space object
     \param pValue value
     \param pDirection direction
     \param pDistance distance
     \return whether neighbor has been created or not
     */
    virtual bool createNeighbor(SpaceObject* pObject1, SpaceObject* pObject2, const Eigen::VectorXf& pValue, const Eigen::VectorXf& pDirection, float pDistance);
    
    /**
     \brief remove neighbor
     \param pNeighborObject neighbor space object
     */
    virtual void removeNeighbor(SpaceObject* pNeighborObject);
    
    /**
     \brief remove neighbor
     \param pNeighborIndex index of neighbor space object
     */
    virtual void removeNeighbor(unsigned int pNeighborIndex);
    
    /**
     \brief removes all neighbors from list
     */
    virtual void removeNeighbors();
    
    /**
     \brief print neighbor algorithm information
     */
    operator std::string() const;
    
    /**
     \brief print neighbor algorithm information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual neighbor algorithm info
     \param pOstream output text stream
     \param pNeighborGroupAlg neighbor group algorithm
     */
    friend std::ostream& operator << ( std::ostream& pOstream, const NeighborGroupAlg& pNeighborGroupAlg )
    {
        pOstream << std::string(pNeighborGroupAlg);
        
        return pOstream;
    };
    
protected:
    /**
     \brief default search radius for finding neighbors
     */
    static float sNeighborRadius;
    
    /**
     \brief default maximum number of neighbors in neighbor list
     */
    static int sMaxNeighborCount;
    
    /**
     \brief default replace more distant neighbors with closer neighbors
     */
    static bool sReplaceNeighborMode;
    
    /**
     \brief search radius for finding neighbors
     */
    float mNeighborRadius;
    
    /**
     \brief maximum number of neighbors in neighbor list
     */
    int mMaxNeighborCount;
    
    /**
     \brief replace more distant neighbors with closer neighbors
     */
    bool mReplaceNeighborMode;
    
    /**
     \brief pointer to space neighbors
     */
    NeighborGroup* mNeighborGroup;
    
    Eigen::VectorXf mNeighborDirection; ///\brief neighbor direction helper variable
};

};

};


#endif
