; StringConversion.s
; Student names: Frank Collebrusco & Jasmine Pok
; Last modification date: 10/26/22
; Runs on TM4C123
; EE319K lab 7 number to string conversions
;
; Used in ECE319K Labs 7,8,9,10. You write these two functions

 
    EXPORT   Dec2String
	EXPORT	 sDec2String
    EXPORT   Fix2String
    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
    PRESERVE8




;-----------------------Dec2String-----------------------
; Convert a 32-bit number into unsigned decimal format
; String the string into the empty array add null-termination
; Input: R0 (call by value) 32-bit unsigned number
;        R1 pointer to empty array
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
Dec2String
nd EQU 0
c EQU 4
d EQU 8
i EQU 12
bufd EQU 16;				bind
	PUSH{R11,LR}
	SUB SP,SP,#24;		alloc
	MOV R11,SP;			stack frame
	
	STR R1,[R11,#bufd];	save buf
	STR R0,[R11,#nd];	save n
	MOV R3,#0
	STR R3,[R11,#c];	init c = 0
	STR R3,[R11,#i];	init i = 0
	
	LDR R0,[R11,#bufd]
	MOV R1,#0
	STR R1,[R0],#4
	STR R1,[R0],#4
	STR R1,[R0],#4;		clear buffer
	
	
loop1;					loop1 calcs each digit and pushes them to the stack
	LDR R0,[R11,#nd]
	MOV R1,#10
	BL UMod
	STR R0,[R11,#d];	d=n%10
	
	LDR R0,[R11,#nd]
	MOV R1,#10
	UDIV R0,R0,R1
	STR R0,[R11,#nd];	n=n/10
	
	LDR R0,[R11,#d];	push d onto stack
	PUSH{R0}
	
	LDR R0,[R11,#c]
	ADD R0,R0,#1
	STR R0,[R11,#c];	c++;
	
	LDR R0,[R11,#nd]
	CMP R0,#0
	BNE loop1;			loop if (n!=0)
	
loop2;					loop2 pops digis off stack to reverse their order
	POP{R0}
	ADD R0,R0,#0x30
	LDR R1,[R11,#bufd]
	LDR R2,[R11,#i]
	ADD R1,R1,R2;		pop->x
	STRB R0,[R1];		buf[i]=x
	
	LDR R0,[R11,#c]
	SUB R0,R0,#1
	STR R0,[R11,#c];	c--;
	
	LDR R0,[R11,#i]
	ADD R0,R0,#1
	STR R0,[R11,#i];	i++;
	
	LDR R0,[R11,#c]
	CMP R0,#0
	BNE loop2;			loop if c!=0
	
	LDR R0,[R11,#bufd]
	MOV R1,#0
	LDR R2,[R11,#i]
	STRB R1,[R0,R2];	null terminate
	
	ADD SP,SP,#24
	POP{R11,PC};		dealloc
;* * * * * * * * End of Dec2String * * * * * * * *

;-----------------------sDec2String-----------------------
; Convert a 32-bit number into signed decimal format
; String the string into the empty array add null-termination
; Input: R0 (call by value) 32-bit signed number
;        R1 pointer to empty array
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
sDec2String
nds EQU 0
cs EQU 4
ds EQU 8
is EQU 12
bufds EQU 16;				bind
	PUSH{R11,LR}
	SUB SP,SP,#24;		alloc
	MOV R11,SP;			stack frame
	
	STR R1,[R11,#bufds];	save buf
	STR R0,[R11,#nds];	save n
	MOV R3,#0
	STR R3,[R11,#cs];	init c = 0
	STR R3,[R11,#is];	init i = 0
	
	LDR R0,[R11,#bufds]
	MOV R1,#0
	STR R1,[R0],#4
	STR R1,[R0],#4
	STR R1,[R0],#4;		clear buffer

	LDR R0,[R11,#nds];	add minus sign if needed
	CMP R0,#0;
	BGE posi
	MOV R1,#0x2D
	LDR R2,[R11,#bufds]
	STR R1,[R2]
	LDR R1,[R11,#is]
	ADD R1,R1,#1
	STR R1,[R11,#is]
	
	LDR R0,[R11,#nds]
	MOV R1,#-1;
	SDIV R0,R0,R1;	flip sign
	STR R0,[R11,#nds]
posi
	
	
loop1s;					loop1 calcs each digit and pushes them to the stack
	LDR R0,[R11,#nds]
	MOV R1,#10
	BL UMod
	STR R0,[R11,#ds];	d=n%10
	
	LDR R0,[R11,#nds]
	MOV R1,#10
	UDIV R0,R0,R1
	STR R0,[R11,#nds];	n=n/10
	
	LDR R0,[R11,#ds];	push d onto stack
	PUSH{R0}
	
	LDR R0,[R11,#cs]
	ADD R0,R0,#1
	STR R0,[R11,#cs];	c++;
	
	LDR R0,[R11,#nds]
	CMP R0,#0
	BNE loop1s;			loop if (n!=0)
	
loop2s;					loop2 pops digis off stack to reverse their order
	POP{R0}
	ADD R0,R0,#0x30
	LDR R1,[R11,#bufds]
	LDR R2,[R11,#is]
	ADD R1,R1,R2;		pop->x
	STRB R0,[R1];		buf[i]=x
	
	LDR R0,[R11,#cs]
	SUB R0,R0,#1
	STR R0,[R11,#cs];	c--;
	
	LDR R0,[R11,#is]
	ADD R0,R0,#1
	STR R0,[R11,#is];	i++;
	
	LDR R0,[R11,#cs]
	CMP R0,#0
	BNE loop2s;			loop if c!=0
	
	LDR R0,[R11,#bufds]
	MOV R1,#0
	LDR R2,[R11,#is]
	STRB R1,[R0,R2];	null terminate
	
	ADD SP,SP,#24
	POP{R11,PC};		dealloc
;* * * * * * * * End of Dec2String * * * * * * * *


; -----------------------Fix2String----------------------
; Create characters for LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
;          R1 pointer to empty array
; Outputs: none
; E.g., R0=0,    then create "0.000 "
;       R0=3,    then create "0.003"
;       R0=89,   then create "0.089"
;       R0=123,  then create "0.123"
;       R0=9999, then create "9.999"
;       R0>9999, then create "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
Fix2String

	MOV R2,R1
	MOV R3,#0
	STR R3,[R2],#4
	STR R3,[R2],#4
	STR R3,[R2],#4;	clear buffer

	LDR R2,=9999
	CMP R0,R2
	BLS validIn
	MOV R2,#0x2E;	decimal point
	MOV R3,#0x2A;	asterisk
	STRB R3,[R1],#1
	STRB R2,[R1],#1
	STRB R3,[R1],#1
	STRB R3,[R1],#1
	STRB R3,[R1],#1;	fills buffer with '*.***'
	B returnF2S
validIn
firstDigi EQU 0;bind
n EQU 4
buf EQU 8
	PUSH{R11,LR}
	SUB SP,SP,#16
	MOV R11,SP;		alloc -------
	
	STR R0,[R11,#n];	get & str first digit
	STR R1,[R11,#buf]
	MOV R3,#1000
	UDIV R0,R0,R3
	STR R0,[R11,#firstDigi]
	LDR R1,[R11,#buf]
	ADD R0,R0,#0x30;
	STRB R0,[R1]
	
	MOV R0,#0x2E;		str deci point
	STR R0,[R1,#1]
	
	LDR R0,[R11,#n]
	LDR R1,=1000
	BL UMod
	STR R0,[R11,#n];	n=n%1000
	
	MOV R3,#100
	UDIV R2,R0,R3
	LDR R1,[R11,#buf]
	ADD R2,R2,#0x30;
	STRB R2,[R1,#2];		buf[2]=n/100
	
	LDR R0,[R11,#n]
	MOV R1,#100
	BL UMod;			n=n%100
	STR R0,[R11,#n]
	
	MOV R3,#10
	UDIV R2,R0,R3
	ADD R2,R2,#0x30
	LDR R1,[R11,#buf];	buf[3]=n/10
	STRB R2,[R1,#3]
	
	MOV R1,#10
	BL UMod;			n=n%10
	STR R0,[R11,#n]
	
	LDR R1,[R11,#buf]
	ADD R0,R0,#0x30
	STRB R0,[R1,#4];	buf[4]=n
	MOV R0,#0x20;		buf[5]=' '
	STRB R0,[R1,#5]
	MOV R0,#0
	STRB R0,[R1,#6];	buf[6]=0x00, null terminate
	
	ADD SP,SP,#16;	dealloc -----
	POP{R11,LR}
returnF2S
	BX LR

UMod ; 2 inputs: R0 & R1, returns R0%R1
	UDIV R2,R0,R1
	MUL R2,R1
	SUB R0,R0,R2
	BX LR

Mod ; 2 inputs: R0 & R1, returns R0%R1
	SDIV R2,R0,R1
	MUL R2,R1
	SUB R0,R0,R2
	BX LR
	

 


     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
