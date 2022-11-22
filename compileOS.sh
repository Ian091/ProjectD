# Overwites diskc so no not enough disk space errors happen.
# It would happen without overwriting after compiling enough times.
dd if=/dev/zero of=diskc.img bs=512 count=1000
nasm bootload.asm
dd if=bootload of=diskc.img bs=512 bs=512 count=1 conv=notrunc

# Makes kernel so loadFile has a kernel to load.
bcc -ansi -c -o kernel_c.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel_c.o kernel_asm.o

as86 userlib.asm -o userlib.o

bcc -c -ansi -o tstpr1.o tstpr1.c
ld86 -d -o tstpr1 tstpr1.o userlib.o

bcc -c -ansi -o tstpr2.o tstpr2.c
ld86 -d -o tstpr2 tstpr2.o userlib.o

bcc -c -ansi -o shell.o shell.c
ld86 -d -o shell shell.o userlib.o

gcc -o loadFile loadFile.c
./loadFile kernel
./loadFile message.txt
./loadFile tstpr1
./loadFile tstpr2
./loadFile shell
