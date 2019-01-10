#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#define LENGTE 4
using namespace std;
using namespace cv;
void detectAndDisplay( Mat frame);
void sorter();
CascadeClassifier face_cascade;
int eerst = 0;
Mat score;//, een, twee, drie, vier, vijf, zes, zeven, acht, negen, zero;
Mat temps[10];
Mat tempsbin[10];
Mat result;
Mat results[10];
Point matchLoc;
int fc = 0;
int counter;
int gloc[LENGTE] = {};
double gval[LENGTE] = {};
Point pa[LENGTE] = {};
int cijfers[LENGTE] = {};
int main( int argc, const char** argv )
{
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{gameplay g| |path to gameplay footage}"
                             );
    VideoCapture capture;
    //-- 2. Read the video stream
    string gp_loc(parser.get<string>("g"));
    cerr << gp_loc << endl;
    temps[0] = imread("zero.png", CV_LOAD_IMAGE_GRAYSCALE);
    temps[1] = imread("een.png", CV_LOAD_IMAGE_GRAYSCALE);
    temps[2] = imread("twee.png", CV_LOAD_IMAGE_GRAYSCALE);
    temps[3] = imread("drie.png", CV_LOAD_IMAGE_GRAYSCALE);
    temps[4] = imread("vier.png", CV_LOAD_IMAGE_GRAYSCALE);
    temps[5] = imread("vijf.png", CV_LOAD_IMAGE_GRAYSCALE);
    temps[6] = imread("zes.png", CV_LOAD_IMAGE_GRAYSCALE);
    temps[7] = imread("zeven.png", CV_LOAD_IMAGE_GRAYSCALE);
    temps[8] = imread("acht.png", CV_LOAD_IMAGE_GRAYSCALE);
    temps[9] = imread("negen.png", CV_LOAD_IMAGE_GRAYSCALE);
    score = imread("scorebegin.png", CV_LOAD_IMAGE_GRAYSCALE);
    /*imshow("een",een);
    imshow("twee",twee);
    imshow("drie",drie);
    imshow("vier",vier);
    imshow("vijf",vijf);
    imshow("zes",zes);
    imshow("zeven",zeven);
    imshow("acht",acht);
    imshow("negen",negen);
    imshow("zero",zero);*/
    imshow("scorebegin.png",score);
    capture.open(gp_loc);
    if ( ! capture.isOpened() )
    {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }
    vector<Point> trackers;
    Mat frame;
    for(int j = 0;j<10;j++){
        threshold(temps[j],tempsbin[j],100,255,THRESH_BINARY);
        imshow(string("tempbin"+j),tempsbin[j]);
    }

    while ( capture.read(frame) )
    {

        if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }
        cvtColor(frame, frame, COLOR_RGB2GRAY);
        //resize(frame, frame, Size(SCALE*frame.cols, SCALE*frame.rows));
        detectAndDisplay(frame);
        //imshow("video", frame);
        if( waitKey(10) == 27 )
        {
            break; // escape
        }
    }
    return 0;
}
void detectAndDisplay( Mat frame)
{
    //waitKey();
    memset(cijfers, 0, sizeof(cijfers));
    memset(gval, 0, sizeof(gval));
    memset(gloc, 0, sizeof(gloc));
    memset(pa, 0, sizeof(pa));
    Mat img;
    Mat onlycheckbin = Mat::zeros(Size(score.cols, score.rows), CV_32FC1);;
    if(eerst == 0){
            imwrite("still.png",frame);
        //cout << "eerst";
        img = frame.clone();
        double maxv, minv;
        Point minLoc; Point maxLoc;
        Mat result;
        matchTemplate(frame, score, result, CV_TM_CCOEFF);
        normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
        minMaxLoc(result, &minv, &maxv, &minLoc,&maxLoc,Mat());
        matchLoc = maxLoc;
    }
    else{

        img = frame.clone();
        Mat onlycheck = img(Rect(matchLoc.x,matchLoc.y+score.rows/2,score.cols,score.rows/2));

        threshold(onlycheck,onlycheckbin,100,255,THRESH_BINARY);
        double maxv = 0;
        double minv = 0;
        Point minLoc = {};
        Point maxLoc = {};
        counter = 0;
        rectangle( img, matchLoc, Point( matchLoc.x + score.cols , matchLoc.y + score.rows ), 255 );
        for(int j = 0;j<(int)(sizeof(results)/sizeof(results[0]));j++){
            //cout << "hier geraakt "<< j << endl;
            //imshow("temps", temps[j]);
            matchTemplate(onlycheckbin, tempsbin[j], results[j], CV_TM_SQDIFF_NORMED);
            //normalize(results[j], results[j], 0, 1, NORM_MINMAX, -1, Mat() );
            minMaxLoc(results[j], &minv, &maxv, &minLoc,&maxLoc,Mat());
            Mat maskc = Mat::zeros(Size(score.cols, score.rows), CV_32FC1);
            threshold(results[j],maskc, 0.27, 255,THRESH_BINARY_INV);
            //inRange(result, 0.95*(float)maxv, maxv, mask);
            maskc.convertTo(maskc,CV_8UC1);
            imshow(string("maskc"+to_string(j)),maskc);
            imshow(string("result"+to_string(j)),results[j]);
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            contours.clear();
            Mat drawing = Mat::zeros( maskc.size(), CV_8UC3 );
            findContours(maskc, contours,hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);

            imshow("conts", drawing);
            Mat res = results[j];
            //cout << "size: "<<contours.size() << endl;
            //cout << "i: "<< i<< endl;
            //cout << "contours.size: "<<contours.size()<<endl;
            for (int i = 0; i < (int)contours.size(); ++i)
            {
                drawContours( drawing, contours, i, Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point() );
                double val = 0;
                //cout << "getal: "<<j<<endl;
                vector<Point> hull;
                convexHull(contours[i], hull);
                Rect rect = boundingRect(hull);
                Point loc;
                minMaxLoc(res(rect), &val, NULL, &loc, NULL ,Mat());
                //cout << "j: "<<j<<endl;
                //cout << "counter: "<<counter<<endl;
                //cout << "valuevoor: "<< val<< endl;
                if (counter >= 4) {
                    for(int r = 0;r<LENGTE;r++){
                        if (val<gval[r] && abs(rect.x-gloc[r])<=15){
                            /*cout << "r: "<< r << endl;
                            cout << "k: "<< gval[r]<<endl;
                            cout << "value: "<< val<< endl;
                            cout << "oud getal: "<< cijfers[r]<<endl;
                            cout << "nieuw getal: "<< j<<endl;*/
                            pa[i] = Point(loc.x + rect.x, loc.y+ rect.y+score.rows/2);
                            gloc[r]=rect.x;
                            gval[r]=val;
                            cijfers[r] = j;
                        }
                    }
                }
                else{
                //cout << "loc:" <<loc << endl;
                    pa[i] = Point(loc.x + rect.x+ matchLoc.x, loc.y+ rect.y+matchLoc.y+score.rows/2);
                    gloc[i]=rect.x;
                    gval[i]=val;
                    cijfers[i] = j;
                }
                counter++;
            }
        }
        for(int r=0;r<LENGTE;r++){
                cout<<"xloc: "<< gloc[r] <<endl;
                rectangle(img, pa[r], Point(pa[r].x + temps[0].cols, pa[r].y + temps[0].rows), 255);

                //cout << "getal "<<r<<": "<< cijfers[r]<<endl;
        }
    }
    sorter();
    cout << "getal: ";
        for (int l = 0;l<LENGTE;l++){
            cout << cijfers[l];
        }

    cout<<endl;
    fc++;
    eerst = 1;
    imshow("Voetganger detection", img);

}

void sorter(){
    int temploc;
    int tempc;
    for(int i = 0; i<LENGTE-1;i++){
        for(int j = 1;j+i<LENGTE;j++){

            if(gloc[i]>gloc[i+j]){
                temploc = gloc[i];
                tempc = cijfers[i];
                gloc[i] = gloc[i+j];
                cijfers[i] = cijfers[i+j];
                gloc[i+j] = temploc;
                cijfers[i+j] = tempc;
            }
        }
    }
}

