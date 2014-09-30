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

#include "exceptions.h" // TOOD: remove

using std::cout;
using std::endl;
using std::string;

int main(){
  cout << "entering tests.main" << endl;
  cout << "===============================================================================" << endl;

  {
    crawler_pp::data::waiting_uri uri("http://www.sueddeutsche.de");
    cout << "0: _" << uri << "_" << endl;
  }
  {
    crawler_pp::data::waiting_uri uri("http://www.sueddEutsche.de/any/../pAth#fragment?");
    cout << "1: _" << uri << "_" << endl;
  }
  try{
    crawler_pp::data::waiting_uri uri("http://www.sueddeutsche.de/../any/path#fragment#invalid");
    cout << "2: _" << uri << "_" << endl;
    assert(false);
  } catch(crawler_pp::exceptions::uri_exception &err) {
    cout << "2: _" << "invalid: " << err.get_message() << "_" << endl;
  }
  try{
    crawler_pp::data::waiting_uri uri("");
    cout << "3: _" << uri << "_" << endl;
    assert(false);
  }catch(crawler_pp::exceptions::uri_exception &err){
    cout << "3: _" << "invalid: " << err.get_message() << "_" << endl;
  }
  try{
    crawler_pp::data::waiting_uri uri("ftp://www.sueddEutsche.de/any/../pAth#fragment?");
    cout << "4: _" << uri << "_" << endl;
  }catch(crawler_pp::exceptions::uri_exception &err){
    cout << "4: _" << "invalid: " << err.get_message() << "_" << endl;
  }
  {
    crawler_pp::data::waiting_uri uri("https://www.sueddEutsche.de/any/../pAth#fragment?");
    cout << "5: _" << uri << "_" << endl;
  }
  {
    crawler_pp::data::waiting_uri uri("http://www.sueddEutsche.de:443/any/../pAth#fragment?");
    cout << "6: _" << uri << "_" << endl;
  }
  {
    crawler_pp::data::waiting_uri uri("http://www.sueddEutsche.de:80/any/../pAth#fragment?");
    cout << "7: _" << uri << "_" << endl;
  }
  {
    crawler_pp::data::waiting_uri uri("http://www.sueddEutsche.de:/any/../pAth#fragment?");
    cout << "8: _" << uri << "_" << endl;
  }

  
  cout << "===============================================================================" << endl;
  cout << "leaving tests.main" << endl;

  return 0;
}
