project(ex4)
cmake_minimum_required(VERSION 3.1)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-strict-aliasing -march=native -m64 -D_GNU_SOURCE -fPIC -fstack-protector -DNDEBUG")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro,-z,now -Wl,-z,noexecstack")

#include_directories(./spdk/include)
#include_directories(./spdk/dpdk-16.11/include/dpdk)

#link_directories(./spdk/build/lib)
#link_directories(./spdk/dpdk-16.11/x86_64-native-linuxapp-gcc/lib)

link_libraries(spdk_ioat)
link_libraries(spdk_log)
link_libraries(spdk_util)
link_libraries(spdk_env_dpdk)
link_libraries(rte_eal)
link_libraries(rte_mempool)
link_libraries(rte_ring)
link_libraries(dl)
link_libraries(rt)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

# export compile commands file to build directory
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
