#!/usr/bin/env python3
"""
File:           setup.py
Author:         Okke van Eck
Description:    This file compiles the depth_first C++ files into a Python
                    modules.
"""

from setuptools import setup, Extension
import os

path = os.path.dirname(__file__)


depth_first_module = Extension("_depth_first",
                               sources=[f"{path}/../../Protein/Protein_wrap.cxx",
                                        f"{path}/../../Protein/Protein.cpp",
                                        f"{path}/depth_first_wrap.cxx",
                                        f"{path}/depth_first.cpp"],
                               )

setup(name="depth_first",
      version="0.1",
      author="okkevaneck",
      description="C++ core compiled into a Python module.",
      ext_modules=[depth_first_module],
      py_modules=["depth_first"],
      license="LICENSE",
      )
