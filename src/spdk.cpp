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


#include "spdk.hpp"

bool probe_cb(void* cb_ctx, spdk_pci_device* pci_device)
{
    // We only want to attach one device.
    spdk_ioat_chan* chan = *(spdk_ioat_chan**)(cb_ctx);
    if (!chan)
        return true;
    return false;
}

void attach_cb(void* cb_ctx, spdk_pci_device* pci_device, spdk_ioat_chan* ioat)
{
    // Check if that device/channel supports the copy operation
    if (spdk_ioat_get_dma_capabilities(ioat) & SPDK_IOAT_ENGINE_COPY_SUPPORTED)
    {
        *(spdk_ioat_chan**)(cb_ctx) = ioat;
    }
}

spdk_ioat_chan* init_spdk()
{
    char* args[] = {(char*)("")};
    rte_eal_init(1, args);

    spdk_ioat_chan* chan = nullptr;

    // Probe available devices.
    // - 'probe_cb' is called for each device found.
    // - 'attach_cb' is then called if 'probe_cb' returns true
    auto ret = spdk_ioat_probe((void*)(&chan), probe_cb, attach_cb);

    if (ret != 0)
        return nullptr;

    return chan;
}

void uninit_spdk(spdk_ioat_chan* chan)
{
    spdk_ioat_detach(chan);
}
