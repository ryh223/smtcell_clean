cmake_minimum_required(VERSION 3.10)

# -------------------------
# Project
# -------------------------
project(
    SMTCell
    VERSION 1.2
    LANGUAGES CXX)

# -------------------------
# External projects (third-party)
# -------------------------
include(ExternalProject)

# =========================
# OFFLINE thirdparty settings
# =========================
# Change this path if your tarballs live elsewhere
set(THIRDPARTY_DIR "/STCO_base/thirdparty" CACHE PATH "Local thirdparty directory containing pre-downloaded archives")

# Local tarball paths
set(BOOST_TARBALL "${THIRDPARTY_DIR}/boost_1_83_0.tar.bz2")
set(FMT_TARBALL   "${THIRDPARTY_DIR}/fmt-10.1.1.tar.gz")
set(JSON_TARBALL  "${THIRDPARTY_DIR}/json-3.11.3.tar.gz")

# Allow overriding BOOST_URL at configure time, otherwise default to local file://
if (NOT DEFINED BOOST_URL)
  set(BOOST_URL "file://${BOOST_TARBALL}")
endif()

# -------------------------
# Boost
# -------------------------
set(BOOST_VERSION 1.83.0)
set(BOOST_VERSION_US 1_83_0)
set(BOOST_INSTALL ${CMAKE_BINARY_DIR}/boost-prefix/src/boost)

ExternalProject_Add(
    boost
    URL ${BOOST_URL}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./bootstrap.sh --prefix=${BOOST_INSTALL}
    BUILD_COMMAND ./b2 --with-regex install link=static variant=release threading=multi runtime-link=static --prefix=${BOOST_INSTALL}
    INSTALL_COMMAND ""
    INSTALL_DIR ${BOOST_INSTALL}
)

set(BOOST_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/boost-prefix/src/boost/include/)
set(BOOST_LIBRARY_DIRS ${CMAKE_BINARY_DIR}/boost-prefix/src/boost/lib/)

# -------------------------
# fmt
# -------------------------
ExternalProject_Add(
    fmt
    URL file://${FMT_TARBALL}
    CMAKE_CACHE_ARGS
      "-DFMT_INSTALL:BOOL=ON"
      "-DFMT_DOC:BOOL=OFF"
      "-DFMT_TEST:BOOL=OFF"
      "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/fmt"
    BUILD_COMMAND make -j8
    INSTALL_COMMAND make install
)

set(FMT_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/fmt/include)
set(FMT_LIBRARY_DIRS ${CMAKE_BINARY_DIR}/fmt/lib)

# -------------------------
# nlohmann-json
# -------------------------
ExternalProject_Add(
    nlohmann-json
    URL file://${JSON_TARBALL}
    CMAKE_CACHE_ARGS
      "-DFMT_INSTALL:BOOL=ON"
      "-DFMT_DOC:BOOL=OFF"
      "-DFMT_TEST:BOOL=OFF"
      "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/nlohmann-json"
    BUILD_COMMAND make -j8
    INSTALL_COMMAND make install
)

# NOTE: CMake variable names cannot contain '-' reliably; use underscore.
set(NLOHMANN_JSON_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/nlohmann-json/include)

# =========================
# Sources
# =========================
set(SMTCELLLIB_SRC
    ./src/schematic.cpp
    ./src/parser.cpp
    ./src/dbConfig.cpp
    ./src/SMTCell.cpp
    ./src/SMTCellLib.cpp
    ./src/ruleParameter.cpp
)


# =========================
# Compiler flags
# =========================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# =========================
# Targets
# =========================

add_executable(SMTCellLib ${SMTCELLLIB_SRC})
add_dependencies(SMTCellLib boost fmt nlohmann-json)
target_include_directories(SMTCellLib PRIVATE
    ${BOOST_INCLUDE_DIRS}
    ${NLOHMANN_JSON_INCLUDE_DIRS}
    ${FMT_INCLUDE_DIRS}
)
target_link_libraries(SMTCellLib PRIVATE
    ${BOOST_LIBRARY_DIRS}/libboost_regex.a
    ${FMT_LIBRARY_DIRS}/libfmt.a
    stdc++fs
)