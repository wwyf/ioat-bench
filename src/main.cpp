#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

#include <spdk/stdinc.h>
#include <spdk/ioat.h>
#include <spdk/env.h>
#include <spdk/queue.h>
#include <spdk/string.h>

#include <libpmemobj.h>

static int
init(void)
{
	struct spdk_env_opts opts;

	spdk_env_opts_init(&opts);
	opts.name = "ioat_bench";
	opts.core_mask = "0x1";
	if (spdk_env_init(&opts) < 0) {
		return 1;
	}

	return 0;
}

// test one channel
namespace test1{
static bool
probe_cb(void *cb_ctx, struct spdk_pci_device *pci_dev)
{
	printf(" Found matching device at %04x:%02x:%02x.%x "
	       "vendor:0x%04x device:0x%04x\n",
	       spdk_pci_device_get_domain(pci_dev),
	       spdk_pci_device_get_bus(pci_dev), spdk_pci_device_get_dev(pci_dev),
	       spdk_pci_device_get_func(pci_dev),
	       spdk_pci_device_get_vendor_id(pci_dev), spdk_pci_device_get_device_id(pci_dev));

	return true;
}



static void
attach_cb(void *cb_ctx, struct spdk_pci_device *pci_dev, struct spdk_ioat_chan *ioat)
{
    struct spdk_ioat_chan ** chan_ptr =  (struct spdk_ioat_chan **)(cb_ctx);
    if ( (*chan_ptr) == NULL){
        *chan_ptr = ioat;
    }
}

struct spdk_ioat_chan *
ioat_init_one_chan(void)
{
    struct spdk_ioat_chan * chan = NULL;
	if (spdk_ioat_probe((void *)(&chan), probe_cb, attach_cb) != 0) {
		fprintf(stderr, "ioat_probe() failed\n");
		return NULL;
	}

	return chan;
}

void ioat_done(void *cb_arg){
    int * done = (int*)cb_arg;
    *done = 1;
}

void test1(){
    struct spdk_ioat_chan * chan = NULL;
    chan = ioat_init_one_chan();
    int length = 16;
    int * buffer_src = (int *)spdk_dma_zmalloc(length*sizeof(int), sizeof(int), NULL);
    int * buffer_dst = (int *)spdk_dma_zmalloc(length*sizeof(int), sizeof(int), NULL);
    std::cout << "buffer_src" << std::endl;
    for (int i = 0; i < length; i++){
        buffer_src[i] = i;
        std::cout << buffer_src[i] << std::endl;
    }
    std::cout << "buffer_dst" << std::endl;
    for (int i = 0; i < length; i++){
        std::cout << buffer_dst[i] << std::endl;
    }

    int done = 0;
 
    spdk_ioat_submit_copy(
        chan,
        &done,
        ioat_done,
        buffer_dst,
        buffer_src,
        length*sizeof(int)
    );
    while (!done){
        spdk_ioat_process_events(chan);
    }
    std::cout << "buffer_src" << std::endl;
    for (int i = 0; i < length; i++){
        std::cout << buffer_src[i] << std::endl;
    }
    std::cout << "buffer_dst" << std::endl;
    for (int i = 0; i < length; i++){
        std::cout << buffer_dst[i] << std::endl;
    }
 
    spdk_ioat_detach(chan);
}



}



// test one channel
namespace bench_spdk_ioatdma_one_chan{
static bool
probe_cb(void *cb_ctx, struct spdk_pci_device *pci_dev)
{
	printf(" Found matching device at %04x:%02x:%02x.%x "
	       "vendor:0x%04x device:0x%04x\n",
	       spdk_pci_device_get_domain(pci_dev),
	       spdk_pci_device_get_bus(pci_dev), spdk_pci_device_get_dev(pci_dev),
	       spdk_pci_device_get_func(pci_dev),
	       spdk_pci_device_get_vendor_id(pci_dev), spdk_pci_device_get_device_id(pci_dev));

	return true;
}



static void
attach_cb(void *cb_ctx, struct spdk_pci_device *pci_dev, struct spdk_ioat_chan *ioat)
{
    struct spdk_ioat_chan ** chan_ptr =  (struct spdk_ioat_chan **)(cb_ctx);
    if ( (*chan_ptr) == NULL){
        *chan_ptr = ioat;
    }
}

struct spdk_ioat_chan *
ioat_init_one_chan(void)
{
    struct spdk_ioat_chan * chan = NULL;
	if (spdk_ioat_probe((void *)(&chan), probe_cb, attach_cb) != 0) {
		fprintf(stderr, "ioat_probe() failed\n");
		return NULL;
	}

	return chan;
}

void print_header(){
    std::cout  << "type"
        << "," << "block_count"
        << "," << "block_size"
        << "," << "duration"
        << "," << "bandwidth(MB/s)"
        << "," << "latency(ns/op)"; 
    std::cout << std::endl;

}
void print_result(const char* type, uint64_t block_count, uint64_t block_size, uint64_t duration){
    std::cout << type
        << "," << block_count 
        << "," << block_size
        << "," << duration
        << "," << (block_count * block_size * 1000LL * 1000LL * 1000LL /duration)/(1024*1024)  // bandwidth MB/s
        << "," << duration/block_count; // latency ns/op 
    std::cout << std::endl;
}


void ioat_done(void *cb_arg){
    int * done = (int*)cb_arg;
    *done = 1;
}



void spdk_copy(struct spdk_ioat_chan * chan ,  uint64_t block_count, uint64_t block_size){
    int check_count = 16;
    // prepare copy task
    char * buffer_src = (char *)spdk_dma_zmalloc(block_count*block_size, block_size, NULL);
    char * buffer_dst = (char *)spdk_dma_zmalloc(block_count*block_size, block_size, NULL);

    // std::cout << "init buffer_src" << std::endl;
    for (uint64_t i = 0; i < block_count * block_size ; i++){
        buffer_src[i] = i % 255;
    }

    std::vector<int> iters;
    for (uint64_t i = 0; i < block_count; i++){
        iters.push_back(i);
    }
    // std::random_shuffle(iters.begin(), iters.end());
    // std::cout << "init ok" << std::endl;

    int done = 0;
    int cur_block = 0;

    uint64_t duration = 0; // ns
    auto start_time = std::chrono::system_clock::now();



    // main loop
    for (uint64_t i = 0; i < block_count; i++){
        cur_block = iters[i];

        start_time = std::chrono::system_clock::now();
        done = 0;
        spdk_ioat_submit_copy(
            chan,
            &done,
            ioat_done,
            buffer_dst + cur_block*block_size,
            buffer_src + cur_block*block_size,
            block_size
        );
        while (!done){
            spdk_ioat_process_events(chan);
        }
        duration += std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::system_clock::now() - start_time).count();

    }

