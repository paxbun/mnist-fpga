// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <mf/File.hh>
#include <mf/Mnist.hh>

#include <fstream>
#include <iostream>
#include <string>

#define READ_FROM_IFS(expr) (ifs.read((char*)(&(expr)), sizeof(expr)))

namespace mf
{

namespace
{

/**
 * Reverts the endianness of the given integer.
 *
 * @param value the integer of which the endianness will be reverted.
 */
void RevertEndian(uint32_t& value)
{
    union
    {
        uint8_t  b[4];
        uint32_t i;
    } u, v;
    u.i    = value;
    v.b[0] = u.b[3];
    v.b[1] = u.b[2];
    v.b[2] = u.b[1];
    v.b[3] = u.b[0];
    value  = v.i;
}

/**
 * Reads image data from the given file.
 *
 * @param imagePath the file to read
 */
std::vector<float> ReadImages(std::filesystem::path const& imagePath)
{
    std::ifstream ifs { imagePath, std::ifstream::binary };
    if (!ifs)
        throw NoSuchFileException { imagePath.string() };

    uint32_t imageMagic;
    READ_FROM_IFS(imageMagic);
    if (imageMagic != 0x03080000)
        throw InvalidMnistDatasetException { imagePath.string() };

    uint32_t numImages;
    READ_FROM_IFS(numImages);
    RevertEndian(numImages);

    uint32_t imageHeight, imageWidth;
    READ_FROM_IFS(imageHeight);
    READ_FROM_IFS(imageWidth);
    RevertEndian(imageHeight);
    RevertEndian(imageWidth);

    if (imageHeight != MnistSample::height || imageWidth != MnistSample::width)
        throw InvalidMnistDatasetException { imagePath.string() };

    uint8_t buffer[MnistSample::height * MnistSample::width];
    float   values[MnistSample::height * MnistSample::width];

    std::vector<float> data;
    data.reserve(numImages * MnistSample::height * MnistSample::width);
    for (uint32_t i = 0; i < numImages; ++i)
    {
        READ_FROM_IFS(buffer);

        for (size_t j = 0; j < MnistSample::height * MnistSample::width; ++j)
            values[j] = buffer[j] / 255.0f;

        data.insert(std::end(data), std::begin(values), std::end(values));
    }

    return data;
}

/**
 * Reads label data from the given file.
 *
 * @param labelPath the file to read
 */
std::vector<MnistLabel> ReadLabels(std::filesystem::path const& labelPath)
{
    std::ifstream ifs { labelPath, std::ifstream::binary };
    if (!ifs)
        throw NoSuchFileException { labelPath.string() };

    uint32_t labelMagic;
    READ_FROM_IFS(labelMagic);
    if (labelMagic != 0x01080000)
        throw InvalidMnistDatasetException { labelPath.string() };

    uint32_t numLabels;
    READ_FROM_IFS(numLabels);
    RevertEndian(numLabels);

    std::vector<MnistLabel> data;
    data.resize(numLabels);
    ifs.read((char*)data.data(), numLabels * sizeof(MnistLabel));

    return data;
}

}

Mnist Mnist::MakeFromFile(std::filesystem::path const& imagePath,
                          std::filesystem::path const& labelPath)
{
    auto images { ReadImages(imagePath) };
    auto labels { ReadLabels(labelPath) };
    if (images.size() != labels.size() * MnistSample::width * MnistSample::height)
        throw MnistSampleNumberDoesNotMatchException {};

    return Mnist { images.size(), std::move(images), std::move(labels) };
}

}
