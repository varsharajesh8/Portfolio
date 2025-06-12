#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include "Image.hpp"
#include "processing.hpp"

int main(int argc, char *argv[]) {
  if (argc !=4 && argc !=5){
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
     << "WIDTH and HEIGHT must be less than or equal to original" << endl;
    return 1;
  }

    //get filenames from command line 
    std::string filename_in = argv[1];
    std::string filename_out = argv[2];

    //initialize and convert wanted width to integer 
    int wanted_width = std::stoi(argv[3]);
    int wanted_height;
    //if height provided, convert to integer, otherwise set as -1 (impossible height)
    if (argc == 5){
        wanted_height = std::stoi(argv[4]);  
    }
    else{
        wanted_height = -1;
    }
    
    //open input file and check if it exists
    Image img;
    ifstream inputFile(filename_in);
    if (!inputFile.is_open()){
        cout << "Error opening file: " << filename_in << endl;
        return 1;
    }

    //initialize image with input file and get original width and height   
    Image_init(&img, inputFile);
    int original_width = Image_width(&img);
    int original_height = Image_height(&img);

    
    //check that wanted width and height are valid, if not print error message
    if(wanted_width <= 0 || wanted_width > original_width 
        || wanted_height > original_height || wanted_height <= 0){
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }

    //seam carve with width
    seam_carve_width(&img, wanted_width);

    //if height provided, do seam carve on height
    if (wanted_height != -1){
        seam_carve_height(&img, wanted_height);
    }
    //if not, set wanted height to the original height
    else{
        wanted_height = original_height;
    }

    //open output file 
    ofstream outputFile(filename_out);
    if (!outputFile.is_open()){
        cout << "Error opening file: " << filename_out << endl;
        return 1;
    }

    //print resized image to output file
    Image_print(&img, outputFile);
    outputFile.close();

    return 0; 
}

