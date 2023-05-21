#!/bin/bash

set -e

# for your board compiler
#GCC_COMPILER=/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf

ROOT_PWD=$( cd "$( dirname $0 )" && cd -P "$( dirname "$SOURCE" )" && pwd )

# build rockx
BUILD_DIR=${ROOT_PWD}/build

if [[ ! -d "${BUILD_DIR}" ]]; then
  mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
cmake .. #\
     # -DCMAKE_C_COMPILER=${GCC_COMPILER}-gcc \
     # -DCMAKE_CXX_COMPILER=${GCC_COMPILER}-g++
make -j4
make install
cd -