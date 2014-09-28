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

namespace crawler_cpp {
  namespace data {

    // This class represents the base type for all uri classes
    class uri {
    public:
      uri(std::string);
      uri(const uri&);
      virtual bool persist() = 0;
      template<typename T> static bool is_known(const uri&);
      template<typename T> static bool is_known(std::string);
      uri& operator=(const uri&);
      std::string get_uri() const;
      virtual ~uri();
      friend class odb::access;
    protected:
      // The default constructor is required by odb
      uri();
      std::string uri_;
      void set_uri(std::string);
    }; // end of class uri

    // This class represents the uri type for all uris that are used by the
    // scheduler to load the next page (described by the next waiting uri)
    class waiting_uri : public uri{
    public:
      waiting_uri(std::string);
      waiting_uri(const waiting_uri&);
      waiting_uri(waiting_uri&&); 
      waiting_uri& operator=(const waiting_uri&);
      waiting_uri& operator=(waiting_uri&&);
      virtual bool persist();
      static waiting_uri get_next();
      static bool has_next();
      virtual ~waiting_uri();
      friend class odb::access;
    protected:
      // The default constructor is required by odb
      waiting_uri();
    }; // end of class waiting_url

    // This class represents the uri type for all uri that marks pages which
    // were already downloaded by the scheduler
    class visited_uri : public uri {
    public:
      visited_uri(std::string);
      visited_uri(const visited_uri&);
      visited_uri(visited_uri&&);
      visited_uri& operator=(const visited_uri&);
      visited_uri& operator=(visited_uri&&);
      virtual bool persist();
      virtual ~visited_uri();
      friend class odb::access;
    protected:
      // The default constructor is required by odb
      visited_uri();
    }; // end of class visited_uri
  } // end of namespace data
} // end of namespace crawler_cpp


// Include the ODB pragmas for persisting the uri classes
#ifdef ODB_COMPILER
#  include "uri_odb_pragma.h"
#endif

#endif // URI_HPP
