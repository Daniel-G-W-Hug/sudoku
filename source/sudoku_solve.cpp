// 345678901234567890123456789012345678901234567890123456789012345678901234567890
#include <algorithm>
#include <tuple>

#include "dyn_assert.h"
#include "sudoku_print.h" // for debugging only
#include "sudoku_solve.h"
#include "sudoku_solve_helper.h"

using namespace std;

//
// routines to update sudoku candidate list
//

void sudoku_update_candidates_cell(Sudoku &s, int cnt) {
  dynamic_assert(s.is_valid_index(cnt), "Index out of range.");
  //
  // if the cell has an entry != 0 the candidate list must be cleared
  //
  // if the cell has an entry == 0 the candidate list must be updated:
  //   each cell entry value != 0 that occurs in a row, col, or block
  //   the cell belongs to has to be removed from the cell's candidate list
  //

  if (s(cnt).val != 0) {
    if (s(cnt).cand.size() != 0)
      s(cnt).cand.clear();
    return;
  }

  // from here on s(cnt) == 0, i.e. empty cells that should have candidate lists

  // remove all values ocurring in curr_row from candidate list
  int curr_row = s(cnt).ri;
  for (int j = 0; j < s.region_size; ++j) {
    int value = s.row(curr_row, j).val;
    if (value > 0)
      s(cnt).cand.remove(value);
  }

  // remove all values ocurring in curr_col from candidate list
  int curr_col = s(cnt).ci;
  for (int j = 0; j < s.region_size; ++j) {
    int value = s.col(curr_col, j).val;
    if (value > 0)
      s(cnt).cand.remove(value);
  }

  // remove all values ocurring in curr_block from candidate list
  int curr_block = s(cnt).bi;
  for (int j = 0; j < s.region_size; ++j) {
    int value = s.block(curr_block, j).val;
    if (value > 0)
      s(cnt).cand.remove(value);
  }
}

void sudoku_update_candidates_affected_by_cell(Sudoku &s, int cnt) {
  dynamic_assert(s.is_valid_index(cnt), "Index out of range.");
  //
  // update all candidate lists of cells in the regions the cell(cnt) belongs to
  //

  // update candidate lists of all cells in curr_row
  int curr_row = s(cnt).ri;
  for (int j = 0; j < s.region_size; ++j) {
    sudoku_update_candidates_cell(s, s.row_to_cnt(curr_row, j));
  }

  // update candidate lists of all cells in curr_col
  int curr_col = s(cnt).ci;
  for (int j = 0; j < s.region_size; ++j) {
    sudoku_update_candidates_cell(s, s.col_to_cnt(curr_col, j));
  }

  // update candidate lists of all cells in curr_block
  int curr_block = s(cnt).bi;
  for (int j = 0; j < s.region_size; ++j) {
    sudoku_update_candidates_cell(s, s.block_to_cnt(curr_block, j));
  }
}

void sudoku_update_candidates_all_cells(Sudoku &s) {
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    sudoku_update_candidates_cell(s, cnt);
  }
}

bool sudoku_is_valid(const Sudoku &s) {
  // check for valid entry values:
  // 0 (=empty indicator, is valid too), valid entries: 1..region_size
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val < 0 || s(cnt).val > s.region_size) {
      return false;
    }
  }

  // check for valid and unique entries in regions
  if (!sudoku_has_unique_entries_in_region(s, Region_t::row))
    return false;
  if (!sudoku_has_unique_entries_in_region(s, Region_t::col))
    return false;
  if (!sudoku_has_unique_entries_in_region(s, Region_t::block))
    return false;

  return true;
}

int sudoku_num_entries(const Sudoku &s) {
  // return no. of entries > 0
  int count = 0;
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val > 0)
      ++count;
  }

  return count;
}

int sudoku_num_empty(const Sudoku &s) {
  // return no. of empty entries, i.e. entries with value 0
  return s.total_size - sudoku_num_entries(s);
}

int sudoku_num_candidates(const Sudoku &s) {
  int num_candidates = 0;

  if (sudoku_num_empty(s) == 0)
    return num_candidates;

  // for all entries == 0 candidates available, if size of candidate list > 0
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val == 0 && s(cnt).cand.size() == 0)
      return num_candidates;
  }

  // if arrived here, there are still candidates available
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val == 0)
      num_candidates += s(cnt).cand.size();
  }

  return num_candidates;
}

bool sudoku_has_candidates(const Sudoku &s) {
  return sudoku_num_candidates(s) > 0;
}

int sudoku_num_naked_singles(const Sudoku &s) {
  int num_naked_singles = 0;

  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val == 0 && s(cnt).cand.size() == 1) {
      ++num_naked_singles;
    }
  }

  return num_naked_singles;
}

