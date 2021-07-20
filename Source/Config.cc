// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <mf/Config.hh>

#define GETENV(VarName, EnvVarName)                                                                \
    char const* VarName { std::getenv(#EnvVarName) };                                              \
    if (VarName == nullptr)                                                                        \
        throw ConfigNotFoundException { #EnvVarName " is missing" };

namespace mf
{

Config Config::MakeFromEnvironment()
{
    GETENV(vendorName, VENDOR_NAME);
    GETENV(deviceName, DEVICE_NAME);
    GETENV(xclbinPath, XCLBIN_PATH);
    GETENV(weightFilePath, WEIGHT_PATH)
    GETENV(mnistImageFilePath, MNIST_IMAGE_PATH);
    GETENV(mnisgLabelFilePath, MNIST_LABEL_PATH);

    return Config {
        vendorName,
        deviceName,
        xclbinPath,
        weightFilePath,
    };
}

}