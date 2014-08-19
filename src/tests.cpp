#include "headers/uri.h"

#include <iostream>

using std::cout;
using std::endl;

using crawler_cpp::data::uri;

int main(){
  uri uri_1("http://this.is/a#test%7eLukas%a2Georgieff?query_1=hello&query_2=world");
  cout << endl;
  uri uri_2("HTTP://123.95.10.23:443/a#test%7eLukas%a2Georgieff?query_1=hello&query_2=world");

  cout << "uri_1: " << uri_1 << endl;

  return 0;
}
