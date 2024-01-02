;************************************************************************* 
; $Source$
;
; Copyright (c) 2007
; Bruker BioSpin MRI GmbH
; D-76275 Ettlingen, Germany
;
; $Id$ 
;



define list<frequency> freqInv={$ACQ_O1_list}

#define inv_r		d4 grad_ramp{Isis_Gr1,0,0}		\n\
			10u		freqInv:f1		\n\
			(p1:sp1		ph0):f1			\n\
			d11 grad_ramp{Isis_Gr2,0,0} freqInv.inc

#define noinv_r		d4 grad_ramp{Isis_Gr1,0,0}		\n\
			10u		freqInv:f1		\n\
			d10					\n\
			d11 grad_ramp{Isis_Gr2,0,0} freqInv.inc

#define inv_p		d4 grad_ramp{0,Isis_Gp1,0}		\n\
			10u		freqInv:f1		\n\
			(p1:sp1		ph0):f1			\n\
			d11 grad_ramp{0,Isis_Gp2,0} freqInv.inc

#define noinv_p		d4 grad_ramp{0,Isis_Gp1,0}		\n\
			10u		freqInv:f1		\n\
			d10					\n\
			d11 grad_ramp{0,Isis_Gp2,0} freqInv.inc

#define inv_s		d4 grad_ramp{0,0,Isis_Gs1}		\n\
			10u		freqInv:f1		\n\
			(p1:sp1		ph0):f1			\n\
			d11 grad_ramp{0,0,Isis_Gs2} freqInv.inc

#define noinv_s		d4 grad_ramp{0,0,Isis_Gs1}		\n\
			10u		freqInv:f1		\n\
			d10					\n\
			d11 grad_ramp{0,0,Isis_Gs2} freqInv.inc

			


subroutine ADDOVS(phase ovsph)
{
  if (IsisNumAddOvsCycles > 0) 
  {
addovs,	1u
    subr Ovs(ovsph)
    lo to addovs times IsisNumAddOvsCycles
  }
}


subroutine inversions(any cyc)
{

if "cyc==0"
{
	noinv_r
	noinv_p
	noinv_s
}

if "cyc==1"
{	
	inv_r
	noinv_p
	inv_s
}
if "cyc==2"
{
	noinv_r
	inv_p
	noinv_s
}
if "cyc==3"
{
	noinv_r
	inv_p
	inv_s
}
if "cyc==4"
{
	inv_r
	noinv_p
	noinv_s
}
if "cyc==5"
{
	noinv_r
	noinv_p
	inv_s
}
if "cyc==6"
{
	inv_r
	inv_p
	noinv_s
}
if "cyc==7"
{
	inv_r
	inv_p
	inv_s
}
d4 grad_off

}
