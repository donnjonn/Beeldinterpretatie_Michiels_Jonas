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
                             "{help h||}");
    VideoCapture capture;
    //-- 2. Read the video stream
    capture.open("people.mp4");
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

    vector<Rect> people;
    vector<double> weights;

    // HOG detection
    hog.detectMultiScale(img, people, weights);

    // Draw detections
    for(int i=0; i < people.size(); ++i) {
        if(weights.at(i) >= 0.1) {
            Point facesCenter = Point(people.at(i).x + people.at(i).width / 2,
                                      people.at(i).y + people.at(i).height / 2);
            circle(img, facesCenter, 5, Scalar(255, 0, 255));
            Rect box = Rect(Point(people.at(i).x, people.at(i).y),
                    Point(people.at(i).x + people.at(i).width, people.at(i).y + people.at(i).height));
            rectangle(img, box, Scalar(255, 0, 255));
            stringstream convert;
            convert << weights.at(i);
            string text = convert.str();
            putText(img, text, Point(people.at(i).x, people.at(i).y), FONT_HERSHEY_DUPLEX, 0.8,
                    Scalar(255, 255, 255));
        }
    }

    imshow("Voetganger detection", img);
}

