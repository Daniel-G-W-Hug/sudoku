
#include "sudoku_class.h"
#include "sudoku_read.h"
#include "sudoku_print.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

  cout << "\nEinlesen der Eingabedatei:\n\n";
    
  // read sudoku size and create sudoku object
  int region_size = read_int();
  int bpr = read_int();
  int bpc = read_int();
  Sudoku s(region_size,bpr,bpc);

  cout << "region_size=" << region_size
       << ", bpr=" << bpr
       << ", bpc=" << bpc << "\n\n"; 

  // initialize sudoku with input values
  for(int cnt=0;cnt<s.total_size;++cnt) {
    s(cnt)=read_int();
  }

  sudoku_print(s,"s"); cout << "\n\n";
  
  // check for valid sudoku
  if (!s.is_valid()) {
    cout << "\nInvalid sudoku! Program terminated.\n";
    return -1;
  }
  
  // sudoku_print(s,"s"); cout << "\n\n";
  // sudoku_print_regions(s,"s_regions"); cout << "\n\n";
  // sudoku_print_cnt_to_x(s,"cnt_to_x"); cout << "\n\n";
  sudoku_print_candidates(s,"s"); cout << "\n\n";  

  cout << "num_entries() = " << s.num_entries() << "\n";
  cout << "num_empty()   = " << s.num_empty() << "\n";
  
  return  0;
}
