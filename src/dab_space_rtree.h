/** \file iso_space_rtree.h
 */

#ifndef _dab_space_rtree_h_
#define _dab_space_rtree_h_

#include "dab_space_shape.h"
#include <vector>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

namespace dab
{

namespace space
{

#define RTREE_DONT_USE_MEMPOOLS // This version does not contain a fixed memory allocator, fill in lines with EXAMPLE to implement one.
#define RTREE_USE_SPHERICAL_VOLUME // Better split classification, may be slower on some systems

// Fwd decl
class RTFileStream;  // File I/O helper class, look below for implementation and notes.


/// \class RTree
/// Implementation of RTree, a multidimensional bounding rectangle tree.
/// Example usage: For a 3-dimensional tree use RTree<Object*, float, 3> myTree;
///
/// This modified, templated C++ version by Greg Douglas at Auran (http://www.auran.com)
///
/// DATATYPE Referenced data, should be int, void*, obj* etc. no larger than sizeof<void*> and simple type
/// ELEMTYPE Type of element such as int or float
/// ELEMTYPEREAL Type of element that allows fractional and large values such as float or double, for use in volume calcs
///
/// NOTES: Inserting and removing data requires the knowledge of its constant Minimal Bounding Rectangle.
///        This version uses new/delete for nodes, I recommend using a fixed size allocator for efficiency.
///        Instead of using a callback function for returned results, I recommend and efficient pre-sized, grow-only memory
///        array similar to MFC CArray or STL Vector for returning search query result.
///
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL = ELEMTYPE, int TMAXNODES = 8, int TMINNODES = TMAXNODES / 2>

class RTree
{
protected:
    
    class Node;  // Fwd decl.  Used by other internal structs and iterator
    unsigned int mDim;
    
public:
    
    // These constant must be declared after Branch and before Node struct
    // Stuck up here for MSVC 6 compiler.  NSVC .NET 2003 is much happier.
    enum
    {
        MAXNODES = TMAXNODES,                         ///< Max elements in node
        MINNODES = TMINNODES,                         ///< Min elements in node
    };
    
    
public:
    
    RTree(unsigned int pDim);
    virtual ~RTree();
    
    /// Insert entry
    /// \param a_min Min of bounding rect
    /// \param a_max Max of bounding rect
    /// \param a_dataId Positive Id of data.  Maybe zero, but negative numbers not allowed.
    void Insert(const ELEMTYPE* a_min, const ELEMTYPE* a_max, const DATATYPE& a_dataId);
    
    /// Remove entry
    /// \param a_min Min of bounding rect
    /// \param a_max Max of bounding rect
    /// \param a_dataId Positive Id of data.  Maybe zero, but negative numbers not allowed.
    void Remove(const ELEMTYPE* a_min, const ELEMTYPE* a_max, const DATATYPE& a_dataId);
    
    /// Find all within search rectangle
    /// \param a_min Min of search bounding rect
    /// \param a_max Max of search bounding rect
    /// \param pSearchResults Search result array.
    void Search(const ELEMTYPE* a_min, const ELEMTYPE* a_max, std::vector<DATATYPE>& pSearchResults);
    
    /// Remove all entries from tree
    void RemoveAll();
    
    /// Count the data elements in this container.  This is slow as no internal counter is maintained.
    int Count();
    
    /// Load tree contents from file
    bool Load(const char* a_fileName);
    /// Load tree contents from stream
    bool Load(RTFileStream& a_stream);
    
    
    /// Save tree contents to file
    bool Save(const char* a_fileName);
    /// Save tree contents to stream
    bool Save(RTFileStream& a_stream);
    
    /// Iterator is not remove safe.
    class Iterator
    {
    private:
        
        enum { MAX_STACK = 32 }; //  Max stack size. Allows almost n^32 where n is number of branches in node
        
        struct StackElement
        {
            Node* m_node;
            int m_branchIndex;
        };
        
    public:
        
        Iterator()                                    { Init(); }
        
        ~Iterator()                                   { }
        
        /// Is iterator invalid
        bool IsNull()                                 { return (m_tos <= 0); }
        
        /// Is iterator pointing to valid data
        bool IsNotNull()                              { return (m_tos > 0); }
        
        /// Access the current data element. Caller must be sure iterator is not NULL first.
        DATATYPE& operator*()
        {
            assert(IsNotNull());
            StackElement& curTos = m_stack[m_tos - 1];
            return curTos.m_node->m_branch[curTos.m_branchIndex].m_data;
        }
        
        /// Access the current data element. Caller must be sure iterator is not NULL first.
        const DATATYPE& operator*() const
        {
            assert(IsNotNull());
            StackElement& curTos = m_stack[m_tos - 1];
            return curTos.m_node->m_branch[curTos.m_branchIndex].m_data;
        }
        
        /// Find the next data element
        bool operator++()                             { return FindNextData(); }
        
    private:
        
        /// Reset iterator
        void Init()                                   { m_tos = 0; }
        
        /// Find the next data element in the tree (For internal use only)
        bool FindNextData()
        {
            for(;;)
            {
                if(m_tos <= 0)
                {
                    return false;
                }
                StackElement curTos = Pop(); // Copy stack top cause it may change as we use it
                
                if(curTos.m_node->IsLeaf())
                {
                    // Keep walking through data while we can
                    if(curTos.m_branchIndex+1 < curTos.m_node->m_count)
                    {
                        // There is more data, just point to the next one
                        Push(curTos.m_node, curTos.m_branchIndex + 1);
                        return true;
                    }
                    // No more data, so it will fall back to previous level
                }
                else
                {
                    if(curTos.m_branchIndex+1 < curTos.m_node->m_count)
                    {
                        // Push sibling on for future tree walk
                        // This is the 'fall back' node when we finish with the current level
                        Push(curTos.m_node, curTos.m_branchIndex + 1);
                    }
                    // Since cur node is not a leaf, push first of next level to get deeper into the tree
                    Node* nextLevelnode = curTos.m_node->m_branch[curTos.m_branchIndex].m_child;
                    Push(nextLevelnode, 0);
                    
                    // If we pushed on a new leaf, exit as the data is ready at TOS
                    if(nextLevelnode->IsLeaf())
                    {
                        return true;
                    }
                }
            }
        }
        
        /// Push node and branch onto iteration stack (For internal use only)
        void Push(Node* a_node, int a_branchIndex)
        {
            m_stack[m_tos].m_node = a_node;
            m_stack[m_tos].m_branchIndex = a_branchIndex;
            ++m_tos;
            assert(m_tos <= MAX_STACK);
        }
        
