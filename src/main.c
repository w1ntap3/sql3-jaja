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

    int rc;

    if (sqlite3_open("jaja.database", &db) != SQLITE_OK)
    {
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

    if (sqlite3_exec(db, createTablesSQL, 0, 0, &err_msg) != SQLITE_OK)
    {
        fprintf(stderr, "Could not initialize tables: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    while (31)
    {
        printf("sql3-jaja >> ");
        if (!fgets(input, sizeof(input), stdin)) continue;
        input[strcspn(input, "\n")] = '\0'; // remove newline

        verb = strtok(input, " ");

        if (!verb) continue;
        if (!strcmp(verb, "newExpense"))
        {
            // input variables
            char amountStr[MAX_LENGTH];
            char category[MAX_LENGTH];
            char date[MAX_LENGTH];
            char description[MAX_LENGTH];

            // statement compile variables
            const char *expenseQuery = "INSERT OR IGNORE INTO categories (name) VALUES (?);";
            sqlite3_stmt *cat_stmt;

            // inputs
            while (31) {
                printf("1. How much money did you spend? >> ");
                fgets(amountStr, sizeof(amountStr), stdin);
                amountStr[strcspn(amountStr, "\n")] = '\0';

                if(!strcmp(amountStr, "")) {
                    fprintf(stderr, "Empty amount of money is not valid. Try again.\n");
                    continue;
                } else
                {
                    break;
                }
            }
            
            while (31) {
                printf("\n2. To which category did you spend the money? >> ");
                fgets(category, sizeof(category), stdin);
                category[strcspn(category, "\n")] = '\0';

                if(!strcmp(category, "")) {
                    fprintf(stderr, "Empty category ID is not valid. Try again.\n");
                    continue;
                } else
                {
                    break;
                }
            }
            
            while (31) {
                printf("\n3. When did the expenditure happen? >> ");
                fgets(date, sizeof(date), stdin);
                date[strcspn(date, "\n")] = '\0';

                if(!strcmp(date, "")) {
                    fprintf(stderr, "Empty category ID is not valid. Try again.\n");
                    continue;
                } else
                {
                    break;
                }
            }
            
            printf("\n4. Describe the expense. >> ");
            fgets(description, sizeof(description), stdin);
            description[strcspn(description, "\n")] = '\0';

            // prepare statement
            if (sqlite3_prepare_v2(db, expenseQuery, -1, &cat_stmt, 0) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to prepare category insert: %s\n", sqlite3_errmsg(db));
                continue;
            }

            // bind input to the statement
            if (sqlite3_bind_text(cat_stmt, 1, category, -1, SQLITE_TRANSIENT) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to bind category insert: %s\n", sqlite3_errmsg(db));
                continue;
            }

            // execute the statement
            if (sqlite3_step(cat_stmt) != SQLITE_DONE)
            {
                fprintf(stderr, "Failed to execute category insert: %s\n", sqlite3_errmsg(db));
                continue;
            }

            // free
            sqlite3_finalize(cat_stmt);

            sqlite3_stmt *get_id_stmt;
            int categoryId = -1;

            if (sqlite3_prepare_v2(db, "SELECT id FROM categories WHERE name = ?;", -1, &get_id_stmt, 0) == SQLITE_OK)
            {
                sqlite3_bind_text(get_id_stmt, 1, category, -1, SQLITE_TRANSIENT);
                if (sqlite3_step(get_id_stmt) == SQLITE_ROW)
                {
                    categoryId = sqlite3_column_int(get_id_stmt, 0);
                }
                sqlite3_finalize(get_id_stmt);
            }

            if (categoryId == -1)
            {
                fprintf(stderr, "Could not retrieve category ID.\n");
                continue;
            }

            sqlite3_stmt *expense_stmt;
            rc = sqlite3_prepare_v2(db,
                                    "INSERT INTO expenses (amount, category_id, date, description) VALUES (?, ?, ?, ?);",
                                    -1, &expense_stmt, 0);

            if (rc != SQLITE_OK)
            {
                fprintf(stderr, "Failed to prepare expense insert: %s\n", sqlite3_errmsg(db));
                continue;
            }

            sqlite3_bind_double(expense_stmt, 1, atof(amountStr));
            sqlite3_bind_int(expense_stmt, 2, categoryId);
            sqlite3_bind_text(expense_stmt, 3, date, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(expense_stmt, 4, description, -1, SQLITE_TRANSIENT);

            if (sqlite3_step(expense_stmt) != SQLITE_DONE)
            {
                fprintf(stderr, "Failed to insert expense: %s\n", sqlite3_errmsg(db));
            }
            else
            {
                printf("Expense added successfully.\n");
            }

            sqlite3_finalize(expense_stmt);
        } else if (!strcmp(verb, "removeExpense"))
        {
            sqlite3_stmt *rmEstmt;
            const char *rmQuery = "DELETE FROM expenses WHERE (id=?);";
            char inputRmId[MAX_LENGTH];
            int rmId;

            printf("ID of the expense you would like to remove >> ");
            if(!fgets(inputRmId, MAX_LENGTH, stdin)) continue;
            inputRmId[strcspn(inputRmId, "\n")] = '\0';
            rmId = atoi(inputRmId);

            if (sqlite3_prepare_v2(db, rmQuery, -1, &rmEstmt, NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to prepare a statement. %s\n", sqlite3_errmsg(db));
                continue;
            }

            if (sqlite3_bind_int(rmEstmt, 1, rmId) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to bind ID to the statement. %s\n", sqlite3_errmsg(db));
                continue;
            }

            if (sqlite3_step(rmEstmt) != SQLITE_DONE)
            {
                fprintf(stderr, "Failed to execute the statement. %s\n", sqlite3_errmsg(db));
                continue;
            } else
            {
                printf("Successfully removed expense with ID \"%d\"\n", rmId);
            }

            sqlite3_finalize(rmEstmt);
        } else if (!strcmp(verb, "editExpense"))
        {
            sqlite3_stmt *editStmt;

            char newAmount[MAX_LENGTH];
            char newCategoryId[MAX_LENGTH];
            char newDate[MAX_LENGTH];
            char newDescription[MAX_LENGTH];

            char inputExpenseId[MAX_LENGTH];
            int expenseId;

            const char *editQuery = 
            "UPDATE expenses "
            "SET amount=?, category_id=?, date=?, description=? "
            "WHERE id = ?;";

            while (31) {
                printf("Which expense do you want to edit? (Provide the ID) >> ");
                if (!fgets(inputExpenseId, MAX_LENGTH, stdin)) continue;
                inputExpenseId[strcspn(inputExpenseId,"\n")] = '\0';

                if (!atoi(inputExpenseId)) continue; else break;
            }

            while (31) {
                printf("How much did you spend >> ");
                if (!fgets(newAmount, MAX_LENGTH, stdin)) continue;
                newAmount[strcspn(newAmount,"\n")] = '\0';

                if (!atof(newAmount)) continue; else break;
            }

            while (31) {
                printf("To what category? (provide Category ID) >> ");
                if (!fgets(newCategoryId, MAX_LENGTH, stdin)) continue;
                newCategoryId[strcspn(newCategoryId,"\n")] = '\0';

                if (!atoi(newCategoryId)) continue; else break;
            }

            while (31) {
                printf("When did it happen >> ");
                if (!fgets(newDate, MAX_LENGTH, stdin)) continue; else break;
                newDate[strcspn(newDate,"\n")] = '\0';
            }

            while (31) {
                printf("Describe the expense >> ");
                if (!fgets(newDescription, MAX_LENGTH, stdin)) continue; else break;
                newDescription[strcspn(newDescription,"\n")] = '\0';
            }

            expenseId = atoi(inputExpenseId);

            if (sqlite3_prepare_v2(db,editQuery,-1,&editStmt,NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to compile the statement. %s\n", sqlite3_errmsg(db));
                continue;
            }

            sqlite3_bind_double(editStmt, 1, atof(newAmount));
            sqlite3_bind_int(editStmt, 2, atoi(newCategoryId));
            sqlite3_bind_text(editStmt, 3, newDate, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(editStmt, 4, newDescription, -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(editStmt, 5, expenseId);

            if (sqlite3_step(editStmt) != SQLITE_DONE)
            {
                fprintf(stderr, "Failed to execute the statement. %s\n", sqlite3_errmsg(db));
                continue;
            } else
            {
                printf("Successfully edited expense with id \"%d\"\n", expenseId);
            }
            
            sqlite3_finalize(editStmt);
        } else if (!strcmp(verb, "newCategory"))
        {
            // input, statement and query variables
            char categoryName[MAX_LENGTH];
            sqlite3_stmt *categoryStmt;
            const char *categoryQuery = "INSERT INTO categories (name) VALUES (?);";

            // Ask for a category
            printf("What is the name of the new category? >> ");
            if (!fgets(categoryName, MAX_LENGTH, stdin)) continue;
            categoryName[strcspn(categoryName, "\n")] = '\0';

            if (!strcmp(categoryName, ""))
            {
                fprintf(stderr, "Failed to initiate category creation process. Empty category name is not valid.\n");
                continue;
            }
            // compile the sqlite3 statement
            if (sqlite3_prepare_v2(db, categoryQuery, -1, &categoryStmt, NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to prepare a category insert statement: %s\n", sqlite3_errmsg(db));
                continue;
            }
            // bind the inserted category to the statement
            if (sqlite3_bind_text(categoryStmt, 1, categoryName, -1, SQLITE_TRANSIENT) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to bind the category name into the statement: %s\n", sqlite3_errmsg(db));
                continue;
            }
            // execute the compiled statement with error handling
            if (sqlite3_step(categoryStmt) != SQLITE_DONE)
            {
                fprintf(stderr, "Failed to create a new category: %s\n", sqlite3_errmsg(db));
                continue;
            }
            else
            {
                fprintf(stderr, "Successfully created category \"%s\"\n", categoryName);
                continue;
            }

            // clean up
            sqlite3_finalize(categoryStmt);
        } else if (!strcmp(verb, "removeCategory"))
        {
            sqlite3_stmt *rmEstmt;
            const char *rmQuery = "DELETE FROM categories WHERE (id=?);";
            char inputRmId[MAX_LENGTH];
            int rmId;

            printf("ID of the category you would like to remove >> ");
            if(!fgets(inputRmId, MAX_LENGTH, stdin)) continue;
            inputRmId[strcspn(inputRmId, "\n")] = '\0';
            rmId = atoi(inputRmId);

            if (sqlite3_prepare_v2(db, rmQuery, -1, &rmEstmt, NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to prepare a statement. %s\n", sqlite3_errmsg(db));
                continue;
            }

            if (sqlite3_bind_int(rmEstmt, 1, rmId) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to bind ID to the statement. %s\n", sqlite3_errmsg(db));
                continue;
            }

            if (sqlite3_step(rmEstmt) != SQLITE_DONE)
            {
                fprintf(stderr, "Failed to execute the statement. %s\n", sqlite3_errmsg(db));
                continue;
            } else
            {
                printf("Successfully removed category with ID \"%d\"\n", rmId);
            }

            sqlite3_finalize(rmEstmt);
        } else if (!strcmp(verb, "help"))
        {
            printf(
                " %-15s  | %-45s \n"
                "===================================================================\n"
                "| %-15s | %-45s |\n"
                "| %-15s | %-45s |\n"
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
                "editExpense", "Remove an expense record",
                "newCategory", "Create a new expense category record",
                "removeCategory", "Remove an expense category record",
                "website", "Website for visualizing your database",
                "kill", "Kill sql3-jaja",
                "help", "List of all commands");
        } else if (!strcmp(verb, "kill"))
        {
            sqlite3_close(db);
            printf("Your session is over.\n");
            return (1);
        } else if (!strcmp(verb, "website"))
        {
            system("xdg-open ../sql3-jaja/website/index.html > /dev/null 2>&1");
        }
    }

    sqlite3_close(db);
    return 0;
}