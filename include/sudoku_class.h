// let emacs know this is a C++ header: -*- C++ -*-
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <utility>          // pair<T,T>(), make_pair()
#include "dyn_assert.h"     // dynamic_assert()


struct Sudoku_cell {
  int val {0};             // entry value 0: empty indicator; 1..N for set value
  int r;                   // row index the cell belongs to
  int c;                   // col index the cell belongs to
  int b;                   // block index the cell belongs to
  std::list<int> cand {};  // list of remaining candidates for this cell (=permissible entries)
  std::list<int> rqrd {};  // list of required entries for this cell
};

//
// interface class Sudoku
//
// Sudoku keeps track of data structure to represent a sudoku of arbitrary size
//

class Sudoku;  // forward declaration for access class

enum class Region {row, col, block}; // enum of region types

// access classes of Sudoku for various access schemes (row, col, block)
class Access {
  
  Sudoku& m_s;                                 // reference to Sudoku instance
  std::vector<std::vector<Sudoku_cell*>> m_rv; // region vectors (row/col/block)
  const Region m_region;                       // region type for this instance

public:
  Access(Sudoku& t_ref, const Region t_region);
  
  // to be called AFTER Sudoku entries are set up in Sudoku constructor
  // in order to set up access references depending on region type
  void init_access();
  
  // access to region(cnt)
  vector<Sudoku_cell*>& operator()(int cnt);
  const vector<Sudoku_cell*>& operator()(int cnt) const;
  // element access at region(i,j)
  Sudoku_cell& operator()(int i, int j);
  const Sudoku_cell& operator()(int i, int j) const;
};


class Sudoku {

  // access to regions
  friend class Access;

  std::vector<Sudoku_cell> m_cell;            // contains Sudoku entries

public:
  
  const int region_size;    // no. of cells per region (= row / col / block)
  const int blocks_per_row; // no. of blocks in "x-direction"
  const int blocks_per_col; // no. of blocks in "y-direction"
  const int total_size;     // total size of sudoku = region_size*region_size

  // constructors
  Sudoku(int t_region_size, int t_blocks_per_row, int t_blocks_per_col);
  Sudoku(const Sudoku& other_Sudoku); 

  // assignment
  Sudoku& operator=(const Sudoku&);

  // element access
  Sudoku_cell& operator()(int cnt);
  const Sudoku_cell& operator()(int cnt) const;
  Sudoku_cell& operator()(int i, int j);
  const Sudoku_cell& operator()(int i, int j) const;
  
  // provide cell access in various forms for regions
  // (all addressing the same memory)
  Access   row;
  Access   col;
  Access block;

  // access by index (this is where the mapping happens)
  int row_to_cnt(int i,int j) const;
  int col_to_cnt(int i,int j) const;
  int block_to_cnt(int i, int j) const;
  int region_to_cnt(const Region region, int i,int j) const;
  
  std::pair<int,int> cnt_to_row(int cnt) const;
  std::pair<int,int> cnt_to_col(int cnt) const;
  std::pair<int,int> cnt_to_block(int cnt) const;
  std::pair<int,int> cnt_to_region(const Region region, int cnt) const;

  bool cell_is_in_affected_regions(int curr_block, int cnt);

  // helpers for checking index values
  bool is_valid_index(int cnt) const;
  bool is_valid_region_index(int cnt) const;
  bool is_valid_region_index(int i, int j) const;

};
