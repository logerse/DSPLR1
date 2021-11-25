#include <iostream>
#include <memory>
#include "Image.hpp"



static double n = 270.0/540;
static double m = 270.0/540;

static void help(const char *);
static void load_nm(const char*, const char*, bool&);



int
main(const int argc, const char **argv)
{
        bool success = true;
        switch (argc) {
        case 4:
          load_nm(argv[2], argv[3], success);
        case 2:
          break;

        default:
            help(*argv);
            return -1;
        }
      
        if (!success) 
          return -1;

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
        std::cout << "[*] Usage: " << std::string(prog) << " image.bmp [N] [M]" << std::endl;
}

static void
load_nm(const char *n_, const char *m_, bool &success)
{
        try {
            n = std::stod(n_);
            m = std::stod(m_);
            if (n <= 0 || m <= 0)
              throw std::runtime_error("N and M should be positive and not equal zero.");
        } catch (std::exception &e) {
            std::cerr << "Invalid value of N and/or M; " << e.what() << std::endl;
            success = false;
        }
}
