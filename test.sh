#!/bin/bash

source /mnt/wyf/admin/softwares-all-in-one/env.sh
spack load gcc@7.5.0
#spack compiler add --scope=site
spack load cmake %gcc@7.5.0

DPDK_HOME=/home/wyf/software/spdk/spdk/dpdk/build
SPDK_HOME=/home/wyf/software/spdk/spdk/build
export PATH=${DPDK_HOME}/bin:${SPDK_HOME}/bin:${PATH}
export CPATH=${DPDK_HOME}/include:${SPDK_HOME}/include:${CPATH}
export LIBRARY_PATH=${DPDK_HOME}/lib:${SPDK_HOME}/lib:${LIBRARY_PATH}
export LD_LIBRARY_PATH=${DPDK_HOME}/lib:${SPDK_HOME}/lib:${LD_LIBRARY_PATH}
export PKG_CONFIG_PATH=${DPDK_HOME}/lib/pkgconfig:${SPDK_HOME}/lib/pkgconfig:${PKG_CONFIG_PATH}

rm -rf build
mkdir -p build
cd build
cmake ..
make

ulimit -c unlimited

./ioat-bench
# ./ex4