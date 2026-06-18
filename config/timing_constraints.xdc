## ==============================================================================
##  ZIGGYOS CORE SILICON TIMING MAP & CONSTRAINT RULES (125 MHz OSCILLATOR)
## ==============================================================================

# 1. Define Primary Master Clock (Period: 8.00ns = 125 MHz target frequency)
create_clock -add -name sys_clk_pin -period 8.00 -waveform {0 4} [get_ports { clk }];

# 2. Establish Hardware Input Delay Guardrails (Max propagation path: 2.5ns)
set_input_delay -clock sys_clk_pin -max 2.50 [get_ports {sdr_raw_i[*]}]
set_input_delay -clock sys_clk_pin -max 2.50 [get_ports {sdr_raw_q[*]}]

# 3. Establish Hardware Output Delay Guardrails (Prevent race conditions on ports)
set_output_delay -clock sys_clk_pin -max 3.00 [get_ports {gematria_tag[*]}]
set_output_delay -clock sys_clk_pin -max 2.00 [get_ports {pqc_secure_latch}]
set_output_delay -clock sys_clk_pin -max 2.00 [get_ports {alert_trigger}]

# 4. Enforce High-Priority Placement Routing Optimizations
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
set_property CONFIG_MODE SPIx4 [current_design]
