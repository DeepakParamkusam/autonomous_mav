#include <stdio.h>
#include <cv.h>
#include <math.h>
#include <highgui.h>
#include <iostream>
#include <typeinfo>

using namespace cv;
using namespace std;

#define pi 3.14159

long int sum_arr(int size, int arr[]);

int main(){
  Mat image;
  Mat planes[3];
  double pr, pg, pb;

  image = imread("../images/test.jpeg");
  if(! image.data )
  {
    cout <<  "Could not open or find the image" << std::endl ;
    return -1;
  }

  int H = image.rows;
  int W = image.cols;
  int _stride = image.step;

  split(image,planes);

  // Mat isgrass = planes[0];
  Mat isgrass(H, W, CV_8UC1, Scalar(0));

  for(int i = 0; i<H; i++){
    for(int j = 0; j<W; j++){
      Scalar pr = planes[0].at<uchar>(i,j);
      Scalar pg = planes[1].at<uchar>(i,j);
      Scalar pb = planes[2].at<uchar>(i,j);
      if((pg.val[0] > pr.val[0]+16)&&(pg.val[0] > pb.val[0]+16)&&(pg.val[0] > 72)){
        isgrass.at<uchar>(i,j) = 1;
      }
      else{
        isgrass.at<uchar>(i,j) = 0;
      }
    }
  }

  for(int i = H; i >= floor(H/2);i=i-4){
    int temp[int(W/2.0)] ;
    for(int j = int(0.25*W); j<=int(0.75*W); j++){
      temp[j- int(0.25*W)] = 1 - isgrass.at<uchar>(i,j);
      cout <<temp[j- int(0.25*W)] <<endl;
    }
    long int S = sum_arr(int(W/2.0),temp);
    cout <<S/255<<endl;
  //
  //   long int  WSum = 0;
  //   for(int j = int(0.25*W);j<=int(0.75*W);j++){
  //     WSum = WSum + j*(1 - isgrass.at<uchar>(i,j));
  //   }

    // int WSum_temp= WSum/(S)/255;
    // long long int CoW = WSum*255/S;


  // // determine if the lowest contact point is in this row
  // if((S > 0.025*W)&&((CoW-W/2) < 0.2*W)){
  //   int H_bott = i;
  //   isgrass(i-floor(0.003*H):i+floor(0.003*H),:) = zeros(floor(0.003*H)*2+1,W); //convert
  //   I(i-floor(0.003*H):i+floor(0.003*H),:,:) = zeros(floor(0.003*H)*2+1,W,3); //convert
  //   Sign = 0;
  //   // threshold for distace to obstacle
  //   if(int i > 0.9*H){
  //     // calculate the 'center of mass' of not-green pixels in the
  //     //neighboring area above the contact point
  //     WSum = zeros(0.2*W,1); //convert
  //     for(int j = 0.25*W;j<=0.75*W;j++){
  //       WSum = WSum + j*(1-isgrass(i-0.2*W+1:i,j)); //convert
  //     }
  //     S = sum(sum(1-isgrass(i-0.2*W+1:i,0.25*W:0.75*W))); //convert
  //     WSum = sum(WSum)/S; //convert
  //     // turn to the direction with less not-green pixels
  //     Sign = -1;
  //     if(WSum < W/2){
  //       Sign = 1;
  //     }
  //   }
  // }
  // break
  // }
  //
  //
  namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
  imshow( "Display window",isgrass);                 // Show our image inside it.
  waitKey(0);
  // imshow( "Display window", planes[1] );                 // Show our image inside it.
  // waitKey(0);
  // imshow( "Display window", planes[2] );                 // Show our image inside it.
  // waitKey(0);


  return 0;

}
}

long int sum_arr(int size, int arr[]){
  long int wut = 0;
  for(int i = 0; i<size; i++){
    wut = wut + arr[i];
  }
  return wut;
}
