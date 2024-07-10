#include "fqg_name.h"

typedef struct fqg_name_buffer_state *YY_BUFFER_STATE;

extern int fqg_namelex();

extern YY_BUFFER_STATE fqg_name_scan_string(char * str);

extern void fqg_name_delete_buffer(YY_BUFFER_STATE buffer);

extern int fqg_namelex_destroy(void);

extern char* fqg_nametext;

extern int fqg_nameleng;


static int pushed_back = 0;

static int next_token()
{
    if (pushed_back) 
    {
        int token_type = pushed_back;
        pushed_back = 0;
        return token_type;
    }
    
    return fqg_namelex();
}


static void put_back(int token_type)
{
    pushed_back = token_type;
}


static void fqg_name_set_type_zero(fqg_name_t* fqg_name)
{
    if (fqg_name != NULL)
    {
        fqg_name->type = 0;
    }
}


static void fqg_name_set_stem_zero(fqg_name_t* fqg_name)
{
    if (fqg_name != NULL)
    {
        memset(fqg_name->stem, '\0', MAX_STEM_LEN-1);
    }
}


static void fqg_name_set_suffix_zero(fqg_name_t* fqg_name)
{
    if (fqg_name != NULL)
    {
        memset(fqg_name->suffix, '\0', MAX_SUFFIX_LEN-1);
    }
}


static void fqg_name_set_sys_suffix_zero(fqg_name_t* fqg_name)
{
    if (fqg_name != NULL)
    {
        memset(fqg_name->sys_suffix, '\0', MAX_SYS_SUFFIX_LEN-1);
    }
}


static void fqg_name_set_inflexion_zero(fqg_name_t* fqg_name)
{
    if (fqg_name != NULL)
    {
        memset(fqg_name->inflexion[0], '\0', MAX_INFLEXION_LEN-1);
        memset(fqg_name->inflexion[1], '\0', MAX_INFLEXION_LEN-1);
    }
}


static void fqg_name_set_name_zero(fqg_name_t* fqg_name)
{
    if (fqg_name != NULL)
    {
        int tot_len = MAX_STEM_LEN + MAX_SUFFIX_LEN + MAX_SYS_SUFFIX_LEN + 2*MAX_INFLEXION_LEN;
        memset(fqg_name->name, '\0', tot_len-1);
    }
}


void fqg_name_set_zero(fqg_name_t* fqg_name)
{
    if (fqg_name != NULL)
    {
        fqg_name_set_type_zero(fqg_name);
        
        fqg_name_set_stem_zero(fqg_name);
        
        fqg_name_set_suffix_zero(fqg_name);
        
        fqg_name_set_sys_suffix_zero(fqg_name);
        
        fqg_name_set_inflexion_zero(fqg_name);
        
        fqg_name_set_name_zero(fqg_name);
    }
}



static void fqg_name_parse_type(fqg_name_t* fqg_name)
{
    fqg_name->type = 0;
    
    if (strcmp(fqg_nametext, "dwf") == 0) 
    {
        fqg_name->type = TT_TYPE_DWF;
    }
    else if (strcmp(fqg_nametext, "swf") == 0) 
    {
        fqg_name->type = TT_TYPE_SWF;
    }
    else if (strcmp(fqg_nametext, "dxg") == 0) 
    {
        fqg_name->type = TT_TYPE_DXG;
    }    
    
}


static void fqg_name_parse_stem(fqg_name_t* fqg_name)
{
    int bufsize = MAX_STEM_LEN;
    
//     int token_len;

//     if type is general, then immidiately parse identifier 
    if (fqg_name->type == 0)
    {        
//         token_len = fqg_nameleng;
        strncat(fqg_name->stem, fqg_nametext, fqg_nameleng);
    }
    

    int is_valid = 0;
    int token = next_token();    
//     token_len = fqg_nameleng;
    
    while ((token != TT_OPENING_PARANTHESIS) && (token != TT_OPENING_CURLY) && (token != TT_POUND))
    {
        is_valid = 0;
        switch (token)
        {
            
            case TT_IDENTIFIER:
                is_valid = 1;
                break;
            case TT_UNDERSCORE:
                is_valid = 1;
                break;
            default:
                is_valid = 0;
                printf("Unrecognized token produced by fqg_name_parse_stem(...)\n");
                printf("Token value is %s\n", fqg_nametext);
                exit(1);
                break;
        }
            
        if (is_valid == 1)
        {
            strncat(fqg_name->stem, fqg_nametext, fqg_nameleng);
        }
        token = next_token();        
    }
    
    if (token == TT_OPENING_CURLY)
    {
        put_back(TT_OPENING_CURLY);
    }
    
    else if (token == TT_OPENING_PARANTHESIS)
    {
        put_back(TT_OPENING_PARANTHESIS);
    }

    else if (token == TT_POUND)
    {
        put_back(TT_POUND);
    }    
}



