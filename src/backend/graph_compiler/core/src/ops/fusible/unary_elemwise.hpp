/*******************************************************************************
 * Copyright 2020-2022 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#ifndef BACKEND_GRAPH_COMPILER_CORE_SRC_OPS_FUSIBLE_UNARY_ELEMWISE_HPP
#define BACKEND_GRAPH_COMPILER_CORE_SRC_OPS_FUSIBLE_UNARY_ELEMWISE_HPP

#include <utility>
#include <vector>
#include <compiler/ir/graph/fusible_op.hpp>

#define DECLARE_COMPUTE_ELEMENT() \
    expr compute_element(expr in, int mask_count, float mask_value) override;

namespace sc {
class relu_op_t : public unary_elementwise_op_t {
public:
    DECLARE_COMPUTE_ELEMENT();
    relu_op_t(const std::vector<graph_tensor_ptr> &ins,
            const std::vector<graph_tensor_ptr> &outs, const any_map_t &attrs)
        : unary_elementwise_op_t("relu", ins, outs, attrs) {
        set_brgemm_alg_kind(brgemm::eltwise_relu);
    }
    relu_op_t(const std::vector<graph_tensor_ptr> &ins, const any_map_t &attrs);
    relu_op_t(graph_tensor_ptr v)
        : unary_elementwise_op_t(std::move(v), "relu") {
        set_brgemm_alg_kind(brgemm::eltwise_relu);
    };
};

class round_op_t : public unary_elementwise_op_t {
public:
    DECLARE_COMPUTE_ELEMENT();

    round_op_t(const std::vector<graph_tensor_ptr> &ins,
            const std::vector<graph_tensor_ptr> &outs, const any_map_t &attrs)
        : unary_elementwise_op_t("round", ins, outs, attrs) {
        set_brgemm_alg_kind(brgemm::eltwise_round);
    }
    round_op_t(
            const std::vector<graph_tensor_ptr> &ins, const any_map_t &attrs);
    round_op_t(graph_tensor_ptr v)
        : unary_elementwise_op_t(std::move(v), "round") {
        set_brgemm_alg_kind(brgemm::eltwise_round);
    };
};

class sigmoid_op_t : public unary_elementwise_op_t {
public:
    DECLARE_COMPUTE_ELEMENT();

    sigmoid_op_t(const std::vector<graph_tensor_ptr> &ins,
            const std::vector<graph_tensor_ptr> &outs, const any_map_t &attrs)
        : unary_elementwise_op_t("sigmoid", ins, outs, attrs) {
        set_brgemm_alg_kind(brgemm::eltwise_logsigmoid);
    }
    sigmoid_op_t(graph_tensor_ptr v)
        : unary_elementwise_op_t(std::move(v), "sigmoid") {
        set_brgemm_alg_kind(brgemm::eltwise_logsigmoid);
    };
};

class exp_op_t : public unary_elementwise_op_t {
public:
    DECLARE_COMPUTE_ELEMENT();
    exp_op_t(const std::vector<graph_tensor_ptr> &ins,
            const std::vector<graph_tensor_ptr> &outs, const any_map_t &attrs)
        : unary_elementwise_op_t("exp", ins, outs, attrs) {
        set_brgemm_alg_kind(brgemm::eltwise_exp);
    }
    exp_op_t(graph_tensor_ptr v) : unary_elementwise_op_t(std::move(v), "exp") {
        set_brgemm_alg_kind(brgemm::eltwise_exp);
    };
};

class tanh_op_t : public unary_elementwise_op_t {
public:
    DECLARE_COMPUTE_ELEMENT();
    tanh_op_t(const std::vector<graph_tensor_ptr> &ins,
            const std::vector<graph_tensor_ptr> &outs, const any_map_t &attrs)
        : unary_elementwise_op_t("tanh", ins, outs, attrs) {
        set_brgemm_alg_kind(brgemm::eltwise_tanh);
    }
    tanh_op_t(graph_tensor_ptr v)
        : unary_elementwise_op_t(std::move(v), "tanh") {
        set_brgemm_alg_kind(brgemm::eltwise_tanh);
    };
};

class erf_op_t : public unary_elementwise_op_t {
public:
    DECLARE_COMPUTE_ELEMENT();
    erf_op_t(const std::vector<graph_tensor_ptr> &ins,
            const std::vector<graph_tensor_ptr> &outs, const any_map_t &attrs)
        : unary_elementwise_op_t("erf", ins, outs, attrs) {
        set_brgemm_alg_kind(brgemm::eltwise_gelu_erf);
    }
    erf_op_t(graph_tensor_ptr v) : unary_elementwise_op_t(std::move(v), "erf") {
        set_brgemm_alg_kind(brgemm::eltwise_gelu_erf);
    };
};

// squared_root: sqrt(x)
class squared_root_op_t : public unary_elementwise_op_t {
public:
    DECLARE_COMPUTE_ELEMENT();

    squared_root_op_t(const std::vector<graph_tensor_ptr> &ins,
            const std::vector<graph_tensor_ptr> &outs, const any_map_t &attrs)
        : unary_elementwise_op_t("squared_root", ins, outs, attrs) {
        set_brgemm_alg_kind(brgemm::eltwise_sqrt);
        reciprocal_ = attrs.get_or_else("reciprocal", false);
    };
    squared_root_op_t(graph_tensor_ptr v, bool reciprocal = false)
        : unary_elementwise_op_t(std::move(v), "squared_root")
        , reciprocal_(reciprocal) {
        set_brgemm_alg_kind(brgemm::eltwise_sqrt);
    }

private:
    // This flag decides return sqrt or rsqrt.
    bool reciprocal_;
};

class cast_op_t : public unary_elementwise_op_t {
public:
    DECLARE_COMPUTE_ELEMENT();

    cast_op_t(const std::vector<graph_tensor_ptr> &ins,
            const std::vector<graph_tensor_ptr> &outs, const any_map_t &attrs);
    cast_op_t(graph_tensor_ptr v, sc_data_type_t out_dtype,
            bool saturated = false);

private:
    sc_data_type_t dtype_;
    bool saturated_;
};

class clamp_op_t : public unary_elementwise_op_t {
public:
    DECLARE_COMPUTE_ELEMENT();

    clamp_op_t(const std::vector<graph_tensor_ptr> &ins,
            const std::vector<graph_tensor_ptr> &outs, const any_map_t &attrs)
        : unary_elementwise_op_t("clamp", ins, outs, attrs) {
        set_brgemm_alg_kind(brgemm::eltwise_clip);
    }
    clamp_op_t(graph_tensor_ptr v, float clamp_min = 0.0, float clamp_max = 1.0)
        : unary_elementwise_op_t(std::move(v), "clamp") {
        set_brgemm_alg_kind(brgemm::eltwise_clip);
        attrs_.set("clamp_min", clamp_min);
        attrs_.set("clamp_max", clamp_max);
    };
};

} // namespace sc
#endif