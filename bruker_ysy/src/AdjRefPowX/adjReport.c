/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2020
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"
#include <libCore/Math/Math.hh>
#include "lib/PvSysinfoClient.h"
#include "adjReport.h"


#include "bruktyp.h"
#include "debugdef.h"
#include "mr/acqtyp.h"
#include "PvUtil/PvUtil.h"
#include "Parx/Parx.h"

#include "common/ovl_toolbox/VisuTools.h"
#include "Visu/Mr/VisuTypes.h"

#ifndef CPROTO
#include <string>
#include <sstream>

static std::string findRelevantDigits(double);
static void check_power_limits(void);
static void draw_search_box(int, int, double, double, double, double, double);
static void draw_graph(int, int, const double*, const double*);
static void draw_ref_line(int, int, double, double, double);
static void draw_zero_line(int, int , double*);

void writeReportFile(void)
{

  double *vlst = ScanMode == CONST_PULSE_POW ? ExcDurList  : ExcVoltList;
  double *flst = ScanMode == CONST_PULSE_POW ? ExcVoltList  : ExcDurList;

  int    max_idx  = PkMaxIdx;
  double max_frac = PkMaxIdxFrac;

  // collect data in file report.dat in expno-dir
  // called in RecoMethodFilter
  std::ofstream report;
  std::ostringstream message;

  char report_file[PATH_MAX], dpath[PATH_MAX] ;
  PvOvlUtilGetExpnoPath(dpath, PATH_MAX, 0);
  sprintf(report_file,"%s/%s",dpath,"adjReport.dat");

  report.open(report_file);
  report.setf(std::ios::left);

  int    lwdth   = 14;  //table line width in report_file

  if ( NExcSteps == 1 )
    return; //seems to be no adj series

  double  refpow = 2e-8 * pow( (vlst[max_idx] + max_frac) * flst[0] * ExcPulse.Sint, 2 );

  if (ScanMode == CONST_PULSE_POW)
  {
    message << "# Max found at pulse dur: " << vlst[max_idx]+max_frac
            << " us, Pulse voltage: " << flst[0] << " V" << std::endl;
  }
  else
  {
    message << "# Max found at pulse voltage: " << vlst[max_idx]+max_frac
            << " V, pulse dur: " << flst[0] << " us" << std::endl;
  }

  message << "# corresponding to a reference power of " << refpow << " W"
          << std::endl ;

  std::string msg = message.str();
  std::cout << msg << std::endl;

  if (ScanMode == CONST_PULSE_POW)
  {
    message << "#" << std::endl << "# Exp" << std::setw(lwdth) << "Pulse Dur"
            << std::setw(lwdth) << "Area" << std::endl;
    message << "#" << std::setw(lwdth) << "  [us]" << std::endl;
    message << std::setfill('-');
    message << '#' << std::endl;
    message << std::setfill(' ');

    for (int i=0; i<NExcSteps; i++)
    {
      message << "  " << std::setw(1) << i + 1
              << std::setw(lwdth  ) << vlst[i]
              << std::setw(lwdth + 3) << PkArea[i]
              << std::endl;
    }
  }
  else
  {
    message << "#" << std::endl << "# Exp" << std::setw(lwdth) << "Voltage"
            << std::setw(lwdth) << "Power" << std::setw(lwdth) << "Area"
            << std::endl;
    message << "#" << std::setw(lwdth) << "  [V]" << std::setw(lwdth) << "  [W]" << std::endl;
    message << std::setfill('-');
    message << '#' << std::endl;
    message << std::setfill(' ');

    for (int i=0; i<NExcSteps; i++)
    {
      message << "  " << std::setw(1) << i + 1
              << std::setw(lwdth  ) << vlst[i]
              << std::setw(lwdth  ) << ExcPowList[i]
              << std::setw(lwdth + 3) << PkArea[i]
              << std::endl;
    }
  }

  if (report.is_open())
  {
    msg = message.str();
    report << msg ;
    report.close();
  }
  else
  {
    //to do: throw appropriate message
    std::cout << "Report file is not open!!" << std::endl;
  }
}


void HandleReports(void)
{
  DB_MSG(("-->HandleReports"));

  bool reportHandled = false;

  if (ParxRelsParHasValue("PVM_ReportHandler"))
  {
    std::string reportTemplate(PVM_ReportHandler);

    if (!reportTemplate.find("AdjRefPowX.xml"))
    {
      HandleAdjRefPowXPDFReport();
      reportHandled = true;
    }
  }

  if(!reportHandled)
    ParxRelsParRelations("PVM_ReportHandler",MODALITY_HIERARCHY);

  DB_MSG(("<--HandleReports"));

   return;
}


