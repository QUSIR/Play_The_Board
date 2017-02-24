#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/pm_runtime.h>
#include <linux/suspend.h>
#include <linux/of.h>
#include <linux/dma-mapping.h>
#include <linux/clk/msm-clk.h>
#include <linux/pinctrl/consumer.h>
#include <linux/irqchip/msm-mpm-irq.h>
#include <soc/qcom/scm.h>
#include <linux/regulator/consumer.h>
#include <linux/mfd/pm8xxx/pm8921-charger.h>
#include <linux/mfd/pm8xxx/misc.h>
#include <linux/mhl_8334.h>
#include <linux/qpnp/qpnp-adc.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>


#define DRIVER_NAME     "law_led"


static struct class *leds_class;


int major;

struct platform_device *law_led=NULL;
struct pinctrl *led_pctrl;
struct pinctrl *led1_pctrl;
struct pinctrl_state *led_sta_on,*led_sta_off;
struct pinctrl_state *led1_sta_on,*led1_sta_off;





static int msm8909_leds_open(struct inode *inode, struct file *file)
{
	printk(" law-led--------open\n");

	return 0;
}



static int msm8909_leds_read(struct file *filp, char __user *buff, 
		size_t count, loff_t *offp)
{
	printk(" law-led--------write\n");


	return 1;
}




static ssize_t msm8909_leds_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{

	char val;
	int ret;
	ret=copy_from_user(&val, buf, 1);


	switch(val)
	{
		case 1:
			{

				pinctrl_select_state(led_pctrl, led_sta_on);
				pinctrl_select_state(led1_pctrl, led1_sta_on);

				break;

			}
		case 0:
			{
				pinctrl_select_state(led_pctrl, led_sta_off);
				pinctrl_select_state(led1_pctrl, led1_sta_off);
				break;
			}


	}

	return 1;
}
static ssize_t lawled_store(struct device *d, struct device_attribute *attr,const char *buf, size_t count)
{
	int val = 0;
	val = simple_strtol(buf, NULL, 10);
	if(val==1){
		pinctrl_select_state(led_pctrl, led_sta_on);
		pinctrl_select_state(led1_pctrl, led1_sta_on);
		return count;
	}

	if(val==0){

		pinctrl_select_state(led_pctrl, led_sta_off);
		pinctrl_select_state(led1_pctrl, led1_sta_off);
		return count;
	}
	return count;
}

static ssize_t lawled_show(struct device *d, struct device_attribute *attr, char *buf)
{
	return 0;


}


static DEVICE_ATTR(lawleds, S_IWUSR | S_IRUGO, lawled_show,lawled_store);



static struct file_operations msm8909_leds_fops = {
	.owner  =   THIS_MODULE,    
	.open   =   msm8909_leds_open,     
	.read	  =	msm8909_leds_read,	   
	.write   =	msm8909_leds_write,	   
};


static int law_led_probe(struct platform_device *pdev)
{
	int ret;
	struct device *tmp;


	if(&pdev->dev == NULL)
	{
		dev_err(&pdev->dev, "unable to get led dev \n");
		ret = -EPROBE_DEFER;
		return ret;
	}


	dev_info(&pdev->dev, "law_led probe\n");


	led_pctrl = devm_pinctrl_get(&pdev->dev);
	led1_pctrl = devm_pinctrl_get(&pdev->dev);

	if (IS_ERR(led_pctrl)) {
		dev_err(&pdev->dev, "pinctrl get failed\n");
		return PTR_ERR(led_pctrl);
	}

	led_sta_on =pinctrl_lookup_state(led_pctrl,"law_led_active");
  led1_sta_on =pinctrl_lookup_state(led1_pctrl,"law_led1_active");

	if (IS_ERR(led_sta_on)) {
		dev_err(&pdev->dev,
				"pinctrl get state failed for seta\n");
		ret = PTR_ERR(led_sta_on);
		return ret;
	}


	led_sta_off =pinctrl_lookup_state(led_pctrl,"law_led_sleep");
	led1_sta_off =pinctrl_lookup_state(led1_pctrl,"law_led1_sleep");

	if (IS_ERR(led_sta_off)) {
		dev_err(&pdev->dev,
				"pinctrl get state failed for seta\n");
		ret = PTR_ERR(led_sta_off);
		return ret;
	}


	major = register_chrdev(0, DRIVER_NAME, &msm8909_leds_fops);
	if (ret < 0) {
		printk(" can't register major number\n");
		return ret;
	}


	leds_class = class_create(THIS_MODULE, "lawsleds");
	if (IS_ERR(leds_class))
		return PTR_ERR(leds_class);



	tmp=  device_create(leds_class,NULL,MKDEV(major, 0),NULL,"%s","lawleds");
	if (IS_ERR(leds_class))
		return PTR_ERR(leds_class);

	pinctrl_select_state(led_pctrl, led_sta_on);
	pinctrl_select_state(led1_pctrl, led1_sta_on);



	printk(" law-led--------initialized-----end\n");

	ret = device_create_file(tmp, &dev_attr_lawleds);


	return 0;
}

static int law_led_remove(struct platform_device *pdev)
{

	unregister_chrdev(major, DRIVER_NAME);


	class_destroy(leds_class);
	return 0;


}



static struct of_device_id law_led_dt_match[] = {
	{       .compatible = "qcom,law-led",
	},
	{}
};


static void law_led_shutdown(struct platform_device *pdev)
{
	printk("zhuhandon-----led  remove\n");

}

static struct platform_driver law_led_driver = {
	.probe = law_led_probe,
	.remove = law_led_remove,
	.shutdown = law_led_shutdown,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = law_led_dt_match,
	},
};

module_platform_driver(law_led_driver);


MODULE_AUTHOR("law.zhu@quectel.com");
MODULE_VERSION("0.1.0");
MODULE_DESCRIPTION(" LED Driver");
MODULE_LICENSE("GPL");
