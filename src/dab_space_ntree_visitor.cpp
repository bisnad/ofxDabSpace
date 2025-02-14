/**	\file dab_space_ntree_visitor.cpp
 */

#include "dab_space_ntree_visitor.h"
#include "dab_space_proxy_object.h"
#include "dab_space_ntree_node_pool.h"
#include <math.h>
#include <cfloat>

using namespace dab;
using namespace dab::space;

NTreeVisitor::NTreeVisitor()
: mDim(1)
, mNodePool(nullptr)
, mCenterPos(mDim)
, mMinPos(mDim)
, mMaxPos(mDim)
{
    //createNodePool();
}

NTreeVisitor::NTreeVisitor(unsigned int pDim)
: mDim(pDim)
, mNodePool(nullptr)
, mCenterPos(mDim)
, mMinPos(mDim)
, mMaxPos(mDim)
{
    //createNodePool();
}

NTreeVisitor::~NTreeVisitor()
{
	if(mNodePool != nullptr) delete mNodePool;
}

void
NTreeVisitor::createNodePool()
{
	if(mNodePool != nullptr) return;
	
	mNodePool = new NTreeNodePool(mDim);
}

void
NTreeVisitor::buildTree(NTree& pTree, std::vector<SpaceProxyObject*>& pObjects)
{
    // create root node
    if(mNodePool != NULL) pTree.mRootNode = mNodePool->retrieve();
	else pTree.mRootNode = new NTreeNode(mDim);
    
    // configure root node
    pTree.mRootNode->mMinPos = pTree.mMinPos;
    pTree.mRootNode->mMaxPos = pTree.mMaxPos;
    pTree.mRootNode->mParent = NULL;
    pTree.mRootNode->mLastCheckedObject = NULL;
    pTree.mRootNode->mLevel = 0;
    pTree.mRootNode->mObjects = pObjects;
    
    // start recursive node creation
    buildTree(pTree, pTree.mRootNode);
}

void
NTreeVisitor::buildTree(NTree& pTree, NTreeNode* pNode)
{
	unsigned int childrenCount = pNode->childrenCount();
    
    std::vector<SpaceProxyObject*>& objects = pNode->mObjects;
	int objectCount = objects.size();
	
	// create children for node
	//std::cout << "objectCount " << objectCount << " maxDepth " << pTree.mMaxDepth << " nodeLevel " << pNode->mLevel << "\n";
	
	if(objectCount > 1 && (pTree.mMaxDepth == -1 || pTree.mMaxDepth > pNode->mLevel) && (pTree.mMinObjectCount == -1 || pTree.mMinObjectCount < objectCount))
	{
		//std::cout << "continue\n";
        
		for(unsigned int dim=0; dim<mDim; ++dim) mCenterPos[dim] = (pNode->mMinPos[dim] + pNode->mMaxPos[dim]) * 0.5;
		
		int dimBits = 0;
		int maskBits = 1;
		
		for(unsigned int childNr = 0; childNr < childrenCount; ++childNr)
		{
			int maskBits = 1;
			
			//std::cout << "childNr " << childNr << " maskBits " << maskBits << "\n";
            
			for(unsigned int dim = 0; dim < mDim; ++dim)
			{
				//std::cout << "dim " << dim << " maskBits " << maskBits << " childNr " << childNr << " childNr & maskBits " << (childNr & maskBits) << "\n";
                
				if(childNr & maskBits)
				{
					mMinPos[dim] = mCenterPos[dim];
					mMaxPos[dim] = pNode->mMaxPos[dim];
				}
				else
				{
					mMinPos[dim] = pNode->mMinPos[dim];
					mMaxPos[dim] = mCenterPos[dim];
				}
				
				maskBits = maskBits << 1;
			}
			
			dimBits += 1;
			
			// create child node
            if(mNodePool != NULL) pNode->mChildren[childNr] = mNodePool->retrieve();
			else pNode->mChildren[childNr] = new NTreeNode(mDim);
            NTreeNode* childNode = pNode->mChildren[childNr];
			
			// configure child node
			childNode->mMinPos = mMinPos;
			childNode->mMaxPos = mMaxPos;
			childNode->mParent = pNode;
			childNode->mLastCheckedObject = NULL;
			childNode->mLevel = pNode->mLevel + 1;
			
			// add all objects within minPos and maxPos to child node
			bool insertObject;
			
			for(int i=0; i<objectCount; ++i)
			{
				insertObject = true;
				
				Eigen::Matrix<float, Eigen::Dynamic, 1>& objectPosition = objects[i]->position();
				
				for(unsigned int j=0; j<mDim; ++j)
				{
					if(objectPosition[j] < childNode->mMinPos[j] || objectPosition[j] > childNode->mMaxPos[j])
					{
						insertObject = false;
						break;
					}
				}
				
				if(insertObject == true) childNode->mObjects.push_back(objects[i]);
			}
			
			//std::cout << "child node created:\n" << *childNode << "\n";
		}
	}
	else
	{
		//std::cout << "abort\n";
		//if(pTree.mMaxDepth > pNode->mLevel) std::cout << "abort building children";
        
		for(int i=0; i<childrenCount; ++i) pNode->mChildren[i] = nullptr;
		return;
	}
    
	for(int i=0; i<childrenCount; ++i)
	{
		buildTree(pTree, pNode->mChildren[i]);
	}
}

