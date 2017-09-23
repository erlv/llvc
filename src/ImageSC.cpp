
#include "ImageSC.hpp"

ImageSC::ImageSC(const int width, const int height, const int step, bool shared,
                 uchar *data) {
  width_ = width;
  height_ = height;
  step_ = step;
  shared_ = shared;
  data_size_ = 0;
  data_ = NULL;

  if (shared) {
    // shared data
    if (data) {
      // set shared data ptr
      SetSharedData(width, height, step, data);
    }
  } else if (width > 0 && height > 0) {
    // need memory allocation
    if (!Reshape(width, height, step)) {
      ERROR_INFO("Failed in Reshape, %d %d %d\n", width, height, step);
    }
    if (data) {
      // need memory copy
      memcpy(data_, data, height * step);
    }
  }
}

ImageSC::~ImageSC() {
  if (data_ && !shared_) {
    FreeAlignedMemory((void *&)data_);
  }
}

bool ImageSC::Reshape(const int width, const int height, const int step) {
  ERROR_IF(shared_, "cannot reshape if shared!");
  width_ = width;
  height_ = height;
  if (step <= 0) {
    // step is not given, round up by aligned step
    step_ = AlignedStepRoundUp(width);
  } else {
    // step is given
    if (step < width) {
      ERROR_INFO("step < width, %d %d\n", step, width);
      return false;
    }
    step_ = step;
  }
  size_t mem_needed = step_ * height;
  if (mem_needed > data_size_) {
    FreeAlignedMemory((void *&)data_);
    if (MallocAlignedMemory((void *&)data_, mem_needed) != 0) {
      ERROR_INFO("Failed in mallocing memory of size %lu", mem_needed);
    }
    data_size_ = mem_needed;
  }
  return true;
}

bool ImageSC::SetSharedData(const int width, const int height, const int step,
                            uchar *data) {
  ERROR_IF(!shared_, "cannot SetSharedData if not shared!");
  ERROR_IF(width <= 0 || height <= 0 || step <= 0 || data == NULL,
           "invalid inputs: %d %d %d %p", width, height, step, data);
  width_ = width;
  height_ = height;
  step_ = step;
  data_ = data;
  return true;
}

void ImageSC::CopyFrom(const int step, uchar *data, int width, int height) {
  if (width < 0) {
    width = width_;
  }
  if (height < 0) {
    height = height_;
  }
  ERROR_IF(width > width_ || height > height_,
           "src cannot be larger than dst, %d %d %d %d", width, width_, height,
           height_);
  for (int y = 0; y < height; y++) {
    uchar *s = data + y * step;
    uchar *d = data_ + y * step_;
    uchar *d_end = d + width;
    while (d < d_end) {
      *d++ = *s++;
    }
  }
}
