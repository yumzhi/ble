;
; MODULE:         sys_boot.s
; PROJECT:        BlueStream
; MAINTAINER:     Ivan Griffin
; CREATION DATE:  28 September 2000
; DESCRIPTION:    common ARM system boot-code
;
; SOURCE CONTROL: $Id: sys_boot.s,v 1.11 2004/07/07 14:30:48 namarad Exp $
;
; REVISION HISTORY:
;    14.Sept.2000         IG      Based on sys_boot.s for AEB
;
; LICENSE:
;     This source code is copyright (c) 2000-2004 Ceva Inc.,
;     All rights reserved.
;
    PRESERVE8

    AREA SYS_Boot, CODE, READONLY

    EXPORT SYSboot_Startup
    ;EXPORT SYSboot_Warm_Startup

    IF :DEF: ADS_TOOLCHAIN_SUPPORTED
        IMPORT SYS_Main[WEAK]
    ELSE
        IMPORT SYS_Main[WEAK]
    ENDIF

    IMPORT SYSpost_Test[WEAK]
    IMPORT hc_warm_reset[WEAK]

    IMPORT SYSirq_IRQ_Handler ; merge from BK3515
    IMPORT SYSirq_FIQ_Handler ; merge from BK3515

    IMPORT Enable_16M_Clock_When_Boot;import new function for turon CLK,lookup BK3515

    IMPORT SYSbuild_Info_Dummy[WEAK]
    ;IMPORT SYS_Vector_Init_Block, WEAK            ; dummy to prevent linker
                                                   ; stripping vector table
;
; common ARM Definitions
;
;move from Vector.s start
;MODE_USR        EQU     0x10
;MODE_FIQ        EQU     0x11
;MODE_IRQ        EQU     0x12
;MODE_SVC        EQU     0x13
;MODE_ABT        EQU     0x17
;MODE_UND        EQU     0x1B
;MODE_SYS        EQU     0x1F ; available on ARM Arch 4 and later

;I_BIT           EQU     0x80 ; when I bit is set, IRQ is disabled
;F_BIT           EQU     0x40 ; when F bit is set, FIQ is disabled

; Amount of memory (in bytes) allocated for stacks for each ARM mode
; Tailor thoese values to your application needs
;USR_STACK_LEGTH     EQU         256     ;64
;SVC_STACK_LEGTH     EQU         16
;FIQ_STACK_LEGTH     EQU         256     ;16
;IRQ_STACK_LEGTH     EQU         256     ;64
;ABT_STACK_LEGTH     EQU         16
;UND_STACK_LEGTH     EQU         16

;OFFSET_FIQ_STACK         EQU     0
;OFFSET_IRQ_STACK         EQU     OFFSET_FIQ_STACK + FIQ_STACK_LEGTH
;OFFSET_SVC_STACK         EQU     OFFSET_IRQ_STACK + IRQ_STACK_LEGTH
;OFFSET_ABT_STACK         EQU     OFFSET_SVC_STACK + SVC_STACK_LEGTH
;OFFSET_UND_STACK         EQU     OFFSET_ABT_STACK + ABT_STACK_LEGTH
;OFFSET_USR_STACK         EQU     OFFSET_UND_STACK + UND_STACK_LEGTH
;move from Vector.s end

ICU_BASE          EQU    0x00800000
ICU_CLKSEL_LOC    EQU    0x01
ICU_CPUCLK_LOC    EQU    0x04

MODE_USR          EQU    0x10
MODE_FIQ          EQU    0x11
MODE_IRQ          EQU    0x12
MODE_SVC          EQU    0x13
MODE_ABORT        EQU    0x14
MODE_UNDEF        EQU    0x1B
MODE_SYS          EQU    0x1F

I_BIT             EQU    0x80
F_BIT             EQU    0x40

;
; common ARM code
;

;
; THIS IS THE ENTRY POINT FOR THE ROM BUILD!!!
; On power-up, the flash is located at 0x00000000
; Program Counter starts at 0x00000000, and we jump to
; high flash at 0x04000000
;
    IF :DEF: BUILD_FOR_ROM
    ENTRY
    INFO 0, "Building for ROM"
    ENDIF
    ;LDR PC, =SYSboot_Startup
    EXPORT SYS_Vector_Init_Block

