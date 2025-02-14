/**	\file dab_space_manager.h
 *	\class dab::space::SpaceManager space manager
 *	\brief space manager
 *
 *  handles all agent parameters that need neighborhood calculation
 *
 *  Created by Daniel Bisig on 3/29/07.
 *  Ported to Open Frameworks by Daniel Bisig on 3/08/17.
 */

#ifndef _dab_space_manager_h_
#define _dab_space_manager_h_

#include <vector>
#include <memory>
#include "dab_space.h"
#include "dab_index_map.h"
#include "dab_exception.h"
#include "dab_singleton.h"

namespace dab
{
    
namespace space
{
    
class SpaceObject;
class SpaceShape;

class SpaceManager : public Singleton<SpaceManager>
{
friend class Singleton<SpaceManager>;
    
public:
    /**
     \brief return space names
     \return space names
     */
    const std::vector< std::string >& spaceNames() const;
    
    /**
     \brief return space
     \param pSpaceName space nme
     \exception Exception space doesn't exist
     */
    std::shared_ptr<Space> space(const std::string& pSpaceName) throw (Exception);
    
    /**
     \brief check if space exists
     \param pSpaceName space name
     \return true if space exists, false otherwise
     */
    bool checkSpace(const std::string& pSpaceName);
    
    /**
     \brief add space to space manager
     \param pSpace space
     \exception Exception space already exists
     */
    void addSpace(std::shared_ptr<Space> pSpace) throw (Exception);
    
    /**
     \brief remove space
     \param pSpaceName name of space
     \exception Exception space does not exist
     */
    void removeSpace(const std::string& pSpaceName) throw (Exception);
    
    /**
     \brief remove all spaces
     */
    void removeSpaces();
    
    /**
     \brief add space object to space
     \param pSpaceName name of space to add space object to
     \param pObject space object
     \param pVisible visibility in neighbor space
     \param pNeighborGroupAlg neighbor group algorithm
     \exception Exception space not found or space dimension and space object dimension don't match or space is of wrong type
     */
    virtual void addObject(const std::string& pSpaceName, SpaceObject* pObject, bool pVisible, NeighborGroupAlg* pNeighborGroupAlg = nullptr) throw (Exception);
    
    /**
     \brief remove single space object handle object
     \param pSpaceName name of space to add space object to
     \param pObject single space object handle to remove
     \exception Exception space and or space object not found or space of wrong type
     */
    virtual void removeObject(const std::string& pSpaceName, SpaceObject* pObject) throw (Exception);
    
    /**
     \brief update all parameters
     \exception Exception failed to update spaces
     update parameter spaces
     */
    void update() throw (Exception);
    
    /**
     \brief print space manager information
     */
    virtual operator std::string() const;
    
    /**
     \brief print space manager information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual space manager info
     \param pOstream output text stream
     \param pSpaceManager space manager
     */
    friend std::ostream& operator << ( std::ostream& pOstream, const SpaceManager& pSpaceManager )
    {
        pOstream << pSpaceManager.info();
        
        return pOstream;
    };
    
protected:
    
    /**
     \brief default constructor
     */
    SpaceManager();
    
    /**
     \brief destructor
     */
    ~SpaceManager();
    
    /**
     \brief spaces
     */
    IndexMap<std::string, std::shared_ptr<Space> > mSpaces;
};
    
};
    
};

#endif

