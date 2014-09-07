// ====================================================================================================================
// Author: Lukas Georgieff
// File: uri.cpp
// Description: This implementation file implements all the uri class that
//              wraps all operations required for uri handling.
// Public interfaces:
//   *uri
// Modification 08/15/2014: Implementation of the class uri.
// Modification 02/09/2014: Implementation of a state machine for uri parsing.
// ====================================================================================================================


#include "uri.h"
#include "exceptions.h"
#include "utils.h"

#include <iostream>
#include <cstddef>
#include <numeric>
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

  namespace uri_statemachine {
    struct uri_parts {
      string scheme;
      string user;
      string host;
      string port;
      string path;
      string query;
      string fragment;
    };

    namespace constants {
      using crawler_cpp::utils::merge_arrays;

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
      // The character that marks the begining of a percent-encoding part within an URI
      const char ENCODING_START('%');
      // The character that marks the end of the scheme part within an URI
      const char SCHEME_END(':');
      // The character that introduces a query within a URI
      const char QUERY_START('?');
      // The character that introduces a fragment within a character
      const char FRAGMENT_START('#');
      // The character that represents the path separator in URIs
      const char PATH_SEPARATOR('/');
      // The charachter that marks the begining of an IPv6 address within an URI
      const char IPV6_START('[');
      // The character that marks the end of an IPv6 address within an URI
      const char IPV6_END(']');
      // The separator of IPv6 sections, e.g. a:b:c:d:e:f:g:h
      const char IPV6_SEPARATOR(':');
      // The separator of IPv4 sections, e.g. a.b.c.d
      const char IPV4_SEPARATOR('.');
      // The number of maximum fields in the syntax a:b:c:d:e:f:g:h of an IPv6 address
      const size_t IPV6_SIZE(8);
    } // end of namespace constants
      

    using constants::ALPHA;
    using constants::ALPHA_SIZE;
    using constants::DIGIT;
    using constants::DIGIT_SIZE;
    using constants::HEX_DIGIT;
    using constants::HEX_DIGIT_SIZE;
    using constants::ENCODING_START;
    using constants::SCHEME_END;
    using constants::QUERY_START;
    using constants::FRAGMENT_START;
    using constants::PATH_SEPARATOR;
    using constants::IPV6_START;
    using constants::IPV6_END;
    using constants::IPV6_SEPARATOR;
    using constants::IPV4_SEPARATOR;
    using crawler_cpp::utils::hex_to_ushort;
    using crawler_cpp::utils::string_to_upper;
    using crawler_cpp::utils::string_to_lower;
    
  
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



    string ip_v6_state_1(const string &uri){
      // TODO: implement
      return "";
    }


    string ip_v6_state_0(const string &uri, string::const_iterator &uri_pos){
      const size_t IPV6_SIZE(8);
      string results[IPV6_SIZE];
      size_t results_length(0);
      string current_str;
      while(uri_pos != uri.end()){
	// ::
	if(*uri_pos == IPV6_SEPARATOR && current_str.empty() && results_length < IPV6_SIZE){
	  results[results_length++] = string() + *uri_pos++;
        // :
	} else if(*uri_pos == IPV6_SEPARATOR && results_length < IPV6_SIZE){
          results[results_length++] = current_str;
	  current_str = "";
	  ++uri_pos;
	// .
	} else if(*uri_pos == IPV4_SEPARATOR && results_length < IPV6_SIZE){
	    // TODO
        // HEXDIGIT or DIGIT
	} else if(std::find(HEX_DIGIT, HEX_DIGIT + HEX_DIGIT_SIZE, *uri_pos) != HEX_DIGIT + HEX_DIGIT_SIZE
		  && results_length < IPV6_SIZE){
	  current_str += *uri_pos++;	  
	// %xx
        } else if(*uri_pos == ENCODING_START){
	  string percentage_result(percentage_state_0(uri, ++uri_pos));
	  if(percentage_result.length() == 1
	     && std::find(HEX_DIGIT, HEX_DIGIT + HEX_DIGIT_SIZE, percentage_result[0]) != HEX_DIGIT + HEX_DIGIT_SIZE) {
	    // TODO: implement
	  } else if(percentage_result.length() == 1 && percentage_result[0] == IPV6_SEPARATOR) {
	    // TODO: implement
	  } else if(percentage_result.length() == 1 && percentage_result[0] == IPV4_SEPARATOR) {
	    // TODO: implement
	  } else if(percentage_result.length() == 1 && percentage_result[0] == IPV6_END) {
	    // TODO: implement
	  } else {
	    throw uri_exception("Bad IP v6 address found!", uri);
	  }
	// ]
        } else if(*uri_pos == IPV6_END) {
          results[results_length++] = current_str;
          current_str = "";
	// bad IPv6
	} else {
          throw uri_exception("Bad IP v6 address found!", uri);
	}
      } // end of while(uri_pos != uri.end())

      string final_result;
      for(size_t i(0); i != results_length; ++i){
	if(results[i].length() == 1 && results[i][0] == IPV6_SEPARATOR)
	  for(size_t j(0); j != IPV6_SIZE + 1 - results_length; ++j) final_result += final_result.empty() ? "0" : ":0";
	else if(std::find(results[i].begin(), results[i].end(), IPV4_SEPARATOR) != results[i].end())
	  final_result += string_to_lower(results[i]);
	else
	  final_result += final_result.empty() ? final_result[i] : IPV6_SEPARATOR + final_result[i];
      }
      return final_result;


      // IPv6address = 6( h16 ":" ) ls32
      //               / "::" 5( h16 ":" ) ls32
      //               / [h16 ] "::" 4( h16 ":" ) ls32
      //               / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
      //               / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
      //               / [ *3( h16 ":" ) h16 ] "::"  h16 ":" ls32
      //               / [ *4( h16 ":" ) h16 ] "::" ls32
      //               / [ *5( h16 ":" ) h16 ] "::" h16
      //               / [ *6( h16 ":" ) h16 ] "::"
        // h16 = 1*4HEXDIG
        // ls32 = ( h16 ":" h16 )
        //        / IPv4address

      // IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
      // dec-octet = DIGIT
      //             / %x31-39 DIGIT
      //             / "1" 2DIGIT
      //             / "2" %x30-34 DIGIT
      //             / "25" %x30-35
    }





    void authority_state_0(const string &uri, string::const_iterator &uri_pos,
			     string &user_info, string &host, string &port){
      if(*uri_pos == IPV6_START){
	ip_v6_state_0(uri, ++uri_pos);
	// TODO: ipv 6
      } else if(*uri_pos == ENCODING_START) {
	// TODO: %..
      } else {
	// TODO: bad character
	throw uri_exception("", uri);
      }

      // authority = [ userinfo "@" ] host [ ":" port ]

        // userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
          // unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
          // pct-encoded = "%" HEXDIG HEXDIG
          // sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="

        // host = IP-literal / IPv4address / reg-name
          // IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
          // IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
           // dec-octet = DIGIT / %x31-39 DIGIT / "1" 2DIGIT / "2" %x30-34 DIGIT / "25" %x30-35
         // reg-name = *( unreserved / pct-encoded / sub-delims )

        // port = *DIGIT


    }

    string query_state_0(const string &uri, string::const_iterator &uri_pos){
      return "";
    }

    string fragment_state_0(const string &uri, string::const_iterator &uri_pos){
      return "";
    }


    /*
The authority component is preceded by a double slash ("//") and is
terminated by the next slash ("/"), question mark ("?"), or number
sign ("#") character, or by the end of the URI.
    */

    /*
URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

hier-part = "//" authority path-abempty / path-absolute  / path-rootless / path-empty

authority = [ userinfo "@" ] host [ ":" port ]

userinfo = *( unreserved / pct-encoded / sub-delims / ":" )

host = IP-literal / IPv4address / reg-name

port = *DIGIT

     */






    /*
   If host matches the rule for IPv4address, then it should be
   considered an IPv4 address literal and not a reg-name.  Although host
   is case-insensitive, producers and normalizers should use lowercase
   for registered names and hexadecimal addresses for the sake of
   uniformity, while only using uppercase letters for percent-encodings.

The authority component is preceded by a double slash ("//") and is
terminated by the next slash ("/"), question mark ("?"), or number
sign ("#") character, or by the end of the URI.

URI producers and normalizers should omit the ":" delimiter that
separates host from port if the port component is empty.  Some
schemes do not allow the userinfo and/or port subcomponents.

If a URI contains an authority component, then the path component
must either be empty or begin with a slash ("/") character. 

Use of the format "user:password" in the userinfo field is
deprecated.  Applications should not render as clear text any data
after the first colon (":") character found within a userinfo
subcomponent unless the data after the colon is the empty string
(indicating no password).  Applications may choose to ignore or
reject such data when it is received as part of a reference and
should reject the storage of such data in unencrypted form.  The
passing of authentication information in clear text has proven to be
a security risk in almost every case where it has been used.


Examples:
de.wikipedia.org
user@example.com:8080
192.0.2.16:80
[2001:db8::7]
       */


	     /*
When authority is present, the path must
either be empty or begin with a slash ("/") character

When authority is not present, the path cannot begin with two slash
characters ("//")
	     */







    // The scheme part is fo the form: ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
    string scheme_state_1(const string &uri, string::const_iterator &uri_pos, string built_scheme){
      if(uri_pos == uri.end()) return built_scheme;
      if(std::find(ALPHA, ALPHA + ALPHA_SIZE, *uri_pos) != ALPHA + ALPHA_SIZE ||
	 std::find(DIGIT, DIGIT + DIGIT_SIZE, *uri_pos) != DIGIT + DIGIT_SIZE ||
	 '+' == *uri_pos || '-' == *uri_pos || '.' == *uri_pos){
	char c(std::tolower(*uri_pos));
	return scheme_state_1(uri, ++uri_pos, built_scheme + c);
      }else if(*uri_pos == SCHEME_END){
	return built_scheme;
      }else if(*uri_pos == ENCODING_START){
	string percentage_result(string_to_lower(percentage_state_0(uri, ++uri_pos)));
	if(percentage_result.length() == 1) return scheme_state_1(uri, uri_pos, built_scheme + percentage_result);
	else throw uri_exception("The scheme part of an URI must contain only alphanumerical characters and the\
 characters '+', '-' and '.'!", uri);
      }else{
	throw uri_exception("The scheme part of an URI must contain only alphanumerical characters and the characters\
 '+', '-' and '.'!", uri);
      }
    }

    // The scheme part is fo the form: ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
    string scheme_state_0(const string &uri, string::const_iterator &uri_pos){
      if(uri_pos == uri.end()) throw uri_exception("Empty URI!", uri);
      if(std::find(ALPHA, ALPHA + ALPHA_SIZE, *uri_pos) != ALPHA + ALPHA_SIZE){
	char c(std::tolower(*uri_pos));
	return scheme_state_1(uri, ++uri_pos, string() + c);
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
      if(*uri_pos == PATH_SEPARATOR && *(++string::const_iterator(uri_pos)) == PATH_SEPARATOR)
	authority_state_0(uri, ++(++uri_pos), parsed_uri_parts.user, parsed_uri_parts.host, parsed_uri_parts.port);

      /*
	TODO: implement hier-part
	  * "//" authority path-abempty
	  * path-absolute
	  * path-rootless
	  * path-empty

       */
      if(*uri_pos == QUERY_START) parsed_uri_parts.query = query_state_0(uri, uri_pos);
      if(*uri_pos == FRAGMENT_START) parsed_uri_parts.fragment = fragment_state_0(uri, uri_pos);
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
