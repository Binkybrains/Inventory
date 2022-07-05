/*
* Nelly Duke
*/

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct itemNode itemNode ;

struct itemNode {
        char id[16];
        unsigned short qty;
        char desc[31];
        itemNode *next;
};

// makeInventory - makes the inventory linked list based on file
void makeInventory(FILE* f);

// add - adds <count> to <id>'s quantity
void add(char* command);

// remove - removes <count> to <id>'s quantity
void removeQty(char* command);

// print - pretty-prints the inventory to stdout
void prettyPrint();

itemNode* inventory = NULL;
size_t inventorySize = 0;

void makeInventory(FILE* f) {

        char *buff = NULL;
        size_t len = 0;

        while(getline(&buff, &len, f) > 1) {

                char line[100];
                sscanf(buff, "%[^\n]%*c\n", line);

                char *token = strtok(line, ":");
                char *id = token;

                token = strtok(NULL, ":");
                char *qty = token;

                if (token == NULL) {
                        fprintf(stderr, "Missing quantity category in file. Exiting.\n");
                        exit(1);
                }

                token = strtok(NULL, ":");

                char *desc = token;

                if (token == NULL) {
                        fprintf(stderr, "Missing description category in file. Exiting.\n");
                        exit(1);
                }

                token = strtok(NULL, ":");

                if (token != NULL) {
                        fprintf(stderr, "Too many data categories in file. Exiting\n");
                        exit(1);
                }

                if (strlen(id) > 15) {
                        fprintf(stderr, "ID from file is too long. Exiting\n");
                        exit(1);
                }

                int isdigit = 0;

                for (int i = 0; i < strlen(qty); i++) {
                        if(!(isdigit(qty[i]))) {
                                isdigit++;
                        }
                }

                if (isdigit != 0) {
                        fprintf(stderr, "Quantity in file contains a non-number. Exiting");
                        exit(1);
                }

                itemNode* node = (itemNode*) malloc(sizeof(itemNode));

                strcpy(node->id, id);
                strcpy(node->desc, desc);

                node->qty= atoi(qty);
                node->next= inventory;

                inventory= node;
                inventorySize++;
        }
}

void add(char* command) {

        int found = 0;
        char *token = strtok(command, ":");
        char *id = token;

        token = strtok(NULL, ":");

        if (token == NULL) {
                printf("NAK Incorrect Syntax.\n");
                return;
        } else if (id[0] != '<' | token[strlen(token)-1] != '>') {
                printf("NAK Incorrect Syntax.\n");
                return;
        }

        memmove(id, id+1, strlen(id)); //removing '<'

        token[strlen(token)-1] = '\0'; //removing '>'

        int count = atoi(token);

        itemNode* head = inventory;

        while( inventory != NULL) {
                if (strcmp(inventory->id, id)==0) {
                        inventory->qty += count;
                        found = 1;
                }
                inventory=inventory->next;
        }

        if (found == 0) {
                printf("NAK Unknown Item\n");
                inventory = head;
                return;
        }

        inventory = head;
        printf("ACK\n");
}


void removeQty(char* command) {
        int found = 0;
        char *token = strtok(command, ":");
        char *id = token;

        token = strtok(NULL, ":");

        if (token == NULL) {
                printf("NAK Incorrect Syntax.\n");
                return;
        } else if (id[0] != '<' | token[strlen(token)-1] != '>') {
                printf("NAK Incorrect Syntax.\n");
                return;
        }

        memmove(id, id+1, strlen(id)); //removing '<'
        token[strlen(token)-1] = '\0'; //removing '>'

        int count = atoi(token);
        itemNode* head = inventory;

        while (inventory != NULL) {

                if (strcmp(inventory->id, id)==0) {
                        if (inventory->qty < count) {
                                printf("NAK Insufficient Quantity\n");
                                return;
                        }
                        inventory->qty -= count;
                        found = 1;
                }
                inventory = inventory->next;
        }

        if (found == 0) {
                printf("NAK Unknown Item\n");
                inventory=head;
                return;
        }

        inventory = head;
        printf("ACK\n");
}

void prettyPrint() {
        printf("  %-24s       QTY     %-11s\n", "DESC", "ID");
        printf("------------------------------  -----  ---------------\n");
        itemNode* temp = inventory;
        while (temp != NULL) {
                printf("%-30s  ", temp->desc);
                printf("%5d  ", temp->qty);
                printf("%-15s", temp->id);

                temp = temp->next;
                printf("\n");
        }
}

int main(int argc, char **argv)  {

        FILE *fin = stdin;


        if (argc>1) {
                fin = fopen(argv[1], "r");
                if (fin == NULL) {
                       fprintf(stderr, "Couldn't open %s for reading. Exiting.\n ", argv[1]);
                        exit(1);
                }
        } else {
                fprintf(stderr, "Incorrect number of arguments, Exiting.\n");
                        exit(1);
        }


        makeInventory(fin);
        fclose(fin);

        char buff[100];
        char *command;
        printf("%% ");

        while (scanf("%[^\n]%*c", buff) != EOF) {
                command = strdup(buff);
                char *token = strtok(command, " ");
                char *com = token;

                token = strtok(NULL, " ");

                if (strcmp(com, "quit") != 0 && strcmp(com, "add") != 0 && strcmp(com, "remove") != 0 && strcmp(com, "print")!= 0) {
                        printf("Unrecognized Command\n");

                } else {
                        if (token == NULL) {

                                if (strcmp(com, "quit") == 0) {
                                        exit(0);
                                        break;

                                } else if (strcmp(com, "print") == 0) {
                                        prettyPrint();
                                } else {
                                        printf("NAK Incorrect Syntax\n");
                                }
                        } else {
                                char* secondArgument = token;
                                token = strtok(NULL, " ");

                                if (token != NULL) {
                                        printf("NAK Incorrect syntax\n");
                                } else {
                                        if (strcmp(com, "add") == 0) {
                                                add(secondArgument);
                                        } else {
                                                removeQty(secondArgument);
                                        }
                                }

                        }
                }
                printf("%% ");
        }

        return 0;
}







