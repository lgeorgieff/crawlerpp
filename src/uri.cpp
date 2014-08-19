// ============================================================================
// Author: Lukas Georgieff
// File: uri.cpp
// Description: This implementation file implements all the uri class that
//              wraps all operations required for uri handling.
// Public interfaces:
//   *uri
// Modification 08/15/2014: Implementation of the class uri.
// ============================================================================


#include "uri.h"
#include "exceptions.h"

#include <iostream>
#include <cctype>
#include <sstream>
#include <algorithm>


using std::string;
using std::toupper;
using std::hex;
using std::stringstream;
using crawler_cpp::exceptions::bad_uri_exception;

// The following namespace contains some helper methods used only in
// this implementation file
namespace {


  /****************************************************************************
   http://tools.ietf.org/html/rfc3986#section-6
   ***************************************************************************/

  // Transforms tha passed string to an unsigned short.
  // Note: the base is 16 (i.e. hex)
  // Note: no prefix such as 0x is required
  unsigned short hex_to_ushort(const string &hex_str){
    unsigned short x;   
    stringstream ss;
    ss << hex << hex_str;
    ss >> x;
    return x;
  }

  // Transforms unreserved %-escaped characters to their actual values
  // and all percent-encodings to upper case.
  // See: http://tools.ietf.org/html/rfc3986#section-2.3
  string normalize_percent_encodings(const string &uri){
    string result;
    for(size_t i(0); i < uri.length(); ++i){
      if(uri[i] == '%' && i < uri.length() - 2){
	unsigned short value(hex_to_ushort(uri.substr(i + 1, 2)));
	if(value == 0x2d || value == 0x2e || value == 0x5f || value == 0x7e ||
	   (value >= 0x41 && value <= 0x5a) ||
	   (value >= 0x61 && value <= 0x7a) ||
	   (value >= 0x30 && value <= 0x39)){
	  result += static_cast<char>(value);
	  i += 2;
	} else {
	  result += uri[i];
	  result += toupper(uri[i + 1]);
	  result += toupper(uri[i + 2]);
	  i += 2;
	}
      }
      else { result += uri[i]; }
    }
    return result;
  }

  // Returns the scheme of the passed uri. If no scheme is found,
  // a bad_uri_exception is thrown.
  string get_scheme_part(const string &uri){
    size_t pos_of_scheme = uri.find(":");
    if(pos_of_scheme == string::npos){
      throw bad_uri_exception("No schem found!", uri);
    } else {
      string scheme(uri.substr(0, pos_of_scheme));
      std::transform(scheme.begin(), scheme.end(), scheme.begin(), tolower);
      return scheme;
    }
  }
}

// === constructors ===========================================================
crawler_cpp::data::uri::uri(const string &uri)
  :original_uri_(uri) {
  this->normalize_(uri);
}

crawler_cpp::data::uri::uri(const crawler_cpp::data::uri &uri)
  :original_uri_(uri.original_uri_), normalized_uri_(uri.normalized_uri_) { }

crawler_cpp::data::uri::~uri(){ }

// === getters ================================================================
bool crawler_cpp::data::uri::is_http() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

bool crawler_cpp::data::uri::is_https() const {
  // TDOO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

bool crawler_cpp::data::uri::is_mail() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

string crawler_cpp::data::uri::get_scheme() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

string crawler_cpp::data::uri::get_host() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

string crawler_cpp::data::uri::get_port() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

string crawler_cpp::data::uri::get_path() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

bool crawler_cpp::data::uri::has_query() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

string crawler_cpp::data::uri::get_query() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

bool crawler_cpp::data::uri::has_fragment() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

string crawler_cpp::data::uri::get_fragment() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

string crawler_cpp::data::uri::get_original_uri() const {
  return this->original_uri_;
}

string crawler_cpp::data::uri::get_normalized_uri() const {
  return this->normalized_uri_;
}

crawler_cpp::data::uri crawler_cpp::data::uri::append(
  const string &suffix) const {
  // TODO: implement
  // See: http://tools.ietf.org/html/rfc3986#section-5.2.1
  throw crawler_cpp::exceptions::not_implemented_exception();
}

// === operators ==============================================================
std::ostream& operator<<(std::ostream& os, const crawler_cpp::data::uri &uri){
  os << uri.get_normalized_uri();
  return os;
}

bool crawler_cpp::data::uri::operator==(const crawler_cpp::data::uri &uri)
  const {
  return this->get_normalized_uri() == uri.get_normalized_uri();
}

bool crawler_cpp::data::uri::operator==(const string &uri) const {
  return this->get_normalized_uri() == uri;
}

bool crawler_cpp::data::uri::operator!=(const crawler_cpp::data::uri &uri)
  const {
  return !(*this == uri);
}

bool crawler_cpp::data::uri::operator!=(const string &uri) const {
  return !(*this == uri);
}

crawler_cpp::data::uri& crawler_cpp::data::uri::operator=(
  const crawler_cpp::data::uri &uri){
  if(*this == uri) return *this;
  this->original_uri_ = uri.original_uri_;
  this->normalized_uri_ = uri.normalized_uri_;
  return *this;
}

// === private helpers ========================================================
string crawler_cpp::data::uri::normalize_(const string& uri){
  string normalized_encodings(normalize_percent_encodings(uri));
  std::cout << "normalized_encodings: _" << normalized_encodings << "_" << std::endl;
  string scheme(get_scheme_part(normalized_encodings));
  std::cout << "scheme: _" << scheme << "_" << std::endl;

  // TODO: implement
  /*
  // * replace IP with domain name
  // * add a port if no one exists
  // * add default port if hosts ends with ":"
  // * remove duplicated slashes
  // * convert scheme and host to lowercase
  // * capitalize letters in escape sequences
  // * Decoding percent-encoded octets of unreserved characters. For consistency, percent-encoded octets in the ranges of ALPHA (%41–%5A and %61–%7A), DIGIT (%30–%39), hyphen (%2D), period (%2E), underscore (%5F), or tilde (%7E) should not be created by URI producers and, when found in a URI, should be decoded to their corresponding unreserved characters by URI normalizers.[2] Example:
http://www.example.com/%7Eusername/ → http://www.example.com/~username/
  // * replacing http on port 443 to https
  // * Removing dot-segments. The segments “..” and “.” can be removed from a URL according to the algorithm described in RFC 3986 (or a similar algorithm). Example:
http://www.example.com/../a/b/../c/./d.html → http://www.example.com/a/c/d.html
However, if a removed ".." component, e.g. "b/..", is a symlink to a directory with a different parent, eliding "b/.." will result in a different path and URL.[3] In rare cases depending on the web server, this may even be true for the root directory (e.g. "//www.example.com/.." may not be equivalent to "//www.example.com/".
  // * Sorting the query parameters. Some web pages use more than one query parameter in the URL. A normalizer can sort the parameters into alphabetical order (with their values), and reassemble the URL. Example:
http://www.example.com/display?lang=en&article=fred → http://www.example.com/display?article=fred&lang=en
However, the order of parameters in a URL may be significant (this is not defined by the standard) and a web server may allow the same variable to appear multiple times.[4]
  // * Removing the "?" when the query is empty. When the query is empty, there may be no need for the "?". Example:
http://www.example.com/display? → http://www.example.com/display
  */
  // Note: throws an uri_exception
  //throw crawler_cpp::exceptions::not_implemented_exception();
  return "";
}
