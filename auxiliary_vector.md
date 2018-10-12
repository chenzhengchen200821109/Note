# **What is a auxiliary vector?**  
auxiliary vector是一种在内核态和用户态交换信息的机制。  

# **Who construct auxiliary vector and where is it？ ** 
auxiliary vector由内核的加载器在加载可执行文件时构建，构建后位于进程地址空间的特定位置（environ指针变量之上）  
![auxvec](https://github.com/chenzhengchen200821109/Note/blob/master/auxvec.png)  

# **auxiliary vector都有哪些？**  
1. **AT_PHDR and AT_ENTRY**: The values for these keys are the address of the ELF program headers of the executable and the entry address of the executable. The dynamic linker uses this information to perform linking and pass control to the executable.  
2. **AT_SECURE**: The kernel assigns a nonzero value to this key if this executable should be treated securely.
3. **AT_UID, AT_EUID, AT_GID, and AT_EGID**: These are the real and effective user and group IDs of the process. Making these values available in the vector saves the dynamic linker the cost of making system calls to determine the values. If the AT_SECURE value is not available, the dynamic linker uses these values to make a decision about whether to handle the executable securely.  
4. **AT_PAGESZ**: The value is the system page size. The dynamic linker needs this information during the linking phase, and the C library uses it in the implementation of the malloc family of functions.  
5. **AT_PLATFORM**: The value is a pointer to a string identifying the hardware platform on which the program is running.   
6. **AT_SYSINFO_EHDR**: The value is a pointer to the page containing the Virtual Dynamic Shared Object (VDSO) that the kernel creates in order to provide fast implementations of certain system calls.  
7. **AT_HWCAP**: The value is a pointer to a multibyte mask of bits whose settings indicate detailed processor capabilities. This information can be used to provide optimized behavior for certain library functions. The contents of the bit mask are hardware dependent (for example, see the kernel source file arch/x86/include/asm/cpufeature.h for details relating to the Intel x86 architecture).  
8. **AT_RANDOM**: The value is a pointer to sixteen random bytes provided by the kernel. The dynamic linker uses this to implement a stack canary. 
