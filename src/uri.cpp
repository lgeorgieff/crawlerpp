// ====================================================================================================================
// Author: Lukas Georgieff
// File: uri.cpp
// Description: This implementation file implements all the uri class that
//              wraps all operations required for uri handling.
// Public interfaces:
//   *uri
// Modification 08/15/2014: Implementation of the class uri.
// Modification 02/09/2014: Implementation of a state machin for uri parsing.
// ====================================================================================================================


#include "uri.h"
#include "exceptions.h"

#include <iostream>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <cstddef>
#include <iostream> // TODO: remove
#include <cassert> // TODO: remove

using std::string;
using std::hex;
using std::stringstream;
using crawler_cpp::exceptions::uri_exception;

// The following namespace contains some helper methods used only in this implementation file
namespace {


  /********************************************************************************************************************
   http://tools.ietf.org/html/rfc3986#section-6
  ********************************************************************************************************************/

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

  // Transforms all characters of the passed string to lower case.
  string string_to_lower(const string &str){
    string result(str);
    transform(result.begin(), result.end(), result.begin(), tolower);
    return result;
  }

  // Transforms all characters of the passed string to upper case.
  string string_to_upper(const string &str){
    string result(str);
    transform(result.begin(), result.end(), result.begin(), toupper);
    return result;
  }

  /*
  bool is_ip(const string &hostname){
    // TODO: implement
    return false;
  }

  string normalize_hostname(const string &hostname){
    string lower_name(string_to_lower(hostname));
    if(is_ip(lower_name)){
      // TODO: resolve IP
      // jsmith@[192.168.2.1]
      //  * IPv4
      //  * IPv6
    } else {
      size_t current_label(0);
      for(size_t i(0); i != hostname.length(); ++i){
	// letters, digits, dots, hyphen
	// not end with hyphen
	// not start with digit or hyphen
	// length of label: 1 - 63
	// max length of hostname (including "."): 255
      }
    }
    return lower_name;
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
  // a uri_exception is thrown.
  string get_scheme_part(const string &uri){
    size_t pos_of_scheme = uri.find(":");
    if(pos_of_scheme == string::npos){
      throw uri_exception("No scheme found!", uri);
    } else {
      string scheme(uri.substr(0, pos_of_scheme));
      return string_to_lower(scheme);
    }
  }

  // Removes all comments, i.e. all parts within "(" and ")" of the passed
  // email part.
  string remove_comments_from_mailto(const string &part){
    size_t first_closing_pos(part.find(")"));
    size_t last_opening_pos(part.rfind("(", first_closing_pos));
    string result;
    if(first_closing_pos == string::npos && last_opening_pos == string::npos){
      return part;
    } else if((string::npos == first_closing_pos && string::npos != last_opening_pos)
       || (string::npos == last_opening_pos &&
	   string::npos == first_closing_pos)){
      throw uri_exception("Bad comment in mailto uri!", part);
    } else {
      result = part.substr(0, last_opening_pos);
      result += part.substr(first_closing_pos + 1,
		 part.length() - first_closing_pos - 1);
    }
    string next(remove_comments_from_mailto(result));
    if(next == result) return result;
    else return remove_comments_from_mailto(next);
  }

  // Sets the passed string references local and global to the local and global
  // part of the passed email.
  // Note: the passed email should already be normalized by means of
  // letter-case and percent-encodings.
  // Note: the passed email will be checked for syntax and length
  void get_parts_from_mailto(const string &mail, string &local, string &global){
    size_t pos_of_scheme(mail.find(":"));
    size_t pos_of_at(mail.find("@", pos_of_scheme + 1));
    if(pos_of_scheme == string::npos){
      throw uri_exception("No scheme found!", mail);
    } else if(pos_of_at == string::npos) {
      throw uri_exception("No \"@\" character found in mailto uri!", mail);
    } else if(pos_of_at == mail.length() - 1) {
      throw uri_exception("No global part found in mailto uri", mail);
    } else {
      local = mail.substr(pos_of_scheme + 1, pos_of_at - pos_of_scheme - 1);
      global = mail.substr(pos_of_at + 1, mail.length() - pos_of_at - 1);
    }
    try {
      local = string_to_lower(remove_comments_from_mailto(local));
      global = string_to_lower(remove_comments_from_mailto(global));
    }
    catch(uri_exception &err){
      throw uri_exception("Bad comment found in mailto uri!", mail);
    }
    if(local.length() > 64)
      throw uri_exception("Local part of mailto uri is too long!", mail);
    if(global.length() > 255)
      throw uri_exception("Global part of mailto uri is too long!", mail);
    if(local.length() + global.length() + 1 > 254)
      throw uri_exception("mailto uri is too long!", mail);
    for(char &c : local)
      if(c < 65 && c > 90 && c  < 97 && c > 122 && c < 48 && c > 57 && c != 33 && c < 35 && c > 39 && c < 42 && c > 43 && c != 45 && c != 47 && c != 61 && c != 63 && c < 95 && c > 96 && c < 123 && c > 126 && c != 46)
	throw uri_exception("The mailto uri contains the invalid character \"" + string(1, c) + "\"", mail);
    if(global[0] == '[' && global[global.length() - 1] == ']'){
      global.erase(global.length() - 1, 1);
      global.erase(0, 1);
    }
    global = normalize_hostname(global);
  }
  */

