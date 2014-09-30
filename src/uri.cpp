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
#include "utils.h"

// std::move
#include <utility>
#include <cassert>
#include <exception>
#include <network/uri.hpp>
#include <iostream>
#include <vector>

using std::string;
using std::vector;
using crawler_pp::exceptions::uri_exception;

// ============================================================================
// === the uri class ==========================================================
// ============================================================================
const size_t crawler_pp::data::uri::MAX_SIZE(2048);

const string crawler_pp::data::uri::SCHEME_HTTP("http");

const string crawler_pp::data::uri::SCHEME_HTTPS("https");

const vector<string> crawler_pp::data::uri::SUPPORTED_SCHEMES {
  crawler_pp::data::uri::SCHEME_HTTP,
  crawler_pp::data::uri::SCHEME_HTTPS
};

crawler_pp::data::uri::uri() :value_("") {}

crawler_pp::data::uri::uri(string uri){
  this->set_value(uri);
}

crawler_pp::data::uri::uri(const crawler_pp::data::uri& uri){
  // self-assignment is OK
  this->value_ = uri.get_value();
}

crawler_pp::data::uri::uri(crawler_pp::data::uri&& uri) {
  // We can abort the execution, since this must be a serious error caused by
  // the user of this code when performing a self-assignment in the move
  // assignment operator
  assert(this != &uri);
  this->value_ = std::move(uri.value_);
}

crawler_pp::data::uri& crawler_pp::data::uri::operator=(const crawler_pp::data::uri& uri){
  // self-assignment is OK
  this->value_ = uri.get_value();
  return *this;
}

crawler_pp::data::uri& crawler_pp::data::uri::operator=(crawler_pp::data::uri&& uri){
  // We can abort the execution, since this must be a serious error caused by
  // the user of this code when performing a self-assignment in the move
  // assignment operator
  assert(this != &uri);
  this->value_ = std::move(uri.value_);
  return *this;
}

bool crawler_pp::data::uri::operator==(const crawler_pp::data::uri &uri) const {
  return !this->compare(uri);
}

bool crawler_pp::data::uri::operator==(const std::string &uri) const {
  return !this->compare(uri);
}

bool crawler_pp::data::uri::operator!=(const crawler_pp::data::uri &uri) const {
  return !(*this == uri);
}

bool crawler_pp::data::uri::operator!=(const std::string &uri) const {
  return !(*this == uri);
}

bool crawler_pp::data::uri::operator<(const crawler_pp::data::uri &uri) const {
  return this->compare(uri) < 0;
}

bool crawler_pp::data::uri::operator<(const std::string &uri) const {
  return this->compare(uri) < 0;
}

bool crawler_pp::data::uri::operator>(const crawler_pp::data::uri &uri) const {
  return this->compare(uri) > 0;
}

bool crawler_pp::data::uri::operator>(const std::string &uri) const {
  return this->compare(uri) > 0;
}

bool crawler_pp::data::uri::operator<=(const crawler_pp::data::uri &uri) const {
  return this->compare(uri) <= 0;
}

bool crawler_pp::data::uri::operator<=(const std::string &uri) const {
  return this->compare(uri) <= 0;
}

bool crawler_pp::data::uri::operator>=(const crawler_pp::data::uri &uri) const {
  return this->compare(uri) >= 0;
}

bool crawler_pp::data::uri::operator>=(const std::string &uri) const {
  return this->compare(uri) >= 0;
}

size_t crawler_pp::data::uri::size() const {
  return this->get_value().size();
}

int crawler_pp::data::uri::compare(const crawler_pp::data::uri& uri) const {
  return this->get_value().compare(uri.get_value());
}

int crawler_pp::data::uri::compare(string uri) const {
  // TODO: implement
  // TODO: do an URI normalization on the passed string
  throw crawler_pp::exceptions::not_implemented_exception();
}

string crawler_pp::data::uri::get_value() const {
  return this->value_;
}

