// let emacs know this is a C++ header: -*- C++ -*-
// 345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <list>
#include <set>
#include <tuple>
#include <vector>
#include <iterator> // prev(), next()

#include "sudoku_class.h"

//
// helper routines provinding general or recurring functionality for
// "sudoku_solve.cpp"
//

// check for valid and unique entries in region
bool sudoku_has_unique_entries_in_region(const Sudoku &s,
                                         const Region_t region);

// concatenate candidate sets in subregion
std::multiset<int> sudoku_concatenate_candidate_sets_in_subregion(
    const Sudoku &s, const Region_t region, const int subregion);

// count how often which sudoku entry occurs in list
std::vector<int>
sudoku_count_candidate_entries(const Sudoku &s,
                               const std::multiset<int> &input_multiset);

// store index where elements occur in current subregion
std::vector<list<int>> sudoku_candidate_positions_in_subregion(
    const Sudoku &s, const Region_t region, const int subregion,
    const std::vector<int> &candidate_count);

// identify hidden singles in subregion
std::tuple<int, std::vector<int>, std::vector<int>>
identify_hidden_singles_in_subregion(const Sudoku &s, const Region_t region,
                                     const int subregion);

// identify naked twins in subregion
std::tuple<int, std::vector<std::list<int>>, std::vector<std::list<int>>>
identify_naked_twins_in_subregion(const Sudoku &s, const Region_t region,
                                  const int subregion);

// identify hidden twins in subregion
std::tuple<int, std::vector<std::list<int>>, std::vector<std::list<int>>>
identify_hidden_twins_in_subregion(const Sudoku &s, const Region_t region,
                                   const int subregion);
