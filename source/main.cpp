
#include "sudoku_class.h"
#include "sudoku_read.h"
#include "sudoku_print.h"
#include "sudoku_solve.h"

#include <iostream>


void sudoku_print_status(const Sudoku& s, const string msg) {
  cout << msg << ", status:\n";
  cout << "is_valid        = " << boolalpha << s.is_valid() << "\n";
  cout << "num_entries     = " << s.num_entries() << "\n";
  cout << "num_empty       = " << s.num_empty() << "\n";
  cout << "has_candidates  = " << boolalpha << sudoku_has_candidates(s) << "\n";
  cout << "num_candidates  = " << sudoku_num_candidates(s) << "\n";
  cout << "has_singles     = " << boolalpha << sudoku_has_singles(s) << "\n";
  cout << "num_singles     = " << sudoku_num_singles(s) << "\n";
  cout << "has_naked_twins = " << boolalpha << sudoku_has_naked_twins(s) << "\n";
  cout << "num_naked_twins = " << sudoku_num_naked_twins(s) << "\n";
  cout << "\n";
}


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
  
  // check for valid sudoku
  if (!s.is_valid()) {
    cout << "\nInvalid sudoku! Program terminated.\n";
    return -1;
  }
  // initialize candidate lists (later to be part of input routines!)
  sudoku_update_candidates_all_cells(s);

  
  sudoku_print(s,"s"); cout << "\n\n";
  // sudoku_print_regions(s,"s_regions"); cout << "\n\n";
  // sudoku_print_cnt_to_x(s,"cnt_to_x"); cout << "\n\n";
  sudoku_print_candidates(s,"s"); cout << "\n\n";  

  sudoku_print_status(s,"s");
    

  int run_count=0;
  int remove_count=0;
  
  while (sudoku_has_singles(s)) {
    ++run_count;
    cout << "run no. " << run_count << "\n";
    
    remove_count += sudoku_remove_singles(s);
  }  
  cout << "no. of removed singles: " << remove_count << "\n\n";
  
  sudoku_print(s,"s"); cout << "\n\n";
  // sudoku_print_candidates(s,"s"); cout << "\n\n";
  sudoku_print_status(s,"s");


  if (sudoku_has_naked_twins(s)) {
    int removed_naked_twins = sudoku_remove_naked_twins(s);
    cout << "removed_naked_twins(s) = " << removed_naked_twins << "\n\n";
  }

  sudoku_print(s,"s"); cout << "\n\n";
  // sudoku_print_candidates(s,"s"); cout << "\n\n";
  sudoku_print_status(s,"s");

  
  run_count=0;
  remove_count=0;
  
  while (sudoku_has_singles(s)) {
    ++run_count;
    cout << "run no. " << run_count << "\n";
    
    remove_count += sudoku_remove_singles(s);
  }  
  cout << "no. of removed singles: " << remove_count << "\n\n";
  
  sudoku_print(s,"s"); cout << "\n\n";
  // sudoku_print_candidates(s,"s"); cout << "\n\n";
  sudoku_print_status(s,"s");

  
  if (sudoku_has_naked_twins(s)) {
    int removed_naked_twins = sudoku_remove_naked_twins(s);
    cout << "removed_naked_twins(s) = " << removed_naked_twins << "\n\n";
  }

  sudoku_print(s,"s"); cout << "\n\n";
  sudoku_print_candidates(s,"s"); cout << "\n\n";
  sudoku_print_status(s,"s");

  
  return  0;
}
