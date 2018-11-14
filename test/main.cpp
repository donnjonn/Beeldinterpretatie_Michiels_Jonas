#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main(){

	Mat image1,image2;
	image1 = imread("cross.png",CV_LOAD_IMAGE_COLOR);
	if(!image1.data){
		cout << "could not find image1" << endl;
	}
	image2 = imread("cross.png",CV_LOAD_IMAGE_GRAYSCALE);
	if(!image2.data){
		cout << "could not find image2" << endl;
	}
	cout << "opencv test" << endl;
	namedWindow("Color Image", WINDOW_AUTOSIZE);
	imshow("Color Image", image1);
	namedWindow("Gray Scale Image", WINDOW_AUTOSIZE);
    imshow("GRAY Scale Image", image2);
	waitKey(0);
	destroyAllWindows();
	return 0;
}
