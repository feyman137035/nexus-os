FROM debian:12-slim

# Install basic tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    bison \
    flex \
    libgmp3-dev \
    libmpc-dev \
    libmpfr-dev \
    texinfo \
    wget \
    nasm \
    qemu-system-x86 \
    make \
    git \
    && rm -rf /var/lib/apt/lists/*

# Build and install i686-elf-gcc and i686-elf-binutils
WORKDIR /tmp
RUN wget https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.xz && \
    tar xf binutils-2.42.tar.xz && \
    mkdir binutils-build && cd binutils-build && \
    ../binutils-2.42/configure --target=i686-elf --prefix=/usr/local --disable-nls && \
    make -j$(nproc) && make install && \
    cd /tmp && rm -rf binutils-2.42 binutils-build binutils-2.42.tar.xz

RUN wget https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.xz && \
    tar xf gcc-14.2.0.tar.xz && \
    cd gcc-14.2.0 && ./contrib/download_prerequisites && \
    mkdir build && cd build && \
    ../configure --target=i686-elf --prefix=/usr/local --disable-nls --enable-languages=c --without-headers && \
    make -j$(nproc) all-gcc all-target-libgcc && make install-gcc install-target-libgcc && \
    cd /tmp && rm -rf gcc-14.2.0 gcc-14.2.0.tar.xz

# Set working directory
WORKDIR /workspace

CMD ["/bin/bash"]
