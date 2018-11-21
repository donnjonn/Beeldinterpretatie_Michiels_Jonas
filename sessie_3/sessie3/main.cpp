
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;



Mat image; Mat templ; Mat result;
Point center;
char* image_window = "Source Image";
char* result_window = "Result window";

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
    string img_loc(parser.get<string>("i"));
    cerr << img_loc << endl;
    string templ_loc(parser.get<string>("s"));
    cerr << templ_loc << endl;
    image = imread(img_loc, CV_LOAD_IMAGE_COLOR);
    templ = imread(templ_loc, CV_LOAD_IMAGE_COLOR);
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );
    char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );
    center.x = input_image.cols / 2;
    center.y = input_image.rows / 2;
    MatchingMethod(0,0);
    waitKey(0);
    return 0;
}
vector<vector<Point> > match(Mat input_image)
{
    Mat result;
    matchTemplate(input_image, templ, result, match_method);
//    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
        result = 1 - result;

//    double maxVal;
//    minMaxLoc(result, NULL, &maxVal);
    Mat mask = Mat::zeros(Size(input_image.cols, input_image.rows), CV_8UC1);
    inRange(result, ((double)threshold_percent/100.0), 1, mask);

    vector<vector<Point> > corners;
    vector<vector<Point> > contours;
    findContours(mask, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
    for (int i = 0; i < contours.size(); ++i)
    {
        vector<Point> hull;
        convexHull(contours[i], hull);
        Rect rect = boundingRect(hull);
//        rectangle(mask, rect.tl(), rect.br(), Scalar::all(50), 2, 8, 0);

        Point loc;
        minMaxLoc(result(rect), NULL, NULL, NULL, &loc);

        Point c(loc.x + rect.x, loc.y + rect.y);

        vector<Point> points;
        points.push_back(c);
        points.push_back(c + Point(0, templ.rows));
        points.push_back(c + Point(templ.cols, templ.rows));
        points.push_back(c + Point(templ.cols, 0));
        corners.push_back(points);
    }

//    imshow("Mask", mask);
//    imshow("Image display", img_display);
//    imshow("Result", result);

    return corners;
}
void MatchingMethod( int, void* )
{
    Mat img_display(image.clone());
    vector<vector<Point> > corners = match(img_display);
    for (int i = 0; i < corners.size(); ++i)
        rectangle(img_display, corners[i][0], corners[i][2], Scalar::all(0));
    imshow("Image display", img_display);
    return;
    /*Mat output(input_image.clone());
    /// Source image to display
    Mat img_display;
    image.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  image.cols - templ.cols + 1;
    int result_rows = image.rows - templ.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( image, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    {
        result = 1.0 - result;
    }

    // get the top 100 maximums...
    queue<Point> locations;
    maxLocs(result, locations, 100);

    /// Show me what you got
    while(!locations.empty())
    {
        Point matchLoc = locations.front();
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
        rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
        locations.pop();
    }

    imshow( image_window, img_display );
    imshow( result_window, result );

    return;*/
}

