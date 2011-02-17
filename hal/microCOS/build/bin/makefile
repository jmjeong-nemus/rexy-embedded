#**************************************************************************
#                                                                         *
#   PROJECT     : ARM port for UCOS-II                                    *
#                                                                         *
#   MODULE      : MAKEFILE                                                *
#                                                                         *
#   AUTHOR      : Michael Anburaj                                         *
#                 URL  : http://geocities.com/michaelanburaj/             *
#                 EMAIL: michaelanburaj@hotmail.com                       *
#                                                                         *
#   PROCESSOR   : S3c2410x (32 bit ARM920T RISC core from Samsung)        *
#                                                                         *
#   IDE         : SDT 2.51 Or ADS 1.2                                     *
#                                                                         *
#   DESCRIPTION :                                                         *
#   Make file for Ex1 ARM_UCOS builds. Use Microsoft's nmake utility to   *
#   build using this file.                                                *
#                                                                         *
#*************************************************************************/


#**************************************************************************
#* Build Configuration
#**************************************************************************

#### Project name ####
PRJ = Ex1

#### Endian ####
# possible values: 0 (for Little), 1 (for Big)
ENDIAN = 0

#### Debug ####
# possible values: 0, 1
DEBUG = 1

#### Target Mode ####
# Possible values: 0 (for arm), 1 (for armthumb)
TARGET = 1

#### Tools ####
# possible values: 0 (for ADS), 1 (for SDT)
TOOLS = 0

#### ARM tools path ####
ifeq ($(TOOLS),0)
TPATH = C:\Progra~1\ARM\ADSv1_2
else
TPATH = C:\ARM251
endif

#### ARM tool Definition ####
ARMLINK= armlink
ARMASM = armasm
ARMCC  = armcc
TCC    = tcc

#### Destination path Definition ####
OBJ=.
ERR=.


#**************************************************************************
#* Build settings (Compiler, Assembler & Linker flags)
#* Note: Do not modify below this line
#**************************************************************************

ifeq ($(DEBUG),1)
DBFLAG = -g
endif

ifeq ($(ENDIAN),1)
ENDIAN_FLAG = -bi
E_FLAG_C = -D __BIG_ENDIAN
E_FLAG_A = -pd "__BIG_ENDIAN SETL {TRUE}"
EC_FLAG_ROM_A = -pd "ENDIAN_CHANGE SETL {TRUE}"
EC_FLAG_RAM_A = -pd "ENDIAN_CHANGE SETL {FALSE}"
else
ENDIAN_FLAG = -li
E_FLAG_C = 
E_FLAG_A = -pd "__BIG_ENDIAN SETL {FALSE}"
EC_FLAG_ROM_A = -pd "ENDIAN_CHANGE SETL {FALSE}"
EC_FLAG_RAM_A = -pd "ENDIAN_CHANGE SETL {FALSE}"
endif

ifeq ($(TARGET),0)
DCC = $(ARMCC)
AC_FLAGS =
AA_FLAGS =
AL_FLAGS =
else
DCC = $(TCC)
AC_FLAGS = -apcs /noswst/interwork -D__APCS_INTERWORK
AA_FLAGS =
AL_FLAGS =
endif

ifeq ($(TOOLS),1)
T_FLAG_C = -DSDT
T_FLAG_A = -pd "ADS SETL {FALSE}"
T_FLAG_L = -libpath $(TPATH)\Lib\embedded
T_CMD_ROM_F = -nozeropad $(PRJ_ROM).axf -bin $(PRJ_ROM).bin
T_CMD_RAM_F = -nozeropad $(PRJ_RAM).axf -bin $(PRJ_RAM).bin
else
T_FLAG_C = -DADS
T_FLAG_A = -pd "ADS SETL {TRUE}"
T_FLAG_L =
T_CMD_ROM_F = -nodebug -bin -output $(PRJ_ROM).bin $(PRJ_ROM).axf
T_CMD_RAM_F = -nodebug -bin -output $(PRJ_RAM).bin $(PRJ_RAM).axf
endif

#### Output file names ####
PRJ_ROM=$(PRJ)_rom
PRJ_RAM=$(PRJ)_ram

#### RO & RW base ####
RO_BADR_RAM=0x30004000
RW_BADR_RAM=0x30100000

