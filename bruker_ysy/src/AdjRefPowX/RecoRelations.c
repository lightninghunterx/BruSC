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
 *
 * $Id$
 *

 ****************************************************************/

static const char resid[] = "$Id: RecoRelations.c,v 1.1.2.2 2001/01/11 CMEI ";

#define DEBUG   0
#define DB_MODULE 0
#define DB_LINE_NR  0

#include "method.h"
#include "adjReport.h"

static void lin_reg (const double *, const double *, double *,
              int, double *, double *);

#ifndef CPROTO
static std::array<int, 2> hz2idx(double *hz);
static std::ofstream  filehandle(std::string);
#endif

#include "lib/PvSysinfoClient.h"
#ifndef CPROTO
#include <string>
#include <vector>
#endif

void SetRecoParam( void )
{
  DB_MSG(("-->SetRecoParam\n"));

  int ni = NI + NoiseScan;
  /* set baselevel reconstruction parameter */
  /* default initialization of reco based on acqp pars allready set */
  ATB_InitUserModeReco(
      1,
      0,
      PVM_SpecMatrix, // acq size
      PVM_SpecMatrix, // image size
      NULL, //effAntiAlias=1,
      NULL, //effPftOverscans=halfsize,
      ni,
      NULL,
      1,
      NULL,  //linear order,
      NULL,  //linear order (not used)
      NULL,  //linear order (not used)
      PVM_EncNReceivers,
      PVM_EncChanScaling,
      0,
      1);

  /* complex channel combination: */
  RecoCombineMode = AddImages;

  /* configure information available during setup mode */
  ACQ_SetupShowDigFilling = Yes;
  ParxRelsParRelations("ACQ_SetupShowDigFilling",Yes);
  ACQ_SetupShuffleProfiles=No; // combine spectra in GSP mode


  ATB_ArrayPhaseSetRecoPhase();

  /* set RECO_image_type*/
  if (RecoMode == CMPLX_RECO_MODE)
    RECO_image_type = COMPLEX_IMAGE;
  else
    RECO_image_type = MAGNITUDE_IMAGE;

  if (ACQ_scan_type != Setup_Experiment)
  {
    //    RecoRepetitionGroupSize=0; //to get all NRs simultaneously
    RecoPreppassProcDim=3; //to get all NIs simultaneously
    if (NR == 1)
      RecoPreppassProcDim--;
  }


  DB_MSG(("<--SetRecoParam\n"));
}


void DeriveReco(void)
{
  std::stringstream filterOptions;

  char avOptions[200], fidFile[PATH_MAX];

  int nrReceivers  = RecoNrActiveReceivers();

  DB_MSG(("-->DeriveReco, nRx= %i",nrReceivers));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update"));
    return;
  }

  /* standard settings for reconstruction */
  if(RecoPrototype == No)
    SetRecoParam();

  /*no automatic FT during first pass but after topspin file generation*/
  if(ACQ_scan_type != Setup_Experiment)
    RecoFTOrder[0]=-1;
  else
    RecoFTOrder[0]=0; //FT during GSP

  ParxRelsParRelations("RecoUserUpdate", Yes);

  //Filter for RefScan and ArrayPhase Adjustment
  ATB_GetRecoDataPoints(PVM_RecoDataPointsRequired,  (NI+NoiseScan)*PVM_EncNReceivers*RECO_inp_size[0]*2, "Q");

  /*Averaging*/
  if (ACQ_ScanPipeJobSettings[0].accumMode==ACCUM_none ||
      ACQ_ScanPipeJobSettings[0].pipelineMode==PIPELINE_raw)
  {
    sprintf(avOptions,"avList=<AverageList>; avListSize=1;"
            " nObj=%d; newSize=<RECO_inp_size>;", /* NI */ 1);
    for (int i = 0; i < nrReceivers; i++)
      RecoComputeAppendStage(RECOFIRSTPASS, i, "CAST", "RecoAverageFilter", "AVE", avOptions);
  }


  if(ACQ_scan_type != Setup_Experiment)
  {
    const char *hook = nrReceivers>1? "SOS":"Q";

     /* insert FT along time after channel combination */
    RecoComputeAppendStage(RECOPREPPASS, 0, hook, "RecoFTShiftFilter", "FTS",
                           "shift=0.5; winDirection=0; exponent=1");

    RecoComputeAppendStage(RECOPREPPASS, 0, "FTS", "RecoFTFilter", "FT",
                           "direction=0; exponent=1");

    /* insert branching for export-to-topspin-file with accumulated (and corrected) data */
    PvOvlUtilGetProcnoPath(fidFile, PATH_MAX, "fid_proc.64");
    filterOptions.str("");
    filterOptions << "filename=\"" << fidFile << "\";shuffle=true";

    RecoComputeAppendStage  (RECOPREPPASS, 0, hook, "RecoTeeFilter", "T","");
    RecoComputeAddStage     (RECOPREPPASS, 0, "RecoFileSink"  , "FSINK", filterOptions.str().c_str());
    RecoComputeConnectStages( RECOPREPPASS, 0, "T", "FSINK");

    //divide by number of channels to avoid integer overflow after channel summation
    filterOptions.str(""); filterOptions << "divisor=" << nrReceivers;
    RecoComputeInsertStage(RECOPREPPASS,0,"FSINK","RecoDivideFilter","DIV",filterOptions.str().c_str());

    RecoComputeAppendStage(RECOPREPPASS, 0, "FT", "RecoMethodFilter", "RefPowX",
                           "cntParameter=<RecoCounter>;bufParameter=<RecoBuffer>;");
  } // end no setup experiment
  else  //setup experiment
  {
    InsertDataEvalFilters();

    if ( !strcmp( ACQ_SetupAutoName,"RgAutoCounter") )
        InsertRgFilter();
  }

  DB_MSG(("<--DeriveReco\n"));
}