bool sudoku_has_naked_singles(const Sudoku &s) {
  return sudoku_num_naked_singles(s) > 0;
}

int num_hidden_singles_in_region(const Sudoku &s, const Region_t region) {
  // hidden single: candidate lists in a region contain a value only once
  //                this is a hidden single, since each region must have
  //                all permissible values

  list<int> lh{};           // helper list to count candidates
  vector<int> cand_count{}; // candidate occurrance count

  int num_hidden_singles_in_region = 0;
  int count = 0;

  for (int i = 0; i < s.region_size; ++i) { // for each subregion

    tie(count, ignore, ignore) =
        identify_hidden_singles_in_subregion(s, region, i);
    num_hidden_singles_in_region += count;
  }

  return num_hidden_singles_in_region;
}

int sudoku_num_hidden_singles(const Sudoku &s) {
  int num_hidden_singles = 0;

  num_hidden_singles += num_hidden_singles_in_region(s, Region_t::row);
  num_hidden_singles += num_hidden_singles_in_region(s, Region_t::col);
  num_hidden_singles += num_hidden_singles_in_region(s, Region_t::block);

  return num_hidden_singles;
}

bool sudoku_has_hidden_singles(const Sudoku &s) {
  return sudoku_num_hidden_singles(s) > 0;
}

int sudoku_num_naked_twins_in_region(const Sudoku &s, const Region_t region) {
  //
  // naked twin: Candidate lists of two cells in a region have the same two
  //             entries. Since these two cells must have those values,
  //             these candidate values can be removed in all other candidate
  //             lists of this region.
  //

  int num_naked_twins_in_region = 0;
  int count = 0;

  for (int i = 0; i < s.region_size; ++i) { // for each subregion within region

    tie(count, ignore, ignore) =
        identify_naked_twins_in_subregion(s, region, i);
    num_naked_twins_in_region += count;
  }

  return num_naked_twins_in_region;
}

int sudoku_num_naked_twins(const Sudoku &s) {
  int num_naked_twins = 0;

  num_naked_twins += sudoku_num_naked_twins_in_region(s, Region_t::row);
  num_naked_twins += sudoku_num_naked_twins_in_region(s, Region_t::col);
  num_naked_twins += sudoku_num_naked_twins_in_region(s, Region_t::block);

  return num_naked_twins;
}

bool sudoku_has_naked_twins(const Sudoku &s) {
  return sudoku_num_naked_twins(s) > 0;
}

int sudoku_num_hidden_twins_in_region(const Sudoku &s, const Region_t region) {
  // hidden twin:
  //
  // if two values occur only twice in the same region and if those two values
  // occur in two cells together exclusively, then all other candiates in those
  // two cells' candidate lists can be removed, because the two values can only
  // occur in these two cells (defined location)
  //

  int num_hidden_twins_in_region = 0;
  int count = 0;

  for (int i = 0; i < s.region_size; ++i) {
    tie(count, ignore, ignore) =
        identify_hidden_twins_in_subregion(s, region, i);
    num_hidden_twins_in_region += count;
  }

  return num_hidden_twins_in_region;
}

int sudoku_num_hidden_twins(const Sudoku &s) {
  int num_hidden_twins = 0;

  num_hidden_twins += sudoku_num_hidden_twins_in_region(s, Region_t::row);
  num_hidden_twins += sudoku_num_hidden_twins_in_region(s, Region_t::col);
  num_hidden_twins += sudoku_num_hidden_twins_in_region(s, Region_t::block);

  return num_hidden_twins;
}

bool sudoku_has_hidden_twins(const Sudoku &s) {
  return sudoku_num_hidden_twins(s) > 0;
}

int sudoku_remove_naked_singles(
    Sudoku &s) { // return no. of removed naked singles

  int num_naked_singles_removed = 0;

  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val == 0 && s(cnt).cand.size() == 1) {
      s(cnt).val = s(cnt).cand.front();
      ++num_naked_singles_removed;
      sudoku_update_candidates_affected_by_cell(s, cnt);
     }
  }

  return num_naked_singles_removed;
}

int sudoku_remove_hidden_singles_in_region(Sudoku &s, const Region_t region) {
  // hidden single: candidate lists in a region contain a value only once
  //                this is a hidden single, since each region must have
  //                all permissible values

  vector<int> hidden_single_values{};
  vector<int> hidden_single_pos{};

  int num_hidden_singles_removed_in_region = 0;

  for (int i = 0; i < s.region_size; ++i) { // for each subregion

    hidden_single_values.clear();
    hidden_single_pos.clear();

    tie(ignore, hidden_single_values, hidden_single_pos) =
        identify_hidden_singles_in_subregion(s, region, i);

    // remove hidden singles
    for (size_t k = 0; k < hidden_single_values.size(); ++k) {
      int cnt = s.region_to_cnt(region, i, hidden_single_pos[k]);
      s(cnt).val = hidden_single_values[k];
      ++num_hidden_singles_removed_in_region;

      sudoku_update_candidates_affected_by_cell(s, cnt);
    }
  }

  return num_hidden_singles_removed_in_region;
}

