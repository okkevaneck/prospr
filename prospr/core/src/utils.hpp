/* File:            utils.hpp
 * Description:     Header file for utility functions.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#ifndef UTILS_H
#define UTILS_H

#include "protein.hpp"

#include <filesystem>
#include <optional>
#include <stack>

/* Return the path to the cache directory for a given algorithm, if the environment variable PROSPR_CACHE_DIR ist set. */
std::optional<std::filesystem::path> get_cache_dir(const std::string &algorithm, bool create = false);

/* Remove leading and trailing whitespace from a string */
void trim_inplace(std::string &s);

/* Parse a line in INI format (key=value) which may contain comments */
bool parse_ini_line(std::string line, std::string &key, std::string &value);

/* Serialize the state of the protein to key=value format */
void dump_protein_state(const Protein &protein, std::ostream &out);

/* Deserialize the state of the protein from key=value format */
void load_protein_state(Protein &protein, std::istream &in);

#endif
