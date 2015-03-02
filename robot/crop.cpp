#include "crop.h"

vector<Mat> img_collection;

vector<Mat> convert(QImage image){
//Mat mat=imread("1.jpg");
cout << image.format()<< endl;
cout << QImage::Format_RGB32<< endl;
Mat  mat( image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine() );
//Mat mat(image.rows(),image.cols(),CV_8UC3,image.scanline());
//show_image(mat);
segmentation_colour(mat);
return img_collection;
}

Mat get_image(Mat &image,locations loc){
    cout << image.type()<< endl;
    int treshold=40;
    loc.x_low= loc.x_low>treshold?(loc.x_low-treshold):loc.x_low;
    loc.y_low=loc.y_low>treshold?(loc.y_low-treshold):loc.y_low;
    loc.x_high=loc.x_high<(640-treshold)?(loc.x_high+treshold):loc.x_high;
    loc.y_high=loc.y_high<(480-treshold)?(loc.y_high+treshold):loc.y_high;
    Mat mat= image(Range(loc.y_low,loc.y_high),Range(loc.x_low,loc.x_high));
    return mat;
}




vector<locations> draw_boxes(vector< vector<Point> > contours,Mat &image1){
   vector <locations> located;

    for(int i=0;i<contours.size();i++){
        locations loc;
        vector <Point> points;
        points= contours.at(i);
        vector<Point>::iterator itr = points.begin();

        for(itr=points.begin();itr<points.end();itr++){
            Point temp=*itr;

            if(loc.x_high<temp.x){
                loc.x_high=temp.x;
            }
            if(loc.x_low>temp.x){

                loc.x_low=temp.x;
            }
            if(loc.y_high<temp.y){

                loc.y_high=temp.y;

            }
            if(loc.y_low>temp.y){

                loc.y_low=temp.y;

            }

        }

        Mat mat=get_image(image1,loc);
        img_collection.push_back(mat);
        located.push_back(loc);

       }
    Scalar colour(255,255,255);
    vector<locations>::iterator itr1 = located.begin();
    for(itr1= located.begin();itr1<located.end();itr1++){
        Point pt1,pt2;
        locations loc = *itr1;
        pt1.x=loc.x_low;
        pt1.y=loc.y_low;
        pt2.x=loc.x_high;
        pt2.y=loc.y_high;

        //rectangle(image1,pt1,pt2,colour,1,8,0);

    }

  /*  for(int i=0;i<img_collection.size();i++){
        Mat mat1=img_collection.at(i);
        namedWindow("abc",CV_WINDOW_AUTOSIZE);
        imshow("abc",mat1);
        waitKey(0);
        destroyWindow("abc");
    }

    namedWindow("disp",CV_WINDOW_AUTOSIZE);
    imshow("disp",image1);
    waitKey(0);
    destroyWindow("disp");*/
    return located;
}






void segmentation_colour(Mat &image){
   //Mat image(image1.size(),image1.type());
    Mat image1= image.clone();
    cvtColor(image,image,CV_BGR2GRAY);
    Mat bin(image.size(),image.type());
    threshold(image,bin,150,255,CV_THRESH_BINARY);

    Mat element = getStructuringElement( 0, Size(20,20 ), Point( 2,2 ) );
    morphologyEx(bin,bin,MORPH_CLOSE,element);
    morphologyEx(bin,bin,MORPH_ERODE,element);
    element = getStructuringElement( 2, Size( 10,10 ), Point( 2,2 ) );
    morphologyEx(bin,bin,MORPH_OPEN,element);
    morphologyEx(bin,bin,MORPH_DILATE,element);

    Mat inv = Scalar::all(255) - bin;
    Mat res(image.size(),image.type());
    image.copyTo(res,bin);
    //namedWindow("Output");
    //imshow("Output",bin);

    vector < vector<Point> > contours;
    vector<Vec4i> hyrachey;

    findContours(bin,contours,hyrachey,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
    Mat cot = Mat::zeros(bin.rows,bin.cols,bin.type());
    vector<locations> located;
    located=draw_boxes(contours,image1);

    }









void show_image(Mat img)
{

//Mat img= imread("1.jpg");
namedWindow("window",CV_WINDOW_AUTOSIZE);
imshow("window",img);
waitKey(0);
destroyAllWindows();
}
