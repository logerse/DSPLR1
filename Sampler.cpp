#include "Sampler.hpp"



Sampler::Sampler(const double T, const IFunction &func)
  : range(std::make_pair((-1)*(T/2), (T/2)))
  , func_(func)
{}


std::vector<std::pair<double, double>> 
Sampler::operator()(void)
const
{
  std::vector<std::pair<double, double>> result;

  for (double x = range.first; x < range.second; x += D_STEP)
    result.push_back(std::make_pair(x, func_(x)));

  return result;
}
