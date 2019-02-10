#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/message.h>

// Serialize the proto object to json format string
std::string messageToJsonString(const google::protobuf::Message &message);

// Serialize the proto object to bytes and return it as the string format.
std::string messageToString(const google::protobuf::Message &message);

#endif
