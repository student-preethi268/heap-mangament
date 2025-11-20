#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 20

#define HASH_SIZE 100

// Structure to represent a symbol entry
typedef struct Symbol {
    char name[MAX_STRING];
    int value;
    struct Symbol *next;
} Symbol;

// Structure to represent a scope
typedef struct Scope {
    int level;
    Symbol *table[HASH_SIZE];  // linked list to store data
    struct Scope *outerScope; // linked to store scope
} Scope;

// Function to create a new symbol
Symbol* createSymbol(char name[], int value) {
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    strcpy(symbol->name, name);
    symbol->value = value;
    symbol->next = NULL;
    return symbol;
}
 
// Function to create a new scope
Scope* createScope(int level, Scope* outerScope) {
    Scope* scope = (Scope*)malloc(sizeof(Scope));
    scope->level = level;
    for (int i = 0; i < HASH_SIZE; i++)
        scope->table[i] = NULL;
    scope->outerScope = outerScope;
    return scope;
}

// Hash function
int hash(char name[]) {
    int hash = 0;
    for (int i = 0; name[i] != '\0'; i++)
        hash = (hash * 2 + name[i]) % HASH_SIZE;
    return hash;
}

// Function to insert a symbol into the current scope
void insertSymbol(Scope* scope, char name[], int value) {
    int index = hash(name);
    Symbol* symbol = createSymbol(name, value);
    symbol->next = scope->table[index];
    scope->table[index] = symbol;
}

// Function to lookup for a symbol in the current scope and its outer scopes
int lookupSymbol(Scope* scope, char name[]) {
    int index = hash(name);
    Symbol* symbol = scope->table[index];
    while (symbol != NULL) {
        if (strcmp(symbol->name, name) == 0)
            return symbol->value;
        symbol = symbol->next;
    }
    if (scope->outerScope != NULL)
        return lookupSymbol(scope->outerScope, name);
    return -1; // Symbol not found
}

int main() {
    Scope* globalScope = createScope(0, NULL);
    Scope* currentScope = globalScope;
    char command[MAX_STRING], name[MAX_STRING];
    int value;

  printf("please give some commands line by line and to create the scopes\n ");
    while (1) {
        scanf("%s", command);
        if (strcmp(command, "begin") == 0) {
            currentScope = createScope(currentScope->level + 1, currentScope);
        } else if (strcmp(command, "end") == 0) {
            if (currentScope->outerScope != NULL)
                currentScope = currentScope->outerScope;
            else
                break; // End of program
        } else if (strcmp(command, "assign") == 0) {
            scanf("%s %d", name, &value);
            insertSymbol(currentScope, name, value);
        } else if (strcmp(command, "print") == 0) {
            scanf("%s", name);
            value = lookupSymbol(currentScope, name);
            if (value != -1)
                printf("%s = %d\n", name, value);
            else
                printf("%s Symbol not found\n", name);
        }
    }
    return 0;
}
