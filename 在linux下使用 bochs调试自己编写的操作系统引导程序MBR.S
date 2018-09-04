在linux下使用 bochs调试自己编写的操作系统引导程序MBR.S
 
编译器 ：gcc 汇编器：as 链接器：ld 二进制操作工具：objcopy
 
调试 步骤：
1.使用bximag生成虚拟硬盘：bximage -hd=60 -mode=create -q hd60M.img
2.汇编： as -o mbr.o mbr.S
3.链接：ld -N -e start -Ttext 0x7c00 -s -o mbr.bin mbr.o
4.去除目标文件mbr.bin中不需要的信息（调试信息，文件头等）：objcopy -S -O binary mbr.bin
5.将得到的二进制文件mbr.bin复制到虚拟硬盘hd60M.img: dd if=/home/chenzheng/Documents/github-OS/AS/mbr.bin 
                      of=/home/chenzheng/Documents/github-OS/AS/hd60M.img bs=512 count=1 conv=notrunc
6.运行bochs： bochs -f bochsrc.disk
