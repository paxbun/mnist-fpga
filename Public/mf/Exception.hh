// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef MNIST_FPGA_EXCEPTION_HH
#define MNIST_FPGA_EXCEPTION_HH

#include <iostream>

namespace mf
{

/**
 * The base class of all custom exception of this project.
 */
struct Exception
{
  public:
    /**
     * Returns human-readable, common information that describes the error that exceptions of this
     * type indicates.
     */
    virtual char const* GetGenericInfo() const noexcept = 0;

    /**
     * Returns human-readable, more detailed information that describes the specific error that this
     * instance indicates.
     */
    virtual char const* GetMessage() const noexcept = 0;
};

/**
 * A simple `mf::Exception` implementation that only contains only one single string member
 * variable. Intended to be the base class of all simple exception classes.
 */
struct SimpleException : Exception
{
  private:
    static constexpr char const* genericInfo = "Generic exception occurred";

  private:
    std::string message;

  public:
    /**
     * @param message the message to display.
     */
    SimpleException(std::string const& message = "") : message { message } {}

    /**
     * Returns human-readable, common information that describes the error that exceptions of this
     * type indicates.
     */
    virtual char const* GetGenericInfo() const noexcept
    {
        return genericInfo;
    }

    /**
     * Returns human-readable, more detailed information that describes the specific error that this
     * instance indicates.
     */
    virtual char const* GetMessage() const noexcept final
    {
        if (message.empty())
            return nullptr;
        return message.c_str();
    }
};

}

#define MF_MAKE_NEW_EXCEPTION(ExceptionName, CommonDescription)                                    \
    struct ExceptionName : public ::mf::SimpleException                                            \
    {                                                                                              \
      private:                                                                                     \
        static constexpr char const* genericInfo = (CommonDescription);                            \
                                                                                                   \
      public:                                                                                      \
        using ::mf::SimpleException::SimpleException;                                              \
        virtual char const* GetGenericInfo() const noexcept override                               \
        {                                                                                          \
            return genericInfo;                                                                    \
        }                                                                                          \
    }

#endif