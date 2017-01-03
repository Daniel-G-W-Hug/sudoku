//345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "sudoku_print.h"

void sudoku_print(const Sudoku& sudoku, const string msg) {

  cout << msg << ":\n";
  for (int cnt=0;cnt<sudoku.total_size; ++cnt) {
    if (cnt>0 &&  cnt%sudoku.region_size==0 ) cout << "\n";
    cout << setw(2) << sudoku.f[cnt];
    if (cnt<sudoku.total_size-1) cout << ", ";
  }
  cout << "\n";
}

void sudoku_print_regions(const Sudoku& sudoku, const string msg) {

  cout << msg << ", rows:\n";
  for (int i=0;i<sudoku.region_size; ++i) {
    cout << i << ": ";
    for (int j=0;j<sudoku.region_size; ++j) {
      cout << setw(2) << sudoku.row(i,j);
      if (j<sudoku.region_size-1) cout << ", ";
    }
    cout << "\n";
  }
  cout << "\n";

  cout << msg << ", cols:\n";
  for (int i=0;i<sudoku.region_size; ++i) {
    cout << i << ": ";
    for (int j=0;j<sudoku.region_size; ++j) {
      cout << setw(2) << sudoku.col(i,j);
      if (j<sudoku.region_size-1) cout << ", ";
    }
    cout << "\n";
  }
  cout << "\n";

  cout << msg << ", blocks:\n";
  for (int i=0;i<sudoku.region_size; ++i) {
    cout << i << ": ";
    for (int j=0;j<sudoku.region_size; ++j) {
      cout << setw(2) << sudoku.block(i,j);
      if (j<sudoku.region_size-1) cout << ", ";
    }
    cout << "\n";
  } 
}


void sudoku_print_cnt_to_x(const Sudoku& sudoku, const string msg) {

  cout << msg << ", row_cnt:\n";
  for (int cnt=0;cnt<sudoku.total_size; ++cnt) {
    if (cnt>0 &&  cnt%sudoku.region_size==0 ) cout << "\n";
    cout << setw(1) << sudoku.cnt_to_row(cnt).first <<
            sudoku.cnt_to_row(cnt).second;
    if (cnt<sudoku.total_size-1) cout << ", ";
  }
  cout << "\n\n";

  cout << msg << ", col_cnt:\n";
  for (int cnt=0;cnt<sudoku.total_size; ++cnt) {
    if (cnt>0 &&  cnt%sudoku.region_size==0 ) cout << "\n";
    cout << setw(1) << sudoku.cnt_to_col(cnt).first <<
            sudoku.cnt_to_col(cnt).second;
    if (cnt<sudoku.total_size-1) cout << ", ";
  }
  cout << "\n\n";

  cout << msg << ", block_cnt:\n";
  for (int cnt=0;cnt<sudoku.total_size; ++cnt) {
    if (cnt>0 &&  cnt%sudoku.region_size==0 ) cout << "\n";
    cout << setw(1) << sudoku.cnt_to_block(cnt).first <<
            sudoku.cnt_to_block(cnt).second;
    if (cnt<sudoku.total_size-1) cout << ", ";
  }
  cout << "\n";
}


void sudoku_print_candidates(const Sudoku& sudoku, const string msg) {

  cout << msg << ", candidates:\n";
  for (int cnt=0;cnt<sudoku.total_size; ++cnt) {
    cout << setw(2) << cnt << ": {";
    for (auto item=sudoku.candidates[cnt].cbegin(),
	           end_of_list=sudoku.candidates[cnt].cend(),
	           last=prev(end_of_list);
	           item!=end_of_list; ++item) {
      cout << setw(2) << *item;
      if (item!=last) cout << ",";
    }
    cout << " }\n";
  }
  cout << "\n";
}
