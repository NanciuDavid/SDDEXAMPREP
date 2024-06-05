#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

typedef struct Programare
{
    char *dataProgramare;
    char *numeClient;
    float durata;
    char *serviciiEfectuate;
    int costProgramare;
    struct Programare *next;
} Programare;

typedef struct ListaProgramari
{
    char *dataProgramare;
    struct Programare *node;
    struct ListaProgramari *next;
} ListaProgramari;

typedef struct Stack{
    Programare *programare;
    struct Stack* next;
}Stack;

Stack* createStackNode(Programare *programare) {
    Stack* node = (Stack*)malloc(sizeof(Stack));
    if (!node) {
        printf("Memory could not be allocated...");
        exit(1);
    }
    node->programare = programare;
    node->next = NULL;
    return node;
} 

void pushNode(Stack** stack, Programare* programare) {
    Stack *node = createStackNode(programare);
    if (!(*stack)) {
        *stack = node;
    } else {
        node->next = *stack;
        *stack = node;
    }
}

void printStack(Stack *stack) {
    Stack *current = stack;
    if(current == NULL) {
        printf("Stack is empty");
    }
    while (current) {
        printf("Service: %s\n", current->programare->serviciiEfectuate);
        current = current->next;
    }
}

void saveByServices(Stack **stack, ListaProgramari *list, char *serviciu) {
    if (!list) {
        printf("List is empty..");
        exit(1);
    }
    ListaProgramari *tmp = list;
    while (tmp) {
        Programare *tempProgramare = tmp->node;
        while (tempProgramare) {
            if (strcmp(tempProgramare->serviciiEfectuate, serviciu) == 0) {
                pushNode(stack, tempProgramare);
            }
            tempProgramare = tempProgramare->next;
        }
        tmp = tmp->next;
    }
}



Programare *createProgramare(char *dataProgramare, char *numeClient, float durata, char *serviciiEfectuate, int costProgramare)
{
    Programare *programare = (Programare *)malloc(sizeof(Programare));
    programare->dataProgramare = (char *)malloc(strlen(dataProgramare) + 1);
    strcpy(programare->dataProgramare, dataProgramare);

    programare->numeClient = (char *)malloc(strlen(numeClient) + 1);
    strcpy(programare->numeClient, numeClient);

    programare->durata = durata;

    programare->serviciiEfectuate = (char *)malloc(strlen(serviciiEfectuate) + 1);
    strcpy(programare->serviciiEfectuate, serviciiEfectuate);

    programare->costProgramare = costProgramare;
    programare->next = NULL;

    return programare;
}


ListaProgramari *createListaProgramari(char *dataProgramare)
{
    ListaProgramari *lista = (ListaProgramari *)malloc(sizeof(ListaProgramari));
    if (!lista)
    {
        printf("Error on allocating memory...");
        exit(1);
    }
    lista->dataProgramare = (char *)malloc(strlen(dataProgramare) + 1);
    strcpy(lista->dataProgramare, dataProgramare);
    lista->node = NULL;
    lista->next = NULL;
    return lista;
}


void insertProgramare(ListaProgramari *lista, Programare *programare)
{
    if (lista)
    {
        if (strcmp(lista->dataProgramare, programare->dataProgramare) == 0)
        {
            if (!lista->node)
            {
                lista->node = programare;
            }
            else
            {
                Programare *temp = lista->node;
                while (temp->next)
                {
                    temp = temp->next;
                }
                temp->next = programare;
            }
        }
    }
}



void createLista(ListaProgramari **lista, Programare *programare)
{
    if (!(*lista))
    {
        *lista = createListaProgramari(programare->dataProgramare);
        insertProgramare(*lista, programare);
    }
    else
    {
        ListaProgramari *tmp = *lista;
        ListaProgramari *prev = NULL;
        while (tmp && strcmp(tmp->dataProgramare, programare->dataProgramare) != 0)
        {
            prev = tmp;
            tmp = tmp->next;
        }
        if (tmp)
        {
            insertProgramare(tmp, programare);
        }
        else
        {
            prev->next = createListaProgramari(programare->dataProgramare);
            insertProgramare(prev->next, programare);
        }
    }
}


