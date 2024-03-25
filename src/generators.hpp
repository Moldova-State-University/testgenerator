#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include <string>
#include <vector>

using integer = size_t;
using string = std::string;

const string numbers = "0123456789";
const string lowercase = "abcdefghijklmnopqrstuvwxyz";
const string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string characters = numbers + lowercase + uppercase;

/**
 * @brief Generate a random integer between min and max (inclusive)
 *
 * @param min
 * @param max
 * @return integer
 */
integer generate_integer(integer min, integer max);

/**
 * @brief Generate a random float number between min and max (inclusive) with a given precision
 *
 * @param min
 * @param max
 * @param precision
 * @return double
 */
double generate_float(double min, double max, int precision = 2);

/**
 * @brief Generate a random string of length between min_length and max_length (inclusive) 
 * using the given characters
 *
 * @param min_length
 * @param max_length
 * @param characters
 * @return string
 */
string generate_string(int min_length, int max_length, string chars = characters);

/**
 * @brief Generate a random integer vector of size between min and max (inclusive)
 *
 * @param size
 * @param min
 * @param max
 * @return std::vector<integer>
 */
std::vector<integer> generate_integer_vector(integer size, integer min, integer max);

/**
 * @brief Generate a random float number vector of size between min and max (inclusive) 
 * with a given precision
 *
 * @param size
 * @param min
 * @param max
 * @param precision
 * @return std::vector<double>
 */
std::vector<double> generate_float_vector(integer size, double min, double max, int precision = 2);

/**
 * @brief Generate a random string vector of size between min and max (inclusive) of 
 * length between min_length and max_length (inclusive) using the given characters
 *
 * @param size
 * @param min_length
 * @param max_length
 * @param characters
 * @return std::vector<std::string>
 */
std::vector<string> generate_string_vector(integer size, int min_length, int max_length, string chars = characters);

#endif // GENERATORS_HPP
