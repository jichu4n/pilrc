; pilrctst.asm
;
; Test harness for pilrc
;
; By Wes Cherry
; 10 Mar 96


; The 'Appl' directive sets the application's name and four character id.

        Appl    "PilRC Test", 'pilr'

; Pilot.inc contains PalmOS constants, structure offsets, and API trap codes.

        include "Pilot.inc"


; Startup.inc contains a standard startup function. This function must be
; the first within an application and is called by the PalmOS after the app
; is loaded. The startup function in Startup.inc (__Startup__) calls the
; application-defined function PilotMain.

        include "Startup.inc"
			
		include "pilrctst.inc"
; Application-defined resource ids

kidbAIB         equ             $7FFE
kidrPREF        equ             1
kidrTVER        equ             1
kidrTAIB        equ             1000

; Some global variables (a5-relative)

        data

global kidForm.w		; current form

        code

; ---------------------------------------------------------------------------
; DWord PilotMain(Word cmd, void *cmdPBP, Word launchflags)
; PilotMain is called by the startup code and implements a simple event
; handling loop.

proc PilotMain(cmd.w, cmdPBP.l, launchFlags.w)
local   err.w
local   evt.EventType

beginproc
        tst.w   cmd(a6)                 ;sysAppLaunchCmdNormalLaunch is 0
        bne     PmReturn                ;not a normal launch, bag out

		move.w	#kidForm1,kidForm(a5)
        systrap FrmGotoForm(kidForm(a5).w)

