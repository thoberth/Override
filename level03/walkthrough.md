on a un exécutable qui attend un password sur l'entrée standard

après quelques analyse sur gdb on comprend que le thème est la cryptographie, il faut juste trouver la bonne clé à envoyer sur l'entrée standard

après avoir désasssembler le programme on remarque qu'il y a une string qui est modifié (un xor est effectué sur chaque caractères) avec notre entrée pour ensuite la comparer avec "Congratulations!"

la string équivaut à "Q}|u`sfg~sf{}|a3" au début

```
(gdb) disas decrypt

...

   0x08048673 <+19>:    movl   $0x757c7d51,-0x1d(%ebp) // "Q}|u"
   0x0804867a <+26>:    movl   $0x67667360,-0x19(%ebp) // "`sfg"
   0x08048681 <+33>:    movl   $0x7b66737e,-0x15(%ebp) // "~sf{"
   0x08048688 <+40>:    movl   $0x33617c7d,-0x11(%ebp) // "}|a3"

...

```

donc il faut que "Q}|u`sfg~sf{}|a3" xor _key_ = "Congratulations!"

on trouve que "Q" xor 18 (decimal) = "C"

https://www.compscilib.com/calculate/binaryxor?variation=default

dans le programme la clé est calculé dans _test()_ avec la constante _nb_ = 322424845

```
void	test(int input, int nb) {
	int	result;

	result = nb - input;

...

}
```

pour que la clé soit égale à 18 il faut que _input_ = 322424845 - 18 = 322424827

```
level03@OverRide:~$ ./level03
***********************************
*               level03         **
***********************************
Password:322424827
$ pwd
/home/users/level03
$ whoami
level04
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```