void
NTreeVisitor::updateTree(NTree& pTree, std::vector<SpaceProxyObject*>& pObjects)
{
    // create root node
    if(pTree.mRootNode == nullptr)
    {
        buildTree(pTree, pObjects);
        return;
    }
    
    // configure root node
    pTree.mRootNode->mLastCheckedObject = nullptr;
    pTree.mRootNode->mObjects = pObjects;
    
    // start recursive node creation
    if(pTree.mRootNode->mChildren[0] == nullptr) buildTree(pTree, pTree.mRootNode);
    else updateTree(pTree, pTree.mRootNode);
}

void
NTreeVisitor::updateTree(NTree& pTree, NTreeNode* pNode)
{
	unsigned int childrenCount = pNode->childrenCount();
    
    std::vector<SpaceProxyObject*>& objects = pNode->mObjects;
	int objectCount = objects.size();
	
	//std::cout << "updateTree node " << *pNode << " objectCount " << objectCount << "\n";
	
	if(objectCount > 1 && (pTree.mMaxDepth == -1 || pTree.mMaxDepth > pNode->mLevel) && (pTree.mMinObjectCount == -1 || pTree.mMinObjectCount < objectCount))
	{
		for(unsigned int childNr = 0; childNr < childrenCount; ++childNr)
		{
			// create child node
            NTreeNode* childNode = pNode->mChildren[childNr];
			
			// configure child node
			childNode->mLastCheckedObject = NULL;
			childNode->mObjects.clear();
			
			// add all objects within minPos and maxPos to child node
			bool insertObject;
			
			for(int i=0; i<objectCount; ++i)
			{
				if(objects[i]->visible() == false) continue;
                
				insertObject = true;
				
                Eigen::Matrix<float, Eigen::Dynamic, 1>& objectPosition = objects[i]->position();
				
				for(unsigned int j=0; j<mDim; ++j)
				{
					if(objectPosition[j] < childNode->mMinPos[j] || objectPosition[j] > childNode->mMaxPos[j])
					{
						insertObject = false;
						break;
					}
				}
				
				if(insertObject == true) childNode->mObjects.push_back(objects[i]);
			}
			
			//std::cout << "child node created:\n" << *childNode << "\n";
		}
	}
	else
	{
		// get rid of children
		for(int i=0; i<childrenCount; ++i)
		{
			clearTree(pNode->mChildren[i]);
			pNode->mChildren[i] = NULL;
		}
		return;
	}
    
	for(int i=0; i<childrenCount; ++i)
	{
        NTreeNode* childNode = pNode->mChildren[i];
		
		if(childNode->mChildren[i] == NULL) buildTree(pTree, childNode);
		else updateTree(pTree, childNode);
	}
}

void
NTreeVisitor::calcNeighbors(NTree& pTree, std::vector<SpaceProxyObject*>& pObjects)
{
    if(pTree.mRootNode != nullptr)
    {
        calcNeighbors(pTree.mRootNode, pObjects);
    }
}

void
NTreeVisitor::calcNeighbors( NTreeNode* pNode, std::vector<SpaceProxyObject*>& pObjects)
{
    // leaf node -> start calculating neighbors
	if(pNode->mChildren[0] == NULL)
	{
		unsigned int objectCount = pObjects.size();
        SpaceProxyObject* object;
		
		for(unsigned int i=0; i<objectCount; ++i)
		{
			object = pObjects[i];
            
			//std::cout << "calcNeighbors i " << i << " object pos " << pObjects[i]->position() << " node level " << pNode->mLevel << " min " << pNode->mMinPos << " max " << pNode->mMaxPos << "\n";
			
			const Eigen::Matrix<float, Eigen::Dynamic, 1>& objectPos = object->position();
			float neighborRadius = object->neighborRadius();
			
			if(neighborRadius >= 0)
			{
				for(unsigned int dim=0; dim<mDim; ++dim)
				{
					mMinPos[dim] = objectPos[dim] - neighborRadius;
					mMaxPos[dim] = objectPos[dim] + neighborRadius;
				}
			}
			else
			{
				for(unsigned int dim=0; dim<mDim; ++dim)
				{
					mMinPos[dim] = -FLT_MAX;
					mMaxPos[dim] = FLT_MAX;
				}
			}
            
			object->removeNeighbors();
            
			calcNeighbors(pNode, object);
		}
		return;
	}
	
	// keep searching for leaf nodes
	// distribute objects into separate object groups for each child node
	unsigned int childNodeCount = pNode->childrenCount();
	unsigned int objectCount = pObjects.size();
    std::vector<SpaceProxyObject*>* childNodeObjects = new std::vector<SpaceProxyObject*>[childNodeCount];
    
    SpaceProxyObject* object;
    NTreeNode* childNode;
	bool insertObject;
	
	for(unsigned int i=0; i<objectCount; ++i)
	{
		object = pObjects[i];
		Eigen::Matrix<float, Eigen::Dynamic, 1>& objectPosition = object->position();
        
		for(unsigned int j=0; j<childNodeCount; ++j)
		{
			childNode = pNode->mChildren[j];
			
			insertObject = true;
            
			for(unsigned int z=0; z<mDim; ++z)
			{
				if(objectPosition[z] < childNode->mMinPos[z] || objectPosition[z] > childNode->mMaxPos[z])
				{
					insertObject = false;
					break;
				}
			}
			
			if(insertObject == true)
			{
				childNodeObjects[j].push_back(object);
				break;
			}
		}
	}
	
	//pObjects.clear();
	
	// proceed into child nodes
	for(unsigned int i=0; i<childNodeCount; ++i) calcNeighbors(pNode->mChildren[i], childNodeObjects[i]);
    
    delete [] childNodeObjects;
}

