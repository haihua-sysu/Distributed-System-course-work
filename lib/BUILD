cc_library(
    name = "network-lib",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
    deps = ["//lib/third_party/protobuf_3_6_1:protobuf"],
)

cc_proto_library(
    name = "network_cc_proto",
    deps = [":network_proto"],
    visibility = ["//visibility:public"],
)

proto_library(
    name = "network_proto",
    srcs = ["network.proto"],
)
