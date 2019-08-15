# Syscalls used by malloc

**brk**: brk obtains memory (non zero initialized) from kernel by increasing  
program break location (brk). Initially start (start_brk) and end of heap  
segment (brk) would point to same location.  
1. When ASLR is turned off, start_brk and brk would point to end of data/bss  
segment (end_data).  
2. When ASLR is turned on, start_brk and brk would be equal to end of data/bss  
segment (end_data) plus random brk offset.  

![linux process memory layout](/home/chenzheng/Documents/Note/linuxFlexibleAddressSpaceLayout.png) 

**mmap**: malloc uses mmap to create a private anonymous mapping segment.   
The primary purpose of private anonymous mapping is to allocate new memory  
(zero filled) and this new memory would be exclusively used by calling process.