static void fqg_name_parse_suffix(fqg_name_t* fqg_name)
{
    int bufsize = MAX_SUFFIX_LEN;
    int nparsed_tokens = 0;
    
    int token = next_token();
    
    while (token != TT_CLOSING_CURLY)
    {
        if (token == TT_OPENING_CURLY)
        {
            printf("parse_suffix(): Second opening curly '{' is found\n");
            exit(1);
        }
        
//         size_t token_len = fqg_nameleng);
        strncat(fqg_name->suffix, fqg_nametext, fqg_nameleng);
        
        token = next_token();
        nparsed_tokens++;
        
        if (nparsed_tokens >= 4)
        {
            printf("parse_suffix(): Closing curly '}' is not found after 5 parsed tokens\n");
            exit(1);
        }
    }
}


static void fqg_name_parse_sys_suffix(fqg_name_t* fqg_name)
{
    int nparsed_tokens = 0;
    
    int token = next_token();
    
    while (token != TT_POUND)
    {
        
//         size_t token_len = fqg_nameleng);
        strncat(fqg_name->sys_suffix, fqg_nametext, fqg_nameleng);
        
        token = next_token();
        nparsed_tokens++;
        
        if (nparsed_tokens >= 10)
        {
            printf("fqg_name_parse_sys_suffix(): TT_POUND is not found after 10 parsed tokens\n");
            exit(1);
        }
    }
}


static void fqg_name_parse_inflexion(fqg_name_t* fqg_name)
{
    int nparsed_tokens = 0;
    
    int token = next_token();
    
    while (token != TT_CLOSING_PARANTHESIS)
    {
        switch (token)
        {
            case TT_ID_INFLEXION:
                strncat(fqg_name->inflexion[nparsed_tokens], fqg_nametext, fqg_nameleng);
                nparsed_tokens++;
                break;
            case TT_COMMA:
                break;
            default:
                printf("Unrecognized token produced by fqg_name_parse_inflexion(...)\n");
                printf("Token value is %s\n", fqg_nametext);
                exit(1);
            
        }
        
        token = next_token();

    }
}



void fqg_name_parse_from_string(char* name_string, fqg_name_t* fqg_name)
{
    YY_BUFFER_STATE buffer = fqg_name_scan_string(name_string);
       
    int token = next_token();
    
    while (token != TT_END_OF_FILE)
    {
//         printf("%d\t%s\n", token, fqg_nametext);
        switch (token)
        {
            case (TT_TYPE_DWF):
                fqg_name_parse_type(fqg_name);
                break;
            case (TT_TYPE_SWF):
                fqg_name_parse_type(fqg_name);
                break;
            case (TT_TYPE_DXG):
                fqg_name_parse_type(fqg_name);
                break;                 
            case (TT_COLON):
                fqg_name_parse_stem(fqg_name);
                break;
            case TT_IDENTIFIER:
                fqg_name_parse_stem(fqg_name);
                break;
            case TT_OPENING_CURLY:
                fqg_name_parse_suffix(fqg_name);
                break;
            case TT_POUND:
                fqg_name_parse_sys_suffix(fqg_name);
                break;
            case TT_OPENING_PARANTHESIS:
                fqg_name_parse_inflexion(fqg_name);
                break;             
            default:
                printf("Urecognized token produced by fqg_name_parse_from_string(%s,...)\n", name_string);
                printf("Token value is %s\n", fqg_nametext);
                exit(1);
                break;
        }

            
        token = next_token();
    }
    
    fqg_name_delete_buffer(buffer);
    fqg_namelex_destroy();
    
}





