#include "Image.h"

Image::Image() {
    width_ = 0;
    height_ = 0;
    imageData_ = NULL;
}

Image::Image(unsigned int width, unsigned int height) {
    width_ = width;
    height_ = height;
    imageData_ = new HSLAPixel[width * height];
}

void Image::lighten() {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            pixel.l += 0.1;
            if (pixel.l>1) {
                pixel.l = 1;
            }
        }
    }
}

void Image::lighten(double amount) {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            pixel.l += amount;
            if (pixel.l>1) {
                pixel.l = 1;
            }
        }
    }
}

void Image::darken() {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            pixel.l -= 0.1;
            if (pixel.l<0) {
                pixel.l = 0;
            }
        }
    }
}

void Image::darken(double amount) {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            pixel.l -= amount;
            if (pixel.l<0) {
                pixel.l = 0;
            }
        }
    }
}

void Image::saturate() {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            pixel.s += 0.1;
            if (pixel.s>1) {
                pixel.s = 1;
            }
        }
    }
}

void Image::saturate(double amount) {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            pixel.s += amount;
            if (pixel.s>1) {
                pixel.s = 1;
            }
        }
    }
}

void Image::desaturate() {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            pixel.s -= 0.1;
            if (pixel.s<0) {
                pixel.s = 0;
            }
        }
    }
}

void Image::desaturate(double amount) {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            pixel.s -= amount;
            if (pixel.s<0) {
                pixel.s = 0;
            }
        }
    }
}

void Image::grayscale() {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            pixel.s = 0;
        }
    }
}

void Image::rotateColor(double degrees) {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            pixel.h+=degrees;
            if (pixel.h>360) {
                pixel.h-=360;
            }
            if (pixel.h<0) {
                pixel.h+=360;
            }
        }
    }  
}

void Image::illinify() {
    for (unsigned int i = 0; i<this->height();i++) {
        for (unsigned int j = 0; j<this->width(); j++) {
            cs225::HSLAPixel &pixel = this->getPixel(j, i);
            if (pixel.h>114 && pixel.h<294) {
                pixel.h = 216;
            } else {
                pixel.h = 11;
            }
        }
    }
}

void Image::scale(double factor) {
    unsigned int width = this->width()*factor;
    unsigned int height = this->height()*factor;

    HSLAPixel* new_image = new HSLAPixel[width*height];

    for (unsigned int i = 0; i<width; i++ ) {
        for (unsigned int j = 0; j<height; j++ ) {
            HSLAPixel &pixel = getPixel(i/factor, j/factor);
            new_image[j*width + i].l = pixel.l;
            new_image[j*width+ i].h = pixel.h;
            new_image[j *width + i].s = pixel.s;
            new_image[j*width + i].a = pixel.a;
        }
    }
    resize(width, height);
    for (unsigned int i = 0; i<width; i++ ) {
        for (unsigned int j = 0; j<height; j++ ) {
            HSLAPixel &pixel = getPixel(i, j);
            pixel.l = new_image[j*width + i].l;
            pixel.h = new_image[j*width + i].h;
            pixel.s = new_image[j*width + i].s;
            pixel.a = new_image[j*width + i].a;
        }
    }
    delete[] new_image;
}

void Image::scale(unsigned w, unsigned h) {
    double factor = std::min((double)w/width(), (double)h/height());
    scale(factor);
}

void Image::resizen(unsigned int newWidth, unsigned int newHeight, unsigned int xOff, unsigned int yOff) {
    HSLAPixel * newImageData = new HSLAPixel[newWidth * newHeight];

    for (unsigned x = 0; x < newWidth; x++) {
      for (unsigned y = 0; y < newHeight; y++) {
        if (x < width_ && y < height_) {
          HSLAPixel & oldPixel = this->getPixel(x, y);
          HSLAPixel & newPixel = newImageData[ (x + xOff + (y * (yOff+newWidth))) ];
          newPixel = oldPixel;
        }
      }
    }

    delete[] imageData_;
    width_ = newWidth;
    height_ = newHeight;
    imageData_ = newImageData;
}

Image::~Image() {
    delete[] imageData_;
}
