//345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "sudoku_solve.h"

//
// routines to update sudoku candidate list
//


int sudoku_num_candidates(const Sudoku& s) {

  int no_of_candidates=0;
  
  if (s.num_empty() == 0) return no_of_candidates;

  // for all entries == 0 candidates available, if size of candidate list > 0
  for (int cnt=0; cnt < s.total_size; ++cnt) {
    if (s(cnt) == 0 && s.candidates(cnt).size()==0 ) return no_of_candidates;
  }

  // if arrived here, there are still candidates available
  for (int cnt=0; cnt < s.total_size; ++cnt) {
    if (s(cnt) == 0) no_of_candidates += s.candidates(cnt).size();
  }
  
  return no_of_candidates;
}


bool sudoku_has_candidates(const Sudoku& s) {
  return sudoku_num_candidates(s) > 0;
}


int sudoku_num_singles(const Sudoku& s) {

  int no_of_singles=0;

  for (int cnt=0; cnt < s.total_size; ++cnt) {
    if (s(cnt) == 0 && s.candidates(cnt).size() == 1) {
      ++no_of_singles;
    }
  }
  
  return no_of_singles;
}

bool sudoku_has_singles(const Sudoku& s) {
  return sudoku_num_singles(s) > 0;
}

int sudoku_num_naked_twins_in_region(const Sudoku& s, int region) {
  //
  // naked twin: Candidate lists of two cells in a region have the same two
  //             entries. Since these two cells have those values,
  //             The candidate values can be removed in all other candidate
  //             lists of this region.
  //

  int no_of_identified_naked_twins=0;
  
  for (int i=0;i < s.region_size; ++i) {

    for (int j=0;j< s.region_size; ++j) {     
      int cnt = s.region_to_cnt(region, i,j);
      if (s(cnt) == 0 && s.candidates(cnt).size() == 2) {
	// potential twin candidate found
	for (int k=j+1;k < s.region_size; k++) {
	  int cnt_k = s.region_to_cnt(region, i,k);
	  if (s(cnt_k) == 0 && s.candidates(cnt_k).size() == 2 &&
	      s.candidates(cnt) == s.candidates(cnt_k)) {
	    // twin found
	    ++no_of_identified_naked_twins;
	  }
	}
      }
    }
  }
  return no_of_identified_naked_twins;
}

int sudoku_num_naked_twins(const Sudoku& s) {

  int no_of_naked_twins=0;

  no_of_naked_twins += sudoku_num_naked_twins_in_region(s, 0);
  //cout << "no. of naked twins (after rows): " << no_of_naked_twins << "\n";

  no_of_naked_twins += sudoku_num_naked_twins_in_region(s, 1);
  //cout << "no. of naked twins (after cols): " << no_of_naked_twins << "\n";

  no_of_naked_twins += sudoku_num_naked_twins_in_region(s, 2);
  //cout << "no. of naked twins (after blocks): " << no_of_naked_twins << "\n";
  
  return no_of_naked_twins;  
}


bool sudoku_has_naked_twins(const Sudoku& s) {
  return sudoku_num_naked_twins(s) > 0;
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


int sudoku_remove_naked_twins_in_region(Sudoku& s, int region) {
  //
  // naked twin: Candidate lists of two cells in a region have the same two
  //             entries. Since these two cells have those values,
  //             The candidate values can be removed in all other candidate
  //             lists of this region.
  //
  
  int no_of_naked_twins_removed_in_region=0;

  vector<list<int>> twin_values;
  vector<list<int>> twin_pos;
  
  for (int i=0;i < s.region_size; ++i) {

    twin_values.clear();
    twin_pos.clear();
    
    for (int j=0;j < s.region_size; ++j) {     
      int cnt = s.region_to_cnt(region, i,j);
      if (s(cnt) == 0 && s.candidates(cnt).size() == 2) {
	// potential twin candidate found
	for (int k=j+1;k < s.region_size; ++k) {
	  int cnt_k = s.region_to_cnt(region, i,k);
	  if (s(cnt_k) == 0 && s.candidates(cnt_k).size() == 2 &&
	      s.candidates(cnt) == s.candidates(cnt_k)) {
	    // twin found
	    twin_values.push_back(s.candidates(cnt));
	    list<int> lh;
	    lh.push_back(j);
	    lh.push_back(k);
	    twin_pos.push_back(lh);
	  }
	}
      }
    }

    // remove twins from candidate list of other cells
    for (size_t t_cnt=0;t_cnt < twin_values.size();++t_cnt) {
	
      for (int j=0; j < s.region_size; ++j) {
	int cnt = s.region_to_cnt(region, i,j);
	if (s(cnt) == 0 &&
	    none_of(twin_pos[t_cnt].begin(),twin_pos[t_cnt].end(),
		    [j](int i){return i==j;}) ) {
	  // only remove if other cell in region
	  remove_from_list_int(s.candidates(cnt),twin_values[t_cnt]);
	}
      }
      
      ++no_of_naked_twins_removed_in_region;
    }
    
  }
  
  return no_of_naked_twins_removed_in_region;
}


int sudoku_remove_naked_twins(Sudoku& s) {


  int no_of_naked_twins_removed=0;

  no_of_naked_twins_removed += sudoku_remove_naked_twins_in_region(s, 0);
  // cout << "no. of naked twins removed (after rows): ";
  // cout << no_of_naked_twins_removed << "\n";

  no_of_naked_twins_removed += sudoku_remove_naked_twins_in_region(s, 1);
  // cout << "no. of naked twins removed (after cols): ";
  // cout << no_of_naked_twins_removed << "\n";

  no_of_naked_twins_removed += sudoku_remove_naked_twins_in_region(s, 2);
  // cout << "no. of naked twins removed (after blocks): ";
  // cout << no_of_naked_twins_removed << "\n";

  return no_of_naked_twins_removed;  
}
