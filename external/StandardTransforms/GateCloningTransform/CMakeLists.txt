cmake_minimum_required(VERSION 3.13.4)

project(OpenPhySynGateCloningTransform VERSION 1.0.0 LANGUAGES CXX)

set(TRANSFORM_NAME gate_clone)

set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS_INIT} -std=c++11 -fPIC")

find_package(TCL REQUIRED)


set(PSN_HOME "/usr/include/OpenPhySyn" CACHE FILEPATH "The path to OpenPhySyn includes")

IF(NOT EXISTS ${PSN_HOME})
message( FATAL_ERROR "Cannot find OpenPhySyn headers, set PSN_HOME option." )
endif()

set(OPEDNDB_HOME ${PSN_HOME}/external/OpenDB CACHE FILEPATH "The path to OpenDB includes")

option(OPENSTA_HOME "The path to OpenSTA" ${PSN_HOME}/external/OpenSTA)

IF(NOT EXISTS ${OPEDNDB_HOME})
message( FATAL_ERROR "${OPEDNDB_HOME} -- ${PSN_HOME} Cannot find OpenDB headers, set OPEDNDB_HOME option." )
endif()
IF(NOT EXISTS ${OPENSTA_HOME})
message( FATAL_ERROR "Cannot find OpenSTA headers, set OPENSTA_HOME option." )
endif()

set(PSN_LIB_PATH "" CACHE STRING "Set OpenPhySyn built library path" 0)

if(NOT PSN_LIB_PATH STREQUAL 0)
link_directories(${PSN_LIB_PATH})
endif()

set(OPENDB_LIB_PATH "" CACHE STRING "Set OpenDB built library path" 0)

if(NOT OPENDB_LIB_PATH STREQUAL "")
link_directories(${OPENDB_LIB_PATH})
link_directories(${OPENDB_LIB_PATH}/src)
link_directories(${OPENDB_LIB_PATH}/src/db)
link_directories(${OPENDB_LIB_PATH}/src/defin)
link_directories(${OPENDB_LIB_PATH}/src/defout)
link_directories(${OPENDB_LIB_PATH}/src/lefin)
link_directories(${OPENDB_LIB_PATH}/src/lefout)
link_directories(${OPENDB_LIB_PATH}/src/zlib)
link_directories(${OPENDB_LIB_PATH}/src/zutil)
link_directories(${OPENDB_LIB_PATH}/src/tm)
endif()

include_directories(
  src/
  ${PSN_HOME}/include/
  ${PSN_HOME}/external/spdlog/include
  ${OPEDNDB_HOME}/include
  ${OPEDNDB_HOME}/src
  ${OPEDNDB_HOME}/src/db
  ${OPENSTA_HOME}/..
)

add_library(${TRANSFORM_NAME} SHARED src/GateCloningTransform.cpp)
target_link_libraries(${TRANSFORM_NAME}
PUBLIC
opendb
defin
defout
# lefin
lefout
# lef
zlib
zutil
tm
${TCL_LIBRARY}
dl
OpenPhySyn
)
if (DEFINED "TRANSFORMS_BUILD_DIR")
set_target_properties(${TRANSFORM_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${TRANSFORMS_BUILD_DIR}"
)
endif()
target_compile_features(${TRANSFORM_NAME} PRIVATE cxx_range_for cxx_auto_type)
target_compile_options(${TRANSFORM_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)
set_property(TARGET ${TRANSFORM_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)


set(PSN_TRANSFORM_INSTALL_PATH "$ENV{HOME}/.OpenPhySyn/transforms" CACHE STRING "Default path for transforms installation")


install(
  TARGETS ${TRANSFORM_NAME}
  DESTINATION ${PSN_TRANSFORM_INSTALL_PATH}
)






