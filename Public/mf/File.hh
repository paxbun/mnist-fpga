// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef MNIST_FPAG_FILE_HH
#define MNIST_FPAG_FILE_HH

#include <mf/Exception.hh>

#include <cstdint>
#include <filesystem>
#include <vector>

namespace mf
{

/**
 * `NoSuchFileException` is thrown when any error occurred during opening and reading the
 * given file.
 */
MF_MAKE_NEW_EXCEPTION(NoSuchFileException, "Could not open the file");

/**
 * Contains file IO helper functions. All member functions of this class is static.
 */
class File
{
  public:
    std::vector<uint8_t> ReadFile(std::filesystem::path const& path);
};

}

#endif