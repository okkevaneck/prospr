#!/usr/bin/env python3
"""
File:           build.py
Description:    This file compiles the Protein C++ files into a Python modules.
"""

from setuptools import setup, Extension
import os


path = os.path.dirname(__file__)

protein_module = Extension("_protein",
                           sources=[f"{path}/protein_wrap.cxx",
                                    f"{path}/protein.cpp"],
                           )

setup(name="protein",
      version="0.1",
      author="okkevaneck",
      description="C++ core compiled into a Python module.",
      ext_modules=[protein_module],
      py_modules=["protein"],
      license="LICENSE",
      )
