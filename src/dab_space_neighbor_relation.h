/** \file dab_space_neighbor_relation.h
*/

#ifndef _dab_space_neighbor_relation_h_
#define _dab_space_neighbor_relation_h_

#include <iostream>
#include <Eigen/Dense>
#include "dab_exception.h"

namespace dab
{

namespace space
{

class SpaceObject;

class SpaceNeighborRelation
{
public:
    /**
     \brief create space neighbor relation
     \param pDim dimension of space neighbor (> 0)
     */
    SpaceNeighborRelation(unsigned int pDim);
    
    /**
     \brief create space neighbor relation (automated distance and direction calculation)
     \param pObject space object
     \param pNeighborObject neighboring space object
     \exception Exception either the two objects are identical are if they differ in their respective dimensions
     */
    SpaceNeighborRelation(SpaceObject* pObject, SpaceObject* pNeighborObject) throw (Exception);
    
    /**
     \brief create space neighbor relation (manually set distance and direction)
     \param pObject space object
     \param pNeighborObject neighboring space object
     \param pDistance distance
     \param pDirection direction
     \exception Exception either the two objects are identical are if they differ in their respective dimensions
     */
    SpaceNeighborRelation(SpaceObject* pObject, SpaceObject* pNeighborObject, float pDistance, const Eigen::VectorXf& pDirection) throw (Exception);
    
    /**
     \brief create space neighbor relation (manually set distance and direction)
     \param pObject space object
     \param pNeighborObject neighboring space object
     \param pValue value
     \param pDirection direction
     \param pDistance distance
     \exception Exception either the two objects are identical are if they differ in their respective dimensions
     */
    SpaceNeighborRelation(SpaceObject* pObject, SpaceObject* pNeighborObject, const Eigen::VectorXf& pValue, const Eigen::VectorXf& pDirection, float pDistance) throw (Exception);
    
    /**
     \brief destructor
     */
    ~SpaceNeighborRelation();
    
    /**
     \brief return neighbor
     */
    SpaceObject* neighbor();
    
    /**
     \brief return value of neighbor relation
     \returns value of neighbor relation
     */
    const Eigen::VectorXf& value() const;
    
    /**
     \brief return direction vector pointing to neighbor
     \returns direction vector pointing to neighbor
     */
    const Eigen::VectorXf& direction() const;
    
    /**
     \brief return distance to neighbor
     \returns distance to neighbor
     */
    float distance() const;
    
    /**
     \brief manually set neighbor space object
     \param pNeighborObject neighbor space object
     \exception Exception either the two objects are identical are if they differ in their respective dimensions
     */
    void setNeighbor(SpaceObject* pNeighborObject) throw (Exception);
    
    /**
     \brief manually set value
     \param pValue value
     \exception Exception value dimension mismatch
     */
    void setValue(const Eigen::VectorXf& pValue) throw (Exception);
    
    /**
     \brief manually set direction
     \param pDirection direction
     \exception Exception direction dimension mismatch
     */
    void setDirection(const Eigen::VectorXf& pDirection) throw (Exception);
    
    /**
     \brief manually set distance
     \param pDistance distance
     */
    void setDistance(float pDistance);
    
    /**
     \brief set new space object and neighbor object
     \param pObject space object
     \param pNeighborObject neighboring space object
     \exception Exception either the two objects are identical are if they differ in their respective dimensions
     */
    void set(SpaceObject* pObject, SpaceObject* pNeighborObject) throw (Exception);
    
    /**
     \brief print neighbor information
     */
    virtual operator std::string() const;
    
    /**
     \brief print neighbor information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual neighbor info
     \param pOstream output text stream
     \param pNeighbor neighbor
     */
    friend std::ostream& operator << ( std::ostream& pOstream, const SpaceNeighborRelation& pNeighborRelation )
    {
        pOstream << std::string(pNeighborRelation);
        
        return pOstream;
    };
    
protected:
    /**
     \brief default constructor
     */
    SpaceNeighborRelation();
    
    /**
     \brief space object
     */
    SpaceObject* mObject;
    
    /**
     \brief neighboring space object
     */
    SpaceObject* mNeighborObject;
    
    /**
     \brief value of neighbor relation
     
     usually the same value as mDirection
     */
    Eigen::VectorXf mValue;
    
    /**
     \brief direction of neighbor relation
     
     usually vector pointing from position of mObject to position of mNeighborObject
     */
    Eigen::VectorXf mDirection;
    
    /**
     \brief distance of neighbor relation
     
     usually length of mDirection
     */
    float mDistance;
};

};
    
};

#endif