RO_BADR_ROM=0x00000000
RW_BADR_ROM=0x30100000

#### Paths ####
UPORT_COMN = ..\..\arm_ucos\common
UPORT_2410 = ..\..\arm_ucos\S3c2410a

#### Include file path ####
INC=-I..\inc -I..\..\source -I$(UPORT_COMN)\inc -I$(UPORT_2410)\inc

#### Build Flags ####
AFLAGS = $(ENDIAN_FLAG) -apcs /noswst $(T_FLAG_A) $(E_FLAG_A) $(AA_FLAGS) $(INC) -cpu ARM920T
CFLAGS = -g -c -O2 $(DBFLAG) $(ENDIAN_FLAG) -apcs /noswst $(T_FLAG_C) $(E_FLAG_C) $(AC_FLAGS) $(INC) -cpu ARM920T
LFLAGS_ROM = -elf -map -xref -list list.txt -first init.eco(Init) $(AL_FLAGS) $(T_FLAG_L)
LFLAGS_RAM = -elf -map -xref -list list.txt -first init.o(Init) $(AL_FLAGS) $(T_FLAG_L)

#### Object combine Definition ####
OBJS_ROM = $(OBJ)\init.eco $(OBJ)\mmu.o $(OBJ)\mmulib.o $(OBJ)\os_cpu_a.o $(OBJ)\consol.o \
	$(OBJ)\queue.o \
	$(OBJ)\HwrRTCTimer.o \
	$(OBJ)\frmwrk.o  $(OBJ)\os_cpu_c.o  $(OBJ)\ucos_ii.o $(OBJ)\ex1.o $(OBJ)\HwrDisplay.o \
	$(OBJ)\HwrKeyMgr.o $(OBJ)\HwrInterrupts.o $(OBJ)\HwrTimer.o 

OBJS_RAM = $(OBJ)\init.o $(OBJ)\mmu.o $(OBJ)\mmulib.o $(OBJ)\os_cpu_a.o $(OBJ)\consol.o \
	$(OBJ)\queue.o \
	$(OBJ)\HwrRTCTimer.o \
	$(OBJ)\frmwrk.o  $(OBJ)\os_cpu_c.o  $(OBJ)\ucos_ii.o $(OBJ)\ex1.o $(OBJ)\HwrDisplay.o \
	$(OBJ)\HwrKeyMgr.o $(OBJ)\HwrInterrupts.o $(OBJ)\HwrTimer.o 


all: $(PRJ_ROM).axf $(PRJ_RAM).axf
	
clean:
	del $(OBJ)\*.o
	del $(OBJ)\*.eco
	del $(ERR)\*.err
	del $(PRJ_ROM).axf
	del $(PRJ_ROM).bin
	del $(PRJ_RAM).axf
	del $(PRJ_RAM).bin


#**************************************************************************
#* Link
#**************************************************************************

$(PRJ_ROM).axf: $(OBJS_ROM)
	$(ARMLINK) -ro-base $(RO_BADR_ROM) -rw-base $(RW_BADR_ROM) $(LFLAGS_ROM) -o $(PRJ_ROM).axf $(OBJS_ROM)
	fromelf $(T_CMD_ROM_F)

$(PRJ_RAM).axf: $(OBJS_RAM)
	$(ARMLINK) -ro-base $(RO_BADR_RAM) -rw-base $(RW_BADR_RAM) $(LFLAGS_RAM) -o $(PRJ_RAM).axf $(OBJS_RAM)
	fromelf $(T_CMD_RAM_F)


#**************************************************************************
#* Compile/Assemble
#**************************************************************************

$(OBJ)\init.o: $(UPORT_2410)\src\init.s
	$(ARMASM) $(EC_FLAG_RAM_A) $(AFLAGS) $(UPORT_2410)\src\init.s -o $(OBJ)\init.o -Errors $(ERR)\init.err

$(OBJ)\init.eco: $(UPORT_2410)\src\init.s
	$(ARMASM) $(EC_FLAG_ROM_A) $(AFLAGS) $(UPORT_2410)\src\init.s -o $(OBJ)\init.eco -Errors $(ERR)\init.err

$(OBJ)\mmulib.o: $(UPORT_2410)\src\mmulib.s
	$(ARMASM) $(AFLAGS) $(UPORT_2410)\src\mmulib.s -o $(OBJ)\mmulib.o -Errors $(ERR)\mmulib.err

