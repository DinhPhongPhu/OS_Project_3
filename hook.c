#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/pgtable_types.h>
#include <linux/highmem.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
#include <linux/fdtable.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Viet_Phu_Son");


void **system_call_table_addr;

//New open syscall
asmlinkage int (*open) (const char*, int, umode_t);      

//New write syscall
asmlinkage ssize_t (*write) (unsigned int, const char*, size_t);        

asmlinkage int new_open(const char* filename, int flags, umode_t mode)
{
        if(strstr(filename,"in.txt")!=NULL){
                printk(KERN_INFO "New write syscall\n");
                printk(KERN_INFO "Calling process: %s\n",current->comm);
                printk(KERN_INFO "Opend file name: %s\n", filename);
        }
 	return open(filename, flags, mode);
}

asmlinkage ssize_t new_write(unsigned int fd, const char* buf, size_t len)
{

 	char *tmp;
    char *pathname;
    struct file *file;
    struct path *path;

    spin_lock(&current->files->file_lock);
    file = fcheck_files(current->files, fd);
    if (!file) {
            spin_unlock(&current->files->file_lock);
            return -ENOENT;
    }

        path = &file->f_path;
        path_get(path);
        spin_unlock(&current->files->file_lock);

        tmp = (char *)__get_free_page(GFP_KERNEL);

        if (!tmp) {
                path_put(path);
                return -ENOMEM;
        }

        pathname = d_path(path, tmp, PAGE_SIZE);
        path_put(path);
	if (IS_ERR(pathname)) {
                free_page((unsigned long)tmp);
                return PTR_ERR(pathname);
        }

	ssize_t bytes;
	bytes=(*write)(fd,buf,len);
	
        

        printk(KERN_INFO "New write syscall\n");
        printk(KERN_INFO "Calling process: %s\n", current->comm);
        printk(KERN_INFO "Write to %s %d bytes\n",pathname,bytes);

        free_page((unsigned long)tmp);
        return bytes;
}

int make_rw(unsigned long address){
 	unsigned int level;
 	pte_t *pte = lookup_address(address, &level);
 	if(pte->pte &~_PAGE_RW){
 	      pte->pte |=_PAGE_RW;
        }
        return 0;
}

int make_ro(unsigned long address){
 	unsigned int level;
 	pte_t *pte = lookup_address(address, &level);
 	pte->pte = pte->pte & ~_PAGE_RW;
        return 0;
}

static int __init entry_point(void){
 	printk(KERN_INFO "Captai Hook loaded successfully!\n");
 	system_call_table_addr = (void*)0xffffffff82000300;

 	open = system_call_table_addr[__NR_open];
 	write = system_call_table_addr[__NR_write];

 	make_rw((unsigned long)system_call_table_addr);

 	system_call_table_addr[__NR_open] = new_open;
	system_call_table_addr[__NR_write] = new_write;
        return 0;
}

static void __exit exit_point(void){
        printk(KERN_INFO "Unloaded Captain Hook successfully\n");
	system_call_table_addr[__NR_open] = open;
	system_call_table_addr[__NR_write] = write;
	make_ro((unsigned long)system_call_table_addr);
}

module_init(entry_point);
module_exit(exit_point);