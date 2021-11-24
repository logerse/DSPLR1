#include <future>
#include <iostream>
#include "Image.hpp"



static double sinc(const double);



bool
Image::load(void)
{
        try {
            bitmap_image image(name_);
            if (!image)
                return false;

            image_ = std::make_unique<bitmap_image>(std::move(image));
        } catch (std::runtime_error &e) {
            std::cerr << "Can't load image; " << e.what() << std::endl;
            return false;
        }

        return true;
}


void
Image::save(const std::string &fn)
const
{ 
        image_->save_image(fn);
}


void
Image::width(const unsigned int width)
{
        rgb_t new_color;
        bitmap_image result(width, this->height());
        for (unsigned int row = 0; row < result.height(); ++row)
        {
            for (unsigned int column = 0; column < result.width(); ++column)
            {
                new_color = this->new_color(row, column, result);
                result.set_pixel(column, row, new_color);
            }
        }
        image_ = std::move(std::make_unique<bitmap_image>(result));
}


void
Image::height(const unsigned int height)
{
        rgb_t new_color;
        bitmap_image result(this->width(), height);
        for (unsigned int row = 0; row < result.height(); ++row)
            for (unsigned int column = 0; column < result.width(); ++column)
            {
                new_color = this->new_color(row, column, result, false);
                result.set_pixel(column, row, new_color);
            }

        image_ = std::move(std::make_unique<bitmap_image>(result));
}




rgb_t
Image::new_color(const unsigned int row,
                 const unsigned int column,
                 const bitmap_image &result,
                 const bool is_width )
const
{
        rgb_t new_color = {0, 0, 0}, old_color;
        const double delta = (is_width) 
            ? this->width() / result.width() 
            : this->height() / result.height();
        const double const_half =  M_PI * delta * 
            ((is_width) ? column : row);

        double supl;
        unsigned int border = (is_width) ? this->width() : this->height();
        for (unsigned int  i = 0; i < this->width();  ++i)
        {
            if (is_width)
                image_->get_pixel(i, row, old_color);
            else
                image_->get_pixel(column, i, old_color);
            supl = sinc(const_half - M_PI*i);
            new_color.red += old_color.red * supl;
            new_color.green += old_color.green * supl;
            new_color.blue += old_color.blue * supl;
        }
        return new_color;
}


static double
sinc(const double x)
{
    return (x == 0) ? 1 : (std::sin(x) / x);
}
