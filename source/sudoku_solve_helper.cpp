// 345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "sudoku_solve_helper.h"
#include "sudoku_print.h"

#include <algorithm>

using namespace std;

// remove all values in list remove_values from list remove_target
void remove_from_list_int(list<int> &remove_target,
                          const list<int> &remove_values) {
  // remove all values provided in list remove_values from remove_target
  for (auto vp = remove_values.begin(); vp != remove_values.end(); ++vp) {
    remove_target.remove(*vp);
  }
}

// check for valid and unique entries in region
bool sudoku_has_unique_entries_in_region(const Sudoku &s,
                                         const Region_t region) {
  for (int i = 0; i < s.region_size; ++i) { // for each row / col / block

    for (int value = 1; value <= s.region_size;
         ++value) { // for each potential value
      int count = 0;
      for (int j = 0; j < s.region_size; ++j) { // occurences at each position
        if (s(s.region_to_cnt(region, i, j)).val == value)
          ++count;
      }
      if (count > 1)
        return false;
    }
  }
  return true;
}

// concatenate candidate lists in subregion
list<int> sudoku_concatenate_candidate_lists_in_subregion(const Sudoku &s,
                                                          const Region_t region,
                                                          const int subregion) {
  list<int> concatenated_list{};
  for (int j = 0; j < s.region_size; ++j) { // for each cell in subregion
    int cnt = s.region_to_cnt(region, subregion, j);
    if (s(cnt).val == 0) { // if cell is empty, there should be candidates
      concatenated_list.insert(concatenated_list.end(), s(cnt).cand.begin(),
                               s(cnt).cand.end());
    }
  }
  // cout << "concatenated cand lists: ";
  // print_list_int(concatenated_list);

  return concatenated_list;
}

// count how often which sudoku entry (1 .. s.region_size) occurs in list
vector<int> sudoku_count_candidate_entries(const Sudoku &s,
                                           const std::list<int> &input_list) {
  vector<int> candidate_count{};
  for (int j = 1; j <= s.region_size; ++j) {
    candidate_count.push_back(count(input_list.begin(), input_list.end(), j));
  }
  // cout << "candidate_count:         ";
  // print_vector_int(candidate_count);

  return candidate_count;
}

// store index where elements occur in current subregion (index i)
vector<list<int>>
sudoku_candidate_positions_in_subregion(const Sudoku &s, const Region_t region,
                                        const int subregion,
                                        const vector<int> &candidate_count) {
  list<int> lp{};                  // position helper list
  vector<list<int>> candidate_pos; // index position of candidates in region

  for (int j = 0; j < s.region_size; ++j) { // for each value (=j+1)
    lp.clear();
    int value = j + 1;
    for (int k = 0; k < s.region_size; ++k) { // for each candidate list
      int cnt = s.region_to_cnt(region, subregion, k);
      if (s(cnt).val == 0 &&        // has candidate list
          candidate_count[j] > 0 && // element does occur in region
          count(s(cnt).cand.begin(), s(cnt).cand.end(), value) > 0) {
        lp.push_back(k);
      }
    }
    candidate_pos.push_back(lp);
  }
  // cout << "pos:                     ";
  // print_vector_list_int(cand_pos);
  // cout << "\n";

  return candidate_pos;
}

// identify hidden twins in subregion (index i)
tuple<int, vector<int>, vector<int>>
identify_hidden_singles_in_subregion(const Sudoku &s, const Region_t region,
                                     const int subregion) {
  list<int> lh{};               // helper list to count candidates
  vector<int> cand_count{};     // candidate occurrance count
  vector<list<int>> cand_pos{}; // candidate position index

  int num_hidden_singles_in_subregion = 0;
  vector<int> hidden_single_values{};
  vector<int> hidden_single_pos{};

  // concatenate candidate lists in current subregion
  lh = sudoku_concatenate_candidate_lists_in_subregion(s, region, subregion);

  // count how often which entry occurs in candidate list
  cand_count = sudoku_count_candidate_entries(s, lh);

  // store index where elements occur in current subregion (index i)
  cand_pos =
      sudoku_candidate_positions_in_subregion(s, region, subregion, cand_count);

  // identify hidden singles
  for (int k = 0; k < s.region_size;
       ++k) { // for each value k+1 of the entry list
    if (cand_count[k] == 1) {
      // hidden single found
      ++num_hidden_singles_in_subregion;

      // cout << "found hidden single in region " << int(region);
      // cout << ", subregion " << subregion;
      // cout << ", value " << k+1 << " ";
      // cout << "at index " << cand_pos[k].front() << "\n\n";

      hidden_single_pos.push_back(cand_pos[k].front());
      hidden_single_values.push_back(k + 1);
    }
  }

  return make_tuple(num_hidden_singles_in_subregion, hidden_single_values,
                    hidden_single_pos);
}

