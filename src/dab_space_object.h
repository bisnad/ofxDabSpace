/** \file dab_space_object.h
*/

#ifndef _dab_space_object_h_
#define _dab_space_object_h_

#include <iostream>
#include <Eigen/Dense>
#include "dab_exception.h"

namespace dab
{

namespace space
{

class Space;
class Neighbors;
class NeighborGroup;
class NeighborGroupAlg;
class SpaceNeighborRelation;
    
class SpaceObject
{
public:
    friend class SpaceProxyObject;
    
	/**
     \brief create space object
     \param pDim dimension of parameter (> 0)
     */
	SpaceObject(unsigned int pDim);
    
	/**
     \brief create space object
     \param pPosition position of object
     */
	SpaceObject(const Eigen::VectorXf& pPosition);
    
	/**
     \brief copy constructor
     \param pSpaceObject space object to copy
     */
	SpaceObject(const SpaceObject& pSpaceObject);
	
	/**
     \brief destructor
     */
	virtual ~SpaceObject();
	
	/**
     \brief return space object id
     \return space object id
     */
	unsigned int id() const;
    
	/**
     \brief return space object dimension
     \return space object dimension
     */
	unsigned int dim() const;
	
	/**
     \brief return space object position
     \return space object position
     */
	Eigen::VectorXf& position();
    
	/**
     \brief return space object position
     \return space object position
     */
	const Eigen::VectorXf& position() const;
	
	/**
     \brief set space object position
     \param pPosition space object position
     \exception Exception if position has wrong dimension
     */
	virtual void setPosition(const Eigen::VectorXf& pPosition) throw (Exception);
    
	/**
     \brief change space object position
     \param pPosition space object position offset
     \exception Exception if  position has wrong dimension
     */
	virtual void changePosition(const Eigen::VectorXf& pPosition) throw (Exception);
    
	/**
     \brief check whether space object has neighor group for particular space
     \param pSpaceName space name
     \return true if it has neighbor group for particular space, false otherwise
     */
	bool checkNeighborGroup(const std::string& pSpaceName) const;
    
	/**
     \brief add new neighbor group
     \param pNeighborGroup new neighbor group
     \exception Exception failed to add neighbor group
     */
	void addNeighborGroup(NeighborGroup* pNeighborGroup) throw (Exception);
    
	/**
     \brief remove neighbor group
     \param pNeighborGroup  neighbor group to remove
     \exception Exception failed to remove neighbor group
     */
	void removeNeighborGroup(NeighborGroup* pNeighborGroup) throw (Exception);
    
	/**
     \brief check whether this object is visible in a particular space or not
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
     \brief set object visibility in a particular space
     \param pSpaceName name of space
     \param pVisible object visibility
     \exception Exception object is not in particular space
     */
	void setVisible(const std::string& pSpaceName, bool pVisible) throw (Exception);
    
	/**
     \brief return neighbors
     \return neighbors
     */
	Neighbors* neighbors();
    
	/**
     \brief return neighbor group index
     \param pSpaceName space name
     \return neighbor group index
     \exception Exception space object is not in particular space or can't have neighbors in that particular space
     */
	unsigned int neighborGroupIndex(const std::string& pSpaceName) throw (Exception);
    
	/**
     \brief return neighbor group based on index
     \param pIndex neighbor group index
     \return neighbor group
     \exception Exception index is out of bounds
     */
	NeighborGroup* neighborGroup(unsigned int pIndex) throw (Exception);
    
	/**
     \brief return neighbor group
     \param pSpaceName space name
     \return neighbor group
     \exception Exception space object is not in particular space or can have neighbors in that particular space
     */
	NeighborGroup* neighborGroup(const std::string& pSpaceName) throw (Exception);
    
	/**
     \brief return neighbor space
     \param pSpaceName space name
     \return neighbor space
     \exception Exception space object is not in particular space
     */
	Space* neighborSpace(const std::string& pSpaceName) throw (Exception);
    
