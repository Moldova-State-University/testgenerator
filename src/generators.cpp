#include "generators.hpp"

#include <cmath>
#include <random>

std::mt19937 rng(std::random_device{}());

integer generate_integer(integer min, integer max)
{
    return std::uniform_int_distribution<integer>(min, max)(rng);
}

double generate_float(double min, double max, int precision)
{
    return std::round(
        std::uniform_real_distribution<double>(min, max)(rng) * std::pow(10, precision)) /
        std::pow(10,
            precision);
}

std::string generate_string(int min_length, int max_length, std::string chars)
{
    std::string result;
    int length = generate_integer(min_length, max_length);
    for (int i = 0; i < length; i++)
    {
        result += chars[generate_integer(0, chars.size() - 1)];
    }
    return result;
}

std::vector<integer> generate_integer_vector(integer size, integer min, integer max)
{
    std::vector<integer> result;
    for (integer i = 0; i < size; i++)
    {
        result.push_back(generate_integer(min, max));
    }
    return result;
}

std::vector<double> generate_float_vector(integer size, double min, double max, int precision)
{
    std::vector<double> result;
    for (integer i = 0; i < size; i++)
    {
        result.push_back(generate_float(min, max, precision));
    }
    return result;
}


std::vector<std::string> generate_string_vector(integer size, int min_length, int max_length, std::string chars)
{
    std::vector<std::string> result;
    for (integer i = 0; i < size; i++)
    {
        result.push_back(generate_string(min_length, max_length, chars));
    }
    return result;
}
