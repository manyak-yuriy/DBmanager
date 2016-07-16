/*
	Yuriy Manyak
	KA-23
	IASA-2013
*/

#include "decl.h"
#include <ctype.h>
#define BUFSIZE 256
#define MAX_FIELDS 100

enum token_type
{
   END = '\n', SYM, STR 
};

static const char *fmt_file = "format.txt";

char **fields = 0;
int n_fields;

struct entry *database = 0; // It's a list of entries
struct entry *outp_base = 0;
char *DB_name = 0;

struct entry *output = 0;

enum token_type get_token(char **token);
int get_comm_id(const char *str);
void seek_eol(void);
void parse(void);

void help_func();
void exit_func();
void append_func();
void add_func();
void print_func();
void create_func();                           
void clear_func();
void save_func();
void load_func();
void printf_func();
void sort_func();
void delete_func();

char *new_line(const char *source);

typedef void (*func)(void);
const char *commands[] = {"exit", "help", "append", "add", "print", "create", "clear",
						  "save", "load", "printf", "sort", "delete", ""};
func handles[] = {&exit_func, &help_func, &append_func, &add_func, &print_func,
					&create_func, &clear_func, &save_func, &load_func, &printf_func,
					&sort_func, &delete_func};


int main()
{
   printf("----------Type help to find about commands-----------\n");
   DB_name = new_line("default base");
   n_fields = get_format(&fields, fmt_file);

   while (1)
   {
      parse();
   }
   return 0;
}

char *new_line(const char *source)
{
   char *str = 0;
   str = (char *) mmalloc( sizeof(char) * (strlen(source) + 1) );
   strcpy(str, source);
   return str;
}

void exit_func()
{
   printf("Are you sure? (y/n) ");
   if ( toupper(getchar()) == 'Y' )
	  my_exit("End of session...");
}

void help_func()
{
   FILE *stream = fopen("help.txt", "r");
   char *str = 0;

   if (stream == 0)
	  printf("Help file is not accessible.\n");
   else
	  while ( (str = fget_line(stream)) != 0)
	  {
		 printf("%s\n", str);
		 //free(str);
	  }
   fclose(stream);
   seek_eol();
}

void append_func()
{
   char *s;
   enum token_type tok = get_token(&s);
   struct entry *new_entry;

   if (tok != END)
   {
	  printf("Too many parameters!\n");
	  seek_eol();
	  if (tok == STR)
		 //free(s);
	  return;
   }
   else
   {
	  new_entry = get_entry();
	  insert_entry(new_entry, &database, LIST_END);
   }
}

void add_func()
{
   char *s;
   int position;
   enum token_type tok;
   struct entry *new_entry;

   if ( scanf("%d", &position) != 1 )
   {
	  printf("First parameter must be decimal value!\n");
	  seek_eol();
	  return;
   }

   tok = get_token(&s);
   if ( tok != END )
   {
	  printf("Too many parameters!\n");
	  seek_eol();
	  //free(s);
	  return;
   }
   else
   {
	  new_entry = get_entry();
	  insert_entry(new_entry, &database, position);
   }
}

void print_func()
{
   print_all_DB(database, DB_name);
   output = database;
   seek_eol();
}

void create_func()
{
   enum token_type tok;
   char *s;

   tok = get_token(&s);
   if (tok == STR)
   {
	  create_DB(&database, &DB_name, s);
	  //free(s);
   }
   else
   {
	  if (tok == SYM) ;
		 //free(s);
   }
   seek_eol();
}

void clear_func()
{
   printf("Are you sure? (y/n) ");
   if ( toupper(getchar()) == 'Y' )
	  clear_DB(&database);
   printf("Database is empty.\n");
   seek_eol();
   seek_eol();
}

void save_func()
{
   enum token_type tok;
   char *s;

   tok = get_token(&s);
   if (tok == STR)
   {
      save_to(database, DB_name, s);
      //free(s);
   }
   else
   {
	  if (tok == SYM);
		 //free(s);
   }
   
   tok = get_token(&s);
   if (tok != END)
   {
	  printf("Too many parameters!\n");
	  //free(s);
	  seek_eol();
   }   
}

void load_func()
{
   enum token_type tok;
   char *s;
   clear_DB(&database);
   tok = get_token(&s);
   if (tok == STR)
   {
	  read_from(&database, &DB_name, s);
	  //free(s);
   }
   else
   {
	  if ( tok == SYM );
		 //free(s);
   }
   tok = get_token(&s);
   if (tok != END)
   {
	  printf("Too many parameters!\n");
	  //free(s);
	  seek_eol();
   }
}