  namespace uri_statemachine {
    struct uri_parts {
      string scheme;
      string here;
      string query;
      string fragment;
    };

    // Merges two arrays of the same type and returns a const pointer to a const T.
    template<typename T>
    const T *const merge_arrays(const T arr_1[], size_t size_1,	const T arr_2[], size_t size_2){
      T *result(new char[size_1 + size_2]);
      for(size_t i(0); i != size_1; ++i) result[i] = arr_1[i];
      for(size_t i(size_1); i != size_1 + size_2; ++i) result[i] = arr_2[i - size_1];
      return result;
    }

    namespace constants {
      const size_t LOWER_ALPHA_SIZE(26);
      const char LOWER_ALPHA[LOWER_ALPHA_SIZE] { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	  'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
      const size_t UPPER_ALPHA_SIZE(26);
      const char UPPER_ALPHA[UPPER_ALPHA_SIZE] { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
	  'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
      const size_t ALPHA_SIZE(LOWER_ALPHA_SIZE + UPPER_ALPHA_SIZE);
      const char* const ALPHA(merge_arrays<char>(LOWER_ALPHA, LOWER_ALPHA_SIZE, UPPER_ALPHA, UPPER_ALPHA_SIZE));
      const size_t DIGIT_SIZE(10);
      const char DIGIT[DIGIT_SIZE] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
      const size_t UNRESERVED_SIZE(ALPHA_SIZE + DIGIT_SIZE + 4);
      const char* const UNRESERVED(merge_arrays<char>
				   (merge_arrays<char>(ALPHA, ALPHA_SIZE, DIGIT, DIGIT_SIZE), ALPHA_SIZE + DIGIT_SIZE,
				    (const char[]){ '-', '.', '_', '~' }, 4));
      // delimiters of the generic URI components
      const size_t GEN_DELIMITER_SIZE(7);
      const char GEN_DELIMITER[GEN_DELIMITER_SIZE] { ':', '/', '?', '#', '[', ']', '@' };
      // reserved for use as subcomponent delimiters
      const size_t SUB_DELIMITER_SIZE(11);
      const char SUB_DELIMITER[SUB_DELIMITER_SIZE] { '!', '$', '&', '\'', '(', ')', '*', '+', ',', ';', '=' };
      const size_t RESERVED_SIZE(GEN_DELIMITER_SIZE + SUB_DELIMITER_SIZE);
      const char* const RESERVED(merge_arrays<char>(GEN_DELIMITER, GEN_DELIMITER_SIZE, SUB_DELIMITER,
						    SUB_DELIMITER_SIZE));
      // characters that can be used for percent-encoding (lower and upper character version)
      const size_t HEX_DIGIT_SIZE(DIGIT_SIZE + 12);
      const char* const HEX_DIGIT(merge_arrays<char>
				  (DIGIT, DIGIT_SIZE,
				   (const char[]){ 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F' }, 12));
      const char ENCODING_START('%');
      const char SCHEME_END(':');
    } // end of namespace constants
      

    using constants::ALPHA;
    using constants::ALPHA_SIZE;
    using constants::DIGIT;
    using constants::DIGIT_SIZE;
    using constants::HEX_DIGIT;
    using constants::HEX_DIGIT_SIZE;
    using constants::ENCODING_START;
    using constants::SCHEME_END;
    
    // The percentage encoding is of the form "%" HEXDIG HEXDIG
    string percentage_state_1(const string &uri, string::const_iterator &uri_pos, char first_encoded_part){
      if(uri_pos == uri.end() || std::find(HEX_DIGIT, HEX_DIGIT + HEX_DIGIT_SIZE, *uri_pos) ==
	 HEX_DIGIT + HEX_DIGIT_SIZE)
	throw uri_exception("Bad percent-encoding, two HEX-characters expected after '%'!", uri);
      unsigned short encoded_value(hex_to_ushort(string() + first_encoded_part + *uri_pos));
      ++uri_pos;
      // encode free characters to their actual char value
      if(encoded_value == 0x2d || encoded_value == 0x2e || encoded_value == 0x5f || encoded_value == 0x7e ||
	 (encoded_value >= 0x41 && encoded_value <= 0x5a) ||
	 (encoded_value >= 0x61 && encoded_value <= 0x7a) ||
	 (encoded_value >= 0x30 && encoded_value <= 0x39)){
	return string() + static_cast<char>(encoded_value);
      // normalize percent encoding to upper case
      } else {
	return string_to_upper(string() + first_encoded_part + *uri_pos);
      }
    }

    // The percentage encoding is of the form "%" HEXDIG HEXDIG
    string percentage_state_0(const string &uri, string::const_iterator &uri_pos){
      if(uri_pos == uri.end() || std::find(HEX_DIGIT, HEX_DIGIT + HEX_DIGIT_SIZE, *uri_pos) ==
	 HEX_DIGIT + HEX_DIGIT_SIZE)
	 throw uri_exception("Bad percent-encoding, two HEX-characters expected after '%'!", uri);
      else return percentage_state_1(uri, ++uri_pos, *uri_pos);
    }

    // The scheme part is fo the form: ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
    string scheme_state_1(const string &uri, string::const_iterator &uri_pos, string built_scheme){
      if(uri_pos == uri.end()) return built_scheme;
      if(std::find(ALPHA, ALPHA + ALPHA_SIZE, *uri_pos) != ALPHA + ALPHA_SIZE ||
	 std::find(DIGIT, DIGIT + DIGIT_SIZE, *uri_pos) != DIGIT + DIGIT_SIZE ||
	 '+' == *uri_pos || '-' == *uri_pos || '.' == *uri_pos){
	return scheme_state_1(uri, ++uri_pos, built_scheme + static_cast<char>(std::tolower(*uri_pos)));
      }else if(*uri_pos == SCHEME_END){
	return built_scheme;
      }else if(*uri_pos == ENCODING_START){
	string percentage_result(string_to_lower(percentage_state_0(uri, ++uri_pos)));
	if(percentage_result.length() == 1) return scheme_state_1(uri, uri_pos, built_scheme + percentage_result);
	else throw uri_exception("The scheme part of an URI must contain only alphanumerical characters and the\
 characters '+', '-' and '.'!", uri);;
      }else{
	throw uri_exception("The scheme part of an URI must contain only alphanumerical characters and the characters\
 '+', '-' and '.'!", uri);
      }
    }

    // The scheme part is fo the form: ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
    string scheme_state_0(const string &uri, string::const_iterator &uri_pos){
      if(uri_pos == uri.end()) throw uri_exception("Empty URI!", uri);
      if(std::find(ALPHA, ALPHA + ALPHA_SIZE, *uri_pos) != ALPHA + ALPHA_SIZE){
	return scheme_state_1(uri, ++uri_pos, string() + char(std::tolower(*uri_pos)));
      }else if(*uri_pos == ENCODING_START){
	string percentage_result(string_to_lower(percentage_state_0(uri, ++uri_pos)));
	if(percentage_result.length() == 1) return scheme_state_1(uri, uri_pos, percentage_result);
	else throw uri_exception("The scheme part of an URI must start with an alphabetic character!", uri);
      }else{
	throw uri_exception("The scheme part of an URI must start with an alphabetic character!", uri);
      }
    }

    // The start state of this state machine
    uri_parts start_state(const string &uri){
      uri_parts parsed_uri_parts;
      string::const_iterator uri_pos(uri.begin());
      parsed_uri_parts.scheme = scheme_state_0(uri, uri_pos);
      std::cout << "scheme: " << parsed_uri_parts.scheme << " (" << uri << std::endl;
      return parsed_uri_parts;
    }
  } // end of namespace
} // end of anonymous namespace

// === constructors ===================================================================================================
crawler_cpp::data::uri::uri(const string &uri)
  :original_uri_(uri) {
  this->normalize_(uri);
}

crawler_cpp::data::uri::uri(const crawler_cpp::data::uri &uri)
  :original_uri_(uri.original_uri_), normalized_uri_(uri.normalized_uri_) { }

crawler_cpp::data::uri::~uri(){ }

// === getters ========================================================================================================
bool crawler_cpp::data::uri::is_http() const {
  return this->get_scheme() == uri::SCHEME_HTTP;
}

bool crawler_cpp::data::uri::is_https() const {
  return this->get_scheme() == uri::SCHEME_HTTPS;
}

bool crawler_cpp::data::uri::is_mailto() const {
  return this->get_scheme() == uri::SCHEME_MAILTO;
}

string crawler_cpp::data::uri::get_scheme() const {
  return string(this->normalized_uri_.substr(0, this->normalized_uri_.find(":")));
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

string crawler_cpp::data::uri::get_local_part() const {
  size_t pos_of_scheme(this->normalized_uri_.find(":"));
  size_t pos_of_at(this->normalized_uri_.find("@", pos_of_scheme + 1));
  return this->normalized_uri_.substr(pos_of_scheme + 1, pos_of_at - pos_of_scheme - 1);
}

string crawler_cpp::data::uri::get_global_part() const {
  size_t pos_of_scheme(this->normalized_uri_.find(":"));
  size_t pos_of_at(this->normalized_uri_.find("@", pos_of_scheme + 1));
  return  this->normalized_uri_.substr(pos_of_at + 1, this->normalized_uri_.length() - pos_of_at - 1);
}

string crawler_cpp::data::uri::get_original_uri() const {
  return this->original_uri_;
}

string crawler_cpp::data::uri::get_normalized_uri() const {
  return this->normalized_uri_;
}

crawler_cpp::data::uri crawler_cpp::data::uri::append(const string &suffix) const {
  // TODO: implement
  // See: http://tools.ietf.org/html/rfc3986#section-5.2.1
  throw crawler_cpp::exceptions::not_implemented_exception();
}

// === operators ======================================================================================================
std::ostream& crawler_cpp::data::operator<<(std::ostream &os, const crawler_cpp::data::uri &uri){
  os << uri.get_normalized_uri();
  return os;
}

bool crawler_cpp::data::uri::operator==(const crawler_cpp::data::uri &uri) const {
  return this->get_normalized_uri() == uri.get_normalized_uri();
}

bool crawler_cpp::data::uri::operator==(const string &uri) const {
  return this->get_normalized_uri() == uri;
}

bool crawler_cpp::data::uri::operator!=(const crawler_cpp::data::uri &uri) const {
  return !(*this == uri);
}

bool crawler_cpp::data::uri::operator!=(const string &uri) const {
  return !(*this == uri);
}

crawler_cpp::data::uri& crawler_cpp::data::uri::operator=(const crawler_cpp::data::uri &uri){
  if(*this == uri) return *this;
  this->original_uri_ = uri.original_uri_;
  this->normalized_uri_ = uri.normalized_uri_;
  return *this;
}

// === private helpers ================================================================================================
string crawler_cpp::data::uri::normalize_(const string &uri){
  /*  string normalized_encodings(normalize_percent_encodings(uri));
  //  std::cout << "normalized_encodings: _" << normalized_encodings << "_" << std::endl;
  string scheme(get_scheme_part(normalized_encodings));
  //  std::cout << "scheme: _" << scheme << "_" << std::endl;

  if(scheme == SCHEME_MAILTO){
    string local;
    string global;
    get_parts_from_mailto(normalized_encodings, local, global);
    this->normalized_uri_ = SCHEME_MAILTO + ":" + local + "@" + global;
  } else {

  }
  */


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



void test_fun(){
  uri_statemachine::start_state("http://this.is/a#test%7eJohn%a2Doe?query_1=hello&query_2=world");
  uri_statemachine::start_state("HTTP://123.95.10.23:443/a#test%7eJohn%a2Doe?query_1=hello&query_2=world");
  uri_statemachine::start_state("%48TTP://123.95.10.23:443/a#test%7eJohn%a2Doe?query_1=hello&query_2=world");
  uri_statemachine::start_state("HT%54P://123.95.10.23:443/a#test%7eJohn%a2Doe?query_1=hello&query_2=world");
  uri_statemachine::start_state("mailto:JOHN.doe@googleMAiL.Com");
  uri_statemachine::start_state("mailto:JOHN.do(any comment)e@google(another (nested comment) comment)MaiL.Com");
  uri_statemachine::start_state("mailto:JOHN.doe(any comment)@[123.226.11.23](An IPv4 address instead a hostname)");
  uri_statemachine::start_state("mailto:JOhn.d(any comment)oe@[::ffff:123.226.11.23](An IPv6 address instead a hostname)");
  uri_statemachine::start_state("mailto:JOhN.(any comment)doe@[2001:0db8:0000:08d3:0000:8a2e:0070:7344](An IPv6 address instead a hostname)");
  uri_statemachine::start_state("mailto:(any comment)john.doe@[2001:db8::1428:57ab](An IPv6 address instead a hostname)");
  try{
    uri_statemachine::start_state("ma%3filto:(any comment)john.doe@[2001:db8::1428:57ab](An IPv6 address instead a hostname)");
    assert(false);
  } catch(uri_exception &err){ }
  try{
    uri_statemachine::start_state(":(any comment)john.doe@[2001:db8::1428:57ab](An IPv6 address instead a hostname)");
    assert(false);
  }catch(uri_exception &err){ }
}