void HandleAdjRefPowXPDFReport(void)
/* -------------------------------------------------------------------
   ------------------------------------------------------------------*/
{
  DB_MSG(("-->HandleAdjRefPowXPDFReport"));

  restoreAdjResults();
  STB_UpdateNuclei(Yes);

  double*    area  = PkArea;
  int       nsteps = NExcSteps;
  int      max_idx = PkMaxIdx;
  int     max_frac = PkMaxIdxFrac;

  double refpow      = 1.0; //reference power
  double refpowdelta = 0.0; //uncertainty of reference power
  double res         = 0.0; //sampling resolution

  double min_area = *std::min_element( area, area + nsteps );
  double max_area = *std::max_element( area, area + nsteps );

  double lr = ScanMode == CONST_PULSE_POW ? SearchDurRange[0] : SearchVoltRange[0];
  double ur = ScanMode == CONST_PULSE_POW ? SearchDurRange[1] : SearchVoltRange[1];

  //running parameter (varialbe list) - dur or pwr dpndng on scanmode
   double* vlst  = ScanMode == CONST_PULSE_POW ? ExcDurList  : ExcVoltList
;
  //constant parameter (fixed list) - pwr or dur dpndng on scanmode
  double* flst  = ScanMode == CONST_PULSE_POW ? ExcVoltList  : ExcDurList;

  double result = ScanMode == CONST_PULSE_POW ? PVM_AdjRefPowXDur
                                              : PVM_AdjRefPowXPwr.pampl;

  int  nlines   = 1; // min nbr of graphs in plot
  YesNo sbox    = No; int sbox_idx=0;    //search box
  YesNo zline   = No; int zline_idx=0;   //zero line
  YesNo refline = No; int refline_idx=0; //reference line

  if (nsteps == 1)
    return;

  //send warnings to report
  check_adc_overflow();

  check_power_limits();

  //if search range modified draw box in plot to identify
  if ( lr != vlst[0] || ur != vlst[nsteps-1] )
  {
    //at least 4 pts to draw box
    if (nsteps > 4)
    {
      sbox = Yes;
      nlines += 1;
      sbox_idx = nlines-1;
    }
  }

  //if needed plot a horizontal line at zero:
  if ( min_area * max_area < 0 )
  {
    zline = Yes;
    nlines += 1;
    zline_idx = nlines-1;
  }
  
  //no result if not yet started from adjustment platform
  if (result != 0)
  {
    refline = Yes;
    nlines +=1;
    refline_idx = nlines-1;
  }

  PARX_change_dims("AdjReport_Plot_X_Vals", nlines , nsteps);
  PARX_change_dims("AdjReport_Plot_Y_Vals", nlines , nsteps);
  PARX_change_dims("AdjReport_RenderFlags", nlines);

  AdjReport_RenderFlags[0] = Yes;   // graph w dots & lines
  for (int i=1; i<nlines; i++)
    AdjReport_RenderFlags[i] = No;  // others only lines

  // results for table 1 in report)
  res         = fabs( vlst[1]-vlst[0] );    // either in us or in V
  refpow      = 2e-8 * pow( (vlst[max_idx] + max_frac) * flst[0] * ExcPulse.Sint, 2 );

  // estimate of uncertainty via error propagation: relative uncertainty: Delta_P/P
  refpowdelta = 200 * res / ( vlst[max_idx] + max_frac); //in per cent

  // find decimal digit for report file
  strcpy( NumRes2, findRelevantDigits( refpow*refpowdelta/10).c_str() );

  // for figure in report file
  draw_graph( 0, nsteps, vlst, area );

  if (sbox)
  {
    double margin = 0.25*(vlst[1] - vlst[0]);
    draw_search_box( sbox_idx, nsteps, margin, lr, ur, min_area, max_area );
  }

  if (zline)
    draw_zero_line( zline_idx, nsteps, vlst );

  if (refline)
    draw_ref_line( refline_idx, nsteps, result, min_area, max_area );

  //control number of digits in pdf-report
  if (nsteps>1)
    strcpy( NumRes, findRelevantDigits( vlst[1]-vlst[0] ).c_str() );

  char refstat[64]="set by AdjRefPowX";
  ParxRelsParSetValue("StudyRefPow.RefPow", &refpow     );
  ParxRelsParSetValue("StudyRefPow.Stat"  , refstat     );
  ParxRelsParSetValue("SamplingResolution", &res        );
  ParxRelsParSetValue("RefPowDelta"       , &refpowdelta);
  ParxRelsParSetYesNoValue("SearchBox"    , sbox        );

  AdjReport_NbrElemTxCoil = PARX_get_dim("VisuCoilTransmitMultiName", 1);
  AdjReport_NbrElemRxCoil = PARX_get_dim("VisuCoilReceiveMultiName" , 1);


  PARX_change_dims("RefPowTable", PVM_NumberOfNuclei);

  for (int i=0; i<PVM_NumberOfNuclei; i++)
  {
    std::string num       =  std::to_string(i+1);
    std::string nuc_name  = "PVM_Nucleus"    + num;
    std::string chan_name = "PVM_RefPowCh"   + num;
    std::string stat_name = "PVM_RefPowStat" + num;

    char nuc[8];
    char stat[256];
    double val;

    ParxRelsParGetValue(  nuc_name.c_str(), &nuc );
    ParxRelsParGetValue( stat_name.c_str(), &stat);
    ParxRelsParGetValue( chan_name.c_str(), &val);

    strcpy( RefPowTable[i].nuc,  nuc  );
    strcpy( RefPowTable[i].stat, stat );
    RefPowTable[i].val = val;

    strcpy( NumRes1, findRelevantDigits(val).c_str() );
  }

  DB_MSG(("<--HandleAdjRefPowXPDFReport"));

  return;
}


