#include <opencv2/opencv.hpp>
#include <iostream>
#define MIN_DISTANCES 3
#define RNG_INIT 12345
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
    //drawMatches(newtempl, kptempl, newimage, kpimage, matches, result);
    double max_dist = 0; double min_dist = 100;
    // Find min and max distance
    for(int i=0; i < dtempl.rows; i++) {
        double distance = matches.at(i).distance;
        if(distance == 0) {
            continue;
        }
        if(distance < min_dist) {
            min_dist = distance;
        }
        if(distance > max_dist) {
            max_dist = distance;
        }
    }
    vector< DMatch > good_matches;
    for(int i=0; i < dtempl.rows; i++ ){
        if(matches.at(i).distance < MIN_DISTANCES * min_dist){
            good_matches.push_back(matches.at(i));
        }
    }
    drawMatches(newtempl, kptempl, newimage, kpimage, good_matches, result);

    vector<Point2f> obj;
    vector<Point2f> scene;

    for(int i=0; i < good_matches.size(); i++)
    {
        obj.push_back(kptempl[good_matches.at(i).queryIdx].pt);
        scene.push_back(kpimage[good_matches.at(i).trainIdx].pt);
    }

    Mat H = findHomography(obj, scene, RANSAC);

    vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint(newtempl.cols, 0 );
    obj_corners[2] = cvPoint(newtempl.cols, newtempl.rows);
    obj_corners[3] = cvPoint(0, newtempl.rows);
    std::vector<Point2f> scene_corners(4);
    perspectiveTransform(obj_corners, scene_corners, H);
    line(result, scene_corners[0] + Point2f(newtempl.cols, 0), scene_corners[1] + Point2f(newtempl.cols, 0), Scalar(0,255,0), 4);
    line(result, scene_corners[1] + Point2f(newtempl.cols, 0), scene_corners[2] + Point2f(newtempl.cols, 0), Scalar(0,255,0), 4);
    line(result, scene_corners[2] + Point2f(newtempl.cols, 0), scene_corners[3] + Point2f(newtempl.cols, 0), Scalar(0,255,0), 4);
    line(result, scene_corners[3] + Point2f(newtempl.cols, 0), scene_corners[0] + Point2f(newtempl.cols, 0), Scalar(0,255,0), 4);

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
