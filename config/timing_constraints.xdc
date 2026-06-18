create_clock -add -name sys_clk_pin -period 8.00 -waveform {0 4} [get_ports { clk }];
set_input_delay -clock sys_clk_pin -max 2.20 [get_ports {sdr_raw_i[*]}]
set_input_delay -clock sys_clk_pin -min 0.40 [get_ports {sdr_raw_i[*]}]
set_input_delay -clock sys_clk_pin -max 2.20 [get_ports {sdr_raw_q[*]}]
set_input_delay -clock sys_clk_pin -min 0.40 [get_ports {sdr_raw_q[*]}]
set_output_delay -clock sys_clk_pin -max 2.50 [get_ports {ram_data_out[*]}]
