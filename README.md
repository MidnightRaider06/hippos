# hippos
🦛hippOS is a hobby OS kernel that I will attempt to implement from scratch during Summer 2025 as a learning experience. I will document my progress over the summer in this README file including any challenges I face and resources I use.

## 6/13/25
I finished following the Bare Bones tutorial on OSDev Wiki to write a simple kernel for 32-bit x86. I got familiar with setting up a cross-compiler, using it to assemble object files and link them into a final kernel, and testing an OS using QEMU. I still have to figure out my next steps with this project, including how a project like this should be properly structured, what my target platform should be (32-bit or 64-bit), and the features I should prioritize first.

![Screenshot 2025-06-13 200743](https://github.com/user-attachments/assets/7a087974-e19a-4b87-95ed-2ba2394c16d8)

Resources:
- https://wiki.osdev.org/Bare_Bones

## 6/18/25
I finished following the Meaty Skeleton tutorial on OSDev Wiki, which is a continuation of the Bare Bones tutorial. It is a template operating system that serves as an example of how to structure the project, including things like a freestanding kernel C library, arch directories for architecture-dependent code, and different Makefiles and build scripts. I plan on taking some time to study this structure and all the code in it so I know what's going on instead of blindly copying and pasting without learning what the different parts do. I've found some resources to help me figure out some next steps, including another OSDev Wiki article and a YouTube playlist by Daedalus Community that looks fairly promising for figuring out general next steps.

Resources:
- https://wiki.osdev.org/Meaty_Skeleton
- https://wiki.osdev.org/Going_Further_on_x86
- https://youtu.be/MwPjvJ9ulSc?feature=shared

## 6/21/25
I spent some time looking at all the Meaty Skeleton code trying to understand what each part does and how they interact with each other. I'd say I have a much better understanding of it than before, although some things are a bit fuzzy (I get it for the most part though). After watching some YouTube videos and reading the "Going Further on x86" article linked below, it seems like creating a GDT is a logical next step. After that, I'll most likely work on interrupts, paging, and getting keyboard input in some order (not too sure yet). I've decided to build on top of the Meaty Skeleton tutorial code instead of starting everything from scratch because this way, I have some foundation instead of going in completely blind which might be counterproductive for a beginner to learn. From my understanding, this is a good way to organize the project and I can always rewrite any parts if I need to.

Resources:
- https://wiki.osdev.org/Going_Further_on_x86
- https://youtu.be/MwPjvJ9ulSc?feature=shared
- https://wiki.osdev.org/Global_Descriptor_Table

## 6/30/25
I have been working on setting up the GDT and reading the OSDev Wiki pages closely. Initially, I got this error message: 

#### Booting from hard disk. Boot failed: could not read the boot disk. Booting from floppy. Boot failed: could not read the boot disk. Booting from DVD/CD

I figured out that the issue was somewhere in my gdt_flush assembly function, which is supposed to flush the GDT set up by GRUB and replace it with my own. I did some research on how to use GDB with the kernel and thankfully there was an OSDev Wiki article. I looked closely at the GDT tutorial page and used GDB to make sure the bits were set correctly for each entry in the table. After making sure there weren't any issues with my gdt_entry struct and how I was setting its fields, I continued using GDB to find the issue. It turns out that I forgot a dollar sign so instead of moving the immediate value 0x10 to the %eax register, I moved the contents of memory address 0x10 to %eax. This messed up the values in the segment registers because I used %eax to set %ds, %es, %fs, %gs, and %ss. The GDT seems to be working properly now after adding the dollar sign. My next step is to start researching and working on the IDT.

Resources:
- https://wiki.osdev.org/Global_Descriptor_Table
- https://wiki.osdev.org/GDT_Tutorial
- https://wiki.osdev.org/GDB
