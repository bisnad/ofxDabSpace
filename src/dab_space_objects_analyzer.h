/** \file dab_space_objects_analyzer.h
 *  \class dab::space::SpaceObjectsAnalyzer abstract base class for analyzing space object distributions
 *  \brief abstract base class for analyzing space object distributions
 *
 *  Created by Daniel Bisig on 5/30/08.
 *  Ported to OpenFrameworks by Daniel Bisig on 08/09/17.
 */

#ifndef _iso_space_objects_analyzer_h_
#define _iso_space_objects_analyzer_h_

#include <vector>
#include <Eigen/Dense>
#include "ofTypes.h"
#include "dab_space_proxy_object.h"
#include "dab_index_map.h"

// WARNING: COMPLETELY UNTESTED!!!!!!

namespace dab
{

namespace space
{

class SpaceProxyObjectGroup
{
    
public:
    SpaceProxyObjectGroup();
    ~SpaceProxyObjectGroup();
    
    unsigned int objectCount() const;
    const SpaceProxyObject* proxyObject( unsigned int pIndex) const throw (Exception);
    const SpaceObject* spaceObject( unsigned int pIndex) const throw (Exception);
    const NeighborGroup* neighborGroup( unsigned int pIndex) const throw (Exception);
    void addObject(SpaceObject* pSpaceObject);
    void addObject(NeighborGroup* pNeighborGroup);
    void removeObject(SpaceObject* pSpaceObject);
    void removeObject(NeighborGroup* pNeighborGroup);
    void removeObjects();
    
protected:
    std::vector< SpaceProxyObject* > mSpaceObjects;
    ofMutex mLock;
};

class SpaceObjectsAnalyzer
{
public:
    /**
     \brief default constructor
     */
    SpaceObjectsAnalyzer();
    
    /**
     \brief parameter based constructor
     \param pName of space data analyzer
     */
    SpaceObjectsAnalyzer(std::string pName);
    
    /**
     \brief destructor
     */
    virtual ~SpaceObjectsAnalyzer();
    
    /**
     \brief return name
     \return name
     */
    const std::string& name() const;
    
    /**
     \brief check whether the analyzer is modifiable
     \return true if it is, false otherwise
     */
    bool modifiable() const;
    
    /**
     \brief check whether object group exists
     \param pGroupName name of object group
     \return true if it does, false otherwise
     */
    bool checkObjectGroup( const std::string& pGroupName ) const;
    
    /**
     \brief add space object group
     \param pGroupName name of object group
     */
    virtual void addObjectGroup( const std::string& pGroupName ) throw (Exception);
    
    /**
     \brief remove space object group
     \param pGroupIndex index of object group
     */
    virtual void removeObjectGroup( unsigned int pGroupIndex ) throw (Exception);
    
    /**
     \brief remove space object group
     \param pGroupName name of object group
     */
    virtual void removeObjectGroup( const std::string& pGroupName ) throw (Exception);
    
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
     \brief add space object
     \param pDataGroupIndex space object group index
     \param pNeighborGroup neighbor group
     */
    virtual void addObject( unsigned int pDataGroupIndex, const NeighborGroup* pNeighborGroup ) throw (Exception);
    
    /**
     \brief add space objects
     \param pDataGroupIndex space object group index
     \param pNeighborGroups vector of neighbor groups
     */
    virtual void addObjects( unsigned int pDataGroupIndex, std::vector< const NeighborGroup* >& pNeighborGroups ) throw (Exception);
    
    /**
     \brief add space object
     \param pGroupName space object group name
     \param pNeighborGroup neighbor group
     */
    virtual void addObject( const std::string& pGroupName, const NeighborGroup* pNeighborGroup ) throw (Exception);
    
    /**
     \brief add space objects
     \param pGroupName space object group name
     \param pNeighborGroups vector of neighbor groups
     */
    virtual void addObjects( const std::string& pGroupName, std::vector< const NeighborGroup* >& pNeighborGroups ) throw (Exception);
    
    /**
     \brief remove space object
     \param pGroupIndex space object group index
     \param pSpaceObject space object
     */
    void removeObject( unsigned int pGroupIndex, const SpaceObject* pSpaceObject ) throw (Exception);
    
    /**
     \brief remove space object
     \param pGroupName space object group name
     \param pSpaceObject space object
     */
    void removeObject( const std::string& pGroupName, const SpaceObject* pSpaceObject ) throw (Exception);
    
    /**
     \brief remove space object
     \param pGroupIndex space object group index
     \param pNeighborGroup neighbor group
     */
    void removeObject( unsigned int pGroupIndex, const NeighborGroup* pNeighborGroup ) throw (Exception);
    
    /**
     \brief remove space object
     \param pGroupName space object group name
     \param pNeighborGroup neighbor group
     */
    void removeObject( const std::string& pGroupName, const NeighborGroup* pNeighborGroup ) throw (Exception);
    
    /**
     \brief remove space objects
     \param pDataGroupIndex space object group index
     */
    void removeObjects( unsigned int pDataGroupIndex ) throw (Exception);
    
    /**
     \brief remove space objects
     \param pGroupName space object group name
     */
    void removeObjects( const std::string& pGroupName ) throw (Exception);
    
    /**
     \brief return analysis result
     \param pResultIndex result index
     \return result as vector
     */
    const Eigen::VectorXf& result( unsigned int pResultIndex ) const throw (Exception);
    
    /**
     \brief return analysis result
     \param pResultName result name
     \return result as vector
     */
    const Eigen::VectorXf& result( const std::string& pResultName ) const throw (Exception);
    
    /**
     \brief return analysis results
     \return QVector of all vector results
     */
    const IndexMap< std::string, Eigen::VectorXf >& results() const;
    
    /**
     \brief perform analysis
     */
    virtual void analyze();
    
protected:
    IndexMap< std::string, SpaceProxyObjectGroup* > mSpaceObjectGroups; ///\brief space data groups
    IndexMap< std::string, Eigen::VectorXf > mResults; ///\brief analysis results
    std::string mName; ///\brief name of analyzer
    
    bool mModifiable; ///\brief flag whether number of data groups can be modified or not
    ofMutex mLock;
};

};

};

#endif