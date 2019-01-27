#include "lib/network.pb.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    NetworkRequest request;
    request.set_client_id(1);
    request.set_client_ip("127.0.0.1");
    request.set_type(NetworkRequest::CLOCK_SYNC);

    string *coding = new string();
    request.SerializeToString(coding);
    cout << (*coding).length() << endl;
    return 0;
}
