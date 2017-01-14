//345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "sudoku_solve.h"

//
// routines to update sudoku candidate list
//

bool has_candidates(const Sudoku& s) {

  if (s.num_empty() == 0) return false;

  // for all entries == 0 candidates available, if size of candidate list > 0
  for (int cnt=0; cnt < s.total_size; ++cnt) {
    if (s(cnt) == 0 && s.candidates(cnt).size()==0 ) return false;
  }
  return true;
}

bool has_singles(const Sudoku& s) {

  int no_of_singles=0;

  for (int cnt=0; cnt < s.total_size; ++cnt) {
    if (s(cnt) == 0 && s.candidates(cnt).size() == 1) {
      ++no_of_singles;
    }
  }
  
  return (no_of_singles > 0);
}

void sudoku_update_candidates_cell(Sudoku& s, int cnt) {
  dynamic_assert(s.is_valid_index(cnt),"Index out of range.");
  //
  // if the cell has an entry != 0 the candidate list must be cleared
  //
  // if the cell has an entry == 0 the candidate list must be updated:
  //   each cell entry value != 0 that occurs in a row, col, or block
  //   the cell belongs to has to be removed from the cell's candidate list
  //
  
  if (s(cnt) != 0) {
    if (s.candidates(cnt).size() != 0) s.candidates(cnt).clear();
    return;
  }

  // from here on s(cnt) == 0, i.e. empty cells that should have candidate lists
  
  // remove all values ocurring in curr_row from candidate list
  int curr_row=s.cnt_to_row(cnt).first;
  for (int j=0;j < s.region_size;++j) {
    int value = s.row(curr_row,j);
    if (value > 0) s.candidates(cnt).remove(value);
  }

  // remove all values ocurring in curr_col from candidate list
  int curr_col=s.cnt_to_col(cnt).first;
  for (int j=0;j < s.region_size;++j) {
    int value = s.col(curr_col,j);
    if (value > 0) s.candidates(cnt).remove(value);
  }
  
  // remove all values ocurring in curr_block from candidate list
  int curr_block=s.cnt_to_block(cnt).first;
  for (int j=0;j < s.region_size;++j) {
    int value = s.block(curr_block,j);
    if (value > 0) s.candidates(cnt).remove(value);
  }
}


void sudoku_update_candidates_affected_by_cell(Sudoku& s, int cnt) {
  dynamic_assert(s.is_valid_index(cnt),"Index out of range.");
  //
  // update all candidate lists of cells in the regions the cell(cnt) belongs to
  //
  
  // update candidate lists of all cells in curr_row
  int curr_row=s.cnt_to_row(cnt).first;
  for (int j=0;j < s.region_size;++j) {
    sudoku_update_candidates_cell(s, s.row_to_cnt(curr_row,j));
  }

  // update candidate lists of all cells in curr_col
  int curr_col=s.cnt_to_col(cnt).first;
  for (int j=0;j < s.region_size;++j) {
    sudoku_update_candidates_cell(s, s.col_to_cnt(curr_col,j));
  }

  // update candidate lists of all cells in curr_block
  int curr_block=s.cnt_to_block(cnt).first;
  for (int j=0;j < s.region_size;++j) {
    sudoku_update_candidates_cell(s, s.block_to_cnt(curr_block,j));
  }
}


void sudoku_update_candidates_all_cells(Sudoku& s) {
  for (int cnt=0;cnt < s.total_size; ++cnt) {
    sudoku_update_candidates_cell(s, cnt);
  }
}


int sudoku_remove_singles(Sudoku& s) {  // return no. of removed singles
  
  int no_removed=0;

  for (int cnt=0; cnt < s.total_size; ++cnt) {
    if (s(cnt) == 0 && s.candidates(cnt).size() == 1) {
      s(cnt)=s.candidates(cnt).front();
      ++no_removed;
      sudoku_update_candidates_affected_by_cell(s, cnt);
    }
  }
  
  return no_removed;
}
