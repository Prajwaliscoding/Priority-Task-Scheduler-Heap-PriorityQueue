# Priority Task Scheduler (C, Heap / Priority Queue)

A simple yet powerful **command-line task manager** built in **C**, using a **Min-Heap (Priority Queue)** data structure to schedule and complete tasks based on their **priority and deadlines**.

This project demonstrates practical use of **data structures and algorithms (DSA)** concepts — especially heaps, dynamic arrays, and sorting — implemented entirely from scratch in C.

---

## Features

✅ **Data Structure:**

- Implements a **Min-Heap / Priority Queue** using a dynamic array.
- Insertions and deletions maintain the heap property in **O(log n)** time.

✅ **Task Management:**

- Add new tasks with description, priority, and optional deadline.
- View, complete, or list tasks in ascending order of priority.
- Save and load all tasks from a `.csv` file for persistence.

✅ **Command-Line Interface:**

- Intuitive text-based interaction.
- Lightweight, portable, and fully written in C.

---

## 🗂️ Folder Structure

```text
Priority-Task-Scheduler-Heap-PriorityQueue/
│
├── include/
│   ├── heap.h
│   ├── io.h
│   └── util.h
│
├── src/
│   ├── heap.c
│   ├── io.c
│   ├── util.c
│   └── main.c
│
└── Makefile
```

⚙️ Build Instructions
Make sure you have gcc installed. (On macOS, it’s included with Xcode Command Line Tools.)

```bash
make
```

▶️ Run

```bash
./scheduler
```

You should see:

```text
Priority Task Scheduler — type 'help' for commands.
>
```

🧾 Commands

```text
Command	                             Description
add "<desc>" <priority> <deadline>	Adds a new task (deadline optional)
pop	                                Removes and prints the highest-priority task
peek	                            Shows the next task without removing it
list	                            Displays all pending tasks (sorted by priority)
save <file.csv>	                    Saves all tasks to a CSV file
load <file.csv>	                    Loads tasks from a CSV file (merges if exists)
clear	                            Deletes all tasks
help	                            Displays command help
quit	                            Exits the program
```

🧩 Example Session

```text
Priority Task Scheduler — type 'help' for commands.
> add "Fix login bug" 1 2025-10-10
Added: "Fix login bug" (priority 1, deadline 2025-10-10)
> add "Write report" 3 2025-10-12
Added: "Write report" (priority 3, deadline 2025-10-12)
> add "Email client" 2 2025-10-07
Added: "Email client" (priority 2, deadline 2025-10-07)

> peek
Next: [1] 2025-10-10   Fix login bug
> list
 1. [1] 2025-10-10   Fix login bug
 2. [2] 2025-10-07   Email client
 3. [3] 2025-10-12   Write report

> pop
Completed: "Fix login bug" (priority 1, deadline 2025-10-10)
> save tasks.csv
Saved tasks to tasks.csv
> clear
Cleared all tasks.
> load tasks.csv
Loaded tasks from tasks.csv
> list
 1. [2] 2025-10-07   Email client
 2. [3] 2025-10-12   Write report
> quit
```

💾 Data Structure Overview

```c
typedef struct {
    char desc[128];
    int  priority;       // smaller = higher priority
    char deadline[32];
} Task;
```

Heap Structure

```c
typedef struct {
    Task* arr;
    int size;
    int capacity;
} Heap;
```

Time Complexities

```text
Operation	Complexity
Insert	O(log n)
Delete / Pop	O(log n)
Peek	O(1)
Build Heap	O(n)
List All (sorted)	O(n log n)
```
