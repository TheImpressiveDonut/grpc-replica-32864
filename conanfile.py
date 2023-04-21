from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain

class CppGrpcReplica(ConanFile):
    name = "grpc-replica-32864"
    version = "1.0.0"
    description = "This is a repo to replicate a bug in the c++ grpc library"
    homepage = "https://grpc.io/docs/languages/cpp/"
    url = "https://github.com/TheImpressiveDonut/grpc-replica-32864"
    topics = ("grpc", "bug", "cpp")
    settings = "os", "compiler", "build_type", "arch"
    requires = ("grpc/1.50.1")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()