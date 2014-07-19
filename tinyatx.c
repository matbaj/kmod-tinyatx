#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/gpio.h>

#define PIN 4

static struct timer_list heartbeat_timer;

static void heartbeat(unsigned long data)
{
  heartbeat_timer.data = !data;
  if (data)
  {
    heartbeat_timer.expires = jiffies + (30*HZ);
    gpio_set_value(PIN, data);
  } 
  else
  {
    heartbeat_timer.expires = jiffies + HZ;
    gpio_set_value(PIN, data);
  }
  
  add_timer(&heartbeat_timer);

}

static int __init tinyatx_init(void)
{
  int ret = 0;

  printk(KERN_INFO "%s started on pin %d\n", __func__, PIN);

  ret = gpio_request_one(PIN, GPIOF_OUT_INIT_HIGH, "Heartbeat PIN");

  if (ret) 
    printk(KERN_ERR "Unable to request GPIO: %d\n", ret);

  gpio_set_value(PIN, 1L); 
  init_timer(&heartbeat_timer);

  heartbeat_timer.function = heartbeat;
  heartbeat_timer.data = 1L;
  heartbeat_timer.expires = jiffies + (30*HZ);
  add_timer(&heartbeat_timer);

  return ret;
}


static void __exit tinyatx_exit(void)
{
  printk(KERN_INFO "%s stopped\n", __func__);
  del_timer_sync(&heartbeat_timer);
  gpio_set_value(PIN, 0); 
  gpio_free(PIN);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mateusz Bajorski");
MODULE_DESCRIPTION("Watchdog for tinyatx");

module_init(tinyatx_init);
module_exit(tinyatx_exit);
