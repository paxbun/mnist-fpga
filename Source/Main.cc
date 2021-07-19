// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <mf/ClFactory.hh>
#include <mf/Config.hh>

#include <iostream>
#include <string>

int main()
try
{
    auto config { mf::Config::MakeFromEnvironment() };
    auto [platform, device] { mf::ClFactory::MakePlatformAndDevice(config) };

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