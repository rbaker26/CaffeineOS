GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -std=c++11 -lstdc++
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o kernel.o
%.o: %.cpp
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: %.s
	as $(ASPARAMS) -o $@ $<
	
kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: kernel.bin
	sudo cp $< /boot/kernel.bin
	
kernel.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout-0' >> iso/boot/grub/grub.cfg
	echo 'set default-0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "CaffeineOS.Kernel"{' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo ' boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso 

run: kernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm "CaffeineOS.Kernel" &

clean:
	rm *.o
	rm *.bin
	rm *.iso
