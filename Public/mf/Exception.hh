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
  private:
    static constexpr char const* genericInfo = "Generic exception occurred";

  private:
    char const* message;

  public:
    /**
     * @param message the message to display.
     */
    Exception(char const* message = nullptr) : message { message ? message : "" } {}

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
    virtual char const* GetMessage() const noexcept
    {
        return message;
    }
};

}

#define MF_MAKE_NEW_EXCEPTION(ExceptionName, CommonDescription)                                    \
    struct ExceptionName : public ::mf::Exception                                                  \
    {                                                                                              \
      private:                                                                                     \
        static constexpr char const* genericInfo = (CommonDescription);                            \
                                                                                                   \
      public:                                                                                      \
        using ::mf::Exception::Exception;                                                          \
        virtual char const* GetGenericInfo() const noexcept override                               \
        {                                                                                          \
            return genericInfo;                                                                    \
        }                                                                                          \
    }

#endif