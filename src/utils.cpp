// ====================================================================================================================
// Author: Lukas Georgieff
// File: utils.cpp
// Description: This implementation file contains al definitions of utility functions reequried for crawler_cpp.
// Public interfaces:
//   *hex_to_ushort
//   *string_to_lower
//   *string_to_upper
//   *merge_arrays
// Modification 09/04/2014: Creation of this file
// ====================================================================================================================

#include "utils.h"

using std::string;

string crawler_cpp::utils::string_to_lower(const string &str){
  string result(str);
  transform(result.begin(), result.end(), result.begin(), tolower);
  return result;
}

string crawler_cpp::utils::string_to_upper(const string &str){
  string result(str);
  transform(result.begin(), result.end(), result.begin(), toupper);
  return result;
}