PmEventLoop
; Doze until an event arrives

        systrap EvtGetEvent(&evt(a6), #evtWaitForever.w)

; System gets first chance to handle the event

        systrap SysHandleEvent(&evt(a6))

        tst.b   d0                      ;handled?
        bne.s   PmEventDone             ;yep

; Menu handler gets second chance to handle the event

        systrap MenuHandleEvent(&0, &evt(a6), &err(a6))

        tst.b   d0                      ;handled?
        bne.s   PmEventDone             ;yep

; Application handler gets third chance to handle the event

        call    ApplicationHandleEvent(&evt(a6))

        tst.b   d0                      ;handled?
        bne.s   PmEventDone             ;yep

; Form handler gets fourth chance to handle the event

        call    MainFormHandleEvent(&evt(a6))

        tst.b   d0                      ;handled?
        bne.s   PmEventDone             ;yep

; Still not handled. We're not interested in it anymore so let the
; default form handler take it.

        systrap FrmGetActiveForm()
        systrap FrmHandleEvent(a0.l, &evt(a6))

; Return from PilotMain when an appStopEvent is received

PmEventDone
        cmpi.w  #appStopEvent,evt+EventType.eType(a6) ;time to stop?
        bne.s   PmEventLoop             ;nope, loop until it is

PmReturn
        moveq   #0,d0
endproc


; ---------------------------------------------------------------------------
; Boolean ApplicationHandleEvent(EventType *pevt)
; Handles these events:
;       frmLoadEvent

proc ApplicationHandleEvent(pevt.l)
beginproc
        movem.l a3,-(a7)                ;save registers we're going to trash

        movea.l pevt(a6),a0             ;a0 = pevt

; Handle frmLoadEvents

        cmpi.w  #frmLoadEvent,EventType.eType(a0) ;frmLoadEvent?
        bne     AHENotHandled           ;no

; Initialize the form and make it active

        systrap FrmInitForm(EventType.data+frmLoad.formID(a0).w)
        move.l  a0,a3                   ;save a copy of FormPtr for later

        systrap FrmSetActiveForm(a0.l)

        moveq.l #1,d0                   ;event handled
        bra.s   AHEReturn

AHENotHandled
        clr.b   d0

AHEReturn
        movem.l (a7)+,a3
endproc


; ---------------------------------------------------------------------------
; Boolean MainFormHandleEvent(EventType *pevt)
; Handles these events:
;

proc MainFormHandleEvent(pevt.l)
local	pfrm.l
beginproc
        movem.l d3,-(a7)                ;save registers we're going to trash

        movea.l pevt(a6),a0             ;a0 = pevt
        move.w  EventType.eType(a0),d0

; Handle frmOpenEvent

        cmp.w   #frmOpenEvent,d0
        bne     MFH1

; Draw the form

        systrap FrmGetActiveForm()
		move.l	a0,pfrm(a6)
        systrap FrmDrawForm(a0.l)

; if we are form 12 (table), init it
        movea.l pevt(a6),a1             ;a0 = pevt
        cmp.w	#kidForm12,EventType.data+frmLoad.formID(a1)
		bne		MFH1A

		systrap FrmGetObjectIndex(pfrm(a6).l, #kidTable.w)
		systrap	FrmGetObjectPtr(pfrm(a6).l, d0.w)
		move.l	a0,d3
					
		systrap TblSetColumnUsable(d3.l, #0.w, #1.b);
		systrap TblSetColumnUsable(d3.l, #1.w, #1.b);
		systrap TblSetColumnUsable(d3.l, #2.w, #1.b);
		systrap TblSetColumnUsable(d3.l, #3.w, #1.b);
		systrap TblSetColumnUsable(d3.l, #4.w, #1.b);
		systrap TblSetRowUsable(d3.l, #0.w, #1.b);
		systrap TblSetRowUsable(d3.l, #1.w, #1.b);

		systrap TblSetItemStyle(d3.l, #0.w, #0.w, #6.b);
		systrap TblSetItemStyle(d3.l, #1.w, #0.w, #6.b);
		systrap TblSetItemStyle(d3.l, #2.w, #0.w, #6.b);
		systrap TblDrawTable(d3.l);
					
MFH1A
        moveq.l #1,d0
        bra     MFHReturn

; Handle frmMenuEvent
MFH1
        cmp.w   #menuEvent,d0
        bne     MFH2


        moveq.l #1,d0
        bra     MFHReturn

; Handle ctlSelectEvent
MFH2
        cmp.w   #ctlSelectEvent,d0
        bne     MFHNotHandled

; Is the Ok button being clicked?

		cmp.w   #kidOk,EventType.data+ctlEnter.controlID(a0)
        bne     MFHNotHandled

		addq.w	#1,kidForm(a5)
		cmp.w	#kidFormLast,kidForm(a5)
		ble		MFH3
		move.w	#kidForm1,kidForm(a5)
MFH3
		systrap	FrmGotoForm(kidForm(a5).w)
        
        bra     MFHReturn

MFHNotHandled
        clr.b   d0

MFHReturn
	movem.l (a7)+,d3
endproc



;
; Resources -----------------------------------------------------------------
;

; Application Icon Bitmap resource. Is automatically converted from Windows
; format to Pilot format and written as a 'tAIB' rather than 'Tbmp' because
; kidbAIB is a special value ($7ffe)

        res 'WBMP', kidbAIB, "pilrcic.bmp"

; 'pref' resource. Defines app launch flags, stack and heap size

        res 'pref', kidrPREF
        dc.w    sysAppLaunchFlagNewStack|sysAppLaunchFlagNewGlobals|sysAppLaunchFlagUIApp|sysAppLaunchFlagSubCall
        dc.l    $1000                           ; stack size
        dc.l    $1000                           ; heap size

; Form resource

		res 'tFRM', kidForm1, "tfrm03e8.bin"
		res 'tFRM', kidForm2, "tfrm03e9.bin"
		res 'tFRM', kidForm3, "tfrm03ea.bin"
		res 'tFRM', kidForm4, "tfrm03eb.bin"
		res 'tFRM', kidForm5, "tfrm03ec.bin"
		res 'tFRM', kidForm6, "tfrm03ed.bin"
		res 'tFRM', kidForm7, "tfrm03ee.bin"
		res 'tFRM', kidForm8, "tfrm03ef.bin"
		res 'tFRM', kidForm9, "tfrm03f0.bin"
		res 'tFRM', kidForm10, "tfrm03f1.bin"
		res 'tFRM', kidForm11, "tfrm03f2.bin"
		res 'tFRM', kidForm12, "tfrm03f3.bin"
		

; Version resource

		res 'tver', kidrTVER, "tver0001.bin"

; Menu resource

        res 'MBAR', kidMenu1, "mbar03e8.bin"

; Alert resources

        res 'Talt', kidAlert1, "talt03e8.bin"


; String resources

		res 'tSTR', kidHelp1, "tSTR03e8.bin"
		res 'tSTR', kidHelp2, "tSTR03e8.bin"

; Bitmap resources
		res	'Tbmp',	kidBitmap, "Tbmp03e8.bin"
		res	'Tbmp',	kidBitmap2, "Tbmp03e9.bin"
		res	'Tbmp',	kidBitmap3, "Tbmp03ea.bin"
		res	'Tbmp',	kidBitmap4, "Tbmp03eb.bin"
		res	'Tbmp',	kidBitmap5, "Tbmp03ec.bin"
