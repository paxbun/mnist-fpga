// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef MNIST_FGPA_MNIST_HH
#define MNIST_FGPA_MNIST_HH

#include <mf/Config.hh>
#include <mf/File.hh>

#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <vector>

namespace mf
{

/**
 * Represents label for one single MNIST sample.
 */
enum class MnistLabel : uint8_t
{
    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine
};

/**
 * Represents one single immutable MNIST sample.
 */
struct MnistSample
{
    constexpr static size_t width { 28 };
    constexpr static size_t height { 28 };

    float const (&image)[width][height];
    MnistLabel const label;
};

MF_MAKE_NEW_EXCEPTION(InvalidMnistDatasetException, "MNIST dataset file is corrupted");

MF_MAKE_NEW_EXCEPTION(MnistSampleNumberDoesNotMatchException,
                      "The number of images and the number of labels are different");

/**
 * Represents a collection of hand-written 28x28 greyscale digit images and corresponding labels.
 */
class Mnist
{
  public:
    /**
     * Reads the given two files and creates one complete MNIST dataset instance.
     *
     * @param imagePath the file containing image data
     * @param labelPath the file containing label data
     * @throws NoSuchFileException
     * @throws InvalidMnistDatasetException
     * @throws MnistSampleNumberDoesNotMatchException
     */
    static Mnist MakeFromFile(std::filesystem::path const& imagePath,
                              std::filesystem::path const& labelPath);

    /**
     * Reads two MNIST data files specified in the config and creates one complete MNIST dataset
     * instance.
     *
     * @param config the configuration
     * @throws NoSuchFileException
     * @throws InvalidMnistDatasetException
     * @throws MnistSampleNumberDoesNotMatchException
     */
    inline static Mnist MakeFromFile(Config const& config)
    {
        return MakeFromFile(config.mnistImageFilePath, config.mnistLabelFilePath);
    }

  private:
    size_t                  _numSamples;
    std::vector<float>      _images;
    std::vector<MnistLabel> _labels;

  private:
    Mnist(size_t numSamples, std::vector<float>&& images, std::vector<MnistLabel>&& labels) :
        _numSamples { numSamples },
        _images { std::move(images) },
        _labels { std::move(labels) }
    {}

  public:
    /**
     * Returns a `MnistSample` instance corresponding to the given index.
     *
     * @param idx the index of the sample
     * @throws std::invalid_argument if the index is equal to or greater than the number of samples
     */
    MnistSample GetSample(size_t idx) const
    {
        if (idx >= _numSamples)
            throw std::invalid_argument { "idx" };

        return MnistSample {
            ((float(*)[MnistSample::width][MnistSample::height])_images.data())[idx],
            _labels[idx],
        };
    }

    /**
     * Returns the internal buffer containing image data. The length of the vector is 28 x 28 x
     * `GetNumberSamples()`.
     */
    std::vector<float> const& GetImages() const noexcept
    {
        return _images;
    }

    /**
     * Returns the internal buffer containing label data. The length of the vector is
     * `GetNumberSamples()`.
     */
    std::vector<MnistLabel> const& GetLabels() const noexcept
    {
        return _labels;
    }

    /**
     * Returns the number of samples.
     */
    size_t GetNumSamples() const noexcept
    {
        return _numSamples;
    }
};

}

#endif