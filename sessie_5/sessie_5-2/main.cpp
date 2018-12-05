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
void desc(Mat img, vector<Point2d> points) {
    Mat hsv;
    Mat hsvpixels(points.size(), 3, CV_32FC1);
    cvtColor(img, hsv, CV_BGR2HSV);
    for(int i=0; i < points.size(); i++) {
        Vec3b pixel = hsv.at<Vec3b>(points.at(i).y, points.at(i).x);
        hsvpixels.at<float>(i, 0) = pixel[0];
        hsvpixels.at<float>(i, 1) = pixel[1];
        hsvpixels.at<float>(i, 2) = pixel[2];
        cout << hsvpixels.at<float>(i, 0) << ", " << hsvpixels.at<float>(i, 1) << ", " << hsvpixels.at<float>(i, 2) << endl;
    }
    cout << hsvpixels.size() << endl;
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
    desc(img, points);
    return 0;
}

