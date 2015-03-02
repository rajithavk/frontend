#ifndef CROP
#define CROP
#include <iostream>
#include <QMainWindow>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

struct locations{

    int x_low=640;
    int x_high=0;
    int y_low=640;
    int y_high=0;

};



Mat get_image(Mat &image,locations loc);
vector<locations> draw_boxes(vector< vector<Point> > contours,Mat &image1);
void show_image(Mat img);
vector<Mat> convert(QImage image);
void segmentation_colour(Mat &image);





inline void tellid(){

    cout<< "Learning QT"<< endl;

}


#endif // CROP

