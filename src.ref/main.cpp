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


#include "display_results.hpp"
#include "par_spdk.hpp"
#include "seq_memcpy.hpp"
#include "seq_spdk.hpp"
#include "spdk.hpp"
#include "utils.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

int main()
{
    std::cout << "[Info   ] Initializing SPDK...\n";

    spdk_ioat_chan* chan = init_spdk();
    if (!chan)
    {
        std::cout << "[Error  ] Failed to initialize SPDK\n";
        exit(-1);
    }

    // Each test is defined by 2 number {a, b}.
    // We're copying chunks of 2^a bytes inside a 2^b bytes buffer.
    const std::vector<std::pair<uint8_t, uint8_t>> tests = {
        {1, 5},   {3, 5},   {3, 9},   {5, 9},   {7, 9},   {7, 13},  {9, 13},
        {11, 13}, {11, 17}, {13, 17}, {15, 17}, {15, 21}, {17, 21}, {19, 21},
        {19, 25}, {21, 25}, {23, 25}, {23, 29}, {25, 29}, {27, 29}};

    display_header();

    for (const auto& exps : tests)
    {
        auto res_1 = seq_memcpy(utils::int_pow(2, exps.first), utils::int_pow(2, exps.second));
        auto res_2 = seq_spdk(utils::int_pow(2, exps.first), utils::int_pow(2, exps.second), chan);
        auto res_3 = par_spdk(utils::int_pow(2, exps.first), utils::int_pow(2, exps.second), chan);

        display_results(res_1, res_2, res_3);
    }

    uninit_spdk(chan);
}
