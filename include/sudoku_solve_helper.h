// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <vector>
#include <list>
#include <tuple>

#include "sudoku_class.h"

//
// helper routines provinding general or recurring functionality for "sudoku_solve.cpp"
//

// remove all values in list remove_values from list remove_target
void remove_from_list_int(std::list<int>& remove_target,
			  const std::list<int>& remove_values);

// check for valid and unique entries in region
bool sudoku_has_unique_entries_in_region(const Sudoku& s, const Region region);

// concatenate candidate lists in subregion
std::list<int> sudoku_concatenate_candidate_lists_in_subregion(const Sudoku& s,
							       const Region region,
							       const int subregion);

// count how often which sudoku entry occurs in list
std::vector<int> sudoku_count_candidate_entries(const Sudoku& s, const std::list<int>& input_list);

// store index where elements occur in current subregion
std::vector<list<int>> sudoku_candidate_positions_in_subregion(
                       const Sudoku& s,
                       const Region region,
		       const int subregion,
		       const vector<int>& candidate_count);

// identify hidden twins in subregion
tuple< int, vector<int>, vector<int> >
      identify_hidden_singles_in_subregion(const Sudoku& s,
                                           const Region region,
                                           const int subregion);


// identify naked twins in subregion
std::tuple< int, std::vector<std::list<int>>, std::vector<std::list<int>> >
           identify_naked_twins_in_subregion(
                                             const Sudoku& s,
                                             const Region region,
                                             const int subregion);


// identify hidden twins in subregion
std::tuple< int, std::vector<std::list<int>>, std::vector<std::list<int>> >
           identify_hidden_twins_in_subregion(
                                              const Sudoku& s,
                                              const Region region,
                                              const int subregion);
