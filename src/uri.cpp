// ====================================================================================================================
// Author: Lukas Georgieff
// File: uri.cpp
// Description: This implementation file implements all the uri classes that wraps all operations required for uri
//              handling.
// Public interfaces:
//   * uri
//   * waiting_uri
//   * visited_uri
// ====================================================================================================================


#include "uri.h"
#include "exceptions.h"

using std::string;

// ====================================================================================================================
// === the uri class ==================================================================================================
// ====================================================================================================================
crawler_cpp::data::uri::uri(string uri){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

crawler_cpp::data::uri::uri(const crawler_cpp::data::uri &uri) :crawler_cpp::data::uri(uri.to_string()) {}

string crawler_cpp::data::uri::to_string() const {
  return this->get_uri();
}

string crawler_cpp::data::uri::get_uri() const {
  return this->uri_;
}

void crawler_cpp::data::uri::set_uri(string uri){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

// ====================================================================================================================
// === the waiting_uri class ==========================================================================================
// ====================================================================================================================

crawler_cpp::data::waiting_uri::waiting_uri(string uri) :crawler_cpp::data::uri(uri){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

crawler_cpp::data::waiting_uri::waiting_uri(const crawler_cpp::data::waiting_uri &uri) :crawler_cpp::data::waiting_uri(uri.to_string()) {
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
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

// ====================================================================================================================
// === the visited_uri class ==========================================================================================
// ====================================================================================================================
crawler_cpp::data::visited_uri::visited_uri(string uri): crawler_cpp::data::uri(uri){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

crawler_cpp::data::visited_uri::visited_uri(const crawler_cpp::data::visited_uri &uri): crawler_cpp::data::visited_uri(uri.to_string()){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}

crawler_cpp::data::visited_uri::visited_uri(const crawler_cpp::data::waiting_uri &uri): crawler_cpp::data::visited_uri(uri.to_string()){
  // TODO: implement
  throw crawler_cpp::exceptions::not_implemented_exception();
}
