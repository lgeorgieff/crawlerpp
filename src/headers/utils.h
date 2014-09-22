// ====================================================================================================================
// Author: Lukas Georgieff
// File: utils.h
// Description: This header file contains al declarations of utility functions reequried for crawler_cpp.
// Public interfaces:
//   *hex_to_ushort
//   *string_to_lower
//   *string_to_upper
//   *merge_arrays
// Modification 09/04/2014: Creation of this file
// ====================================================================================================================

#ifndef UTILS_H
#define UTILS_H

#include <cctype>
#include <algorithm>
#include <string>

namespace crawler_cpp {
  namespace utils {
    // Transforms all characters of the passed string to lower case.
    std::string string_to_lower(const std::string&);

    // Transforms all characters of the passed string to upper case.
    std::string string_to_upper(const std::string&);

    // Merges two arrays of the same type and returns a const pointer to a const T.
    template<typename T>
    inline const T *const merge_arrays(const T arr_1[], size_t size_1, const T arr_2[], size_t size_2){
      T *result(new char[size_1 + size_2]);
      for(size_t i(0); i != size_1; ++i) result[i] = arr_1[i];
      for(size_t i(size_1); i != size_1 + size_2; ++i) result[i] = arr_2[i - size_1];
      return result;
    }
  }
}
#endif // end of UTILS_H
