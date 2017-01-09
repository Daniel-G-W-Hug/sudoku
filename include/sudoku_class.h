// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <utility>          // pair<T,T>(), make_pair()
#include "dyn_assert.h"     // dynamic_assert()

using namespace std;

//
// interface class Sudoku
//
// Sudoku keeps track of data structure to represent a sudoku of arbitrary size
//

class Sudoku;  // forward declaration for access classes


// access classes of Sudoku for various access schemes (row, col, block)
class Row_access {
  Sudoku& ref;
  vector<vector<int*>> r;  // vector of row vectors
public:
  Row_access(Sudoku& _ref);
  void init_row_access();  // to be called AFTER Sudoku entries are set up
  // access to row(cnt)
  vector<int*>& operator()(int cnt);
  const vector<int*>& operator()(int cnt) const;
  // element access at row(i,j)
  int& operator()(int i, int j);
  const int& operator()(int i, int j) const;
};

class Col_access {
  Sudoku& ref;
  vector<vector<int*>> c;
public:
  Col_access(Sudoku& _ref);
  void init_col_access();  // to be called AFTER Sudoku entries are set up
  // access to col(cnt)
  vector<int*>& operator()(int cnt);
  const vector<int*>& operator()(int cnt) const;
  // element access at col(i,j)
  int& operator()(int i, int j);
  const int& operator()(int i, int j) const;
};

class Block_access {
  Sudoku& ref;
  vector<vector<int*>> b;
public:
  Block_access(Sudoku& _ref);
  void init_block_access();  // to be called AFTER Sudoku entries are set up
  // access to block(cnt)
  vector<int*>& operator()(int cnt);
  const vector<int*>& operator()(int cnt) const;
  // element access at block(i,j)
  int& operator()(int i, int j);
  const int& operator()(int i, int j) const;
};


class Sudoku {

  // access to regions
  friend class Row_access;
  friend class Col_access;
  friend class Block_access;

  vector<int> f;            // contains Sudoku entries
  vector<list<int>> cand;   // contains list of candidates for each cell

public:
  
  const int region_size;    // no. of cells per region (= row / col / block)
  const int blocks_per_row; // no. of blocks in "x-direction"
  const int blocks_per_col; // no. of blocks in "y-direction"
  const int total_size;     // total size of sudoku = region_size*region_size

  // constructors
  Sudoku(int _region_size, int _blocks_per_row, int _blocks_per_col);
  Sudoku(const Sudoku& other_Sudoku); 

  // assignment
  Sudoku& operator=(const Sudoku&);

  // element access
  int& operator()(int cnt);
  const int& operator()(int cnt) const;
  int& operator()(int i, int j);
  const int& operator()(int i, int j) const;
  
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

  // update candidate lists
  // void remove_candidate_from_list_of_cell(int cnt, int value);
  void update_candidate_list_of_cell(int cnt);
  void update_candidate_list_of_all_cells();
  
  int row_to_cnt(int i,int j) const;
  int col_to_cnt(int i,int j) const;
  int block_to_cnt(int i, int j) const;
  pair<int,int> cnt_to_row(int cnt) const;
  pair<int,int> cnt_to_col(int cnt) const;
  pair<int,int> cnt_to_block(int cnt) const;

  // helpers for checking 
  bool is_valid_index(int cnt) const;
  bool is_valid_region_index(int cnt) const;
  bool is_valid_region_index(int i, int j) const;

  // validation
  bool is_valid() const;    // check for valid and unique entries in each region
  int num_entries() const;  // return no. of entries != 0 (non empty entries)
  int num_empty() const;    // return no. of entries == 0 (empty entries)

};
