#include "StickerSheet.h"

StickerSheet::StickerSheet(const Image &picture, unsigned max) {
    picture_ = picture;
    max_ = max;
}

StickerSheet::StickerSheet(const StickerSheet &other) {
    _copy(other);
}

const StickerSheet& StickerSheet::operator=(const StickerSheet &other) {
    _copy(other);
    return *this;
}

void StickerSheet::changeMaxStickers(unsigned max) {
    if (max < max_) {
        if (size_ > max) {
            for (unsigned i = size_-1; i>max-1; i--) {
                x_.pop_back();
                y_.pop_back();
                stickers_.pop_back();
            }
            size_ = max;
        }
    }
    max_ = max;
}

int StickerSheet::addSticker(Image &sticker, int x, int y) {
    stickers_.push_back(&sticker);
    x_.push_back(x);
    y_.push_back(y);
    size_++;
    if (size_>max_) {
        max_++;
    }
    return size_ - 1;
}

int StickerSheet::setStickerAtLayer(Image &sticker, unsigned layer, int x, int y) {
    if (layer>=size_) {
        return -1;
    }
    stickers_[layer] = &sticker;
    x_[layer] = x;
    y_[layer] = y;
    return layer;
}

bool StickerSheet::translate(unsigned index, int x, int y) {
    if (index>=size_) {
        return false;
    }
    x_[index] = x;
    y_[index] = y;
    return true;
}

void StickerSheet::removeSticker(unsigned index) {
  if (index >= size_) {
    return;
  }
  stickers_.erase(stickers_.begin() + index);
  x_.erase(x_.begin() + index);
  y_.erase(y_.begin() + index);
  size_--;
}

Image* StickerSheet::getSticker(unsigned index) {
    if (index >= size_) {
        return NULL;
    }
    return stickers_[index];
}

int StickerSheet::layers() const{
    return max_;
}

Image StickerSheet::render() const {
    Image picture(picture_);
    unsigned xMax = picture.width();
    unsigned yMax = picture.height();
    int xOff = 0;
    int yOff = 0;
    for (unsigned i = 0; i<size_; i++) {
        int x = x_[i];
        int y = y_[i];
        unsigned sticker_width, sticker_height;
        if (x_[i]<0) {
            x = x_[i]*-1 + xMax;
            sticker_width = x;
            if (x_[i]*-1 > xOff) {
                xOff = x_[i]*-1;
            }
        } else {
            sticker_width = stickers_[i]->width() + x;
        }
        if (y_[i]<0) {
            y = y_[i]*-1 + yMax;
            sticker_height = y;
            if (y_[i]*-1 > yOff) {
                yOff = y_[i]*-1;
            }
        } else {
            sticker_height = stickers_[i]->height() + y;
        }
      
        if (sticker_width > xMax) {
            xMax = sticker_width;
        }
        if (sticker_height > yMax) {
            yMax = sticker_height;
        }
    }
    if (xOff!=0 || yOff!=0) {
        picture.resizen(xMax, yMax, xOff, yOff);
    } else {
        picture.resize(xMax, yMax);
    }
    for (unsigned i = 0; i<size_; i++) {
        int x = x_[i];
        int y = y_[i];
        if (x_[i]<0) {
            x = x_[i]*-1;
        }
        if (y_[i]<0) {
            y = y_[i]*-1;
        }
        for (unsigned j = 0; j<stickers_[i]->width(); j++) {
            for (unsigned k = 0; k<stickers_[i]->height(); k++) {
                HSLAPixel& sticker = stickers_[i]->getPixel(j, k);
                if (sticker.a != 0) { 
                    HSLAPixel& image = picture.getPixel(j + x, k + y); 
                    image = sticker;
                }
            }
        }
    }
    return picture;
}

void StickerSheet::_copy(const StickerSheet &obj) {
    std::vector<int> new_x;
    std::vector<int> new_y;
    std::vector<Image*> new_stickers;
    x_.clear();
    y_.clear();
    stickers_.clear();
    picture_ = obj.picture_;
    max_ = obj.max_;
    size_ = obj.size_;
    for (unsigned i = 0; i<size_; i++) {
        new_stickers.push_back(obj.stickers_[i]);
        new_x.push_back(obj.x_[i]);
        new_y.push_back(obj.y_[i]);
    }
    x_ = new_x;
    y_ = new_y;
    stickers_ = new_stickers;
}