/* following functions allow display and calculation of linewidth.
   in PRESS they are also used for LocShim and LocSF */

void SetdisplayInfonames(void)
{
  std::string recoinfopars="NormFtArea/Normalized Area";

  PARX_change_dims("ACQ_RecoInfoNames",recoinfopars.size()+1);
  ParxRelsParSetValue("ACQ_RecoInfoNames",recoinfopars.c_str());
  std::string signalinfopars="NormArea/Normalized Area";
  size_t slen=signalinfopars.size()+1;

  int njobs=0,activejobs=0;
  njobs = (int) PARX_get_dim("ACQ_jobs",1);

  for(int i=0;i<njobs;i++)
  {
    if(ACQ_jobs[i].chanNum != 0)
      activejobs++;
  }

  if(activejobs != (int)PARX_get_dim("ACQ_SignalInfoNames",2) ||
     slen > PARX_get_dim("ACQ_SignalInfoNames",2))
  {
    PARX_change_dims("ACQ_SignalInfoNames",activejobs,slen+1);
  }
  ParxRelsParSetValue("ACQ_SignalInfoNames[0]",signalinfopars.c_str());
}


void InsertRgFilter(void)
/* -------------------------------------------------------------------
   This filter is to provide data to RgAutoCounterRel, needed to
   possibly accelerate rg adjmnt
   ------------------------------------------------------------------*/
{
  DB_MSG(("-->InsertRgFilter"));

  int ni = NI+NoiseScan;

  PARX_change_dims("NormFtArea",  ni, 1);

  RecoComputeAppendStage(RECOPREPPASS, 0, "G",
                         "RecoNormalizedAreaFilter",
                         "SPECAREA",
                         "normalizedArea=<NormFtArea>;");

  DB_MSG(("<--InsertRgFilter"));
}


void InsertDataEvalFilters(void)
{
  int nchan = RecoNrActiveReceivers();
  int ni    = NI+NoiseScan;

  PARX_change_dims("NormArea",    ni, nchan);

  SetdisplayInfonames();

  // append filter to calculate normalized area
  RecoComputeAppendStage(RECOFIRSTPASS,
                         0,
                         "Q",
                         "RecoNormalizedAreaFilter",
                         "NAREA",
                         "normalizedArea=<NormArea>;");
}


