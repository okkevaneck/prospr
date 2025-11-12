/* File:            utils.cpp
 * Description:     Implementation file for utility functions.
 * License:         This file is licensed under the GNU LGPL V3 license by
 *                  Okke van Eck (2020 - 2023). See the LICENSE file for the
 *                  specifics.
 */

#include "utils.hpp"

#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir(p, m) _mkdir(p)
#include <sys/stat.h>
#define stat _stat
#ifndef S_ISDIR
#define S_ISDIR(m) (((m)&_S_IFDIR) != 0)
#endif
#ifndef S_ISREG
#define S_ISREG(m) (((m)&_S_IFREG) != 0)
#endif
#else
#include <sys/stat.h>
#endif

/* Check if a file exists.
 * (Not using std::filesystem due to macOS compatibility issues)
 */
bool file_exists(const std::string &path) {
  struct stat info;
  return (stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode));
}

/* Check if a dir exists.
 * (Not using std::filesystem due to macOS compatibility issues)
 */
static bool dir_exists(const std::string &path) {
  struct stat info;
  return stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode);
}

/* Create directories of a path.
 * (Not using std::filesystem due to macOS compatibility issues)
 */
static bool make_dirs(const std::string &path) {
  /* Skip empty paths. */
  if (path.empty())
    return false;

  /* If already exists, done. */
  if (dir_exists(path))
    return true;

  /* Recursively create parent. */
  auto pos = path.find_last_of(PATH_SEPARATOR);
  if (pos != std::string::npos) {
    std::string parent = path.substr(0, pos);
    if (!parent.empty() && !dir_exists(parent)) {
      if (!make_dirs(parent))
        return false;
    }
  }

  /* Create this directory. */
  if (mkdir(path.c_str(), 0755) != 0 && errno != EEXIST) {
    std::cerr << "Warning: mkdir failed for " << path << ": " << strerror(errno)
              << "\n";
    return false;
  }
  return true;
}

/* Return the path to the cache directory for a given algorithm, if the
 * environment variable PROSPR_CACHE_DIR ist set. (Not using std::filesystem due
 * to macOS compatibility issues)
 */
std::optional<std::string> get_cache_dir(const std::string &algorithm,
                                         bool create) {
  /* Load cache path from environment, if set. */
  const char *cache_dir_env = std::getenv("PROSPR_CACHE_DIR");
  if (!cache_dir_env) {
    return std::nullopt;
  }

  /* Check if cache path variable is empty. */
  std::string cache_dir = std::string(cache_dir_env);
  trim_inplace(cache_dir);
  if (cache_dir.empty()) {
    return std::nullopt;
  }

  /* Try to create the cache. */
  cache_dir += PATH_SEPARATOR + algorithm;
  if (create) {
    if (!dir_exists(cache_dir)) {
      if (!make_dirs(cache_dir)) {
        std::cerr << "Warning: Failed to create prospr cache directory at "
                  << cache_dir << "\n";
        return std::nullopt;
      }
    }
  }

  return cache_dir;
}

/* Remove leading and trailing whitespace from a string. */
void trim_inplace(std::string &s) {
  /* Trim first left side, then right. */
  s.erase(0, s.find_first_not_of(" \t\n\r"));
  s.erase(s.find_last_not_of(" \t\n\r") + 1);
}

/* Parse a line in INI format (key=value) which may contain comments. */
bool parse_ini_line(std::string line, std::string &key, std::string &value) {
  /* Remove comments. */
  auto pos = line.find_first_of(";#");
  if (pos != std::string::npos)
    line.erase(pos);

  /* Skip empty lines. */
  trim_inplace(line);
  if (line.empty())
    return false;

  /* Split into key and value. */
  pos = line.find('=');

  /* Skip invalid lines. */
  if (pos == std::string::npos) {
#ifdef PROSPR_DEBUG_STEPS
    std::cerr << "[Debug parse_ini_line] Could not parse this line as INI: "
              << line << std::endl;
#endif
    return false;
  }

  /* Parse remaining line. */
  key = line.substr(0, pos);
  value = line.substr(pos + 1);
  trim_inplace(key);
  trim_inplace(value);
  return true;
}

/* Serialize the state of the protein to key=value format. */
void dump_protein_state(const Protein &protein, std::ostream &out) {
  out << "current_hash=";
  auto current_hash = protein.hash_fold();
  for (size_t i = 0; i < current_hash.size(); ++i) {
    if (i != 0)
      out << ",";
    out << current_hash[i];
  }
  out << "\n";
  out << "aminos_placed=" << protein.get_aminos_placed() << "\n";
  out << "solutions_checked=" << protein.get_solutions_checked() << "\n";
}

/* Deserialize the state of the protein from key=value format. */
void load_protein_state(Protein &protein, std::istream &in) {
  std::string line;
  while (std::getline(in, line)) {
    std::string key;
    std::string value;

    /* Continue on invalid lines. */
    if (!parse_ini_line(line, key, value))
      continue;

    /* Parse valid lines based on variable key. */
    if (key == "current_hash") {
      protein.reset();
      std::stringstream ss(value);
      std::string token;
      while (std::getline(ss, token, ',')) {
        protein.place_amino(std::stoi(token));
      }
    } else if (key == "aminos_placed")
      protein._set_aminos_placed(std::stoi(value));
    else if (key == "solutions_checked")
      protein._set_solutions_checked(std::stoi(value));
  }
}
