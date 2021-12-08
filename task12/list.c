#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdun Nihaal");

struct identity {
	struct list_head node;
	char  name[20];
	int   id;
	bool  busy;
};

LIST_HEAD(identity_list);

int identity_create(char *name, int id)
{
	struct identity *new;

	new = kmalloc(sizeof(struct identity), GFP_KERNEL);
	if (!new)
		return -ENOMEM;
	strcpy(new->name, name);
	new->id = id;
	new->busy = false;
	list_add(&new->node, &identity_list);
	return 0;
}

struct identity *identity_find(int id)
{
	struct identity *iter;

	list_for_each_entry(iter, &identity_list, node) {
		if (iter->id == id)
			return iter;
	}
	return NULL;
}

void identity_destroy(int id)
{
	struct identity *iter;

	list_for_each_entry(iter, &identity_list, node) {
		if (iter->id == id) {
			list_del(&iter->node);
			kfree(iter);
			return;
		}
	}
}

void identity_print(void)
{
	struct identity *iter;

	list_for_each_entry(iter, &identity_list, node) {
		pr_debug("%d: %s\n", iter->id, iter->name);
	}
}

static int __init list_init(void)
{
	struct identity *temp;

	pr_debug("Vanakkam\n");
	identity_create("Aswin", 1);
	identity_create("Allwin", 2);
	identity_create("Akash", 3);
	identity_create("Aravindhan", 10);

	temp = identity_find(3);
	pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");

	pr_debug("Before deleting 1 and 2\n");
	identity_print();
	identity_destroy(2);
	identity_destroy(1);
	pr_debug("After deleting 1 and 2\n");
	identity_print();
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);
	return 0;
}

static void __exit list_exit(void)
{
	pr_debug("Ta Ta\n");
}

module_init(list_init);
module_exit(list_exit);
