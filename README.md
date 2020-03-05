# CaffeineOS
My first OS fueled by caffeine.  

This is a learning project, we will see how far it goes. 
When I say "fueled by caffeine", I literally mean caffeine, not some new OS building framework. But that would be a pretty cool name for an OS building framework.

## Getting Started
I am running this project in Ubuntu18.04 LTS. I am using a remote build using Visual Studio on PC, but you can just use the Makefile directly on a Ubuntu machine. My OS is currently running in a VM in Ubuntu.  I would not recomend running this "on metal" because mistakes can brick the computer by permintaly altering BIOS and ROM data.
The following are the packages I installed in order to get this project going:
 * g++
 * clang
 * make
 * gcc-multilib
 * g++-multilib
 * virtualbox-qt
 * valgrind
 * xorriso

The Makefile will produce an ISO file in the same location with the same name everytime it is run.
In VirtualBox, I made a VM named CaffeineOS with 512MB RAM, no VDK/VMDK and with the optical media pointing at the ISO file and the "Live CD" box checked.  
Even tho the ISO file will change with each build, the VM will always go to the same location to find the ISO and boot from it.  

*****************************************************************************************************************************

## Progress Log  

3/2/2020 1658 PST  
Right now I have a bootable kernal.  It does nothing but print out the OS Name.
So far I have followed totorials, but very few cover anything past this point.  None I found use C++, only C.

I am researching what is the next part of the kernel I should build next. 
My guess is dynamic memory. 
Currently I am achiving output using a VGA address to write text to the screen.  I haven't figured out input yet.
As I said, tutorials dont really get past the bootable "Hello World" OS.


Stardate 3/4/2020 2121 PST 
I have detected the RAM and got values that make sence (low mem ~= 640KB, low+hi ~=512MB), so I am really happy.
I also was able to get a string from the bootloader by casting a uint32_t into a const char* and printing array at the pointer.
I have been using the multiboot.h from the gnu.org site. The multiboot.h files contains emmpty struct and typedef declarations that matches the blocksizes of the info the boot loader (GRUB2.02) sends via the multiboot_info_t*.  By knowing the blocksizes, I can cast the pointers into the data I need from the bootloader.
