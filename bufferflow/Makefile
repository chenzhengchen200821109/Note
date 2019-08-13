#

all: vul_sp vul_nosp

vul_sp: vul.c
	gcc83 -g -fstack-protector vul.c -o vul_sp

vul_nosp: vul.c
	gcc83 -g -fno-stack-protector vul.c -o vul_nosp 

clean:
	rm -rf vul_sp vul_nosp

