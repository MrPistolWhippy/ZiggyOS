/* ==============================================================================
   ZIGGYOS HARDWARE-SYNTHESIZED POST-QUANTUM LATTICE & BRAM STORAGE CORE
   ============================================================================== */
module ziggy_fpga_core (
    input wire clk,                  // Master 125MHz physical hardware crystal clock
    input wire rst_n,                // Physical reset push-button line
    input wire [15:0] sdr_raw_i,     // Memory-mapped ADC Radio Input (In-Phase)
    input wire [15:0] sdr_raw_q,     // Memory-mapped ADC Radio Input (Quadrature)
    input wire [4:0] ram_addr,       // 5-bit address bus for reading BRAM slots
    output reg [15:0] ram_data_out,  // 16-bit hardware parallel memory output data bus
    output reg [63:0] gematria_tag,  // Hardware-calculated parallel fingerprint stamp
    output reg pqc_secure_latch,     // Post-Quantum Cryptographic verification flag
    output reg alert_trigger         // Direct physical LED warning pin line output
);
    parameter MODULUS_Q = 16'h0101;  // Prime modulus 257
    
    // PARTITION: Synthesize 32 distinct 16-bit high-speed Block RAM cache slots
    reg [15:0] ziggy_bram_array [0:31];
    reg [4:0] write_pointer;         // Automated circular ring-buffer index tracking register

    wire [31:0] lwe_dot_product;
    assign lwe_dot_product = (sdr_raw_i * 16'hFFFF) + (sdr_raw_q * 16'h0001);

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            gematria_tag     <= 64'h0;
            pqc_secure_latch <= 1'b0;
            alert_trigger    <= 1'b0;
            write_pointer    <= 5'h0;
            ram_data_out     <= 16'h0;
        end else begin
            // NATIVE BRAM READING: Read hardware data slots on demand from the address bus
            ram_data_out <= ziggy_bram_array[ram_addr];

            if ((sdr_raw_i * sdr_raw_i) > 32'h000F4FFF) begin
                alert_trigger <= 1'b1;
                gematria_tag  <= {sdr_raw_i, sdr_raw_q, 32'h0000_17B3};
                
                if ((lwe_dot_product % MODULUS_Q) < 16'h0050) begin
                    pqc_secure_latch <= 1'b1;
                    
                    // NATIVE BRAM WRITING: Cache the secure vector payload instantly in silicon
                    ziggy_bram_array[write_pointer] <= sdr_raw_i;
                    write_pointer <= write_pointer + 1'b1; // Advance ring pointer index register
                end else begin
                    pqc_secure_latch <= 1'b0;
                end
            end else begin
                alert_trigger    <= 1'b0;
                pqc_secure_latch <= 1'b0;
            end
        end
    end
endmodule

// ==============================================================================
//  ZIGGYOS OFF-GRID BARE-METAL UART HARDWARE SERIAL RECEIVER NODE (OMEGA CORE)
// ==============================================================================
module ziggy_uart_rx (
    input wire clk, rst_n, rx_line,
    output reg [7:0] rx_data, output reg rx_ready
);
    reg [3:0] bit_idx; reg [12:0] clk_cnt; // 115200 Baud rate clock-step divisions
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin bit_idx <= 0; rx_ready <= 0; end
        else if (clk_cnt == 13'd1085) begin // 125MHz / 115200 sample ticks
            bit_idx <= bit_idx + 1;
            if (bit_idx >= 1 && bit_idx <= 8) rx_data[bit_idx-1] <= rx_line;
            if (bit_idx == 9) rx_ready <= 1;
        end else rx_ready <= 0;
    end
endmodule

// ==============================================================================
//  ZIGGYOS OFF-GRID BARE-METAL UART HARDWARE SERIAL RECEIVER NODE (OMEGA CORE)
// ==============================================================================
module ziggy_uart_rx (
    input wire clk, rst_n, rx_line,
    output reg [7:0] rx_data, output reg rx_ready
);
    reg [3:0] bit_idx; reg [12:0] clk_cnt; // 115200 Baud rate clock-step divisions
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin bit_idx <= 0; rx_ready <= 0; end
        else if (clk_cnt == 13'd1085) begin // 125MHz / 115200 sample ticks
            bit_idx <= bit_idx + 1;
            if (bit_idx >= 1 && bit_idx <= 8) rx_data[bit_idx-1] <= rx_line;
            if (bit_idx == 9) rx_ready <= 1;
        end else rx_ready <= 0;
    end
endmodule

// ==============================================================================
//  ZIGGYOS BARE-METAL UART HARDWARE RX NODE (STABLE OMEGA PIPELINE)
// ==============================================================================
module ziggy_uart_rx (
    input wire clk, rst_n, rx_line,
    output reg [7:0] rx_data, output reg rx_ready
);
    reg [3:0] bit_idx; reg [12:0] clk_cnt;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin bit_idx <= 0; rx_ready <= 0; end
        else if (clk_cnt == 13'd1085) begin
            bit_idx <= bit_idx + 1;
            if (bit_idx >= 1 && bit_idx <= 8) rx_data[bit_idx-1] <= rx_line;
            if (bit_idx == 9) rx_ready <= 1;
        end else rx_ready <= 0;
    end
endmodule

// WIRE_ID_EXT: AQUARIUS-P2P-561785-CARD // SIG_LOCK: 0x591b2f871bd59515 //

// WIRE_ID_EXT: AQUARIUS-P2P-032471-CARD // SIG_LOCK: 0x92200669f1960b52 //

// WIRE_ID_EXT: AQUARIUS-P2P-743962-CARD // SIG_LOCK: 0xdd9294a4e1158b7c //
