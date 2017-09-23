#include "ImageSC.hpp"
#include "opencv2/opencv.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

void SplitRGBTo3Imgs(const int width, const int height, const int step_rgb,
                     const uchar *data_rgb, const int step_r, uchar *data_r,
                     const int step_g, uchar *data_g, const int step_b,
                     uchar *data_b) {
  for (int y = 0; y < height; y++) {
    const uint8_t *rowPtr = data_rgb + step_rgb * y;
    for (int x = 0; x < width; x++) {
      *data_b = rowPtr[x * 3 + 0];
      *data_g = rowPtr[x * 3 + 1];
      *data_r = rowPtr[x * 3 + 2];
      data_b++;
      data_g++;
      data_r++;
    }
  }
}

inline void ShowImageSC(char *wnd_name, int img_w, int img_h, int img_step,
                        uchar *img_data) {
  cv::Mat img(img_h, img_w, CV_8UC1, img_data, img_step);
  cv::imshow(wnd_name, img);
}

inline void ShowImageSC(char *wnd_name, ImageSC &img_sc) {
  ShowImageSC(wnd_name, img_sc.GetWidth(), img_sc.GetHeight(),
              img_sc.GetWidthStep(), img_sc.GetData());
}

void dumpData(ofstream &strm, uchar *data, size_t sz, string seperator,
              uint32_t newlinecnt) {
  for (size_t i = 0; i < sz; i++) {
    strm << ((int)data[i] - 128);
    if ((i + 1) % newlinecnt == 0) {
      strm << endl;
    } else {
      strm << seperator;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Error" << endl;
    return -1;
  }
  Mat image_bgr, image_rgb;
  string fn(argv[1]);
  image_bgr = imread(argv[1], 1);
  if (!image_bgr.data) {
    cout << "No image data " << endl;
    return -1;
  }
  imshow("Display BGR Image", image_bgr);

  cvtColor(image_bgr, image_rgb, cv::COLOR_BGR2RGB);
  // imshow("Display RGB Image", image_rgb);

  int img_bgr_w = image_bgr.cols;
  int img_bgr_h = image_bgr.rows;
  int img_bgr_step = image_bgr.step[0];
  printf("bgr w=%d, h=%d, step=%d\n", img_bgr_w, img_bgr_h, img_bgr_step);
  ImageSC img_bgr_r(img_bgr_w, img_bgr_h);
  ImageSC img_bgr_g(img_bgr_w, img_bgr_h);
  ImageSC img_bgr_b(img_bgr_w, img_bgr_h);

  // convert BGR data to single channel data
  SplitRGBTo3Imgs(img_bgr_w, img_bgr_h, img_bgr_step, image_bgr.data,
                  img_bgr_r.GetWidthStep(), img_bgr_r.GetData(),
                  img_bgr_g.GetWidthStep(), img_bgr_g.GetData(),
                  img_bgr_b.GetWidthStep(), img_bgr_b.GetData());

  ShowImageSC("RGB R Channel", img_bgr_r);
  ShowImageSC("RGB G Channel", img_bgr_g);
  ShowImageSC("RGB B Channel", img_bgr_b);

  ofstream bgrtxt_f(fn + "_bgr.txt");
  string rgbtxtfn(fn + "_rgb.txt");
  ofstream rgbtxt_f(rgbtxtfn);
  ofstream rtxt_f(fn + "_r.txt");
  ofstream gtxt_f(fn + "_g.txt");
  ofstream btxt_f(fn + "_b.txt");

  dumpData(bgrtxt_f, image_bgr.data, img_bgr_h * img_bgr_w * 3, " ", 3);
  dumpData(rgbtxt_f, image_rgb.data, img_bgr_h * img_bgr_w * 3, " ", 3);
  dumpData(rtxt_f, img_bgr_r.GetData(), img_bgr_h * img_bgr_w * 1, " ", 1);
  dumpData(gtxt_f, img_bgr_g.GetData(), img_bgr_h * img_bgr_w * 1, " ", 1);
  dumpData(btxt_f, img_bgr_b.GetData(), img_bgr_h * img_bgr_w * 1, " ", 1);
  cv::waitKey();
  return 0;
}