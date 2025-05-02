#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define COL_NAME_MAX_LEN 32
#define HASHMAP_SIZE 100
#define RED 1
#define BLACK 0


typedef struct Hashmap Hashmap;

typedef struct Record {
    int student_number;          
    char general_course_name[COL_NAME_MAX_LEN];        
    char general_course_instructor[COL_NAME_MAX_LEN]; 
    int general_course_score;            
    char core_course_name[COL_NAME_MAX_LEN];           
    char core_course_instructor[COL_NAME_MAX_LEN];     
    int core_course_score;              

    struct Record* prev;               
    struct Record* next;               
} Record;

typedef struct RBNode {
    int key;
    Record* record;
    int color;
    struct RBNode* left;
    struct RBNode* right;
    struct RBNode* parent;
} RBNode;

typedef struct RBTree {
    RBNode* root;
    RBNode* nil;
} RBTree;

typedef struct Table {
    char name[COL_NAME_MAX_LEN];       
    Record* head;           
    Record* tail;            
	RBTree* rb_index;     
} Table;


typedef struct Hashmap {
    Table* tables[HASHMAP_SIZE];  
    int size;                     
} Hashmap;


unsigned int hash(const char* str);
void init_hashmap(Hashmap* map);
Table* create_table(Hashmap* db, const char* table_name);
Table* delete_table(Hashmap* db, const char* table_name);
int add_record(Hashmap* db, const char* table_name, Record* record);
void delete_record(Hashmap* db, const char* table_name, const char* col_name, const char* value);
void update_record(Hashmap* db, const char* table_name, const char* col_name, const char* value, const char* new_value);
void rb_insert(RBTree* tree, int key, Record* record);
void right_rotate(RBTree* tree, RBNode* y);
void left_rotate(RBTree* tree, RBNode* x);
RBTree* create_rbtree();
RBNode* create_rbnode(RBTree* tree, int key, Record* record);
void rb_insert_fixup(RBTree* tree, RBNode* z);
void inorder_rbtree_walk(RBNode* node, const char* col_name, const char* value);
void select(Hashmap* db, const char* table_name, const char* col_name, const char* value, bool sorted);
Record* copy_list(Record* head);
Record* merge_sort(Record* head);
Record* sorted_merge(Record* a, Record* b);
void split_list(Record* source, Record** front_ref, Record** back_ref);
void print_matched_records(Record* sorted_list, const char* col_name, const char* value);
#endif
