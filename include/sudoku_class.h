// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <iostream>
#include <new>
#include <string>
#include <list>
#include <vector>
#include <utility>          // pair<T,T>(), make_pair()
#include "dyn_assert.h"     // dynamic_assert()

using namespace std;

//
// interface class Sudoku
//
// Sudoku keeps track of data structure to represent and solve a sudoku of arbitrary size
//

class Sudoku;  // forward declaration for access classes


// access classes of Sudoku for various access schemes (row, col, block)
class Row_access {
  Sudoku& ref;
public:
  Row_access(Sudoku& _ref);
  // direct element access
  int& operator()(int i, int j);
  const int& operator()(int i, int j) const;
};

class Col_access {
  Sudoku& ref;
public:
  Col_access(Sudoku& _ref);
  // direct element access
  int& operator()(int i, int j);
  const int& operator()(int i, int j) const;
};

class Block_access {
  Sudoku& ref;
public:
  Block_access(Sudoku& _ref);
  // direct element access
  int& operator()(int i, int j);
  const int& operator()(int i, int j) const;
};


class Sudoku {

  // access to regions
  friend class Row_access;
  friend class Col_access;
  friend class Block_access;

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

  // access to candidate lists
  list<int>& candidates(int cnt);
  const list<int>& candidates(int cnt) const;

  list<int>& candidates(int i, int j);
  const list<int>& candidates(int i, int j) const;
  
  int row_to_cnt(int i,int j) const;
  int col_to_cnt(int i,int j) const;
  int block_to_cnt(int i, int j) const;
  pair<int,int> cnt_to_row(int cnt) const;
  pair<int,int> cnt_to_col(int cnt) const;
  pair<int,int> cnt_to_block(int cnt) const;

  // helpers for checking 
  bool is_valid_index(int cnt) const;
  bool is_valid_index(int i, int j) const;

  // constructors / destructors
  Sudoku(int _region_size, int _blocks_per_row, int _blocks_per_col);
  Sudoku(const Sudoku& other_Sudoku); 
  ~Sudoku();

  // assignment
  Sudoku& operator=(const Sudoku&);

  // element access
  int& operator()(int cnt);
  int operator()(int cnt) const;
  int& operator()(int i, int j);
  int operator()(int i, int j) const;

  // validation
  bool is_valid() const;    // check for valid unique entries
  int num_entries() const;  // return no. of entries != 0 (non empty entries)
  int num_empty() const;    // return no. of entries == 0 (empty entries)
  
private:
  int* f;                    // contains Sudoku entries
  vector<list<int>> cand;    // contains list of candidates for each cell
};