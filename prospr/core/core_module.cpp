/* File:            core_module.cpp
 * Description:     Source file defining the pybind11 interfaces for exporting
 *                      the c++ core to a Python interface.
 */

#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "src/protein.hpp"


PYBIND11_MODULE(prospr_core, m) {
    m.doc() = "Prospr core written in C++.";
    py::class_<Protein>(m, "Protein")
        .def(py::init<const std::string, int &>(), "Protein constructor",
                py::arg("sequence"), py::arg("dim")=2)
        .def_property_readonly("sequence", &Protein::get_sequence)
        .def_property_readonly("h_idxs", &Protein::get_h_idxs)
        .def_property_readonly("cur_len", &Protein::get_cur_len)
        .def_property_readonly("dim", &Protein::get_dim)
        .def_property_readonly("last_pos", &Protein::get_last_pos)
        .def_property_readonly("score", &Protein::get_score)
        .def_property_readonly("changes", &Protein::get_changes)

        .def("get_amino", &Protein::get_amino)
        .def("is_hydro", &Protein::is_hydro)
        .def("reset", &Protein::reset)
        .def("reset_conformation", &Protein::reset_conformation)
        .def("is_valid", &Protein::is_valid)
        .def("place_amino", &Protein::place_amino) // TODO: Check default args, maybe import extra pybind headers? Execution causes segfault.
        .def("remove_amino", &Protein::remove_amino)
        .def("change_score", &Protein::change_score)
        .def("hash_fold", &Protein::hash_fold)
        .def("set_hash", &Protein::set_hash)
    ;
    // NOTE: Did not add space or last_move as properties.
}
