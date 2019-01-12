#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cstdio>
#include <ctime>
#include <iostream>
#define LENGTE 4
#define SOORTEN 3
#define STARTAANT 55
#define SCHILDR 22
#define SCHILDG 214
#define SCHILDB 31
using namespace std;
using namespace cv;

///functie-declaraties
void detectscore(Mat framef,Mat frame, std::clock_t start, Mat score, Mat tempsbin[]); // Detecteer huidige score + bereken snelheid
void detectaliens(Mat frame, Mat aliens[]); //tel aantal aliens
void detectshield(Mat frame, Mat schild); //tel schildpixels (groene tinten)
void sorter(int gloc[], int cijfers[]); //Sorteert gevonden cijfers volgens de volgorde op scherm
void drawgraph(int offset, double f, int scalef); //Tekent grafieken + gevonden waardes op totaalv

///globals scoredetectie
Point matchLoc; //match locatie voor score regio

///globals aliendetectie
int totaalteller=55*3; //alle aliens samengeteld per drie frames

///globals schilddetectie
Point matchLoc2; //matchl locatie voor schilden regio
int schildmax; //maximaal aantal pixels groene tinten
int schildteller; //aantal groene tinten pixels samengeteld per 30 frames
float percentage = 100; //percentage schild

///globals verwerking
int fc = 0; //framecounter
Mat totaalv(700, 1680, CV_8UC3, Scalar(0, 0, 0)); //statistieken
Point puntprev[3];//vorig punt (per onderdeel vd statistieken)
int xvar[3]={0,0,0}; //variabele die grafieken niet laat overflowen op volgende grafiek

