#pragma once
#include "AModule.hpp"



class ScalarProduct : public AModule {
public:
  ScalarProduct(const std::vector<std::string> &f1, const std::vector<std::string> &f2)
  {
    auto u1 {std::move(parse(f1))};
    auto u2 {std::move(parse(f2))};

    if (u1.empty())
      return;

    if (u1.size() != u2.size())
      return;

    for (std::size_t i = 0; i < u1.size(); ++i)
      res_ += u1[i].second * u2[i].second;
    }
};
