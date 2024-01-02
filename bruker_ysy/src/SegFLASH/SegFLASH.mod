;****************************************************************
;
; Copyright (c) 2002-2003
; Bruker BioSpin MRI GmbH
; D-76275 Ettlingen, Germany
;
; All Rights Reserved
;
; SegFLASH.mod : Subroutine Module 
;
;****************************************************************


;==========================================================
;   subroutines for dummy handling
;==========================================================
  define loopcounter nDummies     = {$PVM_DummyScans}
  define loopcounter NumADCOff
  define loopcounter NumADCOffDone


subroutine InitDummies()
{
    "NumADCOffDone    = 0"

    if (SegmentSliceOrder == Serial)
    {
	"NumADCOff = nDummies * l1 * l2"
	"l3d  = l3 + nDummies"
    }
    if (SegmentSliceOrder == Alternate)
    {
	"NumADCOff = nDummies * l1 * l2 * l3"
	"l4d  = l4 + nDummies"
    }
}

subroutine NewDummies()
{
    if (SegmentSliceOrder == Alternate)   
    {
        "l4d  = l4 + nDummies"
        "NumADCOffDone = 0"
    }
    if (SegmentSliceOrder == Serial)   
    {
      if "l4 > 1"
      {
        "l3d  = l3 + nDummies"
        "NumADCOffDone = 0"
      }
    }
}

subroutine adcstart(phase receiverphase, phase referencephase)
{
  if "NumADCOffDone < NumADCOff"
  {
      de
      AQ_(job0)
  } 
  else
  {	
                ADC_INIT_(job0, receiverphase, referencephase)
      AQ_(job0)	ADC_START_(job0)
  }
}

subroutine adcend(delay delayadcend)
{
    if "NumADCOff <= NumADCOffDone"
    {
	delayadcend ADC_END_(job0)
    }
    else
    { 
	delayadcend
        0u          r2d.dec
        0u          r3d.dec
	"NumADCOffDone = NumADCOffDone + 1"
    }
}


subroutine myDriftComp()
{
  if "NumADCOffDone < NumADCOff"
  {
     ;dummy scans: no acqisition! d14=PVM_DriftCompModuleTime/1000
     d14  
  } 
  else
  {	
                subr DriftComp()
  }
}

;==========================================================
;   subroutine for flow compensation
;==========================================================
subroutine FlowComp()
{
    if (FlowCompLongTEScan == No)
    {
        d10       grad_shape{RShape()*100, P2Shape()*100*r2d, SShape()*100 - P3Shape()*100*r3d}
    }
    else
    {
        d10       grad_shape{0, 0 , SShape()*100 }
         d2
        d10       grad_shape{RShape()*100, P2Shape()*100*r2d, -P3Shape()*100*r3d}
    }
	; very small delay to compensate gridding EncGradDur on grad raster time
         d7
}

;==========================================================
;   subroutine for selective/non-selective inversion
;==========================================================
if(PVM_SelIrOnOff == On)
{
  define list<frequency> myIrFL = {$PVM_SelIrFL} 
  define pulse myIrP0 = {$PVM_SelIrP0}
  define delay myIrD0 = {$PVM_SelIrD0}
  define delay myIrD1 = {$PVM_SelIrD1}
  define delay myIrD2 = {$PVM_SelIrD2}
  define delay myIrD4 = {$PVM_SelIrD4}
}

subroutine Inversion()
{
  if(PVM_SelIrOnOff == On)
  {
    
       10u      myIrFL:f1
    myIrD4     	grad_ramp{0, 0, PVM_SelIrGradAmp*100}
 
   (myIrP0      :sp42  ph0):f1

    myIrD2     	grad_ramp{0, 0, PVM_SelIrSpoilAmp}
    myIrD4	grad_off
    myIrD1	
  }
}

subroutine IncrInvFreqList()
{
  if(PVM_SelIrOnOff == On)
  {
        0u     	myIrFL.inc
  }
}

;==========================================================
;   subroutine for 3D loop
;==========================================================
subroutine loop3D()
{
lo to start times l5d
}

;==========================================================
;   subroutine for modified trigger handling
;==========================================================
if (TriggerOnOff == On)
{
  define delay trigDur    = {$TriggerDur}
  define delay acqtrigDel = {$AcqTriggerDelay}
  define delay invtrigDel = {$InvTriggerDelay}

  ; convert from ms to s
  "trigDur = trigDur / 1000"
  "acqtrigDel = acqtrigDel / 1000"
  "invtrigDel = invtrigDel / 1000"
}

subroutine TriggerAcq()
{
  if (AcqTrigger == Yes)
  {
    trigDur     trignl1
    acqtrigDel
  }
}

subroutine TriggerInv()
{
  if (InvTrigger == Yes)
  {
    trigDur     trignl1
    invtrigDel
  }
}
