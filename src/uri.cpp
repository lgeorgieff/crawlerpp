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
const size_t crawler_cpp::data::uri::MAX_SIZE = 2048;

crawler_cpp::data::uri::uri() :value_("") {}

crawler_cpp::data::uri::uri(string uri){
  this->set_value(uri);
}

crawler_cpp::data::uri::uri(const crawler_cpp::data::uri& uri){
  // self-assignment is OK
  this->value_ = uri.get_value();
}

crawler_cpp::data::uri::uri(crawler_cpp::data::uri&& uri) {
  // We can abort the execution, since this must be a serious error caused by
  // the user of this code when performing a self-assignment in the move
  // assignment operator
  assert(this != &uri);
  this->value_ = std::move(uri.value_);
}

crawler_cpp::data::uri& crawler_cpp::data::uri::operator=(const crawler_cpp::data::uri& uri){
  // self-assignment is OK
  this->value_ = uri.get_value();
  return *this;
}

crawler_cpp::data::uri& crawler_cpp::data::uri::operator=(crawler_cpp::data::uri&& uri){
  // We can abort the execution, since this must be a serious error caused by
  // the user of this code when performing a self-assignment in the move
  // assignment operator
  assert(this != &uri);
  this->value_ = std::move(uri.value_);
  return *this;
}

bool crawler_cpp::data::uri::operator==(const crawler_cpp::data::uri &uri) const {
  return !this->compare(uri);
}

bool crawler_cpp::data::uri::operator==(const std::string &uri) const {
  return !this->compare(uri);
}

bool crawler_cpp::data::uri::operator!=(const crawler_cpp::data::uri &uri) const {
  return !(*this == uri);
}

bool crawler_cpp::data::uri::operator!=(const std::string &uri) const {
  return !(*this == uri);
}

bool crawler_cpp::data::uri::operator<(const crawler_cpp::data::uri &uri) const {
  return this->compare(uri) < 0;
}

bool crawler_cpp::data::uri::operator<(const std::string &uri) const {
  return this->compare(uri) < 0;
}

bool crawler_cpp::data::uri::operator>(const crawler_cpp::data::uri &uri) const {
  return this->compare(uri) > 0;
}

bool crawler_cpp::data::uri::operator>(const std::string &uri) const {
  return this->compare(uri) > 0;
}

bool crawler_cpp::data::uri::operator<=(const crawler_cpp::data::uri &uri) const {
  return this->compare(uri) <= 0;
}

bool crawler_cpp::data::uri::operator<=(const std::string &uri) const {
  return this->compare(uri) <= 0;
}

bool crawler_cpp::data::uri::operator>=(const crawler_cpp::data::uri &uri) const {
  return this->compare(uri) >= 0;
}

bool crawler_cpp::data::uri::operator>=(const std::string &uri) const {
  return this->compare(uri) >= 0;
}

size_t crawler_cpp::data::uri::size() const {
  return this->get_value().size();
}

int crawler_cpp::data::uri::compare(const crawler_cpp::data::uri& uri) const {
  return this->get_value().compare(uri.get_value());
}

int crawler_cpp::data::uri::compare(string uri) const {
  // TODO: implement
  // TODO: do an URI normalization on the passed string
  throw crawler_cpp::exceptions::not_implemented_exception();
}

string crawler_cpp::data::uri::get_value() const {
  return this->value_;
}

void crawler_cpp::data::uri::set_value(string uri){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

template<typename T>
bool crawler_cpp::data::uri::is_known(const crawler_cpp::data::uri& uri){
  return crawler_cpp::data::uri::is_known<T>(uri.get_value());
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
  :crawler_cpp::data::uri(std::move(uri.value_)) {}

crawler_cpp::data::waiting_uri&
  crawler_cpp::data::waiting_uri::operator=(const crawler_cpp::data::waiting_uri &uri){
  // self-assignment is OK
  this->value_ = uri.get_value();
  return *this;
}

crawler_cpp::data::waiting_uri&
  crawler_cpp::data::waiting_uri::operator=(crawler_cpp::data::waiting_uri&& uri){
  // We can abort the execution, since this must be a serious error caused by
  // the user of this code when performing a self-assignment in the move
  // assignment operator
  assert(this != &uri);
  this->value_ = std::move(uri.value_);
  return *this;
}

bool crawler_cpp::data::waiting_uri::persist() {
  // TOOD: throw db_exception if an error occurs
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
  :crawler_cpp::data::visited_uri(uri.get_value()) {}

crawler_cpp::data::visited_uri&
  crawler_cpp::data::visited_uri::operator=(const crawler_cpp::data::visited_uri& uri){
  // self-assignment is OK
  this->value_ = uri.get_value();
  return *this;
}

crawler_cpp::data::visited_uri&
  crawler_cpp::data::visited_uri::operator=(crawler_cpp::data::visited_uri&& uri){
  // We can abort the execution, since this must be a serious error caused by
  // the user of this code when performing a self-assignment in the move
  // assignment operator
  assert(this != &uri);
  this->value_ = std::move(uri.value_);
  return *this;
}

bool crawler_cpp::data::visited_uri::persist(){
  // TOOD: throw db_exception if an error occurs
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

crawler_cpp::data::visited_uri::~visited_uri() {}

#include "uri.h"

#endif // URI_CPP
