#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Angajat
{
    // mandatory field
    unsigned int cod_departament;

    // chosen fields
    char *nume;
    char *prenume;
    char *adresa;
    int varsta;
    int salariu;
    struct Angajat *next;
} Angajat;

typedef struct Departament
{
    char *numeDepartament;
    struct Angajat *angajati;
    struct Departament *next;
} Departament;

typedef struct Queue
{
    Angajat *item;
    struct Queue *next;
} Queue;

Angajat *createAngajat(unsigned int cod_departament, char *nume, char *prenume, char *adresa, int varsta, int salariu)
{
    Angajat *angajat = (Angajat *)malloc(sizeof(Angajat));
    angajat->cod_departament = cod_departament;
    angajat->nume = (char *)malloc(strlen(nume) + 1);
    strcpy(angajat->nume, nume);
    angajat->prenume = (char *)malloc(strlen(prenume) + 1);
    strcpy(angajat->prenume, prenume);
    angajat->adresa = (char *)malloc(strlen(adresa) + 1);
    strcpy(angajat->adresa, adresa);
    angajat->varsta = varsta;
    angajat->salariu = salariu;

    angajat->next = NULL;

    return angajat;
}

Departament *createDepartament(char *numeDepartament)
{
    Departament *departament = (Departament *)malloc(sizeof(Departament));
    departament->numeDepartament = (char *)malloc(strlen(numeDepartament) + 1);
    strcpy(departament->numeDepartament, numeDepartament);
    departament->angajati = NULL;
    departament->next = NULL;

    return departament;
}

void insertAngajati(Departament *department, Angajat *angajat)
{
    if (department)
    {
        if (department->angajati == NULL)
        {
            department->angajati = angajat;
        }
        else
        {
            Angajat *temp = department->angajati;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = angajat;
        }
    }
}
Departament *insertDepartament(Departament **departament, Departament *departamentNou)
{
    if (*departament == NULL)
    {
        *departament = departamentNou;
    }
    else
    {
        Departament *temp = *departament;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = departamentNou;
    }
    return *departament;
}

Angajat **countSalariu(Departament *department, int salariuMinim)
{
    int count = 0;
    Angajat **angajati = NULL;
    if (department)
    {
        Departament *temp = department;
        while (temp)
        {
            if (temp->angajati)
            {
                Angajat *tempAngajat = temp->angajati;
                while (tempAngajat)
                {
                    if (tempAngajat->salariu > salariuMinim)
                        count++;
                    tempAngajat = tempAngajat->next;
                }
            }
        }
    }

    if (count)
    {
        angajati = (Angajat **)malloc(sizeof(Angajat *) * count);
        for (int i = 0; i < count; i++)
        {
            angajati[i] = NULL;
        }
    }

    if (angajati && department)
    {
        int index = 0;
        Departament *temp = department;
        while (temp)
        {
            if (temp->angajati)
            {
                Angajat *tempAngajat = temp->angajati;
                while (tempAngajat)
                {
                    if (tempAngajat->salariu > salariuMinim)
                    {
                        angajati[index] = tempAngajat;
                        index++;
                    }
                    tempAngajat = tempAngajat->next;
                }
            }
        }
    }

    return angajati;
}

// Queue -> First In First Out

Queue *createQueueNode(Angajat *angajat)
{
    Queue *node = (Queue *)malloc(sizeof(Queue));
    node->item = angajat;
    node->next = NULL;
    return node;
}

void InsertQueue(Queue **queue, Angajat *angajat)
{
    Queue *node = createQueueNode(angajat);
    if ((*queue) == NULL)
    {
        (*queue) = node;
    }
    else
    {
        Queue *tmp = (*queue);
        while (tmp->next)
        {
            tmp = tmp->next;
        }
        tmp->next = node;
    }
}

