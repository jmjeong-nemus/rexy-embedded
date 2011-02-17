;**************************************************************************
;                                                                         *
;   PROJECT     : ARM port for UCOS-II                                    *
;                                                                         *
;   MODULE      : INIT.s                                                  *
;                                                                         *
;   AUTHOR      : Michael Anburaj                                         *
;                 URL  : http://geocities.com/michaelanburaj/             *
;                 EMAIL: michaelanburaj@hotmail.com                       *
;                                                                         *
;   PROCESSOR   : S3c2410x (32 bit ARM920T RISC core from Samsung)        *
;                                                                         *
;   IDE         : SDT 2.51 & ADS 1.2                                      *
;                                                                         *
;   DESCRIPTION :                                                         *
;   S3c2410x processor Start up assembly code file.                       *
;                                                                         *
;*************************************************************************/


        GBLL     PLL_ON_START
PLL_ON_START    SETL     {FALSE}

;        GBLL    ENDIAN_CHANGE
;ENDIAN_CHANGE    SETL    {FALSE}

        GBLA    ENTRY_BUS_WIDTH
ENTRY_BUS_WIDTH    SETA    16

;BUSWIDTH = 16,32
        GBLA    BUSWIDTH            ; max. bus width for the GPIO configuration
BUSWIDTH    SETA    32    


        GBLA    FCLK
FCLK            SETA    184000000

    [   FCLK = 176000000
M_MDIV  EQU     80                  ; Fin=12.0MHz Fout=176.0MHz
M_PDIV  EQU     1
M_SDIV  EQU     1
    ]

    [   FCLK = 180000000
M_MDIV  EQU     82                  ; Fin=12.0MHz Fout=180.0MHz
M_PDIV  EQU     1
M_SDIV  EQU     1
    ]

    [   FCLK = 184000000
M_MDIV  EQU     84                  ; Fin=12.0MHz Fout=184.0MHz
M_PDIV  EQU     1
M_SDIV  EQU     1
    ]

    [   FCLK = 192000000
M_MDIV  EQU     88                  ; Fin=12.0MHz Fout=192.0MHz
M_PDIV  EQU     1
M_SDIV  EQU     1
    ]

    [   FCLK = 200000000
M_MDIV  EQU     92                  ; Fin=12.0MHz Fout=200.0MHz
M_PDIV  EQU     1
M_SDIV  EQU     1
    ]


SRAM_SADDR    EQU 0x40000000        ; SRAM starting address
SRAM_SIZE     EQU 4*1024            ; 4K internal SRAM

SDRAM_SADDR   EQU 0x30000000        ; SDRAM starting address
SDRAM_SIZE    EQU 64*1024*1024      ; 64M SDRAM
ISR_BADDR     EQU 0x33ffff00        ; plus 0x20 is the RW base address -linker setting


SRAM_EADDR    EQU SRAM_SADDR+SRAM_SIZE-1 ; SRAM end address
SDRAM_EADDR   EQU SDRAM_SADDR+SDRAM_SIZE-1 ; SDRAM end address

MMUTT_SIZE    EQU 16*1024           ; It has to be in multiples of 16K
MMUTT_SADDR   EQU SDRAM_SADDR
MMUTT_EADDR   EQU MMUTT_SADDR+MMUTT_SIZE

_SVC_STKSIZE  EQU 1024*64
_UND_STKSIZE  EQU 256
_ABT_STKSIZE  EQU 256
_IRQ_STKSIZE  EQU 1024*1
_FIQ_STKSIZE  EQU 256

STK_SIZE      EQU _SVC_STKSIZE+_UND_STKSIZE+_ABT_STKSIZE+_IRQ_STKSIZE+_FIQ_STKSIZE

STK_SADDR     EQU ISR_BADDR-STK_SIZE


; Register definition
    GET S3c2410x.a
    GET memcfg.a


BIT_SELFREFRESH EQU    (1<<22)


; Pre-defined constants
USERMODE      EQU 0x10
FIQMODE       EQU 0x11
IRQMODE       EQU 0x12
SVCMODE       EQU 0x13
ABORTMODE     EQU 0x17
UNDEFMODE     EQU 0x1b
MODEMASK      EQU 0x1f
NOINT         EQU 0xc0


        MACRO
$HandlerLabel HANDLER $HandleLabel

