#include <iostream>
#include <memory>
#include "Image.hpp"



static double n = 20.0/40;
static double m = 20.0/40;

static void help(const char *);



int
main(const int argc, const char **argv)
{
        if (argc != 2)
        {
            help(*argv);
            return -1;
        }

        std::unique_ptr<Image> image = std::make_unique<Image>(argv[1]);

        if (image->load())
        {
            image->width(image->width() * n);
            image->save("result_width.bmp");

            image->height(image->height() * m);
            image->save("result.bmp");
        } else {
            std::cerr << "Incorrect path of an image or invalid file format." << std::endl;
            return -2;
        }
        
}



static void
help(const char *prog)
{
        std::cout << "[*] Usage: " << std::string(prog) << " image.bmp" << std::endl;
}
