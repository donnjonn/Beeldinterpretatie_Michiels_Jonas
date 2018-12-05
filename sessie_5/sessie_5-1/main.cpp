#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define KERNEL_SIZE 5
#define CIRCLE_THICKNESS -1
#define CIRCLE_RADIUS 5
vector<Point2d> points;
Mat img;
void CallBack(int event, int x, int y, int flags, void* userdata) {
    if  ( event == EVENT_LBUTTONDOWN ){
        cout << "Left click at: " << x << ", " << y << endl;
        points.push_back(Point2d(x, y));
    }
    else if  ( event == EVENT_RBUTTONDOWN ){
        cout << "Right click" << endl;
        points.pop_back();
    }
    else if  ( event == EVENT_MBUTTONDOWN ){
        cout << "Middle click" << endl;
        for (int i = 0; i < points.size(); i++) {
            cout << points.at(i) << endl;
        }
    }
    Mat result = img.clone();
    for(int i=0; i < points.size(); i++) {
        circle(result, points.at(i), CIRCLE_RADIUS, Scalar(0, 255, 0), CIRCLE_THICKNESS);
    }
    imshow("Aardbei", result);
}


int main(int argc, const char** argv) {
    CommandLineParser parser(argc, argv,
                             "{ help h usage ? | | Shows this message.}"
                             "{ strawberry s   | | Loads a strawberry image <REQUIRED> }"
    );
    string strawberry(parser.get<string>("s"));
    img = imread(strawberry, CV_LOAD_IMAGE_COLOR);
    namedWindow("Aardbei", WINDOW_AUTOSIZE);
    imshow("Aardbei", img);
    setMouseCallback("Aardbei", CallBack, NULL);
    waitKey(0);
    return 0;
}

