/*
Copyright (c) <2017>, Intel Corporation

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of Intel Corporation nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "seq_spdk.hpp"
#include "random_number_generator.hpp"
#include "utils.hpp"

static void req_cb(void* arg)
{
    *(bool*)arg = true;
}

benchmark seq_spdk(uint64_t chunk_size, uint64_t buffer_size, spdk_ioat_chan* chan)
{
    using namespace std::chrono_literals;

    // Allocate the whole buffer, 8 bytes-aligned
    uint64_t* buffer64 = (uint64_t*)spdk_dma_malloc(buffer_size, sizeof(uint64_t), nullptr);
    uint8_t*  buffer8  = reinterpret_cast<uint8_t*>(buffer64);

    // Trick the optimizer into not optimizing any copies away
    utils::escape(buffer8);

    // Fill the buffer with random data
    random_number_generator<uint64_t> rnd;
    for (uint i = 0 / sizeof(uint64_t); i < buffer_size / sizeof(uint64_t); ++i)
    {
        buffer64[i] = rnd.get();
    }

    uint64_t                 iterations = 0;
    std::chrono::nanoseconds time       = 0s;

    uint64_t nb_chunks = buffer_size / chunk_size;

    random_number_generator<uint64_t> chunk_idx_gen(0, nb_chunks / 2 - 1);

    bool copy_done = false;

    do
    {
        // pick a random even-indexed buffer as source
        uint64_t src_chunk_idx = chunk_idx_gen.get() * 2;
        // pick a random odd-index buffer as destination
        uint64_t dst_chunk_idx = chunk_idx_gen.get() * 2 + 1;

        auto start_cpy = std::chrono::steady_clock::now();

        copy_done = false;
        // Submit the copy. req_cb is called when the copy is done, and will set 'copy_done' to true
        spdk_ioat_submit_copy(
            chan,
            &copy_done,
            req_cb,
            buffer8 + (dst_chunk_idx * chunk_size),
            buffer8 + (src_chunk_idx * chunk_size),
            chunk_size);

        // We wait for 'copy_done' to have been set to true by 'req_cb'
        do
        {
            spdk_ioat_process_events(chan);
        } while (!copy_done);

        time += (std::chrono::steady_clock::now() - start_cpy);

        // Trick the optimizer into not optimizing any copies away
        utils::clobber();

        ++iterations;
    } while (time < 1s);

    spdk_dma_free(buffer64);

    return {chunk_size, buffer_size, time, iterations};
}
