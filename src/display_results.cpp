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
#include "size.hpp"
#include <iomanip>
#include <iostream>

void display_header()
{
    // clang-format off
    std::cout << "\n";
    std::cout << "|-----------------------|---------------------------|--------------------------------------|-------------------------------------------|\n";
    std::cout << "| Chunk     / Buffer    | Memcpy                    | SPDK (sequential)                    | SPDK (n tasks submitted at once)          |\n";
    std::cout << "| size      / Size      | Time        | Speed       | Time        | Speed       | Improv.  | n  | Time        | Speed       | Improv.  |\n";
    std::cout << "|-----------------------|-------------|-------------|-------------|-------------|----------|----|-------------|-------------|----------|\n";
    // clang-format on
}

void display_results(benchmark seq_memcpy, benchmark seq_spdk, benchmark par_spdk)
{
    std::cout << "| " << std::setw(9) << size::to_string(seq_memcpy.chunk_size, false) << " / ";
    std::cout << std::setw(9) << size::to_string(seq_memcpy.buffer_size, false) << " | ";
    std::cout << std::setw(8) << seq_memcpy.time.count() / seq_memcpy.iterations << " ns | ";
    std::cout << std::setw(9)
              << size::to_string(
                     seq_memcpy.chunk_size *
                         (1000000000 / (seq_memcpy.time.count() / seq_memcpy.iterations)),
                     false)
              << "/s | ";

    std::cout << std::setw(8) << seq_spdk.time.count() / seq_spdk.iterations << " ns | ";
    std::cout << std::setw(9)
              << size::to_string(
                     seq_spdk.chunk_size *
                         (1000000000 / (seq_spdk.time.count() / seq_spdk.iterations)),
                     false)
              << "/s | ";
    std::cout << "x " << std::fixed << std::setw(6) << std::setprecision(2)
              << ((double)seq_memcpy.time.count() / seq_memcpy.iterations) /
                     (seq_spdk.time.count() / seq_spdk.iterations)
              << " | ";

    std::cout << std::setw(2) << par_spdk.buffer_size / par_spdk.chunk_size / 2 << " | ";
    std::cout << std::setw(8) << par_spdk.time.count() / par_spdk.iterations << " ns | ";
    std::cout << std::setw(9)
              << size::to_string(
                     par_spdk.chunk_size *
                         (1000000000 / (par_spdk.time.count() / par_spdk.iterations)),
                     false)
              << "/s | ";
    std::cout << "x " << std::fixed << std::setw(6) << std::setprecision(2)
              << ((double)seq_memcpy.time.count() / seq_memcpy.iterations) /
                     (par_spdk.time.count() / par_spdk.iterations)
              << " |\n";
}
