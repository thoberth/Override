# Search

```
$ ls -l
-rwsr-s---+ 1 level07 users 7907 Sep 10  2016 level06

$ ./level06 
***********************************
*               level06           *
***********************************
-> Enter Login: 1212
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 1212
```

# analyze with gdb

```

(gdb) info function
0x08048748  auth
0x08048879  main

```

## main

main function ask for ```login``` and ```serial``` than put them to parameters to auth function and call ```system('/bin/sh')``` if auth return values is 0.

## auth

- verify if login should be > 5
- use ptrace to keep an eye of tampering (using gdb while execution)
- create a hash with the given login
- then compare the given serial with the hash of the login
- if serial != hash return 1 else return 0

# modify return value of ptrace to have the value of the hash

```
$ gdb -q level06 

(gdb) break *0x080487ba
(gdb) break *0x08048866
(gdb) r
Starting program: /home/users/level06/level06 
***********************************
*               level06           *
***********************************
-> Enter Login: thoberth 
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 42

Breakpoint 1, 0x080487ba in auth ()
(gdb) set $eax=0
(gdb) info reg eax
eax            0x0      0
(gdb) c
Continuing.
Breakpoint 2, 0x08048866 in auth ()
(gdb) x/d $ebp-0x10
0xffffd698:     6231516
```

#### we have the value of our hash for ```thoberth``` so we can now login
```
level06@OverRide:~$ ./level06 
***********************************
*               level06           *
***********************************
-> Enter Login: thoberth
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6231516
Authenticated!
$ whoami  
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```