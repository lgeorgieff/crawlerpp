// ============================================================================
// Author: Lukas Georgieff
// File: uri.h
// Description: This header file defines all the uri class that wraps all
//              operations required for uri handling.
// Public interfaces:
//   *uri
// Modification 08/15/2014: Definition of the class uri.
// ============================================================================


#ifndef URI_HPP
#define URI_HPP

#include "exceptions.h"

#include <string>
#include <iostream>

namespace crawler_cpp {
  namespace data {
    class uri {
    public:
      uri(const std::string&);
      uri(const uri&);
      ~uri();

      std::string get_scheme() const;
      std::string get_host() const;
      std::string get_port() const;
      std::string get_path() const;
      bool has_query() const;
      std::string get_query() const;
      bool has_fragment() const;
      std::string get_fragment() const;
      std::string get_original_uri() const;
      std::string get_normalized_uri() const;
      bool operator==(const uri&) const;
      bool operator==(const std::string&) const;
      bool operator!=(const uri&) const;
      bool operator!=(const std::string&) const;
      uri& operator=(const uri&);
    private:
      std::string normalize_(const std::string&);
      std::string original_uri_;
      std::string normalized_uri_;
    };

    std::ostream& operator<<(std::ostream&, const uri&);
  }
}

#endif // URI_HPP