void RecoBufferRelation(void)
/* -------------------------------------------------------------------
   1. check if received and expected data sizes match
   2. find data point indices for integration range (PkRange is in Hz)
   3. sort incoming double data into real & imag arrays, sum up data
      magnitudes over pkrange, find max area in magnitude data
   4. phase correct for cmplx reco mode: get phases for all data pts of
      the signal which has max magnitude, unwrap & linearly regress
   5. apply lin reg coefficient to other signals and get 'real' areas
   6. analyse resulting data
   ------------------------------------------------------------------*/
{
  DB_MSG(("-->RecoBufferRelation: RecoCounter %i: ", RecoCounter));

  std::ofstream file;
  if (AdjRefPowXLog != none) file=filehandle("open");

  RecoGlobalMemory M(RecoBuffer);
  double *data = (double*) M.getAddress();

  //RecoCounter: size of the incoming data pack
  int size = RecoCounter/(sizeof(double));
  int csz  = PVM_SpecMatrix[0];            // complex size
  int ni   = NI + NoiseScan;
  int i=0, k=0, l=0;

  double     im[csz][ni] = {0.0};
  double     re[csz][ni] = {0.0};
  double   area[ni]      = {0.0};
  double   max_area      = 0.0;
  int      max_idx       = 0;              // idx of max_area

  // 1
  if( size != 2*csz*ni)
    throw PvStdException("RecoBufferRelation: data size not correct:\n"
                         "should: %d from PVM_SpecMatrix[0]*NI, is: %d",
                         2*csz*ni, size);

  // 2
  std::array<int,2> span = hz2idx(PkRange);

  // 3
  for (k=0; k<ni; k++)
  {
    for (i=0; i<csz; i++)
    {
      re[i][k] = *(data+l);
      im[i][k] = *(data+l+1);

      // get 'abs' area
      if( i >= span[0] && i <= span[1] )
      {
        area[k] += sqrt(  re[i][k] * re[i][k]
                        + im[i][k] * im[i][k] );

        if (AdjRefPowXLog == spectra && RecoMode == MAGN_RECO_MODE)
          file << re[i][k] << " " << im[i][k] << std::endl;
      }

      l+=2;
    }

    if ( k>=1 && max_area < area[k] ) //k=0 is noisescan if ni>1
    {
      max_area = area[k];
      max_idx  = k;
    }
  }

  // 4
  if ( RecoMode == CMPLX_RECO_MODE )
  {
    // parameters for phase correction
    double ph[csz] = {0.0};                    // arr for phases
    double ph0, ph1;                           // lin reg result
    double offset = 0.0, ph_diff = 0.0, prev = 0.0;  // lin reg helper
    double arg    = 0.0;
    double lrx[csz] = {0.0};                   // x-axis  for lin_reg
    double lrw[csz] = {0.0};                   // weights for lin_reg

    double _re, _im, _z;                       // helper variables

    double noise = area[0]/csz;                // 1st scan's abs mean

    for(i=0; i<csz; i++)
    {
      _re = re[i][max_idx];
      _im = im[i][max_idx];

      // get phase of data set w max abs ampl
      ph[i]= atan2( _im, _re);

      if ( AdjRefPowXLog == phases_uncor )
        file << ph[i] << std::endl;

      // unwrap
      if (i == 0)
        prev = ph[0];

      if (i > 0)
      {
        ph_diff = ph[i] - prev;

        if (ph_diff > M_PI)
          offset -= 2 * M_PI;

        else if (ph_diff < -M_PI)
          offset += 2 * M_PI;

        prev   = ph[i];
        ph[i] += offset;

        if ( AdjRefPowXLog == phases_cor )
          file << ph[i] << std::endl;
      }

      //x-axis for lin regression
      lrx[i] = float(i);

      //weights for lin regression - the simplest one:
      _z = sqrt( _re*_re + _im*_im );

      if ( _z > 2*noise)
        lrw[i] = 1.0;
      else
        lrw[i] = 0.0;
    }

    // fit phase
    lin_reg( lrx, ph, lrw, csz, &ph0, &ph1);

    // re-init some pars
    max_area = 0.0;
    max_idx  = 0;
    l=0;

    // 5
    for (k=0; k<ni; k++)
    {
      area[k] = 0.0;

      for(i=0; i<csz; i++)
      {
        // phase correct
        arg = (-1) * ph1 * i - ph0;

        _re = re[i][k];
        _im = im[i][k];

        re[i][k] = (_re) * cos(arg) - (_im) * sin(arg);
        im[i][k] = (_re) * sin(arg) + (_im) * cos(arg);

        // get 'real' areas
        if( i >= span[0] && i <= span[1] )
        {
          area[k] += re[i][k];

          if ( AdjRefPowXLog == spectra )
            file << re[i][k] << " " << im[i][k] << std::endl;
        }

        // write  back to stream
        *(data+l  ) = re[i][k];
        *(data+l+1) = im[i][k];

        l+=2;
      }

      // search max in cmplx data (neglect 1st scan: noise)
      if ( k>=1 && ( fabs(max_area) < fabs(area[k])) )
      {
        max_area  = area[k];
        max_idx   = k;
      }
    }

    PhaseCorrVals[0]=ph0; PhaseCorrVals[1]=ph1;
  }

  //scale & write pars
  for ( k=1; k<ni; k++ ) //neglect 1st scan
    PkArea[k-1] = area[k] / area[max_idx];

  // 6.
  analyze_data();

  writeReportFile();

  if ( AdjRefPowXLog != none ) filehandle("close");

  DB_MSG(("<--RecoBufferRelation"));
}


