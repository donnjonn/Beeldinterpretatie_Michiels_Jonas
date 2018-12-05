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
        circle(result, fg_points.at(i), 5, Scalar(255, 0, 255), -1);
    }
    for(int i=0; i < bg_points.size(); i++) {
        circle(result, bg_points.at(i), 5, Scalar(0, 255, 255), -1);
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
    Mat labelKNN;
    Mat maskKNN = Mat::zeros(img.rows, img.cols, CV_8UC1);
    Mat resultKNN;
    Mat HSV = img.clone();
    cvtColor(HSV, HSV, CV_BGR2HSV);
    for(int r=0; r < HSV.rows; r++) {
        for(int c=0; c < HSV.cols; c++) {
            Vec3b pixel = HSV.at<Vec3b>(r, c);
            Mat pixelMat = Mat(1, 3,CV_32FC1);
            pixelMat.at<float>(0, 0) = pixel[0];
            pixelMat.at<float>(0, 1) = pixel[1];
            pixelMat.at<float>(0, 2) = pixel[2];
            knn->predict(pixelMat, labelKNN);
            if (labelKNN.at<float>(0, 0)) {
                maskKNN.at<uchar>(r, c) = 1;
            }
        }
    }
    maskKNN = maskKNN * 255;
    img.copyTo(resultKNN, maskKNN);
    imshow("Mask KNN", maskKNN);
    imshow("Result KNN", resultKNN);
    waitKey(0);
}
void NB(Mat data, Mat labels) {
    Ptr<ml::NormalBayesClassifier> nb = ml::NormalBayesClassifier::create();
    nb->train(data, ml::ROW_SAMPLE, labels);
    cout << "NB trained" << endl;
    Mat labelNB;
    Mat maskNB = Mat::zeros(img.rows, img.cols, CV_8UC1);
    Mat resultNB;
    Mat HSV = img.clone();
    cvtColor(HSV, HSV, CV_BGR2HSV);
    for(int r=0; r < HSV.rows; r++) {
        for(int c=0; c < HSV.cols; c++) {
            Vec3b pixel = HSV.at<Vec3b>(r, c);
            Mat pixelMat = Mat(1, 3,CV_32FC1);
            pixelMat.at<float>(0, 0) = pixel[0];
            pixelMat.at<float>(0, 1) = pixel[1];
            pixelMat.at<float>(0, 2) = pixel[2];
            nb->predict(pixelMat, labelNB);
            if (labelNB.at<float>(0, 0)) {
                maskNB.at<uchar>(r, c) = 1;
            }
        }
    }
    maskNB = maskNB * 255;
    img.copyTo(resultNB, maskNB);
    imshow("Mask NB", maskNB);
    imshow("Result NB", resultNB);
    waitKey(0);
}
void SVM(Mat data, Mat labels) {
    Ptr<ml::SVM> svm = ml::SVM::create();
    svm->setKernel(ml::SVM::LINEAR);
    svm->setType(ml::SVM::C_SVC);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
    svm->train(data, ml::ROW_SAMPLE, labels);
    cout << "SVM trained" << endl;
    Mat labelSVM;
    Mat maskSVM = Mat::zeros(img.rows, img.cols, CV_8UC1);
    Mat resultSVM;
    Mat HSV = img.clone();
    cvtColor(HSV, HSV, CV_BGR2HSV);
    for(int r=0; r < HSV.rows; r++) {
        for(int c=0; c < HSV.cols; c++) {
            Vec3b pixel = HSV.at<Vec3b>(r, c);
            Mat pixelMat = Mat(1, 3,CV_32FC1);
            pixelMat.at<float>(0, 0) = pixel[0];
            pixelMat.at<float>(0, 1) = pixel[1];
            pixelMat.at<float>(0, 2) = pixel[2];
            svm->predict(pixelMat, labelSVM);
            if (labelSVM.at<float>(0, 0)) {
                maskSVM.at<uchar>(r, c) = 1;
            }
        }
    }
    maskSVM = maskSVM * 255;
    img.copyTo(resultSVM, maskSVM);
    imshow("Mask SVM", maskSVM);
    imshow("Result SVM", resultSVM);
    waitKey(0);
}
int main(int argc, const char** argv) {
    CommandLineParser parser(argc, argv,
                             "{ help h usage ? | | Shows this message.}"
                             "{ strawberry s   | | Aardbei afbeelding }"
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