$HandlerLabel
        sub sp,sp,#4
        stmfd sp!,{r0}
        ldr r0,=$HandleLabel
        ldr r0,[r0]
        str r0,[sp,#4]
        ldmfd sp!,{r0,pc}
        MEND

; Check if tasm.exe(armasm -16 ...@ADS 1.0) is used.
    GBLL    THUMBCODE
    [ {CONFIG} = 16 
THUMBCODE SETL  {TRUE}
        CODE32
        |   
THUMBCODE SETL  {FALSE}
        ]

        MACRO
    MOV_PC_LR
        [ THUMBCODE
            bx lr
        |
            mov    pc,lr
        ]
    MEND

        MACRO
    MOVEQ_PC_LR
        [ THUMBCODE
            bxeq lr
        |
            moveq pc,lr
        ]
    MEND


        AREA  |Init|, CODE, READONLY

        ENTRY

        ASSERT    :DEF:ENDIAN_CHANGE
        [ ENDIAN_CHANGE
            ASSERT  :DEF:ENTRY_BUS_WIDTH
            [ ENTRY_BUS_WIDTH=32
                b    ChangeBigEndian         ; DCD 0xea000007 
            ]
        
            [ ENTRY_BUS_WIDTH=16
                andeq    r14,r7,r0,lsl #20   ; DCD 0x0007ea00
            ]
        
            [ ENTRY_BUS_WIDTH=8
                streq    r0,[r0,-r10,ror #1] ; DCD 0x070000ea
            ]
        |
            b    ResetHandler  
        ]
        
        b HandlerUndef              ; handlerUndef
        b HandlerSWI                ; SWI interrupt handler
        b HandlerPabort             ; handlerPAbort
        b HandlerDabort             ; handlerDAbort
        b .                         ; handlerReserved
        b HandlerIRQ                ; handlerIRQ
        b HandlerFIQ                ; handlerFIQ
; @0x20
        b    EnterPWDN
; @0x24
ChangeBigEndian
    [ ENTRY_BUS_WIDTH=32
        DCD    0xee110f10           ; 0xee110f10 => mrc p15,0,r0,c1,c0,0
        DCD    0xe3800080           ; 0xe3800080 => orr r0,r0,#0x80;  //Big-endian
        DCD    0xee010f10           ; 0xee010f10 => mcr p15,0,r0,c1,c0,0
    ]
    [ ENTRY_BUS_WIDTH=16
        DCD 0x0f10ee11
        DCD 0x0080e380    
        DCD 0x0f10ee01    
    ]
    [ ENTRY_BUS_WIDTH=8
        DCD 0x100f11ee    
        DCD 0x800080e3    
        DCD 0x100f01ee    
    ]
    DCD 0xffffffff                  ; swinv 0xffffff is similar with NOP and run well in both endian mode. 
    DCD 0xffffffff
    DCD 0xffffffff
    DCD 0xffffffff
    DCD 0xffffffff
    b ResetHandler
    
    
; Function for entering power down mode
; 1. SDRAM should be in self-refresh mode.
; 2. All interrupt should be maksked for SDRAM/DRAM self-refresh.
; 3. LCD controller should be disabled for SDRAM/DRAM self-refresh.
; 4. The I-cache may have to be turned on. 
; 5. The location of the following code may have not to be changed.

; void EnterPWDN(int CLKCON); 
EnterPWDN            
    mov r2,r0                       ; r2=rCLKCON
    tst r0,#0x8                     ; POWER_OFF mode?
    bne ENTER_POWER_OFF

ENTER_STOP    
    ldr r0,=REFRESH        
    ldr r3,[r0]                     ; r3=rREFRESH    
    mov r1, r3
    orr r1, r1, #BIT_SELFREFRESH
    str r1, [r0]                    ; Enable SDRAM self-refresh

    mov r1,#16                      ; wait until self-refresh is issued. may not be needed.
0   subs r1,r1,#1
    bne %B0

    ldr r0,=CLKCON                  ; enter STOP mode.
    str r2,[r0]    

    mov r1,#32
0   subs r1,r1,#1                   ; 1) wait until the STOP mode is in effect.
    bne %B0                         ; 2) Or wait here until the CPU&Peripherals will be turned-off
                                    
    ; Entering POWER_OFF mode, only the reset by wake-up is available.

    ldr r0,=REFRESH                 ; exit from SDRAM self refresh mode.
    str r3,[r0]
    
    MOV_PC_LR

ENTER_POWER_OFF    
    ; NOTE.
    ; 1) rGSTATUS3 should have the return address after wake-up from POWER_OFF mode.
    
    ldr r0,=REFRESH        
    ldr r1,[r0]                     ; r1=rREFRESH    
    orr r1, r1, #BIT_SELFREFRESH
    str r1, [r0]                    ; Enable SDRAM self-refresh

    mov r1,#16                      ; Wait until self-refresh is issued,which may not be needed.
0   subs r1,r1,#1
    bne %B0

    ldr     r1,=MISCCR
    ldr    r0,[r1]
    orr    r0,r0,#(7<<17)           ; Make sure that SCLK0:SCLK->0, SCLK1:SCLK->0, SCKE=L during boot-up 
    str    r0,[r1]

    ldr r0,=CLKCON
    str r2,[r0]    

    b .                             ; CPU will die here.
    

