

#define EUC		0	// KS X 1001 �ϼ��� + KS X 1003 ������
#define JOL		1	// KS X 1001 ������ + KS X 1003	������ 
#define IW		2	// ISO/IEC 10646-1 �ϼ��� (11,172 ���ڸ���) ������
#define IPF		3	// ISO/IEC 10646-1 ù���� ������ ������ 
#define IPF1	4	// ISO/IEC 10646-1 ù���� ������ �ʼ� �ε���
#define IPF2	5	// ISO/IEC 10646-1 ù���� ������ �߼� �ε���
#define IPF3	6	// ISO/IEC 10646-1 ù���� ������ ���� �ε���
#define IPFSIZE	7	// ISO/IEC 10646-1 ù���� ������ �ѱ��� ����� �ε���

#define EUC_START_CODE	(0x8141)	// �A
#define EUC_END_CODE	(0xC8FE)	// �� (�R X)

/* for 2350 char
#define JOL_START_CODE	(0x8861)	// ��
#define JOL_END_CODE	(0xD3B7)	// ��
/* for 11172 char */
#define JOL_START_CODE	(0x8861)	// ��
#define JOL_END_CODE	(0xD3BD)	// �R

/* for 2350 char
#define IW_START_CODE	(0xAC00)	// ��
#define IW_END_CODE		(0xD79D)	// ��
/* for 11172 char */
#define IW_START_CODE	(0xAC00)	// ��
#define IW_END_CODE		(0xD7A3)	// �R 

/* for 2350 char
#define IPF_START_CODE	(0x00)		// ��
#define IPF_END_CODE	(0x9A46)	// ��
/* for 11172 char */
#define IPF_START_CODE	(0x00)		// ��
#define IPF_END_CODE	(0x9A4C)	// �R


#define	IPF1_START_CODE	(0x1100)	// ��
#define	IPF1_END_CODE	(0x1112)	// ��

#define IPF2_START_CODE	(0x1161)	// ��
#define IPF2_END_CODE	(0x1175)	// ��

#define IPF3_EMPTY_CODE (0x1160)	// ��ħ����
#define IPF3_START_CODE	(0x1160)	// ��ħ����
#define IPF3_START_CODE2	(0x11A8)// ��	
#define IPF3_END_CODE	(0x11C2)	// ��

#define IPF1_RANGE		(IPF1_END_CODE - IPF1_START_CODE + 1)
#define IPF2_RANGE		(IPF2_END_CODE - IPF2_START_CODE + 1)
//#define IPF3_RANGE		(IPF3_END_CODE - IPF3_START_CODE + 1)	����!
#define IPF3_RANGE		(IPF3_END_CODE - IPF3_EMPTY_CODE + 1)	// IPF������ ������ ���� ���� 


int BinaryHanCodeSearch(int wordvalue, int nCode);
int HanCodeConvert( unsigned int fromvalue, unsigned int * tovalue, int nFromCode,int nToCode);

int UTF8toUCS2(unsigned char *utf8, unsigned int *ucs2);
