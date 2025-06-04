#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_LENGTH 1024

int main(int argc, char *argv[]) 
{
    char input[MAX_LENGTH * 4];
    char *verb;

    sqlite3 *db;
    char *err_msg = NULL;

    int rc = sqlite3_open("jaja.database", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Could not initialize SQLite3 database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Create tables
    const char *createTablesSQL =
        "CREATE TABLE IF NOT EXISTS categories ("
        "  id INTEGER PRIMARY KEY NOT NULL, "
        "  name TEXT NOT NULL UNIQUE);"
        "CREATE TABLE IF NOT EXISTS expenses ("
        "  id INTEGER PRIMARY KEY NOT NULL, "
        "  amount REAL NOT NULL, "
        "  category_id INTEGER NOT NULL, "
        "  date TEXT NOT NULL, "
        "  description TEXT, "
        "  FOREIGN KEY (category_id) REFERENCES categories(id));";

    rc = sqlite3_exec(db, createTablesSQL, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Could not initialize tables: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    while (1) {
        printf(">> ");
        if (!fgets(input, sizeof(input), stdin)) continue;
        input[strcspn(input, "\n")] = '\0'; // remove newline

        verb = strtok(input, " ");
        if (!verb) continue;

        if (!strcmp(verb, "newExpense")) {
            char amountStr[MAX_LENGTH];
            char category[MAX_LENGTH];
            char date[MAX_LENGTH];
            char description[MAX_LENGTH];

            printf("1. How much money did you spend? ");
            fgets(amountStr, sizeof(amountStr), stdin);
            amountStr[strcspn(amountStr, "\n")] = '\0';

            printf("\n2. To which category did you spend the money? ");
            fgets(category, sizeof(category), stdin);
            category[strcspn(category, "\n")] = '\0';

            printf("\n3. When did the expenditure happen? ");
            fgets(date, sizeof(date), stdin);
            date[strcspn(date, "\n")] = '\0';

            printf("\n4. Describe the expense. ");
            fgets(description, sizeof(description), stdin);
            description[strcspn(description, "\n")] = '\0';

            sqlite3_stmt *cat_stmt;
            rc = sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO categories (name) VALUES (?);", -1, &cat_stmt, 0);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to prepare category insert: %s\n", sqlite3_errmsg(db));
                continue;
            }
            sqlite3_bind_text(cat_stmt, 1, category, -1, SQLITE_TRANSIENT);
            sqlite3_step(cat_stmt);
            sqlite3_finalize(cat_stmt);

            sqlite3_stmt *get_id_stmt;
            int categoryId = -1;

            rc = sqlite3_prepare_v2(db, "SELECT id FROM categories WHERE name = ?;", -1, &get_id_stmt, 0);
            if (rc == SQLITE_OK) {
                sqlite3_bind_text(get_id_stmt, 1, category, -1, SQLITE_TRANSIENT);
                if (sqlite3_step(get_id_stmt) == SQLITE_ROW) {
                    categoryId = sqlite3_column_int(get_id_stmt, 0);
                }
                sqlite3_finalize(get_id_stmt);
            }

            if (categoryId == -1) {
                fprintf(stderr, "Could not retrieve category ID.\n");
                continue;
            }

            sqlite3_stmt *expense_stmt;
            rc = sqlite3_prepare_v2(db,
                "INSERT INTO expenses (amount, category_id, date, description) VALUES (?, ?, ?, ?);",
                -1, &expense_stmt, 0);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to prepare expense insert: %s\n", sqlite3_errmsg(db));
                continue;
            }

            sqlite3_bind_double(expense_stmt, 1, atof(amountStr));
            sqlite3_bind_int(expense_stmt, 2, categoryId);
            sqlite3_bind_text(expense_stmt, 3, date, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(expense_stmt, 4, description, -1, SQLITE_TRANSIENT);

            rc = sqlite3_step(expense_stmt);
            if (rc != SQLITE_DONE) {
                fprintf(stderr, "Failed to insert expense: %s\n", sqlite3_errmsg(db));
            } else {
                printf("Expense added successfully.\n");
            }

            sqlite3_finalize(expense_stmt);
        } else if (!strcmp(verb, "removeExpense")) {
            // removeExpense logic
        } else if (!strcmp(verb, "newCategory")) {
            // newCategory logic
        } else if (!strcmp(verb, "help")) {
            printf(
                " %-15s  | %-45s \n"
                "===================================================================\n"
                "| %-15s | %-45s |\n"
                "| %-15s | %-45s |\n"
                "| %-15s | %-45s |\n"
                "| %-15s | %-45s |\n"
                "| %-15s | %-45s |\n"
                "| %-15s | %-45s |\n"
                "===================================================================\n",
                "Command", "Description",
                "newExpense", "Create a new expense record",
                "removeExpense", "Remove an expense record",
                "newCategory", "Create a new expense category record",
                "website", "Open the website for visualizing your database",
                "quit", "Kill sql3-jaja",
                "help", "List of all commands"
            );
        } else if (!strcmp(verb, "quit")) {
            sqlite3_close(db);
            printf("Your session is over.\n");
            return(1);
        } else if (!strcmp(verb, "website")) {
            system("xdg-open ../sql3-jaja/website/index.html > /dev/null 2>&1");
        }
    }

    sqlite3_close(db);
    return 0;
}