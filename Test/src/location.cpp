#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(){
  Mat img;
  Mat planes[3];   //destination array
  // Mat I_b,I_g,I_r;
  img = imread("../images/t4.jpeg");   // Read the image in RGB form
  // split(img, bgr);
  split(img,planes);

  int W = img.cols;
  int H = img.rows;
  //Note: OpenCV uses BGR color order
  // I_b = bgr[0];
  // I_g = bgr[1];
  // I_r = bgr[2];

  int isgrass[H][W];
  int i=0,j=0;

  // for (i = 0; i < H; i++){
  //   for(j = 0; j< W; j++){
  //     if(I_g.at<double>(i,j) > (I_r.at<double>(i,j) + 8) && I_g.at<double>(i,j) > (I_b.at<double>(i,j) + 8) && I_g.at<double>(i,j) > 60){
  //       cout << I_g.at<double>(i,j) <<endl;
  //       isgrass[i][j] = 1;
  //     }
  //
  //     else{
  //       isgrass[i][j] = 0;
  //     }
  //
  //   }
  // }

  for(int i = 0; i<H; i++){
    for(int j = 0; j<W; j++){
      Scalar pr = planes[0].at<uchar>(i,j);
      Scalar pg = planes[1].at<uchar>(i,j);
      Scalar pb = planes[2].at<uchar>(i,j);
      if((pg.val[0] > pr.val[0]+8)&&(pg.val[0] > pb.val[0]+8)&&(pg.val[0] > 60)){
        isgrass[i][j] = 1;
      }
      else{
        isgrass[i][j] = 0;
      }
    }
  }

  int m,n,CoW,H_bott, Sign;

  int S = 1;
  int WSum = 0;

  for(i = H-1; i > H/2; i-= 4)
  {
    for(m = W/4; m < W*3/4; m++){
      S += 1 - isgrass[i][m];
      WSum += m*(1 - isgrass[i][m]);
    }
    // cout<<S<<endl;
    CoW = WSum/S;

    if( (S > W/40) && (CoW - W/2) < W/5){
      H_bott = i;
      Sign = 0;
      if(i > 9*H/10){
        WSum = 0;
        S = 1;
        for(m = W/4; m < W*3/4; m++){
          for(n = i - W/5 + 1; n < i; n++){
            WSum += m*(1 - isgrass[n][m]);
            S += 1 - isgrass[n][m];
          }
          WSum = WSum/S;

          Sign = -1;

          if(WSum < W/2){
            Sign = 1;
          }
        }
      }
      // cout <<S<<" "<<WSum<<endl;
      break;
    }
  }
  cout <<Sign<<" "<<H_bott<<endl;
}
