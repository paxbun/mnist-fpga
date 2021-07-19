// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef MNIST_FPGA_CONFIG_HH
#define MNIST_FPGA_CONFIG_HH

#include <mf/Exception.hh>

#include <cstdlib>
#include <string>

namespace mf
{

/**
 * `ConfigNotFoundException` is thrown when any error occurred during retrieving the configuration.
 */
MF_MAKE_NEW_EXCEPTION(ConfigNotFoundException, "Failed to load the configuration");

/**
 * `Config` contains options required during the execution of the program.
 */
struct Config
{
    /**
     * the vendor of the device. Corresponds to the `VENDOR_NAME` environmental variable.
     */
    std::string vendorName;

    /**
     * the name of the device. Corresponds to the `DEVICE_NAME` environmental variable.
     */
    std::string deviceName;

    /**
     * Creates a `Config` instance from environmental variables.
     *
     * @throws ConfigNotFoundException If any required environmental variable is not set.
     */
    static Config MakeFromEnvironment();
};

}

#endif