// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "openvino/core/except.hpp"
#include "intel_gpu/plugin/program.hpp"
#include "intel_gpu/plugin/common_utils.hpp"

#include "ngraph/op/broadcast.hpp"
#include "ngraph/op/constant.hpp"

#include "intel_gpu/primitives/broadcast.hpp"
#include "intel_gpu/primitives/reorder.hpp"
#include "intel_gpu/primitives/reshape.hpp"

namespace ov {
namespace intel_gpu {

static void CreateCommonBroadcastOp(Program& p, const std::shared_ptr<ngraph::Node>& op, const ngraph::AxisSet axis_mapping) {
    auto inputPrimitives = p.GetInputPrimitiveIDs(op);
    std::string layerName = layer_type_name_ID(op);

    auto inputShape = op->get_input_shape(0);
    auto outputShape = op->get_output_shape(0);
    auto inputRank = inputShape.size();
    auto outputRank = outputShape.size();

    auto inputPrimitive = inputPrimitives[0];

    if (inputRank != outputRank) {
        // Add reorder if changing number of dimensions requires changing format
        auto targetFormat = cldnn::format::get_default_format(outputRank);
        if (targetFormat.value != cldnn::format::get_default_format(inputRank).value) {
            auto reorderName = layerName + "_cldnn_in_reorder";
            auto targetDatatype = cldnn::element_type_to_data_type(op->get_input_element_type(0));
            auto reorderPrim = cldnn::reorder(reorderName,
                                              inputPrimitive,
                                              targetFormat,
                                              targetDatatype,
                                              std::vector<float>(),
                                              cldnn::reorder_mean_mode::subtract);
            p.add_primitive(*op, reorderPrim);

            inputPrimitive = reorderName;
        }

        auto reshapeName = layerName + "_cldnn_in_reshape";

        // Extend input dimensions with ones
        if (axis_mapping.empty()) {
            // If axis_mapping is not specified, then we prepend shape with neccesary count of 1-s
            inputShape.insert(inputShape.begin(), outputRank - inputRank, 1ul);
        } else {
            // If axis_mapping is specified, then ones are inserted according to it.
            ngraph::Shape tmp_shape;
            int prev_axis = -1;
            int next_axis = -1;
            size_t currentRank = 0;
            for (auto& axis : axis_mapping) {
                prev_axis = next_axis;
                next_axis = static_cast<int>(axis);

                int ones_count = std::max(next_axis - prev_axis - 1, 0);
                tmp_shape.insert(tmp_shape.begin() + currentRank, ones_count, 1ul);
                tmp_shape.push_back(outputShape[axis]);

                currentRank += ones_count + 1;
            }
            inputShape = tmp_shape;
        }

        auto targetShape = tensor_from_dims(inputShape);

        auto reshapePrim = cldnn::reshape(reshapeName, inputPrimitive, targetShape);
        p.add_primitive(*op, reshapePrim);

        inputPrimitive = reshapeName;
    }

    ov::op::BroadcastModeSpec mode = ov::op::BroadcastType::NONE;
    if (auto broadcast_v3 = std::dynamic_pointer_cast<ngraph::op::v3::Broadcast>(op)) {
        mode = broadcast_v3->get_broadcast_spec();
    } else if (auto broadcast_v1 = std::dynamic_pointer_cast<ngraph::op::v1::Broadcast>(op)) {
        switch (broadcast_v1->get_broadcast_spec().m_type) {
            case ov::op::AutoBroadcastType::NONE: mode = ov::op::BroadcastType::NONE; break;
            case ov::op::AutoBroadcastType::NUMPY: mode = ov::op::BroadcastType::NUMPY; break;
            case ov::op::AutoBroadcastType::PDPD: mode = ov::op::BroadcastType::PDPD; break;
            default:
                throw ov::Exception("[GPU] Can't match Broadcast v1 mode with v3 version");
        }
    } else {
        throw ov::Exception("[GPU] Can't cast Broadcast operation to any supported version");
    }

    auto broadcastPrim = cldnn::broadcast(layerName,
                                          inputPrimitive,
                                          outputShape,
                                          axis_mapping,
                                          mode);

    p.add_primitive(*op, broadcastPrim);
}

static void CreateBroadcastOp(Program& p, const std::shared_ptr<ngraph::op::v1::Broadcast>& op) {
    validate_inputs_count(op, {2, 3});
    if (op->get_broadcast_spec().m_type == ngraph::op::AutoBroadcastType::NONE && op->get_input_size() == 3) {
        auto axis_mapping_node = std::dynamic_pointer_cast<ngraph::op::v0::Constant>(op->get_input_node_shared_ptr(2));
        if (!axis_mapping_node)
            IE_THROW() << "Unsupported parameter nodes type in " << op->get_friendly_name() << " (" << op->get_type_name() << ")";

        auto axis_mapping = axis_mapping_node->get_axis_set_val();
        CreateCommonBroadcastOp(p, op, axis_mapping);
    } else {
        // TODO: check if axis_mapping is not needed in these cases and prepending input shape with ones works fine in all cases
        CreateCommonBroadcastOp(p, op, {});
    }
}

static void CreateBroadcastOp(Program& p, const std::shared_ptr<ngraph::op::v3::Broadcast>& op) {
    validate_inputs_count(op, {2, 3});
    ngraph::AxisSet axis_mapping;
    if (op->get_input_size() == 3) {
        auto axis_mapping_node = std::dynamic_pointer_cast<ngraph::op::v0::Constant>(op->get_input_node_shared_ptr(2));
        if (!axis_mapping_node)
            IE_THROW() << "Unsupported parameter nodes type in " << op->get_friendly_name() << " (" << op->get_type_name() << ")";

        axis_mapping = axis_mapping_node->get_axis_set_val();
    }
    CreateCommonBroadcastOp(p, op, axis_mapping);
}

REGISTER_FACTORY_IMPL(v1, Broadcast);
REGISTER_FACTORY_IMPL(v3, Broadcast);

}  // namespace intel_gpu
}  // namespace ov
