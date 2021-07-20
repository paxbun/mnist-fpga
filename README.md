# Running inference model for MNIST using FPGAs

## How to build and launch

### Install vcpkg

`mnist-fpga` has a dependency on [HDF5](https://www.hdfgroup.org/), which is configured with [Vcpkg](https://github.com/microsoft/vcpkg). Please follow the Vcpkg installation guide and install HDF5 using Vcpkg.

### Clone including all submodules

This repo has dependencies on other repos. To clone this repo including all submodules, Type the following command:
```
git clone --recurse-submodules https://github.com/paxbun/mnist-fpga
```

### Install C++17 feature-complete compiler

Ubuntu 18.04 is shipped with GCC 7, which does not contain some of the standard headers of the C++17 standard. `mnist-fpga` is written in C++17. To properly build the project, please install GCC 9 or higher. You can install GCC 9 by the following commands:
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt-get install gcc-9 g++-9
```

### Configure CMake

Configure the project with `cmake <project directory>`. Use `-D` flag to configure Vitis and Vcpkg. If directories containing `v++` is already in `PATH`, you may not have to set `VITIS_ROOT`.
```
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=<path to Vcpkg>/scripts/buildsystems/vcpkg.cmake \
      -DVITIS_ROOT=/opt/Xilinx/Vitis/2020.1                                   \
      -G "<generator name here>"                                              \
      ..
```

If you are using Visual Studio Code, you can do the same thing in `settings.json`.
```json
{
  "cmake.configureArgs": [
    "-DVITIS_ROOT=/opt/Xilinx/Vitis/2020.1",
    "-DCMAKE_TOOLCHAIN_FILE=<path to Vcpkg>/scripts/buildsystems/vcpkg.cmake"
  ]
}
```

### Build

Build the project with `cmake --build`.
```
cmake --build .
```

### Set required environmental variables and Launch

Set the following environemntal variables to proper values:

* `XILINX_XRT`: the root directory of Xilinx Runtime
* `VENDOR_NAME`: `Xilinx`
* `DEVICE_NAME`: the name of the device (e.g. `xilinx_u250_xdma_201830_2`)
* `XCLBIN_PATH`: the path of the device binary file. (e.g. `./kernels.xclbin`)
* `WEIGHT_PATH`: the path of the weight file. (e.g. `./Model/mnist.h5`)
* `MNIST_IMAGE_PATH`: the path of the MNIST image file. (e.g. `./Model/train-images.idx3-ubyte`)
* `MNIST_LABEL_PATH`: the path of the MNIST label file. (e.g. `./Model/train-labels.idx1-ubyte`)

Note that the weight file and the MNIST dataset are located in [`Model`](./Model). If any of the variable is not properly set, the executable will fail to execute the kernel.

```
export XILINX_XRT=/opt/Xilinx/xrt
export VENDOR_NAME=Xilinx
...
./mnist-fpga
```

If you are using Visual Studio Code, you can do the same by adding configuration to `.vscode/launch.json`.
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Launch",
      "type": "cppdbg",
      "request": "launch",
      "program": "${command:cmake.launchTargetPath}",
      "args": [],
      "environment": [
        {
          "name": "XILINX_XRT",
          "value": "opt/Xilinx/xrt"
        },
        {
          "name": "VENDOR_NAME",
          "value": "Xilinx"
        }
      ]
      // more options here
    }
  ]
}
```

# License

## `mnist-fpga`

<img align="right" src="https://opensource.org/files/OSI_Approved_License.png" width="120">

Licensed under the [MIT License](https://opensource.org/licenses/MIT).

Copyright (c) 2021 Chanjung Kim

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## [`cmake/FindVitis.cmake`](https://github.com/definelicht/hlslib/blob/master/cmake/FindVitis.cmake)

<img align="right" src="https://opensource.org/files/OSI_Approved_License.png" width="120">

Licensed under the [3-Clause BSD License](https://opensource.org/licenses/BSD-3-Clause).

Copyright (c) 2017-2020, Johannes de Fine Licht, Scalable Parallel Computing Lab (SPCL), Department of Computer Science, ETH Zurich

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
