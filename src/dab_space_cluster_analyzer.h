/** \file dab_space_cluster_analyzer.h
 *  \class dan::space::SpaceClusterAnalyzer detects clusters among space objects
 *  \brief detects clusters among space objects
 *
 *  Created by Daniel Bisig on 5/30/08.
 *  Ported to OpenFrameworks by Daniel Bisig on 09/08/17.
 */

#include "dab_space_objects_analyzer.h"

// WARNING: COMPLETELY UNTESTED!!!!!!

namespace dab
{

namespace space
{

class SpaceClusterAnalyzer : public SpaceObjectsAnalyzer
{
public:
    /**
     \brief default constructor
     */
    SpaceClusterAnalyzer();
    
    /**
     \brief parameter based constructor
     \param pName of space data analyzer
     */
    SpaceClusterAnalyzer(std::string pName);
    
    /**
     \brief destructor
     */
    ~SpaceClusterAnalyzer();
    
    /**
     \brief add space object group
     \param pGroupName name of space object group
     */
    virtual void addObjectGroup( const std::string& pGroupName ) throw (Exception);
    
    /**
     \brief add space object
     \param pGroupIndex space object group index
     \param pSpaceObject space object
     */
    virtual void addObject( unsigned int pGroupIndex, const SpaceObject* pSpaceObject ) throw (Exception);
    
    /**
     \brief add space objects
     \param pGroupIndex space object group index
     \param pSpaceObjects vector of space objects
     */
    virtual void addObjects( unsigned int pGroupIndex, std::vector< const SpaceObject* >& pSpaceObjects ) throw (Exception);
    
    /**
     \brief add space object
     \param pGroupName space object group name
     \param pSpaceObject space objects
     */
    virtual void addObject( const std::string& pGroupName, const SpaceObject* pSpaceObject ) throw (Exception);
    
    /**
     \brief add space objects
     \param pGroupName space object group name
     \param pSpaceObjects vector of space objects
     */
    virtual void addObjects( const std::string& pGroupName, std::vector< const SpaceObject* >& pSpaceObjects ) throw (Exception);
    
    /**
     \brief perform analysis
     */
    virtual void analyze();
    
protected:
    
};

};

}; 

