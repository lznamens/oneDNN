/*******************************************************************************
 * Copyright 2021 Intel Corporation
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

#include <algorithm>

#include "compiler_graph.hpp"
#include "compiler_partition_impl.hpp"

namespace dnnl {
namespace graph {
namespace impl {
namespace compiler_impl {

size_t compiler_backend_t::get_mem_size(const logical_tensor_t &lt) const {
    assert(lt.layout_type == layout_type::strided);
    if (lt.ndims == 0) return 0;
    size_t mem_size = 1;
    for (int32_t i = 0; i < lt.ndims; ++i) {
        mem_size *= lt.dims[i];
    }
    return mem_size * logical_tensor_wrapper_t(lt).data_type_size();
}

bool compiler_backend_t::register_passes() {
    // TODO(yifei): passes support will be added in subsequent PR
    return true;
}

status_t compiler_backend_t::get_partitions(
        graph_t &agraph, partition_policy_t policy) {
    if (policy == partition_policy::fusion) {
        impl::pass::pass_manager_t pm(get_pass_registry());
        pm.run_passes(agraph, "", policy);
    }
    return status::success;
}

} // namespace compiler_impl

void register_compiler_backend() {
    backend_registry_t::get_singleton().register_backend(
            &compiler_impl::compiler_backend_t::get_singleton());
}

} // namespace impl
} // namespace graph
} // namespace dnnl
