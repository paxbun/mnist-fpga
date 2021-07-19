// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <mf/ClHelpers.hh>
#include <mf/Config.hh>

#include <iostream>
#include <string>

int main()
try
{
    auto config { mf::Config::MakeFromEnvironment() };

    std::vector<cl::Platform> platforms;
    CL_CHECK(cl::Platform::get(&platforms));

    bool         platformDetected { false };
    cl::Platform targetPlatform;
    for (auto platform : platforms)
    {
        std::string platformVendor;
        CL_CHECK(platform.getInfo(CL_PLATFORM_VENDOR, &platformVendor));

        // platformVendor == "Xilinx" is not working, platformVendor.size() is 7
        // seems like cl::Platform::getInfo appends a redundant null character to the end of the
        // string
        if (strcmp(platformVendor.c_str(), config.vendorName.c_str()) == 0)
        {
            platformDetected = true;
            targetPlatform   = platform;
        }
    }

    if (!platformDetected)
    {
        std::cout << "Could not find the XRT." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<cl::Device> devices;
    CL_CHECK(targetPlatform.getDevices(CL_DEVICE_TYPE_ALL, &devices));

    bool       deviceDetected { false };
    cl::Device targetDevice;
    for (auto device : devices)
    {
        std::string deviceName;
        CL_CHECK(device.getInfo<std::string>(CL_DEVICE_NAME, &deviceName));

        if (strcmp(deviceName.c_str(), config.deviceName.c_str()) == 0)
        {
            deviceDetected = true;
            targetDevice   = device;
        }
    }

    if (!deviceDetected)
    {
        std::cout << "Could not find the device '" << config.deviceName << "'." << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
catch (mf::ClException const& ex)
{
    std::cout << ex.GetGenericInfo();
    if (auto message = ex.GetMessage(); message != nullptr)
        std::cout << ": " << ex.GetMessage();
    std::cout << std::endl;
    return ex.errorCode;
}
catch (mf::Exception const& ex)
{
    std::cout << ex.GetGenericInfo();
    if (auto message = ex.GetMessage(); message != nullptr)
        std::cout << ": " << ex.GetMessage();
    std::cout << std::endl;
    return EXIT_FAILURE;
}