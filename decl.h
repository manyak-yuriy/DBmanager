/*
	Yuriy Manyak
	KA-23
	IASA-2013
*/

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <alloc.h>

#define MAXLEN 256

enum list_position
{
   LIST_END = -1, LIST_START = 0
};

struct entry
{
   char **values;
   struct entry *next;
};

extern char big[];
extern char *big_p;

extern char **fields;
extern int n_fields;

extern void my_exit(const char *mess);
extern int length(struct entry *database);
extern char *fget_line(FILE *stream);
extern void insert_entry(struct entry *node, struct entry **database_p, int position);
extern void clear_DB(struct entry **database_p);
extern void create_DB(struct entry **database_p, char **name_p, const char *new_name);
extern int get_format(char ***fields_p, const char *filename);
extern void print_all_DB(struct entry *database, const char *DB_name);
extern struct entry *get_entry(void);

extern void fwrite_entry(struct entry *node, FILE *stream);
extern struct entry *fread_entry(FILE *stream);
extern void save_to(struct entry *database, const char *DB_name, const char *filename);
extern void read_from(struct entry **database_p, char **name_p, const char *filename);
extern void delete_by_pos(struct entry **database_p, int position);
extern void delete_by_mask(struct entry **database_p, char **fields_mask);

extern struct entry *collect(struct entry *database, char **fields_mask);
extern struct entry *duplicate(const struct entry *source);
extern int compare(const struct entry *entry_1, const struct entry *entry_2,
			const char **criterias, const char *directions, int n);

extern void unlink_by_pos(struct entry **database_p, int position);
extern void unlink_next(struct entry *node);
extern struct entry *get_sorted_copy(struct entry *list,
							  const char **criterias, const char *directions, int n);

char *mmalloc(int size);