WAKEUP_POWER_OFF
    ; Release SCLKn after wake-up from the POWER_OFF mode.

    ldr     r1,=MISCCR
    ldr    r0,[r1]
    bic    r0,r0,#(7<<17)           ; SCLK0:0->SCLK, SCLK1:0->SCLK, SCKE:L->H
    str    r0,[r1]

    ; Set memory control registers
    ldr    r0,=SMRDATA
    ldr    r1,=BWSCON               ; BWSCON Address
    add    r2, r0, #52              ; End address of SMRDATA
0       
    ldr    r3, [r0], #4    
    str    r3, [r1], #4    
    cmp    r2, r0        
    bne    %B0

    mov r1,#256
0   subs r1,r1,#1                   ; 1) wait until the SelfRefresh is released.
    bne %B0        
    
    ldr r1,=GSTATUS3                ; GSTATUS3 has the start address just after POWER_OFF wake-up
    ldr r0,[r1]
    mov pc,r0

    LTORG   
HandlerFIQ     HANDLER HandleFIQ
HandlerIRQ     HANDLER HandleIRQ
HandlerUndef   HANDLER HandleUndef
HandlerSWI     HANDLER HandleSWI
HandlerDabort  HANDLER HandleDabort
HandlerPabort  HANDLER HandlePabort


;*****************************************************************************
;
; ResetHandler is the startup code to be used
;
;*****************************************************************************
ResetHandler
        ldr    r0,=WTCON            ; watch dog disable 
        ldr    r1,=0x0         
        str    r1,[r0]

        ldr    r0,=INTMSK
        ldr    r1,=0xffffffff       ; all interrupt disable
        str    r1,[r0]

        ldr    r0,=INTSUBMSK
        ldr    r1,=0x7ff            ; all sub interrupt disable
        str    r1,[r0]

        ; To reduce PLL lock time, adjust the LOCKTIME register. 
        ldr    r0,=LOCKTIME
        ldr    r1,=0xffffff
        str    r1,[r0]
        
        [ PLL_ON_START
            ; Configure MPLL
            ldr    r0,=MPLLCON          
            ldr    r1,=((M_MDIV<<12)+(M_PDIV<<4)+M_SDIV)  ; Fin=12MHz,Fout=50MHz
            str    r1,[r0]
        ]

        ; Check if the boot is caused by the wake-up from POWER_OFF mode.
        ldr    r1,=GSTATUS2
        ldr    r0,[r1]
        tst    r0,#0x2
        ; In case of the wake-up from POWER_OFF mode, go to POWER_OFF_WAKEUP handler. 
        bne    WAKEUP_POWER_OFF

        EXPORT StartPointAfterPowerOffWakeUp
StartPointAfterPowerOffWakeUp

        ; Set memory control registers
        ldr    r0,=SMRDATA
        ldr    r1,=BWSCON           ; BWSCON Address
        add    r2, r0, #52          ; End address of SMRDATA
0       
        ldr    r3, [r0], #4    
        str    r3, [r1], #4    
        cmp    r2, r0        
        bne    %B0

        ; Initialize stacks
        bl InitStacks

;********************************************************************
; End of Stack Setup
;********************************************************************

        ;
        ; Copy the read-write data block from ROM to RAM.
        ;

        IMPORT  |Image$$RO$$Limit|  ; End of ROM code (=start of ROM data)
        IMPORT  |Image$$RW$$Base|   ; Base of RAM to initialize
        IMPORT  |Image$$ZI$$Base|   ; Base and limit of area
        IMPORT  |Image$$ZI$$Limit|  ; to zero initialize
        IMPORT	_init_alloc

        ldr r0,=|Image$$RO$$Limit|  ; Get pointer to ROM data
        ldr r1,=|Image$$RW$$Base|   ; and RAM copy
        ldr r3,=|Image$$ZI$$Base|        
        ; Zero init base => top of initialized data
                        
        cmp r0,r1                   ; Check that they are different
        beq %F1
0                
        cmp r1,r3                   ; Copy init data
        ldrcc r2,[r0],#4
        strcc r2,[r1],#4
        bcc %B0
1                
        ldr r1,=|Image$$ZI$$Limit|  ; Top of zero init segment
        mov r2,#0
2                
        cmp r3,r1                   ; Zero init
        strcc r2,[r3],#4
        bcc %B2

program
        ;
        ; Call the actual C program.
        ; Should never return.
        ;
        IMPORT C_vMain
	    LDR   r0,=|Image$$ZI$$Limit|
   		LDR	  r1,=STK_SADDR
        bl _init_alloc
        b C_vMain                   ; C Entry