SYS_Vector_Init_Block
    LDR PC, _Reset_Addr
    LDR PC, _Undefined_Addr
    LDR PC, _SWI_Addr
    LDR PC, _Prefetch_Abort_Addr
    LDR PC, _Data_Abort_Addr
    NOP
    LDR PC, _IRQ_Addr ;restore for current hardware
    LDR PC, _FIQ_Addr ;restore for current hardware
    
_Reset_Addr DCD SYSboot_Startup
_Undefined_Addr DCD _Undefined_Handler
_SWI_Addr DCD _SWI_Handler
_Prefetch_Abort_Addr DCD _Prefetch_Abort_Handler
_Data_Abort_Addr DCD  _Abort_Handler
Nouse DCD 0                       ; Reserved Vector
_IRQ_Addr DCD SYSirq_IRQ_Handler ; SYSirq_IRQ_Handler  

_FIQ_Addr              DCD SYSirq_FIQ_Handler
_Undefined_Handler
_SWI_Handler
_Prefetch_Abort_Handler
_Abort_Handler
    B _Abort_Handler

SYSboot_Startup
    BL      _SYSboot_Setup_Stacks_Disable_Intr ;charles temp remove

    BL      Enable_16M_Clock_When_Boot ;new function for turon CLK,lookup BK3515
    BL      _SYSboot_ZI_Init ;charles temp remove
    BL      _SYSboot_Copy_CODE_To_SRAM ;charles temp remove

    IF :DEF: ADS_TOOLCHAIN_SUPPORTED
        B   SYS_Main                          ; and go!
    ELSE
        LDR PC, =SYS_Main                     ; and go!
    ENDIF

_Dummy
    LDR PC, =SYS_Vector_Init_Block            ; dummy to prevent linker - not working for ADS linker
    LDR PC, =SYSbuild_Info_Dummy              ; dummy to prevent linker - not working for ADS linker
                                              ; stripping vector table
_SYSboot_halt   B       _SYSboot_halt         ; should never reach here!!!


; **********************************************************************
;
; Need to include Target Specific ARM Code
;
    INCLUDE sys_boot_chimera.s
;
; FUNCTION:     _SYSboot_Setup_Stacks_Disable_Intr
; DESCRIPTION:  Setup stack pointers for IRQ Mode, FIQ Mode, SVC Mode
;               ensuring all the time that IRQ and FIQ are disabled
;

;_SYSboot_Setup_Stacks_Disable_Intr
    ;LDR     R0, =0xDEADBEEF
    ;LDR     R1, =USR_STACK
    ;STR     R0, [R1]

    ;MOV     R0, #MODE_IRQ:OR:I_BIT:OR:F_BIT
    ;MSR     CPSR_c, R0
    ;LDR     R13, =IRQ_STACK

    ;MOV     R0, #MODE_FIQ:OR:I_BIT:OR:F_BIT
    ;MSR     CPSR_c, R0
    ;LDR     R13, =FIQ_STACK
    ;LDR     R0, =0xDEADBEEF
    ;STR     R0, [R13]

    ;MOV     R0, #MODE_SYS:OR:I_BIT:OR:F_BIT
    ;MSR     CPSR_c, R0
    ;LDR     R13, =SYS_STACK
    ;LDR     R0, =0xDEADBEEF
    ;STR     R0, [R13]

    ;MOV     R0, #MODE_SVC:OR:I_BIT:OR:F_BIT
    ;MSR     CPSR_c, R0
    ;LDR     R13, =SVC_STACK
    ;LDR     R0, =0xDEADBEEF
    ;STR     R0, [R13]
    ;MOV     PC, LR
; **********************************************************************

;
; Generic ARM routines to setup the LSRAM and SRAM and jump into C code
;

