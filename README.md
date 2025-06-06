# sql3-jaja v0.1.1

**sql3-jaja** is a cli tool to track financial expenses using the **sqlite3 library in C**. The project was made for learning purposes. Project is not meant to be cross-platform, subsequently I can't vouch for non-Ubuntu Linux users.

## v0.1.1
1. sanitized all argument input
2. revamped the command format from asking each argument manually to a single line syntax **"verb arg1 arg2 arg3 ..."**
3. you get to create a custom database disk file (not just jaja.database)
4. enhanced removeCategory command to support forced removal of categories and associated expenses
5. better help

## how to use

1. clone the repo.
2. run the `sql3-jaja` executable in terminal.
3. type `help` for the command list, syntax and functions.

## why this project

My future IoT project will need a fast RDBMS to store MQTT client metadata. After thorough research, using the `sqlite3.h` C library turned out to be the best choice for these reasons:
1. SQLite is designed for a single writer usage with concurrency limitations, perfectly fitting my single-user product.
2. SQLite stores the database in a disk file making it very simple. It would be highly unprofessional to overcomplicate storing simple metadata.
3. SQLite is very fast on its own, with the fact that it will be implemented as a C library makes it the fastest possible RDBMS solution (in my project context obviously).

sql3-jaja helped me get solid practice with low-level code (as it's my first ever C project) + RDBMS. Learned a lot about error handling measures, strings in C, sqlite3_stmt preparing->binding->stepping->finalizing routine, sanitizing, safety and building (somewhat) user-friendly CLI. You may not be impressed with sql3-jaja very basic queries, but "complicated" SQL will be applied on my future project itself (This decision is based on the fact that both projects are for learning purposes anyways, so things I didn't learn here, will anyways be covered in depth in future).