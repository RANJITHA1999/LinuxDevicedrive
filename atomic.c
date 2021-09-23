#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h> // therad 
#include <linux/delay.h>   // sleep 
#include <linux/sched.h>  //task_struct
MODULE_LICENSE("GPL");
atomic_t my_global_variable = ATOMIC_INIT(0);


static struct task_struct *char_thread;
static struct task_struct *char_thread1;

int mydata =0;
int my_thread_fun(void *p)
{
	while(!kthread_should_stop())
	{		
		//Atomically add one to v
		atomic_inc(&my_global_variable); // single operation ; no much overhead //1
		printk(KERN_INFO "my_thread_fun after inc  %lu\n", atomic_read(&my_global_variable));
		
			
		//atomic_read(&my_global_variable)
		atomic_add(14, &my_global_variable);
		printk(KERN_INFO "my_thread_fun after add   %lu\n", atomic_read(&my_global_variable));//15

		//Atomically subtract i from v
		atomic_sub(2,&my_global_variable);
		printk(KERN_INFO "my_thread_fun after sub   %lu\n", atomic_read(&my_global_variable));//13

		//Atomically subtract one from v
		atomic_dec(&my_global_variable);
		printk(KERN_INFO "my_thread_fun after dec   %lu\n", atomic_read(&my_global_variable));//12
		
		//Atomically set v equal to i
		atomic_set(&my_global_variable, 14);			
		printk(KERN_INFO "my_thread_fun after set    %lu\n", atomic_read(&my_global_variable));//14
		
		// Function Atomically subtract i from v and return true if the result is zero; otherwise false 
		unsigned int res = atomic_sub_and_test(3, &my_global_variable);//14-3=11 return 0
		printk(KERN_INFO "my_thread_fun after sub and test    %lu\n", res);//0

		// Function Atomically dec one from v and return true(1) if the result is zero; otherwise false (0)
		res = atomic_dec_and_test(&my_global_variable);//11-1=10
		printk(KERN_INFO "my_thread_fun after dec and test    %lu\n", res);//0
		printk(KERN_INFO "my_thread_fun after dec and test    %lu\n", atomic_read(&my_global_variable));//10
			
		//Atomically add i to v and return true if the result is negative; otherwise false
		res = atomic_add_negative(8,&my_global_variable);//10+8=18
		printk(KERN_INFO "my_thread_fun after add neg    %lu\n", res);//0
		printk(KERN_INFO "my_thread_fun after add neg     %lu\n", atomic_read(&my_global_variable));//18

		//Atomically increment v by one and return true if the result is zero; false otherwise
		res = atomic_inc_and_test(&my_global_variable);//18+1=19
		printk(KERN_INFO "my_thread_fun after inc and test     %lu\n", res);//0
		printk(KERN_INFO "my_thread_fun after inc and test      %lu\n", atomic_read(&my_global_variable));//19


		printk(KERN_INFO "*****************ATOMIC BIT WSIE OPERATORS *************");
		
		unsigned long word = 0;

		set_bit(0, &word); //0000    //bit zero is now set (atomically) 
		printk(KERN_INFO "SET BIT      %lu\n", word);//0001=1

		set_bit(1, &word); //0001       // bit one is now set (atomically) 
		printk(KERN_INFO "SET BIT      %lu\n", word);//0011=3

		set_bit(2, &word); //0011       // bit two is now set (atomically) 
		printk(KERN_INFO "SET BIT      %lu\n", word);//0111=7

		clear_bit(1, &word);  //0111   // bit one is now unset (atomically) 
		printk(KERN_INFO "CLEAR BIT      %lu\n", word);//0101=5

		change_bit(0, &word); //0101   // bit zero is flipped; now it is unset (atomically) 
		printk(KERN_INFO "CHANGE BIT      %lu\n", word);//0100=4

		//Atomically set the nr-th bit starting from addr and return the previous value
		res = test_and_set_bit(1, &word);//0
		printk(KERN_INFO"TEST AND SET    %lu\n", res);//0
		printk(KERN_INFO"TEST AND SET      %u\n", word);//0110
		//Atomically clear the nr-th bit starting from addr and return the previous value
		res = test_and_clear_bit(1,&word);//1
		printk(KERN_INFO "TEST AND CLEAR    %lu\n", res);//1
		printk(KERN_INFO "TEST AND CLEAR      %u\n", word);//0100
		
		//Atomically return the value of the nr-th bit starting from addr
		res = test_bit(2,&word);//0100
		printk(KERN_INFO"TEST BIT    %u\n", res); //1

		msleep(1000);
		}
	return 0;	
}

int my_thread_fun1(void *p)
{
	while(!kthread_should_stop())
	{
		
		atomic_inc(&my_global_variable);
		printk(KERN_INFO "my_thread_fun2  %u\n", atomic_read(&my_global_variable));
		
		msleep(1000);
	}
	return 0;
}
static int __init my_simpledriver_init(void)
{
	printk(KERN_INFO"My sample driver start");
  char_thread = kthread_run(my_thread_fun,NULL,"my char thread");
	if(char_thread)
	{
		printk(KERN_INFO" create the thread status is %d",*char_thread);
		printk(KERN_INFO"pid is:%d",char_thread->pid);
		printk(KERN_INFO"status is:%d",char_thread->thread_info.status);
		printk(KERN_INFO"tgid is:%d",char_thread->tgid);
		printk(KERN_INFO"priority is:%d",char_thread->prio);
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
	}
  char_thread1 = kthread_run(my_thread_fun1,NULL,"my char thread");
	if(char_thread1)
	{
		printk(KERN_INFO" create the thread1 status is %d",*char_thread1);
    printk(KERN_INFO"pid is:%d",char_thread1->pid);
		printk(KERN_INFO"status is:%d",char_thread1->thread_info.status);
		printk(KERN_INFO"tgid is:%d",char_thread1->tgid);
		printk(KERN_INFO"priority is:%d",char_thread1->prio);
	}
	else 
	{
	printk(KERN_INFO"unable to create the thread");
	}
return 0;
}
static void __exit my_simpledriver_exit(void)
{
	printk(KERN_INFO"My samle driver exit\n");
  kthread_stop(char_thread);
	kthread_stop(char_thread1);
}
module_init(my_simpledriver_init);
module_exit(my_simpledriver_exit);
