#ifndef __KHOOK_H__
#define __KHOOK_H__

#define OPTSIZE	5

// hook�ṹ��
typedef struct 
{

	// orig_op��orig���������ָ��
	char orig_op[OPTSIZE];
	// hook_op������ת��hook������ָ��
	char hook_op[OPTSIZE];
	// stub_op������תorig������ָ��
	char stub_op[OPTSIZE];

	// orig�������ָ�룬�����ָ��ᱻ�滻����jmp��hook����
	long orig_func_ptr;
	// hook�������ָ�룬ʵ���û����ܣ����ں����е���stub�������൱��orig������
	long hook_func_ptr;
	// stub�������ָ�룬����stub�����൱�ڵ���orig����
	long stub_func_ptr;

} khook_t;


int hook_init(void);
void hook_exit(void);
int hook_create(khook_t *h, char *orig_func_name, long hook_func_ptr, long stub_func_ptr);
void hook_destroy(khook_t *h);

#endif
