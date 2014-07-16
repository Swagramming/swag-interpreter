#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef enum
{
    INT,
    STRING
}type_t;

typedef union
{
    int i;
    char* s;
} any_t;

typedef struct
{
    any_t any;
    type_t tag;
    char* name;
} var_t;

var_t* var_reg = NULL;
int reg_size = 0;

var_t interpretexp(char** line);

int filesize(FILE* file)
{
    int len = 0, cur_pos = ftell(file);
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, cur_pos, SEEK_SET);
    return len;  
}

char* readfile(const char* filename)
{
	char* ptr;
    FILE* file = fopen(filename, "r");
	if (file == NULL) exit(EXIT_FAILURE);
	ptr = malloc(filesize(file) * sizeof(char));
	fread(ptr, filesize(file), 1, file);
    return ptr;
}

char* readtoken(char** line, int (*func)(int), int skip)
{
    if (skip == 0) while (isspace(**line) != 0) ++(*line);
    char* temp1 = *line, *temp2;
    while (func(**line) != 0 && **line != '\0') (*line)++;
    temp2 = malloc((*line - temp1 + 1) * sizeof(char));
    strncpy(temp2, temp1, *line - temp1);
    temp2[*line - temp1] = '\0';
    if (skip == 0) while (isspace(**line) != 0) ++(*line);
    return temp2;
}

void interpretfunc(char* name, char* line)
{
    if (strcmp(name, "print") == 0)
    {
        var_t temp = interpretexp(&line);
        switch (temp.tag)
        {
            case STRING: printf(temp.any.s); break;
            case INT: printf("%d", temp.any.i); break;
            default: puts("unsupported type for func print"); abort();
        }
    }
    else printf("function %s does not exist", name);
}

void interpretvar(char* name, char* line)
{
}

var_t make_int(int i, char* name)
{
	var_t temp;
	temp.any.i = i;
	temp.tag = INT;
	temp.name = malloc(strlen(name) + 1);
	strcat(temp.name, name);
	return temp;
}

var_t make_string(char* s, char* name)
{
	var_t temp;
	temp.any.s = malloc(strlen(s) + 1);
	strcat(temp.any.s, s);
	temp.tag = STRING;
	temp.name = malloc(strlen(name) + 1);
	strcat(temp.name, name);
	return temp;
}

void freevar(var_t var)
{
}

var_t* find_var(char* name)
{
    int i;
    for (i = 0; i <= reg_size; ++i)
        if (strcmp(name, var_reg[i].name) == 0)
            return &var_reg[i];
    return NULL;
}

void store_var(var_t var)
{    
    var_t* temp = malloc((reg_size + 1) * sizeof(var_t));
    memcpy(temp, var_reg, reg_size * sizeof(var_t));
    memcpy(temp + reg_size, &var, sizeof(var_t));
    free(var_reg);
    var_reg = temp;
    ++reg_size; 
}

var_t add_var(var_t var1, var_t var2)
{
    var_t var;
    assert(var1.tag == var2.tag);
    var.tag = var1.tag;
    switch (var1.tag)
    {
        case STRING:
            var.any.s = malloc(strlen(var1.any.s) + strlen(var2.any.s) + 1);
            strcpy(var.any.s, var1.any.s);    
            strcat(var.any.s, var2.any.s);
            if (var1.name == NULL) freevar(var1);
            if (var2.name == NULL) freevar(var2);
            break;
        case INT:
            var.any.i = var1.any.i + var2.any.i;
            break;
        default:
            puts("Error: unsupported addition");
            abort();
    }
    return var;
}

var_t sub_var(var_t var1, var_t var2)
{
    var_t var;
    assert(var1.tag == var2.tag);
    var.tag = var1.tag;
    switch (var1.tag)
    {
		case INT:
			var.any.i = var1.any.i - var2.any.i;
			break;
		default:
			puts("Error: unsupported subtraction");
			abort();
	}
	return var;		
}

var_t interpretexp(char** line)
{
    var_t exp;
    while (isspace(**line) != 0) ++(*line);
    if (**line == '"') 
    {
        ++(*line);
        exp.any.s = readtoken(line, isprint, 1);
        exp.tag = STRING;
    }
    else if (isdigit(**line))
    {
        char* str = readtoken(line, isdigit, 0);
        exp.any.i = atoi(str);
        exp.tag = INT;
        free(str);
    }
    else if (isalpha(**line))
    {
        char* id = readtoken(line, isalpha, 0);
        /*exp = find_var(*/
        free(id);
    }
    else
    {
        printf("Error at:\n %s ", *line);
        exit(EXIT_FAILURE);
    }
    while (isspace(**line) != 0) ++(*line);
    /*if + return add_var(exp, interpretexp(line))
    else if - return sub_var(exp, interpretexp(line))*/
    return exp;
}

void interpretline(char* line)
{
    char* name;
    do
    {
        name = readtoken(&line, isalpha, 0);
        printf(">%s<\n", name);
        switch (*line)
        {
            case '(': interpretfunc(name, ++line); break;
            case '=': interpretvar(name, ++line); break;
            default: printf("compile error at: %s\n", line);
					 abort();
        }
        /*free(name);*/
    } while (*line != '\0');
}

void interpret(char* ptr)
{
    char* line = strtok(ptr, ";");
    do
    {
        printf("Interpreting line %s\n", line);
        interpretline(line);
        line = strtok(NULL, ";");
    } while (line != NULL);
}

int main(int argc, char** argv)
{
    if (argc != 2)
	{
		puts("invalid number of arguments!");
		return EXIT_FAILURE;
	}
	char* ptr = readfile(argv[1]);
    interpret(ptr);
	return EXIT_SUCCESS;
}
