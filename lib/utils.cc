#include "utils.h"

/** Returns a json format of the given message. */
std::string messageToJsonString(const google::protobuf::Message &message) {
    std::string output;
    google::protobuf::util::MessageToJsonString(message, &output);
    return output;
}

std::string messageToString(const google::protobuf::Message &message) {
    std::string data;
    message.SerializeToString(&data);
    return data;
}
