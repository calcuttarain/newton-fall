from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout

class NewtonsFallConan(ConanFile):
    name = "newtons_fall"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def requirements(self):
        self.requires("box2d/3.0.0")
        self.requires("sfml/2.6.2")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.presets_prefix = "project"
        tc.variables["PROJECT_NAME"] = self.name
        tc.variables["CMAKE_RUNTIME_OUTPUT_DIRECTORY"] = f"{self.build_folder}/bin"
        tc.variables["CMAKE_LIBRARY_OUTPUT_DIRECTORY"] = f"{self.build_folder}/lib"
        tc.variables["CMAKE_ARCHIVE_OUTPUT_DIRECTORY"] = f"{self.build_folder}/lib"
        tc.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["newtons_fall"]
