#include "../lib/user.h"

#define MAX_ARG_NUM (32)
#define MAX_STR_LEN (200)

const unsigned char LF = 0x0a;
const unsigned char CR = 0x0d;
const unsigned char DL = 0x7f;
const unsigned char BS = 0x08;

char line[MAX_STR_LEN];
char *argv[MAX_ARG_NUM];

int top = 0;

void push(char c) {
    line[top++] = c;
}

void pop() {
    --top;
}

int is_empty() {
    return top == 0;
}

void clear() {
    top = 0;
}

void get_argv() {
    int argc = 0;
    char *s  = line;
    for (;;) {
        while (*s == ' ') {
            *(s++) = 0;
        }
        if (*s) {
            argv[argc++] = s;
        } else {
            argv[argc] = NULL;
            return;
        }
        while (*s && *s != ' ') s++;
    }
}

void parse_argv() {
    if (strncmp(line, "quit", sizeof("quit")) == 0) {
        exit(0);
    }
    get_argv();
}

char *prompt = "sh >> ";

int main() {
    stdout_nobuf();
    static char buf[200];
    static char *argv[32];
    while (1) {
        printf("%s", prompt);
        gets(buf, sizeof(buf));
        // remove the last \r or \n
        int len = strlen(buf);
        if (len > 0 && (buf[len - 1] == CR || buf[len - 1] == LF)) {
            buf[len - 1] = 0;
        }
        if (strncmp(buf, "quit", 4) == 0) {
            exit(0);
        }
        int argc = 0;
        char *s  = buf;
        for (;;) {
            while (*s == ' ') {
                *(s++) = 0;
            }
            if (*s) {
                argv[argc++] = s;
            } else {
                argv[argc] = NULL;
                break;
            }
            while (*s && *s != ' ') {
                s++;
            }
        }
        int pid = fork();
        if (pid == 0) {
            exec(argv[0], argv);
            printf("sh > exec %s failed\n", argv[0]);
            exit(-1);
        } else {
            int code;
            wait(pid, &code);
            printf("sh > child %d exit with code %d\n", pid, code);
        }
    }
    return 0;
}
