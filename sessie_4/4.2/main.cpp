#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
Mat KPDraw(Mat image, Ptr<Feature2D> detector) {
    vector<KeyPoint> keypoints;
    Mat output = image.clone();
    detector->detect(output, keypoints);
    drawKeypoints(output, keypoints, output);

    return output;
}
Mat MDraw(Mat image, Mat tmp, Ptr<Feature2D> detector) {
    vector<KeyPoint> kptempl;
    vector<KeyPoint> kpimage;
    vector<DMatch> matches;
    Mat dtempl;
    Mat dimage;
    Mat newimage = image.clone();
    Mat newtempl = tmp.clone();
    Mat result;
    detector->detectAndCompute(newimage, Mat(), kpimage, dimage);
    detector->detectAndCompute(newtempl, Mat(), kptempl, dtempl);
    BFMatcher matcher = BFMatcher(NORM_L2);
    matcher.match(dtempl, dimage, matches);
    drawMatches(newtempl, kptempl, newimage, kpimage, matches, result);
    return result;
}

int main(int argc, const char** argv) {
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
    Mat ORBtmp = KPDraw(templ, detectorORB);
    Mat ORBimage = KPDraw(image, detectorORB);
    Mat matchimage = MDraw(image,templ,detectorORB);
    namedWindow("template ORB", WINDOW_AUTOSIZE);
    namedWindow("image ORB", WINDOW_AUTOSIZE);
    namedWindow("matches ORB", WINDOW_AUTOSIZE);
    imshow("template ORB", ORBtmp);
    imshow("image ORB", ORBimage);
    imshow("matches ORB", matchimage);
    waitKey(0);
    return 0;
}
