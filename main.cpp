#include <cmath>
#include <functional>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "configs.hpp"
#include "Sampler.hpp"
#include "ScalarProduct.hpp"
#include "Norm.hpp"



static void   print_help_and_exit(const std::string &);
static void   sampler_mode(const int argc, const char **argv);
static void   scal_prod_mode(const int argc, const char **argv);
static void   norm_mode(const int argc, const char **argv);
static void   ort_mode(const int argc, const char **argv);
static void   orthogonal_analitic(const std::string &f1, const std::string &f2);
static bool   is_orthogonal_basis(const double, const double, const double);


class Sin : public IFunction {
public:
  Sin(const double freq) : freq_{freq} {};

  double operator()(const double x) const override { return std::sin(2*M_PI*freq_*x); };

private:
  const double freq_;
};



static void
print_help_and_exit(const std::string &filename)
{
  std::cout << "[*] Usage: " << filename << " mode" << std::endl;
  std::cout << "Avaible modes:" << std::endl;
  std::cout << "\tsampler\t- create a set of function samples;" << std::endl;
  std::cout << "\tscalar_product\t- calculate scalar product of functions;" << std::endl;
  std::cout << "\tnorm\t- calculate the norm of function;" << std::endl;
  std::cout << "\tort\t- check is function are orthogonal." << std::endl;
  exit(-1);
}


static void 
sampler_mode(const int argc, const char **argv)
{
  std::stringstream help_msg;
  help_msg << "[*] Usage: " << std::string(*argv) 
    << " sampler freq_u1 freq_u2 T result_u1.txt result_u2.txt"
    << std::endl;

  if (argc != 7) {
    std::cout << "Incorrect count of arguments for this mode." << std::endl;
    std::cout << help_msg.str() << std::endl;
    return;
  }

  double f1, f2, T;

  try {
    f1 = std::stod(argv[2]);
    f2 = std::stod(argv[3]);
    T = std::stod(argv[4]);
  } catch (std::exception &e) {
    std::cout << "Incorrect value of sampler parametrs." << std::endl;
    std::cout << help_msg.str() << std::endl;
    return;
  }

  std::ofstream res1, res2;
  res1.open(argv[5]);
  res2.open(argv[6]);
  if (res1.bad() || res2.bad())
  {
    std::cout << "Incorrect an output files." << std::endl;
    std::cout << help_msg.str() << std::endl;
    return;
  }

  Sin u1{f1}, u2{f2};
  Sampler sampler_u1{T, u1}, sampler_u2{T, u2};
  
  for (auto &v : sampler_u1())
    res1 << v.first << "\t" << v.second << std::endl;

  for (auto &v : sampler_u2())
    res2 << v.first << "\t" << v.second << std::endl;

  res1 << f1 << ";" << T << std::endl;
  res2 << f2 << ";" << T << std::endl;

  std::cout << "Success! Samples has been out in result files." << std::endl;
  res1.close();
  res2.close();
}

/* func1.txt func2.txt */
static void
scal_prod_mode(const int argc, const char **argv)
{
  std::stringstream help_msg;
  help_msg << "[*] Usage: " << std::string(*argv) 
    << " scalar_product u1.dat u2.dat"
    << std::endl;

  if (argc != 4) {
    std::cout << "Incorrect count of arguments for this mode." << std::endl;
    std::cout << help_msg.str() << std::endl;
    return;
  }

  std::ifstream u1, u2;
  u1.open(argv[2]);
  u2.open(argv[3]);
  if (u1.bad() || u2.bad()
  || !u1.is_open() || !u2.is_open())
  {
    std::cout << "Incorrect path to an data files" << std::endl;
    std::cout << help_msg.str() << std::endl;
    return;
  }

  std::string tmp;
  std::vector<std::string> ss_u1, ss_u2;
  while (std::getline(u1, tmp))
    ss_u1.push_back(tmp);
  u1.close();

  while (std::getline(u2, tmp))
    ss_u2.push_back(tmp);
  u2.close();

  ss_u1.pop_back();
  ss_u2.pop_back();
  ScalarProduct scalarProduct(ss_u1, ss_u2);
  std::cout << "Success!" << std::endl;
  std::cout << "Approximated scalar product: " << scalarProduct.result() << std::endl;
}


static void
norm_mode(const int argc, const char **argv)
{
  std::stringstream help_msg;
  help_msg << "[*] Usage: " << std::string(*argv) 
    << " norm f.dat"
    << std::endl;

  if (argc != 3) {
    std::cout << "Incorrect count of arguments for this mode." << std::endl;
    std::cout << help_msg.str() << std::endl;
    return;
  }

  std::ifstream u1;
  u1.open(argv[2]);
  if (u1.bad() || !u1.is_open())
  {
    std::cout << "Incorrect path to an data file" << std::endl;
    std::cout << help_msg.str() << std::endl;
    return;
  }

  std::string tmp;
  std::vector<std::string> ss_u1;
  while (std::getline(u1, tmp))
    ss_u1.push_back(tmp);
  u1.close();

  ss_u1.pop_back();
  Norm norm(ss_u1);
  std::cout << "Success!" << std::endl;
  std::cout << "Norm of function: " << norm.result() << std::endl;
}


