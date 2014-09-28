// ============================================================================
// Author: Lukas Georgieff
// File: uri.cpp
// Description: This file contains several funcitons for testing the library.
// Public interfaces:
//   * int main()
// ============================================================================

#include "odb/uri.odb.h"

#include <network/uri.hpp>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(){
  cout << endl;
  cout << "entering tests.main" << endl;
  
  network::uri instance("http://cpp-netlib.github.com/");
  cout << "scheme: " << instance.scheme().get() << endl
       << "host: " << instance.host().get() << endl
       << "path: " << instance.path().get() << endl;

  cout << "leaving tests.main" << endl << endl;

  return 0;
}
