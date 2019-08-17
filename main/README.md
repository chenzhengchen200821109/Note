# How main() is executed on Linux

## Starting
The question is simple: how does linux execute my main()? Through this  
document, I'll use the following simple C program to illustrate how it  
works. It's called "simple.c"
```
int main()
{
	return 0;
}
```

## Build
```
gcc -o simple simple.c
```


## What's in the executable?
To see what's in the executable, let's use a tool "objdump"   
![objdump1](https://github.com/chenzhengchen200821109/Note/blob/master/main/objdump1.png) 
The output gives us some critical information about the executable.  
 First of all, the file is "ELF32" format. Second of all, the start address is  
 "0x080482b0"   
 
## What's ELF?
ELF is acronym for Executable and Linking Format. It's one of several  
object and executable file formats used on Unix systems. For our  
discussion, the interesting thing about ELF is its header format. Every  
ELF executable has ELF header, which is the following.  

```
typedef struct
{
	unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
	Elf32_Half	e_type;			        /* Object file type */
	Elf32_Half	e_machine;		        /* Architecture */
	Elf32_Word	e_version;		        /* Object file version */
	Elf32_Addr	e_entry;		       	/* Entry point virtual address */
	Elf32_Off	e_phoff;				/* Program header table file offset */
	Elf32_Off	e_shoff;				/* Section header table file offset */
	Elf32_Word	e_flags;				/* Processor-specific flags */
	Elf32_Half	e_ehsize;			   	/* ELF header size in bytes */
	Elf32_Half	e_phentsize;			/* Program header table entry size */
	Elf32_Half	e_phnum;				/* Program header table entry count */
	Elf32_Half	e_shentsize;			/* Section header table entry size */
	Elf32_Half	e_shnum;				/* Section header table entry count */
	Elf32_Half	e_shstrndx;				/* Section header string table index */
} Elf32_Ehdr;
```

In the above structure, there is "e_entry" field, which is starting address of an executable.   

## What's at address "0x080482b0", that is, starting address?
For this question, let's disassemble "simple". There are several tools to disassemble an  
executable. I'll use objdump for this purpose.  
```
objdump  --disassemble simple
```
The output is a little bit long so I'll not paste all the output from objdump. Our intention   
is see what's at address 0x080482b0. Here is the output.  
![objdump2](https://github.com/chenzhengchen200821109/Note/blob/master/main/objdump2.png) 
Looks like some kind of starting routine called "_start" is at the starting  
address. What it does is clear a register, push some values into stack  
and call a function. According to this instruction, the stack frame should  
look like this.  
```
Stack Top	------------------------------------
			0x80483a2 (main)
			------------------------------------
			esi = 0x1
			------------------------------------
			ecx = "simple"
			------------------------------------
			0x80483b0 (__libc_csu_init)
			------------------------------------
			0x8048420 (__libc_csu_fini)
			------------------------------------
			edx = 0xb7fed300 (_dl_fini)
			------------------------------------
			esp = 0xbfffef8c
			------------------------------------
			eax = 0x1c
			------------------------------------
```

# Q2>What's at address 80482a0?
Again, let's look for address 80482a0 from the disassembly output.  
 If you look for it, the assembly is  
 ```
 80482cc:	e8 cf ff ff ff ff call   80482a0 <__libc_start_main@plt>
 ```
 
# More about ELF and dymanic linking
With ELF, we can build an executable linked dynamically with libraries.
Here "linked dynamically" means the actual linking process happens at runtime. Otherwise we'd have to build a huge executable containing all the libraries it calls (a "statically-linked executable). If you issue the command
```
ldd simple
``` 
![objdump3](https://github.com/chenzhengchen200821109/Note/blob/master/main/objdump3.ng) 
You can see all the libraries dynamically linked with simple. And all the dynamically linked data and functions have "dynamic relocation entry". 
The concept is roughly like this.
1. We don't know actual address of a dynamic symbol at link time. We can know the actual  
address of the symbol only at runtime.
2. So for the dynamic symbol, we reserve a memory location for the actual address. The   
memory location will be filled with actual address of the symbol at runtime by loader.  
3. Our application sees the dynamic symbol indirectly with the momeory location by using   
kind of pointer operation. In our case, at address 80482bc, there is just a simple jump instruction.  
And the jump location is stored at address 0x8049548 by loader during runtime.  
We can see all dynamic link entries with objdump command.  
![objdump4](https://github.com/chenzhengchen200821109/Note/blob/master/main/objdump4.png) 
Here address 0x8049620 is called "jump slot", which perfectly makes sense. And according   
to the table, actually we want to call __libc_start_main.  

# What's __libc_start_main?
Now the ball is on libc's hand. __libc_start_main is a function in libc.so.6. If you look for  
__libc_start_main in glibc source code, the prototype looks like this.  
```
extern int BP_SYM (__libc_start_main) (int (*main) (int, char **, char **),
		int argc,
		char *__unbounded *__unbounded ubp_av,
		void (*init) (void),
		void (*fini) (void),
		void (*rtld_fini) (void),
		void *__unbounded stack_end)
__attribute__ ((noreturn));
```
And all the assembly instructions do is set up argument stack and call __libc_start_main.  
What this function does is setup/initialize some data structures/environments and call our   
main(). Let's look at the stack frame with this function prototype.  

```
Stack Top	------------------------------------
			0x80483a2 (main)                   main
			------------------------------------
			esi = 0x1                                 argc
			------------------------------------
			ecx = "simple"                        argv
			------------------------------------
			0x80483b0 (__libc_csu_init)
			------------------------------------
			0x8048420 (__libc_csu_fini)
			------------------------------------
			edx = 0xb7fed300 (_dl_fini)
			------------------------------------
			esp = 0xbfffef8c                      stack_end
			------------------------------------
			eax = 0x1c
			------------------------------------
```
According to this stack frame, esi, ecx, edx, esp, eax registers should be filled with appropriate  
values before __libc_start_main() is executed. And clearly this registers are not set by the startup  
assembly instructions shown before. Then, who sets these registers? Now I guess the only thing  
left. The kernel.  
Now let's go back to our third question.  

# Q3>What does the kernel do?
When we execute a program by entering a name on shell, this is what happens on Linux.  
1. The shell calls the kernel system call "execve" with argc/argv.  
2. The kernel system call handler gets control and start handling the system call. In kernel code,  
the handler is "sys_execve". On x86, the user-mode application passes all required parameters   
to kernel with the following registers.  
```
        ebx : pointer to program name string
        ecx : argv array pointer
        edx : environment variable array pointer. 
```
3. The generic execve kernel system call handler, which is do_execve, is called. What it does is  
set up a data structure and copy some data from user space to kernel space and finally calls  
search_binary_handler(). Linux can support more than one executable file format such as a.out  
and ELF at the same time. For this functionality, there is a data structure "struct linux_binfmt",  
which has a function pointer for each binary format loader. And search_binary_handler() just  
looks up an appropriate handler and calls it. In our case, load_elf_binary() is the handler. To  
explain each detail of the function would be lengthy/boring work. So I'll not do that. If you are  
interested in it, read a book about it. As a picture tells a thousand words, a thousand lines of  
source code tells ten thousand words (sometimes). Here is the bottom line of the function.   
It first sets up kernel data structures for file operation to read the ELF executable image in. Then  
it sets up a kernel data structure: code size, data segment start, stack segment start, etc. And  
it allocates user mode pages for this process and copies the argv and environment variables to  
those allocated page addresses. Finally, argc, the argv pointer, and the envrioronment variable  
array pointer are pushed to user mode stack by create_elf_tables(), and start_thread() starts the  
process execution rolling.  

When the _start assembly instruction gets control of execution, the stack frame looks like this.  
```
Stack Top               ---------------------
                            argc
                        ---------------------
                            argv pointer
                        ---------------------
                            env pointer
                        ---------------------
```

And the assembly instructions gets all information from stack by
```
pop %esi 		 	<--- get argc
move %esp, %ecx		<--- get argv
			            actually the argv address is the same as the current
			            stack pointer.
```

# And now we are all set to start executing.
What about the other registers?
For esp, this is used for stack end in application program. After popping all necessary  
information, the _start rountine simply adjusts the stack pointer (esp) by turning off lower  
4 bits from esp register. This perfectly makes sense since actually, to our main program,   
that is the end of stack. For edx, which is used for rtld_fini, a kind of application destructor,   
the kernel just sets it to 0 with the following macro.  
```
#define ELF_PLAT_INIT(_r)	do { \
	_r->ebx = 0; _r->ecx = 0; _r->edx = 0; \
	_r->esi = 0; _r->edi = 0; _r->ebp = 0; \
	_r->eax = 0; \
} while (0)
```
The 0 means we don't use that functionality on x86 linux.
# About the assembly instructions
Where are all those codes from? It's part of GCC code. You can usually find all the object files  
for the code at /usr/lib/gcc-lib/i386-redhat-linux/XXX and /usr/lib where XXX is gcc version.  
File names are crtbegin.o,crtend.o, gcrt1.o.

# Summing up
Here is what happens. 
1. GCC build your program with crtbegin.o/crtend.o/gcrt1.o And the other default libraries are  
dynamically linked by default. Starting address of the executable is set to that of _start.  
2. Kernel loads the executable and setup text/data/bss/stack, especially, kernel allocate page(s)  
for arguments and environment variables and pushes all necessary information on stack.  
3. Control is pased to _start. _start gets all information from stack setup by kernel, sets up  
argument stack for __libc_start_main, and calls it.  
4. __libc_start_main initializes necessary stuffs, especially C library(such as malloc) and thread  
environment and calls our main.  
5. our main is called with main(argv, argv) Actually, here one interesting point is the signature   
of main. __libc_start_main thinks main's signature as main(int, char **, char **) If you are  
curious, try the following prgram.  
```
int main(int argc, char** argv, char** env)
{
        int i = 0;
        while(env[i] != 0)
        {
           printf("%s\n", env[i++]);
        }
        return(0);
}
```

# Conclusion
On Linux, our C main() function is executed by the cooperative work of  
GCC, libc and Linux's binary loader.  

# Ref
1. objdump                         "man objdump" 
2. ELF header                     /usr/include/elf.h 
3. __libc_start_main          glibc source 
					   ./sysdeps/generic/libc-start.c 
4. sys_execve                    linux kernel source code
                                       	    arch/i386/kernel/process.c
5. do_execve                      linux kernel source code
                                            fs/exec.c
6. struct linux_binfmt         linux kernel source code
                                             include/linux/binfmts.h
7. load_elf_binary               linux kernel source code
                                             fs/binfmt_elf.c
8. create_elf_tables            linux kernel source code
                                             fs/binfmt_elf.c
9. start_thread                    linux kernel source code
                                             include/asm/processor.h 
