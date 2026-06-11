const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.resolveTargetQuery(.{
        .cpu_arch = .thumb,
        .os_tag = .freestanding,
        .abi = .none,
        .cpu_model = .{ .explicit = &std.Target.arm.cpu.cortex_m4 },
    });

    const optimize = b.standardOptimizeOption(.{ .default_optimize_mode = .ReleaseFast });

    const elf = b.addExecutable(.{
        .name = "ziggyos.elf",
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });

    // Add your legacy C implementation components cleanly
    elf.addCSourceFile(.{ .file = b.path("src/vector_table.c"), .flags = &.{ "-O2", "-Wall" } });
    elf.addAssemblyFile(b.path("src/context.s"));
    elf.setLinkerScript(b.path("linker.ld"));

    // Extract the raw bare-metal binary from the ELF artifact
    const bin = elf.addObjCopy(.{ .format = .bin });
    const install_bin = b.addInstallBinFile(bin.getOutput(), "ziggyos.bin");
    
    b.getInstallStep().dependOn(&install_bin.step);
    b.installArtifact(elf);
}