int countProgramari(ListaProgramari* head, char* data){
    if(!head) {
        printf("Lista este goala...");
        exit(1);
    }
    int count = 0 ;
    ListaProgramari *tmp = head;
    while(tmp) {
        if(strcmp(tmp->dataProgramare, data) == 0) {
            Programare *tempProgramare = tmp->node;
            while(tempProgramare) {
                count++;
                tempProgramare = tempProgramare->next;
            }
        }
        tmp = tmp->next;
    }

    return count;
}


void modifyPret(ListaProgramari *head, char*name, int newPrice) {
    if(!head) {
        printf("Lista este goala...");
        exit(1);
    }
    ListaProgramari *tmp = head;
    while(tmp) {
        Programare *tempProgramare = tmp->node;
        while(tempProgramare) {
            if(strcmp(tempProgramare->numeClient, name) == 0) {
                tempProgramare->costProgramare = newPrice;
            }
            tempProgramare = tempProgramare->next;
        }
        tmp = tmp->next;
    }
}

void deleteLessThan(ListaProgramari *head, float durata) {
    if(!head) {
        printf("Lista este goala...");
        exit(1);
    }
    ListaProgramari *temp = head;
    while(temp) {
        Programare* prevProgramare = NULL;
        Programare* currentProgramare = temp->node;

        while (currentProgramare && currentProgramare->durata < durata) {
            Programare* toDelete = currentProgramare;
            currentProgramare = currentProgramare->next;
            free(toDelete); 
        }

        temp->node = currentProgramare;


        temp = temp->next;
    }
}


int main() {
    ListaProgramari *head = NULL;
    Stack *stack = NULL;
    FILE *fp = fopen("C:\\Users\\nanci\\OneDrive\\Desktop\\SDDEXAMPREP\\2024\\Bilet03\\programari.txt", "r");

    if (!fp) {
        printf("File could not be opened. Check path...");
        exit(1);
    }

    char buffer[256];
    char delimiter[] = ",";
    char *token;

    while (fgets(buffer, sizeof(buffer), fp)) {
        token = strtok(buffer, delimiter);
        char *dataProgramare = (char *)malloc(strlen(token) + 1);
        strcpy(dataProgramare, token);

        token = strtok(NULL, delimiter);
        char *numeClient = (char *)malloc(strlen(token) + 1);
        strcpy(numeClient, token);

        token = strtok(NULL, delimiter);
        float durata = (float)atof(token);

        token = strtok(NULL, delimiter);
        char *serviciiEfectuate = (char *)malloc(strlen(token) + 1);
        strcpy(serviciiEfectuate, token);

        token = strtok(NULL, delimiter);
        int costProgramare = atoi(token);

        Programare *programare = createProgramare(dataProgramare, numeClient, durata, serviciiEfectuate, costProgramare);

        createLista(&head, programare);
    }

    int count = countProgramari(head, "20/10/2024");
    printf("\n%d\n", count);

    fclose(fp);

    modifyPret(head, "David Nanciu", 90);

    // deleteLessThan(head, 2.50);

    saveByServices(&stack, head, " Tuns+Barba");
    printStack(stack);

    ListaProgramari *tmpLista = head;
    while (tmpLista) {
        printf("\nData: %s\n", tmpLista->dataProgramare);
        Programare *tmpProg = tmpLista->node;
        while (tmpProg) {
            printf("    Client: %s, Durata: %.2f ore, Servicii: %s, Cost: %d RON\n",
                   tmpProg->numeClient, tmpProg->durata, tmpProg->serviciiEfectuate, tmpProg->costProgramare);
            tmpProg = tmpProg->next;
        }
        tmpLista = tmpLista->next;
    }

    // Free allocated memory
    while (head) {
        ListaProgramari *tmpLista = head;
        head = head->next;
        while (tmpLista->node) {
            Programare *tmpProg = tmpLista->node;
            tmpLista->node = tmpLista->node->next;
            free(tmpProg->dataProgramare);
            free(tmpProg->numeClient);
            free(tmpProg->serviciiEfectuate);
            free(tmpProg);
        }
        free(tmpLista->dataProgramare);
        free(tmpLista);
    }

    // Free stack memory
    while (stack) {
        Stack *tmpStack = stack;
        stack = stack->next;
        free(tmpStack);
    }

    return 0;
}