	/**
     \brief check whether this object can have neighbors in a particular space
     \param pSpaceName space name
     \return true: object can have neighbors, false: object can't have neighbors
     \exception Exception object is not in particular space
     */
	bool canHaveNeighbors(const std::string& pSpaceName) const throw (Exception);
    
	/**
     \brief return number maximum of neighbors
     \param pSpaceName space name
     \return maximum number of neighbors
     \exception Exception object is not in particular space
     */
	unsigned int maxNeighborCount(const std::string& pSpaceName) const throw (Exception);
    
	/**
     \brief return number of neighbors
     \param pSpaceName space name
     \return number of neighbors
     \exception Exception object is not in particular space
     */
	unsigned int neighborCount(const std::string& pSpaceName) const throw (Exception);
    
	/**
     \brief return neighbor radius
     \param pSpaceName space name
     \return neighbor radius
     \exception Exception object is not in particular space
     */
	float neighborRadius(const std::string& pSpaceName) const throw (Exception);
    
	bool replaceNeighborMode(const std::string& pSpaceName) const throw (Exception);
	
	/**
     \brief return neighboring object
     \param pSpaceName space name
     \param pIndex neighboring object index
     \return object
     \exception Exception space object is not in particular space or if this object cannot have any neighbors or if pIndex is out of bounds
     */
	virtual SpaceObject* neighbor(const std::string& pSpaceName, unsigned int pIndex) throw (Exception);
    
	/**
     \brief return neighbor relation
     \param pSpaceName space name
     \param pIndex neighbor index
     \return neighbor relation
     \exception Exception space object is not in particular space or if this object cannot have any neighbors or if pIndex is out of bounds
     */
	SpaceNeighborRelation* neighborRelation(const std::string& pSpaceName, unsigned int pIndex) throw (Exception);
	
	/**
     \brief returns true if no more neighbors can be added to neighbor list
     \param pSpaceName space name
     \exception Exception space object is not in particular space
     */
	bool neighborListFull(const std::string& pSpaceName) const throw (Exception);
	
	/**
     \brief add neighbor object to neighbor list
     \param pSpaceName space name
     \param pNeighborObject neighbor object
     \return true, if neighbor object could be added, false otherwise
     \exception Exception space object is not in particular space or cannot have neighbors in that particular space
     */
	bool addNeighbor(const std::string& pSpaceName, SpaceObject* pNeighborObject) throw (Exception);
    
	/**
     \brief add neighbor object to neighbor list
     \param pSpaceName space name
     \param pNeighborObject neighbor object
     \param pDistance distance
     \param pDirection direction
     \return true, if neighbor object could be added, false otherwise
     \exception Exception space object is not in particular space or cannot have neighbors in that particular space
     */
	bool addNeighbor(const std::string& pSpaceName, SpaceObject* pNeighborObject, float pDistance, const Eigen::VectorXf& pDirection) throw (Exception);
	
	/**
     \brief remove all neighbors in all spaces
     */
	void removeNeighbors();
    
	/**
     \brief remove all neighbors in particular space
     \param pSpaceName space name
     \exception Exception space object is not in particular space
     */
	void removeNeighbors(const std::string& pSpaceName) throw (Exception);
    
 	/**
     \brief obtain textual space object information
     */
	virtual operator std::string () const;
    
    /**
     \brief print neighbor information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
	/**
     \brief retrieve textual space object info
     \param pOstream output text stream
     \param pSpaceObject space object
     */
 	friend std::ostream& operator << ( std::ostream& pOstream, const SpaceObject& pSpaceObject )
    {
		pOstream << std::string(pSpaceObject);
        
        return pOstream;
    }; 
	
protected:
	static unsigned int sObjectCount; ///\brief space object count 
    
	/**
     \brief default constructor
     */
	SpaceObject();
    
	unsigned int mId; /// \brief unique id
 	unsigned int mDim; ///\brief dimension
 	Eigen::VectorXf mPosition; ///\brief position
	Neighbors* mNeighbors; ///\brief neighbors
};
    
};
    
};

#endif
