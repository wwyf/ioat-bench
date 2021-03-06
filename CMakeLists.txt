project(ioat-bench)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-strict-aliasing -march=native -m64 -D_GNU_SOURCE -fPIC -fstack-protector -DNDEBUG")
#  -mcmodel=large -g
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro,-z,now -Wl,-z,noexecstack")

find_package(PkgConfig REQUIRED)
pkg_check_modules(dpdk REQUIRED IMPORTED_TARGET libdpdk libdpdk-libs)

set(DPDK_HOME "/home/wyf/software/spdk/spdk/dpdk/build")
set(SPDK_HOME "/home/wyf/software/spdk/spdk/build")

include_directories(${DPDK_HOME}/include)
include_directories(${SPDK_HOME}/include)

link_directories(${DPDK_HOME}/lib)
link_directories(${SPDK_HOME}/lib)

link_libraries(spdk_env_dpdk)
link_libraries(spdk_log)

link_libraries(spdk_env_dpdk)
link_libraries(spdk_bdev)
link_libraries(spdk_bdev_aio)
link_libraries(spdk_bdev_error)
link_libraries(spdk_bdev_gpt)
link_libraries(spdk_bdev_lvol)
link_libraries(spdk_bdev_malloc)
link_libraries(spdk_bdev_null)
link_libraries(spdk_bdev_nvme)
link_libraries(spdk_bdev_passthru)
link_libraries(spdk_bdev_raid)
link_libraries(spdk_bdev_split)
link_libraries(spdk_bdev_virtio)
link_libraries(spdk_blob)
link_libraries(spdk_blob_bdev)
link_libraries(spdk_blobfs)
link_libraries(spdk_conf)
link_libraries(spdk_event)
link_libraries(spdk_event_iscsi)
link_libraries(spdk_event_nbd)
link_libraries(spdk_event_net)
link_libraries(spdk_event_nvmf)
link_libraries(spdk_event_scsi)
link_libraries(spdk_event_vhost)
link_libraries(spdk_ioat)
link_libraries(spdk_iscsi)
link_libraries(spdk_json)
link_libraries(spdk_jsonrpc)
link_libraries(spdk_log)
link_libraries(spdk_lvol)
link_libraries(spdk_nbd)
link_libraries(spdk_net)
link_libraries(spdk_notify)
link_libraries(spdk_nvme)
link_libraries(spdk_nvmf)
link_libraries(spdk_rpc)
link_libraries(spdk_scsi)
link_libraries(spdk_sock)
link_libraries(spdk_sock_posix)
link_libraries(spdk_thread)
link_libraries(spdk_trace)
link_libraries(spdk_util)
link_libraries(spdk_ut_mock)
link_libraries(spdk_vhost)
link_libraries(spdk_virtio)

# link_libraries(PkgConfig::dpdk)
link_libraries(rte_eal)
link_libraries(rte_mempool)
link_libraries(rte_ring)
link_libraries(rte_bus_pci)
link_libraries(dl)
link_libraries(rt)

link_libraries(pmemobj)
link_libraries(pmem)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

# export compile commands file to build directory
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

aux_source_directory(src.ref SRC_LIST_REF)
add_executable(ex4 ${SRC_LIST_REF})