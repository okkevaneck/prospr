/* File:            core_module.cpp
 * Description:     Source file defining the pybind11 interfaces for exporting
 *                      the c++ core to a Python interface.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "src/protein.cpp"
#include <tuple>


PYBIND11_MODULE(prospr_core, m) {
    m.doc() = "Prospr core written in C++.";
    py::class_<Protein>(m, "Protein")
        .def(py::init<const std::string, int &>(), "Protein constructor",
                py::arg("sequence"), py::arg("dim")=2)
        .def_property_readonly("sequence", &Protein::get_sequence)
        .def_property_readonly("h_idxs", &Protein::get_h_idxs)
        /* Custom getter that parses vectors to tuples for Python printing. */
        .def_property_readonly("space", [](Protein &p) {
            std::map<std::vector<int>, std::vector<int>> space = p.get_space();
            std::map<std::tuple<auto>, std::vector<int>> retval = {};

            for (auto item : space) {
                // Key: item.first, value: item.second
                // TODO: Create tuples with length of the vector as the key.
                retval[std::make_tuple(item.first)] = item.second;
            }

            return retval;
        })
        .def_property_readonly("cur_len", &Protein::get_cur_len)
        .def_property_readonly("dim", &Protein::get_dim)
        .def_property_readonly("last_move", &Protein::get_last_move)
        .def_property_readonly("last_pos", &Protein::get_last_pos)
        .def_property_readonly("score", &Protein::get_score)
        .def_property_readonly("changes", &Protein::get_changes)

        .def("get_amino", &Protein::get_amino,
            "Get amino index and next direction from amino at given position",
            py::arg("position"))
        .def("is_hydro", &Protein::is_hydro,
            "Check if the amino is an H at given position", py::arg("index"))
        .def("reset", &Protein::reset, "Reset the whole protein")
        .def("reset_conformation", &Protein::reset_conformation,
            "Reset only the conformation of the protein, not the statistics")
        .def("is_valid", &Protein::is_valid, "Check if a given move is valid",
            py::arg("move"))
        .def("place_amino", &Protein::place_amino,
            "Place a protein in a given direction", py::arg("move"),
            py::arg("track")=true) // TODO: Segfault due to not knowing var space.
        .def("remove_amino", &Protein::remove_amino,
            "Remove the last placed amino")
        .def("change_score", &Protein::change_score,
            "Change the score attribute according to the given move and weight",
            py::arg("move"), py::arg("value"))
        .def("hash_fold", &Protein::hash_fold,
            "Process the current conformation into a sequence of moves")
        .def("set_hash", &Protein::set_hash,
            "Set the conformation to the given sequence of moves",
            py::arg("fold_hash"), py::arg("track")=false)
    ;
    // NOTE: Did not add space or last_move as properties.
}
