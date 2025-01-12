#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>    // Add this for std::vector support
#include "RLGame.h"        // Changed from Game.h
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

    py::class_<RLGame>(m, "RLGame")    // Changed from Game
        .def(py::init<>())
        .def("loadConfig", &RLGame::loadConfig)
        .def("run", &RLGame::run)
        .def("step", &RLGame::step)
        .def("restart", &RLGame::restart)
        .def("isGameOver", &RLGame::isRLGameOver)
        .def("getLastFrame", [](RLGame& game) {    // Changed from Game
            return imageToNumpy(game.getLastFrame());
        });

    py::class_<LoadLevels>(m, "LoadLevels")
        .def_static("loadAllLevels", &LoadLevels::loadAllLevels);

    py::class_<GameConfig>(m, "GameConfig");
}
