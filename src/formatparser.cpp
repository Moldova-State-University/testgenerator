#include "generators.hpp"
#include "formatparser.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>

// used for storing variable names and their values
std::map<std::string, int> variables;

class abstract_type {
public:
    virtual ~abstract_type() = default;
    virtual std::string to_string() = 0;
    virtual bool less(const abstract_type& other) const = 0;
    virtual bool greater(const abstract_type& other) const = 0;
};

class integer_type : public abstract_type {
    int value;
public:
    integer_type(int v) : value(v) {}
    std::string to_string() override {
        return std::to_string(value);
    }
    bool less(const abstract_type& other) const override {
        return value < dynamic_cast<const integer_type&>(other).value;
    }
    bool greater(const abstract_type& other) const override {
        return value > dynamic_cast<const integer_type&>(other).value;
    }
};

class float_type : public abstract_type {
    double value;
public:
    float_type(double v) : value(v) {}
    std::string to_string() override {
        return std::to_string(value);
    }
    bool less(const abstract_type& other) const override {
        return value < dynamic_cast<const float_type&>(other).value;
    }
    bool greater(const abstract_type& other) const override {
        return value > dynamic_cast<const float_type&>(other).value;
    }
};

class string_type : public abstract_type {
    std::string value;
public:
    string_type(const std::string& v) : value(v) {}
    std::string to_string() override {
        return value;
    }
    bool less(const abstract_type& other) const override {
        return value < dynamic_cast<const string_type&>(other).value;
    }
    bool greater(const abstract_type& other) const override {
        return value > dynamic_cast<const string_type&>(other).value;
    }
};

std::unique_ptr<abstract_type> make_abstract(const std::string& type, const std::string& value) {
    if (type == "integer") return std::make_unique<integer_type>(std::stoi(value));
    if (type == "float") return std::make_unique<float_type>(std::stod(value));
    return std::make_unique<string_type>(value);
}

void sort_elements(
    std::vector<std::unique_ptr<abstract_type>>& elements,
    const std::string& order)
{
    if (order == "asc") {
        std::sort(elements.begin(), elements.end(),
            [](const auto& a, const auto& b) { return a->less(*b); });
    } else if (order == "desc") {
        std::sort(elements.begin(), elements.end(),
            [](const auto& a, const auto& b) { return a->greater(*b); });
    }
    // unordered — ничего не делаем
}

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

std::string node_to_vector(const YAML::Node& node)
{
    std::string result{ "" };
    integer size = get_parametrized_value(node["size"].as<std::string>());
    const auto& element = node["element"];
    auto element_type = element["type"].as<std::string>();
    std::string order = node["order"] ? node["order"].as<std::string>() : "unordered";

    if (convertor.at(element_type))
    {
        std::vector<std::unique_ptr<abstract_type>> elements;
        for (int i = 0; i < size; ++i)
        {
            std::string val = convertor.at(element_type)(element);
            elements.push_back(make_abstract(element_type, val));
        }
        sort_elements(elements, order);

        // join elements with space
        for (size_t i = 0; i < elements.size(); ++i) {
            if (i > 0) result += " ";
            result += elements[i]->to_string();
        }
    }
    else
    {
        std::cerr << "Unknown type: " << element_type << std::endl;
    }
    return result;
}
