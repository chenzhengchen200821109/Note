# Program Loading  

File Offset   Executable File    Virtual Address
          0  ---------------------
             |    ELF header     |
             ---------------------
             |                   | ---> Program header table
             ---------------------
             | Other information |
       0x100 --------------------- 0x8048100
             |   Text segment    |
             |   0x2be00 bytes   |
     0x2bf00 --------------------- 0x8073eff/0x8074f00
             |    Data segment   |
             |    0x4e00 bytes   |
     0x30d00 --------------------- 0x8079cff
             | Other information |
             |                   |
             ---------------------  
             
# Program header segments  

  | Member    | Text     | Data     |
  |-----------|----------|----------|
  | p_type    | PT_LOAD  | PT_LOAD  |
  |-----------|----------|----------|
  | p_offset  | 0x100    | 0x2bf00  |
  |-----------|----------|----------|
  | p_vaddr   | 0x8048100|0x8074f00 |
  |-----------|----------|----------|
  | p_paddr   | unknown  |unknown   |
  |-----------|----------|----------|
  | p_filesz  | 0x2be00  | 0x4e00   |
  |-----------|----------|----------|
  | p_memsz   | 0x2be00  | 0x5e24   |
  |-----------|----------|----------|
  | p_flags   | R+x      | R+W+X    |
  |-----------|----------|----------|
  | p_align   | 0x1000   | 0x1000   |

