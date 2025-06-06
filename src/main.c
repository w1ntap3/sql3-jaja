//revamp command format from "verb CRLF arg1 CRLF arg2 ..." to "verb arg1 arg2 arg3 ..."
//sanitize all inputs (only float amounts, unsigned int id's, not null texts, DD-MM-YYYY dates)
//removeCategory doesnt work if an expense uses that category. removecategory force removes the category AND expenses that stored that category.
//changing only the values you want while editExpense

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_LENGTH 1024

int main(int argc, char *argv[])
{
    char input[MAX_LENGTH * 4];
    char tempInput[MAX_LENGTH * 4];
    char dbName[MAX_LENGTH];
    char *verb;

    sqlite3 *db;
    char *err_msg = NULL;

    int rc;
    printf(
        "WELCOME TO SQL3-JAJA\n"
        "====================\n");
    
    while (1) {
        printf("Please enter the database disk file name (located in sql3-jaja/) >> ");
        if (!fgets(dbName, sizeof(dbName), stdin)) continue;
        dbName[strcspn(dbName, "\n")] = '\0';

        if (!strcmp(dbName, "")) 
        {
            printf("Cannot use an empty filename. Try again.\n"); 
            continue; 
        } else break;
    }

    char dbPath[MAX_LENGTH];
    snprintf(dbPath, MAX_LENGTH*2, "db/%s", dbName);
    
    if (sqlite3_open(dbPath, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Could not initialize SQLite3 database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    } else 
    {
        printf("Successfully opened \"%s\" database file\n", dbName);
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

    while (1)
    {
        printf("sql3-jaja >> ");
        if (!fgets(input, sizeof(input), stdin)) continue;
        input[strcspn(input, "\n")] = '\0'; // remove newline

        verb = strtok(strcpy(tempInput, input), " ");

        if (!verb) continue;
        if (!strcmp(verb, "newExpense"))
        {
            
            char amountStr[MAX_LENGTH];
            char category[MAX_LENGTH];
            char date[MAX_LENGTH];
            char description[MAX_LENGTH];

            sscanf(input, "newExpense %s %s %s '%[^']'", amountStr, category, date, description);

            if(!strcmp(amountStr, "") || !strcmp(category, "") || !strcmp(date, ""))
            {
                    fprintf(stderr, "Empty argument(s) are not valid. Try again.\n");
                    continue;
            }

            // statement compile variables
            const char *expenseQuery = "INSERT OR IGNORE INTO categories (name) VALUES (?);";
            sqlite3_stmt *cat_stmt;

            // prepare statement
            if (sqlite3_prepare_v2(db, expenseQuery, -1, &cat_stmt, 0) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to prepare category insert: %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(cat_stmt);
                continue;
            }

            // bind input to the statement
            if (sqlite3_bind_text(cat_stmt, 1, category, -1, SQLITE_TRANSIENT) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to bind category insert: %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(cat_stmt);
                continue;
            }

            // execute the statement
            if (sqlite3_step(cat_stmt) != SQLITE_DONE)
            {
                fprintf(stderr, "Failed to execute category insert: %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(cat_stmt);
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

            sscanf(input, "removeExpense %s", inputRmId);
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

            sscanf(input, "editExpense %s %s %s %s '%[^']'", inputExpenseId, newAmount, newCategoryId, newDate, newDescription);

            if(!strcmp(newAmount, "") || !strcmp(newCategoryId, "") || !strcmp(newDate, ""))
            {
                    fprintf(stderr, "Empty argument(s) are not valid. Try again.\n");
                    continue;
            }

            if (!atoi(inputExpenseId) || !atof(newAmount) || !atoi(newCategoryId)) 
            {
                printf("Invalid arguments. Try again.");
                continue;
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
            sscanf(input, "removeExpense %s", categoryName);

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
                "===============================================================================================================\n"
                "| %-59s | %45s |\n"
                "===============================================================================================================\n"
                "| %-59s | %45s |\n"
                "| %-59s | %45s |\n"
                "| %-59s | %45s |\n"
                "| %-59s | %45s |\n"
                "| %-59s | %45s |\n"
                "| %-59s | %45s |\n"
                "| %-59s | %45s |\n"
                "| %-59s | %45s |\n"
                "===============================================================================================================\n",
                "Command", "Description",
                "newExpense [AMOUNT] [CATEGORY] [DATE] '[DESCRIPTION]'", "Create a new expense record",
                "removeExpense [ID]", "Remove an expense record",
                "editExpense [ID] [AMOUNT] [CATEGORY] [DATE] '[DESCRIPTION]'", "Edit an expense record",
                "newCategory [NAME]", "Create a new expense category record",
                "removeCategory [ID]", "Remove an expense category record",
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