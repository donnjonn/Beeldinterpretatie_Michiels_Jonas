#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<Point2d> fg_points;
vector<Point2d> bg_points;
vector<Point2d> points;
Mat img, labels, data;
int mode = 0;
int lastp = 0;
void on_tb(int, void*){
    Mat result = img.clone();
    for(int i=0; i < fg_points.size(); i++) {
        circle(result, fg_points.at(i), 5, Scalar(0, 0, 255), -1);
    }
    for(int i=0; i < bg_points.size(); i++) {
        circle(result, bg_points.at(i), 5, Scalar(0, 255, 0), -1);
    }

    imshow("Aardbei", result);
}
void CallBack(int event, int x, int y, int flags, void* userdata) {
    if  ( event == EVENT_LBUTTONDOWN ){
        cout << "Left click at: " << x << ", " << y << endl;
        if(mode) {
            bg_points.push_back(Point2d(x, y));
            lastp = 1;
        }
        else {
            fg_points.push_back(Point2d(x, y));
            lastp = 0;
        }
        points.push_back(Point2d(x, y));
    }
    else if  ( event == EVENT_RBUTTONDOWN ){
        cout << "Right click" << endl;
        points.pop_back();
        if(lastp == 1){
            bg_points.pop_back();
        }
        else{
            fg_points.pop_back();
        }
    }
    else if  ( event == EVENT_MBUTTONDOWN ){
        cout << "Middle click" << endl;
        for (int i = 0; i < points.size(); i++) {
            cout << points.at(i) << endl;
        }
    }
    on_tb(0,NULL);
}
void desc() {
    Mat hsv;
    Mat hsvpixels(points.size(), 3, CV_32FC1);
    cvtColor(img, hsv, CV_BGR2HSV);
    Mat fg_labels = Mat::ones(fg_points.size(), 1, CV_32SC1);
    Mat bg_labels = Mat::zeros(bg_points.size(), 1, CV_32SC1);
    for(int i=0; i < points.size(); i++) {
        Vec3b pixel = hsv.at<Vec3b>(points.at(i).y, points.at(i).x);
        hsvpixels.at<float>(i, 0) = pixel[0];
        hsvpixels.at<float>(i, 1) = pixel[1];
        hsvpixels.at<float>(i, 2) = pixel[2];
        cout << hsvpixels.at<float>(i, 0) << ", " << hsvpixels.at<float>(i, 1) << ", " << hsvpixels.at<float>(i, 2) << endl;
    }
    data = hsvpixels.clone();
    vconcat(fg_labels, bg_labels, labels);
    cout << hsvpixels.size() << endl;
}
void KNN(Mat data, Mat labels) {
    Ptr<ml::KNearest> knn = ml::KNearest::create();
    knn->setDefaultK(3);
    knn->setIsClassifier(true);
    knn->setAlgorithmType(ml::KNearest::BRUTE_FORCE);
    knn->train(data, ml::ROW_SAMPLE, labels);
    cout << "KNN trained" << endl;
}
void NB(Mat data, Mat labels) {
    Ptr<ml::NormalBayesClassifier> nb = ml::NormalBayesClassifier::create();
    nb->train(data, ml::ROW_SAMPLE, labels);
    cout << "NB trained" << endl;
}
void SVM(Mat data, Mat labels) {
    Ptr<ml::SVM> svm = ml::SVM::create();
    svm->setKernel(ml::SVM::LINEAR);
    svm->setType(ml::SVM::C_SVC);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
    svm->train(data, ml::ROW_SAMPLE, labels);
    cout << "SVM trained" << endl;
}
int main(int argc, const char** argv) {
    CommandLineParser parser(argc, argv,
                             "{ help h usage ? | | Shows this message.}"
                             "{ strawberry s   | | Aardbei afbeelding}"
    );
    string strawberry(parser.get<string>("s"));
    img = imread(strawberry, CV_LOAD_IMAGE_COLOR);
    namedWindow("Aardbei", WINDOW_AUTOSIZE);
    setMouseCallback("Aardbei", CallBack, NULL);
    createTrackbar("Mode:\nvoorgrond\nachtergrond", "Aardbei", &mode, 1, on_tb);
    on_tb(0, NULL);
    waitKey(0);
    desc();
    KNN(data, labels);
    NB(data, labels);
    SVM(data, labels);
    return 0;
}

