#ifndef FORMAT_PARSER_HPP
#define FORMAT_PARSER_HPP

#include <string>
#include <map>
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

int get_parametrized_value(const std::string& value);

std::string node_to_integer(const YAML::Node& node);
std::string node_to_float(const YAML::Node& node);
std::string node_to_string(const YAML::Node& node);
std::string node_to_vector(const YAML::Node& node);

typedef std::string(*node_to_type)(const YAML::Node& node);

#endif // FORMAT_PARSER_HPP