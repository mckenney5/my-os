myos.bin: src/kernel.c boot.o src/include/stdio.h src/include/vga.h
	# Compile and Link and Runs the Kernel
	i686-elf-gcc -c src/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	# Run the kernel directly in QEMU
	qemu-system-i386 -kernel myos.bin

boot.o: src/boot.asm
	# Make the boot program that sets up memory
	nasm -felf32 src/boot.asm -o boot.o

iso: myos.bin
	# Creates and runs the ISO
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
	qemu-system-i386 -cdrom myos.iso

clean: 
	rm -v kernel.o
	rm -v myos.bin

clean-all: 
	rm -v kernel.o
	rm -v boot.o
	rm -v myos.bin
	rm -v myos.iso
