#include "generators.hpp"

#include <cmath>
#include <random>

std::mt19937 rng(std::random_device{}());

int generate_integer(int min, int max)
{
  return std::uniform_int_distribution<int>(min, max)(rng);
}

double generate_float(double min, double max, int precision)
{
  return std::round(
             std::uniform_real_distribution<double>(min, max)(rng) * std::pow(10, precision)) /
         std::pow(10,
                  precision);
}

std::string generate_string(int min_length, int max_length, std::string characters)
{
  std::string result;
  int length = generate_integer(min_length, max_length);
  for (int i = 0; i < length; i++)
  {
    result += characters[generate_integer(0, characters.size() - 1)];
  }
  return result;
}

std::vector<int> generate_integer_vector(int size, int min, int max)
{
  std::vector<int> result;
  for (int i = 0; i < size; i++)
  {
    result.push_back(generate_integer(min, max));
  }
  return result;
}

std::vector<double> generate_float_vector(int size, double min, double max, int precision)
{
  std::vector<double> result;
  for (int i = 0; i < size; i++)
  {
    result.push_back(generate_float(min, max, precision));
  }
  return result;
}


std::vector<std::string> generate_string_vector(int size, int min_length, int max_length, std::string characters)
{
  std::vector<std::string> result;
  for (int i = 0; i < size; i++)
  {
    result.push_back(generate_string(min_length, max_length, characters));
  }
  return result;
}
