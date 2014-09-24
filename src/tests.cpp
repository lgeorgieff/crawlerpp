#include "uri.h"

#include <network/uri.hpp>

#include <iostream>

using std::cout;
using std::endl;

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
