//345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "sudoku_solve_helper.h"


void remove_from_list_int(list<int>& remove_target,
			  const list<int>& remove_values) {

  // remove all values provided in list remove_values from remove_target
  for (auto vp=remove_values.begin(); vp != remove_values.end();++vp) {
    remove_target.remove(*vp);
  }
  
}
