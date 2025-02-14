/** \file dab_space.h
*/

#ifndef _dab_space_h_
#define _dab_space_h_

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "dab_exception.h"

namespace dab
{

namespace space
{
    
class SpaceAlg;
class NeighborGroupAlg;
class SpaceObject;
class SpaceProxyObject;
    
class Space
{
    
public:
    /**
     \brief create space
     \param pName space name
     \param pSpaceAlg space alorithm
     */
    Space( const std::string& pName, SpaceAlg* pSpaceAlg );
    
    /**
     \brief destructor
     */
    ~Space();
    
    /**
     \brief return space name
     \return space name
     */
    const std::string& name() const;
    
    /**
     \brief return space dimension
     \return space dimension
     */
    unsigned int dim() const;
    
    /**
     \brief return space minimum position
     \return space minimum position
     */
    const Eigen::VectorXf& minPos() const;
    
    /**
     \brief return space maximum position
     \return space maximum position
     */
    const Eigen::VectorXf& maxPos() const;
    
    /**
     \brief return space alg
     \return spaceAlg
     */
    SpaceAlg* spaceAlg();
    
    /**
     \brief check if space contains space object
     \param pSpaceObject space object
     \return true if space contains space object, false otherwise
     */
    bool checkObject(SpaceObject* pSpaceObject) const;
    
    /**
     \brief return space proxy object for space object
     \param pSpaceObject space object
     \return space proxy object
     \exception Exception fails if no space proxy object is found
     */
    SpaceProxyObject* object(SpaceObject* pSpaceObject) throw (Exception);
    
    /**
     \brief return space all space objects
     \return all space objects
     */
    std::vector<SpaceProxyObject*>& objects();
    
    /**
     \brief add space object to space
     \param pObject space object
     \param pVisible visibility in neighbor space
     \param pNeighborGroupAlg neighbor group algorithm
     \exception Exception if object dimension of space object and space don't match
     
     automatically creates or updates the space objects's neighborGroup for this space
     */
    virtual void addObject(SpaceObject* pObject, bool pVisible, NeighborGroupAlg* pNeighborGroupAlg = NULL) throw (Exception);
    
    /**
     \brief update space object
     \param pObject space object
     \param pVisible visibility in neighbor space
     \param pNeighborGroupAlg neighbor group algorithm
     \exception Exception object not stored in space
     
     automatically creates or updates the space objects's neighborGroup for this space
     */
    virtual void setObject(SpaceObject* pObject, bool pVisible, NeighborGroupAlg* pNeighborGroupAlg = NULL) throw (Exception);
    
    /**
     \brief remove single space object handle object
     \param pObject single space object handle to remove
     \exception Exception fails if object is not stored in this space
     */
    virtual void removeObject(SpaceObject* pObject) throw (Exception);

     /**
     \brief remove all space object handles
     */
    void removeObjects() throw (Exception);
    
    /**
     \brief update space
     \exception Exception failed to update space
     */
    void update() throw (Exception);
    
    /**
     \brief print space information
     */
    operator std::string() const;
    
    /**
     \brief print space information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual space info
     \param pOstream output text stream
     \param pSpace space
     */
    friend std::ostream& operator << ( std::ostream& pOstream, const Space& pSpace )
    {
        pOstream << std::string(pSpace);
        
        return pOstream;
    };
    
protected:
    void updateObjects() throw(Exception);
    
    std::string mName; ///\brief name of space
    
    SpaceAlg* mSpaceAlg; ///\brief space algorithm
    std::vector<SpaceProxyObject*> mObjects; ///\brief all space proxy objects in this space
    std::vector<SpaceProxyObject*> mVisibleObjects; ///\brief visible space proxy objects
    std::vector<SpaceProxyObject*> mNeighborObjects; ///\brief space proxy objects that can possess neighbors
    
    bool mLock;
};

    
};
    
};

#endif
