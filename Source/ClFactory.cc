// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <mf/ClFactory.hh>

namespace mf
{

namespace
{

cl::Platform MakePlatform(Config const& config)
{
    std::vector<cl::Platform> platforms;
    CL_CHECK(cl::Platform::get(&platforms));

    for (auto platform : platforms)
    {
        std::string platformVendor;
        CL_CHECK(platform.getInfo(CL_PLATFORM_VENDOR, &platformVendor));

        // platformVendor == "Xilinx" is not working, platformVendor.size() is 7
        // seems like cl::Platform::getInfo appends a redundant null character to the end of the
        // string
        if (strcmp(platformVendor.c_str(), config.vendorName.c_str()) == 0)
            return platform;
    }
    throw PlatformNotFoundException { config.vendorName };
}

cl::Device MakeDevice(Config const& config, cl::Platform platform)
{
    std::vector<cl::Device> devices;
    CL_CHECK(platform.getDevices(CL_DEVICE_TYPE_ALL, &devices));

    for (auto device : devices)
    {
        std::string deviceName;
        CL_CHECK(device.getInfo<std::string>(CL_DEVICE_NAME, &deviceName));

        if (strcmp(deviceName.c_str(), config.deviceName.c_str()) == 0)
            return device;
    }
    throw DeviceNotFoundException { config.deviceName };
}

}

std::pair<cl::Platform, cl::Device> ClFactory::MakePlatformAndDevice(Config const& config)
{
    auto platform { MakePlatform(config) };
    auto device { MakeDevice(config, platform) };
    return std::make_pair(platform, device);
}

}