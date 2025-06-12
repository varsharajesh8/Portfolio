#include <cassert>
#include "Matrix.hpp"
#include <iostream>

// REQUIRES: mat points to a Matrix
//           0 < width && 0 < height
// MODIFIES: *mat 
// EFFECTS:  Initializes *mat as a Matrix with the given width and height,
//           with all elements initialized to 0.
void Matrix_init(Matrix* mat, int width, int height) {
  assert(mat && width >0 && height >0);
  //accesses element of struct mat points to and sets width and height
  mat -> width = width;
  mat -> height = height;
  //creates a vector of size width * height (# elements) and initializes to 0
  mat -> data = std::vector<int>(width * height, 0);
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: os
// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix* mat, std::ostream& os) {
  assert(mat);
  //print width and height of matrix with ostream
  os << mat->width << " " << mat->height << "\n";
  //using nested for loop, iterate through each element
  for(int i = 0; i < mat->height; ++i){
    for(int j = 0; j < mat->width; ++j){
      //print each element with a space
      os << *Matrix_at(mat, i , j) << " ";
    }
    //print new line after each row
    os << "\n"; 
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix* mat) {
  assert(mat);
  //accesses width of struct mat points to
  return mat -> width;
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix* mat) {
  assert(mat);
  //accesses height of struct mat points to
  return mat -> height;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
int* Matrix_at(Matrix* mat, int row, int column) {
  assert(mat);
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column && column < Matrix_width(mat));
  //used row*width + col to get index of element in vector
  return &mat-> data.at(row * mat->width + column);
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int* Matrix_at(const Matrix* mat, int row, int column) {
  assert(mat);
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column && column < Matrix_width(mat));
  return &mat-> data.at(row * mat->width + column);
}
// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix* mat, int value) {
  assert (mat);
  //nested for loop to iterate through each element
  for(int i = 0; i < mat->height; ++i){
    for(int j = 0; j < mat->width; ++j){
      //set each element to value
      *Matrix_at(mat, i, j) = value;
     }
  } 
    
  }
  

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix* mat, int value) {
  assert(mat); 

  //fill first and last rows
  for(int j = 0; j < mat -> width; ++j){
    *Matrix_at(mat, 0, j) = value; //first row
    *Matrix_at(mat, mat->height - 1, j) = value; //last row
    }
  //fill in first and last column (except already filled corners)
  for (int i = 1; i < mat->height - 1; ++i){
    *Matrix_at(mat, i, 0) = value; //first col
    *Matrix_at(mat, i, mat-> width - 1) = value; //last col
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
  assert(mat); 
  //initialize max value to first element
  int max_value = *Matrix_at(mat, 0, 0);
  //nested for loop to iterate through each element
  for (int i = 0; i < mat -> height; ++i){
    for (int j = 0; j < mat -> width; ++j){
      //if element is greater than max value, update max value
      if (*Matrix_at(mat, i, j) > max_value){
        max_value = *Matrix_at(mat, i, j);
      }
    }
  }
  //return max value
  return max_value;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix* mat, int row,
                                      int column_start, int column_end) {
  assert(mat);
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column_start && column_end <= Matrix_width(mat));
  assert(column_start < column_end);
  
  //initialize minimum column and value to first element in row
  int minimum_col = column_start;
  int minimum_value = *Matrix_at(mat, row, column_start);

  //iterate through each element in the given row and constraints
  for(int col = column_start + 1; col < column_end; ++col){
    //if element is less than minimum value, update minimum value and column
    if(*Matrix_at(mat, row, col) < minimum_value){
      minimum_value = *Matrix_at(mat, row, col);
      minimum_col = col;
    }
  }
  //return column of minimum col only
  return minimum_col;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, int row,
                            int column_start, int column_end) {
  assert(mat);
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column_start && column_end <= Matrix_width(mat));
  assert(column_start < column_end);

  //initialize minimum value to first element in row
  int minimum_value = *Matrix_at(mat, row, column_start);

  //iterate through each element in the given row and constraints
  for (int col = column_start + 1; col < column_end; ++col){
    //if element is less than minimum value, update minimum value
    if (*Matrix_at(mat, row, col) < minimum_value){
      minimum_value = *Matrix_at(mat, row, col);
    }
  }
  //return minimum value
  return minimum_value;
}
