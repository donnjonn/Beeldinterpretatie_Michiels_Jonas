#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int morph_elem = 0;
int morph_size = 3;
int main(int argc, const char** argv){
    string img_loc("imageColorAdapted.jpg");
    cerr << img_loc << endl;
    Mat image;
    Mat imageeq;
    Mat imageCLAHE;
    vector<Mat> imageBGR;
    vector<vector<Point> > contours;
    vector<vector<Point> > hull;
    vector<Vec4i> hierarchy;
    image = imread(img_loc, CV_LOAD_IMAGE_COLOR);
    Mat filter = Mat::zeros(image.rows,image.cols, CV_8UC1);
    Mat filter2 = Mat::zeros(image.rows,image.cols, CV_8UC1);
    Mat open1 = Mat::zeros(image.rows,image.cols, CV_8UC1);
    Mat close1 = Mat::zeros(image.rows,image.cols, CV_8UC1);
    Mat contour = Mat::zeros(image.rows,image.cols, CV_8UC1);

    if(!image.data)                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl;
    }
    if(!image.empty()){
        imshow("",image);
        waitKey();
    }
    split(image,imageBGR);
    imshow("",imageBGR[0]);
    waitKey();
    imshow("",imageBGR[1]);
    waitKey();
    imshow("",imageBGR[2]);
    waitKey();
    Mat B = imageBGR[0];
    Mat G = imageBGR[1];
    Mat R = imageBGR[2];
    for( int i = 0; i < image.rows; i++)
    {
        for ( int j = 0; j < image.cols; j++)
        {
            if((R.at<uchar>(i,j)>95) && (G.at<uchar>(i,j)>40) && (B.at<uchar>(i,j)>20) && ((max(R.at<uchar>(i,j),max(G.at<uchar>(i,j),B.at<uchar>(i,j))) - min(R.at<uchar>(i,j),min(G.at<uchar>(i,j),B.at<uchar>(i,j))))>15) && (abs(R.at<uchar>(i,j)-G.at<uchar>(i,j))>15) && (R.at<uchar>(i,j)>G.at<uchar>(i,j)) && (R.at<uchar>(i,j)>B.at<uchar>(i,j))){
                filter.at<uchar>(i,j) = 255;
            }
        }
    }
    filter2 = (R>95) & (G>40) & (B>20) & ((max(R,max(G,B)) - min(R,min(G,B)))>15) & (abs(R-G)>15) & (R>G) & (R>B);
    filter2 = filter2*255;

    imshow("filter",filter);
    waitKey();
    imshow("filter2",filter2);
    waitKey();
    // De ruis is zichtbaar op het getresholde beeld.
    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx(filter, open1, MORPH_OPEN, element); // ruis weg
    imshow("open1",open1);
    waitKey();
    morphologyEx(open1, close1, MORPH_CLOSE, element); //Duim en hoofd terug verbinden
    imshow("close1",close1);
    waitKey();
    findContours(close1,contours,hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    convexHull(contours,hull,true,true);
    drawContours(contour,hull,-1,255);
    //imshow("contours",contour);
    waitKey();
    return 0;
}
