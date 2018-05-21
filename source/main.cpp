
#include "sudoku_class.h"
#include "sudoku_print.h"
#include "sudoku_read.h"
#include "sudoku_solve.h"

#include <iostream>

void sudoku_print_status(const Sudoku &s, const string msg) {
  cout << msg << ", status:\n";
  cout << "is_valid           = " << boolalpha << sudoku_is_valid(s) << "\n";
  cout << "num_entries        = " << sudoku_num_entries(s) << "\n";
  cout << "num_empty          = " << sudoku_num_empty(s) << "\n";
  cout << "has_candidates     = " << boolalpha << sudoku_has_candidates(s)
       << "\n";
  cout << "num_candidates     = " << sudoku_num_candidates(s) << "\n";
  cout << "has_naked_singles  = " << boolalpha << sudoku_has_naked_singles(s)
       << "\n";
  cout << "num_naked_singles  = " << sudoku_num_naked_singles(s) << "\n";
  cout << "has_hidden_singles = " << boolalpha << sudoku_has_hidden_singles(s)
       << "\n";
  cout << "num_hidden_singles = " << sudoku_num_hidden_singles(s) << "\n";
  cout << "has_naked_twins    = " << boolalpha << sudoku_has_naked_twins(s)
       << "\n";
  cout << "num_naked_twins    = " << sudoku_num_naked_twins(s) << "\n";
  cout << "has_hidden_twins   = " << boolalpha << sudoku_has_hidden_twins(s)
       << "\n";
  cout << "num_hidden_twins   = " << sudoku_num_hidden_twins(s) << "\n";

  cout << "\n";
}

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
  cout << "\nEinlesen der Eingabedatei:\n\n";

  // read sudoku size and create sudoku object
  int region_size = read_int();
  int bpr = read_int();
  int bpc = read_int();
  Sudoku s(region_size, bpr, bpc);
  // sudoku_print(s,"s"); cout << "\n\n";
  // sudoku_print_candidates(s,"s"); cout << "\n\n";

  cout << "region_size=" << region_size << ", bpr=" << bpr << ", bpc=" << bpc
       << "\n\n";

  // initialize sudoku with input values
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    s(cnt).val = read_int();
    if (s(cnt).val != 0) {
      sudoku_update_candidates_affected_by_cell(s, cnt);
    }
  }

  // check for valid sudoku
  if (!sudoku_is_valid(s)) {
    cout << "\nInvalid sudoku! Program terminated.\n";
    return -1;
  }
  // initialize candidate lists (later to be part of input routines!)
  // sudoku_update_candidates_all_cells(s);

  // sudoku_print(s,"s"); cout << "\n\n";
  // sudoku_print_regions(s,"s_regions"); cout << "\n\n";
  // sudoku_print_cnt_to_x(s,"cnt_to_x"); cout << "\n\n";
  sudoku_print_candidates(s, "s", Region_t::row);
  cout << "\n\n";
  sudoku_print_candidates(s, "s", Region_t::col);
  cout << "\n\n";
  sudoku_print_candidates(s, "s", Region_t::block);
  cout << "\n\n";

  sudoku_print(s, "s");
  cout << "\n\n";
  sudoku_print_status(s, "s");

  // for (int b=0;b<s.region_size;++b)
  //  s.cell_is_in_affected_regions(b,1);

  // int run_count=0;
  // while (sudoku_has_naked_singles(s)) {
  //   ++run_count;
  //   cout << "remove naked singles - run no. " << run_count << "\n";

  //   int removed_naked_singles_count = sudoku_remove_naked_singles(s);
  //   cout << "no. of removed singles: " << removed_naked_singles_count <<
  //   "\n\n";
  // }

  // sudoku_print(s,"s"); cout << "\n\n";
  // sudoku_print_candidates(s,"s",Region_t::row); cout << "\n\n";
  // sudoku_print_status(s,"s");

  // run_count = 0;
  // bool candidates_removed = true;

  // while (sudoku_has_naked_twins(s) && candidates_removed) {
  //   ++run_count;
  //   cout << "remove naked twins - run no. " << run_count << "\n";

  //   int num_candidates_before = sudoku_num_candidates(s);
  //   int removed_naked_count = sudoku_remove_naked_twins(s);
  //   int num_candidates_after = sudoku_num_candidates(s);

  //   if (num_candidates_before - num_candidates_after != 0) {
  //     cout << "removed_naked_twins(s) = " << removed_naked_count << "\n\n";
  //   } else {
  //     candidates_removed = false;
  //     cout << "no naked twins removed\n\n";
  //   }
  // }

  // sudoku_print(s,"s"); cout << "\n\n";
  // // sudoku_print_candidates(s,"s"); cout << "\n\n";
  // sudoku_print_status(s,"s");

  // run_count=0;
  // while (sudoku_has_naked_singles(s)) {
  //   ++run_count;
  //   cout << "remove naked singles - run no. " << run_count << "\n";

  //   int removed_naked_singles_count = sudoku_remove_naked_singles(s);
  //   cout << "no. of removed singles: " << removed_naked_singles_count <<
  //   "\n\n";
  // }

  // sudoku_print(s,"s"); cout << "\n\n";
  // // sudoku_print_candidates(s,"s"); cout << "\n\n";
  // sudoku_print_status(s,"s");

  // run_count = 0;
  // candidates_removed = true;

  // while (sudoku_has_naked_twins(s) && candidates_removed) {
  //   ++run_count;
  //   cout << "remove naked twins - run no. " << run_count << "\n";

  //   int num_candidates_before = sudoku_num_candidates(s);
  //   int removed_naked_twins = sudoku_remove_naked_twins(s);
  //   int num_candidates_after = sudoku_num_candidates(s);

  //   if (num_candidates_before - num_candidates_after != 0) {
  //     cout << "removed_naked_twins(s) = " << removed_naked_twins << "\n\n";
  //   } else {
  //     candidates_removed = false;
  //     cout << "no naked twins removed\n\n";
  //   }
  // }

  // sudoku_print(s,"s"); cout << "\n\n";
  // sudoku_print_candidates(s,"s"); cout << "\n\n";
  // sudoku_print_status(s,"s");

  // int removed_naked_twins = sudoku_remove_naked_twins(s);
  // removed_naked_twins = sudoku_remove_naked_twins(s);
  // sudoku_print_status(s,"s");

  // sudoku_print(s,"s"); cout << "\n\n";
  // sudoku_print_candidates(s,"s",Region_t::row); cout << "\n\n";
  // sudoku_print_status(s,"s");

  // int num_removed_hidden_twins = sudoku_remove_hidden_twins(s);
  // cout << "no. of removed hidden twins: " << num_removed_hidden_twins <<
  // "\n\n";

  // sudoku_print_candidates(s,"s",Region_t::row); cout << "\n\n";
  // sudoku_print_status(s,"s");
  // sudoku_print(s,"s"); cout << "\n\n";

  // access sudoku s via begin(), end()
  cout << "{";
  for (auto p = s.begin(); p != s.end(); ++p) {
    cout << (*p).val << ", ";
  }
  cout << "}\n\n";

 // access complete regions s via begin(), end()
  cout << "{";
  for (auto pr = s.row.begin(); pr != s.row.end(); ++pr) {
    for (auto p = pr->begin(); p != pr->end(); ++p) {
      cout << (*p)->val << ", ";
    }
  }
  cout << "}\n\n";

  // access single region via begin(), end()
  cout << "{";
  for (auto p = s.block(7).begin(); p != s.block(7).end(); ++p) {
    cout << (*p)->val << ", ";
  }
  cout << "}\n\n";

  return 0;
}
