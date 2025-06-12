#include <cassert>
#include <vector>
#include "processing.hpp"
#include <algorithm> 

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image* img) {

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(&aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }
  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image* img){

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(&aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2) {
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows
  // later on in the algorithm.
  return (dr*dr + dg*dg + db*db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// ------------------------------------------------------------------
// You may change code below this line!


// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.

void compute_energy_matrix(const Image *img, Matrix *energy) {
    //declare width and height of image
    int width = Image_width(img);  
    int height = Image_height(img);  

    //intialize matrix
    Matrix_init(energy, width, height);

    //initialize max energy to 0
    int max_energy = 0;

    //everything but borders
    for (int i = 1; i < height - 1; ++i) {
        for (int j = 1; j < width - 1; ++j) {
          //get north, south, west, east pixels
            Pixel N = Image_get_pixel(img, i - 1, j);  
            Pixel S = Image_get_pixel(img, i + 1, j);   
            Pixel W = Image_get_pixel(img, i, j - 1);  
            Pixel E = Image_get_pixel(img, i, j + 1);  

            //calculate energy value at pixel 
            int energy_at_pix = squared_difference(N, S) + squared_difference(W, E);

            // Set the energy value for the pixel in the matrix
            //using matrix_at because single value of energy instead of 
            //image_get_pixel (RGB input)
            *Matrix_at(energy, i, j) = energy_at_pix;

            // update the max energy if needed
            if (energy_at_pix > max_energy) {
                max_energy = energy_at_pix;
            }
        }
    }

    //iterate through border of the energy matrix
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            //if on border, set energy to max energy
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                *Matrix_at(energy, i, j) = max_energy;
            }
        }
    }
}

// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix* energy, Matrix *cost) {
  assert(energy);
  assert(cost);
  
  //initializing width and height given matrix values for energy
  int width = Matrix_width(energy); 
  int height = Matrix_height(energy);
  
  Matrix_init(cost, width, height); 
  
  for (int j = 0; j < width; ++j){ // filling in cost for 1st row with energy
    *Matrix_at(cost, 0, j) = *Matrix_at(energy, 0, j); // setting the cost to the energy 
  }
  double min_cost = 1e30; //arbritary large number that we will update
  //iterate through everything except first row
  for (int i = 1; i < height; ++i){
    for(int j = 0; j < width; ++j){
      
      //if (j > 0 && j < width - 1){
        //if not in first or last column, check left, right, and up
        //doing j+2 because exclusive [j-1, j+2)
       // min_cost = Matrix_min_value_in_row(cost, i-1, j-1, j+2);  
      //}
      if (j == 0){
        //if in first col, check right and up
        //find min value in previous row [0,2)
        min_cost = Matrix_min_value_in_row(cost, i-1, 0, 2); 
      }
      else if (j == width - 1){
        //if in last col, check left and up
        //find min value in previous row between [width -2, width)
        min_cost = Matrix_min_value_in_row(cost, i-1, width - 2, width); 
      }
      else{
        //if not in first or last column, check left, right, and up
        min_cost = Matrix_min_value_in_row(cost, i-1, j-1, j+2);
      }
      // using equation to compute each cost
      *Matrix_at(cost, i, j)= *Matrix_at(energy, i, j)+ min_cost; 
    }
  }
}


