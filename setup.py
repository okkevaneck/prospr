#!/usr/bin/env python3
"""
File:           setup.py
Description:    This file contains the setup required for distributing the
                package on PyPi.org.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

import sys
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext


compile_args = []
link_args = []

if sys.platform == "darwin":
    # macOS: Requires libomp installed via brew install libomp
    compile_args.append("-Xpreprocessor -fopenmp")
    link_args.extend(["-lomp"])
elif sys.platform == "linux":
    compile_args.append("-fopenmp")
    link_args.append("-fopenmp")
elif sys.platform == "win32":
    compile_args.append("/openmp")

setup(
    ext_modules=[
        Pybind11Extension(
            name="prospr_core",
            sources=["prospr/core/core_module.cpp"],
            extra_compile_args=compile_args,
            extra_link_args=link_args,
            language="c++",
        ),
    ],
    cmdclass={"build_ext": build_ext},
)
