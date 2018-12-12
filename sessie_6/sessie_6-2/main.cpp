#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
void detectAndDisplay( Mat frame );
CascadeClassifier face_cascade;
int main( int argc, const char** argv )
{
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{face_cascade f||Path to face cascade.}");
    String face_cascade_name = parser.get<String>("f");
    if( !face_cascade.load( face_cascade_name ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
    VideoCapture capture;
    //-- 2. Read the video stream
    capture.open("faces.mp4");
    if ( ! capture.isOpened() )
    {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }
    Mat frame;
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }
        detectAndDisplay( frame );
        if( waitKey(10) == 27 )
        {
            break; // escape
        }
    }
    return 0;
}
void detectAndDisplay( Mat frame )
{
    const float scale_factor(1.2f);
    const int min_neighbours(3);
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    std::vector<Rect> faces;
    std::vector<int> reject_levels;
    std::vector<double> level_weights;
    face_cascade.detectMultiScale( frame_gray, faces, reject_levels, level_weights, scale_factor,min_neighbours,0,Size(),Size(),true );
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        putText(frame, std::to_string(level_weights[i]),
                        Point(faces[i].x, faces[i].y), 1, 1, Scalar(0,0,255));
        Mat faceROI = frame_gray( faces[i] );
    }
    //-- Show what you got
    imshow( "Capture - Face detection", frame );
}