        /// Pop element off iteration stack (For internal use only)
        StackElement& Pop()
        {
            assert(m_tos > 0);
            --m_tos;
            return m_stack[m_tos];
        }
        
        StackElement m_stack[MAX_STACK];              ///< Stack as we are doing iteration instead of recursion
        int m_tos;                                    ///< Top Of Stack index
        
        friend class RTree; // Allow hiding of non-public functions while allowing manipulation by logical owner
    };
    
    /// Get 'first' for iteration
    void GetFirst(Iterator& a_it)
    {
        a_it.Init();
        if(m_root && (m_root->m_count > 0))
        {
            a_it.Push(m_root, 0);
            a_it.FindNextData();
        }
    }
    
    /// Get Next for iteration
    void GetNext(Iterator& a_it)                    { ++a_it; }
    
    /// Is iterator NULL, or at end?
    bool IsNull(Iterator& a_it)                     { return a_it.IsNull(); }
    
    /// Get object at iterator position
    DATATYPE& GetAt(Iterator& a_it)                 { return *a_it; }
    
protected:
    
    class Rect
    {
    protected:
        Rect()
        {
            assert("illegal constructor");
        }
        
    public:
        unsigned int mDim;
        ELEMTYPE* mMin;                      ///< Min dimensions of bounding box
        ELEMTYPE* mMax;                      ///< Max dimensions of bounding box
        
        Rect(unsigned int pDim)
        : mDim(pDim)
        {
            mMin = new ELEMTYPE[mDim];
            mMax = new ELEMTYPE[mDim];
        }
        
        Rect(const Rect& pRect)
        : mDim(pRect.mDim)
        {
            mMin = new ELEMTYPE[mDim];
            mMax = new ELEMTYPE[mDim];
            
            for(unsigned int i=0; i<mDim; ++i)
            {
                mMin[i] = pRect.mMin[i];
                mMax[i] = pRect.mMax[i];
            }
        }
        
        ~Rect()
        {
            delete [] mMin;
            delete [] mMax;
        }
        
        const Rect&
        operator=( const Rect& pRect )
        {
            assert(mDim == pRect.mDim);
            
            for(unsigned int i=0; i<mDim; ++i)
            {
                mMin[i] = pRect.mMin[i];
                mMax[i] = pRect.mMax[i];
            }
            
            return pRect;
        }
        
    };
    
    /// May be data or may be another subtree
    /// The parents level determines this.
    /// If the parents level is 0, then this is data
    class Branch
    {
    public:
        Rect mRect;                                  ///< Bounds
        union
        {
            Node* mChild;                              ///< Child node
            DATATYPE mData;                            ///< Data Id or Ptr
        };
        
    protected:
        Branch()
        : mRect(0)
        {
            assert("illegal constructor");
        }
        
    public:
        
        Branch(unsigned int pDim)
        : mRect(pDim)
        {}
        
        ~Branch()
        {}
        
        inline
        Rect&
        rect()
        {
            return mRect;
        }
        
        inline
        Node*
        child()
        {
            return mChild;
        }
        
        inline
        DATATYPE&
        data()
        {
            return mData;
        }
        
        inline
        void
        setRect(Rect& pRect)
        {
            mRect = pRect;
        }
        
        inline
        void
        setChild(Node* pChild)
        {
            mChild = pChild;
        }
        
        inline
        void
        setData(DATATYPE& pData)
        {
            mData = pData;
        }
        
    };
    
    /// Node for each branch level
    class Node
    {
    public:
        int mCount;                                  ///< Count
        int mLevel;                                  ///< Leaf is zero, others positive
        Branch** mBranch;							 ///< Branch
        
    protected:
        Node()
        {
            assert("illegal constructor");
        }
        
    public:
        Node(unsigned int pDim)
        : mCount(0)
        , mLevel(0)
        {
            mBranch = new Branch*[MAXNODES];
            for(unsigned int i=0; i<MAXNODES; ++i) mBranch[i] = new Branch(pDim);
        }
        
        Node(int pDim, int pCount, int pLevel)
        : mCount(pCount)
        , mLevel(pLevel)
        {
            mBranch = new Branch*[MAXNODES];
            for(unsigned int i=0; i<MAXNODES; ++i) mBranch[i] = new Branch(pDim);
        }
        
        ~Node()
        {
            for(unsigned int i=0; i<MAXNODES; ++i) delete mBranch[i];
            delete [] mBranch;
        }
        
        inline
        bool
        IsInternalNode()
        {
            return (mLevel > 0);
        } // Not a leaf, but a internal node
        
        inline
        bool
        IsLeaf()
        {
            return (mLevel == 0);
        } // A leaf, contains data
        
        inline
        int
        level()
        {
            return mLevel;
        }
        
        inline
        int
        count()
        {
            return mCount;
        }
        
        inline
        Branch&
        branch(unsigned int pIndex)
        {
            assert(pIndex < MAXNODES);
            
            return *(mBranch[pIndex]);
        }
        
        inline
        void
        setLevel(int pLevel)
        {
            mLevel = pLevel;
        }
        
        inline
        void
        setCount(int pCount)
        {
            mCount = pCount;
        }
        
        inline
        void
        setBranch(Branch* pBranch, unsigned int pIndex)
        {
            assert(pIndex < MAXNODES);
            
            mBranch[pIndex] = pBranch;
        }
    };
    
    /// A link list of nodes for reinsertion after a delete operation
    class ListNode
    {
    public:
        ListNode* mNext;                             ///< Next in list
        Node* mNode;                                 ///< Node
        
    protected:
        ListNode()
        {
            assert("illegal constructor");
        }
        
    public:
        ListNode(ListNode* pNext, Node* pNode)
        : mNext(pNext)
        , mNode(pNode)
        {}
        
        inline
        ListNode*
        next()
        {
            return mNext;
        }
        
        inline
        Node*
        node()
        {
            return mNode;
        }
        
        inline
        void
        setNext(ListNode* pNext)
        {
            mNext = pNext;
        }
        
        inline
        void
        setNode(Node* pNode)
        {
            mNode = pNode;
        }
    };
    
    /// Variables for finding a split partition
    class PartitionVars
    {
        friend class RTree;
        
    public:
        int mPartition[MAXNODES+1];
        int mTotal;
        int mMinFill;
        int mTaken[MAXNODES+1];
        int mCount[2];
        Rect** mCover;
        ELEMTYPEREAL mArea[2];
        
        Branch** mBranchBuf;
        int mBranchCount;
        Rect mCoverSplit;
        ELEMTYPEREAL mCoverSplitArea;
        
