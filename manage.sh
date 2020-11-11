#!/usr/bin/env bash
# File:       manage.sh
# Author:     Okke van Eck
#
# Description:		This bash file supports the commands:
#						- "Deploy"	for updating the live version.
#						- "Build"	for building the Python interfaces for the
#										C++ code.
#						- "Clean"	for removing the Python interfaces for the
#										C++ code as well as the prerequisite
# 										.cxx files. Also removes all compiled
#										Python caches.
# 					Add module names to the MODULES list for automatic
#					compilation.

set -e

COREDIR=prospr/core
CXX_FILES=$(find ${COREDIR}/ -type f -name "*.cxx")
SO_FILES=$(find ${COREDIR}/ -type f -name "*.so")
PY_FILES=$(find ${COREDIR}/ -type f -name "*.py" ! -name "__init__.py" \
    ! -name "setup.py")
PYCACHES=$(find prospr/ -type d -name "__pycache__")

case "$1" in
    "globals")
        echo -e "Core dir:\n${COREDIR}\n"
        echo -e "CXX files:\n${CXX_FILES}\n"
        echo -e "SO files:\n${SO_FILES}\n"
        echo -e "PY files:\n${PY_FILES}\n"
        echo -e "PyCaches:\n${PYCACHES}\n"
        ;;
    "build")
        echo "~ Creating all .so and .py interfaces.."
        SPATHS=$(find ${COREDIR}/ -type f -name "*setup.py")
        MPATHS=$(foreach PATH, ${SPATHS}, "$(dir ${PATH})")

        echo ${SPATHS}
        echo ${MPATHS}
        exit

        for MPATH in ${MPATHS}; do
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
