#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
void detectAndDisplay( Mat frame, HOGDescriptor hog );
CascadeClassifier face_cascade;
int main( int argc, const char** argv )
{
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{gameplay g| |path to gameplay footage}"
                             );
    VideoCapture capture;
    //-- 2. Read the video stream
    string gp_loc(parser.get<string>("g"));
    cerr << gp_loc << endl;
    Mat zero = imread("zero.png", CV_LOAD_IMAGE_COLOR);
    Mat een = imread("een.png", CV_LOAD_IMAGE_COLOR);
    Mat twee = imread("twee.png", CV_LOAD_IMAGE_COLOR);
    Mat drie = imread("drie.png", CV_LOAD_IMAGE_COLOR);
    Mat vier = imread("vier.png", CV_LOAD_IMAGE_COLOR);
    Mat vijf = imread("vijf.png", CV_LOAD_IMAGE_COLOR);
    Mat zes = imread("zes.png", CV_LOAD_IMAGE_COLOR);
    Mat zeven = imread("zeven.png", CV_LOAD_IMAGE_COLOR);
    Mat acht = imread("acht.png", CV_LOAD_IMAGE_COLOR);
    Mat negen = imread("negen.png", CV_LOAD_IMAGE_COLOR);
    imshow("een",een);
    imshow("twee",twee);
    imshow("drie",drie);
    imshow("vier",vier);
    imshow("vijf",vijf);
    imshow("zes",zes);
    imshow("zeven",zeven);
    imshow("acht",acht);
    imshow("negen",negen);
    capture.open(gp_loc);
    if ( ! capture.isOpened() )
    {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }
    HOGDescriptor hog;
    vector<Point> trackers;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    Mat frame;
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }
        //resize(frame, frame, Size(SCALE*frame.cols, SCALE*frame.rows));
        detectAndDisplay(frame, hog);
        //imshow("video", frame);
        if( waitKey(10) == 27 )
        {
            break; // escape
        }
    }
    return 0;
}
void detectAndDisplay( Mat frame, HOGDescriptor hog )
{
    Mat img = frame.clone();


    imshow("Voetganger detection", img);
}

