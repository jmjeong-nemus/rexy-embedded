                                                  ARM Port of UCOS-II

Date         : 04.01.2003
Version      : 1.10
UCOS-II      : Version 2.61 or lesser
IDE          : SDT 2.51 & ADS 1.2

ported by  Michael Anburaj  
Homepage: http://geocities.com/michaelanburaj/
e-mail  : michaelanburaj@hotmail.com


Installation : UNZIP to desired drive (C:\ is recommended)

Note: All paths & build reference made to UCOS files are relative. So UCOS source & ARM_UCOS must be extracted to the same path.

Folders:
   ARM_UCOS     : Contains the generic code files required for a project development on the ARM core.
                  common   : Contains the ucos ARM platform code.
                  EP73XX   : Contains the EP7312 specific frame work code.
                  S3c2410x : Contains the S3c2410x specific frame work code.
   Ex1_EP73XX   : Sample UCOS application (Ex1) code & build files for EP7312.
   Ex1_S3c2410x : Sample UCOS application (Ex1) code & build files for S3c2410x.

Building & Running the project:
Refer to the notes.doc in the respective doc folder under the ARM processor example project.


Important!!!

UCOS_ARM based applications can either run standalone or in conjunction with some of the following software:

1. ICE boot - For ICE booted applications. The ice boot provided with the package must be burnt at RESET (0x00000000)
2. TMON - For TMON booted RAM & ROM applications. TMON must be burnt at RESET (0x00000000). Samsung’s monitor can be used for their processor based boards.

Very important: This port cannot work with other monitors (like Angel) or HALs!!! There should not be any other monitor program other than TMON underneath the ARM_UCOS ported code.



Notes:
1. Critical section manipulation methods 2 & 3 are implemented for SDT. When built using ADS use only method #3 (very important)
2. All interrupts are under the control of the OS (UCOS-II). And the main IRQ handler is  C_IRQHandler() function in FRMWRK.c
3. Interrupts operate purely in the IRQ/FIQ stack.
4. Porting this to other processors is very simple. Just modify the following functions:
        a. C_IRQHandler() in FRMWRK.c
        b. FRMWRK_vStartTicker() in FRMWRK.c
        c. Init.s
        d. CONSOL.c (not important)


What's new in ver 1.xx:
1. It also builds under ADS 1.x
2. Common Batch build files added for both SDT & ADS.
3. JTAG Target type added. Also ice_boot for the same is provided.
4. Optimized some parts of the assembly code.
5. Added port for S3c2410x, ARM920T from Samsung.


Note: About the EP73xx port
ADS 1.2 has an assembler bug that causes mmu.o to bloat (increased to 16KB). Because of which the executable also increases in size. This bug is not seen in SDT or RVCT tools.

Image          Size (approx.)    Tools
r-ex1.bin      49KB              ADS
r-ex1.bin      33KB              SDT



Untested ports for the following ARM processors are available with me:
1. CS89712 from CIRRUS Logic
2. KS32C41000 (S3C44A0X) from Samsung
3. S3c4510 (SNDS100 Board version 1.0)
4. S3c4530 (SNDS300 Board)

If you need any of these, please email me at michaelanburaj@hotmail.com


This port can be modified very easily to make it work on any ARM core (ARM7, ARM9) based microprocessor or micro-controller. Please feel free to get in touch with me if you have difficulties doing this.

Run-time C library initialization code is not added with the port (ctype functions & malloc need this). Please contact me if you need it.


Cheers,
Michael Anburaj.