// ============================================================================
// Author: Lukas Georgieff
// File: utils.h
// Description: This header file contains al declarations of utility functions
//              reequried for crawler_cpp.
// Public interfaces:
//   * string_to_lower
//   * string_to_upper
//   * merge_arrays
//   * to_string
// ============================================================================

#ifndef UTILS_H
#define UTILS_H

#include <string>

#include <sstream>


namespace crawler_pp {
  namespace utils {
    // Transforms all characters of the passed string to lower case.
    std::string string_to_lower(const std::string&);

    // Transforms all characters of the passed string to upper case.
    std::string string_to_upper(const std::string&);

    // Merges two arrays of the same type and returns a const pointer
    // to a const T.
    template<typename T>
    inline static const T *const merge_arrays(const T arr_1[], size_t size_1,
			    const T arr_2[], size_t size_2){
      T *result(new char[size_1 + size_2]);
      for(size_t i(0); i != size_1; ++i)
	result[i] = arr_1[i];
      for(size_t i(size_1); i != size_1 + size_2; ++i)
	result[i] = arr_2[i - size_1];
      return result;
    }

    // Defines a template that returns the string representation of the passed
    // T instance. Note this template uses the operator<< which must be defined
    // for the passed instance'S type.
    template<typename T>
    inline static std::string to_string(const T &t){
      std::stringstream ss;
      ss << t;
      return ss.str();
    }
  } // end of namespace utils
} // end of namespace crawler_pp

#endif // end of UTILS_H
