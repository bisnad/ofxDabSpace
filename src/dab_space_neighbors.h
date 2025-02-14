/** \file dab_space_neighbors.h
*/

#ifndef _dab_space_neighbors_h_
#define _dab_space_neighbors_h_

#include <iostream>
#include <vector>
#include <map>
#include <Eigen/Dense>
#include "dab_index_map.h"
#include "dab_exception.h"
//#include "dab_space_neighbor_group.h"

namespace dab
{
    
namespace space
{
    
class SpaceObject;
class SpaceNeighborRelation;
class NeighborGroup;
class NeighborGroupAlg;

class Neighbors
{
    
public:
    /**
     \brief create neighbor collection
     \param pSpaceObject space object this neighbor collection is part of
     */
    Neighbors(SpaceObject* pSpaceObject);
    
    /**
     \brief destructor
     */
    ~Neighbors();
    
    /**
     \brief return number of neighbor groups
     \return number of neighbor groups
     */
    unsigned int neighborGroupCount() const;
    
    /**
     \brief check whether neighbor collection for space exists
     \param pSpaceName space name
     \return true, if it exists, false otherwise
     */
    bool checkNeighborGroup(const std::string& pSpaceName);
    
    /**
     \brief return neighbor group index
     \param pSpaceName space name
     \return neighbor group index
     \exception Exception if space object is not in particular space or can have neighbors in that particular space
     */
     unsigned int neighborGroupIndex(const std::string& pSpaceName) throw (Exception);
    
    /**
     \brief return neighbor collection based on index
     \param pIndex index neighbor collection
     \return neighbor collection
     \exception Exception if index is out of bounds
     */
    NeighborGroup* neighborGroup(unsigned int pIndex) throw (Exception);
    
    /**
     \brief return neighbor collection for a particular space
     \param pSpaceName space name
     \return neighbor collection
     \exception Exception space object is not in particular space
     */
    NeighborGroup* neighborGroup(const std::string& pSpaceName) throw (Exception);
    
    /**
     \brief add new neighbor group
     \param pNeighborGroup new neighbor group
     \exception Exception neighbor group for the associated space already exists
     */
    void addNeighborGroup(NeighborGroup* pNeighborGroup) throw (Exception);
    
    /**
     \brief remove neighbor group
     \param pNeighborGroup  neighbor group to remove
     \exception Exception neighbor group for the associated does not exist
     */
    void removeNeighborGroup(NeighborGroup* pNeighborGroup) throw (Exception);
    
    /**
     \brief check whether this object can have neighbors in a particular space
     \param pSpaceName space name
     \return true: object can have neighbors, false: object can't have neighbors
     \exception Exception object is not in particular space
     */
    bool canHaveNeighbors(const std::string& pSpaceName) const throw (Exception);
    
    /**
     \brief check whether this object is visible or not
     \param pSpaceName name of space
     \return true: object is visible, false: otherwise
     \exception Exception object is not in particular space
     
     a visible object can be a neighbor for another object in the same space, an invisible can't
     */
    bool visible(const std::string& pSpaceName) const throw (Exception);
    
    /**
     \brief set object visibility in all spaces
     \param pVisible object visibility
     */
    void setVisible(bool pVisible);
    
    /**
     \brief set object visibility in particular space
     \param pSpaceName name of space
     \param pVisible object visibility
     \exception Exception object is not in particular space
     */
    void setVisible(const std::string& pSpaceName, bool pVisible) throw (Exception);
    
    /**
     \brief return neighbor radius
     \return neighbor radius
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space
     */
    float neighborRadius(const std::string& pSpaceName) const throw (Exception);
    
    bool replaceNeighborMode(const std::string& pSpaceName) const throw (Exception);
    
    /**
     \brief return maximum number of neighbors
     \param pSpaceName space name
     \return maximum number of neighbors
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space
     */
    unsigned int maxNeighborCount(const std::string& pSpaceName) const throw (Exception);
    
    /**
     \brief return number of neighbors
     \param pSpaceName space name
     \return number of neighbors
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space
     */
    unsigned int neighborCount(const std::string& pSpaceName) const throw (Exception);
    
    /**
     \brief returns true if no more neighbors can be added to neighbor list
     \param pSpaceName space name
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space
     */
    bool full(const std::string& pSpaceName) const throw (Exception);
    
    /**
     \brief return neighboring space object
     \param pSpaceName space name
     \param pIndex neighbor index
     \return neighboring space object
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space or if index is out of bounds
     */
    SpaceObject* neighbor(const std::string& pSpaceName, unsigned int pIndex) throw (Exception);
    
    /**
     \brief return direction vector pointing to neighboring space object
     \param pSpaceName space name
     \param pIndex neighbor index
     \returns direction
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space or if index is out of bounds
     */
    const Eigen::VectorXf& direction(const std::string& pSpaceName, unsigned int pIndex) const throw (Exception);
    
    /**
     \brief return distance to neighboring space object
     \param pSpaceName space name
     \param pIndex neighbor index
     \return distance
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space or if index is out of bounds
     */
    float distance(const std::string& pSpaceName, unsigned int pIndex) const throw (Exception);
    
    /**
     \brief return neighbor relations
     \param pSpaceName space name
     \return neighbor relations
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space
     */
    std::vector<SpaceNeighborRelation*>& neighborRelations(const std::string& pSpaceName) throw (Exception);
    
    /**
     \brief return neighbor relation
     \param pSpaceName space name
     \param pIndex neighbor index
     \return neighbor relation
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space or if index is out of bounds
     */
    SpaceNeighborRelation* neighborRelation(const std::string& pSpaceName, unsigned int pIndex) throw (Exception);
    
    /**
     \brief add neighbor object to neighbor list
     \param pSpaceName space name
     \param pNeighborObject neighbor space object
     \return true, if neighbor object could be added, false otherwise
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space
     
     creates new neighbor relation
     */
    bool addNeighbor(const std::string& pSpaceName, SpaceObject* pNeighborObject) throw (Exception);
    
    /**
     \brief add neighbor object to neighbor list
     \param pSpaceName space name
     \param pNeighborObject neighbor space object
     \param pDistance distance
     \param pDirection direction
     \return true, if neighbor object could be added, false otherwise
     \exception Exception if space object is not in particular space or cannot have neighbors in that particular space
     
     creates new neighbor relation
     */
    bool addNeighbor(const std::string& pSpaceName, SpaceObject* pNeighborObject, float pDistance, const Eigen::VectorXf& pDirection) throw (Exception);
    
    /**
     \brief remove all neighbors from all spaces
     */
    void removeNeighbors();
    
    /**
     \brief remove all neighbors from a single space
     \exception Exception space object is not in particular space or cannot have neighbors in that particular space
     */
    void removeNeighbors(const std::string& pSpaceName) throw (Exception);
    
    /**
     \brief print neighbor group information
     */
    virtual operator std::string() const;
    
    /**
     \brief print neighbor group information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
	virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual neighbor group info
     \param pOstream output text stream
     \param pNeighborGroup neighbor group
     */
    friend std::ostream& operator << ( std::ostream& pOstream, const Neighbors& pNeighbors )
    {
        pOstream << std::string(pNeighbors);
        
        return pOstream;
    };
    
protected:
    /**
     \brief default constructor
     */
    Neighbors();
    
    /**
     \brief space object this neighborlists is part of
     */
    SpaceObject* mSpaceObject;
    
    /**
     \brief storage for single space neighbor collections
     */
    IndexMap<std::string, NeighborGroup*> mNeighborGroups;
};
    
};
    
};

#endif
