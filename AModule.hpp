#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

using FuncVector=std::vector<std::pair<double, double>>;



class AModule {
public:
  double result(void) const { return res_; };

protected:
  FuncVector parse(const std::vector<std::string> &func) const;

protected:
  double res_ = 0;
};


FuncVector
AModule::parse(const std::vector<std::string> &func)
const
{
  double x, y;
  FuncVector result;
  std::size_t delimiter_index = 0;

  for (auto &line : func) 
  {
    delimiter_index = line.find('\t');
    try {
      x = std::stod(line.substr(0, delimiter_index));
      y = std::stod(line.substr(delimiter_index + 1));
    } catch (std::exception &e) {
      std::cout << "Incorrect format of input data" << std::endl;
      return FuncVector(0);
    }

    result.push_back(std::make_pair(x, y));
  }
  return result;
}
