# CS220 Course Project: Custom MIPS Processor & FPGA Accelerators

This repository contains the hardware designs and software drivers developed for the **CS220 (Computer Organization)** final project. The project is divided into two core architectures: a custom 32-bit MIPS processor and a suite of high-performance FPGA hardware accelerators.

---

## 1. 32-Bit Multi-Cycle MIPS Processor (`Mini-MIPS-Processor/`)

Engineered a 32-bit MIPS processor from scratch in **Verilog HDL**. To optimize clock frequency and mitigate critical-path timing violations inherent in single-cycle designs, the processor utilizes a custom **3-cycle Finite State Machine (FSM) multi-cycle architecture**.

### Key Architectural Features:
* **Custom Execution Unit:** Implements a robust ALU supporting standard MIPS arithmetic, bitwise logic, and barrel-shifting instructions.
* **Storage & Memory Interfaces:** Features a dual-read asynchronous register file and strict compliance with Big-Endian byte ordering for comprehensive load/store instructions (`lw`, `lb`, `lh`, `lbu`, `lhu`, `sw`, `sb`, `sh`).
* **System Call Integration:** Embedded custom syscall decoding logic to interface directly with peripheral keyboard input and display hardware.
* **Validation & Testing:** Validated architectural correctness via targeted MIPS assembly programs (located in the `assembly/` subdirectory), executing mathematical algorithms such as Taylor series approximations and numerical root-finding.

---

## 2. FPGA Hardware Accelerators (`FPGA-Accelerator/`)

Developed dedicated Verilog datapaths to offload and accelerate computationally intensive mathematical algorithms, shifting execution from a sequential software execution model to high-throughput concurrent hardware.

### Accelerated Modules:
* **Vector Addition & Reduction (`vector_add.v`):** Computes parallel vector addition and reduction for **512-dimensional vectors**, achieving a **150x to 200x execution speedup** compared to sequential processing on the native ARM CPU core.
* **Matrix-Vector Multiplication (`mat_vec_mul.v`):** Computes the product of a $16 \times 16$ matrix and a 16-element vector by leveraging concurrent **hardware reduction trees** to minimize adder latency.
* **Graph Path Finder (`path_finder.v`):** Computes length-$k$ paths between vertices in a graph using hardware-accelerated matrix multiplication. For $k = 31$, this design yielded a massive **~1000x latency reduction** over the ARM processor implementation.

---

## 3. Heterogeneous SoC Integration (`Vitis-Drivers/`)

To evaluate real-world performance, the hardware accelerators were packaged as **AXI4 memory-mapped IPs** within Xilinx Vivado and integrated alongside a Xilinx Zynq ARM processing system. 

The `Vitis-Drivers/` directory contains the embedded C codebase executed on the ARM core. These drivers orchestrate data transfers over the AXI4 interconnect, configure the accelerator IP control registers, and profile exact hardware cycle counts to measure benchmark speedups empirically.
