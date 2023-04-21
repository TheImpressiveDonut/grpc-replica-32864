cmake_minimum_required(VERSION 3.15)

project(gRPC-replica-32864)

find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTO_FILES proto/helloworld.proto)
set(PROTO_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
add_library(helloworld ${PROTO_FILES})
target_link_libraries(helloworld protobuf::libprotobuf gRPC::grpc gRPC::grpc++)
target_include_directories(helloworld PUBLIC $<BUILD_INTERFACE:${PROTO_BINARY_DIR}>)

protobuf_generate(
    TARGET helloworld
    LANGUAGE cpp
    PROTOC_OUT_DIR ${PROTO_BINARY_DIR}
)
protobuf_generate(
    TARGET helloworld
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PROTOC_OUT_DIR ${PROTO_BINARY_DIR}
    PLUGIN protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
)

add_executable(server src/greeter_server.cpp)

target_link_libraries(server PRIVATE helloworld gRPC::grpc++ gRPC::grpc++_reflection absl::flags absl::flags_parse)