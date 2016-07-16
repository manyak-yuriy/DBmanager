/*
	Yuriy Manyak
	KA-23
	IASA-2013
*/

#include "decl.h"
#define offset 1

#define BIG 20000
char big[BIG];
char *big_p = big;

char *mmalloc(int size)
{
   char *result = big_p;
   big_p += size;
   return result;
}

//Desroys previous list structure
struct entry *get_sorted_copy(struct entry *list,
							  const char **criterias, const char *directions, int n)
{
   struct entry *sorted_list = 0;
   struct entry *min_node, *curr_node, *new_node;
   struct entry *fake_node = (struct entry *) malloc( sizeof(struct entry) );
   fake_node -> next = list;
   fake_node -> values = 0;
   while (fake_node -> next != 0)
   {
	  min_node = fake_node;
	  curr_node = fake_node -> next;
	  while (curr_node -> next != 0)
	  {
		 if (compare(curr_node -> next, min_node -> next, criterias, directions, n) < 0)
			min_node = curr_node;
		 curr_node = curr_node -> next;
	  }
	  new_node = min_node -> next;
	  unlink_next(min_node);
	  new_node -> next = 0;
	  insert_entry(new_node, &sorted_list, LIST_END);
   }
   //free(fake_node);
   return sorted_list;   
}

void unlink_next(struct entry *node)
{
   if (node == 0 || node -> next == 0)
	  my_exit("Corrupted entry");

   node -> next = node -> next -> next;
}

void unlink_by_pos(struct entry **database_p, int position)
{
   struct entry *list = *database_p;
   struct entry *next_node;
   int i;
   if (list == 0)
	  my_exit("Database if empty!");
   if (position == 1)
   {
	  *database_p = next_node;
	  return;
   }
   if (position <= 0)
      my_exit("Not a valid position!");
   
   for (i = 1; i <= position - 2; i++)
   {
	  list = list -> next;
	  if (list == 0)
		 my_exit("Incorrect position to delete!");
   }
   if (list -> next == 0)
	  my_exit("Incorrect position to delete!");
   next_node = list -> next -> next;
   list -> next = next_node;
}

int compare(const struct entry *entry_1, const struct entry *entry_2,
			const char **criterias, const char *directions, int n)
{
   int i, result;
   for (i = 0; i < n_fields; i++)
	  if ( strcmp( *criterias, fields[i] ) == 0 )
		 break;
   if (i == n_fields)
	  my_exit("Undefined field handle!");
   result = strcmp( (entry_1 -> values)[i], (entry_2 -> values)[i] );
   if (result == 0)
   {
	  if (n == 1)
		 return 0;
	  else
	     return compare(entry_1, entry_2, criterias + 1, directions + 1, n - 1);
   }
   else switch (*directions)
   {
	  case 'u':
	  {
		 return result;
	  }
	  case 'd':
	  {
		 return -result;
	  }
	  default:
	     my_exit("There's no direction for current criteria!");
   }
   return 0;
}

struct entry *duplicate(const struct entry *source)
{
   struct entry *copy = (struct entry *) malloc( sizeof(struct entry) );
   int i;
   copy -> next = 0;
   copy -> values = (char **) malloc( sizeof(char *) * n_fields );
   for (i = 0; i < n_fields; i++)
   {
	  (copy -> values)[i] = (char *) malloc( sizeof(char) * ( strlen((source -> values)[i]) + 1 ) );
	  strcpy( (copy -> values)[i], (source -> values)[i] );
   }
   return copy;
};

struct entry *collect(struct entry *database, char **fields_mask)
{
   struct entry *result = 0;
   int flag;
   int i;
   struct entry *list = database;
   
   while (list != 0)
   {
	  flag = 1;
	  for (i = 0; i < n_fields; i++)
		 if ( fields_mask[i] != 0 && strcmp(fields_mask[i], "") != 0 && strcmp( fields_mask[i], (list -> values)[i] ) != 0 )
		 {
			flag = 0;
			break;
		 }

	  if (flag)
		 insert_entry( duplicate(list), &result, LIST_END );
	  list = list -> next;
   } 
   return result;
};

