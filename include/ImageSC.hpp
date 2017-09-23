#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "base.hpp"

//
//  Gray Image Container
//
class ImageSC {
protected:
  int width_, height_, step_;   //width, height and width-step
  uchar *data_;                 //pointer to image data
  size_t data_size_;            //actual data size
  bool shared_;                 //whether data is shared

public:
  ImageSC(const int width=0, const int height=0, const int step=0, bool shared=false, uchar *data=NULL);
  ~ImageSC();
  bool SetSharedData(const int width, const int height, const int step, uchar *data);
  bool Reshape(const int width, const int height, const int step=-1);
  void CopyFrom(const int step, uchar *data, int width=-1, int height=-1);
  inline const int GetWidth() const { return width_; }
  inline const int GetHeight() const { return height_; }
  inline const int GetWidthStep() const { return step_; }
  inline uchar* GetData() const { return data_; }
  inline const uchar* GetConstData() const { return data_; }
  inline bool IsShared() const { return shared_; }
};


