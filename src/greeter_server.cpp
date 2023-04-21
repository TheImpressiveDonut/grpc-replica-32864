/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>

#include "proto/helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service
{
    Status SayHello(ServerContext *context, const HelloRequest *request,
                    HelloReply *reply) override
    {
        std::string prefix("Hello ");
        reply->set_message(prefix + request->name());
        return Status::OK;
    }
};

void RunServer(uint16_t port)
{
    const char key[] = R"(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA1nQUjCl7qJaR1nxQrukoh7A1QiDuN03vReaqlTs5EVMHBHrU
qH2zbK5JXO3QyGwnjiaA8M//M+K5oXLD/+MvWjw2HM/wkSF1Te1FuxNOXQaRu/c3
8/wcoSdocXcLRw0PVUA/JoXAzH3Ql4tmHlJvHiiR4gzR3MObHwuZtVU5fYeG9fUB
p7vo6bC/cmz5qkKJg0uRtZFS7T41Xfqo8APCHrFo/YVovspbKrm8g60Nrbcvt8NY
FDZuIDz9PVafITgG+iyW0V0dgZ1e8aFj03JD1QrXim2OSF5W132htCyJkiZOEppY
pdzEhV8Xgw+sX3d413o9Sa8PilqSKLUogS1NAQIDAQABAoIBAQCETo+SvdEl/Ocy
7fWVRBIWTLc++jQd+L4jYSFi+mvF7XSJ5gInhY3aAlkOtUY3MMrin4yzhY/MghfG
rJiR2acQQ8+azQt9bqgBkN6RgWK6b+AfmHFz3NoI7tg4EJrrlfocvYt4h9KgFwtH
Q+CX2oHlmGAQl5ouWpvsdmq5fj/+2inmIGrgz97FGtFNf4nQ0cPtdcs5Zm6qiHVb
vuL/UQ+oqia71Vk3jg6vgI6OWrv/Yzvgm3UW2cSvHAQrsZmemabPvbY/qh10TPvZ
USKas5UDxWgO32Wc+8BNdLnupLEo7c2Rh7S6FHdUFrVlzR150j+asGceg65BIv6G
p0oLHCnBAoGBAPhRR7RhdzBUJyV3KdIUqM/M/aq4gNey8Gu8uvPsbEMM6O376YhJ
0Cs3l4D4cyJ4yNlWsNDFzdn6BshPCpHBpkINCtteSZyZ9Jf3+pyBe3kTzFZDRklF
unsWb2z8ngb5TQJnIDI3cNMMxKLNxqgnJQY49HJ0aUVQMq+DGGmnQeP5AoGBAN0W
lzLdUDpam1vRNBPSVaN0otqAbOa62CsySDfb1g7FIroEYA6LVPqcJotATeEwirXO
2FtjqgRuo6TxTLVvMzgK3glK3n1MuCSLxOD9LAJ+rCkhKwcCnme8L7jj6TQISGP+
Ql/o4jGBBiHJwGg4Mno8gvUAS9Gntr6HNbPwimNJAoGAO8surgPONIf/CSmBm4G4
CJm+924IMGh5NigzCGuEwjyIlk3TtSq5Ar8XL0okR0e4bYVPfGY1JGjtWyyVSfXM
eBk15Q72QMxdtn5TDX6sYcJfsOHwOdKdjeggGqlV0C4V2h9FBHKPK3LmoqFUVcdf
RflO+4SGmNcv7U1vAnnxBtkCgYEAtKIlbJhnWRX4Qt89Jki+UWsVN2KgFy9nOHu/
h2vki3LDeanjdV6fUpZL44/pUWhgnERgfuKB4Vcn+rZvjvlIpqwyVvNFrqgE907+
zzfqQiuOq5f+2J5qH6UgdHY5bZqq0o/FWSkjzYB2Kxr0SfJIQ5i3Akd4qtXVN/uu
g9KurokCgYALAmvn2muYlwmeY6ceCNvoCfXGILvqUKyRwdhPmPJwmOCJE0ra/K0j
f5oYGZpmaZZQWHlmswu/ZXxf07+ws5BLmvL+2cp5MToZh3s1AfLC3Nf6eA47lgzT
OY75ik3+eSvfWF6l3YT9musufmKdKCfYDgF+93AVUKMNLrQl6uKGKQ==
-----END RSA PRIVATE KEY-----
)";

    const char cert[] = R"(
