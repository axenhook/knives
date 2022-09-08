#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/cpu.h>
#include <linux/module.h>

#include "khook.h"


static void *(*ptr_poke_smp)(void *addr, const void *opcode, size_t len);


int hook_create(khook_t *h, char *orig_func_name, long hook_func_ptr, long stub_func_ptr)
{
	s32 hook_offset, orig_offset;
    long orig_func_ptr;

	// �ҵ�Ҫhook�ĺ���
	orig_func_ptr = kallsyms_lookup_name(orig_func_name);
	if (!orig_func_ptr) {
		printk("lookup %s failed\n", orig_func_name);
		return -1;
	}

	// orig��Ϣ����
    h->orig_func_ptr = orig_func_ptr;
	// ����orig���������ָ��
	h->orig_op[0] = ((char*)orig_func_ptr)[0];
	h->orig_op[1] = ((char*)orig_func_ptr)[1];
	h->orig_op[2] = ((char*)orig_func_ptr)[2];
	h->orig_op[3] = ((char*)orig_func_ptr)[3];
	h->orig_op[4] = ((char*)orig_func_ptr)[4];

	// hook��Ϣ��ʼ��
	h->hook_func_ptr = hook_func_ptr;
	// ���ô�orig������ת��hook������ָ��
	h->hook_op[0] = 0xe9; // ��һ���ֽڵ�Ȼ��jump
	// ����Ŀ��hook������orig���������ƫ��
	hook_offset = (s32)(hook_func_ptr - orig_func_ptr - OPTSIZE);
	// ����4���ֽ�Ϊһ�����ƫ��
	(*(s32*)(&h->hook_op[1])) = hook_offset;

	// stub��Ϣ��ʼ��
	h->stub_func_ptr = stub_func_ptr;
	// ���ô�stub������ת��orig������ָ��
	h->stub_op[0] = 0xe9; // ��һ���ֽڵ�Ȼ��jump
	// ����Ŀ��orig������Ҫִ�е�λ�õ���ǰλ�õ�ƫ��
	orig_offset = (s32)(orig_func_ptr + OPTSIZE - (stub_func_ptr + OPTSIZE));
	(*(s32*)(&h->stub_op[1])) = orig_offset;


	// �滻������
	get_online_cpus();
	ptr_poke_smp((void *)stub_func_ptr, h->stub_op, OPTSIZE); // stubָ��������stub�����ϣ�����ת��orig������
	barrier();
	ptr_poke_smp((void *)orig_func_ptr, h->hook_op, OPTSIZE); // hookָ��������orig�����ϣ�����ת��hook������
	put_online_cpus();

    return 0;
}

void hook_destroy(khook_t *h)
{
    if (h->orig_func_ptr == 0)
    {
        return;
    }
    
	get_online_cpus();
	ptr_poke_smp((void *)h->orig_func_ptr, h->orig_op, OPTSIZE); // ��ԭorig�������ָ��
	barrier();
	put_online_cpus();
}

int hook_init(void)
{
	// ���poke������ɵľ�����ӳ�䣬дtext�ε���
	ptr_poke_smp = (void *(*)(void *, const void *, size_t))kallsyms_lookup_name("text_poke_smp");
	if (!ptr_poke_smp) {
		printk("lookup %s failed\n", "text_poke_smp");
		return -1;
	}

	return 0;
}

void hook_exit(void)
{
    
}

