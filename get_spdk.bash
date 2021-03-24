#!/bin/bash

set -x

cd `dirname $0`

mkdir -p ./spdk

SPDK_DIR="`pwd`/spdk"

#rm -rf "$SPDK_DIR"

#git clone https://github.com/spdk/spdk.git spdk
pushd spdk
#wget http://fast.dpdk.org/rel/dpdk-20.11.1.tar.xz
#wget http://fast.dpdk.org/rel/dpdk-16.11.tar.xz
#tar xf dpdk-16.11.tar.xz
#tar xf dpdk-20.11.1.tar.xz
mv dpdk-stable-20.11.1 dpdk-16.11

(cd dpdk-16.11 && make install T=x86_64-native-linuxapp-gcc DESTDIR=.)
make -j4 DPDK_DIR=./dpdk-16.11/x86_64-native-linuxapp-gcc
popd
