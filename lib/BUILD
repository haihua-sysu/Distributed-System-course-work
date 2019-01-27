cc_library(
    name = "network-lib",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
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
