#!/usr/bin/env bash
# File:       manage.sh
# Description:      This bash file supports the commands:
#                       - "init"        for installing the development
#                                           requirements via pip.
#                       - "echo_debug"  for printing the used global variables.
#						- "build"	    for building the Python interfaces for
#										    the C++ code.
#						- "clean"	    for removing the Python interfaces for
#										    the C++ code, the prerequisite .cxx
#										    files, and all compiled Python
#										    caches.
#						- "deploy"	    for updating the live version on PyPI.

set -e

COREDIR=prospr/core
CXX_FILES=$(find ${COREDIR}/ -type f -name "*.cxx")
SO_FILES=$(find ${COREDIR}/ -type f -name "*.so")
PY_FILES=$(find ${COREDIR}/ -type f -name "*.py" ! -name "__init__.py" \
    ! -name "setup.py")
PYCACHES=$(find prospr/ -type d -name "__pycache__")

case "$1" in
    "init")
        pip install -r requirements.txt
    ;;
    "echo_debug")
        echo -e "Core dir:\n${COREDIR}\n"
        echo -e "CXX files:\n${CXX_FILES}\n"
        echo -e "SO files:\n${SO_FILES}\n"
        echo -e "PY files:\n${PY_FILES}\n"
        echo -e "PyCaches:\n${PYCACHES}\n"
        ;;
    "build")
        echo "~ Creating all .so and .py interfaces.."
        SETUP_PATHS=$(find ${COREDIR}/ -type f -name "*setup.py")

        declare -a MODULE_PATHS=()
        for SPATH in ${SETUP_PATHS}; do
            MODULE_PATHS+=( "$(dirname $SPATH)" );
        done

        for MPATH in "${MODULE_PATHS[@]}"; do
            MODULE=$(basename ${MPATH%/})
            echo "Building ${MODULE}.."
            echo -e "\tCompiling source with SWIG.."
            swig -python -c++ ${MPATH}/${MODULE}.i
            echo -e "\tConstructing module with setuptools.."
            python ${MPATH}/setup.py -q build_ext --build-lib=${MPATH}
        done

        echo "~ Done building!"
        ;;
    "clean")
        echo "~ Removing all built .cxx, .so and .py files.."
        rm -f ${CXX_FILES} ${SO_FILES} ${PY_FILES}
        echo -e "\n~ Removing all __pycache__ directories.."
        rm -rf ${PYCACHES}
        echo -e "\n~ Done cleaning!"
        ;;
    "deploy")
        @echo "~ Removing old files.."
        rm -rf build dist prospr.egg-info
        @echo -e "\n~ Creating new package.."
        python3 setup.py sdist bdist_wheel
        @echo -e "\n~ Uploading package.."
        twine upload dist/*
        @echo -e "\n~ Done deploying!"
        ;;
    *)
        echo "No command detected from first argument.."
        ;;
esac
