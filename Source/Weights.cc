// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <mf/Weights.hh>

#include <hdf5.h>

#include <functional>
#include <utility>

namespace mf
{

namespace
{

/**
 * Returns a pair of a file ID and a model ID.
 */
std::pair<hid_t, hid_t> GetFileAndModelWeightsGroup(std::filesystem::path const& path)
{
    hid_t fileId { H5Fopen(path.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT) };
    if (fileId < 0)
        throw NoSuchFileException { path.string() };

    hid_t modelWeightsGroupId { H5Gopen(fileId, "model_weights", H5P_DEFAULT) };
    if (modelWeightsGroupId < 0)
    {
        H5Fclose(fileId);
        throw InvalidWeightFileException {};
    }

    return std::make_pair(fileId, modelWeightsGroupId);
}

/**
 * Returns the list of all members of the given object.
 */
std::vector<std::string> GetMemberNames(hid_t objectId)
{
    std::vector<std::string> memberNames;
    H5Literate2(
        objectId,
        H5_INDEX_NAME,
        H5_ITER_INC,
        nullptr,
        [](hid_t group, char const* name, H5L_info2_t const* info, void* opData) {
            auto& memberNames = *(std::vector<std::string>*)opData;
            memberNames.push_back(name);
            return (herr_t)0;
        },
        &memberNames);

    return memberNames;
}

/**
 * Returns the identifier of the given object of the given group if it exists; returns negative
 * value otherwise.
 */
hid_t GetObjectWithType(hid_t groupId, char const* name, H5O_type_t type)
{
    if (H5Lexists(groupId, name, H5P_DEFAULT) <= 0)
        return -1;

    hid_t objectId { H5Oopen(groupId, name, H5P_DEFAULT) };
    if (objectId < 0)
        return -1;

    H5O_info2_t info {};
    if (H5Oget_info3(objectId, &info, H5O_INFO_BASIC) < 0)
        return -1;

    if (info.type != type)
        return -1;

    return objectId;
}

/**
 * Iterates over the given group and calls the given callback for every bias-kernel pair.
 */
void IterateOverModelWeightsGroup(
    hid_t                                                                             groupId,
    std::function<void(std::string const& layerName, hid_t biasId, hid_t kernelId)>&& func)
{
    // TensorFlow model weights file structure (See /Model/mnist.h5)
    //
    // + model_weights
    // |  + dense_n
    // |  |  + dense_n
    // |  |     + bias:0
    // |  |     + kernel:0
    // |  + dense_m
    // |  |  + dense_m
    // |  |     + bias:0
    // |  |     + kernel:0
    // |  ...
    // + optimizer_weights
    //    ...

    if (!func)
        return;

    std::vector<std::string> memberNames { GetMemberNames(groupId) };
    for (auto& memberName : memberNames)
    {
        hid_t groupId0 { GetObjectWithType(groupId, memberName.c_str(), H5O_TYPE_GROUP) };
        if (groupId0 < 0)
            continue;

        hid_t groupId1 { GetObjectWithType(groupId0, memberName.c_str(), H5O_TYPE_GROUP) };
        if (groupId1 < 0)
        {
            H5Gclose(groupId0);
            continue;
        }

        hid_t biasId { GetObjectWithType(groupId1, "bias:0", H5O_TYPE_DATASET) };
        if (biasId < 0)
        {
            H5Gclose(groupId1);
            H5Gclose(groupId0);
            continue;
        }

        hid_t kernelId { GetObjectWithType(groupId1, "kernel:0", H5O_TYPE_DATASET) };
        if (kernelId < 0)
        {
            H5Dclose(biasId);
            H5Gclose(groupId1);
            H5Gclose(groupId0);
            continue;
        }

        func(memberName, biasId, kernelId);

        H5Dclose(kernelId);
        H5Dclose(biasId);
        H5Gclose(groupId1);
        H5Gclose(groupId0);
    }
}

}

WeightCollection Weights::MakeFromHdf5(std::filesystem::path const& path)
{
    auto [fileId, modelWeightsGroupId] { GetFileAndModelWeightsGroup(path) };

    WeightCollection rtn;
    IterateOverModelWeightsGroup(
        modelWeightsGroupId, [&rtn](std::string const& layerName, hid_t biasId, hid_t kernelId) {
            hid_t biasSpace { H5Dget_space(biasId) };
            if (biasSpace < 0)
                return;

            hid_t kernelSpace { H5Dget_space(kernelId) };
            if (kernelSpace < 0)
                return;

            do {
                int numBiasDims   = H5Sget_simple_extent_ndims(biasSpace);
                int numKernelDims = H5Sget_simple_extent_ndims(kernelSpace);
                if (numBiasDims != 1 || numKernelDims != 2)
                    break;

                hsize_t biasDims[1], kernelDims[2];
                if (H5Sget_simple_extent_dims(biasSpace, biasDims, nullptr) < 0)
                    break;
                if (H5Sget_simple_extent_dims(kernelSpace, kernelDims, nullptr) < 0)
                    break;

                size_t inputSize = kernelDims[0], outputSize = kernelDims[1];
                if (outputSize != biasDims[0])
                    break;

                std::vector<float> bias(outputSize, 0.0f);
                if (H5Dread(biasId, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, bias.data())
                    < 0)
                    break;

                std::vector<float> kernel(inputSize * outputSize, 0.0f);
                if (H5Dread(
                        kernelId, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, kernel.data())
                    < 0)
                    break;

                rtn.insert(std::make_pair(
                    layerName,
                    Weight { inputSize, outputSize, std::move(kernel), std::move(bias) }));

            } while (false);

            H5Sclose(kernelSpace);
            H5Sclose(biasSpace);
        });

    H5Gclose(modelWeightsGroupId);
    H5Fclose(fileId);

    return rtn;
}

}