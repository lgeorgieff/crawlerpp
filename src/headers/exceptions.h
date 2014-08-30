// ============================================================================
// Author: Lukas Georgieff
// File: exceptions.h
// Description: This header file defines all exceptions used by the crawlercpp
//              library.
// Public interfaces:
//   *exception
//   *uri_exception
//   *not_implemented_exception
// Modification 08/15/2014: Definition of the classes exception, uri_exception
//                          and not_implemented_exception
// ============================================================================


#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>

namespace crawler_cpp {
  namespace exceptions {

    // The class for all other defined exception of the crawler_cpp library
    class exception : public std::exception {
    public:
      // The constructor for this class requires exactly one argument
      // representing the message/reasong of the exception
      exception(const std::string&);
      // A getter method for the message member
      std::string get_message() const;
      const char* what() const throw();
      // The desctructor of this class
      virtual ~exception() throw() {}
    protected:
      // The single member (message) of this class
      std::string message_;
    }; // end of class exception

    // The class for all bad uris and uri processing errors
    class uri_exception : public exception {
    public:
      // The constructor for this class requires two arguments:
      // 1: the actual error message
      // 2: the bad uri causing the actual error
      uri_exception(const std::string&, const std::string&);
      // A getter method for the uri member
      std::string get_uri() const;
      // The destructor of this class
      virtual ~uri_exception() throw() {}
    protected:
      // The uri member of this class
      std::string uri_;
    }; // end of class uri_exception

    // The class for exception handling of not implemented code segments
    class not_implemented_exception : public exception {
    public:
      // A constructor of this class that takes a message as argument
      not_implemented_exception(const std::string&);
      // The default constructor of this class
      not_implemented_exception();
      // The desctructor of this class
      virtual ~not_implemented_exception() throw() {}
    }; // end of class not_implemented_exception
  } // end of namespace exceptions
} // end of namespace crawler_cpp

std::ostream& operator<<(std::ostream&,
                const crawler_cpp::exceptions::exception&);

std::ostream& operator<<(std::ostream &,
                const crawler_cpp::exceptions::uri_exception &);

std::ostream& operator<<(std::ostream &,
                const crawler_cpp::exceptions::not_implemented_exception &);

#endif // EXCEPTIONS_H
