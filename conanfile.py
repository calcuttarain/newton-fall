from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout

class NewtonsFallConan(ConanFile):
    name = "newtons_fall"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("box2d/3.0.0")
        self.requires("sfml/2.6.2")
        self.requires("pybind11/2.13.6")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.variables["PROJECT_NAME"] = self.name
        tc.variables["CMAKE_CXX_STANDARD"] = "17"
        tc.variables["CMAKE_CXX_STANDARD_REQUIRED"] = "ON"

        # directoarele de ieșire
        tc.variables["CMAKE_RUNTIME_OUTPUT_DIRECTORY"] = f"{self.build_folder}/bin"
        tc.variables["CMAKE_LIBRARY_OUTPUT_DIRECTORY"] = f"{self.build_folder}/lib"
        tc.variables["CMAKE_ARCHIVE_OUTPUT_DIRECTORY"] = f"{self.build_folder}/lib"

        tc.variables["CUSTOM_DEBUG_OPTION"] = "ON"

        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
