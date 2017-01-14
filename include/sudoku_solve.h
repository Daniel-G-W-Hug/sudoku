// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include "sudoku_class.h"
#include "dyn_assert.h"

using namespace std;

//
//  routines to solve Sudoku
//

bool has_candidates(const Sudoku& s);
// true, if for all entries == 0 still candidates

bool has_singles(const Sudoku& s);
// true, if entries == 0 and candidate lists with only one entry


// update candidate lists
void sudoku_update_candidates_cell(Sudoku& s, int cnt);
void sudoku_update_candidates_affected_by_cell(Sudoku& s, int cnt);
void sudoku_update_candidates_all_cells(Sudoku& s);

// solution steps
int sudoku_remove_singles(Sudoku& s); // return no. of removed singles
