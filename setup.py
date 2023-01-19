#!/usr/bin/env python3
"""
File:           setup.py
Description:    This file contains the setup required for distributing the
                package on PyPi.org.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext


setup(
    ext_modules=[
        Pybind11Extension(
            name="prospr_core",
            sources=["prospr/core/core_module.cpp"],
            language="c++",
        ),
    ],
    cmdclass={"build_ext": build_ext},
)
