// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <string>
#include <vector>

#include "shared_test_classes/base/layer_test_utils.hpp"
#include "ngraph/function.hpp"
#include "ngraph_functions/subgraph_builders.hpp"
#include "functional_test_utils/plugin_cache.hpp"
#include "common_test_utils/unicode_utils.hpp"
#include "openvino/util/common_util.hpp"
#include "base/behavior_test_utils.hpp"

#include <ie_core.hpp>
#include <ie_common.h>

using ngraphFunctionGenerator = std::function<std::shared_ptr<ngraph::Function>(ngraph::element::Type, std::size_t)>;
using nGraphFunctionWithName = std::tuple<ngraphFunctionGenerator, std::string>;

using loadNetworkCacheParams = std::tuple<
        nGraphFunctionWithName, // ngraph function with friendly name
        ngraph::element::Type,  // precision
        std::size_t,            // batch size
        std::string            // device name
        >;

namespace LayerTestsDefinitions {

class LoadNetworkCacheTestBase : public testing::WithParamInterface<loadNetworkCacheParams>,
                                 virtual public BehaviorTestsUtils::IEPluginTestBase,
                                 virtual public LayerTestsUtils::LayerTestsCommon {
    std::string           m_cacheFolderName;
    std::string           m_functionName;
    ngraph::element::Type m_precision;
    size_t                m_batchSize;
public:
    static std::string getTestCaseName(testing::TestParamInfo<loadNetworkCacheParams> obj);
    void SetUp() override;
    void TearDown() override;
    void Run() override;

    bool importExportSupported(InferenceEngine::Core& ie) const;

    // Default functions and precisions that can be used as test parameters
    static std::vector<nGraphFunctionWithName> getStandardFunctions();
};

using compileKernelsCacheParams = std::tuple<
        std::string,            // device name
        std::pair<std::map<std::string, std::string>, std::string>   // device and cache configuration
>;
class LoadNetworkCompiledKernelsCacheTest : virtual public LayerTestsUtils::LayerTestsCommon,
                                            virtual public BehaviorTestsUtils::IEPluginTestBase,
                                            public testing::WithParamInterface<compileKernelsCacheParams> {
public:
    static std::string getTestCaseName(testing::TestParamInfo<compileKernelsCacheParams> obj);
protected:
    std::string test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    std::string cache_path;
    std::vector<std::string> m_extList;

    void SetUp() override {
        std::pair<std::map<std::string, std::string>, std::string> userConfig;
        std::tie(targetDevice, userConfig) = GetParam();
        target_device  = targetDevice;
        APIBaseTest::SetUp();
        function = ngraph::builder::subgraph::makeConvPoolRelu();
        configuration = userConfig.first;
        std::string ext = userConfig.second;
        std::string::size_type pos = 0;
        if ((pos = ext.find(",", pos)) != std::string::npos) {
            m_extList.push_back(ext.substr(0, pos));
            m_extList.push_back(ext.substr(pos + 1));
        } else {
            m_extList.push_back(ext);
        }
        std::replace(test_name.begin(), test_name.end(), '/', '_');
        std::replace(test_name.begin(), test_name.end(), '\\', '_');
        cache_path = "LoadNetwork" + test_name + "_cache";
    }
};
} // namespace LayerTestsDefinitions
