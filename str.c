#include "str.h"

/*
 * Inicializace String
 */
void strInit(String *s)
{
	if(s == NULL)
	{
		return;
	}
	
	s->str = (char*) memory_manager_malloc(STR_LENGTH);
	
	s->str[0] = '\0';
	s->length = 0;
	s->allocated = STR_LENGTH;
}

/*
 * Přidá znak do řetězce
 */
void strAddChar(String *s, char c)
{
	if(s == NULL)
	{
		return;
	}
	
	if(s->length + 1 >= s->allocated)
	{
		s->str = (char*) memory_manager_realloc(s->str, (size_t)(s->allocated * 2));
		s->allocated *= 2;
	}
	
	s->str[s->length] = c;
	s->str[++s->length] = '\0';
}

/*
 * Uvolneni String z pameti
 */
void strFree(String *s)
{
	if(s != NULL)
	{
		memory_manager_free_one(s->str);
		memory_manager_free_one(s);
	}
}

/*
 * Nacte int ze stdin
 */
int strReadInt()
{
	long int cislo;
	int c, velikost = 1, delka = 0;
	char* buffer = (char*) memory_manager_malloc(sizeof(char));
	
	while(1)
	{
		c = getchar();
		
		if(velikost == delka)
		{
			velikost *= 2;
			buffer = (char*) memory_manager_realloc(buffer, sizeof(char) * velikost);
		}
		
		if(c == '\n' || c == EOF)
		{
			buffer[delka] = '\0';
			break;
		}
		
		if(isdigit(c))
		{
			buffer[delka] = c;
		}
		else
		{
			error_f(ERROR_INPUT);
		}
		
		delka++;
	}
	
	if(delka == 0)
	{
		error_f(ERROR_INPUT);
	}
	
	sscanf(buffer, "%ld", &cislo);
	
	memory_manager_free_one(buffer);
	
	if(cislo >= INT_MAX)
	{
		error_f(ERROR_INPUT);
	}
	
	return (int)cislo;
}

/*
 * Nacte double ze stdin
 */
double strReadDouble()
{
	int c, velikost = 1, delka = 0;
	double cislo;
    char* buffer = (char*) memory_manager_malloc(sizeof(char));
    int pom = 0;
    int dot_pom = 0;
    while(1)
    {
        c = getchar();
    
        if(velikost == delka)
        {
            velikost *= 2;
            buffer = (char*) memory_manager_realloc(buffer, sizeof(char) * velikost);
        }
        
        if(c == '\n' || c == EOF){
            buffer[delka] = '\0';       
            break;
        }
        
        if (isdigit(c))
        {
            buffer[delka] = c;
            pom = 1;
        }
        else if(c == 46 && dot_pom == 0) // 46 - tečka
        {
            buffer[delka] = c;
            dot_pom = 1;
        }
        else if(pom == 1 && (c == 69 || c == 101)){ // E - 69 , e - 101
            buffer[delka] = c;
            pom = 2;
        }
        else if (pom == 2 &&  ((buffer[delka - 1] == 69 || buffer[delka - 1] == 101) && (c == 43 || c == 45))){ // 43 - plus , 45 - mínus
            buffer[delka] = c;
        }
        else
        {
            error_f(ERROR_INPUT);
        }
        delka++;
    }
    
    if(delka == 0)
	{
		error_f(ERROR_INPUT);
	}
    
    sscanf(buffer, "%lf", &cislo);
    memory_manager_free_one(buffer);
    buffer = NULL;
    
    if(cislo >= DBL_MAX)
    {
        error_f(ERROR_INPUT);
    }

    return (double)cislo;
}

/*
 * Vrati delku retezce (bez '\0')
 */
int strLength(String *s)
{
	return s->length;
}

/*
 * Prekopiruje druhy retezec do prvniho
 */
void strCopy(String *s1, String *s2)
{
   if (s2->length >= s1->allocated)
   {
      s1->str = (char*) memory_manager_realloc(s1->str, s2->length + 1);
      s1->allocated = s2->length + 1;
   }
   
   strcpy(s1->str, s2->str);
   s1->length = s2->length;
}

/*
 * Vrati podretezec retezce od i, n znaku
 */
String *strSubstr(String *s, int i, int n)
{
	if(i > strLength(s))
	{
		return NULL;
	}
	
	if(i + n > strLength(s))
	{
		n = strLength(s) - i;
	}
	
	String *s_tmp;
	
	s_tmp = (String*) memory_manager_malloc(sizeof(String));
	
	strInit(s_tmp);
	
	int j;
	for(j = 0; j < n; j++)
	{
		strAddChar(s_tmp, s->str[i]);
		i++;
	}
	
	return s_tmp;
}

/*
 * Porovna retezce
 */
int strCompare(String *s1, String *s2)
{	
	int num = strcmp(s1->str, s2->str);
	
	if(num > 0)
	{
		return 1;
	}
	else if(num < 0)
	{
		return -1;
	}
	
	return 0;
}

void strPrintStr(String *s)
{
	printf("%s", s->str);
}