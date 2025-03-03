// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <cpu/x64/jit_generator.hpp>
#include "jit_emitter.hpp"
#include "jit_bf16_emitters.hpp"

namespace ov {
namespace intel_cpu {

class jit_convert_emitter : public jit_emitter {
public:
    jit_convert_emitter(dnnl::impl::cpu::x64::jit_generator *host, dnnl::impl::cpu::x64::cpu_isa_t host_isa,
                        const std::shared_ptr<ngraph::Node>& n, InferenceEngine::Precision exec_prc = InferenceEngine::Precision::FP32);

    size_t get_inputs_num() const override;

protected:
    void emit_data() const override;
    void validate_types() const;

    void float2bfloat(const std::vector<size_t> &in_vec_idxs, const std::vector<size_t> &out_vec_idxs) const;

    ov::element::Type input_type;
    ov::element::Type output_type;

    const ov::element::TypeVector supported_types = {
            ov::element::f32,
            ov::element::i32,
            ov::element::bf16,
            ov::element::i8,
            ov::element::u8
    };

    std::shared_ptr<jit_emu_vcvtneps2bf16> emu_vcvtneps2bf16 = nullptr;
};

// This emitter is covered by specification of "Convert" operation. The implementation uses a "warp-around" conversion.
// Example:
//  int32_t -> int8_t
//   129   -> -127
class jit_convert_truncation_emitter : public jit_convert_emitter {
public:
    jit_convert_truncation_emitter(dnnl::impl::cpu::x64::jit_generator *host, dnnl::impl::cpu::x64::cpu_isa_t host_isa,
                                   const std::shared_ptr<ngraph::Node>& n, InferenceEngine::Precision exec_prc = InferenceEngine::Precision::FP32);

private:
    void emit_impl(const std::vector<size_t>& in, const std::vector<size_t>& out,
                   const std::vector<size_t>& pool, const std::vector<size_t>& gpr,
                   const ov::intel_cpu::emitter_context *emit_context) const override;
    template <dnnl::impl::cpu::x64::cpu_isa_t isa>
    void emit_isa(const std::vector<size_t> &in_vec_idxs, const std::vector<size_t> &out_vec_idxs) const;

    template <dnnl::impl::cpu::x64::cpu_isa_t isa>
    void dword2int8(const std::vector<size_t> &in_vec_idxs, const std::vector<size_t> &out_vec_idxs) const;

    bool is_i8_and_u8_case() const;
    void register_table_entries() override;
};

// This emitter is covered by the common dnnl behavior. The implementation uses a "saturation" conversion.
// Example:
//  int32_t -> int8_t
//   129   -> 127
class jit_convert_saturation_emitter : public jit_convert_emitter {
public:
    jit_convert_saturation_emitter(dnnl::impl::cpu::x64::jit_generator *host, dnnl::impl::cpu::x64::cpu_isa_t host_isa,
                                   const std::shared_ptr<ngraph::Node>& n, InferenceEngine::Precision exec_prc = InferenceEngine::Precision::FP32);

private:
    void emit_impl(const std::vector<size_t>& in, const std::vector<size_t>& out,
                   const std::vector<size_t>& pool, const std::vector<size_t>& gpr,
                   const ov::intel_cpu::emitter_context *emit_context) const override;
    template <dnnl::impl::cpu::x64::cpu_isa_t isa>
    void emit_isa(const std::vector<size_t> &in_vec_idxs, const std::vector<size_t> &out_vec_idxs) const;

    template <dnnl::impl::cpu::x64::cpu_isa_t isa>
    void dword2int8(const std::vector<size_t> &in_vec_idxs, const std::vector<size_t> &out_vec_idxs, bool is_signed) const;

    size_t aux_vecs_count() const override;
};

}   // namespace intel_cpu
}   // namespace ov
