const std = @import("std");
const UART0_DATA: *volatile u32 = @ptrFromInt(0x4000C000);
const UART0_IER: *volatile u32 = @ptrFromInt(0x4000C004);
const UART0_IIR: *volatile u32 = @ptrFromInt(0x4000C008);
const ICSR: *volatile u32 = @ptrFromInt(0xE000ED04);
const SHPR3: *volatile u32 = @ptrFromInt(0xE000ED1C);
const NVIC_ISER0: *volatile u32 = @ptrFromInt(0xE000E100);

pub const TCB = extern struct { sp: ?*volatile u32, id: u32, state: u32, execution_count: u32, sleep_ticks: u32 };
export var os_tasks: [2]TCB = undefined;
export var current_task_ptr: ?*volatile TCB = null;
export var current_task_index: u32 = 0;
var T1_Stk: [128]u32 align(8) = undefined;
var T2_Stk: [128]u32 align(8) = undefined;
export var tick_counter: u32 = 0;
export var trig_sin_out: i32 = 0;
export var decrypted_stream_key: u32 = 0;
export var task_weights: [2]u32 = .{ 1, 3 };
export var current_weight_counter: u32 = 0;
export var handshake_authenticated: u32 = 0;
var cmd_buffer: [16]u8 = undefined; var cmd_idx: u32 = 0;

const DbRecord = extern struct { id: u32, key: [12]u8, val: [16]u8 };
export var ziggy_db: [4]DbRecord = undefined;

