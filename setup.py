#!/usr/bin/env python3
"""
File:           setup.py
Description:    This file contains the setup required for distributing the
                package on PyPi.org.
"""

from __future__ import print_function
import sys


try:
    from skbuild import setup
except ImportError:
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

with open("README.md", "r") as f:
    long_description = f.read()

setup(
    name="prospr",
    version="0.1a11",
    author="okkevaneck",
    description="A toolbox for protein folding with Python.",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/okkevaneck/prospr",
    license_file="LICENSE",
    license="LGPLv3",
    packages=["core"],
    package_dir={"": "prospr"},
    cmake_install_dir="prospr/core",
    platforms=["any"],
    python_requires=">=3.6",
    zip_safe=False,
    install_requires=[
        "matplotlib",
        "seaborn",
        "numpy",
        "pandas",
    ],
    classifiers=[
        "Programming Language :: Python :: 3",
        "Development Status :: 2 - Pre-Alpha",
        "License :: OSI Approved :: GNU Lesser General Public License v3 "
        "(LGPLv3)",
        "Operating System :: OS Independent",
        "Topic :: Education",
        "Topic :: Scientific/Engineering",
        "Topic :: Scientific/Engineering :: Bio-Informatics",
    ],
    keywords=["prospr protein structure prediction toolbox python c++ swig " + \
              "cmake extension heuristics pypi package"],
)





#
# import sys
# from pathlib import Path
# from setuptools import setup
# from cmake_build_extension import BuildExtension, CMakeExtension
#
#
# with open("README.md", "r") as f:
#     long_description = f.read()
#
# setup(
#     name="prospr",
#     version="0.1a11",
#     author="okkevaneck",
#     description="A toolbox for protein folding with Python.",
#     long_description=long_description,
#     long_description_content_type="text/markdown",
#     url="https://github.com/okkevaneck/prospr",
#     license_file="LICENSE",
#     license="LGPLv3",
#     platforms=["any"],
#     python_requires=">=3.6",
#     ext_modules=[
#         CMakeExtension(name="prospr",
#                        install_prefix="prospr",
#                        source_dir=str(Path(".").absolute()),
#                        cmake_configure_options=[
#                            f"-DPython3_ROOT_DIR={Path(sys.prefix)}",
#                            "-DCALL_FROM_SETUP_PY:BOOL=ON",
#                            "-DBUILD_SHARED_LIBS:BOOL=OFF",
#                        ]),
#     ],
#     cmdclass=dict(build_ext=BuildExtension),
#     zip_safe=False,
#     install_requires=[ # TODO: Add version pinning.
#         "matplotlib",
#         "seaborn",
#         "numpy",
#         "pandas"
#     ],
#     classifiers=[
#         "Programming Language :: Python :: 3",
#         "Development Status :: 2 - Pre-Alpha",
#         "License :: OSI Approved :: GNU Lesser General Public License v3 "
#         "(LGPLv3)",
#         "Operating System :: OS Independent",
#         "Topic :: Education",
#         "Topic :: Scientific/Engineering",
#         "Topic :: Scientific/Engineering :: Bio-Informatics",
#     ],
#     keywords=["prospr protein structure prediction toolbox python c++ swig " + \
#               "cmake extension heuristics pypi package"],
# )
