// Struct that holds the data needed for pstree program
// Holds the process's ID, the ID of the parent, and the process name
struct uproc {
  int pid;
  int ppid;
  char name[16];
};

// New system call prototype
// Takes in a max amount of processes and a pointer to an array of structs to hold them
int getprocs(int max, struct uproc *table);