-----BEGIN CERTIFICATE-----
MIIDHzCCAgegAwIBAgIUNjvCVFVcd8l5kEptzFYMhhFEjo4wDQYJKoZIhvcNAQEL
BQAwFDESMBAGA1UEAwwJbG9jYWxob3N0MB4XDTIzMDQyMTExMjIwMloXDTI0MDQy
MDExMjIwMlowFDESMBAGA1UEAwwJbG9jYWxob3N0MIIBIjANBgkqhkiG9w0BAQEF
AAOCAQ8AMIIBCgKCAQEA1nQUjCl7qJaR1nxQrukoh7A1QiDuN03vReaqlTs5EVMH
BHrUqH2zbK5JXO3QyGwnjiaA8M//M+K5oXLD/+MvWjw2HM/wkSF1Te1FuxNOXQaR
u/c38/wcoSdocXcLRw0PVUA/JoXAzH3Ql4tmHlJvHiiR4gzR3MObHwuZtVU5fYeG
9fUBp7vo6bC/cmz5qkKJg0uRtZFS7T41Xfqo8APCHrFo/YVovspbKrm8g60Nrbcv
t8NYFDZuIDz9PVafITgG+iyW0V0dgZ1e8aFj03JD1QrXim2OSF5W132htCyJkiZO
EppYpdzEhV8Xgw+sX3d413o9Sa8PilqSKLUogS1NAQIDAQABo2kwZzAdBgNVHQ4E
FgQUZSjUlHX8RWgkgRENp3rubdBYC7UwHwYDVR0jBBgwFoAUZSjUlHX8RWgkgREN
p3rubdBYC7UwDwYDVR0TAQH/BAUwAwEB/zAUBgNVHREEDTALgglsb2NhbGhvc3Qw
DQYJKoZIhvcNAQELBQADggEBAI0FRbVn6hv7w95FjvlUcs50CjGJnR8Stozfl6Yf
8FTXMLX8r0zvsxNpfzidXIJPfOShdkZPs0jv1A5rVYAMUa6AAXEe4Ae7KmBeURQx
wFt1dp95TEGGSNh0DGLROLSez30B3eXwwko7U5dxD0YTUn1xZo0Rm6kMwGezuTEr
IFgbVqMQnzgM3fKW+f85si/0Z82q6tPrNHHC1R6czlmP8RddAtP7BTDFSyJM95dv
mtSsLhIBDAVxW3ajFPe4se3iO99fo+nmC+z4NGbXCNQ09Qhj95NdNhun2+UTt4Ke
usjF5AgJ7hO/9vEAXTGVprsKH0d+SIpuMMsjcX0VjhJa7Zo=
-----END CERTIFICATE-----
)";

    std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
    GreeterServiceImpl service;

    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;

    grpc::SslServerCredentialsOptions sslOps;
    sslOps.client_certificate_request = grpc_ssl_client_certificate_request_type::GRPC_SSL_DONT_REQUEST_CLIENT_CERTIFICATE;
    grpc::SslServerCredentialsOptions::PemKeyCertPair sslPair;
    sslPair.private_key = key;
    sslPair.cert_chain = cert;
    sslOps.pem_key_cert_pairs.emplace_back(sslPair);

    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::SslServerCredentials(sslOps));
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char **argv)
{
    absl::ParseCommandLine(argc, argv);
    RunServer(absl::GetFlag(FLAGS_port));
    return 0;
}