void printf_func()
{
   enum token_type tok;
   char *s;
   char *filter[MAX_FIELDS];
   int i, position = 0;
   char ch;
   for (i = 0; i < n_fields; i++)
      filter[i] = 0;

   tok = get_token(&s);
   while (tok != END)
   {
      if (tok == STR)
	  {
		 position = -1;
		 for (i = 0; i < n_fields; i++)
			if ( strcmp(fields[i], s) == 0 )
			{
				position = i;
				break;
            }
		 //free(s);
	  }
	  tok = get_token(&s);
	  if (*s != '=')
	  {
		 //free(s);
		 break;
	  }
	  //free(s);

	  tok = get_token(&s);
	  if (tok == STR)
	  {
		 filter[position] = new_line(s);
		 //free(s);
	  }
	  tok = get_token(&s);
   }
   output = collect(database, filter);
   print_all_DB(output, "output");
}

void sort_func()
{
   struct entry *sorted = 0;
   enum token_type tok;
   char *s;
   char *criterias[MAX_FIELDS], directions[MAX_FIELDS];
   int i, number = 0;
   char ch;
   for (i = 0; i < n_fields; i++)
	  criterias[i] = 0;

   tok = get_token(&s);
   while (tok != END)
   {
	  if (tok == STR)
	  {
		 number++;
		 criterias[number - 1] = s;
		 //free(s);
	  }
   
	  tok = get_token(&s);
	  if (tok == SYM)
	  switch (*s)
	  {
		 case '+':
		 {
			directions[number - 1] = 'u';
			break;
		 }
		 case '-':
		 {
			directions[number - 1] = 'd';
			break;
		 }
		 //free(s);
	  }
	  tok = get_token(&s);
   }
   if (output != 0)
	  sorted = get_sorted_copy(output, criterias, directions, number);
   else
	  sorted = get_sorted_copy(database, criterias, directions, number);
   print_all_DB(sorted, "sorted output");
}

void delete_func()
{
   enum token_type tok;
   char *s;
   char *filter[MAX_FIELDS];
   int i, position = 0;
   char ch;
   for (i = 0; i < n_fields; i++)
      filter[i] = 0;

   tok = get_token(&s);
   while (tok != END)
   {
      if (tok == STR)
	  {
		 position = -1;
		 for (i = 0; i < n_fields; i++)
			if ( strcmp(fields[i], s) == 0 )
			{
				position = i;
				break;
            }
		 //free(s);
	  }
	  tok = get_token(&s);
	  if (*s != '=')
	  {
		 //free(s);
		 break;
	  }
	  //free(s);

	  tok = get_token(&s);
	  if (tok == STR)
	  {
		 filter[position] = new_line(s);
		 //free(s);
	  }
	  tok = get_token(&s);
   }
   delete_by_mask(&database, filter);
}

void parse()
{
   int comm_id;
   char *s;
   enum token_type tok;

   printf(">> ");
   tok = get_token(&s);

   if ( tok == STR )
   {
      comm_id = get_comm_id(s);
	  //free(s);
	  if (comm_id != -1)
		 (*handles[comm_id - 1])();
	  else
	  {
		 printf("Unknown command!\n");
		 seek_eol();
	  }
   }
   else
      seek_eol();
   if ( tok != END ) ;
	  //free(s);
   //seek_eol();
}

void seek_eol()
{
   char *s;
   enum token_type tok;
   while ( (tok = get_token(&s)) != END );
	 // if (tok == STR && s != NULL)
		 //free(s);
}

enum token_type get_token(char **token)
{
   static char ch;
   static char buffer[BUFSIZE], *bufp;

   if (ch == '\n')
   {
	  ch = '\0';
	  return '\n';
   }
   while ( isspace(ch = getchar()) && ch != '\n' )
	  ;

   bufp = buffer;
   while ( isalnum(ch) || ch == '.' )
   {
	  *(bufp++) = ch;
	  ch = getchar();
   }
   *bufp = '\0';
   if ( isalnum(*buffer) )
   {
	  *token = (char *) mmalloc( sizeof(char) * (strlen(buffer) + 1) );
	  strcpy(*token, buffer);
	  return STR;
   }
   if (ch == '\n')
   {
	  ch = '\0';
	  return '\n';
   }
   *token = (char *) mmalloc( sizeof(char) );
   **token = ch;
   return SYM;
}

int get_comm_id(const char *str)
{
   char **command = commands;
   int number = 0;
   while ( **command != '\0' )
   {
	  number++;
	  if ( strcmp(*command, str) == 0 )
		 return number;
	  command++;
   }
   return -1;
}




