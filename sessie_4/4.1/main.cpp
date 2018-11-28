
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


const int MAX_FEATURES = 500;
const float GOOD_MATCH_PERCENT = 0.15f;


Mat KPDraw(Mat input, Ptr<Feature2D> detector) {
    vector<KeyPoint> keypoints;
    Mat output = input.clone();
    detector->detect(output, keypoints);
    drawKeypoints(output, keypoints, output);

    return output;
}
int main(int argc, const char** argv){
const string keys =
        "{help h usage ? |      | print this message   }"
        "{image i | |path to image}"
        "{template t| |[path to template}"
        ;
    CommandLineParser parser(argc, argv, keys);
    parser.about("Application name v1.0.0");
    string img_loc(parser.get<string>("i"));
    cerr << img_loc << endl;
    string tmp_loc(parser.get<string>("t"));
    cerr << img_loc << endl;
    Mat image = imread(img_loc, CV_LOAD_IMAGE_GRAYSCALE);
    Mat templ = imread(tmp_loc, CV_LOAD_IMAGE_GRAYSCALE);
    namedWindow("template", WINDOW_AUTOSIZE);
    namedWindow("input", WINDOW_AUTOSIZE);
    imshow("template", templ);
    imshow("input", image);
    waitKey();
    Ptr<Feature2D> detectorORB = ORB::create();
    Ptr<Feature2D> detectorBRISK = BRISK::create();
    Ptr<Feature2D> detectorAKAZE = AKAZE::create();
    Mat ORBtmp = KPDraw(templ, detectorORB);
    Mat ORBimage = KPDraw(image, detectorORB);
    Mat BRISKtmp = KPDraw(templ, detectorBRISK);
    Mat BRISKimage = KPDraw(image, detectorBRISK);
    Mat AKAZEtmp = KPDraw(templ, detectorAKAZE);
    Mat AKAZEimage = KPDraw(image, detectorAKAZE);
    namedWindow("template ORB", WINDOW_AUTOSIZE);
    namedWindow("image ORB", WINDOW_AUTOSIZE);
    imshow("template ORB", ORBtmp);
    imshow("image ORB", ORBimage);
    waitKey();
    namedWindow("template BRISK", WINDOW_AUTOSIZE);
    namedWindow("image BRISK", WINDOW_AUTOSIZE);
    imshow("template BRISK", BRISKtmp);
    imshow("image BRISK", BRISKimage);
    waitKey();
    namedWindow("template AKAZE", WINDOW_AUTOSIZE);
    namedWindow("image AKAZE", WINDOW_AUTOSIZE);
    imshow("template AKAZE", AKAZEtmp);
    imshow("image AKAZE", AKAZEimage);
    waitKey(0);
    return 0;
}



