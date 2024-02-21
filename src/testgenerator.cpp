/**
 * @file testgenerator.cpp
 * @author Mihail Croitor (you@domain.com)
 * @brief This is a simple test data generator that reads a configuration file and
 * generates random test data
 * @version 0.1
 * @date 2024-02-20
 *
 * @copyright Copyright (c) 2024
 *
 */

/*
 * The configuration file is a YAML file with the following structure:
 * filename: "test%.txt"
 * nr_tests: 10
 * description: "Test description"
 * lines:
 *   - line:
 *       type: integer
 *       min: 1
 *       max: 100
 *       name: N
 *   - line:
 *       type: array
 *       size: N
 *       element:
 *         type: integer
 *         min: -100
 *         max: 100
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <yaml-cpp/yaml.h>

// Random number generator
std::mt19937 rng(std::random_device{}());

const std::string numbers = "0123456789";
const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string characters = numbers + lowercase + uppercase;

// used for storing variable names and their values
std::map<std::string, int> variables;

/**
 * @brief Generate a random integer between min and max (inclusive)
 *
 * @param min
 * @param max
 * @return int
 */
int generate_integer(int min, int max)
{
  return std::uniform_int_distribution<int>(min, max)(rng);
}

/**
 * @brief Generate a random real number between min and max (inclusive) with a given precision
 *
 * @param min
 * @param max
 * @param precision default is 2
 * @return double
 */
double generate_real(double min, double max, int precision = 2)
{
  return std::round(
             std::uniform_real_distribution<double>(min, max)(rng) * std::pow(10, precision)) /
         std::pow(10,
                  precision);
}

/**
 * @brief Generate a random string of length between min_length and max_length (inclusive)
 * using the given characters
 *
 * @param min_length
 * @param max_length
 * @param characters
 * @return std::string
 */
std::string generate_string(int min_length, int max_length, std::string characters = characters)
{
  std::string result;
  int length = generate_integer(min_length, max_length);
  for (int i = 0; i < length; i++)
  {
    result += characters[generate_integer(0, characters.size() - 1)];
  }
  return result;
}

/**
 * @brief Generate a random integer vector of size between min and max (inclusive)
 *
 * @param size
 * @param min
 * @param max
 * @return std::vector<int>
 */
std::vector<int> generate_integer_vector(int size, int min, int max)
{
  std::vector<int> result;
  for (int i = 0; i < size; i++)
  {
    result.push_back(generate_integer(min, max));
  }
  return result;
}

/**
 * @brief Generate a random real number vector of size between min and max (inclusive)
 * with a given precision
 *
 * @param size
 * @param min
 * @param max
 * @param precision
 * @return std::vector<double>
 */
std::vector<double> generate_real_vector(int size, double min, double max, int precision = 2)
{
  std::vector<double> result;
  for (int i = 0; i < size; i++)
  {
    result.push_back(generate_real(min, max, precision));
  }
  return result;
}

/**
 * @brief Generate a random string vector of size between min and max (inclusive)
 * of length between min_length and max_length (inclusive) using the given characters
 *
 * @param size
 * @param min_length
 * @param max_length
 * @param characters
 * @return std::vector<std::string>
 */
std::vector<std::string> generate_string_vector(int size, int min_length, int max_length, std::string characters = characters)
{
  std::vector<std::string> result;
  for (int i = 0; i < size; i++)
  {
    result.push_back(generate_string(min_length, max_length, characters));
  }
  return result;
}

std::string node_to_integer(const YAML::Node &node)
{
  int min = node["min"].as<int>();
  int max = node["max"].as<int>();
  int number = generate_integer(min, max);

  // if node has name field, store this value in the variables map
  if (node["name"])
  {
    std::string name = node["name"].as<std::string>();
    variables[name] = number;
  }
  return std::to_string(number);
}

std::string node_to_real(const YAML::Node &node)
{
  double min = node["min"].as<double>();
  double max = node["max"].as<double>();
  int precision = node["precision"].as<int>();
  double number = generate_real(min, max, precision);
  return std::to_string(number);
}