void
NTreeVisitor::calcNeighbors( NTreeNode* pNode, SpaceProxyObject* pObject)
{
	// check whether this node has already been visited when searching for neighbors for this object
	if(pNode->mLastCheckedObject == pObject) return;
	pNode->mLastCheckedObject = pObject;
    
	// check whether the object accepts more neighbors
	if(pObject->neighborListFull() == true) return;
    
	// check whether this node is within the neighbor search radius of this object
	for(unsigned int i=0; i<mDim; ++i) if(mMaxPos[i] < pNode->mMinPos[i] || mMinPos[i] > pNode->mMaxPos[i]) return;
	
	// process leaf node
	if(pNode->mChildren[0] == nullptr) // leaf node
	{
		//std::cout << "process leaf node\n";
        
		// add objects within this node as neighbors
		int objectCount = pNode->mObjects.size();
        
		for(int i=0; i<objectCount; ++i)
		{
			if(pObject != pNode->mObjects[i])
			{
				//std::cout << "object ( " << pObject <<  " ) " << pObject->position() << " add neigbhbor ( " << pNode->mObjects[i] << " ) " << pNode->mObjects[i]->position() << "\n";
                
				pObject->addNeighbor(pNode->mObjects[i]->spaceObject());
				
				//std::cout << "neighbor added\n";
			}
		}
	}
	
	// progress into child nodes
	if(pNode->mChildren[0] != nullptr) // non leaf node
	{
		//std::cout << "progress into child nodes\n";
        
		unsigned int childrenCount = pNode->childrenCount();
		for(int i=0; i<childrenCount; ++i)
		{
			if(pNode->mChildren[i]->mLastCheckedObject != pObject) calcNeighbors(pNode->mChildren[i], pObject);
		}
	}
	
	// progress into parent nodes
	if(pNode->mParent != NULL && pNode->mParent->mLastCheckedObject != pObject)
	{
		// check whether this node completely encompasses this agents search region -> no need to proceed to the parent node
		bool objectWithinNode = true;
		
		for(unsigned int i=0; i<mDim; ++i)
		{
			if(mMinPos[i] < pNode->mMinPos[i] || mMaxPos[i] > pNode->mMaxPos[i])
			{
				objectWithinNode = false;
				break;
			}
		}
		
		if(objectWithinNode == true) return;
		calcNeighbors(pNode->mParent, pObject);
	}
}

void
NTreeVisitor::clearTree(NTree& pTree)
{
	if(pTree.mRootNode != NULL) clearTree(pTree.mRootNode);
	pTree.mRootNode = NULL;
}

void
NTreeVisitor::clearTree(NTreeNode* pNode)
{
	unsigned int childrenCount = pNode->childrenCount();
    
	for(unsigned int i=0; i<childrenCount; ++i)
	{
		if(pNode->mChildren[i] != nullptr)
		{
			clearTree(pNode->mChildren[i]);
		}
	}
	
	pNode->clear();
    
	if(mNodePool != NULL) mNodePool->release(pNode);
	else delete pNode;
}

std::string
NTreeVisitor::info(const NTree& pTree) const
{
    std::stringstream stream;
    
	if(pTree.mRootNode != nullptr) stream << info(pTree.mRootNode);
	
	return stream.str();
}

std::string
NTreeVisitor::info(const NTreeNode* pNode) const
{
	std::stringstream stream;
    
	unsigned int childrenCount = pNode->childrenCount();
    
	for(unsigned int i=0; i<childrenCount; ++i)
	{
		if(pNode->mChildren[i] != nullptr)
		{
            stream << info(pNode->mChildren[i]);
		}
	}	
	
	if(pNode->objectCount() > 0)
	{
        stream << pNode->info() << "\n";
	}
	
	return stream.str();
}