static void check_power_limits(void)
{
  char errbuff[1024]; int sysstate;

    sysstate = SysInfo_TxMaxAllowedCWPower( 1, &AdjReport_TxMaxAllowedCWPower );

    if(sysstate)
    {
      snprintf(errbuff,1023,
               "SysInfo_TxMaxAllowedCWPower: Error retrieving max CW power\n"
               "Reason: %s", SysInfo_getErrorMessage(sysstate));

      UT_ReportError(errbuff);
    }

    AdjReport_TxMaxAllowedPeakPower = SysInfo_TxMaxAllowedPeakPower(1);

    sysstate = SysInfo_TxCoilMaxPowPulselength( 1, &AdjReport_TxMaxAllowedPeakDur );

    if(sysstate)
    {
      snprintf(errbuff,1023,
               "SysInfo_TxCoilMaxPowPulselength: Error retrieving dur for max power"
               "\nReason: %s", SysInfo_getErrorMessage(sysstate));
      std::cout << "UT_ReportError(errbuff) " << errbuff << std::endl;
      UT_ReportError(errbuff);
    }
}


static void draw_graph(int nbr, int nsteps, const double* x, const double* y  )
{
  for (int i=0; i<nsteps; i++)
  {
    AdjReport_Plot_X_Vals[nbr][i] = *(x+i);
    AdjReport_Plot_Y_Vals[nbr][i] = *(y+i);
  }
}


static void draw_search_box(int   nbr, int nsteps, double margin,
                            double x1, double x2,
                            double y1, double y2 )
/* -------------------------------------------------------------------
   for modified search range draw box into plot to identify it
   (x1,y1): lower left corner, (x2,y1): lower right
   (x1,y2); upper left       , (x2,y2): upper right
   use margins to avoid covering a refline
   ------------------------------------------------------------------*/
{
  x1 -= margin; x2 +=margin;

  AdjReport_Plot_X_Vals[nbr][0] = x1;
  AdjReport_Plot_Y_Vals[nbr][0] = y1;

  AdjReport_Plot_X_Vals[nbr][1] = x1 ;
  AdjReport_Plot_Y_Vals[nbr][1] = y2;

  for (int k=2; k<nsteps-2; k++)
  {
    AdjReport_Plot_X_Vals[nbr][k] = x2;
    AdjReport_Plot_Y_Vals[nbr][k] = y2;
  }

  AdjReport_Plot_X_Vals[nbr][nsteps-2] = x2;
  AdjReport_Plot_Y_Vals[nbr][nsteps-2] = y1;

  AdjReport_Plot_X_Vals[nbr][nsteps-1] = x1;
  AdjReport_Plot_Y_Vals[nbr][nsteps-1] = y1;
}


static void draw_zero_line(int nbr, int nsteps, double* x)
{
  for (int i=0; i<nsteps; i++)
  {
    AdjReport_Plot_X_Vals[nbr][i] = *(x+i);
    AdjReport_Plot_Y_Vals[nbr][i] = 0.0;
  }
}


static void draw_ref_line(int nbr, int nsteps, double x, double lb, double ub)
{
  for (int i=0; i<nsteps-1; i++)
  {
    AdjReport_Plot_X_Vals[nbr][i] = x;
    AdjReport_Plot_Y_Vals[nbr][i] = lb;
  }
  //one pt at upper border to get a line
  AdjReport_Plot_X_Vals[nbr][nsteps-1] =  x;
  AdjReport_Plot_Y_Vals[nbr][nsteps-1] =  ub;
}


std::string findRelevantDigits(double val)
/* -------------------------------------------------------------------
   A way to get number of post decimal digits to print array table in
   a pdf report. E.g. for ExcDurList, ExcPowList - both are equally
   spaced . Here val is difference of two neighboring elements.
   Output: string of "0.00..." as needed in report xml-file
   ------------------------------------------------------------------*/
{
  double buf = val;
  std::string str = "0.0";

  if ( fabs(val) > 1e-8 )
    do
    {
      buf = 10*buf;
      str.append("0") ;
    } while ( (int)buf==0 );

  return str;
}

#endif // CPROTO

/****************************************************************/
/*		E N D   O F   F I L E			                               	*/
/****************************************************************/
