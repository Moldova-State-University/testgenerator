#include "generators.hpp"
#include "formatparser.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>

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

int get_parametrized_value(const std::string& value)
{
    integer size = 0;
    if (value.find_first_not_of("-0123456789") == std::string::npos)
    {
        size = std::stoull(value);
    }
    else
    {
        // if sizeStr is a variable name then use it as size
        if (variables.find(value) != variables.end())
        {
            size = variables[value];
        }
        else
        {
            std::cerr << "Unknown value: " << value << std::endl;
        }
    }
    return size;
}

std::string node_to_integer(const YAML::Node& node)
{
    integer min = get_parametrized_value(node["min"].as<std::string>());
    integer max = get_parametrized_value(node["max"].as<std::string>());

    if (min > max)
    {
        std::swap(min, max);
    }
    integer number = generate_integer(min, max);

    // if node has name field, store this value in the variables map
    if (node["name"])
    {
        std::string name = node["name"].as<std::string>();
        variables[name] = number;
    }
    return std::to_string(number);
}

std::string node_to_float(const YAML::Node& node)
{
    double min = node["min"].as<double>();
    double max = node["max"].as<double>();
    if (min > max)
    {
        std::swap(min, max);
    }
    int precision = (node["precision"]) ? node["precision"].as<int>() : 2;
    double number = generate_float(min, max, precision);
    return std::to_string(number);
}

std::string node_to_string(const YAML::Node& node)
{
    int min_length = get_parametrized_value(node["min_length"].as<std::string>());
    int max_length = get_parametrized_value(node["max_length"].as<std::string>());
    if (min_length > max_length)
    {
        std::swap(min_length, max_length);
    }
    std::string chars = (node["characters"]) ? node["characters"].as<std::string>() : characters;
    std::string string = generate_string(min_length, max_length, chars);
    return string;
}

void sort(
    std::vector<std::string>& elements,
    const std::string& type,
    const std::string& order) {
    if (order == "unordered")
    {
        return;
    }
    if (type == "integer") {
        if(order == "asc") {
            std::sort(elements.begin(), elements.end(), [](const std::string& a, const std::string& b) {
                return std::stoi(a) < std::stoi(b);
            });
        } else if (order == "desc") {
            std::sort(elements.begin(), elements.end(), [](const std::string& a, const std::string& b) {
                return std::stoi(a) > std::stoi(b);
            });
        }
        return;
    }
    if (type == "float") {
        if(order == "asc") {
            std::sort(elements.begin(), elements.end(), [](const std::string& a, const std::string& b) {
                return std::stod(a) < std::stod(b);
            });
        } else if (order == "desc") {
            std::sort(elements.begin(), elements.end(), [](const std::string& a, const std::string& b) {
                return std::stod(a) > std::stod(b);
            });
        }
        return;
    }
    if(order == "asc") {
        std::sort(elements.begin(), elements.end());
    } else if (order == "desc") {
        std::sort(elements.rbegin(), elements.rend());
    }
}

std::string node_to_vector(const YAML::Node& node)
{
    std::string result{ "" };

    integer size = get_parametrized_value(node["size"].as<std::string>());

    const auto& element = node["element"];

    auto element_type = element["type"].as<std::string>();
    // check if element has order field
    std::string order = "unordered";
    if (node["order"])
    {
        order = node["order"].as<std::string>();
    }
    if (convertor.at(element_type))
    {
        // generate vector of elements
        std::vector<std::string> elements;
        for (int i = 0; i < size; ++i)
        {
            elements.push_back(convertor.at(element_type)(element));
        }
        sort(elements, element_type, order);

        // join elements with space
        result = std::accumulate(
            elements.begin(),
            elements.end(),
            std::string{},
            [](const std::string& a, const std::string& b) {
                return a.empty() ? b : a + " " + b;
            });
    }
    else
    {
        std::cerr << "Unknown type: " << element_type << std::endl;
    }

    return result;
}
