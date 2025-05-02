#include "database.h"


unsigned int hash(const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * 31) + (*str++);
    }
    return hash % 100; 
}

bool search_record(Hashmap* db, const char* table_name, int student_number) {
    unsigned int index = hash(table_name);
    Table* table = db->tables[index];

    if (table == NULL) {
        return false;
    }

    Record* current = table->head;

    while (current != NULL) {
        if (current->student_number == student_number) {
            return true; 
        }
        current = current->next;
    }

    return false;
}

Record* get_sorted_copy(Hashmap* db, const char* table_name) {
    unsigned int index = hash(table_name);
    Table* table = db->tables[index];
    Record* copy_head = copy_list(table->head);
    return merge_sort(copy_head);
}

Record* copy_list(Record* head) {
    if (head == NULL) {
        return NULL;
    }

    Record* new_head = (Record*)malloc(sizeof(Record));
    new_head->student_number = head->student_number;
    strcpy(new_head->general_course_name, head->general_course_name);
    strcpy(new_head->general_course_instructor, head->general_course_instructor);
    new_head->general_course_score = head->general_course_score;
    strcpy(new_head->core_course_name, head->core_course_name);
    strcpy(new_head->core_course_instructor, head->core_course_instructor);
    new_head->core_course_score = head->core_course_score;
    new_head->prev = NULL;
    new_head->next = NULL;

    Record* current = new_head;
    Record* original_current = head->next;

    while (original_current != NULL) {
        Record* new_node = (Record*)malloc(sizeof(Record));
        new_node->student_number = original_current->student_number;
        strcpy(new_node->general_course_name, original_current->general_course_name);
        strcpy(new_node->general_course_instructor, original_current->general_course_instructor);
        new_node->general_course_score = original_current->general_course_score;
        strcpy(new_node->core_course_name, original_current->core_course_name);
        strcpy(new_node->core_course_instructor, original_current->core_course_instructor);
        new_node->core_course_score = original_current->core_course_score;
        new_node->prev = current;
        new_node->next = NULL;
        current->next = new_node;
        current = new_node;
        original_current = original_current->next;
    }

    return new_head;
}

Record* merge_sort(Record* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    Record* front = NULL;
    Record* back = NULL;
    split_list(head, &front, &back);

    front = merge_sort(front);
    back = merge_sort(back);

    return sorted_merge(front, back);
}

Record* sorted_merge(Record* a, Record* b) {
    if (a == NULL) return b;
    if (b == NULL) return a;

    Record* result = NULL;
    if (a->student_number <= b->student_number) {
        result = a;
        result->next = sorted_merge(a->next, b);
    } else {
        result = b;
        result->next = sorted_merge(a, b->next);
    }

    return result;
}

void split_list(Record* source, Record** front_ref, Record** back_ref) {
    Record* slow = source;
    Record* fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front_ref = source;
    *back_ref = slow->next;
    slow->next = NULL;
}

void print_matched_records(Record* sorted_list, const char* col_name, const char* value) {
    Record* current = sorted_list;

    while (current != NULL) {
        bool match = false;
        if (strcmp(col_name, "student_number") == 0) {
            match = (atoi(value) == current->student_number);
        } else if (strcmp(col_name, "general_course_name") == 0) {
            match = (strcmp(value, current->general_course_name) == 0);
        } else if (strcmp(col_name, "general_course_instructor") == 0) {
            match = (strcmp(value, current->general_course_instructor) == 0);
        } else if (strcmp(col_name, "general_course_score") == 0) {
            match = (atoi(value) == current->general_course_score);
        } else if (strcmp(col_name, "core_course_name") == 0) {
            match = (strcmp(value, current->core_course_name) == 0);
        } else if (strcmp(col_name, "core_course_instructor") == 0) {
            match = (strcmp(value, current->core_course_instructor) == 0);
        } else if (strcmp(col_name, "core_course_score") == 0) {
            match = (atoi(value) == current->core_course_score);
        }
        if (match) {
            printf("Student Number: %d\n", current->student_number);
            printf("General Course: %s | Instructor: %s | Score: %d\n",
                   current->general_course_name,
                   current->general_course_instructor,
                   current->general_course_score);
            printf("Core Course: %s | Instructor: %s | Score: %d\n\n",
                   current->core_course_name,
                   current->core_course_instructor,
                   current->core_course_score);
        }
        current = current->next;
    }
}

Table* create_table(Hashmap* db, const char* table_name) {
    unsigned int index = hash(table_name);
    if (db->tables[index]) {
        return NULL;  
    }

    Table* table = (Table*)malloc(sizeof(Table));
    strcpy(table->name, table_name);
    table->head = table->tail = NULL;

    db->tables[index] = table;
    db->size++;
    return table;
}

Table* delete_table(Hashmap* db, const char* table_name) {
    unsigned int index = hash(table_name);
    Table* table = db->tables[index];
    if (!table) return NULL;

    Record* current = table->head;
    while (current) {
        Record* temp = current;
        current = current->next;
        free(temp);
    }
	Table* tem = table;
    free(table);
    db->tables[index] = NULL;
    db->size--;
    return tem;
}

