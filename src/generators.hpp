#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include <string>
#include <vector>

const std::string numbers = "0123456789";
const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string characters = numbers + lowercase + uppercase;

/**
 * @brief Generate a random integer between min and max (inclusive)
 *
 * @param min
 * @param max
 * @return int
 */
int generate_integer(int min, int max);

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
 * @return std::string
 */
std::string generate_string(int min_length, int max_length, std::string chars = characters);

/**
 * @brief Generate a random integer vector of size between min and max (inclusive)
 *
 * @param size
 * @param min
 * @param max
 * @return std::vector<int>
 */
std::vector<int> generate_integer_vector(int size, int min, int max);

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
std::vector<double> generate_float_vector(int size, double min, double max, int precision = 2);

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
std::vector<std::string> generate_string_vector(int size, int min_length, int max_length, std::string chars = characters);

#endif // GENERATORS_HPP
