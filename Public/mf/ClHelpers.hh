// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef MNIST_FPGA_CL_HELPERS_HH
#define MNIST_FPGA_CL_HELPERS_HH

#include <CL/cl.hpp>

#include <stdexcept>
#include <string>

#define CL_CHECK(expr)                                                                             \
    if (auto errorCode = (expr); errorCode != CL_SUCCESS)                                          \
        throw ::mf::ClException { errorCode, #expr };

namespace mf
{

/**
 * `ClException` is thrown when the result of the expression passed to `CL_CHECK` indicates a
 * failure.
 */
struct ClException : public std::exception
{
    /**
     * the expression passed to `CL_CHECK`. Possibly `nullptr`.
     */
    char const* expression;

    /**
     * the result of the expression.
     */
    cl_int errorCode;

    /**
     * the description of the error. `ClException::what` returns the same string.
     * @see ClException::what
     */
    std::string description;

    /**
     * @param errorCode the result of the expression.
     * @param expression the expression passed to `CL_CHECK`.
     */
    ClException(cl_int errorCode, char const* expression = nullptr) :
        expression { expression },
        errorCode { errorCode },
        description { expression == nullptr ? "Some expression" : expression }
    {
        description += " failed with error code (";
        description += std::to_string(errorCode);
        description += ")";
    }

    virtual char const* what() const noexcept override
    {
        return description.c_str();
    }
};

}

#endif