    protected:
        PartitionVars()
        : mCoverSplit(0)
        {
            assert("illegal contructor");
        }
        
    public:
        
        PartitionVars(unsigned int pDim)
        : mCoverSplit(pDim)
        {
            mCover = new Rect*[2];
            mCover[0] = new Rect(pDim);
            mCover[1] = new Rect(pDim);
            
            mBranchBuf = new Branch*[MAXNODES+1];
            for(unsigned int i=0; i<MAXNODES+1; ++i) mBranchBuf[i] = new Branch(pDim);
        }
        
        ~PartitionVars()
        {
            delete mCover[0];
            delete mCover[1];
            delete [] mCover;
            
            for(unsigned int i=0; i<MAXNODES+1; ++i) delete mBranchBuf[i];
            delete [] mBranchBuf;
        }
        
        inline
        Branch&
        branch(unsigned int pIndex)
        {
            assert(pIndex < MAXNODES+1);
            
            return *(mBranchBuf[pIndex]);
        }
        
        inline
        Rect&
        cover(unsigned int pIndex)
        {
            assert(pIndex < 2);
            
            return *(mCover[pIndex]);
        }
        
        inline
        void
        setBranch(Branch& pBranch, unsigned int pIndex)
        {
            assert(pIndex < MAXNODES+1);
            
            Branch* branch = mBranchBuf[pIndex];
            
            branch->setRect( pBranch.rect() );
            branch->setChild( pBranch.child() );
            branch->setData( pBranch.data() );
        }
        
        inline
        void
        setCover(Rect& pRect, unsigned int pIndex)
        {
            assert(pIndex < 2);
            
            *(mCover[pIndex]) = pRect;
        }
    };
    
    Node* AllocNode();
    void FreeNode(Node* a_node);
    void InitNode(Node* a_node);
    void InitRect(Rect* a_rect);
    bool InsertRectRec(Rect* a_rect, const DATATYPE& a_id, Node* a_node, Node** a_newNode, int a_level);
    bool InsertRect(Rect* a_rect, const DATATYPE& a_id, Node** a_root, int a_level);
    Rect NodeCover(Node* a_node);
    bool AddBranch(Branch* a_branch, Node* a_node, Node** a_newNode);
    void DisconnectBranch(Node* a_node, int a_index);
    int PickBranch(Rect* a_rect, Node* a_node);
    Rect CombineRect(Rect* a_rectA, Rect* a_rectB);
    void SplitNode(Node* a_node, Branch* a_branch, Node** a_newNode);
    ELEMTYPEREAL RectSphericalVolume(Rect* a_rect);
    ELEMTYPEREAL RectVolume(Rect* a_rect);
    ELEMTYPEREAL CalcRectVolume(Rect* a_rect);
    void GetBranches(Node* a_node, Branch* a_branch, PartitionVars* a_parVars);
    void ChoosePartition(PartitionVars* a_parVars, int a_minFill);
    void LoadNodes(Node* a_nodeA, Node* a_nodeB, PartitionVars* a_parVars);
    void InitParVars(PartitionVars* a_parVars, int a_maxRects, int a_minFill);
    void PickSeeds(PartitionVars* a_parVars);
    void Classify(int a_index, int a_group, PartitionVars* a_parVars);
    bool RemoveRect(Rect* a_rect, const DATATYPE& a_id, Node** a_root);
    bool RemoveRectRec(Rect* a_rect, const DATATYPE& a_id, Node* a_node, ListNode** a_listNode);
    ListNode* AllocListNode();
    void FreeListNode(ListNode* a_listNode);
    bool Overlap(Rect* a_rectA, Rect* a_rectB);
    void ReInsert(Node* a_node, ListNode** a_listNode);
    bool Search(Node* a_node, Rect* a_rect, std::vector<DATATYPE>& pSearchResults);
    void RemoveAllRec(Node* a_node);
    void Reset();
    void CountRec(Node* a_node, int& a_count);
    
    bool SaveRec(Node* a_node, RTFileStream& a_stream);
    bool LoadRec(Node* a_node, RTFileStream& a_stream);
    
    Node* m_root;                                    ///< Root of tree
    ELEMTYPEREAL m_unitSphereVolume;                 ///< Unit sphere constant for required number of dimensions
};


// Because there is not stream support, this is a quick and dirty file I/O helper.
// Users will likely replace its usage with a Stream implementation from their favorite API.
class RTFileStream
{
    FILE* m_file;
    
public:
    
    
    RTFileStream()
    {
        m_file = NULL;
    }
    
    ~RTFileStream()
    {
        Close();
    }
    
    bool OpenRead(const char* a_fileName)
    {
        m_file = fopen(a_fileName, "rb");
        if(!m_file)
        {
            return false;
        }
        return true;
    }
    
    bool OpenWrite(const char* a_fileName)
    {
        m_file = fopen(a_fileName, "wb");
        if(!m_file)
        {
            return false;
        }
        return true;
    }
    
    void Close()
    {
        if(m_file)
        {
            fclose(m_file);
            m_file = NULL;
        }
    }
    
    template< typename TYPE >
    size_t Write(const TYPE& a_value)
    {
        assert(m_file);
        return fwrite((void*)&a_value, sizeof(a_value), 1, m_file);
    }
    
    template< typename TYPE >
    size_t WriteArray(const TYPE* a_array, int a_count)
    {
        assert(m_file);
        return fwrite((void*)a_array, sizeof(TYPE) * a_count, 1, m_file);
    }
    
    template< typename TYPE >
    size_t Read(TYPE& a_value)
    {
        assert(m_file);
        return fread((void*)&a_value, sizeof(a_value), 1, m_file);
    }
    
