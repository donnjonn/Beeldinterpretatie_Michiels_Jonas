
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int morph_elem = 0;
int morph_size = 3;
const int hue_slider_max = 180;
int hueb_slider = 10;
int hueo_slider = 168;
const int sat_slider_max = 255;
int sat_slider = 150;
double alpha;
double beta;
Mat src1;
Mat dst;
void on_tb(int, void*){
    inRange(imageHSV[0],hueo_slider,180,h1);
    inRange(imageHSV[0],0,hueb_slider,h2);
    inRange(imageHSV[1],sat_slider,255,s1);
    Mat H = Mat::zeros(image.rows,image.cols, CV_8UC3);
    H = (h1|h2)&s1;
    imshow("",H);
    src1 = H;
    waitKey();

    /// Create Windows
    imshow( "test", src1 );
}
int main(int argc, const char** argv){
const string keys =
        "{help h usage ? |      | print this message   }"
        "{image i | |path to image}"
        ;
    CommandLineParser parser(argc, argv, keys);
    parser.about("Application name v1.0.0");
    string img_loc(parser.get<string>("i"));
    cerr << img_loc << endl;
    vector<Mat> imageBGR;

    Mat image = imread(img_loc, CV_LOAD_IMAGE_COLOR);
    Mat rood_th = Mat::zeros(image.rows,image.cols, CV_8UC1);


    if(!image.data)                          // Check for invalid input
    {
        cout <<  "Could not open or find on of the images" << std::endl;
    }
    if(!image.empty()){
        imshow("",image);
        waitKey();
    }

    split(image,imageBGR);

    threshold(imageBGR[2], rood_th, 150, 255, THRESH_BINARY);
    Mat tmp1 = imageBGR[0]&rood_th;
    Mat tmp2 = imageBGR[1]&rood_th;
    Mat tmp3 = imageBGR[2]&rood_th;
    Mat imagef(image.rows,image.cols,CV_8UC3);
    Mat in[] = {tmp1,tmp2,tmp3};
    int from_to[] = {0,0,1,1,2,2};
    mixChannels(in,3,&imagef,1,from_to,3);
    imshow("",imagef);
    waitKey();

    Mat imagev2;
    vector<Mat> imageHSV;
    cvtColor(image, imagev2, CV_BGR2HSV);
    imshow("",imagev2);
    waitKey();
    split(imagev2,imageHSV);
    Mat h1, h2, s1;
    inRange(imageHSV[0],hueo_slider,180,h1);
    inRange(imageHSV[0],0,hueb_slider,h2);
    inRange(imageHSV[1],sat_slider,255,s1);
    Mat H = Mat::zeros(image.rows,image.cols, CV_8UC3);
    H = (h1|h2)&s1;
    imshow("",H);
    src1 = H;
    waitKey();

    /// Create Windows
    imshow( "test", src1 );
    /// Create Trackbars
    //sprintf( "h1", "Hue boven %d", hue_slider_max );
    //sprintf( "h2", "Hue onder %d", hue_slider_max );
    //sprintf( "h3", "Saturation onder %d", hue_slider_max );
    createTrackbar( "Hue onder", "test", &hueb_slider, hue_slider_max, on_tb);
    createTrackbar( "Hue boven", "test", &hueo_slider, hue_slider_max, on_tb);
    createTrackbar( "Saturation onder", "test", &sat_slider, sat_slider_max, on_tb);

    /// Show some stuff

    /// Wait until user press some key
    waitKey(0);
    return 0;
}

