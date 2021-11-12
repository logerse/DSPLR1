#pragma once
#include <cfloat>
#include <cmath>
#include "AModule.hpp"
#include "configs.hpp"



class Norm : public AModule {
public:
  Norm(const std::vector<std::string> &func)
  {
    double min_ = DBL_MAX;
    double max_ = DBL_MIN;
    auto f {std::move(parse(func))};

    if (f.empty())
      return;
    
    for (auto &sample : f)
    {
      min_ = (min_ > sample.first) ? sample.first : min_;
      max_ = (max_ < sample.first) ? sample.first : max_;

      res_ += sample.second * sample.second;
    }

    double T = max_ - min_;
    res_  = (res_/T) * D_STEP;
    res_ = sqrt(res_);
  }
};
