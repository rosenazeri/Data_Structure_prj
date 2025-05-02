#include "database.c"

void handle_command(Hashmap* db, char* command) {
    char action[COL_NAME_MAX_LEN], table_name[COL_NAME_MAX_LEN], column_name[COL_NAME_MAX_LEN], value[COL_NAME_MAX_LEN], new_value[COL_NAME_MAX_LEN];

    if (sscanf(command, "CREATE TABLE %31s", table_name) == 1) {
        Table* table = create_table(db ,table_name);
        if (table) {
        	db->tables[hash(table_name)] = table;
        	printf("[SUCCESS] Table '%s' created.\n", table_name);
		}
		else
		    printf("[ERROR] Table '%s' already exists.\n", table_name);
    }
    else if (sscanf(command, "DELETE TABLE %31s", table_name) == 1) {
    	if (delete_table(db, table_name))
        	printf("[SUCCESS] Table '%s' deleted.\n", table_name);
        else 
        	printf("[ERROR] Table '%s' does not exist.\n", table_name);
    }
    else if (sscanf(command, "CREATE INDEX %31s", table_name) == 1) {
        if (create_index(db, table_name))
            printf("[SUCCESS] Table '%s' is indexed.\n", table_name);
   		else
            printf("[ERROR] Table '%s' not found.\n", table_name);
    }
	else if (strncmp(command, "ADD", 3) == 0) {
	    char table_name[COL_NAME_MAX_LEN];
	    if (sscanf(command, "ADD %31s", table_name) != 1) {
	        printf("[ERROR] Syntax error: Missing table name.\n");
	        return;
	    }
	
	    Record* record = (Record*)malloc(sizeof(Record));
	    record->student_number = -1;
	    strcpy(record->general_course_name, "");
	    strcpy(record->general_course_instructor, "");
	    record->general_course_score = -1;
	    strcpy(record->core_course_name, "");
	    strcpy(record->core_course_instructor, "");
	    record->core_course_score = -1;
	    record->prev = record->next = NULL;
	
	    char* token = strtok(command + strlen("ADD ") + strlen(table_name), " ");
	    char column_name[COL_NAME_MAX_LEN];
	    char value[COL_NAME_MAX_LEN];
	    int matched_columns = 0;
	
	    while (token) {
	        if (sscanf(token, "%31s", column_name) == 1) {
	            token = strtok(NULL, " ");
	            if (!token || sscanf(token, "%31s", value) != 1) {
	                printf("[ERROR] Syntax error: Missing value for column '%s'.\n", column_name);
	                free(record);
	                return;
	            }
	
	            if (strcmp(column_name, "student_number") == 0) {
	                record->student_number = atoi(value);
	                matched_columns++;
	            } else if (strcmp(column_name, "general_course_name") == 0) {
	                strncpy(record->general_course_name, value, COL_NAME_MAX_LEN - 1);
	                matched_columns++;
	            } else if (strcmp(column_name, "general_course_instructor") == 0) {
	                strncpy(record->general_course_instructor, value, COL_NAME_MAX_LEN - 1);
	                matched_columns++;
	            } else if (strcmp(column_name, "general_course_score") == 0) {
	                record->general_course_score = atoi(value);
	                matched_columns++;
	            } else if (strcmp(column_name, "core_course_name") == 0) {
	                strncpy(record->core_course_name, value, COL_NAME_MAX_LEN - 1);
	                matched_columns++;
	            } else if (strcmp(column_name, "core_course_instructor") == 0) {
	                strncpy(record->core_course_instructor, value, COL_NAME_MAX_LEN - 1);
	                matched_columns++;
	            } else if (strcmp(column_name, "core_course_score") == 0) {
	                record->core_course_score = atoi(value);
	                matched_columns++;
	            } else {
	                printf("[ERROR] Unknown column '%s'.\n", column_name);
	                free(record);
	                return;
	            }
	        }
	        token = strtok(NULL, " ");
	    }
	
	    if (matched_columns != 7) {
	        printf("[ERROR] Missing required column values.\n");
	        free(record);
	        return;
	    }
	
	    int res = add_record(db, table_name, record);
	    if (res == -1) {
	        printf("[ERROR] Table '%s' not found.\n", table_name);
	        free(record);
	    } else if (res == 0) {
	        printf("[ERROR] Student number must be unique.\n");
	        free(record);
	    } else {
	        printf("[SUCCESS] Added record '%d' to the table '%s'.\n", record->student_number, table_name);
	    }
	}

	
    else if (sscanf(command, "DELETE %31s %31s %31s", table_name, column_name, value) == 3) {
    	delete_record(db, table_name, column_name, value);
    }
    else if (sscanf(command, "UPDATE %31s %31s %31s %31s", table_name, column_name, value, new_value) == 4) {
        	update_record(db, table_name, column_name, value, new_value);
        }
	else if (strncmp(command, "SELECT", 6) == 0) {
    char table_name[COL_NAME_MAX_LEN], column_name[COL_NAME_MAX_LEN], value[COL_NAME_MAX_LEN];
    int sorted = 0;

    if (strstr(command, "SORTED")) {
        if (sscanf(command, "SELECT %31s %31s %31s SORTED", table_name, column_name, value) == 3) {
            sorted = 1;  
            select(db, table_name, column_name, value, sorted);
        } else {
            printf("[ERROR] Syntax Error in SELECT with SORTED.\n");
        }
    } else {
        if (sscanf(command, "SELECT %31s %31s %31s", table_name, column_name, value) == 3) {
            select(db, table_name, column_name, value, sorted);
        } else {
            printf("[ERROR] Syntax Error in SELECT.\n");
        }
    }
}
    else {
        printf("[ERROR] Invalid Command.\n");
    }
}

int main() {
	printf("===== ROSE NAZERI DATABASE =====\n\n");
    
    Hashmap db;
    init_hashmap(&db);

    char command[256];

    while (true) {
        printf("\n> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; 

        if (strcmp(command, "EXIT") == 0) {
            printf("[INFO] EXIT PROGRAM...\n");
            break;
        }

        handle_command(&db, command);
    }
	return 0;
}