// void set_fqg_name(fqg_name_t* fqg_name, int type, char* stem, char* suffix, char* sys_suffix, char* inflexion[2])
void update_fqg_name(fqg_name_t* fqg_name)
{
    fqg_name_set_name_zero(fqg_name);
    
    
    char type_string[8];
    for (int i = 0; i < 8; i++)
    {
        type_string[i] = '\0';
    }
    
    int type = fqg_name->type;
    
    if (type == TT_TYPE_DWF)
    {
        strncat(type_string, "dwf:", 7);
    }
    else if (type == TT_TYPE_SWF)
    {
        strncat(type_string, "swf:", 7);
    }
    else if (type == TT_TYPE_DXG)
    {
        strncat(type_string, "dxg:", 7);
    }
    
    strncat(fqg_name->name, type_string, 7);
    
    if (strlen(fqg_name->stem) > 0)
    {
        strncat(fqg_name->name, fqg_name->stem, MAX_STEM_LEN-1);
    }
    
    if (strlen(fqg_name->suffix) > 0)
    {
        strncat(fqg_name->name, "{", 2);
        strncat(fqg_name->name, fqg_name->suffix, MAX_SUFFIX_LEN-1);
        strncat(fqg_name->name, "}", 2);
    }
    
    if (strlen(fqg_name->sys_suffix) > 0)
    {
        strncat(fqg_name->name, "#", 2);
        strncat(fqg_name->name, fqg_name->sys_suffix, MAX_SYS_SUFFIX_LEN-1);
        strncat(fqg_name->name, "#", 2);
    }    
    
    if ((strlen(fqg_name->inflexion[0]) > 0) && (strlen(fqg_name->inflexion[1]) == 0))
    {
        strncat(fqg_name->name, "(", 2);
        strncat(fqg_name->name, fqg_name->inflexion[0], MAX_INFLEXION_LEN-1);
        strncat(fqg_name->name, ")", 2);
    }
    
    if ((strlen(fqg_name->inflexion[0]) > 0) && (strlen(fqg_name->inflexion[1]) > 0))
    {
        strncat(fqg_name->name, "(", 2);
        strncat(fqg_name->name, fqg_name->inflexion[0], MAX_INFLEXION_LEN-1);
        strncat(fqg_name->name, ",", 2);
        strncat(fqg_name->name, fqg_name->inflexion[1], MAX_INFLEXION_LEN-1);
        strncat(fqg_name->name, ")", 2);
    }        

}
           
           
void set_fqg_type(fqg_name_t* fqg_name, int type)
{
    if (fqg_name != NULL)
    {
        fqg_name->type = type;
        update_fqg_name(fqg_name);
    }
}


void set_fqg_stem(fqg_name_t* fqg_name, char* stem)
{
    if (fqg_name != NULL)
    {
        strncpy(fqg_name->stem, stem, MAX_STEM_LEN-1);
        update_fqg_name(fqg_name);
    }
}

void set_fqg_suffix(fqg_name_t* fqg_name, const char* format, ...)
{
    char new_suffix[MAX_SUFFIX_LEN];
    memset(new_suffix, '\0', MAX_SUFFIX_LEN-1);
    
    
    va_list args;
    va_start(args, format);
    vsnprintf(new_suffix, MAX_SUFFIX_LEN-1, format, args);
    va_end(args);
    
    if (fqg_name != NULL)
    {
        strncpy(fqg_name->suffix, new_suffix, MAX_SUFFIX_LEN-1);
        update_fqg_name(fqg_name);
    }
}


void set_fqg_sys_suffix(fqg_name_t* fqg_name, const char* format, ...)
{
    char new_sys_suffix[MAX_SYS_SUFFIX_LEN];
    memset(new_sys_suffix, '\0', MAX_SYS_SUFFIX_LEN-1);
    
    
    va_list args;
    va_start(args, format);
    vsnprintf(new_sys_suffix, MAX_SYS_SUFFIX_LEN-1, format, args);
    va_end(args);
    
    if (fqg_name != NULL)
    {
        strncpy(fqg_name->sys_suffix, new_sys_suffix, MAX_SYS_SUFFIX_LEN-1);
        update_fqg_name(fqg_name);
    }
}


void set_fqg_inflexion(fqg_name_t* fqg_name, char* inflexion[2])
{
    if (fqg_name != NULL)
    {
        strncpy(fqg_name->inflexion[0], inflexion[0], MAX_INFLEXION_LEN-1);
        strncpy(fqg_name->inflexion[1], inflexion[1], MAX_INFLEXION_LEN-1);
        update_fqg_name(fqg_name);
    }
}


fqg_name_t* new_fqg_name(char* name_string)
{
    fqg_name_t* fqg_name = malloc(sizeof(fqg_name_t));
    
    fqg_name_set_zero(fqg_name);
    
    fqg_name_parse_from_string(name_string, fqg_name);
    
    update_fqg_name(fqg_name);

    return fqg_name;
}



void delete_fqg_name(fqg_name_t* fqg_name)
{
    if (fqg_name != NULL)
    {
        free(fqg_name);
    }
}


void print_fqg_name(fqg_name_t* fqg_name)
{
    printf("%-8s\t%-8d\n", "type",  fqg_name->type);
    printf("%-8s\t%-32s\n", "stem",  fqg_name->stem);
    printf("%-8s\t%-16s\n", "suffix",  fqg_name->suffix);
    printf("%-8s\t%-16s\n", "sys_suffix",  fqg_name->sys_suffix);
    printf("%-8s\t%-8s%-8s\n", "inflexion",  fqg_name->inflexion[0], fqg_name->inflexion[1]);
    printf("%-8s\t%-64s\n", "name",  fqg_name->name);
}