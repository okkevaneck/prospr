#!/usr/bin/env bash
# File:             manage.sh
# Description:      This bash file supports the commands:
#                       - "init"        for setting up a developing environment.
#                       - "echo_debug"  for printing the used global variables.
#						- "build"	    for building the Python interfaces for
#										    the core .cpp files.
#						- "clean"	    for removing all Python interfaces,
#	                    					.cxx files, and Python caches.
#                       - "test"        for testing all Python code with pytest.

set -e

COREDIR=prospr/core
CXX_FILES=$(find ${COREDIR}/ -type f -name "*.cxx")
SO_FILES=$(find prospr/ -type f -name "*.so")
PY_FILES=$(find ${COREDIR}/ -type f -name "*.py" ! -name "__init__.py" \
    ! -name "setup.py")
PYCACHES=$(find prospr/ -type d -name "__pycache__")

case "$1" in
    # Setup a developing environment.
    "init")
        pip install -r requirements.txt
        pre-commit install
    ;;
    # Echo all global variables.
    "echo_debug")
        echo -e "Core dir:\n${COREDIR}\n"
        echo -e "CXX files:\n${CXX_FILES}\n"
        echo -e "SO files:\n${SO_FILES}\n"
        echo -e "PY files:\n${PY_FILES}\n"
        echo -e "PyCaches:\n${PYCACHES}\n"
        ;;
    # Build all Python interfaces for the core .cpp files.
    "build")
        echo "~ Creating the .py interface for the core.."
        c++ -O3 -Wall -shared -std=c++11 -fPIC \
            $(python3 -m pybind11 --includes) "${COREDIR}/core_module.cpp" \
            -o "prospr"/prospr_core$(python3-config --extension-suffix)
        echo "~ Done building!"
        ;;
    # Remove all Python interfaces, .cxx files, and Python caches.
    "clean")
        echo "~ Removing all built .cxx, .so and .py files.."
        rm -f ${CXX_FILES} ${SO_FILES} ${PY_FILES}
        echo -e "\n~ Removing all __pycache__ directories.."
        rm -rf ${PYCACHES}
        echo -e "\n~ Done cleaning!"
        ;;
    # Build core, test all Python code, and then clean everything.
    "test")
        echo "~ Uninstalling old prospr.."
        pip uninstall -qy prospr
        echo "~ Installing new prospr.."
        pip install .
        echo "~ Running pytest.."
        pytest || true
        echo "~ Uninstalling old prospr.."
        pip uninstall -qy prospr
        echo "~ Done running tests!"
        ;;
    *)
        echo "No command detected from first argument.."
        ;;
esac