    print_result("ioat", block_count, block_size, duration);
    // std::cout << "buffer_src" << std::endl;
    // for (uint64_t i = 0; i < check_count; i++){
    //     std::cout << (int)buffer_src[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "buffer_dst" << std::endl;
    // for (uint64_t i = 0; i < check_count; i++){
    //     std::cout << (int)buffer_dst[i] << " ";
    // }
    // std::cout << std::endl;

    spdk_dma_free(buffer_dst);
    spdk_dma_free(buffer_src);
 
}

void memcpy_copy(uint64_t block_count, uint64_t block_size){
    int check_count = 16;
    // prepare copy task
    char * buffer_src = (char *)spdk_dma_zmalloc(block_count*block_size, block_size, NULL);
    char * buffer_dst = (char *)spdk_dma_zmalloc(block_count*block_size, block_size, NULL);

    // std::cout << "init buffer_src" << std::endl;
    for (uint64_t i = 0; i < block_count * block_size ; i++){
        buffer_src[i] = i % 255;
    }

    std::vector<int> iters;
    for (uint64_t i = 0; i < block_count; i++){
        iters.push_back(i);
    }
    // std::random_shuffle(iters.begin(), iters.end());
    // std::cout << "init ok" << std::endl;

    int done = 0;
    int cur_block = 0;

    uint64_t duration = 0; // ns
    auto start_time = std::chrono::system_clock::now();



    // main loop
    for (uint64_t i = 0; i < block_count; i++){
        cur_block = iters[i];

        start_time = std::chrono::system_clock::now();
        done = 0;
        memcpy(
            buffer_dst + cur_block*block_size,
            buffer_src + cur_block*block_size,
            block_size
        );
        duration += std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::system_clock::now() - start_time).count();

    }
    print_result("memcpy", block_count, block_size, duration);

    // std::cout << "buffer_src" << std::endl;
    // for (uint64_t i = 0; i < check_count; i++){
    //     std::cout << (int)buffer_src[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "buffer_dst" << std::endl;
    // for (uint64_t i = 0; i < check_count; i++){
    //     std::cout << (int)buffer_dst[i] << " ";
    // }
    // std::cout << std::endl;
 
    spdk_dma_free(buffer_dst);
    spdk_dma_free(buffer_src);
}

void pmem_memcpy_copy(PMEMobjpool * pop, uint64_t block_count, uint64_t block_size){
    int check_count = 16;
    // prepare copy task
    PMEMoid buffer_dst_oid;
    pmemobj_zalloc(pop, &buffer_dst_oid, (block_count*block_size)+4096, 0);

    char * buffer_src = (char *)spdk_dma_zmalloc(block_count*block_size, block_size, NULL);
    char * buffer_dst = ((char *)pmemobj_direct(buffer_dst_oid)) + 48;

    // std::cout << "init buffer_src" << std::endl;
    for (uint64_t i = 0; i < block_count * block_size ; i++){
        buffer_src[i] = i % 255;
    }

    std::vector<int> iters;
    for (uint64_t i = 0; i < block_count; i++){
        iters.push_back(i);
    }
    // std::random_shuffle(iters.begin(), iters.end());
    // std::cout << "init ok" << std::endl;

    int done = 0;
    int cur_block = 0;

    uint64_t duration = 0; // ns
    auto start_time = std::chrono::system_clock::now();



    // main loop
    for (uint64_t i = 0; i < block_count; i++){
        cur_block = iters[i];

        start_time = std::chrono::system_clock::now();
        done = 0;
        // memcpy(
        //     buffer_dst + cur_block*block_size,
        //     buffer_src + cur_block*block_size,
        //     block_size
        // );
        
        // FIXME: will stall
        pmemobj_memcpy_persist(
            pop,
            buffer_dst + cur_block*block_size,
            buffer_src + cur_block*block_size,
            block_size
        );
        duration += std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::system_clock::now() - start_time).count();

    }
    print_result("pmem-memcpy", block_count, block_size, duration);

    // std::cout << "buffer_src" << std::endl;
    // for (uint64_t i = 0; i < check_count; i++){
    //     std::cout << (int)buffer_src[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "buffer_dst" << std::endl;
    // for (uint64_t i = 0; i < check_count; i++){
    //     std::cout << (int)buffer_dst[i] << " ";
    // }
    // std::cout << std::endl;
 
    spdk_dma_free(buffer_src);
    pmemobj_free(&buffer_dst_oid);
}


