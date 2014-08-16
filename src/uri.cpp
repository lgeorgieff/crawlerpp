// ============================================================================
// Author: Lukas Georgieff
// File: uri.cpp
// Description: This implementation file implements all the uri class that
//              wraps all operations required for uri handling.
// Public interfaces:
//   *uri
// Modification 08/15/2014: Implementation of the class uri.
// ============================================================================


#include "uri.h"

#include <iostream>

// === constructors ===========================================================
crawler_cpp::data::uri::uri(const std::string &uri)
  :original_uri_(uri) {
  this->normalize_(uri);
}

crawler_cpp::data::uri::uri(const crawler_cpp::data::uri &uri)
  :original_uri_(uri.original_uri_), normalized_uri_(uri.normalized_uri_) { }

crawler_cpp::data::uri::~uri(){ }


// === getters ================================================================
std::string crawler_cpp::data::uri::get_scheme() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

std::string crawler_cpp::data::uri::get_host() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

std::string crawler_cpp::data::uri::get_port() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

std::string crawler_cpp::data::uri::get_path() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

bool crawler_cpp::data::uri::has_query() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

std::string crawler_cpp::data::uri::get_query() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

bool crawler_cpp::data::uri::has_fragment() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

std::string crawler_cpp::data::uri::get_fragment() const {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

std::string crawler_cpp::data::uri::get_original_uri() const {
  return this->original_uri_;
}

std::string crawler_cpp::data::uri::get_normalized_uri() const {
  return this->normalized_uri_;
}

// === operators ==============================================================
std::ostream& operator<<(std::ostream& os, const crawler_cpp::data::uri &uri){
  os << uri.get_normalized_uri();
  return os;
}

bool crawler_cpp::data::uri::operator==(const crawler_cpp::data::uri &uri)
  const {
  return this->get_normalized_uri() == uri.get_normalized_uri();
}

bool crawler_cpp::data::uri::operator==(const std::string &uri) const {
  return this->get_normalized_uri() == uri;
}

bool crawler_cpp::data::uri::operator!=(const crawler_cpp::data::uri &uri)
  const {
  return !(*this == uri);
}

bool crawler_cpp::data::uri::operator!=(const std::string &uri) const {
  return !(*this == uri);
}

crawler_cpp::data::uri& crawler_cpp::data::uri::operator=(
  const crawler_cpp::data::uri &uri){
  if(*this == uri) return *this;
  this->original_uri_ = uri.original_uri_;
  this->normalized_uri_ = uri.normalized_uri_;
  return *this;
}

// === private helpers ========================================================
std::string crawler_cpp::data::uri::normalize_(const std::string& uri){
  // TODO: implement
  // Note: throws an uri_exception
  throw crawler_cpp::exceptions::not_implemented_exception();
}
