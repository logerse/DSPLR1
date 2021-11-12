#pragma once
#include <functional>
#include <vector>
#include "configs.hpp"



class IFunction {
public:
  virtual double operator()(const double x) const = 0;
};



class Sampler {
public:
  Sampler(const double T, const IFunction &func);

  std::vector<std::pair<double, double>> operator()(void) const;

private:
  const std::pair<double, double> range;
  const IFunction &func_;
};
