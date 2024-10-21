/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once

#include "Image.h"
#include <vector>

class StickerSheet {
    public:
        StickerSheet(const Image &picture, unsigned max);
        StickerSheet(const StickerSheet &other);
        const StickerSheet& operator=(const StickerSheet &other);
        void changeMaxStickers(unsigned max);
        int addSticker(Image &sticker, int x, int y);
        int setStickerAtLayer(Image &sticker, unsigned layer, int x, int y);
        bool translate(unsigned index, int x, int y);
        void removeSticker(unsigned index);
        Image* getSticker(unsigned index);
        int layers() const;
        Image render() const;
        void _copy(const StickerSheet&);
    private:
        Image picture_;
        unsigned max_;
        unsigned size_ = 0;
        std::vector<Image*> stickers_;
        std::vector<int> x_;
        std::vector<int> y_;
};