void analyze_data()
/* -------------------------------------------------------------------
   1. search_90 searches the value of absolute maximum within range
   2. search_180 searches the first zero-crossing within range
      this new search_180 leads to zero-cross values not necessarily
      on the grid of indices as for search_90. Aiming not to rewrite
      everything variable PkMaxIdxFrac has been introduced
   ------------------------------------------------------------------ */
{
  DB_MSG(("-->analyze_data"));

  double* x = ScanMode == CONST_PULSE_POW ? ExcDurList : ExcVoltList;
  double* y            = PkArea;
  double  z            = 0.0; //value of zero-crossing for search_180
  int     n            = NExcSteps;

  double lval = ScanMode == CONST_PULSE_POW ? SearchDurRange[0]
                                            : SearchVoltRange[0];
  double uval = ScanMode == CONST_PULSE_POW ? SearchDurRange[1]
                                            : SearchVoltRange[1];

  int lidx = std::find( x, x + n, lval ) - x;
  int uidx = std::find( x, x + n, uval ) - x;

  // see that range order is ok
  if (lidx > uidx)
  {
    double buf = lidx;
    lidx = uidx;
    uidx = buf;
  }

  double     max_area = 0.0;
  int        max_idx  = lidx;
  double max_idx_frac = 0.0;

  for ( int k=lidx; k<=uidx; k++ )
  {
    if ( fabs(max_area) < fabs(y[k]) )
    {
      max_area = y[k];
      max_idx  = k;
    }
  }

  if ( SearchMode == search_180 )
  {
    for (int i=lidx+1; i<=uidx; i++)
    {
      if ( z==0.0  && y[i]*y[i-1] < 0 )
        PkZeroCross = z = ( y[i]*x[i-1] - y[i-1]*x[i] ) / ( y[i] - y[i-1] );
    }

    if (z>0)
    {
      z /= 2;

      max_idx      = MAX_OF( 0, std::lower_bound( x, x + n, z ) - x - 1 );
      max_idx_frac = z - x[max_idx];

    }
    else
    {
      SearchMode = search_90;
      setmessage("No zero-crossing found. Switching to maximum search.");
    }
  }

  PkMaxIdx     = max_idx;
  PkMaxIdxFrac = max_idx_frac;

  DB_MSG(("<--analyze_data"));
}


static void lin_reg (const double *x, const double *y, double *w, int n, double *ph0, double *ph1)
/* -------------------------------------------------------------------
   linear regression  y = ph0 + ph1*x (finds ph0 and ph1)
   -------------------------------------------------------------------*/
{

  if (n <= 0)
    throw PvStdException("lin_reg(): Array length should be a "
                         "positive integer, got n = %d", n);
  double weights[n];

  //if no w's given create own ones
  if ( w == NULL )
  {
    for (int k=0; k<n; k++)
      weights[k] = 1.0;

    w = weights;
  }

  double sx = 0, sy = 0, sxx = 0, sxy = 0, sw = 0;

  for (int i = 0; i < n; i++)
  {
    sw  += w[i];
    sx  += w[i] * x[i];
    sy  += w[i] * y[i];
    sxx += w[i] * x[i] * x[i];
    sxy += w[i] * x[i] * y[i];
  }

  double delta = sw * sxx - sx * sx;

  if (delta == 0)
    throw PvStdException("In function lin_reg(...): Linear regression "
                         "is ill-conditioned.");

  *ph1 = (sw * sxy - sx * sy)  / delta;
  *ph0 = (sxx * sy - sx * sxy) / delta;
}


#ifndef CPROTO
static std::array<int, 2> hz2idx(double *hz)
/* -------------------------------------------------------------------
   convert PkRange from Hz to array index.
   input ptr to 2-element double array PkRange
   ------------------------------------------------------------------*/
{
  double bw = PVM_SpecSWH[0];
  int  npts = PVM_SpecMatrix[0];

  std::array<int, 2> idx;

  for (int i=0; i<2; i++)
    idx[i] = ( *(hz + i) + 0.5* bw) * (npts-1)/bw;

  return idx;
}


static std::ofstream filehandle(std::string str)
/* -------------------------------------------------------------------
   log data for debug purposes into procno
   ------------------------------------------------------------------*/
{
  char f_name [PATH_MAX], dpath  [PATH_MAX];

  PvOvlUtilGetProcnoPath(dpath, PATH_MAX, 0);

  if ( !AdjRefPowXLog == none )
  {
    char dat2log[32];
    dat2log[0] = dat2log[31] = '\0';
    strncpy(dat2log, ParxRelsIntToEnumStr("AdjRefPowXLog", AdjRefPowXLog), 32);
    sprintf(f_name, "%s/%s", dpath, dat2log);
  }

  std::ofstream file;

  if (str.compare("open") == 0)
  {
    file.open(f_name);

    if (AdjRefPowXLog == spectra)
      file << "# real  imag" << std::endl;
    if ( AdjRefPowXLog == phases_uncor )
      file << "# phases [rad] of signal w max magn before lin regr" << std::endl;
    if ( AdjRefPowXLog == phases_cor )
      file << "# phases [rad] of signal w max magn after lin regr" << std::endl;
  }
  else if (str.compare("close") == 0)
    if (file.is_open())
      file.close();

  return file;

}
#endif

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