void init_hashmap(Hashmap* map) {
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        map->tables[i] = NULL;
    }
}


RBNode* create_rbnode(RBTree* tree, int key, Record* record) {
    RBNode* new_node = (RBNode*)malloc(sizeof(RBNode));
    new_node->key = key;
    new_node->record = record;
    new_node->color = RED;
    new_node->left = tree->nil;
    new_node->right = tree->nil;
    new_node->parent = tree->nil;
    return new_node;
}

RBTree* create_rbtree() {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    tree->nil = (RBNode*)malloc(sizeof(RBNode));
    tree->nil->color = BLACK;
    tree->root = tree->nil;
    return tree;
}

void left_rotate(RBTree* tree, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void right_rotate(RBTree* tree, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    if (x->right != tree->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == tree->nil) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void rb_insert_fixup(RBTree* tree, RBNode* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void rb_insert(RBTree* tree, int key, Record* record) {
    RBNode* z = create_rbnode(tree, key, record);
    RBNode* y = tree->nil;
    RBNode* x = tree->root;

    while (x != tree->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == tree->nil) {
        tree->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    rb_insert_fixup(tree, z);
}


RBTree* create_index(Hashmap* db, const char* table_name) {
	unsigned int index = hash(table_name);
	Table* table = db->tables[index];
    if (!table) {
        return NULL;  
    }
    
    RBTree* tree = create_rbtree();
    Record* current = table->head;
    while (current != NULL) {
        rb_insert(tree, current->student_number, current);
        current = current->next;
    }
	table->rb_index = tree;
    return tree;
}

int add_record(Hashmap* db, const char* table_name, Record* record) {
	unsigned int index_table = hash(table_name);
	Table* table = db->tables[index_table];
	if (!table)
		return -1;
	
	if (search_record(db, table_name, record->student_number))
		return 0;	

    if (!table->head) {
        table->head = table->tail = record;
    } else {
        table->tail->next = record;
        record->prev = table->tail;
        table->tail = record;
    }
	if (table->rb_index)
		rb_insert(table->rb_index, record->student_number, record);
    
    return 1;
}

void delete_record(Hashmap* db, const char* table_name, const char* col_name, const char* value) {
    unsigned int index = hash(table_name);
    Table* table = db->tables[index];

    if (!table) {
        printf("[ERROR] Table '%s' not found.\n", table_name);
        return;
    }

    Record* current = table->head;
    bool record_deleted = false;

    while (current != NULL) {
        Record* next = current->next;

        bool match = false;
        if (strcmp(col_name, "student_number") == 0) {
            if (atoi(value) == current->student_number) {
                match = true;
            }
        } else if (strcmp(col_name, "general_course_name") == 0) {
            if (strcmp(value, current->general_course_name) == 0) {
                match = true;
            }
        } else if (strcmp(col_name, "general_course_instructor") == 0) {
            if (strcmp(value, current->general_course_instructor) == 0) {
                match = true;
            }
        } else if (strcmp(col_name, "general_course_score") == 0) {
            if (atoi(value) == current->general_course_score) {
                match = true;
            }
        } else if (strcmp(col_name, "core_course_name") == 0) {
            if (strcmp(value, current->core_course_name) == 0) {
                match = true;
            }
        } else if (strcmp(col_name, "core_course_instructor") == 0) {
            if (strcmp(value, current->core_course_instructor) == 0) {
                match = true;
            }
        } else if (strcmp(col_name, "core_course_score") == 0) {
            if (atoi(value) == current->core_course_score) {
                match = true;
            }
        } else {
            printf("[ERROR] Invalid column name '%s'.\n", col_name);
            return;
        }

        if (match) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                table->head = current->next;  
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                table->tail = current->prev; 
            }
            free(current);
            record_deleted = true;
        }

        current = next;
    }
	if (table->rb_index != NULL) {
        create_index(db, table_name);
    }
    if (record_deleted) {
        printf("[SUCCESS] Record(s) with '%s' = '%s' deleted from table '%s'.\n", col_name, value, table_name);
    } else {
        printf("[INFO] No records with '%s' = '%s' found in table '%s'.\n", col_name, value, table_name);
    }
}


void update_record(Hashmap* db, const char* table_name, const char* col_name, const char* value, const char* new_value) {
    unsigned int index = hash(table_name);
    Table* table = db->tables[index];

    if (table == NULL) {
        printf("[ERROR] Table '%s' not found.\n", table_name);
        return;
    }

    Record* current = table->head;
    bool record_updated = false;

    while (current != NULL) {
        bool match = false;

  		 if (strcmp(col_name, "general_course_name") == 0) {
            if (strcmp(value, current->general_course_name) == 0) {
                strncpy(current->general_course_name, new_value, COL_NAME_MAX_LEN);
                match = true;
            }
        } else if (strcmp(col_name, "general_course_instructor") == 0) {
            if (strcmp(value, current->general_course_instructor) == 0) {
                strncpy(current->general_course_instructor, new_value, COL_NAME_MAX_LEN);
                match = true;
            }
        } else if (strcmp(col_name, "general_course_score") == 0) {
            if (atoi(value) == current->general_course_score) {
                current->general_course_score = atoi(new_value);
                match = true;
            }
        } else if (strcmp(col_name, "core_course_name") == 0) {
            if (strcmp(value, current->core_course_name) == 0) {
                strncpy(current->core_course_name, new_value, COL_NAME_MAX_LEN);
                match = true;
            }
        } else if (strcmp(col_name, "core_course_instructor") == 0) {
            if (strcmp(value, current->core_course_instructor) == 0) {
                strncpy(current->core_course_instructor, new_value, COL_NAME_MAX_LEN);
                match = true;
            }
        } else if (strcmp(col_name, "core_course_score") == 0) {
            if (atoi(value) == current->core_course_score) {
                current->core_course_score = atoi(new_value);
                match = true;
            }
        } else {
            printf("[ERROR] Invalid column name '%s'.\n", col_name);
            return;
        }

        if (match) {
            record_updated = true;
        }

        current = current->next;
    }

    if (record_updated) {
        printf("[SUCCESS] Record(s) in table '%s' updated: '%s' = '%s' -> '%s'.\n", table_name, col_name, value, new_value);
    } else {
        printf("[INFO] No records in table '%s' matched '%s' = '%s'.\n", table_name, col_name, value);
    }
}


void inorder_rbtree_walk(RBNode* node, const char* col_name, const char* value) {
    if (node == NULL) return;

    inorder_rbtree_walk(node->left, col_name, value);

    Record* record = node->record;
    bool match = false;

    if (strcmp(col_name, "student_number") == 0) {
        match = (atoi(value) == record->student_number);
    } else if (strcmp(col_name, "general_course_name") == 0) {
        match = (strcmp(value, record->general_course_name) == 0);
    } else if (strcmp(col_name, "general_course_instructor") == 0) {
        match = (strcmp(value, record->general_course_instructor) == 0);
    } else if (strcmp(col_name, "general_course_score") == 0) {
        match = (atoi(value) == record->general_course_score);
    } else if (strcmp(col_name, "core_course_name") == 0) {
        match = (strcmp(value, record->core_course_name) == 0);
    } else if (strcmp(col_name, "core_course_instructor") == 0) {
        match = (strcmp(value, record->core_course_instructor) == 0);
    } else if (strcmp(col_name, "core_course_score") == 0) {
        match = (atoi(value) == record->core_course_score);
    }

    if (match) {
        printf("Student Number: %d\n", record->student_number);
        printf("General Course: %s | Instructor: %s | Score: %d\n",
               record->general_course_name,
               record->general_course_instructor,
               record->general_course_score);
        printf("Core Course: %s | Instructor: %s | Score: %d\n\n",
               record->core_course_name,
               record->core_course_instructor,
               record->core_course_score);
    }

    inorder_rbtree_walk(node->right, col_name, value);
}

void select(Hashmap* db, const char* table_name, const char* col_name, const char* value, bool sorted) {
    unsigned int index = hash(table_name);
    Table* table = db->tables[index];

    if (table == NULL) {
        printf("[ERROR] Table '%s' not found.\n", table_name);
        return;
    }

    if (sorted) {
        if (table->rb_index == NULL) {
        	print_matched_records(get_sorted_copy(db, table_name), col_name, value);
        	return;
        }
        inorder_rbtree_walk(table->rb_index->root, col_name, value);
    } else {
        Record* current = table->head;
        while (current != NULL) {
            bool match = false;

            if (strcmp(col_name, "student_number") == 0) {
                match = (atoi(value) == current->student_number);
            } else if (strcmp(col_name, "general_course_name") == 0) {
                match = (strcmp(value, current->general_course_name) == 0);
            } else if (strcmp(col_name, "general_course_instructor") == 0) {
                match = (strcmp(value, current->general_course_instructor) == 0);
            } else if (strcmp(col_name, "general_course_score") == 0) {
                match = (atoi(value) == current->general_course_score);
            } else if (strcmp(col_name, "core_course_name") == 0) {
                match = (strcmp(value, current->core_course_name) == 0);
            } else if (strcmp(col_name, "core_course_instructor") == 0) {
                match = (strcmp(value, current->core_course_instructor) == 0);
            } else if (strcmp(col_name, "core_course_score") == 0) {
                match = (atoi(value) == current->core_course_score);
            }

            if (match) {
                printf("Student Number: %d\n", current->student_number);
                printf("General Course: %s | Instructor: %s | Score: %d\n",
                       current->general_course_name,
                       current->general_course_instructor,
                       current->general_course_score);
                printf("Core Course: %s | Instructor: %s | Score: %d\n\n",
                       current->core_course_name,
                       current->core_course_instructor,
                       current->core_course_score);
            }

            current = current->next;
        }
    }
}
