/** \file dab_space_neighbor_group.h
*/

#ifndef _dab_space_neighbor_group_h_
#define _dab_space_neighbor_group_h_

#include <iostream>
#include <list>
#include <Eigen/Dense>
#include "dab_exception.h"

namespace dab
{

namespace space
{

class Space;
class SpaceObject;
class SpaceNeighborRelation;
class NeighborGroupAlg;
    
class NeighborGroup
{
    
friend class SpaceProxyObject;
friend class NeighborGroupAlg;
    
public:
    /**
     \brief create neighbor collection
     \param pSpaceObject space object this neighborlist is part of
     \param pSpace space this neighborgroup is based on
     \param pVisible visiblity of space object
     */
    NeighborGroup(SpaceObject* pSpaceObject, Space* pSpace, bool pVisible);
    
    /**
     \brief create neighbor collection
     \param pSpaceObject space object this neighbor list is part of
     \param pSpace space this neighborgroup is based on
     \param pVisible visiblity of space object
     \param pNeighborGroupAlg neighbor group algorithm
     */
    NeighborGroup(SpaceObject* pSpaceObject, Space* pSpace, bool pVisible, NeighborGroupAlg* pNeighborGroupAlg);
    
    /**
     \brief destructor
     */
    virtual ~NeighborGroup();
    
    /**
     \brief return space object this neighborlist is associated with
     \return space object
     */
    SpaceObject* spaceObject();
    
    /**
     \brief return space object this neighborlist is associated with
     \return space object
     */
    const SpaceObject* spaceObject() const;
    
    /**
     \brief return space name
     \return space name
     */
    const std::string& name() const;
    
    /**
     \brief return space this neighborlist is based on
     \return space
     */
    Space* space();
    
    /**
     \brief check whether the space object is visible or not
     \return true: object is visible, false: otherwise
     
     a visible object can be a neighbor for another object in the same space, an invisible can't
     */
    bool visible() const;
    
    /**
     \brief set space object visibility
     \param pVisible object visibility
     */
    void setVisible(bool pVisible);
    
    /**
     \brief check whether this neighbor group can have neighbors
     \return true: neighbor group can have neighbors, false: object can't have neighbors
     */
    bool canHaveNeighbors() const;
    
    /**
     \brief return neighborhood algorithm
     \return neighborhood algorithm
     */
    NeighborGroupAlg* neighborGroupAlg();
    
    /**
     \brief set neigbhorhood algorithm
     \param pNeighborGroupAlg neighborhood algorithm
     */
    void setNeighborGroupAlg(NeighborGroupAlg* pNeighborGroupAlg);
    
    /**
     \brief return neighbor radius
     \return neighbor radius
     \exception Exception object can't have neighbors
     */
    float neighborRadius() const throw (Exception);
    
    /**
     \brief return number of neighbors
     \return number of neighbors
     */
    unsigned int neighborCount() const;
    
    /**
     \brief return maximum number of neighbors
     \return maximum number of neighbors
     \exception Exception object can't have neighbors
     */
    unsigned int maxNeighborCount() const throw (Exception);
    
    bool replaceNeighborMode() const throw (Exception);
    
    /**
     \brief returns true if no more neighbors can be added to neighbor list
     \exception Exception object can't have neighbors
     */
    bool full() const throw (Exception);
    
    /**
     \brief set neighbor radius
     \param pNeighborRadius neighbor radius
     \exception Exception object can't have neighbors
     */
    void setNeighborRadius(float pNeighborRadius) throw (Exception);
    
    /**
     \brief return neighboring space object
     \param pIndex neighbor index
     \return neighboring space object
     \exception Exception index is out of bounds
     */
    SpaceObject* neighbor(unsigned int pIndex) throw (Exception);
    
    /**
     \brief return neighboring space object
     \param pIndex neighbor index
     \return neighboring space object
     \exception Exception index is out of bounds
     */
    const SpaceObject* neighbor(unsigned int pIndex) const throw (Exception);
    
