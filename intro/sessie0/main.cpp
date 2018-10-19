#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int argc, const char** argv){
    const string keys =
        "{help h usage ? |      | print this message   }"
        "{image1 i1 | |path to image1}"
        "{image2 i2 | |path to image2}"
        ;
    CommandLineParser parser(argc, argv, keys);
    parser.about("Application name v1.0.0");
    string img1_loc(parser.get<string>("i1"));
    string img2_loc(parser.get<string>("i2"));
    if (parser.has("help") or img1_loc.empty()) //check for help or empty argument
    {
        parser.printMessage();
        return 0;
    }
    cerr << img1_loc << endl;
    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }
    Mat image1;
    Mat image2;
    image1 = imread(img1_loc, CV_LOAD_IMAGE_COLOR);
    image2 = imread(img2_loc, CV_LOAD_IMAGE_GRAYSCALE);
    if(! image1.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the first image" << std::endl ;
        return -1;
    }
     if(! image2.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the second image" << std::endl ;
        return -1;
    }
    imshow("",image1);
    imshow("",image2);
    waitKey(0);
}
