// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <iostream>
#include <iomanip>          // setw()
#include <string>
#include <utility>          // pair<T,T>().first/.second

#include "sudoku_class.h"

//
//  printing routines for class sudoku
//  (made friend of Sudoku in Sudoku_class.h)
//

void sudoku_print(const Sudoku& s, const std::string msg);
void sudoku_print_regions(const Sudoku& s, const std::string msg);
void sudoku_print_cnt_to_x(const Sudoku& s, const std::string msg);
void sudoku_print_candidates(const Sudoku& s, const std::string msg, const Region region = Region::row);
void sudoku_print_required(const Sudoku& s, const std::string msg, const Region region = Region::row);

//
// for debugging
//

void print_vector_int(const std::vector<int>& v);
void print_vector_list_int(const std::vector<std::list<int>>& v);

void print_list_int(const std::list<int>& l);
void print_list_list_int(const std::list<std::list<int>>& ll);
