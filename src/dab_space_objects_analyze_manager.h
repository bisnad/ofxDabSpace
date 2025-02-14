/** \file dab_space_objects_analyze_manager.h
 *  \class dab::space::SpaceObjectsAnalyzeManager space objects analyze manager
 *  \brief space objects analyze manager
 *
 *  Created by Daniel Bisig on 6/4/08.
 *  Ported to OpenFrameworks by Daniel Bisig on 09/08/17.
 */

#ifndef _dab_space_objects_analyze_manager_h_
#define _dab_space_objects_analyze_manager_h_

#include "dab_space_objects_analyzer.h"
#include "dab_index_map.h"
#include "dab_exception.h"
#include "dab_singleton.h"

// WARNING: COMPLETELY UNTESTED!!!!!!

namespace dab
{

namespace space
{

class SpaceObjectsAnalyzeManager : public Singleton<SpaceObjectsAnalyzeManager>
{
public:
    /**
     \brief check if space objects analyzer exists
     \param pAnalyzerName space objects analyzer name
     \return true if space objects analyzer exists, false otherwise
     */
    bool checkAnalyzer(const std::string& pAnalyzerName);
    
    /**
     \brief return space objects analyzer
     \param pAnalyzerName space objects analyzer name
     \exception Exception if space object analyzer doesn't exist
     */
    SpaceObjectsAnalyzer* analyzer(const std::string& pAnalyzerName) throw (Exception);
    
    /**
     \brief add space objects analyzer to space objects analyzer manager
     \param pAnalyzer space objects analyzer
     \exception Exception if space objects analyzer already exists
     */
    void addAnalyzer(SpaceObjectsAnalyzer* pAnalyzer) throw (Exception);
    
    /**
     \brief remove space objects analyzer
     \param pAnalyzerName name of space objects analyzer
     \exception Exception if space objects analyzer does not exist
     */
    void removeAnalyzer(const std::string& pAnalyzerName) throw (Exception);
    
    /**
     \brief add space object group
     \param pAnalyzerName name of space objects analyzer
     \param pGroupName space object group name
     \exception Exception if space objects analyzer does not exist
     */
    void addObjectGroup( const std::string& pAnalyzerName, const std::string& pGroupName ) throw (Exception);
    
    /**
     \brief remove space object group
     \param pAnalyzerName name of space objects analyzer
     \param pGroupIndex space object group index
     \exception Exception if space objects analyzer does not exist
     */
    void removeObjectGroup( const std::string& pAnalyzerName, unsigned int pGroupIndex ) throw (Exception);
    
    /**
     \brief remove space object group
     \param pAnalyzerName name of space objects analyzer
     \param pGroupName space object group name
     \exception Exception if space object analyzer does not exist
     */
    void removeObjectGroup( const std::string& pAnalyzerName, const std::string& pGroupName ) throw (Exception);
    
    /**
     \brief add space object
     \param pAnalyzerName name of space objects analyzer
     \param pGroupIndex space object group index
     \param pSpaceObject space object
     \exception Exception if space objects analyzer does not exist
     */
    void addObject( const std::string& pAnalyzerName, unsigned int pGroupIndex, const SpaceObject* pSpaceObject ) throw (Exception);
    
    /**
     \brief add space object
     \param pAnalyzerName name of space objects analyzer
     \param pGroupName space object group name
     \param pSpaceObject space object
     \exception Exception if space objects analyzer does not exist
     */
    void addObject( const std::string& pAnalyzerName, const std::string& pGroupName, const SpaceObject* pSpaceObject ) throw (Exception);
    
    /**
     \brief add space object
     \param pAnalyzerName name of space objects analyzer
     \param pGroupIndex space object group index
     \param pNeighborGroup neighbor group
     \exception Exception if space objects analyzer does not exist
     */
    void addObject( const std::string& pAnalyzerName, unsigned int pGroupIndex, const NeighborGroup* pNeighborGroup ) throw (Exception);
    
    /**
     \brief add space object
     \param pAnalyzerName name of space objects analyzer
     \param pGroupName space object group name
     \param pNeighborGroup neighbor group
     \exception Exception if space objects analyzer does not exist
     */
    void addObject( const std::string& pAnalyzerName, const std::string& pGroupName, const NeighborGroup* pNeighborGroup ) throw (Exception);
    
