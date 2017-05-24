#include <stdio.h>		// �ʿ��� header file �߰� ����
#include "student.h"
// �ϴ�, ��������� �Ⱦ��°ŷ� �����ϰ�, �״㿡 �ٲܰ�.
 
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

//������� ¯���...
//�� ���� ����

int main(int argc, char *argv[])
{
	FILE *fp;						// ��� file processing operation�� C library�� ����� ��.
	fp=fopen("students.dat","w+b");
	
	//�Ʒ� �ڵ�� argv[], ��� ��ǲ���� ����Ǿ�� �մϴ�.
	STUDENT temp;
	
	scanf("%s",temp.id);
	scanf("%s",temp.name);
	scanf("%s",temp.dept);
	scanf("%s",temp.year);
	scanf("%s",temp.addr);
	scanf("%s",temp.phone);
	scanf("%s",temp.email);
	
	writeRecord(fp,temp);
	
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

//void parse_Record(); 
int readPage(FILE *fp, char *pagebuf, int pagenum){
}
int getNextRecord(const char *pagebuf, char *recordbuf){
}
void unpack(const char *recordbuf, STUDENT *s){
	char *temp;
	int size;
	int count;
	int j;
	count = 0;
	j=0;
	size = sizeof(recordbuf)/sizeof(recordbuf[0]);
	temp=(char)malloc(sizeof(char)*31); // �ִ�ġ
	for (int i=0;i<size;i++){		//or MAX_RECORD_SIZE
		if ((int)recordbuf[i]==35){
			if (count==0){
				//�й� �ְ�
			}
			else if(count==1){
				//�̸� �ְ�
			}
			else if(count==2){
				//�а� �ְ�
			}
			else if(count==3){
				//�ּ� �ְ�
			}
			else if(count==4){
				//�̸��� �ּ� �ְ�
			} //7�̳�?			
			else if(count==5){
			}
			else if(count==6){
			}
			else
				printf("���� ���� �߻�\n");
			j=0;
			count++;
		}
		else{
			temp[j]=recordbuf[i];
			j++
		}	
			
	}
	
}
//1 ������ = [���][���ڵ�][���ڵ�][���ڵ�]
//��� = [���ڵ� ����][���ڵ������][���ڵ�offset][���ڵ����][���ڵ�offset][���ڵ����]... = 100byte
//���ڵ� = [7�� ������]*n = 924byte
int writeRecord(FILE *fp, const STUDENT *s){
	int record_count;	//���� ������ �������� ���ڵ尡 ��ΰ�.
	int last_offset;	//������ ������ offset
	int last_len;		//������ ������ length
	char *buf;
	buf = (char)melloc(sizeof(s));			//�ǳ�
	record_count=0;
	
	pack(buf,s);
	
	if(buf!=NULL){
		strlen(buf);
		//��� ������, ��� ������ �� Ȯ��.
		fseek(fp, PAGE_SIZE, SEEK_SET);	// �ϴ� ���� �������� ����.
		//while(�� ������ �߽߰ñ���)
		// if (���ڵ� ������ + strlen(buf) <= 924)	
		//�������� ����� ����. (#record)
		fseek(fp,4+4*record_count-1,SEEK_CUR);
		last_offset=//offset ����
		last_len=//length ����
		
		//����
		next_offset(2byte) = last_offset+last_len;
		next_size(2byte) = strlen(buf);
		
		//���� �������� �� ������ �ǵ��ư�.
		fseek;//
		//#record ����
		//#TRS ����
		fseek(fp,96,SEEK_CUR);
		fseek(fp,next_offset,SEEK_CUR);
		fwrite((void *)buf, next_size, 1, fp);
		
		return 1;
	}
	else
		return 0;
	
	
}
void pack(char *recordbuf, const STUDENT *s){
	//http://mwultong.blogspot.com/2006/10/c-strcat.html
	sprintf(recordbuf,"%s#%s#%s#%s#%s#%s#%s\n",s.id,s.name,s.dept,s.year,s.addr,s.phone,s.email);
	
}
void search(FILE *fp, FIELD f, char *keyval){
}
void printRecord(const STUDENT *s, int n){
	
	
}

