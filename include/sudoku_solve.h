// let emacs know this is a C++ header: -*- C++ -*-
// 345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include "sudoku_class.h"

//
//  routines to solve Sudoku
//

// update candidate lists
void sudoku_update_candidates_cell(Sudoku &s, int cnt);
void sudoku_update_candidates_affected_by_cell(Sudoku &s, int cnt);
void sudoku_update_candidates_all_cells(Sudoku &s);

// validation of sudoku
bool sudoku_is_valid(
    const Sudoku &s); // valid and unique entries in each region
int sudoku_num_entries(
    const Sudoku &s); // return no. of entries != 0 (non empty entries)
int sudoku_num_empty(
    const Sudoku &s); // return no. of entries == 0 (empty entries)

// true, if for entries == 0 there are still candidates available
int sudoku_num_candidates(const Sudoku &s);
bool sudoku_has_candidates(const Sudoku &s);

int sudoku_num_naked_singles(const Sudoku &s);
bool sudoku_has_naked_singles(const Sudoku &s);

// identify hidden singles
int num_hidden_singles_in_region(const Sudoku &s, const Region_t region);
int sudoku_num_hidden_singles(const Sudoku &s);
bool sudoku_has_hidden_singles(const Sudoku &s);

int sudoku_num_naked_twins_in_region(const Sudoku &s, const Region_t region);
int sudoku_num_naked_twins(const Sudoku &s);
bool sudoku_has_naked_twins(const Sudoku &s);

int sudoku_num_hidden_twins_in_region(const Sudoku &s, const Region_t region);
int sudoku_num_hidden_twins(const Sudoku &s);
bool sudoku_has_hidden_twins(const Sudoku &s);

// solution steps (return no. of removed singles)
// acutally removes naked singles by filling candidate value into cell
int sudoku_remove_naked_singles(Sudoku &s);

// removes hidden singles by filling candidate value into cell
int sudoku_remove_hidden_singles_in_region(Sudoku &s, const Region_t region);
int sudoku_remove_hidden_singles(Sudoku &s);

// solution steps (return no. of removed naked twins)
// just reduces candidate lists of cells in region, does not fill in cells
int sudoku_remove_naked_twins_in_region(Sudoku &s, const Region_t region);
int sudoku_remove_naked_twins(Sudoku &s);

// solution steps (return no. of removed hidden twins)
// just reduces candidate lists of cells in region, does not fill in cells
int sudoku_remove_hidden_twins_in_region(Sudoku &s, const Region_t region);
int sudoku_remove_hidden_twins(Sudoku &s);
