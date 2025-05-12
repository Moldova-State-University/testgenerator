/**
 * @file testgenerator.cpp
 * @author Mihail Croitor (mcroitor@gmail.com)
 * @brief This is a simple test data generator that reads a configuration file and
 * generates random test data
 * @version 0.1.2
 * @date 2024-04-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

#include "generators.hpp"
#include "formatparser.hpp"

extern std::map<std::string, node_to_type> convertor;

const std::string YAML_SAMPLE =
"filename: \"input%.txt\"\n"
"from: 1\n"
"to: 10\n"
"description: \"Test description\"\n"
"blocks:\n"
"  - nr_of_lines: 1\n"
"    line:\n"
"      - type: integer\n"
"        min: 1\n"
"        max: 10\n"
"        name: N\n"
"      - type: integer\n"
"        min: 1\n"
"        max: 10\n"
"        name: M\n"
"  - nr_of_lines: N\n"
"    line:\n"
"      - type: array\n"
"        size: M\n"
"        element:\n"
"          type: string\n"
"          min_length: 8\n"
"          max_length: 8\n"
"          characters: \"abc\"\n";

bool debug = false;

std::string generate_line(const YAML::Node& line)
{
    if (debug)
    {
        std::cout << "[DEBUG] Generating line" << std::endl;
    }

    std::string result;
    auto it = line.begin();
    while (it != line.end())
    {
        const auto& node = *it;
        auto type = node["type"].as<std::string>();
        if (debug)
        {
            std::cout << "[DEBUG] Generating type: " << type << std::endl;
        }

        if (convertor.find(type) != convertor.end())
        {
            result += convertor.at(type)(node);
        }
        else if (type == "array")
        {
            result += node_to_vector(node);
        }
        else
        {
            std::cerr << "Unknown type: " << type << std::endl;
        }
        ++it;
        if (it != line.end())
        {
            result += " ";
        }
    }
    if (debug)
    {
        std::cout << "[DEBUG] Line generated" << std::endl;
    }
    return result;
}

/**
 * @brief Generate a test based on the given structure
 *
 * @param std::string filename
 * @param YAML::Node blocks
 * @return std::string
 */
void generate_test(const std::string& filename, const YAML::Node& blocks)
{
    std::cerr << "Generating test " << filename << std::endl;
    std::ofstream file(filename);

    if (!file)
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return;
    }

    if (blocks.Type() != YAML::NodeType::Sequence)
    {
        std::cerr << "[ERROR] blocks is not a sequence" << std::endl;
        return;
    }

    if (debug) {
        std::cout << "[DEBUG] blocks has " << blocks.size() << " elements" << std::endl;
    }

    std::string result;
    for (const auto& block : blocks)
    {
        int nr_of_lines = get_parametrized_value(block["nr_of_lines"].as<std::string>());
        if (debug)
        {
            std::cout << "[DEBUG] Generating " << nr_of_lines << " lines" << std::endl;
        }
        const auto& line = block["line"];
        for (int i = 0; i < nr_of_lines; ++i)
        {
            result += generate_line(line);
            result += "\n";
        }
    }

    file << result;

    file.close();
    std::cout << "Test " << filename << " generated." << std::endl;
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

    YAML::Node config = YAML::Load(YAML_SAMPLE);

    file << config;
    file.close();

    std::cout << "Sample test description file generated" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::string filename = std::filesystem::path(argv[0]).filename().string();
        std::cerr << "Usage: " << filename << " <config_file> | --sample" << std::endl;
        std::cerr << "\t--sample \t- generate a sample test description file" << std::endl;
        return 1;
    }

    std::string config{ argv[1] };

    if (config == "--sample")
    {
        sample_test_description_generator();
        return 0;
    }

    std::cout << "[DEBUG] debug mode = " << debug << std::endl;
    std::cout << "[DEBUG] config file: " << config << std::endl;

    if (!std::filesystem::exists(config))
    {
        std::cerr << "Error: file " << config << " does not exist" << std::endl;
        return 1;
    }

    // Load the configuration file
    const std::vector<YAML::Node> config_nodes = YAML::LoadAllFromFile(config);

    for (const auto& config_node : config_nodes)
    {
        std::string filename_template = config_node["filename"].as<std::string>();
        int from = config_node["from"].as<int>();
        int to = config_node["to"].as<int>();

        if (debug) {
            std::cout << "[DEBUG] filename_template: " << filename_template << std::endl;
            std::cout << "[DEBUG] from: " << from << std::endl;
            std::cout << "[DEBUG] to: " << to << std::endl;
        }
        const auto& blocks = config_node["blocks"];

        std::cout << "Generating " << (to - from + 1) << " tests." << std::endl;

        for (int i = from; i <= to; ++i)
        {
            std::string filename = filename_template;
            std::string test_number = std::to_string(i);
            filename.replace(filename.find("%"), 1, test_number);

            generate_test(filename, blocks);
        }
    }

    return 0;
}
