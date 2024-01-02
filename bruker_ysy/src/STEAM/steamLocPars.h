/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/







/* 
 * parameter used as delays, 
 * pulse duration and gradient amplitudes in pulse program 
 */


/* delays */
double parameter St_D0;
double parameter St_D1;
double parameter St_D2;
double parameter St_D3;
double parameter St_D4;
double parameter St_D5;
double parameter St_D6;
double parameter St_D7;
double parameter St_D8;
double parameter St_D9;



/* pulse durations */

double parameter St_P0;
double parameter St_P1;
double parameter St_P2;

/* gradient amplitudes */

double parameter St_Gr1;
double parameter St_Gr2;
double parameter St_Gr3;
double parameter St_Gr4;
double parameter St_Gr5;
                                         
                                         
double parameter St_Gp1;
double parameter St_Gp2;
double parameter St_Gp3;
double parameter St_Gp4;
double parameter St_Gp5;
                                         
double parameter St_Gs1;
double parameter St_Gs2;
double parameter St_Gs3;
double parameter St_Gs4;
double parameter St_Gs5;

units SPOIL_TYPE
{
  dur "ms";
  grad_dir1 "%";
  grad_dir2 "%";
  grad_dir3 "%";
  spoil_dir1 "{cycles}/mm";
  spoil_dir2 "{cycles}/mm";
  spoil_dir3 "{cycles}/mm";
};

format SPOIL_TYPE
{
  dur "%.3f";
  grad_dir1 "%f";
  grad_dir2 "%f";
  grad_dir3 "%f";
  spoil_dir1 "%f";
  spoil_dir2 "%f";
  spoil_dir3 "%f";
};

display_name SPOIL_TYPE
{
  dur "Spoiler Duration";
  grad_dir1 "Gradient Dir 1";
  grad_dir2 "Gradient Dir 2";
  grad_dir3 "Gradient Dir 3";
  spoil_dir1 "Spoiling Dir 1";
  spoil_dir2 "Spoiling Dir 2";
  spoil_dir3 "Spoiling Dir 3";
};


SPOIL_TYPE parameter 
{
  display_name "1st TE/2 Spoiler";
  relations StSpTE1Rel;
}StSpTE1;

SPOIL_TYPE parameter 
{
  display_name "2nd TE/2 Spoiler";
  relations StSpTE2Rel;
}StSpTE2;

SPOIL_TYPE parameter 
{
  display_name "TM Spoiler";
  relations StSpTMRel;
}StSpTM;

int parameter
{
  display_name "Gradient Scheme";
  relations StGschemeRel;
}StGscheme;

int parameter
{
  relations StHandlerRel;
}StHandler;


double parameter
{
  display_name "Mixing Time";
  format "%.3f";
  units "ms";
  relations StTMRel;
}StTM;


double parameter
{
  display_name "STEAM Module Duration";
  relations StDurRel;
  format "%f";
  units "ms";
}StDur;


double parameter 
{
  display_name "Gradient Stabilization";
  short_description "Delay between gradient ramps and RF pulses.";
  units "ms";
  format "%.2f";
  relations StGStabDRel;
}StGStabD;

