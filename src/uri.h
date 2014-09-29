// ============================================================================
// Author: Lukas Georgieff
// File: uri.h
// Description: This header file defines the uri classes that wrap all
//              operations required for uri handling.
// Public interfaces:
//   * uri
//   * waiting_uri
//   * visited_uri
// ============================================================================


#ifndef URI_HPP
#define URI_HPP

#include <odb/core.hxx> 

#include <string>
#include <iostream>

namespace crawler_pp {
  namespace data {

    // This class represents the base type for all uri classes
    class uri {
    public:
      // Defines the max length of an URI, this length is applied on normalized
      // URIs, i.e. the passed URI string can be longer than MAX_LENGTH but
      // after normalization the URI must not be longer.
      static const size_t MAX_SIZE;
      // This constructor takes a string representing an URI and uses this
      // value as internal URI of this class.
      uri(std::string);
      // The copy constructor, copies the normalized URI value from the passed
      // uri instance.
      uri(const uri&);
      // The move constructor, moves the uri value from the passed uri instance
      // to the target uri instance
      uri(uri&&);
      // This method must be implemented by all derived classes and performs
      // a persist operation, i.e. the value is stored in the DB.
      // If the operation succeeded and the value was stored, the return value
      // is true.
      // If the the value was not stored, since the same URI already existed in
      // the DB, the return value is false.
      // If any error occurrs the crawler_pp::excpetions::db_exception is
      // thrown.
      virtual bool persist() = 0;
      // Returns true if the passed uri is already known, i.e. stored in the DB
      // Note this function is a template, i.e. only the uri type is checked
      // that is specified by the type parameter.
      template<typename T> static bool is_known(const uri&);
      // Returns true if the passed uri is already known, i.e. stored in the DB
      // Note this function is a template, i.e. only the uri type is checked
      // that is specified by the type parameter.
      template<typename T> static bool is_known(std::string);
      // The assignment operator for for the uri class.
      uri& operator=(const uri&);
      // The move assignment operator for the uri class.
      uri& operator=(uri&&);
      // Returns true if the passed uri instance is equal to this instance.
      bool operator==(const uri&) const;
      // Returns true if the passed uri-string is semantically equal to this
      // instance.
      bool operator==(const std::string&) const;
      // Returns true if the passed uri instance is unequal to this instance.
      bool operator!=(const uri&) const;
      // Returns true if the passed uri-string is semantically unequal to this
      // instance.
      bool operator!=(const std::string&) const;
      // Returns true if this instance has an uri-string value that is lower
      // than the uri-string of the passed uri instance.
      bool operator<(const uri&) const;
      // Returns true if this instance has an uri-string value that is lower
      // than the passed uri-string.
      bool operator<(const std::string&) const;
      // Returns true if this instance has an uri-string value that is greater
      // than the uri-string of the passed uri instance.      
      bool operator>(const uri&) const;
      // Returns true if this instance has an uri-string value that is greater
      // than the passed uri-string.
      bool operator>(const std::string&) const;
      // Returns true if this instance has an uri-string value that is lower
      // or equal than the uri-string of the passed uri instance.
      bool operator<=(const uri&) const;
      // Returns true if this instance has an uri-string value that is lower
      // or equal than the passed uri-string.
      bool operator<=(const std::string&) const;
      // Returns true if this instance has an uri-string value that is greater
      // or equal than the uri-string of the passed uri instance.
      bool operator>=(const uri&) const;
      // Returns true if this instance has an uri-string value that is greater
      // or equal than the passed uri-string.
      bool operator>=(const std::string&) const;
      // Returns the size of the stored string value of the URI which is
      // represented by this uri instance.
      size_t size() const;
      // Returns 0 if both uri instances are equal.
      // Returns <0 if either the value of the first character that does not
      // match is lower in the compared uri, or all compared characters
      // match but the compared uri is shorter.
      // Returns >0 if either the value of the first character that does not
      // match is greater in the compared uri, or all compared characters
      // match but the compared uri is longer.
      virtual int compare(const uri&) const;
      // Returns 0 if this uri instance and the passed strin are semantically
      // equal, i.e. the normalized value if the passed uri string is used
      // internally.
      // Returns <0 if either the value of the first character that does not
      // match is lower in the compared uri, or all compared characters
      // match but the compared uri is shorter.
      // Returns >0 if either the value of the first character that does not
      // match is greater in the compared uri-string, or all compared
      // characters match but the compared uri-string is longer.
      virtual int compare(const std::string) const;
      // A getter for the member value_
      std::string get_value() const;
      // The virtual destructor - there is nothig todo here
      virtual ~uri() = 0;
      // The type odb::access if declared a friend of this class to be able to
      // access private/protected members.
      friend class odb::access;
    protected:
      // The default constructor is required by odb
      uri();
      // The actual data is stored as a string
      std::string value_;
      // The setter for this member is private and performs URI normalization,
      // i.e. this method should only be used if an URI is passed to this class
      // that is extern and unknown yet. Copy constructors and move
      // constructors souldn't use this setter but rather assign the stirng
      // value directly to increase efficiency.
      void set_value(std::string);
    }; // end of class uri