std::string node_to_string(const YAML::Node &node)
{
  int min_length = node["min_length"].as<int>();
  int max_length = node["max_length"].as<int>();
  std::string characters = node["characters"].as<std::string>();
  std::string string = generate_string(min_length, max_length, characters);
  return string;
}

typedef std::string (*node_to_type)(const YAML::Node &node);

/**
 * @brief map of named YAML convertors
 *
 */
std::map<std::string, node_to_type> convertor = {
    {"integer", node_to_integer},
    {"real", node_to_real},
    {"string", node_to_string},
};

std::string node_to_vector(const YAML::Node &node)
{
  std::string result{""};

  auto sizeStr = node["size"].as<std::string>();
  int size = 0;
  // if sizeStr is integer then use it as size
  if (sizeStr.find_first_not_of("0123456789") == std::string::npos)
  {
    size = std::stoi(sizeStr);
  }
  else
  {
    // if sizeStr is a variable name then use it as size
    if (variables.find(sizeStr) != variables.end())
    {
      size = variables[sizeStr];
    }
    else
    {
      std::cerr << "Unknown array size: " << sizeStr << std::endl;
      return result;
    }
  }

  const auto &element = node["element"];

  auto element_type = element["type"].as<std::string>();
  if (convertor.at(element_type))
  {
    for (int i = 0; i < size; i++)
    {
      result += convertor.at(element_type)(element) + " ";
    }
  }
  else
  {
    std::cerr << "Unknown type: " << element_type << std::endl;
  }

  return result;
}

/**
 * @brief Generate a test based on the given structure
 *
 * @param structure
 * @return std::string
 */
std::string generate_test(const YAML::Node &lines)
{
  std::string result;
  // std::map<std::string, int> variables;
  for (const auto &node : lines)
  {
    auto type = node["type"].as<std::string>();

    if (convertor.find(type) != convertor.end())
    {
      result += convertor.at(type)(node) + "\n";
    }
    else if (type == "array")
    {
      result += node_to_vector(node) + "\n";
    }
    else
    {
      std::cerr << "Unknown type: " << type << std::endl;
    }
  }
  return result;
}

/**
 * @brief Generate a sample test description file
 *
 */
void sample_test_description_generator()
{
  std::ofstream file("sample_test_description.yaml");
  if (!file)
  {
    std::cerr << "Error: could not open file sample_test_description.yaml" << std::endl;
    return;
  }

  YAML::Node config;

  config["filename"] = "input%.txt";
  config["from"] = 1;
  config["to"] = 10;
  config["lines"].push_back(YAML::Load("line:\n  type: integer\n  min: 1\n  max: 100\n  name: N"));
  config["lines"].push_back(YAML::Load("line:\n  type: real\n  min: 0.\n  max: 1.\n precision: 2"));
  config["lines"].push_back(YAML::Load("line:\n  type: array\n  size: N\n  element:\n    type: integer\n    min: -100\n    max: 100"));

  file << config;
  file.close();

  std::cout << "Sample test description file generated" << std::endl;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::string filename = std::filesystem::path(argv[0]).filename().string();
    std::cerr << "Usage: " << filename << " <config_file> | --sample" << std::endl;
    std::cerr << "\t--sample \t- generate a sample test description file" << std::endl;
    return 1;
  }

  std::string config{argv[1]};

  if (config == "--sample")
  {
    sample_test_description_generator();
    return 0;
  }

  // Load the configuration file
  const YAML::Node config_node = YAML::LoadFile(config);

  std::string filename_template = config_node["filename"].as<std::string>();
  int from = config_node["from"].as<int>();
  int to = config_node["to"].as<int>();
  const auto &lines = config_node["lines"];

  std::cout << "Generating " << (to - from) << " tests." << std::endl;

  for (int i = from; i <= to; ++i)
  {
    std::string filename = filename_template;
    std::string test_number = std::to_string(i);
    filename.replace(filename.find("%"), 1, test_number);
    std::ofstream file(filename);

    std::cerr << "Generating test " << filename << std::endl;

    if (!file)
    {
      std::cerr << "Error: could not open file " << filename << std::endl;
      return 1;
    }

    file << generate_test(lines);

    file.close();
    std::cout << "Test " << filename << " generated." << std::endl;
  }

  return 0;
}
