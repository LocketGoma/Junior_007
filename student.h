#ifndef _STUDENT_H_
#define _STUDENT_H_

#define MAX_RECORD_SIZE		120	//id(8) + name(20) + department(20) + year(1) + address(30) + phone(15) +  email(26)
#define PAGE_SIZE		1024	// 1 KBytes
#define HEADER_SIZE		100	// 100 Bytes
// �ʿ��� ��� 'define'�� �߰��� �� ����

typedef enum {ID=0, NAME, DEPT, YEAR,  ADDR, PHONE, EMAIL} FIELD;

typedef struct _STUDENT
{
	char id[9];		// �й�
	char name[21];		// �̸�
	char dept[21];		// �а�
	char year[2];		// �г�
	char addr[31];		// �ּ�
	char phone[16];		// ��ȭ��ȣ
	char email[27];		// �̸��� �ּ�
} STUDENT;

typedef struct _HEADSIZE		//�� ���ڵ��� ���-ù��° 2�� �б��.
{
	short records;	//#records, ���ڵ� ����
	short size;		//���ڵ� ������
}HEADSIZE;

typedef struct _OFFSET			//�� ���ڵ忡 �ִ� offset/len �б��
{
	short offset;			//offset��
	short length;			//length��
}OFFSET;

#endif
