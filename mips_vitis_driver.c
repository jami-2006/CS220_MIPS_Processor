#include <stdio.h>
#include "xil_io.h"

#define BASE_ADDR 0x43C00000 

int main() {
    unsigned reset = 1, ins_addr, done_storing = 0, done;
    unsigned total_cycles, proc_cycles;
    unsigned waiting_for_input, input_value_valid = 0;
    int input_value;
    int out[4];
    unsigned flush_io_regs, done_copying_io_regs = 0, print_count, print_out_count = 1;

    Xil_Out32(BASE_ADDR, reset);
    Xil_Out32(BASE_ADDR + 3, done_storing);
    Xil_Out32(BASE_ADDR + 4, done_copying_io_regs);
    Xil_Out32(BASE_ADDR + 6, input_value_valid);

    reset = 0;
    Xil_Out32(BASE_ADDR, reset);

    done_storing = 1;
    Xil_Out32(BASE_ADDR + 3, done_storing);

    while (1) {
        flush_io_regs = 0;
        done = 0;
        waiting_for_input = 0;

        while (!flush_io_regs && !done && !waiting_for_input) {
            flush_io_regs = Xil_In32(BASE_ADDR + 14);
            done = Xil_In32(BASE_ADDR + 7);
            waiting_for_input = Xil_In32(BASE_ADDR + 16);
        }

        if (waiting_for_input) {
            printf("Enter input:\n");
            scanf("%d", &input_value);
            Xil_Out32(BASE_ADDR + 5, input_value);
            input_value_valid = 1;
            Xil_Out32(BASE_ADDR + 6, input_value_valid);
            input_value_valid = 0;
            Xil_Out32(BASE_ADDR + 6, input_value_valid);
        }

        if (flush_io_regs) {
            out[0] = Xil_In32(BASE_ADDR + 8);
            out[1] = Xil_In32(BASE_ADDR + 9);
            out[2] = Xil_In32(BASE_ADDR + 10);
            out[3] = Xil_In32(BASE_ADDR + 11);

            done_copying_io_regs = 1;
            Xil_Out32(BASE_ADDR + 4, done_copying_io_regs);
            done_copying_io_regs = 0;
            Xil_Out32(BASE_ADDR + 4, done_copying_io_regs);

            for (int i=0; i<4; i++) {
                printf("out%d=%d\n", print_out_count, out[i]);
                print_out_count++;
            }
        }

        if (done) {
            print_count = Xil_In32(BASE_ADDR + 15);
            int i = 8;
            while (print_count > 0) {
                out[0] = Xil_In32(BASE_ADDR + i);
                printf("out%d=%d\n", print_out_count, out[0]);
                print_count--;
                print_out_count++;
                i++;
            }
            break;
        }
    }

    total_cycles = Xil_In32(BASE_ADDR + 12);
    proc_cycles = Xil_In32(BASE_ADDR + 13);
    printf("Total cycles: %u, computation cycles: %u\n", total_cycles, proc_cycles);

    return 0;
}
