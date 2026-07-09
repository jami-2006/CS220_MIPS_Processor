module MatrixVectorAccelerator(
    input clk,
    input [3:0] row_idx,
    input [3:0] col_idx,
    input [31:0] val_in,
    input is_matrix,
    input input_done,
    input start_arm,
    input stop_arm,
    input [3:0] read_row_idx, 
    output [31:0] y_out,
    output done,
    output [31:0] arm_cycles_out
);

    reg [31:0] M [0:15][0:15];
    reg [31:0] X [0:15];
    reg [31:0] Y [0:15];

    reg [31:0] prod [0:15];

    reg [3:0] state = 4'd0;
    reg [3:0] current_row = 4'd0;
    reg [31:0] arm_cycles = 32'd0;
    reg is_done = 1'b0;

    assign arm_cycles_out = arm_cycles;
    assign done = is_done;
    assign y_out = Y[read_row_idx]; 

    always @(posedge clk) begin
        if (state == 4'd0) begin
            if (input_done) state <= 4'd1;
            current_row <= 4'd0;
        end
        else if (state == 4'd1) state <= 4'd2; 
        else if (state == 4'd2) state <= 4'd3; 
        else if (state == 4'd3) state <= 4'd4; 
        else if (state == 4'd4) state <= 4'd5; 
        else if (state == 4'd5) state <= 4'd6; 
        else if (state == 4'd6) begin
            Y[current_row] <= prod[0];         
            current_row <= current_row + 1;    
            state <= 4'd7;
        end
        else if (state == 4'd7) begin
            if (current_row == 4'd0) state <= 4'd8; 
            else state <= 4'd1; 
        end
        else if (state == 4'd8) begin
            is_done <= 1'b1;
            if (start_arm && !stop_arm) arm_cycles <= arm_cycles + 1;
        end
    end

    integer r, c;
    always @(posedge clk) begin
        if (state == 4'd0 && !input_done) begin
            if (is_matrix) M[row_idx][col_idx] <= val_in;
            else X[row_idx] <= val_in;
        end
    end

    genvar i;
    generate 
        for (i = 0; i < 16; i = i + 1) begin : compute_math
            always @(posedge clk) begin
                if (state == 4'd1) begin
                    prod[i] <= M[current_row][i] * X[i]; 
                end
                else if (state == 4'd2 && i < 8) begin
                    prod[i] <= prod[2*i] + prod[2*i+1];
                end
                else if (state == 4'd3 && i < 4) begin
                    prod[i] <= prod[2*i] + prod[2*i+1];
                end
                else if (state == 4'd4 && i < 2) begin
                    prod[i] <= prod[2*i] + prod[2*i+1];
                end
                else if (state == 4'd5 && i == 0) begin
                    prod[i] <= prod[2*i] + prod[2*i+1];
                end
            end
        end
    endgenerate

endmodule
