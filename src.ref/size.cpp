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


#include "size.hpp"
#include "utils.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

uint64_t size::from_string(std::string str, bool force_binary)
{
    std::size_t digits = 0;

    auto value = std::stoull(str, &digits);

    auto              suffix  = str.substr(digits);
    uint8_t           exp     = 0;
    bool              decimal = true;
    std::vector<char> prefixes{'K', 'M', 'G', 'T'};

    auto i = 0u;
    for (; i < suffix.size(); ++i)
    {
        char c = utils::to_upper(suffix[i]);

        for (uint8_t j = 0; j < prefixes.size(); ++j)
        {
            if (c == prefixes[j])
            {
                exp = j + 1;
                break;
            }
        }
        if (exp != 0)
            break;
    }

    if (exp == 0)
        return value;

    ++i;
    if (i < suffix.size())
    {
        if (force_binary || suffix[i] == 'i')
            decimal = false;
    }

    return value * (utils::int_pow(decimal ? 1000 : 1024, exp));
}

std::string size::to_string(uint64_t bytes, bool decimal)
{
    std::stringstream s;

    auto base = decimal ? 1000u : 1024u;

    if (bytes < base)
    {
        s << bytes << " B";
        return s.str();
    }

    std::vector<std::string> prefixes = {"k", "M", "G", "T"};

    int         exp  = (int)(log(bytes) / log(base));
    std::string unit = prefixes[exp - 1] + (decimal ? "B" : "iB");

    s << std::fixed << std::setprecision(1) << bytes / pow(base, exp) << " " << unit;
    return s.str();
}
