// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

/**
 * @brief This is a C header file for the ov_compiled_model API, which is a C wrapper for ov::CompiledModel class.
 * A compiled model is compiled by a specific device by applying multiple optimization
 * transformations, then mapping to compute kernels.
 * @file ov_compiled_model.h
 */

#pragma once

#include "openvino/c/ov_common.h"
#include "openvino/c/ov_infer_request.h"
#include "openvino/c/ov_model.h"
#include "openvino/c/ov_node.h"
#include "openvino/c/ov_property.h"

typedef struct ov_compiled_model ov_compiled_model_t;

// Compiled Model
/**
 * @defgroup compiled_model compiled_model
 * @ingroup openvino_c
 * Set of functions representing of Compiled Model.
 * @{
 */

/**
 * @brief Get the input size of ov_compiled_model_t.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param input_size the compiled_model's input size.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e) ov_compiled_model_inputs_size(const ov_compiled_model_t* compiled_model, size_t* size);

/**
 * @brief Get a const input port of ov_compiled_model_t.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param input_port A pointer to the ov_output_const_port_t.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_input(const ov_compiled_model_t* compiled_model, ov_output_const_port_t** input_port);

/**
 * @brief Get a const input port of ov_compiled_model_t by port index.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param index input index.
 * @param input_port A pointer to the ov_output_const_port_t.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_input_by_index(const ov_compiled_model_t* compiled_model,
                                 const size_t index,
                                 ov_output_const_port_t** input_port);

/**
 * @brief Get a const input port of ov_compiled_model_t by name.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param name input name (char *).
 * @param input_port A pointer to the ov_output_const_port_t.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_input_by_name(const ov_compiled_model_t* compiled_model,
                                const char* name,
                                ov_output_const_port_t** input_port);

/**
 * @brief Get the output size of ov_compiled_model_t.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param input_size the compiled_model's input size.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_outputs_size(const ov_compiled_model_t* compiled_model, size_t* size);

/**
 * @brief Get a const output port of ov_compiled_model_t.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param output_port A pointer to the ov_output_const_port_t.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_output(const ov_compiled_model_t* compiled_model, ov_output_const_port_t** output_port);

/**
 * @brief Get a const output port of ov_compiled_model_t by port index.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param index input index.
 * @param output_port A pointer to the ov_output_const_port_t.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_output_by_index(const ov_compiled_model_t* compiled_model,
                                  const size_t index,
                                  ov_output_const_port_t** output_port);

/**
 * @brief Get a const output port of ov_compiled_model_t by name.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param name input name (char *).
 * @param output_port A pointer to the ov_output_const_port_t.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_output_by_name(const ov_compiled_model_t* compiled_model,
                                 const char* name,
                                 ov_output_const_port_t** output_port);

/**
 * @brief Gets runtime model information from a device.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param model A pointer to the ov_model_t.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_get_runtime_model(const ov_compiled_model_t* compiled_model, ov_model_t** model);

/**
 * @brief Creates an inference request object used to infer the compiled model.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param infer_request A pointer to the ov_infer_request_t.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_create_infer_request(const ov_compiled_model_t* compiled_model, ov_infer_request_t** infer_request);

/**
 * @brief Sets properties for the current compiled model.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param property ov_properties_t.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_set_property(const ov_compiled_model_t* compiled_model, const ov_properties_t* property);

/**
 * @brief Gets properties for current compiled model.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param property_name Property name.
 * @param property_value A pointer to property value.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_get_property(const ov_compiled_model_t* compiled_model, const char* key, ov_any_t* value);

/**
 * @brief Exports the current compiled model to an output stream `std::ostream`.
 * The exported model can also be imported via the ov::Core::import_model method.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t.
 * @param export_model_path Path to the file.
 * @return Status code of the operation: OK(0) for success.
 */
OPENVINO_C_API(ov_status_e)
ov_compiled_model_export_model(const ov_compiled_model_t* compiled_model, const char* export_model_path);

/**
 * @brief Release the memory allocated by ov_compiled_model_t.
 * @ingroup compiled_model
 * @param compiled_model A pointer to the ov_compiled_model_t to free memory.
 */
OPENVINO_C_API(void) ov_compiled_model_free(ov_compiled_model_t* compiled_model);

/** @} */  // end of compiled_model
