
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int morph_elem = 0;
int morph_size = 3;
const int hue_slider_max = 180;
int hueb_slider = 38;
int hueo_slider = 165;
const int sat_slider_max = 255;
int sat_slider = 43;
Mat h1, h2, s1;
vector<Mat> imageHSV;
vector<Mat> imageBGR;
Mat image;
void on_tb(int, void*){
    inRange(imageHSV[0],hueo_slider,180,h1);
    inRange(imageHSV[0],0,hueb_slider,h2);
    inRange(imageHSV[1],sat_slider,255,s1);
    Mat H = Mat::zeros(image.rows,image.cols, CV_8UC3);
    H = (h1|h2)&s1;
    imshow("Trackbars",H);
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
    Mat image = imread(img_loc, CV_LOAD_IMAGE_COLOR);
    Mat rood_th = Mat::zeros(image.rows,image.cols, CV_8UC1);
    if(!image.data)                          //Check voor goede input
    {
        cout <<  "Could not open or find on of the images" << std::endl;
    }
    if(!image.empty()){
        imshow("",image);
        waitKey();
    }
    ///BGR SEGMENTATIE
    split(image,imageBGR); //splits in rgb
    threshold(imageBGR[2], rood_th, 150, 255, THRESH_BINARY);//tresholden naar rood
    Mat tmp1 = imageBGR[0]&rood_th;
    Mat tmp2 = imageBGR[1]&rood_th;
    Mat tmp3 = imageBGR[2]&rood_th;
    Mat imagef(image.rows,image.cols,CV_8UC3);
    Mat in[] = {tmp1,tmp2,tmp3};
    int from_to[] = {0,0,1,1,2,2};
    mixChannels(in,3,&imagef,1,from_to,3); //terug samenvoegen
    imshow("",imagef);
    waitKey();
    Mat imagev2;
    cvtColor(image, imagev2, CV_BGR2HSV);//conversie naar hsv
    imshow("",imagev2);
    waitKey();
    ///HSV SEGMENTATIE
    split(imagev2,imageHSV);
    inRange(imageHSV[0],hueo_slider,180,h1); //tresholden
    inRange(imageHSV[0],0,hueb_slider,h2);
    inRange(imageHSV[1],sat_slider,255,s1);
    Mat H = Mat::zeros(image.rows,image.cols, CV_8UC3);
    H = (h1|h2)&s1; //volledig beeld filteren
    imshow("",H);
    waitKey();
    ///CONNECTED COMPONENTS
    Mat ccimage;
    Mat stats;
    Mat centroids;
    int numlabels = connectedComponentsWithStats(H, ccimage, stats, centroids);
    cout << "Total Connected Components Detected: " << numlabels << endl;
    vector<Vec3b> colors(numlabels+1);
    colors[0] = Vec3b(0,0,0); //achtergrond zwart
    for( int i = 1; i <= numlabels; i++ ) {
        colors[i] = Vec3b(rand()%256, rand()%256, rand()%256);
        if( stats.at<int>(i, CC_STAT_AREA) < 10000){
            colors[i] = Vec3b(0,0,0); //Andere oppervlakken zwqrt
        }
        cout << stats.at<int>(i, CC_STAT_AREA) <<endl;
    }
    Mat img_color = Mat::zeros(image.size(), CV_8UC3);
    for( int y = 0; y < img_color.rows; y++ )
        for( int x = 0; x < img_color.cols; x++ )
        {
            int label = ccimage.at<int>(y, x);
            CV_Assert(0 <= label && label <= numlabels);
            img_color.at<Vec3b>(y, x) = colors[label];
        }
    imshow("Labeled map", img_color);
    waitKey();
    ///TRACKBARS
    namedWindow("Trackbars", 1);
    createTrackbar( "Hue boven", "Trackbars", &hueb_slider, hue_slider_max, on_tb);
    createTrackbar( "Hue onder", "Trackbars", &hueo_slider, hue_slider_max, on_tb);
    createTrackbar( "Saturation onder", "Trackbars", &sat_slider, sat_slider_max, on_tb);
    on_tb(hueb_slider, 0 );
    on_tb(hueo_slider, 0 );
    on_tb(sat_slider, 0 );
    waitKey();
    return 0;
}

