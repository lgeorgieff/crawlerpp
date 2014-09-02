// ====================================================================================================================
// Author: Lukas Georgieff
// File: uri.h
// Description: This header file defines the uri class that wraps all
//              operations required for uri handling.
// Public interfaces:
//   *uri
// Modification 08/15/2014: Definition of the class uri.
// ====================================================================================================================


#ifndef URI_HPP
#define URI_HPP

#include "exceptions.h"

#include <string>
#include <iostream>

namespace crawler_cpp {
  namespace data {

    // This class represents uris and all processes that are offered for uris.
    class uri {
    public:
      const std::string SCHEME_HTTP = "http";
      const std::string SCHEME_HTTPS = "https";
      const std::string SCHEME_MAILTO = "mailto";
      // This constructor requires a string that represents the original uri
      // which is normalized and finally wrapped by this class
      uri(const std::string&);
      // A copy constructor for this class
      uri(const uri&);
      // The destructor which actually has nothing to do!
      ~uri();

      // Returns true if the scheme of the uri represented by this class is
      // "http"
      bool is_http() const;
      // Returns true if the scheme of the uri represented by this class is
      // "https"
      bool is_https() const;
      // Returns true if the scheme of the uri represented by this class is
      // "mailto"
      bool is_mailto() const;
      // A getter for the scheme part of the normalized uri
      std::string get_scheme() const;
      // A getter for the host part of the normalized uri
      std::string get_host() const;
      // A getter for the port part of the normalized uri
      std::string get_port() const;
      // A getter for the path part of the normalized uri
      std::string get_path() const;
      // Returns true if the normalized uri has a query part
      bool has_query() const;
      // A getter for the query part of the normalized uri
      std::string get_query() const;
      // Returns true if the normalized uri has a fragment part
      bool has_fragment() const;
      // A getter for the fragment part of the normalized uri
      std::string get_fragment() const;
      // A getter for the originaly passed uri to this class
      std::string get_original_uri() const;
      // A getter for the normalized uri represented by this class
      std::string get_normalized_uri() const;
      // A getter for the local part of the normalized uri that is
      // an email represented by this class
      std::string get_local_part() const;
      // A getter for the global part of the normalized uri that is
      // an email represented by this class
      std::string get_global_part() const;
      // This method appends the passed suffix to the current uri instance
      // and returns the merged uri as a new uri instance
      uri append(const std::string&) const;

      // The equals operator returns true if the normalized uris of both uri
      // instances are equal.
      bool operator==(const uri&) const;
      // The equals operator returns true if the normalized uris of the uri
      // and the string instances are equal.
      bool operator==(const std::string&) const;
      // The unequals operator returns true if the normalized uris of both uri
      // instances are unequal.
      bool operator!=(const uri&) const;
      // The unequals operator returns true if the normalized uris of the uri
      // and the string instances are unequal.
      bool operator!=(const std::string&) const;
      // The assignment operator for this class
      uri& operator=(const uri&);
    private:
      // Normalizes the passed uri and returns the normalized uri string
      std::string normalize_(const std::string&);
      // Holds the original uri string
      std::string original_uri_;
      // Holds the normalized uri string
      std::string normalized_uri_;
    }; // end of class uri

    std::ostream& operator <<(std::ostream&, const crawler_cpp::data::uri&);
  } // end of namespace data
} // end of namespace crawler_cpp


void test_fun(); // TODO: remove

#endif // URI_HPP
