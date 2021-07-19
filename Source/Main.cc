// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <mf/ClHelpers.hh>

#include <iostream>

int main()
try
{
    std::vector<cl::Platform> platforms;
    CL_CHECK(cl::Platform::get(&platforms));

    bool platformDetected = false;
    for (auto platform : platforms)
    {
        std::string platformVendor;
        CL_CHECK(platform.getInfo(CL_PLATFORM_VENDOR, &platformVendor));

        std::cout << "Platform: " << platformVendor << std::endl;
    }
    return 0;
}
catch (mf::ClException const& ex)
{
    std::cout << ex.what() << std::endl;
    return ex.errorCode;
}