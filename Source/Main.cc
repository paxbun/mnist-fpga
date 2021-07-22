// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <mf/ClFactory.hh>
#include <mf/Config.hh>
#include <mf/Mnist.hh>
#include <mf/Weights.hh>

#include <algorithm>
#include <iostream>
#include <string>

void Apply(float const* in, float* out, mf::Weight const& layer)
{
    auto& weight = layer.GetKernelWeight();
    auto& bias   = layer.GetBiasWeight();

    for (size_t i = 0, li = layer.GetOutputSize(); i < li; ++i)
    {
        out[i] = 0.0f;
        for (size_t j = 0, lj = layer.GetInputSize(); j < lj; ++j)
            out[i] += in[j] * weight[j * li + i];
        out[i] += bias[i];
        if (out[i] < 0.0f)
            out[i] = 0.0f;
    }
}

int main()
try
{
    auto config { mf::Config::MakeFromEnvironment() };
    // auto [platform, device] { mf::ClFactory::MakePlatformAndDevice(config) };
    // auto [context, queue] { mf::ClFactory::MakeContextAndQueue(device) };
    // auto program { mf::ClFactory::MakeProgram(config, context, device) };
    auto weights { mf::Weights::MakeFromHdf5(config) };
    auto mnist { mf::Mnist::MakeFromFile(config) };

    std::vector<float> out1(128, 0.0f), out2(64, 0.0f), out3(10, 0.0f);
    auto&              layer1 = weights.at("dense_3");
    auto&              layer2 = weights.at("dense_4");
    auto&              layer3 = weights.at("dense_5");

    size_t correct = 0;
    for (size_t i = 0, li = mnist.GetNumSamples(); i < li; ++i)
    {
        auto sample = mnist.GetSample(i);
        Apply((float*)sample.image, out1.data(), layer1);
        Apply(out1.data(), out2.data(), layer2);
        Apply(out2.data(), out3.data(), layer3);

        auto   it    = std::max_element(out3.begin(), out3.end());
        size_t label = std::distance(out3.begin(), it);

        if (label == (size_t)sample.label)
            ++correct;

        if (i % 100 == 0)
        {
            std::cout << correct << " out of " << i << std::endl;
        }
    }
    std::cout << correct << " out of " << mnist.GetNumSamples() << std::endl;
    return 0;
}
catch (mf::ClException const& ex)
{
    std::cout << ex.GetGenericInfo();
    if (auto message { ex.GetMessage() }; message != nullptr)
        std::cout << ": " << message;
    std::cout << std::endl;
    return ex.errorCode;
}
catch (mf::Exception const& ex)
{
    std::cout << ex.GetGenericInfo();
    if (auto message { ex.GetMessage() }; message != nullptr)
        std::cout << ": " << message;
    std::cout << std::endl;
    return EXIT_FAILURE;
}
catch (std::exception const& ex)
{
    std::cout << ex.what() << std::endl;
    return EXIT_FAILURE;
}