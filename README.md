# 🗄️ Mini Database Engine — Data Structure Final Project

A lightweight, in-memory relational database engine written in **C**, built as a final project for a Data Structures and Algorithm Design course. The engine supports table management, CRUD operations, indexing, and sorted queries — all through a custom command-line interface.

---

## 📁 Project Structure

```
Data_Structure_prj/
└── DB/
    ├── database.h      # Data structure definitions and function declarations
    ├── database.c      # Core database logic implementation
    ├── main.c          # CLI command parser and entry point
    └── test.txt        # Sample commands for testing
```

---

## 🧩 Core Data Structures

| Structure | Role |
|---|---|
| **Hashmap** | Stores and retrieves tables by name using a hash function (size: 100 buckets) |
| **Doubly Linked List** | Holds records within each table in insertion order |
| **Red-Black Tree (RBTree)** | Optional index on `student_number` for O(log n) sorted access |
| **Merge Sort** | Used for sorted queries when no RB-Tree index exists |

---

## 📦 Record Schema

Every record in a table follows a fixed schema with exactly **7 fields**:

| Field | Type | Description |
|---|---|---|
| `student_number` | `int` | Unique student identifier |
| `general_course_name` | `string` | Name of the general course |
| `general_course_instructor` | `string` | Instructor of the general course |
| `general_course_score` | `int` | Score in the general course |
| `core_course_name` | `string` | Name of the core course |
| `core_course_instructor` | `string` | Instructor of the core course |
| `core_course_score` | `int` | Score in the core course |

> **Note:** `student_number` must be unique within a table. Max string length is 31 characters.

---

## ⚙️ Build & Run

### Requirements
- GCC compiler
- Linux / macOS (or WSL on Windows)

### Compile
```bash
cd DB
gcc main.c -o database
```

### Run
```bash
./database
```

You will see:
```
===== ROSE NAZERI DATABASE =====

>
```

---

## 💬 Supported Commands

### Table Management

```bash
# Create a new table
CREATE TABLE <table_name>

# Delete a table and all its records
DELETE TABLE <table_name>

# Build a Red-Black Tree index on student_number for a table
CREATE INDEX <table_name>
```

### Record Operations

```bash
# Add a record (all 7 fields are required, order matters)
ADD <table_name> student_number <val> general_course_name <val> general_course_instructor <val> general_course_score <val> core_course_name <val> core_course_instructor <val> core_course_score <val>

# Delete all records matching a column value
DELETE <table_name> <column_name> <value>

# Update all records where column matches value
UPDATE <table_name> <column_name> <old_value> <new_value>
```

### Query

```bash
# Select records matching a condition
SELECT <table_name> <column_name> <value>

# Select records sorted by student_number
# (uses RB-Tree if indexed, otherwise falls back to merge sort)
SELECT <table_name> <column_name> <value> SORTED
```

### Exit

```bash
EXIT
```

---

## 🧪 Example Session

The following commands are based on `test.txt`:

```bash
CREATE TABLE studentsRB
CREATE TABLE students
CREATE INDEX studentsRB

ADD studentsRB student_number 40231016 general_course_name Physics general_course_instructor Ali general_course_score 14 core_course_name Math core_course_instructor Reza core_course_score 9
ADD studentsRB student_number 40231012 general_course_name Physics general_course_instructor Ali general_course_score 14 core_course_name Math core_course_instructor Reza2 core_course_score 11

ADD students student_number 40231016 general_course_name Physics general_course_instructor Ali3 general_course_score 14 core_course_name Math core_course_instructor Reza core_course_score 13
ADD students student_number 40231022 general_course_name Physics general_course_instructor Ali3 general_course_score 14 core_course_name Math core_course_instructor Reza2 core_course_score 11

DELETE students core_course_score 8
UPDATE studentsRB general_course_name Physics Math2

SELECT students general_course_score 14 SORTED
SELECT studentsRB core_course_name Math SORTED
SELECT students core_course_score 13
EXIT
```

You can run the test file directly:
```bash
./database < test.txt
```

---

## 🔍 How It Works

### Hashing
Tables are stored in a fixed-size hashmap (100 buckets). The hash function multiplies a rolling hash by 31 for each character, inspired by Java's `String.hashCode()`.

### Indexing (Red-Black Tree)
Running `CREATE INDEX` on a table builds an RB-Tree keyed on `student_number`. After that, `ADD` automatically keeps the index up to date. If a record is deleted, the index is fully rebuilt.

### Sorted SELECT
- **With index:** performs an in-order traversal of the RB-Tree → O(n log n) build, O(n) traversal
- **Without index:** deep-copies the linked list and runs merge sort → O(n log n)

---

## ⚠️ Limitations

- Data is **in-memory only** — all records are lost on exit.
- Schema is **fixed** — all tables share the same 7-column student record structure.
- No support for multi-word values (spaces not allowed in field values).
- Hashmap has no collision chaining — two table names that hash to the same bucket will conflict.
- `UPDATE` cannot change `student_number` (to preserve index integrity).

---

## 👩‍💻 Author

**Rose Nazeri**  
Data Structures and Algorithm Design — Final Project
