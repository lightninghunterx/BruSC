;************************************************************************* 
; Copyright (c) 2006
; Bruker BioSpin MRI GmbH
; D-76275 Ettlingen, Germany
;
; $Id$
;
; All Rights Reserved
; 
; includes acqdec.mod (decoupling module)
;
; Declaration of pulseprogram parameters and definition of subroutine
; SteamSpecLoc

#include <acqdec.mod>

define delay StD0 = {$St_D0}
define delay StD1 = {$St_D1}
define delay StD2 = {$St_D2}
define delay StD3 = {$St_D3}
define delay StD4 = {$St_D4}
define delay StD5 = {$St_D5}
define delay StD6 = {$St_D6}
define delay StD7 = {$St_D7}
define delay StD7m20
"StD7m20=StD7-20u"          ;compensates for 20u delay (including de) in acqdec
define delay StD8 = {$St_D8}
define delay StD9 = {$St_D9}
define delay de20
"de20=20u-de"


define pulse StP0 = {$St_P0}
define pulse StP1 = {$St_P1}
define pulse StP2 = {$St_P2}

define list<frequency> freqTx={$ACQ_O1_list}

;D/P------------GRAD-----------------------------------------------RF/ACQ------------

subroutine SteamSpecLoc(any psh1,phase phpul1, phase phpul2, phase phpul3, phase phrec, phase phref)
{
  StD0            grad_ramp{St_Gr1, 0, 0}                        freqTx:f1
  StD1								 freqTx.inc
  (StP0:psh1 phpul1):f1
  StD2            grad_ramp{St_Gr2, St_Gp2, St_Gs2}              freqTx:f1
  if(StGscheme ==  1)  ;long TE -> switch grad off
  {
    StD9 grad_off
    StD3            grad_ramp{0, St_Gp1, 0}                      
  }
  if(StGscheme == 0) ;short TE -> switch to next grad immediately
  {
    StD3            grad_ramp{0, St_Gp1, 0}                      
  }
  (StP1:sp2 phpul2):f1
  StD4            grad_ramp{St_Gr3, St_Gp3, St_Gs3}              freqTx.inc
  StD5            grad_ramp{0, 0, St_Gs1}			 freqTx:f1
  (StP2:sp3 phpul3):f1
  StD6            grad_ramp{St_Gr4, St_Gp4, St_Gs4}		 freqTx.inc
  StD7m20         grad_off
  if(OPT_ManAdjustment==NoManAdjustment)
  { 
     subr acqdec(phrec,phref,AQ_(job0))	;lasts aqq+20u
  }
  else ; lasts aqq+20u+D0+D0
  {
     de20
		  ADC_INIT_(job0, phrec,phref)
     StD0         grad_ramp{St_Gr5, St_Gp5, St_Gs5}    ; switch a read gradient
     AQ_(job0)    ADC_START_(job0)
     StD0         grad_off
  }

}








