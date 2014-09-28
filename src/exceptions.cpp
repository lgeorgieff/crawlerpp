// ============================================================================
// Author: Lukas Georgieff
// File: exceptions.cpp
// Description: This implementation file defines all exceptions used by the
//              crawlercpp library.
// Public interfaces:
//   *exception
//   *uri_exception
//   *not_implemented_exception
// ============================================================================


#include "exceptions.h"

#include <iostream>

// === class exception ========================================================
crawler_cpp::exceptions::exception::exception(const std::string &message)
  :message_(message) {}

std::string crawler_cpp::exceptions::exception::get_message() const {
  return this->message_;
}

const char* crawler_cpp::exceptions::exception::what() const throw(){
  return this->get_message().c_str();
}

std::ostream& crawler_cpp::exceptions::operator<<(std::ostream &os,
						  const crawler_cpp::exceptions::exception &err){
  os << err.get_message();
  return os;
}


// === class uri_exception ====================================================
crawler_cpp::exceptions::uri_exception::uri_exception(const std::string &message,
						      const std::string &uri)
  :exception(message), uri_(uri) { }

std::string crawler_cpp::exceptions::uri_exception::get_uri() const {
  return this->uri_;
}

std::ostream& crawler_cpp::exceptions::operator<<(std::ostream &os,
						  const crawler_cpp::exceptions::uri_exception &err){
  os << err.get_message() << " (bad uri: " << err.get_uri() << ")";
  return os;
}

// === class not_implemented_exception ========================================
crawler_cpp::exceptions::not_implemented_exception::not_implemented_exception(const std::string &message)
  :exception(message) {}

crawler_cpp::exceptions::not_implemented_exception::not_implemented_exception()
  :not_implemented_exception("") {}

std::ostream& crawler_cpp::exceptions::operator<<(std::ostream &os,
						  const crawler_cpp::exceptions::not_implemented_exception &err){
  os << err.get_message();
  return os;
}
