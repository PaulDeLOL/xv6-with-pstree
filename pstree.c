#include "types.h"
#include "stat.h"
#include "user.h"
#include "uproc.h"

// Per the assignment instructions, this program assumes the system has less than 64 processes
#define MAX_PROCESSES 64
#define MAX_CHILDREN 16

// Blank process array containing a max of 64
struct uproc procs[MAX_PROCESSES];
int process_count;

// Node structure for tree building
struct node
{
  struct uproc *proc;
  int child_count;
  struct node *children[MAX_CHILDREN];
};

// Array of nodes to assemble the tree, as well as the root that points to a node
struct node nodes[MAX_PROCESSES];
struct node *root = 0;

// Function prototypes
void fetch_processes();
void build_tree();
void print_tree(struct node *n, int depth);

int main() {
  fetch_processes();
  build_tree();

  // If a root process (like init) exists, print the process tree
  // Otherwise, print an error message
  if (root)
    print_tree(root, 0);
  else
    printf(2, "Error: root process not found\n");
  
  exit();
}

// Calls the getprocs() system call to fill the procs array with all the currently
// running processes this instant
void fetch_processes()
{
  process_count = getprocs(MAX_PROCESSES, procs);

  // If the process count was less than 0, the system call failed and returned -1
  if (process_count < 0) {
    printf(2, "Error: getprocs system call failed\n");
    exit();
  }
}

// Builds the process tree
void build_tree()
{
  // Creates a flat, linear array of nodes for each process with 0 children each
  for (int i = 0; i < process_count; i++)
  {
    nodes[i].proc = &procs[i];
    nodes[i].child_count = 0;
  }

  // Links each node to its parent (if it has one), skipping fatherless processes
  for (int i = 0; i < process_count; i++)
  {
    if (procs[i].ppid == 0)
      continue;

    for (int j = 0; j < process_count; j++)
    {
      if (procs[i].ppid == procs[j].pid)
      {
        if (nodes[j].child_count < MAX_CHILDREN)
          nodes[j].children[nodes[j].child_count++] = &nodes[i];

        break;
      }
    }
  }

  // Looks for the process with pid 1 (init) and assigns root to point to it
  for (int i = 0; i < process_count; i++)
  {
    if (procs[i].pid == 1)
    {
      root = &nodes[i];
      break;
    }
  }
}

// Prints the process tree recursively in a depth-first fashion
void print_tree(struct node *n, int depth)
{
  if (!n)
    return;

  for (int i = 0; i < depth; i++)
    printf(1, "   ");
  printf(1, "%s[%d]\n", n->proc->name, n->proc->pid);

  for (int i = 0; i < n->child_count; i++)
    print_tree(n->children[i], depth+1);
}
