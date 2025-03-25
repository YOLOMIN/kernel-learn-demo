#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/page-flags.h>  // 包含一些页面状态宏

#define page_mapcount(page) (page_count(page) - 1)

void my_dump_page(struct page *page)
{
    /* 通过page->flags直接获取页标志 */
    unsigned long flags = page->flags;
    /* 使用内核宏获取引用计数和映射计数 */
    int refcount = page_ref_count(page);
    int mapcount = page_mapcount(page);

    printk(KERN_INFO "========== Dump Page Info ==========\n");
    printk(KERN_INFO "Page address: %p\n", page);
    printk(KERN_INFO "Flags: 0x%lx\n", flags);
    printk(KERN_INFO "Reference count: %d\n", refcount);
    printk(KERN_INFO "Map count: %d\n", mapcount);

    /* 如果struct page中存在index字段（通常用于文件页），打印页索引 */
    printk(KERN_INFO "Page index: %lu\n", page->index);
    printk(KERN_INFO "======================================\n");
}

static int __init my_alloc_init(void)
{
    struct page *page;
    unsigned int order = 0;  // 分配一个基本页
    
    // 分配物理页
    page = alloc_pages(GFP_KERNEL, order);
    if (!page) {
        printk(KERN_ERR "alloc_pages failed!\n");
        return -ENOMEM;
    }

    // 打印页信息，假设内核中存在dump_page()函数
    my_dump_page(page);  // 此函数需要根据实际内核版本及调试方法实现

    printk(KERN_INFO "Allocated page at address: %p\n", page);
    return 0;
}

static void __exit my_alloc_exit(void)
{
    // 这里可以调用__free_pages释放已分配的页
    printk(KERN_INFO "Module exit\n");
}

module_init(my_alloc_init);
module_exit(my_alloc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Physical Memory Allocator Debug Module");

