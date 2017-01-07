//345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "sudoku_class.h"

// access classes of Sudoku for various access schemes (row, col, block)
  
Row_access::Row_access(Sudoku& _ref) : ref(_ref) {}
    
int& Row_access::operator()(int i, int j) {
  // i... row index
  // j... index within row
  dynamic_assert(ref.is_valid_index(i,j),"Index out of range.");
  return ref.f[ref.row_to_cnt(i,j)];
}
    
const int& Row_access::operator()(int i, int j) const {
  // i... row index
  // j... index within row
  dynamic_assert(ref.is_valid_index(i,j),"Index out of range.");
  return ref.f[ref.row_to_cnt(i,j)];
}

  
Col_access::Col_access(Sudoku& _ref) : ref(_ref) {}
    
int& Col_access::operator()(int i, int j) {
  // i... col index
  // j... index within col
  dynamic_assert(ref.is_valid_index(i,j),"Index out of range.");
  return ref.f[ref.col_to_cnt(i,j)];
}

const int& Col_access::operator()(int i, int j) const {
  // i... col index
  // j... index within col
  dynamic_assert(ref.is_valid_index(i,j),"Index out of range.");
  return ref.f[ref.col_to_cnt(i,j)];
}


Block_access::Block_access(Sudoku& _ref) : ref(_ref) {}
    
int& Block_access::operator()(int i, int j) {
  // i... block index
  // j... index within block
  dynamic_assert(ref.is_valid_index(i,j),"Index out of range.");
  return ref.f[ref.block_to_cnt(i,j)];
}

const int& Block_access::operator()(int i, int j) const {
  // i... block index
  // j... index within block
  dynamic_assert(ref.is_valid_index(i,j),"Index out of range.");
  return ref.f[ref.block_to_cnt(i,j)];
}


list<int>& Sudoku::candidates(int cnt) {
  dynamic_assert(is_valid_index(cnt),"Index out of range.");
  return cand[cnt];
}

const list<int>& Sudoku::candidates(int cnt) const {
  dynamic_assert(is_valid_index(cnt),"Index out of range.");
  return cand[cnt];
}

list<int>& Sudoku::candidates(int i, int j) {
  dynamic_assert(is_valid_index(i,j),"Index out of range.");
  return cand[row_to_cnt(i,j)];
}

const list<int>& Sudoku::candidates(int i, int j) const {
  dynamic_assert(is_valid_index(i,j),"Index out of range.");
  return cand[row_to_cnt(i,j)];
}

//
// Sudoku regular constructor
//
Sudoku::Sudoku(int _region_size, int _blocks_per_row, int _blocks_per_col) :
  region_size(_region_size),
  blocks_per_row(_blocks_per_row),
  blocks_per_col(_blocks_per_col),
  total_size(_region_size*_region_size),
  row(*this),
  col(*this),
  block(*this) {

  // cout << "regular constructor called.\n";

  // check for valid parameters
  dynamic_assert(region_size/blocks_per_row == blocks_per_col &&
		 region_size>0 &&
		 region_size%blocks_per_row ==0,
		 "Invalid Sudoku parameters.");
  
  f = new int[total_size];
  
  // initialize member array as empty Sudoku
  for (int cnt=0;cnt<total_size;++cnt) {
    f[cnt]=0;
  }

  // initialize and assign candidate lists for empty Sudoku
  cand.reserve(total_size);
  for (int cnt=0;cnt<total_size;++cnt) {
    // cand.push_back({});
    cand.push_back({1,2,3,4,5,6,7,8,9});
  }  
  
}

//
// Sudoku copy constructor
//
Sudoku::Sudoku(const Sudoku& other_Sudoku) :
  region_size(other_Sudoku.region_size),
  blocks_per_row(other_Sudoku.blocks_per_row),
  blocks_per_col(other_Sudoku.blocks_per_col),
  total_size(other_Sudoku.region_size*other_Sudoku.region_size),
  row(*this),
  col(*this),
  block(*this) {

  // cout << "copy constructor called.\n";
  
  f = new int[total_size];
  
  // copy member array
  for (int cnt=0;cnt<total_size;++cnt) {
    f[cnt]=other_Sudoku.f[cnt];
  }

  // initialize and assign candidate lists
  cand.reserve(total_size);
  for (int cnt=0;cnt<total_size;++cnt) {
    cand.push_back(other_Sudoku.cand[cnt]);
  }
  
}

Sudoku::~Sudoku() {
  // cout << "destructor called.\n";
  delete[] f;
}

// assignment
Sudoku& Sudoku::operator=(const Sudoku& other_Sudoku) {

  // cout << "assignment operator called\n";
  
  if (this != &other_Sudoku) {  // no self-assignment
    
    // only assign to Sudoku of identical layout
    dynamic_assert( ((region_size == other_Sudoku.region_size) &&
	             (blocks_per_row == other_Sudoku.blocks_per_row) &&
                     (blocks_per_col == other_Sudoku.blocks_per_col) &&
		     (total_size == other_Sudoku.total_size)),
		    "Invalid assignment. Layout of source and destination must be identical.");
      
    copy(&other_Sudoku.f[0], &other_Sudoku.f[0] + size_t(total_size), &f[0]);
    for (int cnt=0;cnt<total_size;++cnt) {
      cand[cnt]=other_Sudoku.cand[cnt];
    }  
  }
  return *this;
}

