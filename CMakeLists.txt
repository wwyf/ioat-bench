project(ioat-bench)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-strict-aliasing -march=native -m64 -D_GNU_SOURCE -fPIC -fstack-protector -DNDEBUG")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro,-z,now -Wl,-z,noexecstack")

find_package(PkgConfig REQUIRED)
pkg_check_modules(dpdk REQUIRED IMPORTED_TARGET libdpdk libdpdk-libs)

set(DPDK_HOME "/home/wyf/software/spdk/spdk/dpdk/build")
set(SPDK_HOME "/home/wyf/software/spdk/spdk/build")

include_directories(${DPDK_HOME}/include)
include_directories(${SPDK_HOME}/include)

link_directories(${DPDK_HOME}/include)
link_directories(${SPDK_HOME}/include)

link_libraries(spdk_env_dpdk)
link_libraries(spdk_ioat)
link_libraries(spdk_log)
link_libraries(spdk_util)
# link_libraries(PkgConfig::dpdk)
link_libraries(rte_eal)
link_libraries(rte_mempool)
link_libraries(rte_ring)
link_libraries(rte_bus_pci)
link_libraries(dl)
link_libraries(rt)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

# export compile commands file to build directory
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})