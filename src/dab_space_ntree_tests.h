/** \file dab_space_ntree_tests.h
*/

#ifndef _dab_space_ntree_tests_h_
#define _dab_space_ntree_tests_h_

#include <iostream>

#include "dab_space_ntree_tests.h"
#include "dab_exception.h"
#include "dab_singleton.h"
#include "dab_space_object.h"

namespace dab
{

namespace space
{
    
class NtreeTests : public dab::Singleton<NtreeTests>
{
public:
    void runTests();
    void test() throw (dab::Exception);
};
    
};
    
};

#endif
