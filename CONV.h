#ifndef CONV_H
#define CONV_H

/*
   Functions to convert between OpenCV's cv::Mat and Qt's QImage and QPixmap.

   Andy Maloney
   23 November 2013
   http://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap
 */

#include <QImage>
#include <QPixmap>
#include <QDebug>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"


 inline QImage  cvMatToQImage( const cv::Mat &inMat )
   {
      switch ( inMat.type() )
      {
         // 8-bit, 4 channel
         case CV_8UC4:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

            return image;
         }

         // 8-bit, 3 channel
         case CV_8UC3:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

            return image.rgbSwapped();
         }

         // 8-bit, 1 channel
         case CV_8UC1:
         {
            static QVector<QRgb>  sColorTable;

            // only create our color table once
            if ( sColorTable.isEmpty() )
            {
               for ( int i = 0; i < 256; ++i )
                  sColorTable.push_back( qRgb( i, i, i ) );
            }

            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

            image.setColorTable( sColorTable );

            return image;
         }

         default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
      }

      return QImage();
   }

   inline QPixmap cvMatToQPixmap( const cv::Mat &inMat )
   {
      return QPixmap::fromImage( cvMatToQImage( inMat ) );
   }




   const int middle_line=200;

   struct location{

       int x_low;
       int x_high;
       int y_low;
       int y_high;

   };


   inline float get_distance(std::vector< std::vector<cv::Point> >contours,cv::Mat frame1){

       float dist;
       float temp_dist=0;
       float distance;
       bool detection_check=false;
       location temp_location;
       for(int i=0;i<contours.size();i++){
           location loc;
           loc.x_high=0;
           loc.x_low=640;
           loc.y_high=0;
           loc.y_low=640;
           std::vector <cv::Point> points;
           points= contours.at(i);
           std::vector<cv::Point>::iterator itr = points.begin();

           for(itr=points.begin();itr<points.end();itr++){
               cv::Point temp=*itr;

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

           dist=(loc.x_high+loc.x_low)/2;
           //cout <<dist << endl;
           if(dist<=middle_line){
               detection_check=true;
               if(dist>temp_dist){
                   temp_location=loc;
                   temp_dist=dist;
                   distance=320-dist;

               }

           }


       }

      cv:: Point pt1,pt2;
           pt1.x=temp_location.x_low;
           pt1.y=temp_location.y_low;
           pt2.x=temp_location.x_high;
           pt2.y=temp_location.y_high;
           cv::Scalar colour(255,255,255);
           cv::rectangle(frame1,pt1,pt2,colour,1,8,0);
          // cv::imshow("name",frame1);


           if(detection_check){
       return distance;
           }else{

               return middle_line;

           }





   }







   inline float segment(cv::Mat image){

   float distance;
   //Mat image = imread("24.jpg");
   //Mat image(image1.size(),image1.type());
   cv::Mat image1= image.clone();
   cv::cvtColor(image,image,CV_BGR2GRAY);
   cv::Mat bin(image.size(),image.type());
   cv::threshold(image,bin,150,255,CV_THRESH_BINARY);

   cv::Mat element = cv::getStructuringElement( 0, cv::Size(20,20 ), cv::Point( 2,2 ) );
   cv::morphologyEx(bin,bin,cv::MORPH_CLOSE,element);
   cv::morphologyEx(bin,bin,cv::MORPH_ERODE,element);
   element = cv::getStructuringElement( 2, cv::Size( 10,10 ), cv::Point( 2,2 ) );
   cv::morphologyEx(bin,bin,cv::MORPH_OPEN,element);
   cv::morphologyEx(bin,bin,cv::MORPH_DILATE,element);

   cv::Mat inv = cv::Scalar::all(255) - bin;
   cv::Mat res(image.size(),image.type());
   image.copyTo(res,bin);
   //namedWindow("Output");
   //imshow("Output",bin);

   std::vector < std::vector<cv::Point> > contours;
   std::vector<cv::Vec4i> hyrachey;

   cv::findContours(bin,contours,hyrachey,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cv::Point(0,0));
   cv::Mat cot = cv::Mat::zeros(bin.rows,bin.cols,bin.type());
   cv::vector<location> located;
   distance=get_distance(contours,image);
   /*
   for(int i=0;i<contours.size();i++){
          Scalar color(255,255,255);
          drawContours( cot, contours, i, color, 2, 8, hyrachey, 0, Point() );
   }

          namedWindow("cont");
          imshow("cont",cot);
   */
   cout << "distance = "<< distance <<endl;
   return distance;
   }






#endif // CONV_H
