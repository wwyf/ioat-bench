## Prerequisite

Install the dependencies:
* a c++14 compliant c++ compiler
* cmake >= 3.1
* make
* git
* CUnit library
* AIO library
* OpenSSL library

```
sudo apt-get update
sudo apt-get install gcc g++ make cmake git libcunit1-dev libaio-dev libssl-dev

```

Also needed is the latest versions of spdk. The `get_spdk.bash` script can be used to get it.
The script will download the spdk from its official GitHub repository, build it, and install it in `./spdk`.

* `bash ./libs/get_spdk.bash`

## Build

* `mkdir <build-dir>`
* `cd <build-dir>`
* `cmake -DCMAKE_BUILD_TYPE=Release $OLDPWD`
* `make`

## Getting the system ready for SPDK

The following command needs to be run once before running any SPDK application. It should be run as a priviledged user.

* `(cd ./spdk && sudo scripts/setup.sh)`

## Run

* `cd <build-bir>`
* `sudo ./ex4` # The benchmark most likely needs to be run as a priviledged user

## Usage

```
Usage: ./ex4
```
## Example run

[Info   ] Initializing SPDK...
EAL: Detected 56 lcore(s)
EAL: No free hugepages reported in hugepages-1048576kB
EAL: Probing VFIO support...
EAL: PCI device 0000:00:04.0 on NUMA socket 0
EAL:   probe driver: 8086:2f20 spdk_ioat
EAL: PCI device 0000:00:04.2 on NUMA socket 0
EAL:   probe driver: 8086:2f22 spdk_ioat
EAL: PCI device 0000:00:04.3 on NUMA socket 0
EAL:   probe driver: 8086:2f23 spdk_ioat
EAL: PCI device 0000:00:04.4 on NUMA socket 0
EAL:   probe driver: 8086:2f24 spdk_ioat
EAL: PCI device 0000:00:04.5 on NUMA socket 0
EAL:   probe driver: 8086:2f25 spdk_ioat
EAL: PCI device 0000:00:04.6 on NUMA socket 0
EAL:   probe driver: 8086:2f26 spdk_ioat
EAL: PCI device 0000:00:04.7 on NUMA socket 0
EAL:   probe driver: 8086:2f27 spdk_ioat
EAL: PCI device 0000:80:04.0 on NUMA socket 1
EAL:   probe driver: 8086:2f20 spdk_ioat
EAL: PCI device 0000:80:04.2 on NUMA socket 1
EAL:   probe driver: 8086:2f22 spdk_ioat
EAL: PCI device 0000:80:04.3 on NUMA socket 1
EAL:   probe driver: 8086:2f23 spdk_ioat
EAL: PCI device 0000:80:04.4 on NUMA socket 1
EAL:   probe driver: 8086:2f24 spdk_ioat
EAL: PCI device 0000:80:04.5 on NUMA socket 1
EAL:   probe driver: 8086:2f25 spdk_ioat
EAL: PCI device 0000:80:04.6 on NUMA socket 1
EAL:   probe driver: 8086:2f26 spdk_ioat
EAL: PCI device 0000:80:04.7 on NUMA socket 1
EAL:   probe driver: 8086:2f27 spdk_ioat
EAL: PCI device 0000:00:04.0 on NUMA socket 0
EAL:   probe driver: 8086:2f20 spdk_ioat
EAL: PCI device 0000:00:04.2 on NUMA socket 0
EAL:   probe driver: 8086:2f22 spdk_ioat
EAL: PCI device 0000:00:04.3 on NUMA socket 0
EAL:   probe driver: 8086:2f23 spdk_ioat
EAL: PCI device 0000:00:04.4 on NUMA socket 0
EAL:   probe driver: 8086:2f24 spdk_ioat
EAL: PCI device 0000:00:04.5 on NUMA socket 0
EAL:   probe driver: 8086:2f25 spdk_ioat
EAL: PCI device 0000:00:04.6 on NUMA socket 0
EAL:   probe driver: 8086:2f26 spdk_ioat
EAL: PCI device 0000:00:04.7 on NUMA socket 0
EAL:   probe driver: 8086:2f27 spdk_ioat
EAL: PCI device 0000:80:04.0 on NUMA socket 1
EAL:   probe driver: 8086:2f20 spdk_ioat
EAL: PCI device 0000:80:04.2 on NUMA socket 1
EAL:   probe driver: 8086:2f22 spdk_ioat
EAL: PCI device 0000:80:04.3 on NUMA socket 1
EAL:   probe driver: 8086:2f23 spdk_ioat
EAL: PCI device 0000:80:04.4 on NUMA socket 1
EAL:   probe driver: 8086:2f24 spdk_ioat
EAL: PCI device 0000:80:04.5 on NUMA socket 1
EAL:   probe driver: 8086:2f25 spdk_ioat
EAL: PCI device 0000:80:04.6 on NUMA socket 1
EAL:   probe driver: 8086:2f26 spdk_ioat
EAL: PCI device 0000:80:04.7 on NUMA socket 1
EAL:   probe driver: 8086:2f27 spdk_ioat
|-----------------------|---------------------------|--------------------------------------|-------------------------------------------|
| Chunk     / Buffer    | Memcpy                    | SPDK (sequential)                    | SPDK (n tasks submitted at once)          |
| size      / Size      | Time        | Speed       | Time        | Speed       | Improv.  | n  | Time        | Speed       | Improv.  |
|-----------------------|-------------|-------------|-------------|-------------|----------|----|-------------|-------------|----------|
|       2 B /      32 B |       26 ns |  73.4 MiB/s |      628 ns |   3.0 MiB/s | x   0.04 |  8 |      288 ns |   6.6 MiB/s | x   0.09 |
|       8 B /      32 B |       24 ns | 317.9 MiB/s |      628 ns |  12.1 MiB/s | x   0.04 |  2 |      436 ns |  17.5 MiB/s | x   0.06 |
|       8 B /     512 B |       24 ns | 317.9 MiB/s |      631 ns |  12.1 MiB/s | x   0.04 | 32 |      252 ns |  30.3 MiB/s | x   0.10 |
|      32 B /     512 B |       25 ns |   1.2 GiB/s |      629 ns |  48.5 MiB/s | x   0.04 |  8 |      289 ns | 105.6 MiB/s | x   0.09 |
|     128 B /     512 B |       28 ns |   4.3 GiB/s |      675 ns | 180.8 MiB/s | x   0.04 |  2 |      476 ns | 256.5 MiB/s | x   0.06 |
|     128 B /   8.0 kiB |       28 ns |   4.3 GiB/s |      669 ns | 182.5 MiB/s | x   0.04 | 32 |      255 ns | 478.7 MiB/s | x   0.11 |
|     512 B /   8.0 kiB |       32 ns |  14.9 GiB/s |      739 ns | 660.7 MiB/s | x   0.04 |  8 |      308 ns |   1.5 GiB/s | x   0.11 |
|   2.0 kiB /   8.0 kiB |       83 ns |  23.0 GiB/s |      951 ns |   2.0 GiB/s | x   0.09 |  2 |      646 ns |   3.0 GiB/s | x   0.13 |
|   2.0 kiB / 128.0 kiB |       95 ns |  20.1 GiB/s |      949 ns |   2.0 GiB/s | x   0.10 | 32 |      383 ns |   5.0 GiB/s | x   0.25 |
|   8.0 kiB / 128.0 kiB |      318 ns |  24.0 GiB/s |     1839 ns |   4.1 GiB/s | x   0.17 |  8 |     1276 ns |   6.0 GiB/s | x   0.25 |
|  32.0 kiB / 128.0 kiB |     1475 ns |  20.7 GiB/s |     6101 ns |   5.0 GiB/s | x   0.24 |  2 |     5780 ns |   5.3 GiB/s | x   0.26 |
|  32.0 kiB /   2.0 MiB |     2478 ns |  12.3 GiB/s |     6014 ns |   5.1 GiB/s | x   0.41 | 32 |     5560 ns |   5.5 GiB/s | x   0.45 |
| 128.0 kiB /   2.0 MiB |    10027 ns |  12.2 GiB/s |    22774 ns |   5.4 GiB/s | x   0.44 |  8 |    22347 ns |   5.5 GiB/s | x   0.45 |
| 512.0 kiB /   2.0 MiB |    42505 ns |  11.5 GiB/s |    89535 ns |   5.5 GiB/s | x   0.47 |  2 |    89328 ns |   5.5 GiB/s | x   0.48 |
| 512.0 kiB /  32.0 MiB |    44173 ns |  11.1 GiB/s |    95309 ns |   5.1 GiB/s | x   0.46 | 32 |    93473 ns |   5.2 GiB/s | x   0.47 |
|   2.0 MiB /  32.0 MiB |   176335 ns |  11.1 GiB/s |   381207 ns |   5.1 GiB/s | x   0.46 |  8 |   373533 ns |   5.2 GiB/s | x   0.47 |
|   8.0 MiB /  32.0 MiB |   697918 ns |  11.2 GiB/s |  1719498 ns |   4.5 GiB/s | x   0.41 |  2 |  1483958 ns |   5.3 GiB/s | x   0.47 |
|   8.0 MiB / 512.0 MiB |  1097400 ns |   7.1 GiB/s |  1918314 ns |   4.1 GiB/s | x   0.57 | 32 |  1926736 ns |   4.1 GiB/s | x   0.57 |
|  32.0 MiB / 512.0 MiB |  4407167 ns |   7.1 GiB/s |  7655206 ns |   4.1 GiB/s | x   0.58 |  8 |  7661259 ns |   4.1 GiB/s | x   0.58 |
| 128.0 MiB / 512.0 MiB | 18520951 ns |   6.6 GiB/s | 30704649 ns |   4.0 GiB/s | x   0.60 |  2 | 30958922 ns |   4.0 GiB/s | x   0.60 |
