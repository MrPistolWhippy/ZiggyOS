FROM alpine:3.19
RUN apk update && apk add --no-cache bash python3 build-base git gcc-riscv64-none-elf binutils-riscv64-none-elf
WORKDIR /root
RUN mkdir -p "/root/iSh files/mesh_node_backup"
COPY . /root/
ENTRYPOINT ["/bin/bash", "-c", "source /root/.profile && /root/mesh_omni_core.sh --daemon"]
