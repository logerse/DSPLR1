#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

#include "configs.hpp"


static double u1(const double t);
static double u2(const double t);
static std::vector<std::pair<double, double>> sampling(const std::function<double(double)> func, const double min,  const double max);

int 
main(const int argc, const char **argv) 
{
  auto samples = sampling(&u1, 1.0, M_PI);
  for (auto &sample : samples)
    std::cout << sample.first << "\t" << sample.second << std::endl;
}



static double
u1(const double t)
{
  return std::sin(2*M_PI*t*3);
}

static double
u2(const double t)
{
  return std::sin(2*M_PI*t*4);
}

#define D_STEP 1
/* Creating samples. Used step value: D_STEP (macros in configs.hpp) */
static std::vector<std::pair<double, double>>
sampling(const std::function<double(double)> func, const double min_,  const double max_)
{
  std::size_t sample_count = static_cast<std::size_t>(
      std::round(((max_-min_)/D_STEP))
      );
  std::vector<std::pair<double, double>> result{sample_count};

  for (double i = min_; i < max_; i += D_STEP) 
  {
    result[i].first = i;
    result[i].second = i;//func(i);
  }

  return result;
}
