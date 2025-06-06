# sql3-jaja 

**sql3-jaja** is a cli tool to track financial expenses using the **sqlite3 library in C**. The project was made for learning purposes. Project is not meant to be cross-platform, subsequently I can't vouch for non-Ubuntu Linux users.

## soon to come
1. sanitize all inputs (only float amounts, unsigned int id's, not null texts, DD-MM-YYYY dates)
2. revamp command format from **"verb CRLF arg1 CRLF arg2 ..."** to **"verb arg1 arg2 arg3 ..."**
3. create a custom database file (not just jaja.database)
4. changing only the values you want while editExpense
5. cannot remove the category if an expense(s) uses it
## how to use

1. clone the repo.
2. run the `sql3-jaja` executable in terminal.
3. type `help` for the command list.

## why this project

My future iot project will need a fast RDBMS to store MQTT client metadata. After thorough research, using the `sqlite3.h` C library turned out to be the best choice for these reasons:
1. SQLite is designed for a single writer usage with concurrency limitations, perfectly fitting my single-user product.
2. SQLite stores the database in a disk file making it very simple, I'm not trying to overcomplicate storing simple metadata.
3. SQLite is very fast on its own, with the fact that it will be implemented as a C library makes it the fastest possible RDBMS solution (in my project context obviously).

sql3-jaja helped me get solid practice with low-level code (as it's my first ever C project) + RDBMS. Learned a lot about error handling measures, strings in C, sqlite3_stmt preparing->binding->stepping->finalizing routine and building user-friendly CLI. You may argue that sql3-jaja uses very basic queries, but "complicated" SQL will be applied on my future project itself (This decision is based on the fact that both projects are for learning purposes anyways, so things I didn't learn here, will anyways be covered in depth in future).