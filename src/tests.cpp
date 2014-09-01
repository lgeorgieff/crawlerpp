#include "headers/uri.h"

#include <iostream>

using std::cout;
using std::endl;

using crawler_cpp::data::uri;
using crawler_cpp::data::operator<<;

int main(){
  try{
    uri uri_1("http://this.is/a#test%7eLukas%a2Georgieff?query_1=hello&query_2=world");
    //  cout << "uri_1: " << uri_1 << endl;
    uri uri_2("HTTP://123.95.10.23:443/a#test%7eLukas%a2Georgieff?query_1=hello&query_2=world");
    //  cout << "uri_2: " << uri_2 << endl;
    uri uri_3("mailto:LUKAS.georgieff@hotmaiL.Com");
    cout << "uri_3: " << uri_3 << endl;
    uri uri_4("mailto:LUKAS.geor(any comment)gieff@hotm(another (nested comment) comment)aiL.Com");
    cout << "uri_4: " << uri_4 << endl;
    uri uri_5("mailto:LUKAS.geor(any comment)gieff@[123.226.11.23](An IPv4 address instead a hostname)");
    cout << "uri_5: " << uri_5 << endl;
    uri uri_6("mailto:LUKAS.geor(any comment)gieff@[::ffff:123.226.11.23](An IPv6 address instead a hostname)");
    cout << "uri_6: " << uri_6 << endl;
    uri uri_7("mailto:LUKAS.geor(any comment)gieff@[2001:0db8:0000:08d3:0000:8a2e:0070:7344](An IPv6 address instead a hostname)");
    cout << "uri_7: " << uri_7 << endl;
    uri uri_8("mailto:LUKAS.geor(any comment)gieff@[2001:db8::1428:57ab](An IPv6 address instead a hostname)");
    cout << "uri_8: " << uri_8 << endl;




  } catch(crawler_cpp::exceptions::uri_exception &err){
    cout << err << endl;
  }

  return 0;
}
