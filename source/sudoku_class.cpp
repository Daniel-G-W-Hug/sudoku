//345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "sudoku_class.h"
#include "sudoku_print.h"    // only for debugging

#include <algorithm>

using namespace std;

// access classes of Sudoku for various access schemes (row, col, block)
  
Access::Access(Sudoku& t_s, const Region t_region) : m_s(t_s),
						     m_region(t_region) {}

void Access::init_access() {

  // initialize references for access in regions (row / col / block)
  for (int i=0;i<m_s.region_size;++i) {
    vector<Sudoku_cell*> rh;
    for (int j=0;j<m_s.region_size;++j) {

      switch (m_region) {

      case Region::row:
	// set links within row
	rh.push_back(&m_s.m_cell[m_s.row_to_cnt(i,j)]);
        break;
	
      case Region::col:
	// set links within row
	rh.push_back(&m_s.m_cell[m_s.col_to_cnt(i,j)]);
        break;
	
      case Region::block:
	// set links within row
	rh.push_back(&m_s.m_cell[m_s.block_to_cnt(i,j)]);
        break;

      }
      
    }
    // add region vector
    m_rv.push_back(rh);
  }
}

vector<Sudoku_cell*>& Access::operator()(int cnt) {
  dynamic_assert(m_s.is_valid_region_index(cnt),"Index out of range.");
  return m_rv[cnt];
}
    
const vector<Sudoku_cell*>& Access::operator()(int cnt) const {
  dynamic_assert(m_s.is_valid_region_index(cnt),"Index out of range.");
  return m_rv[cnt];
}

Sudoku_cell& Access::operator()(int i, int j) {
  // i... region index
  // j... index within region
  dynamic_assert(m_s.is_valid_region_index(i,j),"Index out of range.");
  return *m_rv[i][j];
}
    
const Sudoku_cell& Access::operator()(int i, int j) const {
  // i... region index
  // j... index within region
  dynamic_assert(m_s.is_valid_region_index(i,j),"Index out of range.");
  return *m_rv[i][j];
}


////////////////////////////////////////////////////////////////////////////////
//
// Sudoku regular constructor
//
////////////////////////////////////////////////////////////////////////////////

Sudoku::Sudoku(int t_region_size, int t_blocks_per_row, int t_blocks_per_col) :
  region_size(t_region_size),
  blocks_per_row(t_blocks_per_row),
  blocks_per_col(t_blocks_per_col),
  total_size(t_region_size*t_region_size),
  row(*this,   Region::row),
  col(*this,   Region::col),
  block(*this, Region::block) {

  // cout << "regular constructor called.\n";

  // check for valid parameters
  dynamic_assert(region_size/blocks_per_row == blocks_per_col &&
		 region_size>0 &&
		 region_size%blocks_per_row ==0,
		 "Invalid Sudoku parameters.");
  
  // initialize empty Sudoku incl. region assignment and candidate list for each cell
  Sudoku_cell sc;
  
  for (int i=1;i<=region_size;++i) {
    sc.cand.push_back(i);  // all candidates possible
  }

  // let the current cell know which regions it belongs to
  for (int cnt=0;cnt<total_size;++cnt) {
    sc.r=cnt_to_row(cnt).first;
    sc.c=cnt_to_col(cnt).first;
    sc.b=cnt_to_block(cnt).first;
    m_cell.push_back(sc);
  }

  // initialize links for region access
  row.init_access();
  col.init_access();
  block.init_access();
}

//
// Sudoku copy constructor
//
Sudoku::Sudoku(const Sudoku& other_Sudoku) :
  region_size(other_Sudoku.region_size),
  blocks_per_row(other_Sudoku.blocks_per_row),
  blocks_per_col(other_Sudoku.blocks_per_col),
  total_size(other_Sudoku.region_size*other_Sudoku.region_size),
  row(*this,   Region::row),
  col(*this,   Region::col),
  block(*this, Region::block) {

  // cout << "copy constructor called.\n";
  
  // copy member array
  for (int cnt=0;cnt<total_size;++cnt) {
    m_cell.push_back(other_Sudoku.m_cell[cnt]);
  }

  // initialize links for region access
  row.init_access();
  col.init_access();
  block.init_access();
}

