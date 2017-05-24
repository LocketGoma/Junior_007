#include <stdio.h>		// �ʿ��� header file �߰� ����

//
// readPage() �Լ��� ���ڵ� ���Ͽ��� �־��� pagenum(=0, 1, 2, ...)�� �ش��ϴ� page��
// �о pagebuf�� ����Ű�� ���� �����ϴ� ������ �����Ѵ�. �������� ������ �� ���
// '1'��, �׷��� ���� ���� '0'�� �����Ѵ�.
// getNextRecord() �Լ��� readPage()�� ���� �о�� page���� record�� ���ʴ�� �����ϴ�
// ���� �Ѵ�. ���� page���� ������ record�� ������ '0'��, �׷��� ������ '1'�� �����Ѵ�.
// unpack() �Լ��� recordbuf�� ����Ǿ� �ִ� record���� �� field ���� �����Ͽ� �л� ��ü�� �����ϴ� ���� �Ѵ�.
//
int readPage(FILE *fp, char *pagebuf, int pagenum);
int getNextRecord(const char *pagebuf, char *recordbuf);
void unpack(const char *recordbuf, STUDENT *s);

//
// writePage() �Լ��� ���α׷� ���� data page(pagebuf)�� ���ڵ� ������ �־��� pagenum(=0, 1, 2, ...)�� ��ġ�� 
// �����ϴ� ������ �����Ѵ�. �������� ������ �� ��� '1'��, �׷��� ���� ���� '0'�� �����Ѵ�.
//
int writePage(FILE *fp, const char *pagebuf, int pagenum);

//
// �־��� �л� ��ü�� ���ڵ� ���Ͽ� ������ �� ���Ǹ�, �̶� ������ �����ϴ� 
// page �߿��� �Ǹ����� page�� �ش� record�� �����Ѵ�. ����, �Ǹ����� page�� ���ο�
// ���ڵ带 ������ ������ �����ϸ� ���ο� page�� �ϳ� �Ҵ��ؼ� ���⿡ �����Ѵ�.
// writeRecord() �Լ��� ���ο����� pack() �Լ��� ȣ���ϸ�, pack() �Լ��� �־��� �л� ��ü�� ������ 
// ���ο� record�� page�� ����Ǿ��� ���� ���¸� recordbuf�� �����ϴ� ������ �����Ѵ�.
// writeRecord()�� ���������� �����ϸ� '1'��, �׷��� ������ '0'�� �����Ѵ�.
//
int writeRecord(FILE *fp, const STUDENT *s);
void pack(char *recordbuf, const STUDENT *s);

//
// ���ڵ� ���Ͽ��� field�� Ű��(keyval)�� ���� ���ڵ带 �˻��ϰ� �� ����� ����Ѵ�.
// �˻��� ���ڵ带 ����� �� �ݵ�� printRecord() �Լ��� ����Ѵ�.
//
void search(FILE *fp, FIELD f, char *keyval);
void printRecord(const STUDENT *s, int n);

//
// ���ڵ��� �ʵ���� enum FIELD Ÿ���� ������ ��ȯ���� �ش�.
// ���� ���, ����ڰ� ������ ��ɾ��� ���ڷ� "NAME"�̶�� �ʵ���� ����Ͽ��ٸ� 
// ���α׷� ���ο��� �̸� NAME(=1)���� ��ȯ�� �ʿ伺�� ������, �̶� �� �Լ��� �̿��Ѵ�.
//
FIELD getField(char *fieldname);

int main(int argc, char *argv[])
{
	FILE *fp;						// ��� file processing operation�� C library�� ����� ��.

	return 1;
}

void printRecord(const STUDENT *s, int n)
{
	int i;

	printf("NumOfRecords = %d\n", n);
	
	for(i=0; i<n; i++)
	{
		printf("%s#%s#%s#%s#%s#%s#%s\n", s[i].id, s[i].name, s[i].dept, s[i].year, s[i].addr, s[i].phone, s[i].email);
	}
}