;
; FUNCTION:     _SYSboot_Copy_CODE_To_LSRAM
; DESCRIPTION:  copy vectors to LSRAM
;
;_SYSboot_Copy_CODE_To_LSRAM
;IMPORT |Load$$LSRAM$$Base|[WEAK]        ; Load Addr of RAM (Local RW)
;IMPORT |Image$$LSRAM$$Base|[WEAK]       ; Exec Addr of RAM (Local RW)
;IMPORT |Image$$LSRAM$$Length|[WEAK]     ; Length of RAM Exec (Local RW)

    ;LDR     R0, =|Load$$LSRAM$$Base|
    ;LDR     R1, =|Image$$LSRAM$$Base|
    ;MOV     R2, R1
    ;LDR     R4, =|Image$$LSRAM$$Length|
    ;ADD     R2, R2, R4
    ;B       _copy

;
; FUNCTION:     _SYSboot_Setup_Temp_POST_Stack
; DESCRIPTION:  setup a temporary stack in LSRAM for power-on self test
;
; _SYSboot_Setup_Temp_POST_Stack
    ;MOV     R0, #MODE_SVC:OR:I_BIT:OR:F_BIT
    ;MSR     CPSR_c, R0

    ;LDR     R13, =|Image$$LSRAM$$Base|
    ;LDR     R4, =|Image$$LSRAM$$Length|
    ;ADD     R13, R13, R4

    ;MOV     PC, LR

;
; FUNCTION:     _SYSboot_Copy_CODE_To_SRAM
; DESCRIPTION:  copy main stack code from FLASH/ROM to SRAM
;
_SYSboot_Copy_CODE_To_SRAM

    IMPORT |Load$$SRAM$$Base|[WEAK]         ; Load Addr of RAM (RW)
    IMPORT |Image$$SRAM$$Base|[WEAK]        ; Exec Addr of RAM (RW)
    IMPORT |Image$$SRAM$$Length|[WEAK]      ; Length of RAM Exec (RW)

    LDR     R0, =|Load$$SRAM$$Base|
    LDR     R1, =|Image$$SRAM$$Base|
    MOV     R2, R1
    LDR     R4, =|Image$$SRAM$$Length|
    ADD     R2, R2, R4
_copy                                       ; block copy function
    CMP r1, r2
    LDRLO   R3, [r0], #4
    STRLO   R3, [r1], #4
    BLO     _copy
    BX lr
;
; FUNCTION:     _SYSboot_ZI_Init
; DESCRIPTION:  Initialise Zero-Init Data Segment
;
_SYSboot_ZI_Init
;IMPORT |Image$$SRAM$$ZI$$Base|,   WEAK    ; Exec Addr of RAM (ZI)
;IMPORT |Image$$SRAM$$ZI$$Length|, WEAK    ; Length of RAM Exec (ZI)
;IMPORT |Image$$SRAM$$ZI$$Base|            ; Exec Addr of RAM (ZI)
;IMPORT |Image$$SRAM$$ZI$$Length|          ; Length of RAM Exec (ZI)

    ;LDR R2, =|Image$$SRAM$$ZI$$Length|
    ;LDR R0, =|Image$$SRAM$$ZI$$Base|
    ;MOV R1, R0
    ;ADD R1, R1, R2
    ;MOV R2, #0
    ;_ZI_Loop
    ;CMP R0, R1
    ;STRLO R2, [R0], #4
    ;BLO _ZI_Loop
    ;MOV PC, LR
    LDR R2, =|SIZE_OF_RAM_MEMORY|          ;total 30K
    LDR R0, =|BOTTOM_OF_RAM_MEMORY|
    MOV R1, R0
    ADD R1, R1, R2
    MOV R2, #0
_ZI_Loop
    CMP R0, R1
    STRLO R2, [R0], #4
    BLO _ZI_Loop
    BX lr

;IMPORT |Image$$LSRAM$$ZI$$Limit|, WEAK
;EXPORT __user_initial_stackheap

;__user_initial_stackheap
    ;LDR R0, =|Image$$LSRAM$$ZI$$Limit|
    ;MOV PC, LR
    EXPORT __user_initial_stackheap


__user_initial_stackheap
    LDR   r0,=bottom_of_heap
    BX lr


__rt_div0
    B       __rt_div0
    AREA    Myheap, DATA, NOINIT, ALIGN=2

bottom_of_heap     SPACE   256

    END
