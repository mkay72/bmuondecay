#include "name.h"

void append_suffix(char name[FQGRID_MAX_NAME], char* s, char name_new[FQGRID_MAX_NAME])
{
    const char* p = strchr(name, '(');
    if (p != NULL) 
    {
        size_t index = p - name;
        strncpy(name_new, name, index); 
        name_new[index] = '\0';
        strcat(name_new, s);
        strcat(name_new, p); 
    } 
    else 
    {
        strcpy(name_new, name);
    }
}


name_t* new_name(const char* format, ...) 
{
    name_t *name = malloc(sizeof(name_t));
    memset(name->inflexion, '\0', sizeof(name->inflexion));
    memset(name->suffix, '\0', sizeof(name->suffix));
    memset(name->radical, '\0', sizeof(name->radical));
    memset(name->fullname, '\0', sizeof(name->fullname));

    va_list args;
    va_start(args, format);
    vsnprintf(name->fullname, FQGRID_MAX_NAME, format, args);
    va_end(args);

    parse_name(name);

    return name;
}



void delete_name(name_t* name)
{
    if (name != NULL)
    {
        free(name);
    }
}



static void parse_name(name_t* name)
{
    int has_suffix = 0;
    int has_inflexion = 0;

    char* suffix_start = parse_suffix(name->fullname, name->suffix, &has_suffix);
    char* inflexion_start = parse_inflexion(name->fullname, name->inflexion, &has_inflexion);

    int nradical = 0;
    char* radical_start = name->fullname;
    nradical = (has_suffix == 1) ? suffix_start - radical_start : inflexion_start - radical_start;
    strncpy(name->radical, radical_start, nradical);

}



static char* parse_suffix(const char fullname[FQGRID_MAX_NAME], char suffix[FQGRID_NAME_SUFFIX_MAX], int *has_suffix)
{
    char* suffix_start = strchr(fullname, '{');
    char* suffix_end = strchr(fullname, '}') + 1;

    int nsuffix = 0;

    if (suffix_start != NULL)
    {
        nsuffix = suffix_end - suffix_start;
        strncpy(suffix, suffix_start, nsuffix);
        *has_suffix = 1;
    }

    return suffix_start;
}



static char* parse_inflexion(const char fullname[FQGRID_MAX_NAME], char inflexion[FQGRID_NAME_INFLEXION_MAX], int *has_inflexion)
{
    char* inflexion_start = strchr(fullname, '(');
    char* inflexion_end = strchr(fullname, ')') + 1;

    int ninflexion = 0;

    if (inflexion_start != NULL)
    {
        ninflexion = inflexion_end - inflexion_start;
        strncpy(inflexion, inflexion_start, ninflexion);
        *has_inflexion = 1;
    }

    return inflexion_start;
}



void modify_radical(name_t *name, const char* format, ...)
{
    char new_radical[FQGRID_NAME_RADICAL_MAX];
    memset(new_radical, '\0', FQGRID_NAME_RADICAL_MAX);

    va_list args;
    va_start(args, format);
    vsnprintf(new_radical, FQGRID_NAME_RADICAL_MAX, format, args);
    va_end(args);

    int nradical_new = strlen(new_radical);

    if (nradical_new >= 0)
    {
        strncpy(name->radical, new_radical, FQGRID_NAME_RADICAL_MAX);
        update_name(name);
    }

}    



void modify_suffix(name_t *name, const char* format, ...)
{
    char new_suffix[FQGRID_NAME_SUFFIX_MAX];
    memset(new_suffix, '\0', FQGRID_NAME_SUFFIX_MAX);

    va_list args;
    va_start(args, format);
    vsnprintf(new_suffix, FQGRID_NAME_SUFFIX_MAX, format, args);
    va_end(args);

    int nsuffix_new = strlen(new_suffix);

    if (nsuffix_new >= 0)
    {
        strncpy(name->suffix, new_suffix, FQGRID_NAME_SUFFIX_MAX);
        update_name(name);
    }
}     



static void update_name(name_t* name)
{
    char new_fullname[FQGRID_MAX_NAME];
    memset(new_fullname, '\0', sizeof(new_fullname));


    strcat(new_fullname, name->radical);
    strcat(new_fullname, name->suffix);
    strcat(new_fullname, name->inflexion);

    strncpy(name->fullname, new_fullname, FQGRID_MAX_NAME);

}


void print_name(name_t* name)
{
    printf("\n");
    printf("radical  [%2d] = ", strlen(name->radical));
    printf("%s\n", name->radical);

    printf("suffix   [%2d] = ", strlen(name->suffix));
    printf("%s\n", name->suffix);

    printf("inflexion[%2d] = ", strlen(name->inflexion));
    printf("%s\n", name->inflexion);

    printf("fullname [%2d] = ", strlen(name->fullname));
    printf("%s\n", name->fullname);
}