    // This class represents the uri type for all uris that are used by the
    // scheduler to load the next page (described by the next waiting uri)
    class waiting_uri : public uri{
    public:
      // This constructor takes a string representing an URI and uses this
      // value as internal URI of this class.
      waiting_uri(std::string);
      // The copy constructor, copies the normalized URI value from the passed
      // waiting_uri instance.
      waiting_uri(const waiting_uri&);
      // The move constructor, moves the uri value from the passed waiting_uri
      // instance to the target waiting_uri instance
      waiting_uri(waiting_uri&&); 
      // The assignment operator for for the waiting_uri class.
      waiting_uri& operator=(const waiting_uri&);
      // The move assignment operator for the waiting_uri class.
      waiting_uri& operator=(waiting_uri&&);
      // See: crawler_pp::data::uri::persist
      virtual bool persist();
      // Returns the next waiting_uri instance from the DB. If not one exists
      // in the DB, the crawler_pp::exceptions::db_exception is thrown
      static waiting_uri get_next();
      // Returns true if a waiting_uri exists in the DB. Otherwise false is
      // returned.
      static bool has_next();
      // The destructor of this class - there is nothig todo here
      ~waiting_uri();
      // The type odb::access if declared a friend of this class to be able to
      // access private/protected members.
      friend class odb::access;
    protected:
      // The default constructor is required by odb
      waiting_uri();
    }; // end of class waiting_url

    // This class represents the uri type for all uri that marks pages which
    // were already downloaded by the scheduler
    class visited_uri : public uri {
    public:
      // This constructor takes a string representing an URI and uses this
      // value as internal URI of this class.
      visited_uri(std::string);
      // The copy constructor, copies the normalized URI value from the passed
      // visited_uri instance.
      visited_uri(const visited_uri&);
      // The move constructor, moves the uri value from the passed visited_uri
      // instance to the target visited_uri instance
      visited_uri(visited_uri&&);
      // The assignment operator for for the visited_uri class.
      visited_uri& operator=(const visited_uri&);
      // The move assignment operator for the visited_uri class.
      visited_uri& operator=(visited_uri&&);
      // see: crawler_pp::data::uri::persist
      virtual bool persist();
      // The destructor of this class - there is nothig todo here
      ~visited_uri();
      // The type odb::access if declared a friend of this class to be able to
      // access private/protected members.
      friend class odb::access;
    protected:
      // The default constructor is required by odb
      visited_uri();
    }; // end of class visited_uri

    // Writes the passed uri instance to the given ostream.
    std::ostream& operator<<(std::ostream&, const uri&);
  } // end of namespace data
} // end of namespace crawler_pp


// Include the ODB pragmas for persisting the uri classes
// see: http://www.codesynthesis.com/products/odb/doc/manual.xhtml#14
#ifdef ODB_COMPILER
  #include "uri.pragma.h"
#endif

#endif // URI_HPP
