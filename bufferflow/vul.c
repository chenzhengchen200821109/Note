//GCC中的Canaries探测
//通过一个例子分析GCC堆栈保护所生成的代码。
//分别使用-fstack-protector选项和-fno-stack-protector
//编译代码得到可执行文件vul_sp(-fstack-protector)，
//vul_nosp(-fno-stack-protector)。

int main()
{
    int i;
    char buffer[64];
    i = 1;
    buffer[0] = 'a';

    return 0;
}