int sudoku_remove_hidden_singles(Sudoku &s) {
  int num_hidden_singles_removed = 0;

  num_hidden_singles_removed +=
      sudoku_remove_hidden_singles_in_region(s, Region_t::row);
  num_hidden_singles_removed +=
      sudoku_remove_hidden_singles_in_region(s, Region_t::col);
  num_hidden_singles_removed +=
      sudoku_remove_hidden_singles_in_region(s, Region_t::block);

  return num_hidden_singles_removed;
}

int sudoku_remove_naked_twins_in_region(Sudoku &s, const Region_t region) {
  //
  // naked twin: Candidate lists of two cells in a region have the same two
  //             entries. Since these two cells have those values,
  //             The candidate values can be removed in all other candidate
  //             lists of this region.
  //

  int num_naked_twins_removed_in_region = 0;

  vector<list<int>> twin_values;
  vector<list<int>> twin_pos;

  for (int i = 0; i < s.region_size; ++i) {
    twin_values.clear();
    twin_pos.clear();

    tie(ignore, twin_values, twin_pos) =
        identify_naked_twins_in_subregion(s, region, i);

    // if (twin_pos.size()>0) {
    //   cout << "region: " << int(region) << ", i: " << i << ", ";
    //   cout << "twin_values: ";
    //   print_vector_list_int(twin_values);
    //   cout << "region: " << int(region) << ", i: " << i << ", ";
    //   cout << "twin_pos: ";
    //   print_vector_list_int(twin_pos);
    //   cout << "\n";
    // }

    // remove twins from candidate list of other cells
    for (size_t t_cnt = 0; t_cnt < twin_values.size(); ++t_cnt) {
      for (int j = 0; j < s.region_size; ++j) {
        int cnt = s.region_to_cnt(region, i, j);
        if (s(cnt).val == 0 &&
            none_of(twin_pos[t_cnt].begin(), twin_pos[t_cnt].end(),
                    [j](int i) { return i == j; })) {
          // only remove if other cell in region
          remove_from_list_int(s(cnt).cand, twin_values[t_cnt]);
        }
      }
      ++num_naked_twins_removed_in_region;
    }
  }

  return num_naked_twins_removed_in_region;
}

int sudoku_remove_naked_twins(Sudoku &s) {
  int num_naked_twins_removed = 0;

  num_naked_twins_removed +=
      sudoku_remove_naked_twins_in_region(s, Region_t::row);
  num_naked_twins_removed +=
      sudoku_remove_naked_twins_in_region(s, Region_t::col);
  num_naked_twins_removed +=
      sudoku_remove_naked_twins_in_region(s, Region_t::block);

  return num_naked_twins_removed;
}

int sudoku_remove_hidden_twins_in_region(Sudoku &s, const Region_t region) {
  // hidden twin:
  //
  // if two values occur only twice in the same region and if those two values
  // occur in two cells together exclusively, then all other candiates in those
  // two cells' candidate lists can be removed, because the two values can only
  // occur in these two cells (defined location)
  //

  int num_hidden_twins_in_region = 0;

  vector<list<int>> twin_values{};
  vector<list<int>> twin_pos{};

  for (int i = 0; i < s.region_size; ++i) {
    twin_values.clear();
    twin_pos.clear();

    tie(ignore, twin_values, twin_pos) =
        identify_hidden_twins_in_subregion(s, region, i);

    // to remove other candidates from list,
    // replace candidate list at twin positions with twin values,
    // effectively removing all other entries
    for (size_t k = 0; k < twin_values.size(); ++k) {
      int cnt1 = s.region_to_cnt(region, i, twin_pos[k].front());
      int cnt2 = s.region_to_cnt(region, i, twin_pos[k].back());
      s(cnt1).cand = twin_values[k];
      s(cnt2).cand = twin_values[k];
    }
  }

  return num_hidden_twins_in_region;
}

int sudoku_remove_hidden_twins(Sudoku &s) {
  int num_hidden_twins_removed = 0;

  num_hidden_twins_removed +=
      sudoku_remove_hidden_twins_in_region(s, Region_t::row);
  num_hidden_twins_removed +=
      sudoku_remove_hidden_twins_in_region(s, Region_t::col);
  num_hidden_twins_removed +=
      sudoku_remove_hidden_twins_in_region(s, Region_t::block);

  return num_hidden_twins_removed;
}
