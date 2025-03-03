# Copyright (C) 2018-2022 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#

set(TARGET_NAME "openvino_intel_gpu_kernels")

find_host_package(PythonInterp 3 QUIET)
if(NOT PYTHONINTERP_FOUND)
  message(WARNING "[clDNN] Project requires Python 3.x interpreter to build (with python loader). CMake could not detect it correctly.
      If you have installed this interpreter, please disregard this warning or specify PYTHON_EXECUTABLE in CMake command-line."
    )
endif()

file(GLOB_RECURSE LIBRARY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  )

file(GLOB_RECURSE KERNELS
    "${CMAKE_CURRENT_SOURCE_DIR}/core/cl_kernels/*.cl"
  )

# Path which points to root directory where code generated elements are created
# (specific to build configuration).
set(CODEGEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/codegen")
set(CODEGEN_CACHE_DIR  "${CODEGEN_DIR}/cache")

# Path which points to automatically included directory with code generated elements
# (to support "copy-if-different" optimization).
set(CODEGEN_INCDIR  "${CODEGEN_DIR}/include")


set(PRIM_DB "ks_primitive_db.inc")
set(PRIM_DB_BATCH_HEADERS "ks_primitive_db_batch_headers.inc")
set(CODEGEN_CACHE_SOURCES "${CODEGEN_INCDIR}/${PRIM_DB}"
                          "${CODEGEN_INCDIR}/${PRIM_DB_BATCH_HEADERS}")

set(CODEGEN_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/core/common/primitive_db_gen.py")
# Helping with some generators.
set_property(SOURCE ${CODEGEN_CACHE_SOURCES} PROPERTY GENERATED TRUE)

add_custom_command(OUTPUT "${CODEGEN_CACHE_DIR}/${PRIM_DB}"
  COMMAND "${CMAKE_COMMAND}" -E make_directory "${CODEGEN_CACHE_DIR}"
  COMMAND "${PYTHON_EXECUTABLE}" "${CODEGEN_SCRIPT}" -out_path "${CODEGEN_CACHE_DIR}"
                                                     -out_file_name_prim_db "${PRIM_DB}"
                                                     -out_file_name_batch_headers "${PRIM_DB_BATCH_HEADERS}"
                                                     -kernels "${CMAKE_CURRENT_SOURCE_DIR}/core/cl_kernels/"
  DEPENDS ${KERNELS} "${CODEGEN_SCRIPT}"
  COMMENT "Generating ${CODEGEN_CACHE_DIR}/${PRIM_DB} ..."
)

add_custom_command(OUTPUT "${CODEGEN_INCDIR}/${PRIM_DB}"
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CODEGEN_CACHE_DIR}/${PRIM_DB}" "${CODEGEN_INCDIR}/${PRIM_DB}"
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CODEGEN_CACHE_DIR}/${PRIM_DB_BATCH_HEADERS}" "${CODEGEN_INCDIR}/${PRIM_DB_BATCH_HEADERS}"
  DEPENDS "${CODEGEN_CACHE_DIR}/${PRIM_DB}" "${KERNELS}" "${CODEGEN_SCRIPT}"
  COMMENT "Updating file if the file changed (${PRIM_DB}) ..."
)

add_library(${TARGET_NAME} STATIC ${LIBRARY_SRC} ${CODEGEN_CACHE_SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${INCLUDE_DIR}>
                                                 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                                 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/>
                                                 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/common/>
                                                 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/actual_kernels/>
                                                 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/cache/>
                                                 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common/>
                                                 $<BUILD_INTERFACE:${CODEGEN_INCDIR}>)

target_compile_options(${TARGET_NAME} PRIVATE
  $<$<CONFIG:Release>:$<IF:$<CXX_COMPILER_ID:MSVC>,/Os,-Os>>)

if(COMMAND add_cpplint_target)
  add_cpplint_target(${TARGET_NAME}_cpplint FOR_TARGETS ${TARGET_NAME})
endif()

target_link_libraries(${TARGET_NAME} PUBLIC OpenCL rapidjson inference_engine_plugin_api)

set_target_properties(${TARGET_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE ${ENABLE_LTO})

if(WIN32)
  target_link_libraries(${TARGET_NAME} PRIVATE setupapi)
elseif((NOT ANDROID) AND (UNIX))
  target_link_libraries(${TARGET_NAME} PRIVATE pthread)
endif()

if(WIN32)
  set(TUNING_CACHE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>")
else((NOT ANDROID) AND (UNIX))
  set(TUNING_CACHE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/")
endif()

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/core/cache/cache.json ${TUNING_CACHE_PATH}/cache.json)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/core/cache/cache.json
        DESTINATION ${OV_CPACK_PLUGINSDIR}
        COMPONENT gpu)

ov_install_static_lib(${TARGET_NAME} gpu)