Queue *insertAngajatiInQueue(Queue **queue, Departament *departament, char *numeDepartament)
{
    if (!queue)
    {
        (*queue) = (Queue *)malloc(sizeof(Queue));
        (*queue)->item = NULL;
        (*queue)->next = NULL;
    }
    Departament *temp = departament;
    while (temp)
    {
        if (strcmp(temp->numeDepartament, numeDepartament) == 0)
        {
            Angajat *tempAngajat = temp->angajati;
            while (tempAngajat)
            {
                InsertQueue(&(*queue), tempAngajat);
                tempAngajat = tempAngajat->next;
            }
        }
        temp = temp->next;
    }
    return (*queue);
}

Angajat **selectSalariiMici(Queue *queue)
{
    Angajat **angajatiSalarii = (Angajat **)malloc(sizeof(Angajat) * 3);
    for (int i = 0; i < 3; i++)
    {
        angajatiSalarii[i] = NULL;
    }

    Queue *temp = queue;
    while (temp)
    {
        Angajat *current = temp->item;
        for (int i = 0; i < 3; i++)
        {
            if (angajatiSalarii[i] == NULL || current->salariu < angajatiSalarii[i]->salariu)
            {
                for (int j = 2; j > i; j--)
                {
                    angajatiSalarii[j] = angajatiSalarii[j - 1];
                }
                angajatiSalarii[i] = current;
                break;
            }
        }
        temp = temp->next;
    }

    return angajatiSalarii;
}


void citireDateFisier(const char *numeFisier, Departament **departamente) {
    FILE *file = fopen("C:\\Users\\nanci\\OneDrive\\Desktop\\SDDEXAMPREP\\angajat\\Solutie\\angajati.txt", "r");
    if (!file) {
        perror("Nu s-a putut deschide fisierul");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        unsigned int cod_departament;
        char nume[50], prenume[50], adresa[50], numeDepartament[50];
        int varsta, salariu;
        sscanf(buffer, "%u %49s %49s %49s %d %d", &cod_departament, nume, prenume, adresa, &varsta, &salariu);

        sprintf(numeDepartament, "Departament%u", cod_departament);  // Utilizăm un nume generic pentru departament

        Angajat *angajat = createAngajat(cod_departament, nume, prenume, adresa, varsta, salariu);

        // Căutăm departamentul corespunzător
        Departament *departament = *departamente;
        while (departament && strcmp(departament->numeDepartament, numeDepartament) != 0) {
            departament = departament->next;
        }

        // Dacă nu există departamentul, îl creăm
        if (!departament) {
            departament = createDepartament(numeDepartament);
            *departamente = insertDepartament(departamente, departament);
        }

        // Inserăm angajatul în departament
        insertAngajati(departament, angajat);
    }

    fclose(file);
}



int main() {
    Departament *departamente = NULL;

    // Citirea datelor din fisier
    citireDateFisier("angajati.txt", &departamente);

    // Afișarea angajaților din departamente
    Departament *tempDep = departamente;
    while (tempDep) {
        printf("Departament %s:\n", tempDep->numeDepartament);
        Angajat *tempAngajat = tempDep->angajati;
        while (tempAngajat) {
            printf("Nume: %s %s, Adresa: %s, Varsta: %d, Salariu: %d\n",
            tempAngajat->nume, tempAngajat->prenume, tempAngajat->adresa, tempAngajat->varsta, tempAngajat->salariu);
            tempAngajat = tempAngajat->next;
        }
        tempDep = tempDep->next;
    }

    // Crearea unei cozi cu angajații dintr-un departament specific
    Queue *queue = NULL;
    queue = insertAngajatiInQueue(&queue, departamente, "Departament1");

    // Selectarea angajaților cu cele mai mici 3 salarii
    Angajat **angajatiSalariiMici = selectSalariiMici(queue);
    printf("\nAngajații cu cele mai mici 3 salarii:\n");
    for (int i = 0; i < 3; i++) {
        if (angajatiSalariiMici[i]) {
            printf("Nume: %s %s, Salariu: %d\n", angajatiSalariiMici[i]->nume, angajatiSalariiMici[i]->prenume, angajatiSalariiMici[i]->salariu);
        }
    }

    // Eliberarea memoriei alocate (funcții de eliberare a memoriei trebuie implementate)
    // eliberareDepartament(departamente);
    // eliberareQueue(queue);
    // free(angajatiSalariiMici);

    return 0;
}
