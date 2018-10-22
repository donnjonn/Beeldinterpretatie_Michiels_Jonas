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
    Mat imageBGR[3];
    Mat kleurGW;
    Mat canvas(320, 240, CV_8UC3, Scalar(255,255,255));
    image1 = imread(img1_loc, CV_LOAD_IMAGE_COLOR);
    image2 = imread(img2_loc, CV_LOAD_IMAGE_GRAYSCALE);
    if(! image1.data)                              // Check for invalid input
    {
        cout <<  "Could not open or find the first image" << std::endl ;
    }
    if(! image2.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the second image" << std::endl ;
    }
    if(! image2.data && ! image1.data ) {
        return -1;
    }
    if(!image1.empty()){
        imshow("",image1);
        waitKey();
    }
    if(!image2.empty()){
        imshow("",image2);
        waitKey();
    }
    split(image1,imageBGR);
    imwrite("b.png",imageBGR[0]);
    imwrite("g.png",imageBGR[1]);
    imwrite("r.png",imageBGR[2]);
    imshow("",imageBGR[0]);
    waitKey();
    imshow("",imageBGR[1]);
    waitKey();
    imshow("",imageBGR[2]);
    waitKey();
    cvtColor(image1,kleurGW, COLOR_RGB2GRAY);
    imshow("",kleurGW);
    std::cout << kleurGW.type() << std::endl;
    //for( int i = 0; i < kleurGW.rows; ++i)
    //{
    //    for ( int j = 0; j < kleurGW.cols; ++j)
    //    {
    //        std::cout << (int)kleurGW.at<uchar>(i,j);
    //    }
    //    std::cout << std::endl;
    //}
    waitKey();
    circle(canvas, Point(100,100), 50, Scalar(0, 255, 0),6);
    rectangle(canvas, Point(130,120), Point(160,230), Scalar(125,125,0),5, LINE_4);
    arrowedLine(canvas, Point(130,120), Point(160,230), Scalar(125,0,125),3);
    imshow("",canvas);
    waitKey();
    return 0;
}
