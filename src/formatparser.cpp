#include "generators.hpp"
#include "formatparser.hpp"
#include <iostream>

// used for storing variable names and their values
std::map<std::string, int> variables;

/**
 * @brief map of named YAML convertors
 *
 */
std::map<std::string, node_to_type> convertor = {
    {"integer", node_to_integer},
    {"float", node_to_float},
    {"string", node_to_string},
};

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

std::string node_to_float(const YAML::Node &node)
{
  double min = node["min"].as<double>();
  double max = node["max"].as<double>();
  int precision = (node["precision"]) ? node["precision"].as<int>() : 2;
  double number = generate_float(min, max, precision);
  return std::to_string(number);
}

std::string node_to_string(const YAML::Node &node)
{
  int min_length = node["min_length"].as<int>();
  int max_length = node["max_length"].as<int>();
  std::string chars = (node["characters"]) ? node["characters"].as<std::string>() : characters;
  std::string string = generate_string(min_length, max_length, chars);
  return string;
}


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