void delete_by_mask(struct entry **database_p, char **fields_mask)
{
   struct entry *list = *database_p;
   int i, position = 0;
   int flag;

   while (list != 0)
   {
	  position++;
	  flag = 1;
	  for (i = 0; i < n_fields; i++)
		 if ( fields_mask[i] != 0 && strcmp(fields_mask[i], "") != 0 && strcmp( fields_mask[i], (list -> values)[i] ) != 0 )
		 {
			flag = 0;
			break;
		 }
	  list = list -> next;
	  if (flag)
	  {
		 delete_by_pos(database_p, position);
		 position--;
	  }
   }
}

void delete_by_pos(struct entry **database_p, int position)
{
   struct entry *list = *database_p;
   struct entry *next_node;
   int i;
   if (list == 0)
	  my_exit("Database if empty!");
   if (position == 1)
   {
	  for (i = 0; i < n_fields; i++)  ;
		// free( (list -> values)[i] );
	  next_node = list -> next;
	  //free(list);
	  *database_p = next_node;
	  return;
   }
   if (position <= 0)
      my_exit("Not a valid position!");
   
   for (i = 1; i <= position - 2; i++)
   {
	  list = list -> next;
	  if (list == 0)
		 my_exit("Incorrect position to delete!");
   }
   if (list -> next == 0)
      my_exit("Incorrect position to delete!");
   for (i = 0; i < n_fields; i++)  ;
	 // free( (list -> next -> values)[i] );
   next_node = list -> next -> next;
   //free(list -> next);
   list -> next = next_node;
}

void read_from(struct entry **database_p, char **name_p, const char *filename)
{
   struct entry *list, *next_node;
   int len, i;
   char *temp;
   FILE *stream;
   stream = 0;
   stream = fopen(filename, "r");
   if (stream == NULL)
	  my_exit("Can't read this file!");

   if ( fscanf(stream, "%d", &len) == 0 || len <= 0)
	  my_exit("Corrupted file or incorrect data!");
   fgetc(stream);
   temp = fget_line(stream);
   *name_p = temp;
   list = *database_p = fread_entry(stream);
   for (i = 2; i <= len; i++)
   {
	  next_node = fread_entry(stream);
	  list -> next = next_node;
	  list = next_node;
   }
   fclose(stream);
}

void save_to(struct entry *database, const char *DB_name, const char *filename)
{
   struct entry *list;
   FILE *stream = fopen(filename, "w");
   if (stream == NULL)
	  my_exit("Can't create an output file!");
   fprintf(stream, "%d\n", length(database));
   fputs(DB_name, stream);
   fputc('\n', stream);
   list = database;
   while (list != 0)
   {
	  fwrite_entry(list, stream);
	  list = list -> next;
   }
   fclose(stream);
}


struct entry *fread_entry(FILE *stream)
{
   struct entry *node = (struct entry *) malloc( sizeof(struct entry) );
   int i;
   node -> values = (char **) malloc( sizeof(char *) * n_fields );
   node -> next = 0;

   for (i = 0; i < n_fields; i++)
	  (node -> values)[i] = fget_line(stream);
   return node;
}

void fwrite_entry(struct entry *node, FILE *stream)
{
   int i;
   for (i = 0; i < n_fields; i++)
   {
	  fputs( (node -> values)[i], stream );
	  fputc('\n', stream);
   }
}

void my_exit(const char *mess)
{
   printf("%s", mess);
   getch();
   exit(1);
}

int length(struct entry *database)
{
   struct entry *list = database;
   int len = 0;
   while (list != 0)
   {
	  len++;
	  list = list -> next;
   }
   return len;
}

char *fget_line(FILE *stream)
{
   static char buf[MAXLEN];
   char *str = 0;
   int len;

   if ( fgets(buf, MAXLEN - 1, stream) == 0 )
	  return NULL;

   len = strlen(buf) + 1;
   str = (char *) mmalloc( len * sizeof(char) );
   strcpy(str, buf);
   if ( str[strlen(str) - 1] == '\n' )
	  str[strlen(str) - 1] = '\0';
   return str;
}

