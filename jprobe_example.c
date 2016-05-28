
  
  #include <linux/kernel.h>
  #include <linux/module.h>
  #include <linux/kprobes.h>
  #include <linux/time.h>
  #include<linux/miscdevice.h>
  #include<linux/moduleparam.h>
int pid_curr;
int pid_val=0;
module_param(pid_val,int,S_IWUSR);


 int fault_handler(struct mm_struct *mm, struct vm_area_struct *vma,
                     unsigned long address, unsigned int flags)
  {
        
        pid_curr = task_pid_nr(current);
        if(pid_val==pid_curr)
        
        {
        //ns=current_kernel_time();	
	printk("Virtual address 0x%lx and pid: %d\n",address,pid_val);
        }
          jprobe_return();
          return 0;
  }
  
  static struct jprobe my_jprobe = {
          .entry                  = fault_handler,
          .kp = {
                  .symbol_name    = "handle_mm_fault",
          },
  };
  
  static int __init jprobe_init(void)
  {
          int ret;
  
          ret = register_jprobe(&my_jprobe);
          if (ret < 0) {
                  printk(KERN_INFO "register_jprobe failed, returned %d\n", ret);
                return -1;
          }
         printk(KERN_INFO "Planted jprobe at %p, handler addr %p\n",
                 my_jprobe.kp.addr, my_jprobe.entry);
          return 0;
  }
  
  static void __exit jprobe_exit(void)
  {
         unregister_jprobe(&my_jprobe);
         printk(KERN_INFO "jprobe at %p unregistered\n", my_jprobe.kp.addr);
 }
 
 module_init(jprobe_init)
 module_exit(jprobe_exit)
 MODULE_LICENSE("GPL");
