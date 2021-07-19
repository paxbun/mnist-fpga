// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <mf/File.hh>

#include <fstream>

namespace mf
{

std::vector<uint8_t> File::ReadFile(std::filesystem::path const& path)
{
    std::ifstream ifs { path };
    if (!ifs)
        throw NoSuchFileException { path.string() };

    ifs.seekg(0, std::ios::end);
    size_t fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<uint8_t> vec;
    vec.resize(fileSize);
    ifs.read((char*)vec.data(), fileSize);

    return vec;
}

}