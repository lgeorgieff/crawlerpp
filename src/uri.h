// ====================================================================================================================
// Author: Lukas Georgieff
// File: uri.h
// Description: This header file defines the uri classes that wrap all operations required for uri handling.
// Public interfaces:
//   * uri
//   * waiting_uri
//   * visited_uri
// ====================================================================================================================


#ifndef URI_HPP
#define URI_HPP

#include <odb/core.hxx> 

#include <string>

namespace crawler_cpp {
  namespace data {

    // This class represents the base type for all uri classes
    #pragma db object
    class uri {
    public:
      uri(std::string);
      uri(const uri&);
      virtual bool persist() = 0;
      std::string to_string() const;
      friend class odb::access;
    protected:
      std::string uri_;
      std::string get_uri() const;
      void set_uri(std::string);
    }; // end of class uri

    // This class represents the uri type for all uris that are used by the scheduler to load the next page (described
    // by the next waiting uri)
    class waiting_uri : public uri{
    public:
      waiting_uri(std::string);
      waiting_uri(const waiting_uri&);
      virtual bool persist();
      static waiting_uri get_next();
      static bool has_next();
    }; // end of class waiting_url

    // This class represents the uri type for all uri that marks pages which were already downloaded by the scheduler
    class visited_uri : public uri {
    public:
      visited_uri(std::string);
      visited_uri(const visited_uri&);
      visited_uri(const waiting_uri&);
      virtual bool persist();
      static bool is_known(const uri&);
      static bool is_known(std::string);
    }; // end of class visited_uri
  } // end of namespace data
} // end of namespace crawler_cpp


#endif // URI_HPP
