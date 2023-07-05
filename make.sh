#!/bin/bash
set -e

if [ $1 == clean ]; then
rm -rf ./build
rm -rf ./install
else
# set the opencv lib version
ocv_version="3"
for par in $@; do
    if [ ${par%=*} == "ocv_version" ]; then
      ocv_version=${par#*=}
    fi
done
# for your board compiler
#GCC_COMPILER=/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf
ROOT_PWD=$( cd "$( dirname $0 )" && cd -P "$( dirname "$SOURCE" )" && pwd )

# build rockx
BUILD_DIR=${ROOT_PWD}/build

if [[ ! -d "${BUILD_DIR}" ]]; then
  mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
cmake .. -DOCV_LIB_VERSION=${ocv_version} #\
     # -DCMAKE_C_COMPILER=${GCC_COMPILER}-gcc \
     # -DCMAKE_CXX_COMPILER=${GCC_COMPILER}-g++
make -j4
make install
cd -
fi