#include <iostream>

#include <spdk/stdinc.h>
#include <spdk/ioat.h>
#include <spdk/env.h>
#include <spdk/queue.h>
#include <spdk/string.h>

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





int main(){
    std::cout << "hello, spdk" << std::endl;

	if (init() != 0) {
		return 1;
	}

    test1::test1();
    return 0;

}