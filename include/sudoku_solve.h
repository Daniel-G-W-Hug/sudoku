// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <algorithm>
#include "sudoku_class.h"
#include "dyn_assert.h"
#include "sudoku_print.h"  // for debugging only
#include "sudoku_solve_helper.h"

using namespace std;

//
//  routines to solve Sudoku
//

int  sudoku_num_candidates(const Sudoku& s);
bool sudoku_has_candidates(const Sudoku& s);
// true, if for entries == 0 there are still candidates available

int  sudoku_num_singles(const Sudoku& s);
bool sudoku_has_singles(const Sudoku& s);

int  sudoku_num_naked_twins(const Sudoku& s);
bool sudoku_has_naked_twins(const Sudoku& s);

// update candidate lists
void sudoku_update_candidates_cell(Sudoku& s, int cnt);
void sudoku_update_candidates_affected_by_cell(Sudoku& s, int cnt);
void sudoku_update_candidates_all_cells(Sudoku& s);

// solution steps (return no. of removed singles)
// acutally removes singles by filling candidate value into cell
int sudoku_remove_singles(Sudoku& s);

// solution steps (return no. of removed naked twins)
// just reduces candidate lists of cells in region, does not fill in cells
int sudoku_remove_naked_twins(Sudoku& s);
