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



int main(){
    std::cout << "hello, spdk" << std::endl;

	if (init() != 0) {
		return 1;
	}

}