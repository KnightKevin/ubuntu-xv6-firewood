# initDir = temp-env-init
# rm -rf $initDir
# mkdir -p $initDir
# cd $initDir
# wget https://mirrors.tuna.tsinghua.edu.cn/gnu/gdb/gdb-9.2.tar.gz
# apt-get update
# apt-get install -y libncurses5-dev python python-dev texinfo libreadline-dev
# apt install -y libgmp-dev
# tar -zxvf gdb-9.2.tar.gz
# cd gdb-9.2
# mkdir build
# cd build
# ../configure --prefix=/usr/local --with-python=/usr/bin/python --target=riscv64-unknown-elf --enable-tui=yes
# make clean
# make -j$(nproc)
# make install
