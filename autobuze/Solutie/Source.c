#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Autobuz {
    unsigned int numarAutobuz;
    char* statiePlecare;
    char* statieCapat;
    int nrStatii;
    int* durataStatie;
} Autobuz;

typedef struct LinkedList {
    struct Autobuz* item;
    struct LinkedList *next;
    struct LinkedList *prev;
} LinkedList;

typedef struct TreeNode {
    Autobuz *item;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *createTreeNode(Autobuz* autobuz) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) {
        printf("Memory could not be allocated...");
        exit(1);
    }
    node->item = autobuz;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode* insertBST(TreeNode** root, Autobuz* autobuz) {
    if (!(*root)) {
        *root = createTreeNode(autobuz);
        return *root;
    }
    
    if ((*root)->item->numarAutobuz < autobuz->numarAutobuz) {
        insertBST(&(*root)->right, autobuz);
    } else if ((*root)->item->numarAutobuz > autobuz->numarAutobuz) {
        insertBST(&(*root)->left, autobuz);
    } else {
        printf("Autobuz already existing...\n");
    }
    return *root;
}

void inorder(TreeNode* root)
{
    if (root)
    {
        inorder(root->left);
        printf("Autobuz: %d, Statia de Plecare: %s, Statia Capat: %s\n",
               root->item->numarAutobuz, root->item->statiePlecare, root->item->statieCapat);
        inorder(root->right);
    }
}

Autobuz* createAutobuz(unsigned int numar, char* statiePlecare, char* statieCapat, int nrStatii, int* durataStatie) {
    Autobuz *autobuz = (Autobuz*)malloc(sizeof(Autobuz));
    if (!autobuz) {
        printf("Error on allocating memory ...");
        exit(1);
    }
    autobuz->numarAutobuz = numar;

    autobuz->statiePlecare = (char*)malloc(strlen(statiePlecare) + 1);
    strcpy(autobuz->statiePlecare, statiePlecare);

    autobuz->statieCapat = (char*)malloc(strlen(statieCapat) + 1);
    strcpy(autobuz->statieCapat, statieCapat);

    autobuz->nrStatii = nrStatii;

    autobuz->durataStatie = (int*)malloc(sizeof(int) * autobuz->nrStatii);
    for (int i = 0; i < nrStatii; i++) {
        autobuz->durataStatie[i] = durataStatie[i];
    }

    return autobuz;    
}

LinkedList *createNode(Autobuz *autobuz) {
    LinkedList *node = (LinkedList*)malloc(sizeof(LinkedList));
    if (!node) {
        printf("Error on allocation memory...");
        exit(1);
    }

    node->item = autobuz;
    node->next = node->prev = NULL;
    return node;
}

void insertNode(LinkedList **head, Autobuz* autobuz) {
    LinkedList* node = createNode(autobuz);
    if ((*head) == NULL) {
        (*head) = node;
    } else {
        LinkedList *tmp = (*head);
        while (tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = node;
        node->prev = tmp;
    }
}

void printAutobuz(Autobuz *autobuz) {
    if (!autobuz) {
        printf("The given parameter does not exist");
        return;
    }
    printf("\nAutobuzul %d, pleaca de la statia %s are capatul la statia %s si  are %d statii cu durata de (min): ", 
        autobuz->numarAutobuz, autobuz->statiePlecare, autobuz->statieCapat, autobuz->nrStatii);
    for (int i = 0; i < autobuz->nrStatii; i++) {
        printf(" %d , ", autobuz->durataStatie[i]);
    }
}

void deleteAutobuz(LinkedList** head, unsigned int id) {
    LinkedList* tmp = *head;

    if (!head) {
        printf("List is empty...");
        exit(1);
    } 
    if (tmp->item->numarAutobuz == id) {
        *head = tmp->next;
        if (*head != NULL) {
            (*head)->prev = NULL;
        }
        free(tmp->item->statiePlecare);
        free(tmp->item->statieCapat);
        free(tmp->item->durataStatie);
        free(tmp->item);
        free(tmp);
        return;
    }
    while (tmp != NULL) {
        if (tmp->item->numarAutobuz == id) {
            if (tmp->next != NULL) {
                tmp->next->prev = tmp->prev;
            }
            if (tmp->prev != NULL) {
                tmp->prev->next = tmp->next;
            }
            free(tmp->item->statiePlecare);
            free(tmp->item->statieCapat);
            free(tmp->item->durataStatie);
            free(tmp->item);
            free(tmp);
            return;
        }
        tmp = tmp->next;
    }
}


int countStangaDreapta(TreeNode *root) {
    int count = 0 ;
    if(root) {
        if(root->left && root->right)
        count++;
        countStangaDreapta(root->left);
        countStangaDreapta(root->right);
    }
    return count;
}
int main() {
    unsigned int numarAutobuz;
    char* statiePlecare = NULL;
    char* statieCapat;
    int nrStatii;
    int* durataStatie;

    char buffer[256];
    char delimiter[] = " ";
    char *token;

    LinkedList *head = NULL;
    TreeNode *root = NULL;

    FILE *fp = fopen("C:\\Users\\nanci\\OneDrive\\Desktop\\SDDEXAMPREP\\autobuze\\Solutie\\autobuz.txt", "r");

    if (!fp) {
        printf("File could not be opened...");
        exit(1);
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        token = strtok(buffer, delimiter);
        numarAutobuz = atoi(token);

        token = strtok(NULL, delimiter);
        if (token == NULL) continue;
        statiePlecare = (char*)malloc(strlen(token) + 1);
        strcpy(statiePlecare, token);

        token = strtok(NULL, delimiter);
        statieCapat = (char*)malloc(strlen(token) + 1);
        strcpy(statieCapat, token);

        token = strtok(NULL, delimiter);
        nrStatii = atoi(token);

        durataStatie = (int*)malloc(sizeof(int) * nrStatii);

        for (int i = 0; i < nrStatii; i++) {
            token = strtok(NULL, delimiter);
            durataStatie[i] = atoi(token);
        }

        Autobuz* autobuz = createAutobuz(numarAutobuz, statiePlecare, statieCapat, nrStatii, durataStatie);
        insertNode(&head, autobuz);

    }

    fclose(fp);

    LinkedList *temp = head;
    deleteAutobuz(&head, 116);
    while (temp) {
        insertBST(&root, temp->item);
        //printAutobuz(temp->item);
        temp = temp->next;
    }

    inorder(root);

    // Free linked list memory
    temp = head;
    while (temp) {
        LinkedList *next = temp->next;
        free(temp);
        temp = next;
    }

    int count = countStangaDreapta(root);
    printf("\n%d", count);
    return 0;
}
