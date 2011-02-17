

#define EUC		0	// KS X 1001 ¿Ï¼ºÇü + KS X 1003 ±¸ºĞÀÚ
#define JOL		1	// KS X 1001 Á¶ÇÕÇü + KS X 1003	±¸ºĞÀÚ 
#define IW		2	// ISO/IEC 10646-1 ¿Ï¼ºÇü (11,172 ±ÛÀÚ¸¶µğ) ±¸ºĞÀÚ
#define IPF		3	// ISO/IEC 10646-1 Ã¹°¡³¡ Á¶ÇÕÇü ±¸ºĞÀÚ 
#define IPF1	4	// ISO/IEC 10646-1 Ã¹°¡³¡ Á¶ÇÕÇü ÃÊ¼º ÀÎµ¦½º
#define IPF2	5	// ISO/IEC 10646-1 Ã¹°¡³¡ Á¶ÇÕÇü Áß¼º ÀÎµ¦½º
#define IPF3	6	// ISO/IEC 10646-1 Ã¹°¡³¡ Á¶ÇÕÇü Á¾¼º ÀÎµ¦½º
#define IPFSIZE	7	// ISO/IEC 10646-1 Ã¹°¡³¡ Á¶ÇÕÇü ÇÑ±ÛÀÚ ¿öµå¼ö ÀÎµ¦½º

#define EUC_START_CODE	(0x8141)	// A
#define EUC_END_CODE	(0xC8FE)	// Èş (ÆR X)

/* for 2350 char
#define JOL_START_CODE	(0x8861)	// °¡
#define JOL_END_CODE	(0xD3B7)	// Èş
/* for 11172 char */
#define JOL_START_CODE	(0x8861)	// °¡
#define JOL_END_CODE	(0xD3BD)	// ÆR

/* for 2350 char
#define IW_START_CODE	(0xAC00)	// °¡
#define IW_END_CODE		(0xD79D)	// Èş
/* for 11172 char */
#define IW_START_CODE	(0xAC00)	// °¡
#define IW_END_CODE		(0xD7A3)	// ÆR 

/* for 2350 char
#define IPF_START_CODE	(0x00)		// °¡
#define IPF_END_CODE	(0x9A46)	// Èş
/* for 11172 char */
#define IPF_START_CODE	(0x00)		// °¡
#define IPF_END_CODE	(0x9A4C)	// ÆR


#define	IPF1_START_CODE	(0x1100)	// ¤¡
#define	IPF1_END_CODE	(0x1112)	// ¤¾

#define IPF2_START_CODE	(0x1161)	// ¤¿
#define IPF2_END_CODE	(0x1175)	// ¤Ó

#define IPF3_EMPTY_CODE (0x1160)	// ¹ŞÄ§¾øÀ½
#define IPF3_START_CODE	(0x1160)	// ¹ŞÄ§¾øÀ½
#define IPF3_START_CODE2	(0x11A8)// ¤¡	
#define IPF3_END_CODE	(0x11C2)	// ¤¾

#define IPF1_RANGE		(IPF1_END_CODE - IPF1_START_CODE + 1)
#define IPF2_RANGE		(IPF2_END_CODE - IPF2_START_CODE + 1)
//#define IPF3_RANGE		(IPF3_END_CODE - IPF3_START_CODE + 1)	ÁÖÀÇ!
#define IPF3_RANGE		(IPF3_END_CODE - IPF3_EMPTY_CODE + 1)	// IPF°íÀ¯°ª »ı¼ºÀ» À§ÇÑ ¹üÀ§ 


int BinaryHanCodeSearch(int wordvalue, int nCode);
int HanCodeConvert( unsigned int fromvalue, unsigned int * tovalue, int nFromCode,int nToCode);

int UTF8toUCS2(unsigned char *utf8, unsigned int *ucs2);
