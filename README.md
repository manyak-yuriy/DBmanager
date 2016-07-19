# DBmanager

Console-based Database management system.

# Help

help
   Show help.

append
   Initiate a dialog to input a new item and put it to the end.

add <number>
   Initiate a dialog to input a new item and put it to custom positon.

print
   Print out the entire database.

create
   Create a new database. The current one is not saved.

clear
   Clear current database.

save <filename>
   Save current database to a file.

load <filename>
   Load a database from a custom file.

printf <field_1 = value_1> ... <field_n = value_n>
   Print out items which have custom values of the fields.

delete <field_1 = value_1> ... <field_n = value_n>
   Delete items which have custom values of the fields.

sort <field_1> <-|+> ... <field_n> <-|+>
   Sort the last printed items according to the values of fields in lexicographical order and print them again.
