// ============================================================================
// Author: Lukas Georgieff
// File: exceptions.cpp
// Description: This implementation file defines all exceptions used by the
//              crawlercpp library.
// Public interfaces:
//   *exception
//   *uri_exception
//   *db_exception
//   *not_implemented_exception
// ============================================================================


#include "exceptions.h"

#include <iostream>

// === class exception ========================================================
crawler_pp::exceptions::exception::exception(const std::string &message)
  :message_(message) {}

std::string crawler_pp::exceptions::exception::get_message() const {
  return this->message_;
}

const char* crawler_pp::exceptions::exception::what() const throw(){
  return this->get_message().c_str();
}

std::ostream& crawler_pp::exceptions::operator<<(std::ostream &os,
						  const crawler_pp::exceptions::exception &err){
  os << err.get_message();
  return os;
}

crawler_pp::exceptions::exception::~exception() throw() {}

// === class uri_exception ====================================================
crawler_pp::exceptions::uri_exception::uri_exception(const std::string &message,
						      const std::string &uri)
  :exception(message), uri_(uri) { }

std::string crawler_pp::exceptions::uri_exception::get_uri() const {
  return this->uri_;
}

std::ostream& crawler_pp::exceptions::operator<<(std::ostream &os,
						  const crawler_pp::exceptions::uri_exception &err){
  os << err.get_message() << " (bad uri: " << err.get_uri() << ")";
  return os;
}

crawler_pp::exceptions::uri_exception::~uri_exception() throw() {}

// === class db_exception =====================================================
crawler_pp::exceptions::db_exception::db_exception(const std::string &message)
  :exception(message) {}

crawler_pp::exceptions::db_exception::~db_exception() throw() {}

// === class not_implemented_exception ========================================
crawler_pp::exceptions::not_implemented_exception::not_implemented_exception(const std::string &message)
  :exception(message) {}

crawler_pp::exceptions::not_implemented_exception::not_implemented_exception()
  :not_implemented_exception("") {}

crawler_pp::exceptions::not_implemented_exception::~not_implemented_exception() throw() {}
