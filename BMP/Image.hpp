#pragma once

#include <string>
#include <memory>
#include "bitmap/bitmap_image.hpp"



class Image {
public:
        explicit Image(const std::string &fn) : name_{std::move(fn)} {};
        bool load(void);
        void save(const std::string &fn) const;
        void width(const unsigned int);
        void height(const unsigned int);

        unsigned int width() const { return image_->width(); }
        unsigned int height() const { return image_->height(); }

private:
        rgb_t new_color(const unsigned int, const unsigned int, const bitmap_image&, bool is_width=true) const;

private:
        const std::string name_;
        std::unique_ptr<bitmap_image> image_;
};
