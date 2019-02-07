cc_binary(
    name = "callback",
    srcs = ["src/callback_test.cc"],
    deps = ["//lib:network-lib"],
    linkopts = ["-pthread"],
)
cc_binary(
    name = "server1",
    srcs = ["src/server_1.cc"],
    deps = ["//lib:network-lib"],
    linkopts = ["-pthread"],
)

cc_binary(
    name = "server2",
    srcs = ["src/server_2.cc"],
    deps = ["//lib:network-lib"],
    linkopts = ["-pthread"],
)

cc_binary(
    name = "client",
    srcs = ["src/client.cc", "src/client.h"],
    deps = ["//lib:network-lib"],
    linkopts = ["-pthread"],
)

cc_binary(
    name = "test-proto",
    srcs = ["src/test_proto.cc"],
    deps = ["//lib:network_cc_proto"],
)