bool Sudoku::is_valid_index(int i) const {
  return ((i>=0) && (i<total_size));
}

bool Sudoku::is_valid_index(int i, int j) const {
  return ((i>=0) && (i<region_size) && (j>=0) && (j<region_size));
}

int& Sudoku::operator()(int cnt) {
  dynamic_assert(is_valid_index(cnt),"Index out of range.");
  return f[cnt];
}

int Sudoku::operator()(int cnt) const {
  dynamic_assert(is_valid_index(cnt),"Index out of range.");
  return f[cnt];
}

int& Sudoku::operator()(int i, int j) {
  dynamic_assert(is_valid_index(i,j),"Index out of range.");
  return f[row_to_cnt(i,j)];
}

int Sudoku::operator()(int i, int j) const {
  dynamic_assert(is_valid_index(i,j),"Index out of range.");
  return f[row_to_cnt(i,j)];
}

int Sudoku::row_to_cnt(int i, int j) const {
  // i... row index
  // j... index within row
  return i*region_size+j;
}

int Sudoku::col_to_cnt(int i, int j) const {
  // i... col index
  // j... index within col
  return i+region_size*j;
}

int Sudoku::block_to_cnt(int i, int j) const {
  // i... block index
  // j... index within block
  //
  // row_in_block = j/cols_per_block
  // col_in_block = j%cols_per_block
  //
  // row = row_in_block + rows_per_block * (i / blocks_per_row)
  // col = col_in_block + cols_per_block * (i % blocks_per_row)
  //
  // cnt = col + row*region_size

  int rows_per_block = region_size/blocks_per_col;
  int cols_per_block = region_size/blocks_per_row;

  int cnt = j%cols_per_block + cols_per_block*(i%blocks_per_row)+
    (j/cols_per_block + rows_per_block*(i/blocks_per_row))*region_size;
  return cnt;
}


pair<int,int> Sudoku::cnt_to_row(int cnt) const {
  // first = row index = cnt/region_size
  // second= col index = cnt%region_size
  return make_pair(cnt/region_size,cnt%region_size);
}

pair<int,int> Sudoku::cnt_to_col(int cnt) const {
  // first = col index = cnt%region_size
  // second= row index = cnt/region_size
  return make_pair(cnt%region_size,cnt/region_size);
}

pair<int,int> Sudoku::cnt_to_block(int cnt) const {
  // first = block index
  // second= index in block
  //
  // row = cnt/region_size
  // col = cnt%region_size
  //
  // rows_per_block = region_size/blocks_per_col
  // cols_per_block = region_size/blocks_per_row
  //
  // 1st index:
  //
  // block index = col/cols_per_block + (row/rows_per_block)*blocks_per_row
  // row_in_block = row % rows_per_block
  // col_in_block = col % cols_per_block
  //
  // 2nd index:
  //
  // index in block = col_in_block + row_in_block*cols_per_block

  int row = cnt/region_size;
  int col = cnt%region_size;
  int rows_per_block = region_size/blocks_per_col;
  int cols_per_block = region_size/blocks_per_row;

  int block_idx    = col/cols_per_block + (row/rows_per_block)*blocks_per_row;
  int idx_in_block = col%cols_per_block + (row%rows_per_block)*cols_per_block;
    
  return make_pair(block_idx,idx_in_block);
}


bool Sudoku::is_valid() const {

  // check for valid entry values:
  // 0 (=empty indicator, is valid too), valid entries: 1..region_size
  for (int cnt=0;cnt<total_size;++cnt) {
    if (f[cnt]<0 || f[cnt]>region_size) {
      return false;
    }
  }

  // check for unique entries in rows
  for (int cnt=0;cnt<region_size;++cnt) { // for each row
    for (int value=1;value<=region_size;++value) { // for each potential value
      int count=0;
      for (int pos=0;pos<region_size;++pos) { // occurences at each position
	if (row(cnt,pos)==value) ++count;
      }
      if (count>1) return false;
    }
  }

  // check for unique entries in cols
  for (int cnt=0;cnt<region_size;++cnt) { // for each col
    for (int value=1;value<=region_size;++value) { // for each potential value
      int count=0;
      for (int pos=0;pos<region_size;++pos) { // occurences at each position
	if (col(cnt,pos)==value) ++count;
      }
      if (count>1) return false;
    }
  }

  // check for unique entries in blocks
  for (int cnt=0;cnt<region_size;++cnt) { // for each block
    for (int value=1;value<=region_size;++value) { // for each potential value
      int count=0;
      for (int pos=0;pos<region_size;++pos) { // occurences at each position
	if (block(cnt,pos)==value) ++count;
      }
      if (count>1) return false;
    }
  }

  return true;
}


int Sudoku::num_entries() const {

  // return no. of entries > 0
  int count=0;
  for (int cnt=0; cnt<total_size; ++cnt) {
    if (f[cnt] > 0) ++count;
  }
  return count;
}

int Sudoku::num_empty() const {

  // return no. of empty entries, i.e. entries with value 0
  return total_size - num_entries();
}