InitStacks
        ; Don't use DRAM,such as stmfd,ldmfd......
        ; SVCstack is initialized before
        ; Under toolkit ver 2.50, 'msr cpsr,r1' can be used instead of 'msr cpsr_cxsf,r1'
        
        mrs r0,cpsr
        bic r0,r0,#MODEMASK
        orr r1,r0,#UNDEFMODE|NOINT
        msr cpsr_cxsf,r1            ; UndefMode
        ldr sp,=UndefStack
        
        orr r1,r0,#ABORTMODE|NOINT
        msr cpsr_cxsf,r1            ; AbortMode
        ldr sp,=AbortStack

        orr r1,r0,#IRQMODE|NOINT
        msr cpsr_cxsf,r1            ; IRQMode
        ldr sp,=IRQStack
        
        orr r1,r0,#FIQMODE|NOINT
        msr cpsr_cxsf,r1            ; FIQMode
        ldr sp,=FIQStack

        bic r0,r0,#MODEMASK|NOINT
        orr r1,r0,#SVCMODE
        msr cpsr_cxsf,r1            ; SVCMode
        ldr sp,=SVCStack

        ; USER mode is not initialized.

        mov pc,lr                   ; The LR register may be not valid for the mode changes.

    LTORG

SMRDATA DATA
; Memory configuration should be optimized for best performance 
; The following parameter is not optimized.                     
; Memory access cycle parameter strategy
; 1) The memory settings is  safe parameters even at HCLK=75Mhz.
; 2) SDRAM refresh period is for HCLK=75Mhz. 

        DCD (0+(B1_BWSCON<<4)+(B2_BWSCON<<8)+(B3_BWSCON<<12)+(B4_BWSCON<<16)+(B5_BWSCON<<20)+(B6_BWSCON<<24)+(B7_BWSCON<<28))
        DCD ((B0_Tacs<<13)+(B0_Tcos<<11)+(B0_Tacc<<8)+(B0_Tcoh<<6)+(B0_Tah<<4)+(B0_Tacp<<2)+(B0_PMC))   ;GCS0
        DCD ((B1_Tacs<<13)+(B1_Tcos<<11)+(B1_Tacc<<8)+(B1_Tcoh<<6)+(B1_Tah<<4)+(B1_Tacp<<2)+(B1_PMC))   ;GCS1 
        DCD ((B2_Tacs<<13)+(B2_Tcos<<11)+(B2_Tacc<<8)+(B2_Tcoh<<6)+(B2_Tah<<4)+(B2_Tacp<<2)+(B2_PMC))   ;GCS2
        DCD ((B3_Tacs<<13)+(B3_Tcos<<11)+(B3_Tacc<<8)+(B3_Tcoh<<6)+(B3_Tah<<4)+(B3_Tacp<<2)+(B3_PMC))   ;GCS3
        DCD ((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Tcoh<<6)+(B4_Tah<<4)+(B4_Tacp<<2)+(B4_PMC))   ;GCS4
        DCD ((B5_Tacs<<13)+(B5_Tcos<<11)+(B5_Tacc<<8)+(B5_Tcoh<<6)+(B5_Tah<<4)+(B5_Tacp<<2)+(B5_PMC))   ;GCS5
        DCD ((B6_MT<<15)+(B6_Trcd<<2)+(B6_SCAN))    ;GCS6
        DCD ((B7_MT<<15)+(B7_Trcd<<2)+(B7_SCAN))    ;GCS7
        DCD ((REFEN<<23)+(TREFMD<<22)+(Trp<<20)+(Trc<<18)+(Tchr<<16)+REFCNT)    



        DCD 0x32                    ; SCLK power saving mode, BANKSIZE 128M/128M

        DCD 0x30                    ; MRSR6 CL=3clk
        DCD 0x30                    ; MRSR7
;        DCD 0x20                    ; MRSR6 CL=2clk
;        DCD 0x20                    ; MRSR7

;
;*****************************************************************************
;
; Zero-initialized read/write data area for stacks.
; This area is determined by the RW value in the Linker under "entry and base".  
;*****************************************************************************
        AREA  SYS_STK, DATA, READWRITE, NOINIT

;*****************************************************************************
;
; Memory buffers to contain the stacks for the various processor modes which
; we will be using.
;
;*****************************************************************************

        ^       STK_SADDR

UserStack       #       _SVC_STKSIZE
SVCStack        #       _UND_STKSIZE
UndefStack      #       _ABT_STKSIZE
AbortStack      #       _IRQ_STKSIZE
IRQStack        #       _FIQ_STKSIZE
FIQStack        #       0 


        AREA  ISR_HOOK, DATA, READWRITE, NOINIT

        ^  ISR_BADDR
HandleReset     # 4
HandleUndef     # 4
HandleSWI       # 4
HandlePabort    # 4
HandleDabort    # 4
HandleReserved  # 4
HandleIRQ       # 4
HandleFIQ       # 4

;
;*****************************************************************************
;
        END
