#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int argc, const char** argv){
    string img1_loc = "imageColor.jpg";
    string img2_loc = "imageBimodal.jpg";
    cerr << img1_loc << endl;
    cerr << img2_loc << endl;
    Mat image1;
    Mat image2;
    Mat image2eq;
    Mat imageCLAHE;
    vector<Mat> imageBGR;
    image1 = imread(img1_loc, CV_LOAD_IMAGE_COLOR);
    image2 = imread(img2_loc, CV_LOAD_IMAGE_GRAYSCALE);

    Mat filter = Mat::zeros(image1.rows,image1.cols, CV_8UC1);
    Mat filter2 = Mat::zeros(image1.rows,image1.cols, CV_8UC1);
    Mat imageOT = Mat::zeros(image2.rows,image2.cols, CV_8UC1);
    Mat imageOTeq = Mat::zeros(image2.rows,image2.cols, CV_8UC1);
    Mat imageOTCLAHE = Mat::zeros(image2.rows,image2.cols, CV_8UC1);
    if(! image1.data)                              // Check for invalid input
    {
        cout <<  "Could not open or find the first image" << std::endl;
    }
    if(! image2.data)                              // Check for invalid input
    {
        cout <<  "Could not open or find the second image" << std::endl;
    }
    if(!image1.empty()){
        imshow("",image1);
        waitKey();
    }
    split(image1,imageBGR);
    imshow("",imageBGR[0]);
    waitKey();
    imshow("",imageBGR[1]);
    waitKey();
    imshow("",imageBGR[2]);
    waitKey();
    Mat B = imageBGR[0];
    Mat G = imageBGR[1];
    Mat R = imageBGR[2];
    for( int i = 0; i < image1.rows; i++)
    {
        for ( int j = 0; j < image1.cols; j++)
        {
            if((R.at<uchar>(i,j)>95) && (G.at<uchar>(i,j)>40) && (B.at<uchar>(i,j)>20) && ((max(R.at<uchar>(i,j),max(G.at<uchar>(i,j),B.at<uchar>(i,j))) - min(R.at<uchar>(i,j),min(G.at<uchar>(i,j),B.at<uchar>(i,j))))>15) && (abs(R.at<uchar>(i,j)-G.at<uchar>(i,j))>15) && (R.at<uchar>(i,j)>G.at<uchar>(i,j)) && (R.at<uchar>(i,j)>B.at<uchar>(i,j))){
                filter.at<uchar>(i,j) = 255;
            }
        }
    }
    filter2 = (R>95) & (G>40) & (B>20) & ((max(R,max(G,B)) - min(R,min(G,B)))>15) & (abs(R-G)>15) & (R>G) & (R>B);
    filter2 = filter2*255;
    imshow("",filter);
    waitKey();
    imshow("",filter2);
    waitKey();
    imshow("",image2);
    waitKey();
    threshold(image2, imageOT, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    imshow("",imageOT); //Rechterkant wordt zwart
    waitKey();
    equalizeHist(image2,image2eq);
    imshow("",image2eq);
    waitKey();
    threshold(image2eq, imageOTeq, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    imshow("",imageOTeq); //Rechterkant wordt zwart
    waitKey();
    Ptr <CLAHE> clahe_pointer = createCLAHE();
    clahe_pointer->setTilesGridSize(Size(15,15));
    clahe_pointer->setClipLimit(1);
    clahe_pointer->apply(image2.clone(), imageCLAHE);
    imshow("",imageCLAHE);
    waitKey();
    threshold(imageCLAHE, imageOTCLAHE, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    imshow("",imageOTCLAHE);
    waitKey();
    return 0;
}
