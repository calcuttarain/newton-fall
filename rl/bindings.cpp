#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "Game.h"        
#include "LoadLevels.h" 
#include <string>
#include <iostream>

namespace py = pybind11;

py::array_t<uint8_t> imageToNumpy(const sf::Image& image) {
    auto size = image.getSize();
    std::vector<size_t> shape = {size.y, size.x, 4};
    std::vector<size_t> strides = {size.x * 4, 4, 1};
    
    return py::array_t<uint8_t>(
        py::buffer_info(
            const_cast<uint8_t*>(image.getPixelsPtr()),  // data pointer
            sizeof(uint8_t),                             // size of one scalar
            py::format_descriptor<uint8_t>::format(),    // Python struct-style format descriptor
            3,                                          // Number of dimensions
            shape,                                      // Shape of the matrix
            strides                                     // Strides for each axis
        )
    );
}

PYBIND11_MODULE(bindmodule, m) {
    m.doc() = "Game bindings for Python";

    py::class_<Game>(m, "Game")
        .def(py::init([](bool headless) {
            try {
                return std::unique_ptr<Game>(new Game(headless));
            } catch (const std::exception& e) {
                throw std::runtime_error(std::string("Game constructor failed: ") + e.what());
            }
        }), py::arg("headless") = false)
        .def("loadConfig", &Game::loadConfig)
        .def("step", &Game::step)
        .def("isGameOver", &Game::isGameOver)
        .def("getLastFrame", [](Game& game) {
            return imageToNumpy(game.getLastFrame());
        });

    py::class_<LoadLevels>(m, "LoadLevels")
        .def_static("loadAllLevels", &LoadLevels::loadAllLevels);

    py::class_<GameConfig>(m, "GameConfig");
}
