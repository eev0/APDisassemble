// APDisassemble.c - "disassemble" Apple's APTicket
//
// by evo on 07/12/15
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int fileExists(char *filename);

int out2Text() {
    FILE *fp = fopen("apticket.dis.txt", "ab+");
    FILE *ap;
    ap = popen("openssl asn1parse -inform DER -in apticket.der", "r");
    if (!ap) {
        return -1;
    }
    char    buffer[256];
    size_t chread;
    size_t comalloc = 256;
    size_t comlen   = 0;
    char *comout    = malloc(comalloc);
    while ((chread = fread(buffer, 1, sizeof(buffer),ap)) != 0) {
        if (comlen + chread >= comalloc) {
            comalloc *= 2;
            comout = realloc(comout, comalloc);
        }
        memmove(comout + comlen, buffer, chread);
        comlen += chread;
    }
    fwrite(comout, 1, comlen, fp);
    free(comout);
    pclose(ap);
    printf("APTicket disassembled in apticket.dis.txt file.\n");
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("usage: %s [OPTIONS] FILE\n",argv[0]);
        printf("Options:\n-p\t\t\tPrint output to terminal\n");
        printf("-t\t\t\tStore output in apticket.dis.txt file\n");
        return 0;
    }
    int opt = 0;
    if (!fileExists ("apticket.der")) {
        printf("Incorrect usage.\n");
        printf("Execute %s in a directory that has an apticket.der.\n", argv[0]);
        return -1;
    }
    
    while ((opt = getopt(argc, argv, "pt")) != -1) {
        switch (opt) {
                case 'p':
                    system("openssl asn1parse -inform DER -in apticket.der");
                    break;
                case 't':
                    out2Text();
                    break;
                default:
                    printf("usage: %s [OPTIONS] FILE\n",argv[0]);
                    printf("Options:\n-p\t\t\tPrint output to terminal\n");
                    printf("-t\t\t\tStore output in apticket.dis.txt file\n");
                    break;
        }
    }
    return 0;
}

int fileExists(char *filename) {
    struct stat     buffer;
    return (stat (filename, &buffer) == 0);
}
