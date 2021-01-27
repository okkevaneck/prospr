cmake_minimum_required(VERSION 3.16)
project(prospr VERSION 0.11)

include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use -fPIC even if statically compiled
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find Python3
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Get source, header and binding files.
file(GLOB LIB_SOURCES prospr/core/**/*.cpp)
file(GLOB LIB_HEADERS prospr/core/**/*.hpp)
file(GLOB LIB_BINDINGS prospr/core/**/*.i)


# ==============
# Compile prospr
# ==============

add_library(prospr ${LIB_SOURCES} ${LIB_HEADERS})

# set_target_properties(${lib} PROPERTIES
#     PUBLIC_HEADER prospr/core/${lib}/${lib}.hpp)

target_include_directories(prospr PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


# =============
# SWIG bindings
# =============

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

find_package(SWIG 4.0 REQUIRED)
set(UseSWIG_MODULE_VERSION 2)
include(${SWIG_USE_FILE})

set(swig_name "bindings")
set_source_files_properties(${LIB_BINDINGS} PROPERTIES
    CPLUSPLUS ON
    SWIG_MODULE_NAME "bindings")

swig_add_library(${swig_name}
    TYPE SHARED
    LANGUAGE python
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/prospr
    OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES ${LIB_BINDINGS})

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