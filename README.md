# sql3-jaja

**sql3-jaja** is a cli tool to create, query, export, and visualize financial expenses table using the `sqlite3.h` library in C. The project was made for learning purposes. Project is not meant to be cross-platform, subsequently I can't vouch for non-Ubuntu Linux users.

## soon to come
* [] allow only properly DD-MM-YYYY formatted date values.
* [] revamp command format (from inputting each argument to "verb arg1 arg2 arg3 ...")

## how to use

1. clone the repo.
2. run the `sql3-jaja` executable in your terminal.
3. type `help` for the command list.
4. manipulate your table fields and records as you wish.
6. query

## why this project

My future iot project will need a fast RDBMS to store sensor metadata. After thorough research, using the `sqlite3.h` C library turned out to be the best choice for certain reasons. sql3-jaja helped me get solid practice with low-level code (as it's my first ever C project) + relational database management systems with SQLite.