const UartBuffer = struct {
    data: [16]u8 = undefined, head: u32 = 0, tail: u32 = 0,
    pub fn write(self: *UartBuffer, char: u8) void {
        const next = (self.head + 1) % 16; if (next != self.tail) { self.data[self.head] = char; self.head = next; }
    }
    pub fn read(self: *UartBuffer) ?u8 {
        if (self.head == self.tail) return null; const char = self.data[self.tail]; self.tail = (self.tail + 1) % 16; return char;
    }
};
var ring_buffer = UartBuffer{};
fn printStr(str: []const u8) void { for (str) |char| { UART0_DATA.* = @intCast(char); } }
pub export fn panic_dump(fault_stack: [*]u32) void {
    printStr("\n!!! KERNEL PANIC !!!\nPC = 0x"); var val = fault_stack[6]; var buf: u8 = undefined; var i: usize = 8;
    while (i > 0) { i -= 1; const nibble: u8 = @intCast(val & 0xF); buf[i] = if (nibble < 10) ('0' + nibble) else ('A' + nibble - 10); val >>= 4; }
    printStr(&buf); while(true) {}
}
fn printHex(val_in: u32) void {
    var val = val_in; var buf:u8 = undefined; var i: usize = 8;
    while (i > 0) { i -= 1; const nibble: u8 = @intCast(val & 0xF); buf[i] = if (nibble < 10) ('0' + nibble) else ('A' + nibble - 10); val >>= 4; }
    printStr(&buf);
}
fn dbSearch(query: []const u8) i32 {
    var i: usize = 0;
    while (i < 4) : (i += 1) {
        const k = &ziggy_db[i].key; var m: bool = true; var j: usize = 0;
        while (j < query.len and j < 12) : (j += 1) { if (k[j] != query[j]) { m = false; break; } }
        if (m and (j == query.len or k[j] == 0)) return @intCast(i);
    }
    return -1;
}
fn osYield() void { if (current_task_ptr) |ptr| { ptr.execution_count += 1; } ICSR.* = (1 << 28); asm volatile ("isb"); }
fn osSleep(ticks: u32) void { if (current_task_ptr) |ptr| { ptr.sleep_ticks = ticks; ptr.state = 2; } osYield(); }
pub export fn UART0_Handler() void { if ((UART0_IIR.* & 0x0E) == 0x04) { ring_buffer.write(@intCast(UART0_DATA.* & 0xFF)); } }
fn fixedSin(x_in: i32) i32 {
    var x = x_in; if (x > 3141) x -= 6283; if (x < -3141) x += 6283;
    const num = 16 * x * (3141 - if (x < 0) -x else x); const den = 5 * 3141 * 3141 - 4 * x * (3141 - if (x < 0) -x else x); return @divTrunc(num, @divTrunc(den, 1000));
}
fn task1TrigGenerator() callconv(.C) void { var angle: i32 = 0; while (true) { trig_sin_out = fixedSin(angle); angle = (angle + 100) % 6283; osSleep(5); } }
fn processPacketFrame(pkt: [*]u8, len: u32) void { if (len < 4 or pkt[0] != 0xAA) return; var checksum: u8 = 0; var i: u32 = 0; while (i < (pkt[1] + 2)) : (i += 1) { checksum ^= pkt[i]; } if (checksum == pkt[pkt[1] + 2]) { printStr("\n[FRAME SUCCESS]: Checksum Clear\n"); } }
fn task2TrigDecrypt() callconv(.C) void {
    var pkt_buf: [16]u8 = undefined; var p_idx: u32 = 0;
    while (true) {
        if (handshake_authenticated == 0) {
            if (ring_buffer.read()) |key| { if (key == 'Z') handshake_authenticated = 1; }
        } else {
            decrypted_stream_key = @bitCast(trig_sin_out ^ 0x5AA5A5A5);
            if (ring_buffer.read()) |ch| { if (p_idx < 16) { pkt_buf[p_idx] = ch; p_idx += 1; if (p_idx >= (pkt_buf[1] + 3) and pkt_buf[0] == 0xAA) { processPacketFrame(&pkt_buf, p_idx); p_idx = 0; } } }
        }
        osYield();
    }
}
pub export fn SysTick_Handler() void {
    tick_counter += 1; current_weight_counter += 1;
    var i: usize = 0; while (i < 2) : (i += 1) { if (os_tasks[i].state == 2) { if (os_tasks[i].sleep_ticks > 0) { os_tasks[i].sleep_ticks -= 1; } else { os_tasks[i].state = 0; } } }
    if (current_weight_counter >= task_weights[current_task_index]) {
        current_weight_counter = 0; const next_idx = (current_task_index + 1) % 2;
        if (task_weights[current_task_index] > 1) task_weights[current_task_index] -= 1;
        if (os_tasks[next_idx].state == 0 or os_tasks[next_idx].state == 1) { os_tasks[current_task_index].state = 0; current_task_index = next_idx; current_task_ptr = &os_tasks[current_task_index]; current_task_ptr.?.state = 1; ICSR.* = (1 << 28); }
    }
}
fn initStk(t: *TCB, stk_array: []u32, func: *const fn () callconv(.C) void, id: u32) void {
    var s: [*]u32 = @ptrCast(&stk_array[stk_array.len]);
    s -= 1; s[0] = 0x01000000; s -= 1; s[0] = @intCast(@intFromPtr(func)); s -= 1; s[0] = 0xFFFFFFFD;
    var i: u32 = 0; while (i < 13) : (i += 1) { s -= 1; s[0] = 0; }
    t.sp = @ptrCast(s); t.id = id; t.state = 0; t.execution_count = 0; t.sleep_ticks = 0;
}
pub export fn Reset_Handler() void {
    SHPR3.* |= 0x00FF0000; initStk(&os_tasks[0], &T1_Stk, task1TrigGenerator, 1); initStk(&os_tasks[1], &T2_Stk, task2TrigDecrypt, 2);
    current_task_index = 0; current_task_ptr = &os_tasks[0]; current_task_ptr.?.state = 1;
    UART0_IER.* |= 0x01; NVIC_ISER0.* |= (1 << 24);
    asm volatile ("ldr r0, =current_task_ptr \n ldr r1, [r0] \n ldr sp, [r1] \n pop {r4-r11} \n pop {r0-r3, r12, lr} \n pop {r4} \n pop {r5} \n cpsie i \n bx r4" : : : "memory");
    while (true) {}
}

// Action 1 & 2: Automated Idle Thread and Performance Logging
fn taskIdleBackground() callconv(.C) void { while (true) { asm volatile ("wfi"); } }
pub export fn dump_metrics() void { printStr("\n--- DIAGS ---\nTicks: "); printHex(tick_counter); }
