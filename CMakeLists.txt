project(ex4)
cmake_minimum_required(VERSION 3.1)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-strict-aliasing -march=native -m64 -D_GNU_SOURCE -fPIC -fstack-protector -DNDEBUG")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro,-z,now -Wl,-z,noexecstack")

#include_directories(./spdk/include)
#include_directories(./spdk/dpdk-16.11/include/dpdk)

#link_directories(./spdk/build/lib)
#link_directories(./spdk/dpdk-16.11/x86_64-native-linuxapp-gcc/lib)

find_package(PkgConfig REQUIRED)

pkg_check_modules(dpdk REQUIRED IMPORTED_TARGET libdpdk)

#pkg_check_modules(spdk REQUIRED IMPORTED_TARGET spdk_accel_ioat spdk_accel_modules spdk_accel spdk_bdev_aio spdk_bdev_delay spdk_bdev_error spdk_bdev_ftl spdk_bdev_gpt spdk_bdev_lvol spdk_bdev_malloc spdk_bdev_modules spdk_bdev_null spdk_bdev_nvme spdk_bdev_passthru spdk_bdev spdk_bdev_pmem spdk_bdev_raid spdk_bdev_split spdk_bdev_virtio spdk_bdev_zone_block spdk_blob_bdev spdk_blobfs_bdev spdk_blobfs spdk_blob spdk_conf spdk_dpdklibs spdk_env_dpdk spdk_env_dpdk_rpc spdk_event_accel spdk_event_bdev spdk_event_iscsi spdk_event_nbd spdk_event_net spdk_event_nvmf spdk_event spdk_event_scsi spdk_event_sock spdk_event_vhost spdk_event_vmd spdk_ftl spdk_ioat spdk_iscsi spdk_json spdk_jsonrpc spdk_log spdk_lvol spdk_nbd spdk_net spdk_notify spdk_nvme spdk_nvmf spdk_rdma spdk_rpc spdk_scsi spdk_sock_modules spdk_sock spdk_sock_posix spdk_syslibs spdk_thread spdk_trace spdk_util spdk_ut_mock spdk_vhost spdk_virtio spdk_vmd )

pkg_check_modules(spdk REQUIRED IMPORTED_TARGET spdk_ioat spdk_env_dpdk spdk_util spdk_log )
#pkg_check_modules(spdk REQUIRED IMPORTED_TARGET spdk_accel_ioat spdk_ioat spdk_env_dpdk spdk_util spdk_log )

#link_libraries(spdk_ioat)
#link_libraries(spdk_log)
#link_libraries(spdk_util)
#link_libraries(spdk_env_dpdk)
#link_libraries(rte_eal)
#link_libraries(rte_mempool)
#link_libraries(rte_ring)
link_libraries(dl)
link_libraries(rt)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

link_libraries(PkgConfig::dpdk)
link_libraries(PkgConfig::spdk)

# export compile commands file to build directory
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