void pmem_spdk_copy(PMEMobjpool * pop, struct spdk_ioat_chan * chan ,uint64_t block_count, uint64_t block_size){
    int check_count = 16;
    // prepare copy task
    PMEMoid buffer_dst_oid;
    pmemobj_zalloc(pop, &buffer_dst_oid, (block_count*block_size)+4096, 0);

    char * buffer_src = (char *)spdk_dma_zmalloc(block_count*block_size, block_size, NULL);
    char * buffer_dst = ((char *)pmemobj_direct(buffer_dst_oid)) + 48;

    // std::cout << "init buffer_src" << std::endl;
    for (uint64_t i = 0; i < block_count * block_size ; i++){
        buffer_src[i] = i % 255;
    }

    std::vector<int> iters;
    for (uint64_t i = 0; i < block_count; i++){
        iters.push_back(i);
    }
    // std::random_shuffle(iters.begin(), iters.end());
    // std::cout << "init ok" << std::endl;

    int done = 0;
    int cur_block = 0;

    uint64_t duration = 0; // ns
    auto start_time = std::chrono::system_clock::now();



    // main loop
    for (uint64_t i = 0; i < block_count; i++){
        cur_block = iters[i];

        start_time = std::chrono::system_clock::now();
        done = 0;

        // FIXME: will stall
        spdk_ioat_submit_copy(
            chan,
            &done,
            ioat_done,
            buffer_dst + cur_block*block_size,
            buffer_src + cur_block*block_size,
            block_size
        );
        while (!done){
            spdk_ioat_process_events(chan);
        }
        duration += std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::system_clock::now() - start_time).count();

    }
    print_result("pmem-memcpy", block_count, block_size, duration);

    // std::cout << "buffer_src" << std::endl;
    // for (uint64_t i = 0; i < check_count; i++){
    //     std::cout << (int)buffer_src[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "buffer_dst" << std::endl;
    // for (uint64_t i = 0; i < check_count; i++){
    //     std::cout << (int)buffer_dst[i] << " ";
    // }
    // std::cout << std::endl;
 
    spdk_dma_free(buffer_src);
    pmemobj_free(&buffer_dst_oid);
}




void bench_dram_copy(){
    uint64_t one_gb = 1ULL*1024ULL*1024ULL*1024ULL;
    // uint64_t max_capacity = 2ULL*1024ULL*1024ULL*1024ULL;
    uint64_t max_capacity = 512ULL*1024ULL*1024ULL;

    std::string pool_file = "/mnt/pmem/test_write_pool";
    std::remove(pool_file.c_str());
    sleep(2);
    PMEMobjpool * pop = pmemobj_create(pool_file.c_str(), "TEST_WRITE", max_capacity + one_gb, 0066);

    std::vector<uint64_t> block_sizes;
    std::vector<uint64_t> block_counts;
    // uint64_t block_size_start = 64;
    uint64_t block_size_start = 4096;
    for (uint64_t block_size = block_size_start; block_size <= 1024*1024*8; block_size *= 2){
        block_sizes.push_back(block_size);
        block_counts.push_back(max_capacity/block_size);
    }
    int length_block_sizes = block_sizes.size();
    struct spdk_ioat_chan * chan = NULL;
    chan = ioat_init_one_chan();

    print_header();

    for (int i = 0; i < length_block_sizes; i++){
        spdk_copy(chan, block_counts[i],block_sizes[i]);
        memcpy_copy(block_counts[i],block_sizes[i]);
        pmem_memcpy_copy(pop, block_counts[i], block_sizes[i]);
        pmem_spdk_copy(pop, chan, block_counts[i], block_sizes[i]);
    }

    spdk_ioat_detach(chan);
    pmemobj_close(pop);
    std::remove(pool_file.c_str());
}


}


// namespace bench_spdk_ioatdma_queue_depth {

// struct ioat_task {

//     void* buffer_src;
//     void* buffer_dst;
// }

// void prepare_ioat_task(){

// }


// }


int main(){
    std::cout << "hello, spdk" << std::endl;

	if (init() != 0) {
		return 1;
	}

    // test1::test1();
    bench_spdk_ioatdma_one_chan::bench_dram_copy();
    return 0;

}