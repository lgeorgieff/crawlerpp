// ============================================================================
// Author: Lukas Georgieff
// File: uri.cpp
// Description: This file contains several funcitons for testing the library.
// Public interfaces:
//   * int main()
// ============================================================================

#include "odb/uri.odb.h"
#include "uri.h"

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(){
  cout << "entering tests.main" << endl;
  cout << "===============================================================================" << endl;

  crawler_pp::data::waiting_uri uri_0("http://www.sueddeutsche.de");
  cout << "uri_0: _" << uri_0 << "_" << endl;
  crawler_pp::data::waiting_uri uri_1("http://www.sueddEutsche.de/any//path#fragment");
  cout << "uri_1: _" << uri_1 << "_" << endl;
  crawler_pp::data::waiting_uri uri_2("http://www.sueddeutsche.de//any/path#fragment#invalid");
  cout << "uri_2: _" << uri_2 << "_" << endl;
  crawler_pp::data::waiting_uri uri_3("");
  cout << "uri_3: _" << uri_3 << "_" << endl;

  cout << "===============================================================================" << endl;
  cout << "leaving tests.main" << endl;

  return 0;
}
