// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef MNIST_FPGA_WEIGHTS_HH
#define MNIST_FPGA_WEIGHTS_HH

#include <mf/Config.hh>
#include <mf/Exception.hh>
#include <mf/File.hh>

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace mf
{

/**
 * `InvalidWeightFileException` is thrown when the opened HDF5 file does not contain model weights.
 */
MF_MAKE_NEW_EXCEPTION(InvalidWeightFileException, "HDF5 file does not contain model weights");

/**
 * `Weight` contains parameter values for one single FC layer.
 */
class Weight
{
    friend class Weights;

  private:
    size_t             _inputSize;
    size_t             _outputSize;
    std::vector<float> _kernel;
    std::vector<float> _bias;

  public:
    /**
     * Returns the length of the input.
     */
    size_t GetInputSize() const noexcept
    {
        return _inputSize;
    }

    /**
     * Returns the length of the output.
     */
    size_t GetOutputSize() const noexcept
    {
        return _outputSize;
    }

    /**
     * Returns the weight of the matmul operation. The dimension of the matrix is (I, O), where
     * I is the length of the input and O is the length of the output.
     */
    std::vector<float> const& GetKernelWeight() const noexcept
    {
        return _kernel;
    }

    /**
     * Returns the weight of the vector addition. The length of the vector is O.
     */
    std::vector<float> const& GetBiasWeight() const noexcept
    {
        return _bias;
    }

  private:
    Weight(size_t               inputSize,
           size_t               outputSize,
           std::vector<float>&& kernel,
           std::vector<float>&& bias) :
        _inputSize { inputSize },
        _outputSize { outputSize },
        _kernel { std::move(kernel) },
        _bias { std::move(bias) }
    {}
};

/**
 * `WeightCollection` is a dictionary where the key is the name of the layer, and the value is its
 * weight.
 */
using WeightCollection = std::map<std::string, Weight>;

/**
 * `Weights` contains helper functions to create `ModelWeight` and `Weight` instances. All member
 * functions of `Weights` are static.
 */
class Weights
{
  public:
    /**
     * Reads layer weights from given HDF5 file.
     *
     * @param path the path of the HDF5 file to read.
     * @throws NoSuchFileException
     */
    static WeightCollection MakeFromHdf5(std::filesystem::path const& path);

    /**
     * Reads layer weights from the file specified in the configuration.
     *
     * @param config the configuration
     * @throws NoSuchFileException
     */
    inline static WeightCollection MakeFromHdf5(Config const& config)
    {
        return MakeFromHdf5(config.weightFilePath);
    }
};

}

#endif