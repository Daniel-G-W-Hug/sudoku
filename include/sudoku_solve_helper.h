// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <list>
#include <algorithm>

using namespace std;

//
// helper routines provinding general functionality
//

// remove all values in list remove_values from list remove_target
void remove_from_list_int(list<int>& remove_target,
			  const list<int>& remove_values);
