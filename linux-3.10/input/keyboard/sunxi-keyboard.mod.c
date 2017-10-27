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
	{ 0x9a343982, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x155856d4, __VMLINUX_SYMBOL_STR(platform_driver_register) },
	{ 0xfcec0987, __VMLINUX_SYMBOL_STR(enable_irq) },
	{ 0x85c12b64, __VMLINUX_SYMBOL_STR(clk_disable) },
	{ 0x27bbf221, __VMLINUX_SYMBOL_STR(disable_irq_nosync) },
	{ 0x79a3f0ab, __VMLINUX_SYMBOL_STR(clk_unprepare) },
	{ 0x7602eda3, __VMLINUX_SYMBOL_STR(clk_enable) },
	{ 0x1d731b3a, __VMLINUX_SYMBOL_STR(clk_prepare) },
	{ 0x23981db9, __VMLINUX_SYMBOL_STR(input_free_device) },
	{ 0x778564ed, __VMLINUX_SYMBOL_STR(input_register_device) },
	{ 0x2072ee9b, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0xc6b75fcb, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0xae8c4d0c, __VMLINUX_SYMBOL_STR(set_bit) },
	{ 0x1f7288a, __VMLINUX_SYMBOL_STR(input_allocate_device) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xa5f389eb, __VMLINUX_SYMBOL_STR(of_property_read_u32_array) },
	{ 0xec17ebb0, __VMLINUX_SYMBOL_STR(of_match_node) },
	{ 0xdaa16b82, __VMLINUX_SYMBOL_STR(of_clk_get) },
	{ 0x561e4b77, __VMLINUX_SYMBOL_STR(irq_of_parse_and_map) },
	{ 0x11c9185c, __VMLINUX_SYMBOL_STR(of_iomap) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xaf05b91b, __VMLINUX_SYMBOL_STR(of_device_is_available) },
	{ 0x4292df35, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xd4a0be46, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xdc635f7b, __VMLINUX_SYMBOL_STR(input_event) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x507a9257, __VMLINUX_SYMBOL_STR(input_unregister_device) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x5a61c574, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0x1fdc7df2, __VMLINUX_SYMBOL_STR(_mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Callwinner,keyboard_1350mv*");
MODULE_ALIAS("of:N*T*Callwinner,keyboard_2000mv*");

MODULE_INFO(srcversion, "5E47DEF88A8CA45904E6085");
