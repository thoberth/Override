#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void    clear_stdin() {
    unsigned char   c = 0;

    while ((c = getchar())) {
        if (c == 10 || c == 255)
            break ;
    }
}

int     get_unum() {
    unsigned int    n = 0;

    fflush(stdout);
    scanf("%u", &n);
    clear_stdin();
    return (n);
}

int     store_number(unsigned int *tab) {
    unsigned int    number = 0;
    unsigned int    index = 0;

    printf(" Number: ");
    number = get_unum();
    printf(" Index: ");
    index = get_unum();
    if ((index % 3) == 0 || number >> 24 == 183) {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return (1);
    }
    index *= 4;
    tab += index;
    *tab = number;
    return (0);
}

int     read_number(unsigned int *tab) {
    unsigned int    index = 0;
    
    printf(" Index: ");
    index = get_unum();
    tab += (index * 4);
    printf(" Number at data[%u] is %u\n", index, *tab);
    return (0);
}

int     main(int ac, char** av, char** ev) {
    unsigned int    tab[100];
    char            input[20];
    int             ret = 0;

    (void)ac;
    while (*av) {
        memset(*av, 0, strlen(*av));
        av++;
    }
    while (*ev) {
        memset(*ev, 0, strlen(*ev));
        ev++;
    }
    puts("----------------------------------------------------\n  Welcome to wil\'s crappy number storage service!   \n----------------------------------------------------\n Commands:                                          \n    store - store a number into the data storage    \n    read  - read a number from the data storage     \n    quit  - exit the program                        \n----------------------------------------------------\n   wil has reserved some storage :>                 \n----------------------------------------------------\n");
    while (1) {
        printf("Input command: ");
        ret = 1;
        fgets(input, 20, stdin);
        if (strncmp(input, "store", 5) == 0)
            ret = store_number(tab);
        else if (strncmp(input, "read", 4) == 0)
            ret = read_number(tab);
        else if (strncmp(input, "quit", 4) == 0)
            return (0);
        if (ret == 0)
            printf(" Completed %s command successfully\n", input);
        else
            printf(" Failed to do %s command\n", input);
    }
    return (0);
}