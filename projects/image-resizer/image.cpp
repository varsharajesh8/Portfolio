#include <cassert>
#include "Image.hpp"

// REQUIRES: img points to an Image
//           0 < width && 0 < height
// MODIFIES: *img
// EFFECTS:  Initializes the Image with the given width and height, with
//           all pixels initialized to RGB values of 0.
void Image_init(Image* img, int width, int height) {
  assert(img);
  assert(0 < width && 0 < height);

  //get width and height (members) from pointer to struct
  img->width = width;
  img->height = height;

  //initialize all color channels to width and height
  Matrix_init(&img->red_channel, width, height);
  Matrix_init(&img->green_channel, width, height);
  Matrix_init(&img->blue_channel, width, height);

  //initialize all color channels to 0
  //do we need this step?
  //for(int i = 0; i < height; ++i){
    //for(int j = 0; j < width; ++j){
      //*Matrix_at(&img->red_channel, i, j) = 0;
      //*Matrix_at(&img->green_channel, i, j) = 0;
      //*Matrix_at(&img->blue_channel, i, j) = 0;
    //}
  //}
}
// REQUIRES: img points to an Image
//           is contains an image in PPM format without comments
//           (any kind of whitespace is ok)
// MODIFIES: *img, is
// EFFECTS:  Initializes the Image by reading in an image in PPM format
//           from the given input stream.
// NOTE:     See the project spec for a discussion of PPM format.
void Image_init(Image* img, std::istream& is) {
  assert(img);
  //do we need to check if no comments

  //declaring header and max val
  std::string header;
  int max_val = 255;

  //read in header
  is >> header;
  assert(header == "P3");

  //reading in width, height, and max_val
  int width, height;
  is >> width >> height >> max_val; 

  //initialize image with width and height
  img->width = width;
  img->height = height;

  //initialize color channels with width and height
  Matrix_init(&img->red_channel, width, height);
  Matrix_init(&img->green_channel, width, height);
  Matrix_init(&img->blue_channel, width, height);

  //read in RGB values for each pixel and set them in the image
  for (int i = 0; i < height; ++i){
    for (int j = 0; j < width; ++j){
      int red, green, blue;
      is >> red >> green >> blue;
      *Matrix_at(&img->red_channel, i, j) = red;
      *Matrix_at(&img->green_channel, i, j) = green;
      *Matrix_at(&img->blue_channel, i, j) = blue;
    }
  }
}

// REQUIRES: img points to a valid Image
// MODIFIES: os
// EFFECTS:  Writes the image to the given output stream in PPM format.
//           You must use the kind of whitespace specified here.
//           First, prints out the header for the image like this:
//             P3 [newline]
//             WIDTH [space] HEIGHT [newline]
//             255 [newline]
//           Next, prints out the rows of the image, each followed by a
//           newline. Each pixel in a row is printed as three ints
//           for its red, green, and blue components, in that order. Each
//           int is followed by a space. This means that there will be an
//           "extra" space at the end of each line. See the project spec
//           for an example.
void Image_print(const Image* img, std::ostream& os) {
  assert(img);
  //print header (always the same)
  os << "P3\n" << img->width << " " << img->height << "\n255\n";

  //print each pixel in each row (RGB vals)
  for (int i = 0; i < img->height; ++i){
    for (int j = 0; j < img->width; ++j){
      os << *Matrix_at(&img->red_channel, i, j) << " ";
      os << *Matrix_at(&img->green_channel, i, j) << " ";
      os << *Matrix_at(&img->blue_channel, i, j) << " ";
    }
    //print new line after each row
    os << "\n";
  }
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the width of the Image.
int Image_width(const Image* img) {
  assert(img);
  return img->width;
  }

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the height of the Image.
int Image_height(const Image* img) {
  assert(img); 
  return img-> height; 
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// EFFECTS:  Returns the pixel in the Image at the given row and column.
Pixel Image_get_pixel(const Image* img, int row, int column) {
  assert(img);
  assert(0 <= row && row < Image_height(img));
  assert(0 <= column && column < Image_width(img));
  //return RGB vals of pixel at given row and column
  return Pixel{*Matrix_at(&img->red_channel, row, column),
               *Matrix_at(&img->green_channel, row, column),
               *Matrix_at(&img->blue_channel, row, column)};
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Sets the pixel in the Image at the given row and column
//           to the given color.
void Image_set_pixel(Image* img, int row, int column, Pixel color) {
  assert(img); 
  assert(0 <= row && row < Image_height(img));
  assert(0 <= column && column < Image_width(img));
  //set RGB vals of pixel at given row and column to given color's RGB vals
  *Matrix_at(&img->red_channel, row, column) = color.r;
  *Matrix_at(&img->green_channel, row, column) = color.g;
  *Matrix_at(&img->blue_channel, row, column) = color.b;
}

// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  Sets each pixel in the image to the given color.
void Image_fill(Image* img, Pixel color) {
  assert(img);
  //iterate through each pixel in image with nested for loop
  for(int i = 0; i < Image_height(img); ++i){
    for(int j = 0; j < Image_width(img); ++j){
      //set each pixel to given color
      Image_set_pixel(img, i, j, color);
    }
  }
}