static void
ort_mode(const int argc, const char **argv)
{
  std::stringstream help_msg;
  help_msg << "[*] Usage: " << std::string(*argv) 
    << " ort u1.dat u2.dat [full]"
    << std::endl;

  if (!(argc == 4 || argc == 5)) {
    std::cout << "Incorrect count of arguments for this mode." << std::endl;
    std::cout << help_msg.str() << std::endl;
    return;
  }

  std::ifstream u1, u2;
  u1.open(argv[2]);
  u2.open(argv[3]);
  if (u1.bad() || u2.bad()
  || !u1.is_open() || !u2.is_open())
  {
    std::cout << "Incorrect path to an data files" << std::endl;
    std::cout << help_msg.str() << std::endl;
    return;
  }

  std::string tmp;
  std::vector<std::string> ss_u1, ss_u2;
  while (std::getline(u1, tmp))
    ss_u1.push_back(tmp);
  u1.close();

  while (std::getline(u2, tmp))
    ss_u2.push_back(tmp);
  u2.close();
  
  std::string parametrs_f1 {ss_u1.back()};
  std::string parametrs_f2 {ss_u2.back()};
  ss_u1.pop_back(); ss_u2.pop_back();
  ScalarProduct scalarProduct(ss_u1, ss_u2);
  std::cout << "This function are "  
    << ((scalarProduct.result() == 0) ? "" : "not ")
    << "orthogonal." << std::endl;

  /*
  if (argc == 4 || std::string(argv[4]) != "full")
    return;
*/
  orthogonal_analitic(parametrs_f1, parametrs_f2);
}


static void
orthogonal_analitic(const std::string &f1, const std::string &f2)
{
  double freq1, freq2, T;
  std::string freq1_s, freq2_s, T_s;
  auto del_f1 = f1.find(';');
  auto del_f2 = f2.find(';');
  
  if (del_f1 == std::string::npos || del_f2 == std::string::npos)
  {
    std::cout << "Corrupted input files." << std::endl;
    return;
  }

  freq1_s = f1.substr(0, del_f1);
  freq2_s = f2.substr(0, del_f2);
  T_s = f1.substr(del_f1+1);

  try {
    freq1 = std::stod(freq1_s);
    freq2 = std::stod(freq2_s);
    T = std::stod(T_s);
  } catch (...) {
    std::cout << "Corrupted input files." << std::endl;
    return;
  }

  std::cout << "Work with a normalized functions u1 and u2:" << std::endl;

  {
    std::cout << "[Tnew = T/2;\tF1new = F1;\tF2new = F2]\tResult:\t";

    std::cout 
      << ((is_orthogonal_basis(freq1, freq2, T/2)) ? "YES" : "NO")
      << std::endl;
  }

  {
    std::cout << "[Tnew = T;\tF1new = 2*F1;\tF2new = 2*F2]\tResult:\t";

    std::cout 
      << ((is_orthogonal_basis(2*freq1, 2*freq2, T)) ? "YES" : "NO")
      << std::endl;
  }

  {
    std::cout << "[Tnew = 2*T;\tF1new = F1;\tF2new = F2]\tResult:\t";

    std::cout 
      << ((is_orthogonal_basis(freq1, freq2, 2*T)) ? "YES" : "NO")
      << std::endl;
  }
}


static bool
is_orthogonal_basis(const double freq_u1, const double freq_u2, const double T)
{
  std::stringstream ss;
  Sin u1{freq_u1}, u2{freq_u2};
  std::vector<std::string> samples_f1, samples_f2;

  auto S_u1  = Sampler(T, u1)();
  auto S_u2  = Sampler(T, u2)();

  for (auto &v : S_u1) {
    ss << v.first << "\t" << v.second;
    samples_f1.push_back(ss.str());
    ss.str("");
  }

  for (auto &v : S_u2) {
    ss << v.first << "\t" << v.second;
    samples_f2.push_back(ss.str());
    ss.str("");
  }

  const double norm_u1 = Norm(samples_f1).result();
  const double norm_u2 = Norm(samples_f2).result();
  
  samples_f1.clear();
  samples_f2.clear();

  for (auto &v : S_u1) {
    ss << v.first << "\t" << (v.second / norm_u1);
    samples_f1.push_back(ss.str());
    ss.str("");
  }

  for (auto &v : S_u2) {
    ss << v.first << "\t" << (v.second / norm_u2);
    samples_f2.push_back(ss.str());
    ss.str("");
  }

  ScalarProduct scalarProduct(samples_f1, samples_f2);

  return (Norm(samples_f1).result() - 1 == 0) 
    && (Norm(samples_f2).result() - 1 == 0)
    && (scalarProduct.result() == 0);
}



int 
main(const int argc, const char **argv) 
{
  if (argc < 2) 
    print_help_and_exit(*argv);

  const std::string mode{argv[1]};
  if (mode == "sampler")
  {
    sampler_mode(argc, argv);
  } else if (mode == "scalar_product") {
    scal_prod_mode(argc, argv);
  } else if (mode == "norm") {
    norm_mode(argc, argv);
  } else if (mode == "ort") {
    ort_mode(argc, argv);
  } else {
    print_help_and_exit(*argv);
  }
}
