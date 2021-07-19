// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef MNIST_FPGA_CL_HELPERS_HH
#define MNIST_FPGA_CL_HELPERS_HH

#include <CL/cl.hpp>
#include <mf/Exception.hh>

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
struct ClException : public Exception
{
  private:
    constexpr static char const* genericInfo = "OpenCL call failed";

    static std::string MakeMessage(cl_int errorCode, char const* expression)
    {
        std::string rtn { expression ? expression : "" };
        if (expression == nullptr)
            rtn += "Failed with error code (";
        else
            rtn += " failed with error code (";

        rtn += std::to_string(errorCode);
        rtn += ")";
        return rtn;
    }

  private:
    std::string description;

  public:
    /**
     * the result of the expression.
     */
    cl_int errorCode;

    /**
     * the expression passed to `CL_CHECK`. Possibly `nullptr`.
     */
    char const* expression;

    /**
     * @param errorCode the result of the expression.
     * @param expression the expression passed to `CL_CHECK`.
     */
    ClException(cl_int errorCode, char const* expression = nullptr) :
        errorCode { errorCode },
        expression { expression },
        description { MakeMessage(errorCode, expression) }
    {}

    virtual char const* GetGenericInfo() const noexcept override
    {
        return genericInfo;
    }

    virtual char const* GetMessage() const noexcept override
    {
        return description.c_str();
    }
};

}

#endif