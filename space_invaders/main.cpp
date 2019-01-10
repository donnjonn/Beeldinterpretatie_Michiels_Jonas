#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cstdio>
#include <ctime>
#include <iostream>
#define LENGTE 4
#define SOORTEN 3
#define STARTAANT 55
using namespace std;
using namespace cv;
void detectAndDisplayscore( Mat frame);
void detectAndDisplayaliens(Mat frame);
void sorter();
CascadeClassifier face_cascade;
int eerst = 0;
double duration;
double snelheid;
std::clock_t start;
Mat score;
Mat temps[10];
Mat aliens[6];
Mat tempsbin[10];
Mat result;
Mat results[10];
Mat resultsa[3];
Mat snelheidv(50, 500, CV_8UC3, Scalar(0, 0, 0));
Mat alienv(100, 500, CV_8UC3, Scalar(0, 0, 0));
Point matchLoc;
int fc = 0;
int counter;
int gloc[LENGTE] = {};
double gval[LENGTE] = {};
Point pa[LENGTE] = {};
int cijfers[LENGTE] = {};
int tellers[SOORTEN] = {};
int totaal;
int aloc[SOORTEN] = {};
int aval[SOORTEN] = {};
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
    aliens[0] = imread("alien1a.png", CV_LOAD_IMAGE_GRAYSCALE);
    aliens[1] = imread("alien1b.png", CV_LOAD_IMAGE_GRAYSCALE);
    aliens[2] = imread("alien2a.png", CV_LOAD_IMAGE_GRAYSCALE);
    aliens[3] = imread("alien2b.png", CV_LOAD_IMAGE_GRAYSCALE);
    aliens[4] = imread("alien3a.png", CV_LOAD_IMAGE_GRAYSCALE);
    aliens[5] = imread("alien3b.png", CV_LOAD_IMAGE_GRAYSCALE);
    for(int i=0;i<SOORTEN*2;i++){
        resize(aliens[i],aliens[i],{0,0},0.5,0.5,INTER_NEAREST);
    }
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
    //imshow("scorebegin.png",score);
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
        //imshow(string("tempbin"+j),tempsbin[j]);
    }

    start = std::clock();
    while ( capture.read(frame) )
    {

        if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }
        cvtColor(frame, frame, COLOR_RGB2GRAY);
        //resize(frame, frame, Size(SCALE*frame.cols, SCALE*frame.rows));
        detectAndDisplayscore(frame);
        resize(frame,frame,{0,0},0.5,0.5,INTER_NEAREST);
        detectAndDisplayaliens(frame);
        //imshow("video", frame);
        if( waitKey(10) == 27 )
        {
            break; // escape
        }
    }

    return 0;
}
void detectAndDisplayscore( Mat frame)
{
    //waitKey();
    memset(cijfers, 0, sizeof(cijfers));
    memset(gval, 0, sizeof(gval));
    memset(gloc, 0, sizeof(gloc));
    memset(pa, 0, sizeof(pa));
    Mat img;
    Mat onlycheckbin = Mat::zeros(Size(score.cols, score.rows), CV_32FC1);
    if(eerst == 0){
        //imwrite("still.png",frame);
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
            threshold(results[j],maskc, 0.21, 255,THRESH_BINARY_INV);
            //inRange(result, 0.95*(float)maxv, maxv, mask);
            maskc.convertTo(maskc,CV_8UC1);
            //imshow(string("maskc"+to_string(j)),maskc);
            //imshow(string("result"+to_string(j)),results[j]);
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            contours.clear();
            //Mat drawing = Mat::zeros( maskc.size(), CV_8UC3 );
            findContours(maskc, contours,hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);

            //imshow("conts", drawing);
            Mat res = results[j];
            //cout << "size: "<<contours.size() << endl;
            //cout << "i: "<< i<< endl;
            //cout<<"j: "<<j<<endl;
            //cout << "contours.size: "<<contours.size()<<endl;
            for (int i = 0; i < (int)contours.size(); ++i)
            {
                //drawContours( drawing, contours, i, Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point() );
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
                //cout << "value: "<< val<< endl;
                //cout << "loc:" <<rect.x<< endl;
                if (counter >= 4) {
                    for(int r = 0;r<LENGTE;r++){
                        if (val<gval[r] && abs(rect.x-gloc[r])<=15){
                            //cout << "r: "<< r << endl;
                            //cout << "k: "<< gval[r]<<endl;
                            //cout << "oud getal: "<< cijfers[r]<<endl;
                            //cout << "nieuw getal: "<< j<<endl;
                            pa[r] = Point(loc.x + rect.x, loc.y+ rect.y+score.rows/2);
                            gloc[r]=rect.x;
                            gval[r]=val;
                            cijfers[r] = j;
                        }
                    }
                }
                else{
                    int ok = 0;
                    for(int r = 0;r<LENGTE;r++){
                        if (abs(rect.x-gloc[r])<=15){
                            ok = 1;
                            if(val<gval[r]){
                                pa[r] = Point(loc.x + rect.x, loc.y+ rect.y+score.rows/2);
                                gloc[r]=rect.x;
                                gval[r]=val;
                                cijfers[r] = j;
                                break;
                            }
                            else{
                                break;
                            }
                        }
                        else{
                            continue;
                        }
                    }
                    if(ok == 0){
                        //cout << "loc:" <<loc << endl;
                        pa[counter] = Point(loc.x + rect.x+ matchLoc.x, loc.y+ rect.y+matchLoc.y+score.rows/2);
                        gloc[counter]=rect.x;
                        gval[counter]=val;
                        cijfers[counter] = j;
                    }
                }
                counter++;
            }
        }
        for(int r=0;r<LENGTE;r++){
                //cout<<"xloc: "<< gloc[r] <<endl;
                rectangle(img, pa[r], Point(pa[r].x + temps[0].cols, pa[r].y + temps[0].rows), 255);

                //cout << "getal "<<r<<": "<< cijfers[r]<<endl;
        }
    }
    sorter();
    int i, score = 0;
    for (i = 0; i < LENGTE; i++)
        score = 10 * score + cijfers[i];
    //cout << "score: "<< score << endl;
    if(fc%5==0){
        snelheidv = Scalar(0,0,0);
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        snelheid = (float)score/duration;
        //cout<<"snelheid: "<<snelheid<<endl;
        string snelheids(to_string(snelheid));
        putText(snelheidv, snelheids + "punten/s", Point(10,40),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
        imshow("snelheid",snelheidv);
    }
    fc++;
    eerst = 1;
    imshow("Voetganger detection", img);
}
void detectAndDisplayaliens(Mat frame){
    totaal = 0;
    //cout << "hier"<<endl;
    memset(tellers, 0, sizeof(tellers));
    memset(aval, 0, sizeof(aval));
    memset(aloc, 0, sizeof(aloc));
    memset(pa, 0, sizeof(pa));
    Mat img;
    Point minLoc; Point maxLoc;
    img = frame.clone();

    minLoc = {};
    maxLoc = {};

    for(int j = 0;j<(int)(sizeof(resultsa)/sizeof(resultsa[0]));j++){
            Mat tempresult[2];
            matchTemplate(img, aliens[j*2], tempresult[0], CV_TM_SQDIFF_NORMED);
            //cout << "hier2"<<endl;
            Mat maskc1 = Mat::zeros(Size(img.cols, img.rows), CV_32FC1);
            threshold(tempresult[0],maskc1, 0.27, 255,THRESH_BINARY_INV);
            maskc1.convertTo(maskc1,CV_8UC1);
            matchTemplate(img, aliens[j*2+1], tempresult[1], CV_TM_SQDIFF_NORMED);
            //cout << "hier2"<<endl;
            Mat maskc2 = Mat::zeros(Size(img.cols, img.rows), CV_32FC1);
            threshold(tempresult[0],maskc2, 0.27, 255,THRESH_BINARY_INV);
            maskc2.convertTo(maskc2,CV_8UC1);
            //cout << "hier2"<<endl;
            Mat masktot;
            bitwise_or(maskc1,maskc2,masktot);
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            contours.clear();
            findContours(masktot, contours,hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
            tellers[j] = contours.size();
    }

    //cout << "aliens: ";
    for(int i=0;i<SOORTEN;i++){
            //cout << tellers[i]<<endl;
            totaal += tellers[i];
    }
    //cout<<endl;
    alienv = Scalar(0,0,0);
    string totaald(to_string(STARTAANT-totaal));
    string totaall(to_string(totaal));
    putText(alienv, totaald + " gedood", Point(10,40),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(alienv, totaall + " over", Point(10,80),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    imshow("gedood",alienv);

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