    /**
     \brief remove space object
     \param pAnalyzerName name of space objects analyzer
     \param pGroupIndex space object group index
     \param pSpaceObject space object
     \exception Exception if space objects analyzer does not exist
     */
    void removeObject( const std::string& pAnalyzerName, unsigned int pGroupIndex, const SpaceObject* pSpaceObject ) throw (Exception);
    
    /**
     \brief remove space object
     \param pAnalyzerName name of space objects analyzer
     \param pGroupName space object group name
     \param pSpaceObject space object
     \exception Exception if space objects analyzer does not exist
     */
    void removeObject( const std::string& pAnalyzerName, const std::string& pGroupName, const SpaceObject* pSpaceObject ) throw (Exception);
    
    /**
     \brief remove space object
     \param pAnalyzerName name of space objects analyzer
     \param pGroupIndex space object group index
     \param pNeighborGroup neighbor group
     \exception Exception if space objects analyzer does not exist
     */
    void removeObject( const std::string& pAnalyzerName, unsigned int pGroupIndex, const NeighborGroup* pNeighborGroup ) throw (Exception);
    
    /**
     \brief remove space object
     \param pAnalyzerName name of space objects analyzer
     \param pGroupName space object group name
     \param pNeighborGroup neighbor group
     \exception Exception if space objects analyzer does not exist
     */
    void removeObject( const std::string& pAnalyzerName, const std::string& pGroupName, const NeighborGroup* pNeighborGroup ) throw (Exception);
    
    /**
     \brief remove space objects
     \param pAnalyzerName name of space objects analyzer
     \param pGroupIndex space object group index
     \exception Exception if space objects analyzer does not exist
     */
    void removeObjects( const std::string& pAnalyzerName, unsigned int pGroupIndex ) throw (Exception);
    
    /**
     \brief remove space objects
     \param pAnalyzerName name of space objects analyzer
     \param pGroupName space data group name
     \exception Exception if space objects analyzer does not exist
     */
    void removeObjects( const std::string& pAnalyzerName, const std::string& pGroupName ) throw (Exception);
    
    /**
     \brief return analysis result
     \param pAnalyzerName name of space objects analyzer
     \param pResultIndex result index
     \return result as vector
     \exception Exception if space objects analyzer does not exist
     */
    const Eigen::VectorXf& result( const std::string& pAnalyzerName, unsigned int pResultIndex ) const throw (Exception);
    
    /**
     \brief return analysis result
     \param pAnalyzerName name of space objects analyzer
     \param pResultName result name
     \return result as vector
     \exception Exception if space objects analyzer does not exist
     */
    const Eigen::VectorXf& result( const std::string& pAnalyzerName, const std::string& pResultName ) const throw (Exception);
    
    /**
     \brief return analysis results
     \param pAnalyzerName name of space objects analyzer
     \return IndexHash of all vector results
     \exception Exception if space objects analyzer does not exist
     */
    const IndexMap< std::string, Eigen::VectorXf >& results(  const std::string& pAnalyzerName ) const throw (Exception);
    
    /**
     \brief update all space objects analyzers
     */	
    void update();
    
    /**
     \brief update all space objects analyzers
     */
    void analyze();

    /**
     \brief print data analyzer manager information
     */
    virtual operator std::string() const;
    
    /**
     \brief print data analyzer manager information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual space objects analyzer manager info
     \param pOstream output text stream
     \param pAnalyzeManager space objects analyze manager
     */
    friend std::ostream& operator << ( std::ostream& pOstream, const SpaceObjectsAnalyzeManager& pAnalyzeManager )
    {
        pOstream << pAnalyzeManager.info();
        
        return pOstream;
    };
    
protected:
    /**
     \brief default constructor
     */
    SpaceObjectsAnalyzeManager();
    
    /**
     \brief destructor
     */
    ~SpaceObjectsAnalyzeManager();
    
    /**
     \brief spaces
     */
    IndexMap<std::string, SpaceObjectsAnalyzer*> mAnalyzers;
};

};

};

#endif