    template< typename TYPE >
    size_t ReadArray(TYPE* a_array, int a_count)
    {
        assert(m_file);
        return fread((void*)a_array, sizeof(TYPE) * a_count, 1, m_file);
    }
};


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::RTree(unsigned int pDim)
: mDim(pDim)
{
    assert(MAXNODES > MINNODES);
    assert(MINNODES > 0);
    
    
    // We only support machine word size simple data type eg. integer index or object pointer.
    // Since we are storing as union with non data branch
    assert(sizeof(DATATYPE) == sizeof(void*) || sizeof(DATATYPE) == sizeof(int));
    
    // Precomputed volumes of the unit spheres for the first few dimensions
    const float UNIT_SPHERE_VOLUMES[] = {
        0.000000f, 2.000000f, 3.141593f, // Dimension  0,1,2
        4.188790f, 4.934802f, 5.263789f, // Dimension  3,4,5
        5.167713f, 4.724766f, 4.058712f, // Dimension  6,7,8
        3.298509f, 2.550164f, 1.884104f, // Dimension  9,10,11
        1.335263f, 0.910629f, 0.599265f, // Dimension  12,13,14
        0.381443f, 0.235331f, 0.140981f, // Dimension  15,16,17
        0.082146f, 0.046622f, 0.025807f, // Dimension  18,19,20
    };
    
    m_root = AllocNode();
    m_root->mLevel = 0;
    m_unitSphereVolume = (ELEMTYPEREAL)UNIT_SPHERE_VOLUMES[mDim];
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::~RTree()
{
    Reset(); // Free, or reset node memory
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Insert(const ELEMTYPE* a_min, const ELEMTYPE* a_max, const DATATYPE& a_dataId)
{
#ifdef _DEBUG
    for(int index=0; index<mDim; ++index)
    {
        assert(a_min[index] <= a_max[index]);
    }
#endif //_DEBUG
    
    Rect rect(mDim);
    
    for(int axis=0; axis<rect.mDim; ++axis)
    {
        rect.mMin[axis] = a_min[axis];
        rect.mMax[axis] = a_max[axis];
    }
    
    InsertRect(&rect, a_dataId, &m_root, 0);
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Remove(const ELEMTYPE* a_min, const ELEMTYPE* a_max, const DATATYPE& a_dataId)
{
#ifdef _DEBUG
    for(int index=0; index<mDim; ++index)
    {
        assert(a_min[index] <= a_max[index]);
    }
#endif //_DEBUG
    
    Rect rect(mDim);
    
    for(int axis=0; axis<rect.mDim; ++axis)
    {
        rect.mMin[axis] = a_min[axis];
        rect.mMax[axis] = a_max[axis];
    }
    
    RemoveRect(&rect, a_dataId, &m_root);
}

template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
    RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Search(const ELEMTYPE* a_min, const ELEMTYPE* a_max, std::vector<DATATYPE>& pSearchResults)
{
#ifdef _DEBUG
    for(int index=0; index<mDim; ++index)
    {
        assert(a_min[index] <= a_max[index]);
    }
#endif //_DEBUG
    
    pSearchResults.clear();
    
    Rect rect(mDim);
    
    for(int axis=0; axis<rect.mDim; ++axis)
    {
        rect.mMin[axis] = a_min[axis];
        rect.mMax[axis] = a_max[axis];
    }
    
    // NOTE: May want to return search result another way, perhaps returning the number of found elements here.
    
    Search(m_root, &rect, pSearchResults);
}

template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
int
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Count()
{
    int count = 0;
    CountRec(m_root, count);
    
    return count;
}



template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::CountRec(Node* a_node, int& a_count)
{
    if(a_node->IsInternalNode())  // not a leaf node
    {
        for(int index = 0; index < a_node->m_count; ++index)
        {
            CountRec(a_node->m_branch[index].m_child, a_count);
        }
    }
    else // A leaf node
    {
        a_count += a_node->m_count;
    }
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Load(const char* a_fileName)
{
    RemoveAll(); // Clear existing tree
    
    RTFileStream stream;
    if(!stream.OpenRead(a_fileName))
    {
        return false;
    }
    
    bool result = Load(stream);
    
    stream.Close();
    
    return result;
};



template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Load(RTFileStream& a_stream)
{
    // Write some kind of header
    int _dataFileId = ('R'<<0)|('T'<<8)|('R'<<16)|('E'<<24);
    int _dataSize = sizeof(DATATYPE);
    int _dataNumDims = mDim;
    int _dataElemSize = sizeof(ELEMTYPE);
    int _dataElemRealSize = sizeof(ELEMTYPEREAL);
    int _dataMaxNodes = TMAXNODES;
    int _dataMinNodes = TMINNODES;
    
    int dataFileId = 0;
    int dataSize = 0;
    int dataNumDims = 0;
    int dataElemSize = 0;
    int dataElemRealSize = 0;
    int dataMaxNodes = 0;
    int dataMinNodes = 0;
    
    a_stream.Read(dataFileId);
    a_stream.Read(dataSize);
    a_stream.Read(dataNumDims);
    a_stream.Read(dataElemSize);
    a_stream.Read(dataElemRealSize);
    a_stream.Read(dataMaxNodes);
    a_stream.Read(dataMinNodes);
    
    bool result = false;
    
    // Test if header was valid and compatible
    if(    (dataFileId == _dataFileId)
       && (dataSize == _dataSize)
       && (dataNumDims == _dataNumDims)
       && (dataElemSize == _dataElemSize)
       && (dataElemRealSize == _dataElemRealSize)
       && (dataMaxNodes == _dataMaxNodes)
       && (dataMinNodes == _dataMinNodes)
       )
    {
        // Recursively load tree
        result = LoadRec(m_root, a_stream);
    }
    
    return result;
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::LoadRec(Node* a_node, RTFileStream& a_stream)
{
    a_stream.Read(a_node->m_level);
    a_stream.Read(a_node->m_count);
    
    if(a_node->IsInternalNode())  // not a leaf node
    {
        for(int index = 0; index < a_node->m_count; ++index)
        {
            Branch* curBranch = &a_node->m_branch[index];
            
            a_stream.ReadArray(curBranch->m_rect.mMin, mDim);
            a_stream.ReadArray(curBranch->m_rect.mMax, mDim);
            
            curBranch->m_child = AllocNode();
            LoadRec(curBranch->m_child, a_stream);
        }
    }
    else // A leaf node
    {
        for(int index = 0; index < a_node->m_count; ++index)
        {
            Branch* curBranch = &a_node->m_branch[index];
            
            a_stream.ReadArray(curBranch->m_rect.mMin, mDim);
            a_stream.ReadArray(curBranch->m_rect.mMax, mDim);
            
            a_stream.Read(curBranch->m_data);
        }
    }
    
    return true; // Should do more error checking on I/O operations
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Save(const char* a_fileName)
{
    RTFileStream stream;
    if(!stream.OpenWrite(a_fileName))
    {
        return false;
    }
    
    bool result = Save(stream);
    
    stream.Close();
    
    return result;
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Save(RTFileStream& a_stream)
{
    // Write some kind of header
    int dataFileId = ('R'<<0)|('T'<<8)|('R'<<16)|('E'<<24);
    int dataSize = sizeof(DATATYPE);
    int dataNumDims = mDim;
    int dataElemSize = sizeof(ELEMTYPE);
    int dataElemRealSize = sizeof(ELEMTYPEREAL);
    int dataMaxNodes = TMAXNODES;
    int dataMinNodes = TMINNODES;
    
    a_stream.Write(dataFileId);
    a_stream.Write(dataSize);
    a_stream.Write(dataNumDims);
    a_stream.Write(dataElemSize);
    a_stream.Write(dataElemRealSize);
    a_stream.Write(dataMaxNodes);
    a_stream.Write(dataMinNodes);
    
    // Recursively save tree
    bool result = SaveRec(m_root, a_stream);
    
    return result;
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::SaveRec(Node* a_node, RTFileStream& a_stream)
{
    a_stream.Write(a_node->m_level);
    a_stream.Write(a_node->m_count);
    
    if(a_node->IsInternalNode())  // not a leaf node
    {
        for(int index = 0; index < a_node->m_count; ++index)
        {
            Branch* curBranch = &a_node->m_branch[index];
            
            a_stream.WriteArray(curBranch->m_rect.mMin, mDim);
            a_stream.WriteArray(curBranch->m_rect.mMax, mDim);
            
            SaveRec(curBranch->m_child, a_stream);
        }
    }
    else // A leaf node
    {
        for(int index = 0; index < a_node->m_count; ++index)
        {
            Branch* curBranch = &a_node->m_branch[index];
            
            a_stream.WriteArray(curBranch->m_rect.mMin, mDim);
            a_stream.WriteArray(curBranch->m_rect.mMax, mDim);
            
            a_stream.Write(curBranch->m_data);
        }
    }
    
    return true; // Should do more error checking on I/O operations
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::RemoveAll()
{
    // Delete all existing nodes
    Reset();
    
    m_root = AllocNode();
    m_root->mLevel = 0;
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Reset()
{
#ifdef RTREE_DONT_USE_MEMPOOLS
    // Delete all existing nodes
    RemoveAllRec(m_root);
#else // RTREE_DONT_USE_MEMPOOLS
    // Just reset memory pools.  We are not using complex types
    // EXAMPLE
#endif // RTREE_DONT_USE_MEMPOOLS
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::RemoveAllRec(Node* a_node)
{
    assert(a_node);
    assert(a_node->mLevel >= 0);
    
    if(a_node->IsInternalNode()) // This is an internal node in the tree
    {
        for(int index=0; index < a_node->mCount; ++index)
        {
            RemoveAllRec(a_node->mBranch[index]->mChild);
        }
    }
    FreeNode(a_node);
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
typename RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Node*
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::AllocNode()
{
    Node* newNode;
#ifdef RTREE_DONT_USE_MEMPOOLS
    newNode = new Node(mDim);
#else // RTREE_DONT_USE_MEMPOOLS
    // EXAMPLE
#endif // RTREE_DONT_USE_MEMPOOLS
    InitNode(newNode);
    return newNode;
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::FreeNode(Node* a_node)
{
    assert(a_node);
    
#ifdef RTREE_DONT_USE_MEMPOOLS
    delete a_node;
#else // RTREE_DONT_USE_MEMPOOLS
    // EXAMPLE
#endif // RTREE_DONT_USE_MEMPOOLS
}


// Allocate space for a node in the list used in DeletRect to
// store Nodes that are too empty.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
typename RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::ListNode*
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::AllocListNode()
{
#ifdef RTREE_DONT_USE_MEMPOOLS
    return new ListNode(NULL, NULL);
#else // RTREE_DONT_USE_MEMPOOLS
    // EXAMPLE
#endif // RTREE_DONT_USE_MEMPOOLS
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::FreeListNode(ListNode* a_listNode)
{
#ifdef RTREE_DONT_USE_MEMPOOLS
    delete a_listNode;
#else // RTREE_DONT_USE_MEMPOOLS
    // EXAMPLE
#endif // RTREE_DONT_USE_MEMPOOLS
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::InitNode(Node* a_node)
{
    a_node->mCount = 0;
    a_node->mLevel = -1;
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::InitRect(Rect* a_rect)
{
    for(int index = 0; index < a_rect->mDim; ++index)
    {
        a_rect->mMin[index] = (ELEMTYPE)0;
        a_rect->mMax[index] = (ELEMTYPE)0;
    }
}


// Inserts a new data rectangle into the index structure.
// Recursively descends tree, propagates splits back up.
// Returns 0 if node was not split.  Old node updated.
// If node was split, returns 1 and sets the pointer pointed to by
// new_node to point to the new node.  Old node updated to become one of two.
// The level argument specifies the number of steps up from the leaf
// level to insert; e.g. a data rectangle goes in at level = 0.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::InsertRectRec(Rect* a_rect, const DATATYPE& a_id, Node* a_node, Node** a_newNode, int a_level)
{
    assert(a_rect && a_node && a_newNode);
    assert(a_level >= 0 && a_level <= a_node->mLevel);
    
    int index;
    Branch branch(mDim);
    Node* otherNode;
    
    // Still above level for insertion, go down tree recursively
    if(a_node->mLevel > a_level)
    {
        index = PickBranch(a_rect, a_node);
        if (!InsertRectRec(a_rect, a_id, a_node->mBranch[index]->mChild, &otherNode, a_level))
        {
            // Child was not split
            a_node->mBranch[index]->mRect = CombineRect( a_rect, &(a_node->mBranch[index]->mRect) );
            return false;
        }
        else // Child was split
        {
            a_node->mBranch[index]->mRect = NodeCover(a_node->mBranch[index]->mChild);
            branch.mChild = otherNode;
            branch.mRect = NodeCover(otherNode);
            return AddBranch(&branch, a_node, a_newNode);
        }
    }
    else if(a_node->level() == a_level) // Have reached level for insertion. Add rect, split if necessary
    {
        branch.mRect = *a_rect;
        branch.mChild = (Node*) a_id;
        // Child field of leaves contains id of data record
        return AddBranch(&branch, a_node, a_newNode);
    }
    else
    {
        // Should never occur
        assert(0);
        return false;
    }
}


// Insert a data rectangle into an index structure.
// InsertRect provides for splitting the root;
// returns 1 if root was split, 0 if it was not.
// The level argument specifies the number of steps up from the leaf
// level to insert; e.g. a data rectangle goes in at level = 0.
// InsertRect2 does the recursion.
//
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::InsertRect(Rect* a_rect, const DATATYPE& a_id, Node** a_root, int a_level)
{
    assert(a_rect && a_root);
    assert(a_level >= 0 && a_level <= (*a_root)->level());
    
    Node* newRoot;
    Node* newNode;
    Branch branch(mDim);
    
    if(InsertRectRec(a_rect, a_id, *a_root, &newNode, a_level))  // Root split
    {
        newRoot = AllocNode();  // Grow tree taller and new root
        newRoot->mLevel = (*a_root)->mLevel + 1;
        branch.mRect = NodeCover(*a_root);
        branch.mChild = *a_root;
        AddBranch(&branch, newRoot, NULL);
        branch.mRect = NodeCover(newNode);
        branch.mChild = newNode;
        AddBranch(&branch, newRoot, NULL);
        *a_root = newRoot;
        return true;
    }
    
    return false;
}


// Find the smallest rectangle that includes all rectangles in branches of a node.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
typename RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Rect
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::NodeCover(Node* a_node)
{
    assert(a_node);
    
    int firstTime = true;
    Rect rect(mDim);
    InitRect(&rect);
    
    for(int index = 0; index < a_node->mCount; ++index)
    {
        if(firstTime)
        {
            rect = a_node->mBranch[index]->mRect;
            firstTime = false;
        }
        else
        {
            rect = CombineRect(&rect, &(a_node->mBranch[index]->mRect));
        }
    }
    
    return rect;
}


// Add a branch to a node.  Split the node if necessary.
// Returns 0 if node not split.  Old node updated.
// Returns 1 if node split, sets *new_node to address of new node.
// Old node updated, becomes one of two.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::AddBranch(Branch* a_branch, Node* a_node, Node** a_newNode)
{
    assert(a_branch);
    assert(a_node);
    
    if(a_node->count() < MAXNODES)  // Split won't be necessary
    {
        *(a_node->mBranch[a_node->mCount]) = *a_branch;
        ++a_node->mCount;
        
        return false;
    }
    else
    {
        assert(a_newNode);
        
        SplitNode(a_node, a_branch, a_newNode);
        return true;
    }
}


// Disconnect a dependent node.
// Caller must return (or stop using iteration index) after this as count has changed
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::DisconnectBranch(Node* a_node, int a_index)
{
    assert(a_node && (a_index >= 0) && (a_index < MAXNODES));
    assert(a_node->mCount > 0);
    
    // Remove element by swapping with the last element to prevent gaps in array
    *(a_node->mBranch[a_index]) = *(a_node->mBranch[a_node->mCount - 1]);
    
    --a_node->mCount;
}


// Pick a branch.  Pick the one that will need the smallest increase
// in area to accomodate the new rectangle.  This will result in the
// least total area for the covering rectangles in the current node.
// In case of a tie, pick the one which was smaller before, to get
// the best resolution when searching.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
int
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::PickBranch(Rect* a_rect, Node* a_node)
{
    assert(a_rect && a_node);
    
    bool firstTime = true;
    ELEMTYPEREAL increase;
    ELEMTYPEREAL bestIncr = (ELEMTYPEREAL)-1;
    ELEMTYPEREAL area;
    ELEMTYPEREAL bestArea;
    int best;
    Rect tempRect(mDim);
    
    for(int index=0; index < a_node->count(); ++index)
    {
        Rect* curRect = &( a_node->mBranch[index]->mRect );
        area = CalcRectVolume(curRect);
        tempRect = CombineRect(a_rect, curRect);
        increase = CalcRectVolume(&tempRect) - area;
        if((increase < bestIncr) || firstTime)
        {
            best = index;
            bestArea = area;
            bestIncr = increase;
            firstTime = false;
        }
        else if((increase == bestIncr) && (area < bestArea))
        {
            best = index;
            bestArea = area;
            bestIncr = increase;
        }
    }
    return best;
}


// Combine two rectangles into larger one containing both
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
typename RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Rect
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::CombineRect(Rect* a_rectA, Rect* a_rectB)
{
    assert(a_rectA && a_rectB);
    
    Rect newRect(mDim);
    
    for(int index = 0; index < newRect.mDim; ++index)
    {
        newRect.mMin[index] = std::min(a_rectA->mMin[index], a_rectB->mMin[index]);
        newRect.mMax[index] = std::max(a_rectA->mMax[index], a_rectB->mMax[index]);
    }
    
    return newRect;
}



// Split a node.
// Divides the nodes branches and the extra one between two nodes.
// Old node is one of the new ones, and one really new one is created.
// Tries more than one method for choosing a partition, uses best result.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::SplitNode(Node* a_node, Branch* a_branch, Node** a_newNode)
{
    assert(a_node);
    assert(a_branch);
    
    // Could just use local here, but member or external is faster since it is reused
    PartitionVars localVars(mDim);
    PartitionVars* parVars = &localVars;
    int level;
    
    // Load all the branches into a buffer, initialize old node
    level = a_node->mLevel;
    GetBranches(a_node, a_branch, parVars);
    
    // Find partition
    ChoosePartition(parVars, MINNODES);
    
    // Put branches from buffer into 2 nodes according to chosen partition
    *a_newNode = AllocNode();
    (*a_newNode)->mLevel = a_node->mLevel = level;
    LoadNodes(a_node, *a_newNode, parVars);
    
    assert((a_node->mCount + (*a_newNode)->mCount) == parVars->mTotal);
}


// Calculate the n-dimensional volume of a rectangle
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
ELEMTYPEREAL 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::RectVolume(Rect* a_rect)
{
    assert(a_rect);
    
    ELEMTYPEREAL volume = (ELEMTYPEREAL)1;
    
    for(int index=0; index<a_rect->mDIm; ++index)
    {
        volume *= a_rect->mMax[index] - a_rect->mMin[index];
    }
    
    assert(volume >= (ELEMTYPEREAL)0);
    
    return volume;
}


// The exact volume of the bounding sphere for the given Rect
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
ELEMTYPEREAL 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::RectSphericalVolume(Rect* a_rect)
{
    assert(a_rect);
    
    ELEMTYPEREAL sumOfSquares = (ELEMTYPEREAL)0;
    ELEMTYPEREAL radius;
    
    for(int index=0; index < mDim; ++index) 
    {
        ELEMTYPEREAL halfExtent = ((ELEMTYPEREAL)a_rect->mMax[index] - (ELEMTYPEREAL)a_rect->mMin[index]) * 0.5f;
        sumOfSquares += halfExtent * halfExtent;
    }
    
    
    radius = (ELEMTYPEREAL)sqrt(sumOfSquares);
    
    // Pow maybe slow, so test for common dims like 2,3 and just use x*x, x*x*x.
    if(mDim == 3)
    {
        return (radius * radius * radius * m_unitSphereVolume);
    }
    else if(mDim == 2)
    {
        return (radius * radius * m_unitSphereVolume);
    }
    else
    {
        return (ELEMTYPEREAL)(pow((ELEMTYPEREAL)radius, (ELEMTYPEREAL)mDim) * m_unitSphereVolume);
    }
}


// Use one of the methods to calculate retangle volume
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
ELEMTYPEREAL 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::CalcRectVolume(Rect* a_rect)
{
#ifdef RTREE_USE_SPHERICAL_VOLUME
    return RectSphericalVolume(a_rect); // Slower but helps certain merge cases
#else // RTREE_USE_SPHERICAL_VOLUME
    return RectVolume(a_rect); // Faster but can cause poor merges
#endif // RTREE_USE_SPHERICAL_VOLUME  
}


// Load branch buffer with branches from full node plus the extra branch.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::GetBranches(Node* a_node, Branch* a_branch, PartitionVars* a_parVars)
{
    assert(a_node);
    assert(a_branch);
    
    assert(a_node->mCount == MAXNODES);
    
    // Load the branch buffer
    for(int index=0; index < MAXNODES; ++index)
    {
        *(a_parVars->mBranchBuf[index]) = *(a_node->mBranch[index]);
    }
    *(a_parVars->mBranchBuf[MAXNODES]) = *a_branch;
    a_parVars->mBranchCount = MAXNODES + 1;
    
    // Calculate rect containing all in the set
    a_parVars->mCoverSplit = a_parVars->mBranchBuf[0]->mRect;
    for(int index=1; index < MAXNODES+1; ++index)
    {
        a_parVars->mCoverSplit = CombineRect(&a_parVars->mCoverSplit, &a_parVars->mBranchBuf[index]->mRect);
    }
    a_parVars->mCoverSplitArea = CalcRectVolume(&a_parVars->mCoverSplit);
    
    InitNode(a_node);
}


// Method #0 for choosing a partition:
// As the seeds for the two groups, pick the two rects that would waste the
// most area if covered by a single rectangle, i.e. evidently the worst pair
// to have in the same group.
// Of the remaining, one at a time is chosen to be put in one of the two groups.
// The one chosen is the one with the greatest difference in area expansion
// depending on which group - the rect most strongly attracted to one group
// and repelled from the other.
// If one group gets too full (more would force other group to violate min
// fill requirement) then other group gets the rest.
// These last are the ones that can go in either group most easily.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::ChoosePartition(PartitionVars* a_parVars, int a_minFill)
{
    assert(a_parVars);
    
    ELEMTYPEREAL biggestDiff;
    int group, chosen, betterGroup;
    
    InitParVars(a_parVars, a_parVars->mBranchCount, a_minFill);
    PickSeeds(a_parVars);
    
    while (((a_parVars->mCount[0] + a_parVars->mCount[1]) < a_parVars->mTotal)
           && (a_parVars->mCount[0] < (a_parVars->mTotal - a_parVars->mMinFill))
           && (a_parVars->mCount[1] < (a_parVars->mTotal - a_parVars->mMinFill)))
    {
        biggestDiff = (ELEMTYPEREAL) -1;
        for(int index=0; index<a_parVars->mTotal; ++index)
        {
            if(!a_parVars->mTaken[index])
            {
                Rect* curRect = &a_parVars->mBranchBuf[index]->mRect;
                Rect rect0 = CombineRect(curRect, a_parVars->mCover[0]);
                Rect rect1 = CombineRect(curRect, a_parVars->mCover[1]);
                ELEMTYPEREAL growth0 = CalcRectVolume(&rect0) - a_parVars->mArea[0];
                ELEMTYPEREAL growth1 = CalcRectVolume(&rect1) - a_parVars->mArea[1];
                ELEMTYPEREAL diff = growth1 - growth0;
                if(diff >= 0)
                {
                    group = 0;
                }
                else
                {
                    group = 1;
                    diff = -diff;
                }
                
                if(diff > biggestDiff)
                {
                    biggestDiff = diff;
                    chosen = index;
                    betterGroup = group;
                }
                else if((diff == biggestDiff) && (a_parVars->mCount[group] < a_parVars->mCount[betterGroup]))
                {
                    chosen = index;
                    betterGroup = group;
                }
            }
        }
        Classify(chosen, betterGroup, a_parVars);
    }
    
    // If one group too full, put remaining rects in the other
    if((a_parVars->mCount[0] + a_parVars->mCount[1]) < a_parVars->mTotal)
    {
        if(a_parVars->mCount[0] >= a_parVars->mTotal - a_parVars->mMinFill)
        {
            group = 1;
        }
        else
        {
            group = 0;
        }
        for(int index=0; index<a_parVars->mTotal; ++index)
        {
            if(!a_parVars->mTaken[index])
            {
                Classify(index, group, a_parVars);
            }
        }
    }
    
    assert((a_parVars->mCount[0] + a_parVars->mCount[1]) == a_parVars->mTotal);
    assert((a_parVars->mCount[0] >= a_parVars->mMinFill) && 
           (a_parVars->mCount[1] >= a_parVars->mMinFill));
}


// Copy branches from the buffer into two nodes according to the partition.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::LoadNodes(Node* a_nodeA, Node* a_nodeB, PartitionVars* a_parVars)
{
    assert(a_nodeA);
    assert(a_nodeB);
    assert(a_parVars);
    
    for(int index=0; index < a_parVars->mTotal; ++index)
    {
        assert(a_parVars->mPartition[index] == 0 || a_parVars->mPartition[index] == 1);
        
        if(a_parVars->mPartition[index] == 0)
        {	
            AddBranch(a_parVars->mBranchBuf[index], a_nodeA, NULL);
        }
        else if(a_parVars->mPartition[index] == 1)
        {
            AddBranch(a_parVars->mBranchBuf[index], a_nodeB, NULL);
        }
    }
}


// Initialize a PartitionVars structure.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::InitParVars(PartitionVars* a_parVars, int a_maxRects, int a_minFill)
{
    assert(a_parVars);
    
    a_parVars->mCount[0] = a_parVars->mCount[1] = 0;
    a_parVars->mArea[0] = a_parVars->mArea[1] = (ELEMTYPEREAL)0;
    a_parVars->mTotal = a_maxRects;
    a_parVars->mMinFill = a_minFill;
    for(int index=0; index < a_maxRects; ++index)
    {
        a_parVars->mTaken[index] = false;
        a_parVars->mPartition[index] = -1;
    }
}


template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::PickSeeds(PartitionVars* a_parVars)
{
    int seed0, seed1;
    ELEMTYPEREAL worst, waste;
    ELEMTYPEREAL area[MAXNODES+1];
    
    for(int index=0; index<a_parVars->mTotal; ++index)
    {
        area[index] = CalcRectVolume(&a_parVars->mBranchBuf[index]->mRect);
    }
    
    worst = -a_parVars->mCoverSplitArea - 1;
    for(int indexA=0; indexA < a_parVars->mTotal-1; ++indexA)
    {
        for(int indexB = indexA+1; indexB < a_parVars->mTotal; ++indexB)
        {
            Rect oneRect = CombineRect(&a_parVars->mBranchBuf[indexA]->mRect, &a_parVars->mBranchBuf[indexB]->mRect);
            waste = CalcRectVolume(&oneRect) - area[indexA] - area[indexB];
            if(waste > worst)
            {
                worst = waste;
                seed0 = indexA;
                seed1 = indexB;
            }
        }
    }
    Classify(seed0, 0, a_parVars);
    Classify(seed1, 1, a_parVars);
}


// Put a branch in one of the groups.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Classify(int a_index, int a_group, PartitionVars* a_parVars)
{
    assert(a_parVars);
    assert(!a_parVars->mTaken[a_index]);
    
    a_parVars->mPartition[a_index] = a_group;
    a_parVars->mTaken[a_index] = true;
    
    if (a_parVars->mCount[a_group] == 0)
    {
        *(a_parVars->mCover[a_group]) = a_parVars->mBranchBuf[a_index]->mRect;
    }
    else
    {
        *(a_parVars->mCover[a_group]) = CombineRect(&a_parVars->mBranchBuf[a_index]->mRect, a_parVars->mCover[a_group]);
    }
    
    a_parVars->mArea[a_group] = CalcRectVolume( a_parVars->mCover[a_group] );
    ++a_parVars->mCount[a_group];
}


// Delete a data rectangle from an index structure.
// Pass in a pointer to a Rect, the tid of the record, ptr to ptr to root node.
// Returns 1 if record not found, 0 if success.
// RemoveRect provides for eliminating the root.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::RemoveRect(Rect* a_rect, const DATATYPE& a_id, Node** a_root)
{
    assert(a_rect && a_root);
    assert(*a_root);
    
    Node* tempNode;
    ListNode* reInsertList = NULL;
    
    if(!RemoveRectRec(a_rect, a_id, *a_root, &reInsertList))
    {
        // Found and deleted a data item
        // Reinsert any branches from eliminated nodes
        while(reInsertList)
        {
            tempNode = reInsertList->mNode;
            
            for(int index = 0; index < tempNode->mCount; ++index)
            {
                InsertRect(&(tempNode->mBranch[index]->mRect),
                           tempNode->mBranch[index]->mData,
                           a_root,
                           tempNode->mLevel);
            }
            
            ListNode* remLNode = reInsertList;
            reInsertList = reInsertList->mNext;
            
            FreeNode(remLNode->mNode);
            FreeListNode(remLNode);
        }
        
        // Check for redundant root (not leaf, 1 child) and eliminate
        if((*a_root)->mCount == 1 && (*a_root)->IsInternalNode())
        {
            tempNode = (*a_root)->mBranch[0]->mChild;
            
            assert(tempNode);
            FreeNode(*a_root);
            *a_root = tempNode;
        }
        return false;
    }
    else
    {
        return true;
    }
}


// Delete a rectangle from non-root part of an index structure.
// Called by RemoveRect.  Descends tree recursively,
// merges branches on the way back up.
// Returns 1 if record not found, 0 if success.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::RemoveRectRec(Rect* a_rect, const DATATYPE& a_id, Node* a_node, ListNode** a_listNode)
{
    assert(a_rect && a_node && a_listNode);
    assert(a_node->mLevel >= 0);
    
    if(a_node->IsInternalNode())  // not a leaf node
    {
        for(int index = 0; index < a_node->mCount; ++index)
        {
            if(Overlap(a_rect, &(a_node->mBranch[index]->mRect)))
            {
                if(!RemoveRectRec(a_rect, a_id, a_node->mBranch[index]->mChild, a_listNode))
                {
                    if(a_node->mBranch[index]->mChild->mCount >= MINNODES)
                    {
                        // child removed, just resize parent rect
                        a_node->mBranch[index]->mRect = NodeCover(a_node->mBranch[index]->mChild);
                    }
                    else
                    {
                        // child removed, not enough entries in node, eliminate node
                        ReInsert(a_node->mBranch[index]->mChild, a_listNode);
                        DisconnectBranch(a_node, index); // Must return after this call as count has changed
                    }
                    return false;
                }
            }
        }
        return true;
    }
    else // A leaf node
    {
        for(int index = 0; index < a_node->mCount; ++index)
        {
            if(a_node->mBranch[index]->mChild == (Node*)a_id)
            {
                DisconnectBranch(a_node, index); // Must return after this call as count has changed
                return false;
            }
        }
        return true;
    }
}


// Decide whether two rectangles overlap.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Overlap(Rect* a_rectA, Rect* a_rectB)
{
    assert(a_rectA && a_rectB);
    
    for(int index=0; index < mDim; ++index)
    {
        if (a_rectA->mMin[index] > a_rectB->mMax[index] ||
            a_rectB->mMin[index] > a_rectA->mMax[index])
        {
            return false;
        }
    }
    return true;
    
}


// Add a node to the reinsertion list.  All its branches will later
// be reinserted into the index structure.
template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
void 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::ReInsert(Node* a_node, ListNode** a_listNode)
{
    ListNode* newListNode;
    
    newListNode = AllocListNode();
    newListNode->mNode = a_node;
    newListNode->mNext = *a_listNode;
    *a_listNode = newListNode;
}

template<class DATATYPE, class ELEMTYPE, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
bool 
RTree<DATATYPE, ELEMTYPE, ELEMTYPEREAL, TMAXNODES, TMINNODES>::Search(Node* a_node, Rect* a_rect, std::vector<DATATYPE>& pSearchResults)
{
    assert(a_node);
    assert(a_node->mLevel >= 0);
    assert(a_rect);
    
    if(a_node->IsInternalNode()) // This is an internal node in the tree
    {
        for(int index=0; index < a_node->mCount; ++index)
        {
            if(Overlap(a_rect, &a_node->mBranch[index]->mRect))
            {
                if(!Search(a_node->mBranch[index]->mChild, a_rect, pSearchResults))
                {
                    return false; // Don't continue searching
                }
            }
        }
    }
    else // This is a leaf node
    {
        for(int index=0; index < a_node->mCount; ++index)
        {
            if( Overlap( a_rect, &a_node->mBranch[index]->mRect ) )
            {
                DATATYPE& id = a_node->mBranch[index]->mData;
                
                pSearchResults.push_back(id);
            }
        }
    }
    
    return true; // Continue searching	
}

};

};


#endif