// identify naked twins in subregion (index i)
tuple<int, vector<list<int>>, vector<list<int>>>
identify_naked_twins_in_subregion(const Sudoku &s, const Region_t region,
                                  const int subregion) {
  list<int> lh{};           // helper list to count candidates
  vector<int> cand_count{}; // candidate occurrance count

  int num_naked_twins_in_subregion = 0;
  vector<list<int>> twin_values{};
  vector<list<int>> twin_pos{};

  // concatenate candidate lists in current subregion
  lh = sudoku_concatenate_candidate_lists_in_subregion(s, region, subregion);

  // count how often which entry occurs in candidate list
  cand_count = sudoku_count_candidate_entries(s, lh);

  for (int j = 0; j < s.region_size; ++j) {
    int cnt = s.region_to_cnt(region, subregion, j);
    if (s(cnt).val == 0 && s(cnt).cand.size() == 2) {
      // potential twin candidate found
      for (int k = j + 1; k < s.region_size; ++k) {
        int cnt_k = s.region_to_cnt(region, subregion, k);
        if (s(cnt_k).val == 0 && s(cnt_k).cand.size() == 2 &&
            s(cnt).cand == s(cnt_k).cand &&
            (cand_count[s(cnt).cand.front() - 1] > 2 ||
             cand_count[s(cnt).cand.back() - 1] > 2)) {
          // naked twin found
          ++num_naked_twins_in_subregion;
          twin_values.push_back(s(cnt).cand);
          list<int> lh;
          lh.push_back(j);
          lh.push_back(k);
          twin_pos.push_back(lh);
        }
      }
    }
  }

  return make_tuple(num_naked_twins_in_subregion, twin_values, twin_pos);
}

// identify hidden twins in subregion (index i)
tuple<int, vector<list<int>>, vector<list<int>>>
identify_hidden_twins_in_subregion(const Sudoku &s, const Region_t region,
                                   const int subregion) {
  list<int> lh{};               // helper list to count candidates
  vector<int> cand_count{};     // candidate occurrance count
  vector<list<int>> cand_pos{}; // candidate position index

  int num_hidden_twins_in_subregion = 0;
  vector<list<int>> twin_values{};
  vector<list<int>> twin_pos{};

  // concatenate candidate lists in current subregion
  lh = sudoku_concatenate_candidate_lists_in_subregion(s, region, subregion);

  // count how often which entry occurs in candidate list
  cand_count = sudoku_count_candidate_entries(s, lh);

  // store index where elements occur in current subregion (index i)
  cand_pos =
      sudoku_candidate_positions_in_subregion(s, region, subregion, cand_count);

  // check for hidden twins
  for (int j = 0; j < s.region_size; ++j) { // for each value
    if (cand_count[j] == 2) {               // potential hidden twin candidate
      list<int> curr_cand_pos = cand_pos[j];
      for (int k = j + 1; k < s.region_size; ++k) {
        int cnt1 = s.region_to_cnt(region, subregion, curr_cand_pos.front());
        int cnt2 = s.region_to_cnt(region, subregion, curr_cand_pos.back());
        if (cand_count[k] == 2 && curr_cand_pos == cand_pos[k] &&
            (s(cnt1).cand.size() > 2 || s(cnt2).cand.size() > 2)) {
          // hidden twin found
          ++num_hidden_twins_in_subregion;

          // cout << "found hidden twin in region " << int(region);
          // cout << ", subregion " << subregion;
          // cout << ", value (" << j+1 << ", " << k+1 << ") ";
          // cout << "at index (" << curr_cand_pos.front() << ", ";
          // cout << curr_cand_pos.back() << ").\n\n";

          list<int> llh;
          llh.push_back(curr_cand_pos.front());
          llh.push_back(curr_cand_pos.back());
          twin_pos.push_back(llh);
          llh.clear();
          llh.push_back(j + 1);
          llh.push_back(k + 1);
          twin_values.push_back(llh);
        }
      }
    }
  }

  return make_tuple(num_hidden_twins_in_subregion, twin_values, twin_pos);
}
