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
	{ 0x1976aa06, __VMLINUX_SYMBOL_STR(param_ops_bool) },
	{ 0xd6bd05ea, __VMLINUX_SYMBOL_STR(i2c_del_driver) },
	{ 0xeb4c65cd, __VMLINUX_SYMBOL_STR(i2c_register_driver) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0x778564ed, __VMLINUX_SYMBOL_STR(input_register_device) },
	{ 0x2072ee9b, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0x11f447ce, __VMLINUX_SYMBOL_STR(__gpio_to_irq) },
	{ 0xae8c4d0c, __VMLINUX_SYMBOL_STR(set_bit) },
	{ 0x1f9c03d8, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x1f7288a, __VMLINUX_SYMBOL_STR(input_allocate_device) },
	{ 0x4292df35, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xd4a0be46, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x65d6d0f0, __VMLINUX_SYMBOL_STR(gpio_direction_input) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0xa8f59416, __VMLINUX_SYMBOL_STR(gpio_direction_output) },
	{ 0xdc635f7b, __VMLINUX_SYMBOL_STR(input_event) },
	{ 0x754d2aca, __VMLINUX_SYMBOL_STR(i2c_transfer) },
	{ 0x5792f848, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0xc6b75fcb, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x23981db9, __VMLINUX_SYMBOL_STR(input_free_device) },
	{ 0x507a9257, __VMLINUX_SYMBOL_STR(input_unregister_device) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x5a61c574, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0x1fdc7df2, __VMLINUX_SYMBOL_STR(_mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:iqs263");

MODULE_INFO(srcversion, "59537EDE42CDB958302F3D1");