// REQUIRES: cost points to a valid Matrix
// EFFECTS:  Returns the vertical seam with the minimal cost according to the given
//           cost matrix, represented as a vector filled with the column numbers for
//           each pixel along the seam, with index 0 representing the lowest numbered
//           row (top of image). The length of the returned vector is equal to
//           Matrix_height(cost).
//           While determining the seam, if any pixels tie for lowest cost, the
//           leftmost one (i.e. with the lowest column number) is used.
//           See the project spec for details on computing the minimal seam.
//           Note: When implementing the algorithm, compute the seam starting at the
//           bottom row and work your way up.
vector<int> find_minimal_vertical_seam(const Matrix* cost) {
  assert(cost);
  //create vector to store seam (column indexes with min cost)
  std::vector<int> seam;

  //find minimum cost pixel in last row
  //goes through last row of matrix, from column 0 to width - 1 
  int min_col = Matrix_column_of_min_value_in_row(cost, Matrix_height(cost)-1, 0, 
  Matrix_width(cost)); 
  seam.push_back(min_col);
  // finding the min cost of each pixel in the row above 
  for(int i= Matrix_height(cost)-2; i>=0; i--){
    int column = seam.back(); //gets last element
    int optimal_column = 0;
   
  if (column == 0){
      //if in the first column, check the first two columns
      optimal_column = Matrix_column_of_min_value_in_row(cost, i, 0, 2); 
    }
  //if in the last column, check the last two columns
  else if (column == Matrix_width(cost)-1){ 
      optimal_column = Matrix_column_of_min_value_in_row(cost, i, 
      Matrix_width(cost)-2, Matrix_width(cost)); 
    }
  else{
    //if not in first or last col, check the 3 columns
    optimal_column = Matrix_column_of_min_value_in_row(cost, i, column-1, column+2); 
  }
  //add the optimal column to the seam
  seam.push_back(optimal_column); 
  }
  
  //reverse seam from bottom to top to top to bottom to fit requirements 
  reverse(seam.begin(), seam.end());
  return seam;
}



// REQUIRES: img points to a valid Image with width >= 2
//           seam.size() == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with column equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Declare a new variable to hold the smaller Image, and
//           then do an assignment at the end to copy it back into the
//           original image.
void remove_vertical_seam(Image *img, const vector<int> &seam) {
  assert(img);
  assert(seam.size() == Image_height(img));
  //get old width and height of image
  int old_width = Image_width(img);
  int height = Image_height(img);
  //new width is one less than old width, remove pixel from each row
  int new_width = old_width - 1;

  Image aux_img;
  //make auxilliary image one column less
  Image_init(&aux_img, new_width, height);

  for (int i = 0; i < height; ++i){
    int seam_col = seam[i]; //get column index from seam vector at row i
    assert(0 <= seam_col && seam_col < old_width);

    //everything left of seam_col, copies pixels directly in no shifting
    for (int j = 0; j < seam_col; ++j){
      Pixel pix = Image_get_pixel(img, i, j);
      Image_set_pixel(&aux_img, i, j, pix);
    }
    //everything right of seam_col, shift left by 1 column
    for(int j = seam_col + 1; j< old_width; ++j){ // might be new_width
      Pixel pix = Image_get_pixel(img, i, j);
      Image_set_pixel(&aux_img, i, j-1, pix);
      
    }
  }
  *img = aux_img; //copies small auxilliary image back into original image
}


// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
// NOTE:     Use a vector to hold the seam, and make sure that it has
//           the right size. You can use .data() on a vector to get
//           the underlying array.
void seam_carve_width(Image *img, int newWidth) {
  assert(img);
  assert(newWidth > 0 && newWidth <= Image_width(img));
  
  int current_width = Image_width(img);
  while (current_width > newWidth){
    //compute energy matrix for current image 
    Matrix energy;
    compute_energy_matrix(img, &energy);
    //compute cost matrix for current image
    Matrix cost;
    //find minimal cost seam
    compute_vertical_cost_matrix(&energy, &cost);
    vector<int> seam = find_minimal_vertical_seam(&cost);
    //remove that seam
    remove_vertical_seam(img, seam);
    //update current width after removing minimal cost seam (-1)
    --current_width;
  }
}

// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight) {
  assert(img);
  assert(newHeight > 0 && newHeight <= Image_height(img));
  // rotate image left by 90 degrees
  rotate_left(img);
  //Apply seam_carve_width
  seam_carve_width(img, newHeight);
  //rotate image right by 90 degrees
  rotate_right(img);   
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight) {
  assert(img);
  assert(0 < newWidth && newWidth <= Image_width(img));
  assert(0 < newHeight && newHeight <= Image_height(img));
  
  //APPLY seam_carve_width and seam_carve_height
  seam_carve_width(img, newWidth);
  seam_carve_height(img, newHeight); 
}