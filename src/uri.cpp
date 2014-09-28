// ============================================================================
// Author: Lukas Georgieff
// File: uri.cpp
// Description: This implementation file implements all the uri classes that
//              wraps all operations required for uri handling.
// Public interfaces:
//   * uri
//   * waiting_uri
//   * visited_uri
// ============================================================================


#ifndef URI_CPP
#define URI_CPP

#include "uri.h"
#include "exceptions.h"

// std::move
#include <utility>
#include <cassert>

using std::string;

// ============================================================================
// === the uri class ==========================================================
// ============================================================================
crawler_cpp::data::uri::uri() :uri_("") {}

crawler_cpp::data::uri::uri(string uri){
  this->set_uri(uri);
}

crawler_cpp::data::uri::uri(const crawler_cpp::data::uri& uri){
  // self-assignment is OK
  this->uri_ = uri.get_uri();
}

crawler_cpp::data::uri& crawler_cpp::data::uri::operator=(const crawler_cpp::data::uri& uri){
  // self-assignment is OK
  this->uri_ = uri.get_uri();
  return *this;
}

string crawler_cpp::data::uri::get_uri() const {
  return this->uri_;
}

void crawler_cpp::data::uri::set_uri(string uri){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}


template<typename T>
bool crawler_cpp::data::uri::is_known(const crawler_cpp::data::uri& uri){
  return crawler_cpp::data::uri::is_known<T>(uri.get_uri());
}

template<typename T>
bool crawler_cpp::data::uri::is_known(std::string uri){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

crawler_cpp::data::uri::~uri() {}

// ============================================================================
// === the waiting_uri class ==================================================
// ============================================================================
crawler_cpp::data::waiting_uri::waiting_uri() :crawler_cpp::data::uri() {}

crawler_cpp::data::waiting_uri::waiting_uri(string uri)
  :crawler_cpp::data::uri(uri) {}

crawler_cpp::data::waiting_uri::waiting_uri(const crawler_cpp::data::waiting_uri &uri)
  :crawler_cpp::data::uri(uri) {}

crawler_cpp::data::waiting_uri::waiting_uri(crawler_cpp::data::waiting_uri &&uri)
  :crawler_cpp::data::uri(std::move(uri.uri_)) {}

crawler_cpp::data::waiting_uri&
  crawler_cpp::data::waiting_uri::operator=(const crawler_cpp::data::waiting_uri &uri){
  // self-assignment is OK
  this->uri_ = uri.get_uri();
  return *this;
}

crawler_cpp::data::waiting_uri&
  crawler_cpp::data::waiting_uri::operator=(crawler_cpp::data::waiting_uri&& uri){
  // We can abort the execution, since this must be a serious error caused by
  // the user of this code when performing a self-assignment in the move
  // assignment operator
  assert(this != &uri);
  this->uri_ = std::move(uri.uri_);
  return *this;
}

bool crawler_cpp::data::waiting_uri::persist() {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

crawler_cpp::data::waiting_uri crawler_cpp::data::waiting_uri::get_next(){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

bool crawler_cpp::data::waiting_uri::has_next(){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

crawler_cpp::data::waiting_uri::~waiting_uri() {}

// ============================================================================
// === the visited_uri class ==================================================
// ============================================================================
crawler_cpp::data::visited_uri::visited_uri() :crawler_cpp::data::uri() {}

crawler_cpp::data::visited_uri::visited_uri(string uri)
  :crawler_cpp::data::uri(uri) {}

crawler_cpp::data::visited_uri::visited_uri(const crawler_cpp::data::visited_uri &uri)
  :crawler_cpp::data::visited_uri(uri.get_uri()) {}

crawler_cpp::data::visited_uri&
  crawler_cpp::data::visited_uri::operator=(const crawler_cpp::data::visited_uri& uri){
  // self-assignment is OK
  this->uri_ = uri.get_uri();
  return *this;
}

crawler_cpp::data::visited_uri&
  crawler_cpp::data::visited_uri::operator=(crawler_cpp::data::visited_uri&& uri){
  // We can abort the execution, since this must be a serious error caused by
  // the user of this code when performing a self-assignment in the move
  // assignment operator
  assert(this != &uri);
  this->uri_ = std::move(uri.uri_);
  return *this;
}

crawler_cpp::data::visited_uri::~visited_uri() {}

#include "uri.h"

#endif // URI_CPP
