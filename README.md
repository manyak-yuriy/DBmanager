# DBmanager

Console-based Database management system.

# Help

<code>help</code>
   Show help.

<code>append</code>
   Initiate a dialog to input a new item and put it to the end.

<code>add number</code>
   Initiate a dialog to input a new item and put it to custom positon.

<code>print</code>
   Print out the entire database.

<code>create DBname</code>
   Create a new database. The current one is not saved.

<code>clear</code>
   Clear current database.

<code>save <filename> </code>
   Save current database to a file.

<code>load <filename> </code>
   Load a database from a custom file.

<code>printf <field_1 = value_1> ... <field_n = value_n> </code>
   Print out items which have custom values of the fields.

<code>delete <field_1 = value_1> ... <field_n = value_n> </code>
   Delete items which have custom values of the fields.

<code>sort <field_1> <-|+> ... <field_n> <-|+>
   Sort the last printed items according to the values of fields in lexicographical order and print them again.