    /**
     \brief return value of neighbor relation
     \param pIndex neighbor index
     \returns value
     \exception Exception index is out of bounds
     */
    const Eigen::VectorXf& value(unsigned int pIndex) const throw (Exception);
    
    /**
     \brief return direction of neighbor relation
     \param pIndex neighbor index
     \returns direction
     \exception Exception index is out of bounds
     */
    const Eigen::VectorXf& direction(unsigned int pIndex) const throw (Exception);
    
    /**
     \brief return distance of neighbor relation
     \param pIndex neighbor index
     \return distance
     \exception Exception index is out of bounds
     */
    float distance(unsigned int pIndex) const throw (Exception);
    
    /**
     \brief return neighbor relations
     \return neighbor relations
     */
    std::vector<SpaceNeighborRelation*>& neighborRelations();
    
    /**
     \brief return neighbor relation
     \param pIndex neighbor index
     \return neighbor relation
     \exception Exception index is out of bounds
     */
    SpaceNeighborRelation* neighborRelation(unsigned int pIndex) throw (Exception);
    
    /**
     \brief return neighbor relation
     \param pIndex neighbor index
     \return neighbor relation
     \exception Exception index is out of bounds
     */
    const SpaceNeighborRelation* neighborRelation(unsigned int pIndex) const throw (Exception);
    
    /**
     \brief return neighbor relation
     \param pIndex neighbor index
     \return neighbor relation
     \exception Exception index is out of bounds
     */
    SpaceNeighborRelation* operator[](unsigned int pIndex) throw (Exception);
    
    /**
     \brief add neighbor object to neighbor list
     \param pNeighborObject neighbor space object
     \exception Exception neighbor object could not be added
     
     creates new neighbor relation
     */
    bool addNeighbor(SpaceObject* pNeighborObject) throw (Exception);
    
    /**
     \brief add neighbor object to neighbor list
     \param pNeighborObject neighbor space object
     \param pDistance distance
     \param pDirection direction
     \exception Exception neighbor object could not be added
     
     creates new neighbor relation
     */
    bool addNeighbor(SpaceObject* pNeighborObject, float pDistance, const Eigen::VectorXf& pDirection) throw (Exception);
    
    /**
     \brief remove neighbor
     \param pNeighborObject neighbor space object
     \exception Exception neighbor object could not be removed
     */
    void removeNeighbor(SpaceObject* pNeighborObject) throw (Exception);
    
    /**
     \brief remove neighbor
     \param pNeighborIndex index of neighbor space object
     \exception Exception neighbor object could not be removed
     */
    void removeNeighbor(unsigned int pNeighborIndex) throw (Exception);
    
    /**
     \brief remove neighbors
     \exception Exception neighbors could not be removed
     */
    void removeNeighbors() throw (Exception);
    
    /**
     \brief print neighbor collection information
     */
    virtual operator std::string() const;
    
    /**
     \brief print neighbor collection information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual neighbor group info
     \param pOstream output text stream
     \param pNeighborGroup neighbor group
     */
    friend std::ostream& operator << ( std::ostream& pOstream, const NeighborGroup& pNeighborGroup )
    {
        pOstream << std::string(pNeighborGroup);
        
        return pOstream;
    }; 

    
protected:
    /**
     \brief default constructor
     */
    NeighborGroup();
    
    /**
     \brief space object
     */
    SpaceObject* mSpaceObject;
    
    /**
     \brief space this neighborlist is based on
     */
    Space* mSpace;
    
    /**
     \brief visibility of space object in this space
     */
    bool mVisible;
    
    /**
     \brief neighborhood algorithm
     */
    NeighborGroupAlg* mNeighborGroupAlg;	
    
    /**
     \brief list of neighbor relations
     */
    std::vector<SpaceNeighborRelation*> mNeighborRelations;
};
    

};
    
};

#endif
