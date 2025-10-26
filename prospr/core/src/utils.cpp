/* File:            utils.cpp
 * Description:     Implementation file for utility functions.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include "utils.hpp"

#include <iostream>

/* Return the path to the cache directory for a given algorithm, if the environment variable PROSPR_CACHE_DIR ist set. */
std::optional<std::filesystem::path> get_cache_dir(const std::string &algorithm, bool create)
{
    const char *cache_dir_env = std::getenv("PROSPR_CACHE_DIR");
    if (!cache_dir_env)
    {
        return std::nullopt; // env var not set
    }
    std::filesystem::path cache_dir(cache_dir_env);
    cache_dir /= algorithm;
    if (create)
    {
        try
        {
            if (!std::filesystem::exists(cache_dir))
            {
                std::filesystem::create_directories(cache_dir);
            }
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Warning: Failed to create prospr cache directory: " << e.what() << "\n";
            return std::nullopt;
        }
    }
    return cache_dir;
}

/* Remove leading and trailing whitespace from a string */
void trim_inplace(std::string &s)
{
    s.erase(0, s.find_first_not_of(" \t\n\r")); // left
    s.erase(s.find_last_not_of(" \t\n\r") + 1); // right
}

/* Parse a line in INI format (key=value) which may contain comments */
bool parse_ini_line(std::string line, std::string &key, std::string &value)
{
    // Remove comments
    auto pos = line.find_first_of(";#");
    if (pos != std::string::npos)
        line.erase(pos);
    // Skip empty lines
    trim_inplace(line);
    if (line.empty())
        return false;
    // Split into key and value
    pos = line.find('=');
    if (pos == std::string::npos)
    {
#ifdef PROSPR_DEBUG_STEPS
        std::cerr << "[Debug parse_ini_line] Could not parse this line as INI: " << line << std::endl;
#endif
        return false; // Skip invalid lines
    }
    key = line.substr(0, pos);
    value = line.substr(pos + 1);
    trim_inplace(key);
    trim_inplace(value);
    return true;
}

/* Serialize the state of the protein to key=value format */
void dump_protein_state(const Protein &protein, std::ostream &out)
{
    out << "current_hash=";
    auto current_hash = protein.hash_fold();
    for (size_t i = 0; i < current_hash.size(); ++i)
    {
        if (i != 0)
            out << ",";
        out << current_hash[i];
    }
    out << "\n";
    out << "aminos_placed=" << protein.get_aminos_placed() << "\n";
    out << "solutions_checked=" << protein.get_solutions_checked() << "\n";
}

/* Deserialize the state of the protein from key=value format */
void load_protein_state(Protein &protein, std::istream &in)
{
    std::string line;
    while (std::getline(in, line))
    {
        std::string key;
        std::string value;
        if (!parse_ini_line(line, key, value))
            continue;
        if (key == "current_hash")
        {
            protein.reset();
            std::stringstream ss(value);
            std::string token;
            while (std::getline(ss, token, ','))
            {
                protein.place_amino(std::stoi(token));
            }
        }
        else if (key == "aminos_placed")
            protein.set_aminos_placed(std::stoi(value));
        else if (key == "solutions_checked")
            protein.set_solutions_checked(std::stoi(value));
    }
}
