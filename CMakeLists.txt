cmake_minimum_required(VERSION 3.16)
project(prospr VERSION 0.11)

include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use -fPIC even if statically compiled
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find Python3
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Set list with names of the libraries to compile.
set(libs "protein" "depth_first" "depth_first_bnb")

# Set list with paths to binding files.
set(swig_bindings "prospr/core/protein/protein.i"
    "prospr/core/depth_first/depth_first.i"
    "prospr/core/depth_first_bnb/depth_first_bnb.i")


# =================
# Compile libraries
# =================
foreach (lib IN libs)
    add_library(${lib}
        prospr/core/${lib}/${lib}.hpp
        prospr/core/${lib}/${lib}.cpp)

    set_target_properties(protein PROPERTIES
        PUBLIC_HEADER prospr/core/${lib}/${lib}.hpp)

    target_include_directories(protein PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endforeach()


# =============
# SWIG bindings
# =============

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

find_package(SWIG 4.0 REQUIRED)
set(UseSWIG_MODULE_VERSION 2)
include(${SWIG_USE_FILE})

set(swig_name "bindings")
set_source_files_properties(${swig_bindings} PROPERTIES
    CPLUSPLUS ON
    SWIG_MODULE_NAME "bindings")

swig_add_library(${swig_name}
    TYPE SHARED
    LANGUAGE python
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/prospr
    OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES ${swig_bindings})

target_link_libraries(${swig_name}
    PRIVATE prospr Python3::Python)

set_target_properties(${swig_name} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/prospr)

set_property(TARGET ${swig_name} PROPERTY
    SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

set_property(TARGET ${swig_name}
    PROPERTY SWIG_COMPILE_OPTIONS -doxygen)

if(APPLE)
    set_target_properties(${swig_name} PROPERTIES SUFFIX ".so")
endif()


# =======
# Install
# =======

# Install the target with C++ code
install(
    TARGETS prospr
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Handle where to install the resulting Python package
if(CALL_FROM_SETUP_PY)
    # The CMakeExtension will set CMAKE_INSTALL_PREFIX to the root
    # of the resulting sdist / wheel archive
    set(PROSPR_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
else()
    # The Python package is installed directly in the folder of the
    # detected interpreter (system, user, or virtualenv)
    set(PROSPR_INSTALL_PREFIX ${Python3_SITELIB})
endif()

get_property(WRAPPER_PY_FILE
    TARGET ${swig_name}
    PROPERTY SWIG_SUPPORT_FILES)

# Install the .py file
install(
    FILES ${WRAPPER_PY_FILE}
    DESTINATION ${PROSPR_INSTALL_PREFIX})

# Install the SWIG library
install(
    TARGETS ${swig_name}
    LIBRARY DESTINATION ${PROSPR_INSTALL_PREFIX}
    ARCHIVE DESTINATION ${PROSPR_INSTALL_PREFIX}
    RUNTIME DESTINATION ${PROSPR_INSTALL_PREFIX})