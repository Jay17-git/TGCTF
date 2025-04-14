#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char name[0xd0];
int order_count = 0;
void *chunk_list[0x100];
void mneu()
{
    puts("1. new something");
    puts("2. delete something");
    puts("3. change your name");
    puts("4. exit");
}

unsigned int get_order_index() {
    for (int i = 0; i < 0x10; i++) {
        if (chunk_list[i] == NULL) {
            return i;
        }
    }
    return -1;
}

void add() {
    int size;
    int idx = get_order_index();
    if (idx < 0) {
        puts("Out of space!");
        exit(-1);
    }
    puts("size?");
    printf("> ");
    scanf("%d", &size);
    if (size > 0x80||size<=0) {
        puts("Too big(small)!");
        exit(-1);
    }
    chunk_list[idx] = malloc(size);
    puts("else?");
    printf("> ");
    read(0, chunk_list[idx], size);
    puts("good!");
}

void delete() {
    int index;  
    puts("Do you want delete?");
    printf("> ");
    scanf("%d", &index);
    if (index >= 0x10||index<0) {
        puts("Invalid index!");
        exit(-1);
    }
    free(chunk_list[index]);
    puts("finish");
}

void change() {
    puts("change your name?");
    printf("> ");
    read(0, name, 0xd0);
    printf("Ah, right! nice to see you %s!\n", name);
}

int main(int argc, char **argv) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    puts("Welcome to TGCTF 2025!");
    puts("What's your name?");
    printf("> ");
    read(0, name, 0xd0);
    while (1) {
        mneu();
        printf("> ");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                add();
                break;
            case 2:
                delete();
                break;
            case 3:
                change();
                break;
            case 4:
                puts("bye bye");
                exit(0);
            default:
                exit(0);
        }
    }
}
