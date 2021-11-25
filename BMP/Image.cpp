#include <future>
#include <iostream>
#include "Image.hpp"
#include "io_context_group.hpp"



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
        image_ = std::move(std::make_unique<bitmap_image>(resize(width, this->height(), true)));
}


void
Image::height(const unsigned int height)
{
        image_ = std::move(std::make_unique<bitmap_image>(resize(this->width(), height, false)));
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
            ? static_cast<double>(this->width()) / result.width() 
            : static_cast<double>(this->height()) / result.height();
        const double const_half =  M_PI * delta * 
            ((is_width) ? column : row);

        double supl;
        struct {double r,g,b;} c = {0,0,0};
        unsigned int border = (is_width) ? this->width() : this->height();
        for (unsigned int  i = 0; i < border;  ++i)
        {
            if (is_width)
                image_->get_pixel(i, row, old_color);
            else
                image_->get_pixel(column, i, old_color);
            supl = sinc(const_half - M_PI*i);
            c.r += old_color.red * supl;
            c.g += old_color.green * supl;
            c.b += old_color.blue * supl;
        }
        new_color.red = c.r;
        new_color.green = c.g;
        new_color.blue = c.b;
        return new_color;
}


bitmap_image
Image::resize(const unsigned int w, const unsigned int h, const bool is_width_resize)
const
{
        io_context_group iog{std::thread::hardware_concurrency()};
        rgb_t new_color;
        bitmap_image result(w, h);
        for (unsigned int row = 0; row < result.height(); ++row)
            boost::asio::post(iog.query(), 
                [&, row] () {
                for (unsigned int column = 0; column < result.width(); ++column)
                {
                    new_color = this->new_color(row, column, result, is_width_resize);
                    /*
                    new_color.red = std::abs(new_color.red);
                    new_color.green = std::abs(new_color.green);
                    new_color.blue = std::abs(new_color.blue);
                    */
                    result.set_pixel(column, row, new_color);
                }
            });
        iog.run();
        return result;
}


static double
sinc(const double x)
{
    return (x == 0) ? 1 : (std::sin(x) / x);
}


