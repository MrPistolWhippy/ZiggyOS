const std = @import("std");
pub fn build(b: *std.Build) void {
    const k = b.addExecutable(.{
        .name = "kernel.bin", .root_source_file = b.path("main.zig"),
        .target = b.resolveTargetQuery(.{.cpu_arch = .x86, .os_tag = .freestanding, .abi = .none}),
        .optimize = b.standardOptimizeOption(.{}),
    });
    k.root_module.strip = true; k.setLinkerScript(b.path("linker.ld"));
    b.installArtifact(k);
}
