// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <iostream> //TODO: error handling in separate unit (dynamic_assert)
#include <new>
#include <string>
#include <list>
#include <vector>
#include <exception>        // terminate()
#include <utility>          // pair<T,T>(), make_pair(), p

using namespace std;

//
// interface class Sudoku
//
// Sudoku keeps track of data structure to represent and solve a sudoku of arbitrary size
//

class Sudoku;  // forward declaration for access classes


// access classes of Sudoku for various access schemes (row, col, block)
class Row_access {
  const Sudoku& ref;
public:
  Row_access(const Sudoku& _ref);    
  int& operator()(int i, int j);
  int operator()(int i, int j) const;
};

class Col_access {
  const Sudoku& ref;
public:
  Col_access(const Sudoku& _ref);
  int& operator()(int i, int j);
  int operator()(int i, int j) const;
};

class Block_access {
  const Sudoku& ref;
public:
  Block_access(const Sudoku& _ref);
  int& operator()(int i, int j);
  int operator()(int i, int j) const;
};



class Sudoku {

  // access regions
  friend class Row_access;
  friend class Col_access;
  friend class Block_access;

  // printing
  friend void sudoku_print(const Sudoku&, const string);
  friend void sudoku_print_regions(const Sudoku&, const string);
  friend void sudoku_print_cnt_to_x(const Sudoku&, const string);
  friend void sudoku_print_candidates(const Sudoku&, const string);
  
public:
  
  const int region_size;    // no. of cells per region (= row / col / block)
  const int blocks_per_row; // no. of blocks in "x-direction"
  const int blocks_per_col; // no. of blocks in "y-direction"
  const int total_size;     // total size of sudoku = region_size*region_size

  // provide cell access in various forms for regions
  // (all addressing the same memory)
  Row_access   row;
  Col_access   col;
  Block_access block;

  int row_to_cnt(int i,int j) const;
  int col_to_cnt(int i,int j) const;
  int block_to_cnt(int i, int j) const;
  pair<int,int> cnt_to_row(int cnt) const;
  pair<int,int> cnt_to_col(int cnt) const;
  pair<int,int> cnt_to_block(int cnt) const;

  // helpers for checking 
  bool is_valid_index(int cnt) const;
  bool is_valid_index(int i, int j) const;
  void dynamic_assert(bool assertion, string message) const;

  // constructors / destructors
  Sudoku(int _region_size, int _blocks_per_row, int _blocks_per_col);
  Sudoku(const Sudoku& other_Sudoku); 
  ~Sudoku();

  // assignment
  Sudoku& operator=(const Sudoku&);

  // element access
  int& operator()(int i);
  int operator()(int i) const;
  int& operator()(int i, int j);
  int operator()(int i, int j) const;

  // validation
  bool is_valid() const;
  
private:
  int* f;                        // contains Sudoku entries
  vector<list<int>> candidates;  // contains list of candidates for each cell
};
