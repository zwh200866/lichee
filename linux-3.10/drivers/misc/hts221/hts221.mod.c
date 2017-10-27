#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x27183c55, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0x1b972928, __VMLINUX_SYMBOL_STR(device_remove_file) },
	{ 0xd4a0be46, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x1fdc7df2, __VMLINUX_SYMBOL_STR(_mcount) },
	{ 0xc6b75fcb, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0xd6bd05ea, __VMLINUX_SYMBOL_STR(i2c_del_driver) },
	{ 0x46608fa0, __VMLINUX_SYMBOL_STR(getnstimeofday) },
	{ 0x754d2aca, __VMLINUX_SYMBOL_STR(i2c_transfer) },
	{ 0x6b06fdce, __VMLINUX_SYMBOL_STR(delayed_work_timer_fn) },
	{ 0x593a99b, __VMLINUX_SYMBOL_STR(init_timer_key) },
	{ 0x797c8fa9, __VMLINUX_SYMBOL_STR(cancel_delayed_work_sync) },
	{ 0xfded7251, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xdc635f7b, __VMLINUX_SYMBOL_STR(input_event) },
	{ 0x3c80c06c, __VMLINUX_SYMBOL_STR(kstrtoull) },
	{ 0x1f9c03d8, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x4b0a68b8, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x5792f848, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0x627dbf4f, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xeb4c65cd, __VMLINUX_SYMBOL_STR(i2c_register_driver) },
	{ 0x9791c129, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0xeeec26a7, __VMLINUX_SYMBOL_STR(queue_delayed_work_on) },
	{ 0x3bd1b1f6, __VMLINUX_SYMBOL_STR(msecs_to_jiffies) },
	{ 0x778564ed, __VMLINUX_SYMBOL_STR(input_register_device) },
	{ 0x23981db9, __VMLINUX_SYMBOL_STR(input_free_device) },
	{ 0x4292df35, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x4829a47e, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x507a9257, __VMLINUX_SYMBOL_STR(input_unregister_device) },
	{ 0xae8c4d0c, __VMLINUX_SYMBOL_STR(set_bit) },
	{ 0x5a61c574, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0x1f7288a, __VMLINUX_SYMBOL_STR(input_allocate_device) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cst,hts221*");
MODULE_ALIAS("i2c:hts221");

MODULE_INFO(srcversion, "F4AFD59202F6BB540B4C479");
