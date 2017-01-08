// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include "sudoku_class.h"

#include <iostream>
#include <iomanip>          // setw()
#include <string>
#include <utility>          // pair<T,T>().first/.second

using namespace std;

//
//  printing routines for class sudoku
//  (made friend of Sudoku in Sudoku_class.h)
//

void sudoku_print(const Sudoku& s, const string msg);
void sudoku_print_regions(const Sudoku& s, const string msg);
void sudoku_print_cnt_to_x(const Sudoku& s, const string msg);
void sudoku_print_candidates(const Sudoku& s, const string msg);