void crawler_pp::data::uri::set_value(string uri){
  try {
    network::uri tmp(network::uri(uri).normalize(network::uri_comparison_level::syntax_based));
    if(!tmp.is_absolute() || tmp.empty())
      throw uri_exception("Uri must be absolute!", uri);
    if(crawler_pp::utils::to_string<network::uri>(tmp).size() > crawler_pp::data::uri::MAX_SIZE)
      throw uri_exception("Uri must be shorter or equal to " + std::to_string(MAX_SIZE) +
			  " characters!", uri);
    if(std::find(SUPPORTED_SCHEMES.begin(), SUPPORTED_SCHEMES.end(), tmp.scheme().get()) ==
       SUPPORTED_SCHEMES.end())
      throw uri_exception("The scheme " + static_cast<string>(tmp.scheme().get()) +
			  " is not supported!", uri);

    // TODO: always set a port (default port, :, ...)
    // TODO: add asserts to test.cpp::main

    this->value_ = crawler_pp::utils::to_string<network::uri>(tmp);
  } catch(std::exception& e){
    throw uri_exception(e.what(), uri);
  } catch(...) {
    throw uri_exception("Could not create uri from string!", uri);
  }
}

template<typename T>
bool crawler_pp::data::uri::is_known(const crawler_pp::data::uri& uri){
  return crawler_pp::data::uri::is_known<T>(uri.get_value());
}

template<typename T>
bool crawler_pp::data::uri::is_known(std::string uri){
  // TODO: implement
  throw crawler_pp::exceptions::not_implemented_exception();
}

crawler_pp::data::uri::~uri() {}

std::ostream& crawler_pp::data::operator<<(std::ostream &os, const crawler_pp::data::uri &uri){
  os << uri.get_value();
  return os;
}

// ============================================================================
// === the waiting_uri class ==================================================
// ============================================================================
crawler_pp::data::waiting_uri::waiting_uri() :crawler_pp::data::uri() {}

crawler_pp::data::waiting_uri::waiting_uri(string uri)
  :crawler_pp::data::uri(uri) {}

crawler_pp::data::waiting_uri::waiting_uri(const crawler_pp::data::waiting_uri &uri)
  :crawler_pp::data::uri(uri) {}

crawler_pp::data::waiting_uri::waiting_uri(crawler_pp::data::waiting_uri &&uri)
  :crawler_pp::data::uri(std::move(uri.value_)) {}

crawler_pp::data::waiting_uri&
  crawler_pp::data::waiting_uri::operator=(const crawler_pp::data::waiting_uri &uri){
  // self-assignment is OK
  this->value_ = uri.get_value();
  return *this;
}

crawler_pp::data::waiting_uri&
  crawler_pp::data::waiting_uri::operator=(crawler_pp::data::waiting_uri&& uri){
  // We can abort the execution, since this must be a serious error caused by
  // the user of this code when performing a self-assignment in the move
  // assignment operator
  assert(this != &uri);
  this->value_ = std::move(uri.value_);
  return *this;
}

bool crawler_pp::data::waiting_uri::persist() {
  // TOOD: throw db_exception if an error occurs
  // TODO: implement
  throw crawler_pp::exceptions::not_implemented_exception();
}

crawler_pp::data::waiting_uri crawler_pp::data::waiting_uri::get_next(){
  // TODO: implement
  throw crawler_pp::exceptions::not_implemented_exception();
}

bool crawler_pp::data::waiting_uri::has_next(){
  // TODO: implement
  throw crawler_pp::exceptions::not_implemented_exception();
}

crawler_pp::data::waiting_uri::~waiting_uri() {}

// ============================================================================
// === the visited_uri class ==================================================
// ============================================================================
crawler_pp::data::visited_uri::visited_uri() :crawler_pp::data::uri() {}

crawler_pp::data::visited_uri::visited_uri(string uri)
  :crawler_pp::data::uri(uri) {}

crawler_pp::data::visited_uri::visited_uri(const crawler_pp::data::visited_uri &uri)
  :crawler_pp::data::visited_uri(uri.get_value()) {}

crawler_pp::data::visited_uri&
  crawler_pp::data::visited_uri::operator=(const crawler_pp::data::visited_uri& uri){
  // self-assignment is OK
  this->value_ = uri.get_value();
  return *this;
}

crawler_pp::data::visited_uri&
  crawler_pp::data::visited_uri::operator=(crawler_pp::data::visited_uri&& uri){
  // We can abort the execution, since this must be a serious error caused by
  // the user of this code when performing a self-assignment in the move
  // assignment operator
  assert(this != &uri);
  this->value_ = std::move(uri.value_);
  return *this;
}

bool crawler_pp::data::visited_uri::persist(){
  // TOOD: throw db_exception if an error occurs
  // TODO: implement
  throw crawler_pp::exceptions::not_implemented_exception();
}

crawler_pp::data::visited_uri::~visited_uri() {}

#include "uri.h"

#endif // URI_CPP
