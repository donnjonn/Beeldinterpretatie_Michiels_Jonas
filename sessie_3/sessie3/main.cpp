
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void maxLocs(const Mat& src, queue<Point>& dst, size_t size)
{
    float maxValue = -1.0f * numeric_limits<float>::max();
    float* srcData = reinterpret_cast<float*>(src.data);

    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 0; j < src.cols; j++)
        {
            if(srcData[i*src.cols + j] > maxValue)
            {
                maxValue = srcData[i*src.cols + j];

                dst.push(Point(j, i));

                // pop the smaller one off the end if we reach the size threshold.
                if(dst.size() > size)
                {
                    dst.pop();
                }
            }
        }
    }
}

Mat image; Mat templ; Mat result;
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
    MatchingMethod(0,0);
    waitKey(0);
    return 0;
}

void MatchingMethod( int, void* )
{
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

    return;
}

