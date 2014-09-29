// ============================================================================
// Author: Lukas Georgieff
// File: exceptions.h
// Description: This header file defines all exceptions used by the crawlercpp
//              library.
// Public interfaces:
//   *exception
//   *uri_exception
//   *db_exception
//   *not_implemented_exception
// ============================================================================


#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>

namespace crawler_pp {
  namespace exceptions {

    // The class for all other defined exception of the crawler_pp library
    class exception : public std::exception {
    public:
      // We want no default constructor
      exception() = delete;
      // The constructor for this class requires exactly one argument
      // representing the message/reason of the exception
      exception(const std::string&);
      // A getter method for the message member
      std::string get_message() const;
      const char* what() const throw();
      // The desctructor of this class
      virtual ~exception() throw();
    protected:
      // The single member (message) of this class
      std::string message_;
    }; // end of class exception

    // The class for all bad uris and uri processing errors
    class uri_exception : public exception {
    public:
      // We want no default constructor
      uri_exception() = delete;
      // The constructor for this class requires two arguments:
      // 1: the actual error message
      // 2: the bad uri causing the actual error
      uri_exception(const std::string&, const std::string&);
      // A getter method for the uri member
      std::string get_uri() const;
      // The destructor of this class
      virtual ~uri_exception() throw();
    protected:
      // The uri member of this class
      std::string uri_;
    }; // end of class uri_exception

    // The class for all database and database processing errors
    class db_exception : public exception {
    public:
      // We want no default constructor
      db_exception() = delete;
      // The constructor for this class requires exactly one argument
      // representing the message/reason of the exception
      db_exception(const std::string&);
      // The destructor of this class
      virtual ~db_exception() throw();
    };

    // The class for exception handling of not implemented code segments
    class not_implemented_exception : public exception {
    public:
      // A constructor of this class that takes a message as argument
      not_implemented_exception(const std::string&);
      // The default constructor of this class
      not_implemented_exception();
      // The desctructor of this class
      virtual ~not_implemented_exception() throw();
    }; // end of class not_implemented_exception

    std::ostream& operator<<(std::ostream&, const crawler_pp::exceptions::exception&);

    std::ostream& operator<<(std::ostream &, const crawler_pp::exceptions::uri_exception &);
  } // end of namespace exceptions
} // end of namespace crawler_pp
#endif // EXCEPTIONS_H
