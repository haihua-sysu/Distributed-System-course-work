#include "lib/network.pb.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    NetworkRequest request;
    request.set_client_id(123);
    request.set_client_ip("127.0.0.1");
    request.set_type(NetworkRequest::CLOCK_SYNC);

    // serialize to string
    string *coding = new string();
    request.SerializeToString(coding);

    // parse from string
    NetworkRequest newRequest;
    newRequest.ParseFromString(*coding);./
    cout << newRequest.client_id() << endl;
    cout << newRequest.client_ip() << endl;
    cout << newRequest.type() << endl;
    return 0;
}
