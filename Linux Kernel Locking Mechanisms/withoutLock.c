#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/delay.h>


static char buf[80];

ssize_t my_read(struct file *fp, char *c, size_t i, loff_t *k) 
{
    copy_to_user(c, buf, i);
    printk ("Myread is invoked...");
    return 0;
}

ssize_t my_write(struct file *fp, const char *c, size_t i, loff_t *k) 
{
    printk("Start write %d",task_pid_nr(current));
    copy_from_user(buf, c, i);
    mdelay(10000);
    printk ("Write done..pid = %d",task_pid_nr(current));
    return 0;
}

int my_open(struct inode *i, struct file *fp)
{
    printk ("Myopen is invoked....");
    printk ("Major number: %d", MAJOR(i->i_rdev));
    printk ("Minor number: %d", MINOR(i->i_rdev));
    return 0;
}

int my_close(struct inode *i, struct file *fp) 
{
    printk ("Myclose is called...");
    return 0;
}

static struct file_operations fops = {
 read: my_read,
 write: my_write,
 open: my_open, 
 release: my_close
};

static int major; 

int my_init(void) 
{

    major = register_chrdev(0, "mydevice", &fops);
    printk ("Major number is : %d", major); 
    printk ("Module insertion is successful.....");
    return 0;
}

void my_cleanup(void) 
{
    unregister_chrdev(major, "mydevice");
    printk ("Module removal is successful .....");
}

module_init(my_init);
module_exit(my_cleanup);

MODULE_LICENSE("GPL");

