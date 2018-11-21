
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;



Mat image; Mat templ; Mat result;
int thresholdv = 50;
int match_method;
int max_Trackbar = 5;


void MatchingMethod( int, void* );

int main(int argc, const char** argv){
const string keys =
        "{help h usage ? |      | print this message   }"
        "{image i | |path to image}"
        "{source s | |path}"
        ;
    CommandLineParser parser(argc, argv, keys);
    parser.about("Application name v1.0.0");
    if (!parser.check())
    {
        parser.printErrors();
        return -1;
    }
    string img_loc(parser.get<string>("i"));
    cerr << img_loc << endl;
    string templ_loc(parser.get<string>("s"));
    cerr << templ_loc << endl;
    image = imread(img_loc, CV_LOAD_IMAGE_COLOR);
    templ = imread(templ_loc, CV_LOAD_IMAGE_COLOR);
    namedWindow( "image_window", CV_WINDOW_AUTOSIZE );
    char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    createTrackbar( trackbar_label, "image_window", &match_method, max_Trackbar, MatchingMethod);
    createTrackbar("Threshold(0%-100%)", "image_window", &thresholdv, 100, MatchingMethod);
    MatchingMethod(0,0);
    waitKey(0);
    return 0;
}

void MatchingMethod( int, void* )
{
    double maxv, minv;
    Mat img_display(image.clone());
    Mat result;
    matchTemplate(image, templ, result, match_method);
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED){
        result = 1 - result; //Bij SQDIFF is lqger beter, qndere methodes is hoger beter
    }
    minMaxLoc(result, &minv, &maxv);
    Mat mask = Mat::zeros(Size(image.cols, image.rows), CV_32FC1);
    inRange(result, maxv * ((double)thresholdv/100.0), maxv, mask);
    mask.convertTo(mask,CV_8UC1);
    vector<vector<Point> > contours;
    findContours(mask, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
    for (int i = 0; i < contours.size(); ++i)
    {
        vector<Point> hull;
        convexHull(contours[i], hull);
        Rect rect = boundingRect(hull);
        Point loc;
        minMaxLoc(result(rect), NULL, NULL, NULL, &loc);

        Point c(loc.x + rect.x, loc.y + rect.y);
        rectangle(img_display, c, Point(c.x + templ.cols, c.y + templ.rows), Scalar(0, 0, 255));
    }
    imshow("image_window", img_display);
    return;

}


