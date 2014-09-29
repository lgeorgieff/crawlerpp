// ============================================================================
// Author: Lukas Georgieff
// File: utils.cpp
// Description: This implementation file contains al definitions of utility
//              functions reequried for crawler_cpp.
// Public interfaces:
//   * string_to_lower
//   * string_to_upper
//   * merge_arrays
//   * to_string
// ============================================================================

#include "utils.h"

#include <cctype>
#include <algorithm>
#include <sstream>

using std::string;

string crawler_pp::utils::string_to_lower(const string &str){
  string result(str);
  transform(result.begin(), result.end(), result.begin(), tolower);
  return result;
}

string crawler_pp::utils::string_to_upper(const string &str){
  string result(str);
  transform(result.begin(), result.end(), result.begin(), toupper);
  return result;
}
