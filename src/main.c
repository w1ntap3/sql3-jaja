//removeCategory doesnt work if an expense uses that category. removecategory FORCED removes the category AND expenses that stored that category.

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
            char date[11] = "DD-MM-YYYY";
            char description[MAX_LENGTH];
            unsigned int day = 316972, month = 316972, year = 316972;

            sscanf(input, "newExpense %s %s %d-%d-%d '%[^']'", amountStr, category, &day,&month,&year, description);
            if(
                !strcmp(amountStr, " ") ||
                !strcmp(category, " ") ||
                (day == 316972 || !(day <= 31 && day >=0)) ||
                (month == 316972 || !(month <= 12 && month >=1)) ||
                (year == 316972 || !(year <= 9999 && year >=0))
            )
            {
                    fprintf(stderr, "Invalid syntax. \"help\" to see the valid syntax.\n");
                    continue;
            }

            snprintf(date, sizeof(date), "%02d-%02d-%04d", day, month, year);
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
            char newCategory[MAX_LENGTH];
            char newDate[11] = "DD-MM-YYYY";
            char newDescription[MAX_LENGTH];
            unsigned int day = 316972, month = 316972, year = 316972;

            char inputExpenseId[MAX_LENGTH];
            int expenseId;

            const char *editQuery = 
            "UPDATE expenses "
            "SET amount=?, category_id=?, date=?, description=? "
            "WHERE id = ?;";

            sscanf(input, "editExpense %s %s %s %d-%d-%d '%[^']'", inputExpenseId, newAmount, newCategory, &day, &month, &year, newDescription);

            if(
                !strcmp(newAmount, " ") ||
                !strcmp(newCategory, " ") ||
                (day == 316972 || !(day <= 31 && day >=0)) ||
                (month == 316972 || !(month <= 12 && month >=1)) ||
                (year == 316972 || !(year <= 9999 && year >=0))
            )
            {
                    fprintf(stderr, "Invalid syntax. \"help\" to see the valid syntax.\n");
                    continue;
            }

            snprintf(newDate, sizeof(newDate), "%02d-%02d-%04d", day, month, year);

            expenseId = atoi(inputExpenseId);

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
            if (sqlite3_bind_text(cat_stmt, 1, newCategory, -1, SQLITE_TRANSIENT) != SQLITE_OK)
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
                sqlite3_bind_text(get_id_stmt, 1, newCategory, -1, SQLITE_TRANSIENT);
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

            if (sqlite3_prepare_v2(db,editQuery,-1,&editStmt,NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to compile the statement. %s\n", sqlite3_errmsg(db));
                continue;
            }

            sqlite3_bind_double(editStmt, 1, atof(newAmount));
            sqlite3_bind_int(editStmt, 2, categoryId);
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
            // statement for cat removal
            sqlite3_stmt *rmCstmt;
            const char *rmQuery = "DELETE FROM categories WHERE id=?;";

            // input args
            char inputCatName[MAX_LENGTH];
            char forced[MAX_LENGTH];

            // Category ID found through name
            int foundId = -1;

            sscanf(input, "removeCategory %s %s", inputCatName, forced);            
            // statement for cat id via cat name
            sqlite3_stmt *getIdstmt;
            const char *getIdQuery = "SELECT id from categories WHERE name = ?;";
            
            if (sqlite3_prepare_v2(db, getIdQuery, -1, &getIdstmt, 0) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to prepare the getId statement. %s\n", sqlite3_errmsg(db));
                continue;
            }

            if (sqlite3_bind_text(getIdstmt, 1, inputCatName, -1, NULL) != SQLITE_OK) {
                fprintf(stderr, "Failed to bind the Id to the statement. %s\n", sqlite3_errmsg(db));
                continue;
            }
            
            int stepResult = sqlite3_step(getIdstmt);

            if (stepResult == SQLITE_ROW) {
                foundId = sqlite3_column_int(getIdstmt, 0); // successful step --> found an id
            } else if (stepResult == SQLITE_DONE) {
                printf("No such category named \"%s\"", inputCatName); // successful step --> category doesnt exist
                continue;
            } else {
                fprintf(stderr, "Failed to properly execute the getId statement. %s\n", sqlite3_errmsg(db)); // failed step (error)
                continue;
            }

            sqlite3_finalize(getIdstmt);

            // statement for removing expense records
            sqlite3_stmt *removeEstmt;
            const char *removeEquery = "DELETE FROM expenses WHERE category_id = ?;";
            
            if (sqlite3_prepare_v2(db, removeEquery, -1, &removeEstmt, 0) != SQLITE_OK) {
                fprintf(stderr, "Failed to prepare the check statement. %s\n", sqlite3_errmsg(db));
                continue;
            }

            if (sqlite3_bind_int(removeEstmt, 1, foundId) != SQLITE_OK) {
                fprintf(stderr, "Failed to bind ID to the statement. %s\n", sqlite3_errmsg(db));
                continue;
            }

            if (sqlite3_prepare_v2(db, rmQuery, -1, &rmCstmt, NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to prepare a statement. %s\n", sqlite3_errmsg(db));
                continue;
            }

            if (sqlite3_bind_int(rmCstmt, 1, foundId) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to bind ID to the statement. %s\n", sqlite3_errmsg(db));
                continue;
            }
            
            if (!strcmp(forced, "FORCED")) {
                if (sqlite3_step(removeEstmt) == SQLITE_DONE) {
                printf("Successfully removed all expense records with \"%s\".\n", inputCatName);
                }

                if (sqlite3_step(rmCstmt) == SQLITE_DONE)
                {
                    printf("Successfully removed category \"%s\"(ID %d)\n", inputCatName, foundId);
                    return SQLITE_OK;
                } else
                {
                    fprintf(stderr, "Failed to execute the statement. %s\n", sqlite3_errmsg(db));
                    return SQLITE_ERROR;
                }

                sqlite3_finalize(removeEstmt);
                sqlite3_finalize(rmCstmt);
            } else {
                sqlite3_finalize(removeEstmt);

                sqlite3_stmt *anyRowsStmt;
                const char *anyRowsQuery = "SELECT * FROM expenses WHERE category_id = ?;";
                int anyRows = -1;

                sqlite3_prepare_v2(db,anyRowsQuery,-1,&anyRowsStmt, 0);
                sqlite3_bind_int(anyRowsStmt, 1, foundId);
                if (sqlite3_step(anyRowsStmt) == SQLITE_ROW) {
                    anyRows = 0; // Found rows
                } else {
                    anyRows = 1; // No rows
                }
                sqlite3_finalize(anyRowsStmt);

                if (anyRows) {
                    if (sqlite3_step(rmCstmt) == SQLITE_DONE)
                    {
                        printf("Successfully removed category \"%s\"(ID %d)\n", inputCatName, foundId);
                        return SQLITE_OK;
                    } else
                    {
                        fprintf(stderr, "Failed to execute the statement. %s\n", sqlite3_errmsg(db));
                        return SQLITE_ERROR;
                    }

                    sqlite3_finalize(rmCstmt);
                } else {
                    printf(
                        "Cannot remove the category \"%s\": There are expense records with this category.\n"
                        "(Try \"removeCategory [CATEGORY_NAME] FORCED\" to remove both category and expenses)", inputCatName);
                }
            }
        } else if (!strcmp(verb, "help"))
        {
            printf(
                "| %-60s | %45s |\n"
                "================================================================================================================\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n"
                "| %-60s | %45s |\n",
                "Command", "Description",
                "newExpense [AMOUNT] [CATEGORY] [DATE (in DD-MM-YYYY ", "Create a new expense record",
                "format only)] '[DESCRIPTION]'", "",
                "removeExpense [ID]", "Remove an expense record",
                "editExpense [ID] [AMOUNT] [CATEGORY] [DATE] '[DESCRIPTION]'", "Edit an expense record",
                "newCategory [NAME]", "Create a new expense category record",
                "removeCategory [ID]", "Remove a category record (Will not pass ",
                "", "through if category is used somewhere)",
                "removeCategory [ID] forced", "Removes a category record + all the expenses",
                "", "with that category",
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