int main(int argc, const char** argv){
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{gameplay g| |path to gameplay footage}"
                             );
    Mat frame;
    Mat score = imread("scorebegin.png", CV_LOAD_IMAGE_GRAYSCALE);//template voor scoreregio
    Mat schild = imread("schildbegin.png", CV_LOAD_IMAGE_COLOR);//template voor schildregio
    Mat temps[10]; //templates voor cijfers
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
    Mat tempsbin[10]; //binaire conversie van cijfer templates (performance increase)
    for(int j = 0;j<10;j++){
        threshold(temps[j],tempsbin[j],100,255,THRESH_BINARY);
    }
    Mat aliens[6]; //templates voor aliens: 0&1 alien 1; 2&3 alien 2; 4&5 alien 6
    aliens[0] = imread("alien1a.png", CV_LOAD_IMAGE_GRAYSCALE);
    aliens[1] = imread("alien1b.png", CV_LOAD_IMAGE_GRAYSCALE);
    aliens[2] = imread("alien2a.png", CV_LOAD_IMAGE_GRAYSCALE);
    aliens[3] = imread("alien2b.png", CV_LOAD_IMAGE_GRAYSCALE);
    aliens[4] = imread("alien3a.png", CV_LOAD_IMAGE_GRAYSCALE);
    aliens[5] = imread("alien3b.png", CV_LOAD_IMAGE_GRAYSCALE);
    for(int i=0;i<SOORTEN*2;i++){
        resize(aliens[i],aliens[i],{0,0},0.5,0.5,INTER_NEAREST); //Resize voor performance increase
    }
    VideoCapture capture;
    string gp_loc(parser.get<string>("g"));
    cerr << gp_loc << endl;
    capture.open(gp_loc);
    if ( ! capture.isOpened() ){
        cout << "Probleem bij het openen van de video";
        return -1;
    }

    ///Initialiseren van assen en waardes voor grafieken
    line(totaalv,Point(60,700),Point(60,100),Scalar(0,255,0),2);
    putText(totaalv, "5", Point(5,565),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, "10", Point(5,415),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, "15", Point(5,265),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, "20", Point(5,115),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    line(totaalv,Point(620,700),Point(620,100),Scalar(0,255,0),2);
    putText(totaalv, "10", Point(565,615),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, "20", Point(565,515),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, "30", Point(565,415),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, "40", Point(565,315),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, "50", Point(565,215),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    line(totaalv,Point(1178,700),Point(1178,100),Scalar(0,255,0),2);
    putText(totaalv, "20%", Point(1125,595),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, "40%", Point(1125,475),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, "60%", Point(1125,355),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, "80%", Point(1125,235),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );

    ///Begin inlezen video
    std::clock_t start = std::clock();
    while ( capture.read(frame) ){
        if( frame.empty() ){
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }
        Mat framebw;
        cvtColor(frame, framebw, COLOR_RGB2GRAY); // Converteer naar b&w voor performance increase
        detectscore(framebw,frame,start,score,tempsbin);
        if(fc%10==0){
            resize(framebw,framebw,{0,0},0.5,0.5,INTER_NEAREST);//Resize frame voor performance increase
            detectaliens(framebw, aliens);
        }
        detectshield(frame,schild);
        imshow("Space Invaders", frame);
        if( waitKey(10) == 27 ){
            break;
        }
        fc++;
    }
    return 0;
}
void detectscore(Mat framef, Mat frame, std::clock_t start, Mat score, Mat tempsbin[]){
    int counter; //teller voor aantal gevonden cijfer matches
    int gloc[LENGTE] = {}; //locatie van het maximum binnen elke cijfermatch(rect)
    int cijfers[LENGTE] = {}; //gevonden cijfers
    double duration; //tijd voorbij
    double snelheid; //punten/sec
    double gval[LENGTE] = {}; //waarde van elke geaccepteerde cijfermatch
    Mat img;
    Mat onlycheckbin = Mat::zeros(Size(score.cols, score.rows), CV_32FC1);
    Mat result; //resultaat van template matching van de scoreregio
    Mat results[10]; //resultaten van de templatematching van cijfers
    Point pa[LENGTE] = {}; //locatie van elke cijfermatch in heel de frame
    memset(cijfers, 0, sizeof(cijfers));
    memset(gval, 0, sizeof(gval));
    memset(gloc, 0, sizeof(gloc));
    memset(pa, 0, sizeof(pa));
    if(fc == 0){
        img = framef.clone();
        double maxv, minv;
        Point minLoc; Point maxLoc;
        Mat result;
        matchTemplate(framef, score, result, CV_TM_CCOEFF);
        normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat() );
        minMaxLoc(result, &minv, &maxv, &minLoc,&maxLoc,Mat());
        matchLoc = maxLoc;
    }
    else{
        img = framef.clone();
        Mat onlycheck = img(Rect(matchLoc.x,matchLoc.y+score.rows/2,score.cols,score.rows/2));
        threshold(onlycheck,onlycheckbin,100,255,THRESH_BINARY);
        double maxv = 0;
        double minv = 0;
        Point minLoc = {};
        Point maxLoc = {};
        counter = 0;
        rectangle( frame, matchLoc, Point( matchLoc.x + score.cols , matchLoc.y + score.rows ), Scalar(0,0,255) );
        for(int j = 0;j<(int)(sizeof(results)/sizeof(results[0]));j++){
            matchTemplate(onlycheckbin, tempsbin[j], results[j], CV_TM_SQDIFF_NORMED);
            minMaxLoc(results[j], &minv, &maxv, &minLoc,&maxLoc,Mat());
            Mat maskc = Mat::zeros(Size(score.cols, score.rows), CV_32FC1);
            threshold(results[j],maskc, 0.21, 255,THRESH_BINARY_INV);
            maskc.convertTo(maskc,CV_8UC1);
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            contours.clear();
            findContours(maskc, contours,hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
            Mat res = results[j];
            for (int i = 0; i < (int)contours.size(); ++i){
                double val = 0;
                vector<Point> hull;
                convexHull(contours[i], hull);
                Rect rect = boundingRect(hull);
                Point loc;
                minMaxLoc(res(rect), &val, NULL, &loc, NULL ,Mat());
                if (counter >= 4) {
                    for(int r = 0;r<LENGTE;r++){
                        if (val<gval[r] && abs(rect.x-gloc[r])<=15){
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
                rectangle(frame, pa[r], Point(pa[r].x + tempsbin[0].cols, pa[r].y + tempsbin[0].rows), Scalar(0,0,255));
        }
    }
    sorter(gloc, cijfers);
    int i, scoregetal = 0;
    for (i = 0; i < LENGTE; i++)
        scoregetal = 10 * scoregetal + cijfers[i];
    if(fc%5==0){
        totaalv(Rect(0,0,500,50)) = Scalar(0,0,0);
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        snelheid = (double)scoregetal/duration;
        string snelheids(to_string(snelheid));
        putText(totaalv, snelheids + " punten/s", Point(50,40),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
        imshow("statistieken",totaalv);
    }
    if(fc%30==0){
        drawgraph(60,(double)snelheid,30);
    }
}
void detectaliens(Mat frame, Mat aliens[]){
    int tellers[SOORTEN] = {}; //teller per soort alien
    int totaal; //alle aliens samengeteld
    totaal = 0;
    memset(tellers, 0, sizeof(tellers));
    Mat img;
    Mat resultsa[3]; //resultaten van templatematching van aliens
    Point minLoc; Point maxLoc;
    img = frame.clone();
    minLoc = {};
    maxLoc = {};
    for(int j = 0;j<(int)(sizeof(resultsa)/sizeof(resultsa[0]));j++){
            Mat tempresult[2];
            matchTemplate(img, aliens[j*2], tempresult[0], CV_TM_SQDIFF_NORMED);
            Mat maskc1 = Mat::zeros(Size(img.cols, img.rows), CV_32FC1);
            threshold(tempresult[0],maskc1, 0.27, 255,THRESH_BINARY_INV);
            maskc1.convertTo(maskc1,CV_8UC1);
            matchTemplate(img, aliens[j*2+1], tempresult[1], CV_TM_SQDIFF_NORMED);
            Mat maskc2 = Mat::zeros(Size(img.cols, img.rows), CV_32FC1);
            threshold(tempresult[0],maskc2, 0.27, 255,THRESH_BINARY_INV);
            maskc2.convertTo(maskc2,CV_8UC1);
            Mat masktot;
            bitwise_or(maskc1,maskc2,masktot);
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            contours.clear();
            findContours(masktot, contours,hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
            tellers[j] = contours.size();
    }
    for(int i=0;i<SOORTEN;i++){
            totaal += tellers[i];
    }
    totaalv(Rect(620,0,500,100)) = Scalar(0,0,0);
    if(fc%30==0){
        int totaalgem = round(totaalteller/3.0);
        totaalteller = 0;
        drawgraph(620,(double)STARTAANT-totaalgem,10);
    }
    totaalteller += totaal;
    string totaald(to_string(STARTAANT-totaal));
    string totaall(to_string(totaal));
    putText(totaalv, totaald + " gedood", Point(620,40),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    putText(totaalv, totaall + " over", Point(620,90),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
    imshow("statistieken",totaalv);
}
void detectshield(Mat frame, Mat schild){
    Mat img;
    int schildpixels = 0; //aantal pixels groene tinten(schild)
    img = frame.clone();
    if(fc==0){
        double maxv, minv;
        Point minLoc; Point maxLoc;
        Mat result;
        matchTemplate(frame, schild, result, CV_TM_CCOEFF);
        normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
        minMaxLoc(result, &minv, &maxv, &minLoc,&maxLoc,Mat());
        matchLoc2 = maxLoc;
        schildteller = 0;
    }
    rectangle(frame, matchLoc2, Point( matchLoc2.x + schild.cols , matchLoc2.y + schild.rows ), Scalar(0,0,255) );
    Mat onlycheck = img(Rect(matchLoc2.x,matchLoc2.y,schild.cols,schild.rows));
    for(int x = 0; x<schild.cols; x++){
        for(int y = 0; y<schild.rows; y++){
            Vec3b colour = onlycheck.at<Vec3b>(Point(x, y));
            if(colour.val[1]>120){
                schildpixels++;
            }
        }
    }
    schildmax = max(schildmax,schildpixels);
    schildteller += schildpixels;
    if(fc%30==0){
        int schildgem = 0;
        if(fc==0){
            schildgem = schildteller;
        }
        else{
            schildgem = (float)schildteller/30.0;
        }
        totaalv(Rect(1180,0,500,50)) = Scalar(0,0,0);
        percentage=min(float(100.0),(float)schildgem/(float)schildmax*100);
        string perc;
        perc = to_string(percentage);
        putText(totaalv, "schild: " + perc + "%", Point(1190,40),  FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),1,LINE_8,false );
        imshow("statistieken",totaalv);
        schildteller = 0;
        drawgraph(1180,(double)percentage,6);
    }
}
void sorter(int gloc[], int cijfers[]){
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
void drawgraph(int offset, double f, int scalef){
    Point puntnu;//huidig punt (bij elke statistiek geupdate
    if(fc==0){
        puntprev[offset/590] = Point(offset,700);
    }
    if(3*fc/30-xvar[offset/590]*250>=480){
        totaalv(Rect(offset,100,250,600)) = Scalar(0,0,0);
        totaalv(Rect(offset+250,100,250,600)).copyTo(totaalv(Rect(offset,100,250,600)));
        totaalv(Rect(offset+250,100,250,600))=Scalar(0,0,0);
        xvar[offset/590]++;
        puntnu = Point(offset+3*fc/30-xvar[offset/590]*250,700-scalef*(f));
        puntprev[offset/590]=puntnu;
    }
    else{
        puntnu = Point(offset+3*fc/30-xvar[offset/590]*250,700-scalef*(f));
    }
    line(totaalv,puntprev[offset/590],puntnu, Scalar(255,0,0),3);
    puntprev[offset/590] = puntnu;
    imshow("statistieken", totaalv);
}


