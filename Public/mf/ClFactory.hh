// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef MNIST_FPGA_CL_FACTORY_HH
#define MNIST_FPGA_CL_FACTORY_HH

#include <mf/ClHelpers.hh>
#include <mf/Config.hh>
#include <mf/Exception.hh>

#include <utility>

namespace mf
{

/**
 * `PlatformNotFoundException` is thrown when the given platform is not found.
 */
MF_MAKE_NEW_EXCEPTION(PlatformNotFoundException, "Failed to find the platform");

/**
 * `DeviceNotFoundException` is thrown when the given device is not found.
 */
MF_MAKE_NEW_EXCEPTION(DeviceNotFoundException, "Failed to find the device");

/**
 * `ClFactory` contains helper functions to create OpenCL resources. All member functions of
 * `ClFactory` is static.
 */
class ClFactory
{
  public:
    /**
     * Creates a `cl::Platform` and a `cl::Device` instance.
     * @param config the configuration containig the name of the vendor and the device
     * @throws ClException
     * @throws PlatformNotFoundException
     * @throws DeviceNotFoundException
     */
    static std::pair<cl::Platform, cl::Device> MakePlatformAndDevice(Config const& config);

    /**
     * Creates a `cl::Context` and a `cl::Queue` instance in out-of-order mode.
     * @param device the `cl::Device` instance in which the context and the queue instance is to be
     * created.
     * @throws ClException
     */
    // static std::pair<cl::Context, cl::Queue> MakeContextAndQueue(cl::Device device);
};

}

#endif