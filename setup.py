#!/usr/bin/env python3
"""
File:           setup.py
Description:    This file contains the setup required for distributing the
                package on PyPi.org.
License:        This file is licensed under the GNU LGPL V3 license by
                Okke van Eck (2020 - 2023). See the LICENSE file for the
                specifics.
"""

import os
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

module = Pybind11Extension(
    name="prospr_core",
    sources=[
        "prospr/core/core_module.cpp",
        "prospr/core/src/utils.cpp",
    ],
    language="c++",
    cxx_std=20,
)

# Check enable debugging
# (Console output and pausing in depth_first_bnb(...))
if os.getenv("PROSPR_DEBUG_STEPS", "false").lower() in ["1", "yes", "true", "on"]:
    module.define_macros = list(module.define_macros or [])
    module.define_macros.append(("PROSPR_DEBUG_STEPS", None))

setup(
    ext_modules=[module],
    cmdclass={"build_ext": build_ext},
)
