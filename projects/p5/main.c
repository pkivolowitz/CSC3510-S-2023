#include <stdio.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char ** argv) {
    int retval = 1;
    char * dirname = ".";
    char *fmt = "%-20llu 0x%02x %c %s\n";

    if (argc > 1)
        dirname = argv[1];

    DIR * dir = opendir(dirname);
    if (dir) {
        struct dirent * de;
        errno = 0;
        while ((de = readdir(dir)) != NULL) {
            char symbol;
            switch (de->d_type) {

            case DT_CHR:
                symbol = 'C';
                break;

            case DT_DIR:
                symbol = 'D';
                break;

            case DT_FIFO:
                symbol = 'P';
                break;

            case DT_LNK:
                symbol = 'L';
                break;

            case DT_REG:
                symbol = 'O';
                break;

            case DT_SOCK:
                symbol = 'S';
                break;

            default:
                symbol = '?';
                break;
            }
            printf(fmt, de->d_ino, de->d_type, symbol, de->d_name);
        }
        if (errno != 0)
            perror("readdir() failed");
        closedir(dir);
        retval = (errno != 0); // force error return to be 1
    }
    else
        perror(dirname);
    return retval;
}