// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <mf/Config.hh>

namespace mf
{

Config Config::MakeFromEnvironment()
{
    char const* vendorName { std::getenv("VENDOR_NAME") };
    if (vendorName == nullptr)
        throw ConfigNotFoundException { "VENDOR_NAME is missing" };

    char const* deviceName { std::getenv("DEVICE_NAME") };
    if (deviceName == nullptr)
        throw ConfigNotFoundException { "DEVICE_NAME is missing" };

    return Config { vendorName, deviceName };
}
}