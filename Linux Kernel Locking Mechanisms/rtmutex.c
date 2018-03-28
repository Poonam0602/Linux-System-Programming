#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/rtmutex.h>
#include <linux/delay.h>

static DEFINE_RT_MUTEX(test_rtmutex);
static char buf[80];

ssize_t my_read(struct file *fp, char *c, size_t i, loff_t *k) 
{
    copy_to_user(c, buf, i);
    printk ("Myread is invoked...\n");
    return 0;
}

ssize_t my_write(struct file *fp, const char *c, size_t i, loff_t *k) 
{
    printk("RT Mutex is being locked by %d",task_pid_nr(current));
    rt_mutex_lock(&test_rtmutex);
    printk("RT Mutex is locked by %d",task_pid_nr(current));
    copy_from_user(buf, c, i);
    mdelay(20000);
    rt_mutex_unlock(&test_rtmutex);          /// Releases the mutex
    printk ("RT Mutex is unlocked...pid = %d  Buf=%s",task_pid_nr(current),buf);
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
    printk ("Myclose is called...\n");
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
    rt_mutex_init(&test_rtmutex);       /// Initialize the mutex lock dynamically at runtime
    return 0;
}

void my_cleanup(void) 
{
    unregister_chrdev(major, "mydevice");
    rt_mutex_destroy(&test_rtmutex);        /// destroy the dynamically-allocated mutex
    printk ("Module removal is successful .....");
}

module_init(my_init);
module_exit(my_cleanup);

MODULE_LICENSE("GPL");


