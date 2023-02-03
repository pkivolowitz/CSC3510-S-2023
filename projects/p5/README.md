# Reading Directories

The program will use library calls to open the given Linux directory (or
the current directory if no command line argument is given) for reading.
It will read every directory entry, printing out each file's inode
number, numerical type, symbolic type and name.

*Note that the Macintosh implementation of `dirent` is different from
that of Linux.*

## Samples

### Output when no command line argument is given

```text
% ./a.out               
11468143             0x04 D .
1833880              0x04 D ..
11468217             0x08 O README.md
51723738             0x08 O a.out
11468218             0x08 O main.c
11468214             0x08 O .gitignore
11468220             0x08 O project.s
11468144             0x04 D .git
11468215             0x04 D .vscode
% 
```

The first column is the named file's inode number. Think of an inode
number as a file's unique (per file system) serial number. You must
print it left justified in a field of 20 digits using `printf`.

Since it is possible you've never used `printf` before, I will supply
the correct template string in the C version of the program presented
below.

The second column is the file's type printed as a single byte's worth of
hex.

The third column is a single character chosen to symbolically match the
value in the second column.

The fourth column is the file's name.

### Output when a command line argument is given

```text
% ./a.out /usr/share/vim
1152921500312447599  0x04 D .
1152921500312429992  0x04 D ..
1152921500312447600  0x04 D vim90
1152921500312451414  0x08 O vimrc
% 
```

It says that `.`, `..` and `vim90` are directories and that `vimrc` is
an ordinary file.

### Output when a bad command line argument is given

```text
% ./a.out nibble  
nibble: No such file or directory
% 
```

The error string is produced by `perror()`.

## man

Here is where you will get documentation for `perror()`, `opendir()`,
`closedir()`, and `readdir()`. The man page for `readdir()` also
describes `struct dirent`.

```text
man perror
man opendir
man closedir
man readdir
```

`man` is your friend, though of course in the 21st century it should be
called `person`. To learn more about `man`, do the obvious thing:

```text
man man
```

**FOR THIS PROJECT DON'T USE MAN FROM A MAC TERMINAL -- WHY? STEVE
FRIGGEN JOBS THAT'S WHY.**

It will be equally pointless to try the above Linux shell commands from
a Windows command prompt but hey - give it a try.

The reason to not read the `man` pages from the Mac is that everything
beyond the name of the functions you need will be different. You know,
"Think Different."

## `opendir()`

This function takes a NULL terminated `C-string` and attempts to open it
as a directory. Get the details from the `man` page. If you get an error
return, pass the attempted directory name to `perror()` to get the right
error message.

## `closedir()`

Call this function to close a successfully opened directory. Get the
details from the `man` page.

## `readdir()`

Call this function to be given a pointer to the next `dirent` or `NULL`
if there are no more (or there is an error). Pay attention to the `man`
page to distinguish between no more `dirent` structures and an error. In
short, `errno` should be initialized to 0 then checked once you've
gotten a `NULL` back from `readdir()`.

## Source code to a `C` version

Here is the source code to my `C` version.

```c
#include <stdio.h>                                                  // 1 
#include <errno.h>                                                  // 2 
#include <dirent.h>                                                 // 3 
                                                                    // 4 
int main(int argc, char ** argv) {                                  // 5 
    int retval = 1;                                                 // 6 
    char * dirname = ".";                                           // 7 
    char *fmt = "%-10llu 0x%02x %c %s\n";                           // 8 
                                                                    // 9 
    if (argc > 1)                                                   // 10 
        dirname = argv[1];                                          // 11 
                                                                    // 12 
    DIR * dir = opendir(dirname);                                   // 13 
    if (dir) {                                                      // 14 
        struct dirent * de;                                         // 15 
        errno = 0;                                                  // 16 
        while ((de = readdir(dir)) != NULL) {                       // 17 
            char symbol;                                            // 18 
            switch (de->d_type) {                                   // 19 
                                                                    // 20 
            case DT_CHR:                                            // 21 
                symbol = 'C';                                       // 22 
                break;                                              // 23 
                                                                    // 24 
            case DT_DIR:                                            // 25 
                symbol = 'D';                                       // 26 
                break;                                              // 27 
                                                                    // 28 
            case DT_FIFO:                                           // 29 
                symbol = 'P';                                       // 30 
                break;                                              // 31 
                                                                    // 32 
            case DT_LNK:                                            // 33 
                symbol = 'L';                                       // 34 
                break;                                              // 35 
                                                                    // 36 
            case DT_REG:                                            // 37 
                symbol = 'O';                                       // 38 
                break;                                              // 39 
                                                                    // 40 
            case DT_SOCK:                                           // 41 
                symbol = 'S';                                       // 42 
                break;                                              // 43 
                                                                    // 44 
            default:                                                // 45 
                symbol = '?';                                       // 46 
                break;                                              // 47 
            }                                                       // 48 
            printf(fmt, de->d_ino, de->d_type, symbol, de->d_name); // 49 
        }                                                           // 50 
        if (errno != 0)                                             // 51 
            perror("readdir() failed");                             // 52 
        closedir(dir);                                              // 53 
        retval = (errno != 0); // force error return to be 1        // 54 
    }                                                               // 55 
    else                                                            // 56 
        perror(dirname);                                            // 57 
    return retval;                                                  // 58 
}                                                                   // 59 
```

## Getting the address of `errno`

`errno` is an `extern`. To store anything into it (or query its
contents), you must have its address. For reasons which will not be
explained, getting its address is accomplished by calling a [library
function](http://refspecs.linux-foundation.org/LSB_4.0.0/LSB-Core-generic/LSB-Core-generic/baselib---errno-location.html).

## Remember to properly set the return value of `main()`

If all ends well, zero should be returned from `main()`. If any error is
found, a value of 1 should be returned.

Check in this way:

```text
echo $?
```

`$?` is a shell variable that contains the value returned from the last
program run by the shell.

## Setting the symbol to its correct value

The above source code indicates the valid symbols as various cases of
the `switch` statement. You can implement the `switch` as a series of if
statements (in assembly language, of course) or you can do it in a
relatively tiny number of statements using an array or rather, a
C-string 13 bytes long (not including a null terminator).

My specifying the exact length of the C-string is in fact, a strong hint
as to how to go about this.

*Those who correctly make use of the C-string method will be awarded
five points above their project grade (i.e. extra credit).*

## Likely source of error

If you're printing garbage, double check your calculations of offsets
within the `dirent`. While this isn't the only explanation, it is a
likely explanation.

## What to turn in

Turn in only your nicely commented assembly language program.

## Work Rules

All work is to be done solo.
