# Base image leveraging enterprise-grade lightweight Alpine Linux
FROM alpine:3.19

# 1. Install necessary core toolchains, cross-compilers, and dependencies
RUN apk update && apk add --no-cache \
    bash \
    python3 \
    build-base \
    git \
    gcc-riscv64-none-elf \
    binutils-riscv64-none-elf

# 2. Structure the internal immutable workspace paths
WORKDIR /root
RUN mkdir -p "/root/iSh files/mesh_node_backup"

# 3. Inject our verified system modules straight into the build context
COPY mesh_omni_core.sh enterprise_omni_core.sh swarm_parser.py /root/
COPY riscv_watchdog.s riscv_driver.c /root/
COPY .profile /root/.profile

# 4. Assert absolute execution permissions across all automated blocks
RUN chmod +x /root/*.sh /root/*.py

# Establish persistent daemon monitoring on container initial startup lifecycle
ENTRYPOINT ["/bin/bash", "-c", "source /root/.profile && /root/mesh_omni_core.sh --daemon"]