struct entry *get_entry()
{
   struct entry *node = (struct entry *) malloc( sizeof(struct entry) );
   int i;
   node -> values = (char **) malloc( sizeof(char *) * n_fields );
   node -> next = 0;

   printf("Adding new entry..\n");
   for (i = 0; i < n_fields; i++)
   {
	  printf("Input %s\n", fields[i]);
	  (node -> values)[i] = fget_line(stdin);
   }
   return node;
}

void insert_entry(struct entry *node, struct entry **database_p, int position)
{
   struct entry *list, *next_node;
   int i;
   if (position < -1)
   {
	  printf("Incorrect database position\n");
	  return;
   }
   switch (position)
   {
	  case LIST_START:
	  {
		 if (*database_p == 0)
			*database_p = node;
		 else
		 {
			next_node = *database_p;
			*database_p = node;
			node -> next = next_node;
		 }
		 break;
	  }
	  case LIST_END:
	  {
		 if (*database_p == 0)
			*database_p = node;
		 else
		 {
			list = *database_p;
			while (list -> next != 0)
			   list = list -> next;
			list -> next = node;
		 }
		 break;
	  }
	  default:
	  {
		 // Needs to be a correct position
		 if (*database_p == 0)
		 {
			printf("Empty database. Can't append to this position!\n");
			return;
         }

		 list = *database_p;
		 for (i = 1; i <= position - 1; i++)
		 {
			list = list -> next;
			if (list == 0)
			{
			   printf("Incorrect database position\n");
			   return;
            }
		 }
		 next_node = list -> next;
		 list -> next = node;
		 node -> next = next_node;
		 break;
	  }
   }
}

void clear_DB(struct entry **database_p)
{
   int i;
   struct entry *list = *database_p, *_list;
   while (list != 0)
   {
	  for (i = 0; i < n_fields; i++) ;
		// free( (list -> values)[i] );
	  //free(list -> values);
	  _list = list -> next;
	  //free(list);
	  list = _list;
   }
   *database_p = 0;
}

void create_DB(struct entry **database_p, char **name_p, const char *new_name) //Returns handle of DB
{
   int len;
   char *temp;
   if (*name_p != 0)
   {
	 // free(*name_p);
	  *name_p = 0;
   }
   len = strlen(new_name);
   temp = (char *) mmalloc( sizeof(char) * (len + 1) );
   *name_p = temp;
   strcpy(*name_p, new_name);
   clear_DB(database_p);
}

int get_format(char ***fields_p, const char *filename)
{
   char *fget_line(FILE *);
   int i, n_fields = 0;

   FILE *inf = fopen(filename, "r");
   fscanf(inf, "%d", &n_fields);
   *fields_p = (char **) malloc( n_fields * sizeof (char *) );
   fgetc(inf);
   for (i = 0; i < n_fields; i++)
	  (*fields_p)[i] = fget_line(inf);
   fclose(inf);
   return n_fields;
}

void print_all_DB(struct entry *database, const char *DB_name)
{
   int *most_long;
   int i, len;
   struct entry *list = database;

   printf("Database <%s>:\n", DB_name);
   if (database == 0)
   {
	  printf("No items.\n");
	  return;
   }

   // Calculating column's width
   most_long = (int *) malloc( sizeof(int) * n_fields );
   for (i = 0; i < n_fields; i++)
	  most_long[i] = strlen(fields[i]);
   while (list != 0)
   {
	  for (i = 0; i < n_fields; i++)
		 if ( ( len = strlen( (list -> values)[i]) ) > most_long[i] )
			most_long[i] = len;
	  list = list -> next;
   }
   //
   list = database;
   for (i = 0; i < n_fields; i++)
		 printf( " %-*s|", most_long[i] + offset, fields[i] );
   putchar('\n');

   while (list != 0)
   {
	  for (i = 0; i < n_fields; i++)
		 printf( " %-*s|", most_long[i] + offset, (list -> values)[i] );
	  putchar('\n');
	  list = list -> next;
   }

   //free(most_long);
}


