// ============================================================================
// Author: Lukas Georgieff
// File: exceptions.h
// Description: This header file defines all exceptions used by the crawlercpp
//              library.
// Public interfaces:
//   *exception
//   *bad_uri_exception
//   *not_implemented_exception
// Modification 08/15/2014: Definition of the classes exception, uri_exception
//                          and not_implemented_exception
// ============================================================================


#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

namespace crawler_cpp {
  namespace exceptions {

    // The class for all other defined exception of the crawler_cpp library
    class exception {
    public:
      // The constructor for this class requires exactly one argument
      // representing the message/reasong of the exception
      exception(const std::string&);
      // A getter method for the message member
      std::string get_message() const;
    protected:
      // The single member (message) of this class
      std::string message_;
    }; // end of class exception

    // The class for all bad uris and uri processing errors
    class bad_uri_exception : public exception {
    public:
      // The constructor for this class requires two arguments:
      // 1: the actual error message
      // 2: the bad uri causing the actual error
      bad_uri_exception(const std::string&, const std::string&);
      // A getter method for the uri member
      std::string get_uri() const;
    protected:
      // The uri member of this class
      std::string uri_;
    }; // end of class bad_uri_exception

    // The class for exception handling of not implemented code segments
    class not_implemented_exception : public exception {
    public:
      not_implemented_exception(const std::string&);
      not_implemented_exception();
    }; // end of class not_implemented_exception
  } // end of namespace exceptions
} // end of namespace crawler_cpp

std::ostream& operator<<(std::ostream&,
                const crawler_cpp::exceptions::exception&);

std::ostream& operator<<(std::ostream &,
                const crawler_cpp::exceptions::bad_uri_exception &);

std::ostream& operator<<(std::ostream &,
                const crawler_cpp::exceptions::not_implemented_exception &);

#endif // EXCEPTIONS_H
