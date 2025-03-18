#include "types.h"
#include "user.h"

// This program simply retrieves the current process's ID and prints it to the screen
// This is the program I used to trace the getpid() system call with GDB
// It served as the starting and ending point in the user-space
int main() {  
  int pid = getpid();
  printf(1, "My PID is %d\n", pid);
  exit();
}