$(OBJ)\mmu.o: $(UPORT_2410)\src\mmu.c
	$(ARMCC) $(CFLAGS) $(UPORT_2410)\src\mmu.c -o $(OBJ)\mmu.o -Errors $(ERR)\mmu.err

$(OBJ)\os_cpu_a.o: $(UPORT_COMN)\src\os_cpu_a.s
	$(ARMASM) $(AFLAGS) $(UPORT_COMN)\src\os_cpu_a.s -o $(OBJ)\os_cpu_a.o -Errors $(ERR)\os_cpu_a.err

$(OBJ)\consol.o: $(UPORT_2410)\src\consol.c $(UPORT_2410)\inc\S3c2410A.h $(UPORT_2410)\inc\frmwrk.h
	$(DCC) $(CFLAGS) $(UPORT_2410)\src\consol.c -o $(OBJ)\consol.o -Errors $(ERR)\consol.err

$(OBJ)\frmwrk.o: $(UPORT_2410)\src\frmwrk.c $(UPORT_2410)\inc\S3c2410A.h $(UPORT_2410)\inc\frmwrk.h
	$(ARMCC) $(CFLAGS) $(UPORT_2410)\src\frmwrk.c -o $(OBJ)\frmwrk.o -Errors $(ERR)\frmwrk.err

$(OBJ)\os_cpu_c.o: $(UPORT_COMN)\src\os_cpu_c.c
	$(ARMCC) $(CFLAGS) $(UPORT_COMN)\src\os_cpu_c.c -o $(OBJ)\os_cpu_c.o -Errors $(ERR)\os_cpu_c.err

$(OBJ)\ucos_ii.o: ..\..\source\ucos_ii.c
	$(ARMCC) $(CFLAGS) ..\..\source\ucos_ii.c -o $(OBJ)\ucos_ii.o -Errors $(ERR)\ucos_ii.err

$(OBJ)\ex1.o: ..\src\ex1.c $(UPORT_2410)\inc\S3c2410x.h $(UPORT_2410)\inc\frmwrk.h
	$(ARMCC) $(CFLAGS) ..\src\ex1.c -o $(OBJ)\ex1.o -Errors $(ERR)\ex1.err

$(OBJ)\HwrDisplay.o: $(UPORT_2410)\src\HwrDisplay.c $(UPORT_2410)\inc\S3C2410A.h
	$(ARMCC) $(CFLAGS) $(UPORT_2410)\src\HwrDisplay.c -o $(OBJ)\HwrDisplay.o -Errors $(ERR)\HwrDisplay.err

$(OBJ)\HwrKeyMgr.o: $(UPORT_2410)\src\HwrKeyMgr.c $(UPORT_2410)\inc\S3C2410A.h
	$(ARMCC) $(CFLAGS) $(UPORT_2410)\src\HwrKeyMgr.c -o $(OBJ)\HwrKeyMgr.o -Errors $(ERR)\HwrKeyMgr.err
	
$(OBJ)\HwrInterrupts.o: $(UPORT_2410)\src\HwrInterrupts.c $(UPORT_2410)\inc\S3C2410A.h
	$(ARMCC) $(CFLAGS) $(UPORT_2410)\src\HwrInterrupts.c -o $(OBJ)\HwrInterrupts.o

$(OBJ)\HwrTimer.o: $(UPORT_2410)\src\HwrTimer.c $(UPORT_2410)\inc\S3C2410A.h
	$(ARMCC) $(CFLAGS) $(UPORT_2410)\src\HwrTimer.c -o $(OBJ)\HwrTimer.o -Errors $(ERR)\HwrTimer.err

$(OBJ)\queue.o: $(UPORT_2410)\src\queue.c
	$(ARMCC) $(CFLAGS) $(UPORT_2410)\src\queue.c -o $(OBJ)\queue.o -Errors $(ERR)\queue.err

$(OBJ)\HwrRTCTimer.o: $(UPORT_2410)\src\HwrRTCTimer.c
	$(ARMCC) $(CFLAGS) $(UPORT_2410)\src\HwrRTCTimer.c -o $(OBJ)\HwrRTCTimer.o -Errors $(ERR)\HwrRTCTimer.err

#**************************************************************************
#* End
#**************************************************************************
