# grpc-replica-32864

This is a repo to replicate a bug in the c++ grpc library, [Github issue](https://github.com/grpc/grpc/issues/32864)

## Content


## Usage

### Cert generation

Cert are already generated and put in plaintext

```
openssl genrsa -out ssl/server.key 2048
openssl req -new -x509 -sha256 -key ssl/server.key -out ssl/server.crt -days 365 -subj "/CN=localhost" -addext "subjectAltName = DNS:localhost"
```

### Build

pr_17... or pr_15... both crashes

```
conan install . -if build -pr:b="./conan_profiles/pr_15_v141_x86_64_release" -pr:h"./conan_profiles/pr_15_v141_x86_64_release"
cmake -B build -G "Visual Studio 15 2017" -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake"
cmake --build build --config release
```

### Bug replication

In cmd #1:
```
.\build\Debug\server.exe
```

In cmd #2: (could probably do client in cpp that simulate this)
```
./ghz --cacert ssl/server.crt -n 1000 --call helloworld.Greeter.SayHello localhost:50051
```

## Status

All different configurations tested (works: &#x2611; | crash: &#x2612; | not-done: ?) | @own/build meaning grpc package build locally

| grpc version | compiler version | compiler toolset | abseil version | config | status |
| --- | --- | --- | --- | --- | --- |
| 1.48.0 | 15 | v141 | abseil/20220623.0 | Debug | &#x2611; |
| 1.48.0 | 15 | v141 | abseil/20220623.0 | Release | &#x2612; |
| 1.47.0 | 15 | v141 | abseil/20211102.0 | Debug | &#x2611; |
| 1.47.0 | 15 | v141 | abseil/20211102.0 | Release | &#x2612; |
| 1.48.0 | 17 | v140 | abseil/20220623.0 | Debug | &#x2611; |
| 1.48.0 | 17 | v140 | abseil/20220623.0 | Release | ? |
| 1.48.0@own/build | 17 | v140 | abseil/20211102.0 | Release | &#x2611; |
| 1.48.0 | 17 | v141 | abseil/20220623.0 | Debug | &#x2611; |
| 1.48.0 | 17 | v141 | abseil/20220623.0 | Release | &#x2612; |
| 1.48.0@own/build | 17 | v141 | abseil/20220623.0 | Release | &#x2612; |
| 1.48.0@own/build | 17 | v142 | abseil/20211102.0 | Debug | &#x2611; |
| 1.48.0@own/build | 17 | v142 | abseil/20211102.0 | Release | &#x2611; |
| 1.48.0@own/build | 17 | v142 | abseil/20211102.0 | Debug | ? |
| 1.48.0@own/build | 17 | v142 | abseil/20211102.0 | Release | &#x2611; |
| 1.52.1@own/build | 17 | v140 | abseil/20220623.0 | Release | not buildable |
| 1.52.1@own/build | 17 | v141 | abseil/20220623.0 | Release | &#x2612; |
| 1.52.1@own/build | 17 | v142 | abseil/20220623.0 | Release | &#x2611; |