//
// assignment
//
Sudoku& Sudoku::operator=(const Sudoku& other_Sudoku) {

  // cout << "assignment operator called\n";
  
  if (this != &other_Sudoku) {  // no self-assignment
    
    // only assign to Sudoku of identical layout
    dynamic_assert( ((region_size == other_Sudoku.region_size) &&
	             (blocks_per_row == other_Sudoku.blocks_per_row) &&
                     (blocks_per_col == other_Sudoku.blocks_per_col) &&
		     (total_size == other_Sudoku.total_size)),
		    "Invalid assignment. Layout of source and destination must be identical.");
      
    for (int cnt=0;cnt<total_size;++cnt) {
      m_cell[cnt]=other_Sudoku.m_cell[cnt];
    }
    
    // initialize links for region access
    row.init_access();
    col.init_access();
    block.init_access();
  }
  return *this;
}


Sudoku_cell& Sudoku::operator()(int cnt) {
  dynamic_assert(is_valid_index(cnt),"Index out of range.");
  return m_cell[cnt];
}

const Sudoku_cell& Sudoku::operator()(int cnt) const {
  dynamic_assert(is_valid_index(cnt),"Index out of range.");
  return m_cell[cnt];
}

Sudoku_cell& Sudoku::operator()(int i, int j) {
  dynamic_assert(is_valid_region_index(i,j),"Index out of range.");
  return m_cell[row_to_cnt(i,j)];   // use row conversion for 2D array mem_order
}

const Sudoku_cell& Sudoku::operator()(int i, int j) const {
  dynamic_assert(is_valid_region_index(i,j),"Index out of range.");
  return m_cell[row_to_cnt(i,j)];  // use row conversion for 2D array mem_order
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

int Sudoku::region_to_cnt(const Region region, int i, int j) const {
  // i... region index
  // j... index within region
  dynamic_assert(is_valid_region_index(i,j),"Index out of range.");

  int tmp;
  
  switch (region) {
  
  case Region::row:
    tmp = row_to_cnt(i,j);
    break;

  case Region::col:
    tmp = col_to_cnt(i,j);
    break;

  case Region::block:
    tmp = block_to_cnt(i,j);
    break;

  }
  return tmp;
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

pair<int,int> Sudoku::cnt_to_region(const Region region, int cnt) const {
  dynamic_assert(is_valid_index(cnt),"Index out of range.");

  pair<int,int> tmp;

  switch (region) {
    
  case Region::row:
    tmp = cnt_to_row(cnt);
    break;
    
  case Region::col:
    tmp = cnt_to_col(cnt);
    break;
    
  case Region::block:
    tmp = cnt_to_block(cnt);
    break;

  }
  return tmp;
}

bool Sudoku::cell_is_in_affected_regions(int curr_block, int cnt) {
  //
  // cells are affected region if they are in rows or cols
  // which cross the current block (and thus extend into other blocks)
  //

  // provide block indices of blocks belonging to affected regions
  list<int> affected_blocks;
  // blocks which are in affected region horizontally
  for (int nb=0;nb<blocks_per_row;++nb) {
    affected_blocks.push_back((curr_block/blocks_per_row)*blocks_per_row+nb);
  }
  // blocks which are in affected region vertically
  for (int nb=0;nb<blocks_per_col;++nb) {
    affected_blocks.push_back(curr_block%blocks_per_row+nb*blocks_per_row);
  }
  // next two lines just for better readability during debugging (not required for algo)
  affected_blocks.sort();
  affected_blocks.unique();
  // cout << "affected blocks:\n"; print_list_int(affected_blocks);

  // check whether cell m_cell[cnt] is in one of the affected blocks
  dynamic_assert(is_valid_index(cnt),"Invalid cnt value.");
  auto p = find(affected_blocks.begin(),affected_blocks.end(),m_cell[cnt].b);
  if (p != affected_blocks.end()) {
    return true;
  }
  
  return false;
}


bool Sudoku::is_valid_index(int i) const {
  return ((i >= 0) && (i < total_size));
}

bool Sudoku::is_valid_region_index(int i) const {
  return ((i >= 0) && (i < region_size));
}

bool Sudoku::is_valid_region_index(int i, int j) const {
  return ((i >= 0) && (i < region_size) && (j >= 0) && (j < region_size));
}
