/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/


static const char resid[] = "$Id$ (C) 2004 Bruker BioSpin MRI GmbH";

#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0


#include "method.h"


/**********************************************************************/
/* Local functions for calculation of distribution of angles          */
/**********************************************************************/

/** Number of angles distributed evenly on a hemisphere.

  @param n       matrix size
  @param factor  additional factor (i.e. from undersampling)
  
  @return number of angles
*/
int radialAngles(int n, double factor)
{
  return (int)ceil(M_PI * 0.5 * (double)n * factor);
}

/** Angle value on a half sphere.

  The zero value is excluded.

  @param i       angle index
  @param n       number of angles
  
  @return angle value [rad]
*/
double radialAngle(int i, int n)
{
  return M_PI * ((double)i + 0.5) / (double)n;
}

/**********************************************************************/
/* Local functions for reordering projections                         */
/**********************************************************************/

/** Find an unused Phi for a given Theta for reordering.

  @param iTheta  index for Theta angle
  @param nPhiMax maximum number of Phi values (= array size in 2nd dimension)
  @param mask    mask, where 0 = unused and 1 = used
  
  @return Yes, if an unused Phi was found
*/
YesNo reorderFindPhi(int iTheta, int nPhiMax, int *mask)
{
  int i0 = iTheta * nPhiMax;

  for (int i = 0; i < nPhiMax; i++)
  {
    if (mask[i0 + i] == 0) return Yes;
  }
  
  return No;
}

/** Increment array index within boundaries for reordering.

  If incrementing in the current direction is not possible, the direction is changed.

  @param n  maximum index value (minimum is 0)
  @param i  on input: index to be incremented; on output: incremented index value
  @param d  on input: step size giving direction (+1 or -1); on output: may be changed 

  @return Yes, if direction was changed
*/
YesNo reorderIncrIndex(int n, int *i, int *d)
{
  YesNo changed = No;
  
  if (*i + *d > n - 1 || *i + *d < 0)
  {
    *d *= -1;
    changed = Yes;
  }
  *i += *d;
  
  return changed;
}

/** Calculate continuous projection index for reordering.

  @param iTheta  index of Theta angle
  @param iPhi    index of Phi angle
  @param nPhi    list with number of Phi angles for each Theta
  
  @return index in projection list
*/
int reorderProjectionIndex(int iTheta, int iPhi, int *nPhi)
{
  int ii = 0;
  
  for (int i = 0; i < iTheta; i++) ii += nPhi[i];
  
  return ii + iPhi;
}

/** Reorder radial projections for improved spoiling.

  Two rules are applied:
  - Use small gradient steps.
  - Avoid multiple repetitions of similar gradient values.
  
  @param nTheta  number of Theta angles
  @param nPhi    list with number of Phi angles for each Theta
  @param gr      list of Read gradient values
  @param gp      list of Phase gradient values
  @param gs      list of Slice gradient values
*/
void reorderProjections(int nTheta, int *nPhi, double *gr, double *gp, double *gs)
{
  // maximum number of Phi angles per Theta
  int nPhiMax = 0;
  for (int iTheta = 0; iTheta < nTheta; iTheta++) nPhiMax = MAX_OF(nPhiMax, nPhi[iTheta]);
  
  // number of projections
  int nPro = reorderProjectionIndex(nTheta, 0, nPhi);
  
  // prepare arrays
  PARX_change_dims("ReorderGR", nPro);
  PARX_change_dims("ReorderGP", nPro);
  PARX_change_dims("ReorderGS", nPro);
  PARX_change_dims("ReorderMask", nTheta, nPhiMax);
      
  // initialise mask
  for (int iTheta = 0; iTheta < nTheta; iTheta++)
  {
    for (int iPhi = 0; iPhi < nPhi[iTheta]; iPhi++)       ReorderMask[iTheta][iPhi] = 0;
    for (int iPhi = nPhi[iTheta]; iPhi < nPhiMax; iPhi++) ReorderMask[iTheta][iPhi] = 1;
  }
      
  // indices
  int iTheta = 0;      // index for angle Theta
  int dTheta = 1;      // step for Theta index
  int iPhi   = 0;      // index for angle Phi
  int dPhi   = 1;      // step for Phi index
        
  // loop until each projection has a new position
  for (int i = 0; i < nPro; i++)
  {
    // find next Theta with unused Phi
    while (!reorderFindPhi(iTheta, nPhiMax, ReorderMask))
    {
      reorderIncrIndex(nTheta, &iTheta, &dTheta);
    }
        
    // find next Phi
    while (ReorderMask[iTheta][iPhi] == 1)
    {
      reorderIncrIndex(nPhiMax, &iPhi, &dPhi);
    }
          
    // set value
    int ii = reorderProjectionIndex(iTheta, iPhi, nPhi);
    ReorderGR[i] = gr[ii];
    ReorderGP[i] = gp[ii];
    ReorderGS[i] = gs[ii];
    ReorderMask[iTheta][iPhi] = 1;
    
    // increase Theta and Phi
    reorderIncrIndex(nTheta, &iTheta, &dTheta);
    reorderIncrIndex(nPhiMax, &iPhi, &dPhi);
  }
      
  // write reordered values to output arrays
  for (int i = 0; i < nPro; i++)
  {
    gr[i] = ReorderGR[i];
    gp[i] = ReorderGP[i];
    gs[i] = ReorderGS[i];
  }
      
  // free memory
  ParxRelsResetPar("ReorderMask");
  ParxRelsResetPar("ReorderGR");
  ParxRelsResetPar("ReorderGP");
  ParxRelsResetPar("ReorderGS");
}

void reorderProjectionsSilent(int nTheta, int *nPhi, double *gr, double *gp, double *gs)
{
  int i = 0;
  
  for (int iTheta = 0; iTheta < nTheta; iTheta++)
  {
    if (iTheta % 2 == 1)
    {
      for (int iPhi = 0; iPhi < nPhi[iTheta] / 2; iPhi++)
      {
        int i0 = i+iPhi;
        int i1 = i+nPhi[iTheta]-1-iPhi;
        swap(&gr[i0], &gr[i1]);
        swap(&gp[i0], &gp[i1]);
        swap(&gs[i0], &gs[i1]);
      }
    }     
    i += nPhi[iTheta];
  }
}

void swap(double *a, double *b)
{
  double c = *a;
  *a = *b;
  *b = c;
}


/**********************************************************************/
/* Functions to be called in method                                   */
/**********************************************************************/

/** Calculate gradients for 3D radial imaging.

  Usually this function is called in the following way:
  - Call function with the array pointers set to NULL in order to obtain the number of projections.
  - Create the arrays with the determined size.
  - Call the function again with the array pointers set in order to calculate the gradients.

  Distribution of angular values:
  - The zero values are excluded.
  - The end of the spokes run on rings attatched to a sphere.
  - Every other ring is started with opposite direction to keep steps small.
  - The second hemisphere, if requested, has opposite gradients.
  - First, the first hemisphere is executed, than the second one.

  @param matrixSize     matrix size of final image
  @param underSampling  polar undersampling factor (reduces number of projections linearly)
  @param halfSphere     Yes, if only one hemisphere in k-space should be covered
  @param useOrigin      Yes, if the first projection is additional without gradients
  @param reOrder        Yes, if the projections should be reordered for improved spoiling
  @param gradR          array for read gradient values, may be NULL
  @param gradP          array for phase gradient values, may be NULL
  @param gradS          array for slice gradient values, may be NULL
    
  @return total number of projections
*/
int radialGrad3D(
  int    matrixSize,
  double underSampling,
  YesNo  halfSphere,
  YesNo  useOrigin,
  YesNo  reOrder,
  double *gradR,
  double *gradP,
  double *gradS)
{
  DB_MSG(("-->radialGrad3D"));
  
  double underSamp = sqrt(underSampling);
  int nTheta = radialAngles(matrixSize, 1.0 / underSamp);
  int nPro = 0, iPro = 0, nPro0 = 0;
  bool calcGrad = (gradR != NULL && gradP != NULL && gradS != NULL);
  
  // without gradient
  if (useOrigin)
  {
    if (calcGrad)
    {
      gradR[iPro] = 0.0;
      gradP[iPro] = 0.0;
      gradS[iPro] = 0.0;
      iPro++;
    }
    nPro0 = 1;
  }
  
  PARX_change_dims("RadialNPhi", nTheta);
  
  // evenly distributed projections on one hemisphere (rings for different theta angles)
  for (int iTheta = 0; iTheta < nTheta; iTheta++)
  {
    double theta = radialAngle(iTheta, nTheta);
    int nPhi = radialAngles(matrixSize, sin(theta) / underSamp);
    
    RadialNPhi[iTheta] = nPhi;
    
    // total number of projections 
    nPro += nPhi;
    
    // calculate gradients
    if (calcGrad)
    {
      for (int iPhi = 0; iPhi < nPhi; iPhi++)
      {
        double phi = radialAngle(iPhi, nPhi);
        gradR[iPro] = sin(theta) * cos(phi);
        gradP[iPro] = sin(theta) * sin(phi);
        gradS[iPro] = cos(theta);
        iPro++;
      }
    }
  }
  
  // reorder projections
  if (calcGrad)
  {
    if (reOrder)
    {
      reorderProjections(nTheta, RadialNPhi, gradR + nPro0, gradP + nPro0, gradS + nPro0);
    }
    else
    {
      reorderProjectionsSilent(nTheta, RadialNPhi, gradR + nPro0, gradP + nPro0, gradS + nPro0);
    }
  }
  
  ParxRelsResetPar("RadialNPhi");
  
  // calculate gradients for other hemisphere
  if (!halfSphere)
  {
    if (calcGrad)
    {
      for (int i = 0; i < nPro; i++)
      {
        gradR[iPro] = - gradR[iPro - nPro];
        gradP[iPro] = - gradP[iPro - nPro];
        gradS[iPro] = - gradS[iPro - nPro];
        iPro++;
      }
    }
    nPro *= 2;
  }
  
  nPro += nPro0;
  
#if DEBUG
  if (calcGrad)
  {
    FILE *fp = NULL;

    if ((fp = fopen("/tmp/zte_grps", "w")) == NULL) return -1;

    for (int i = 0; i < nPro; i++)
    {
      fprintf(fp, "%f %f %f\n", gradR[i], gradP[i], gradS[i]);
    }
    fclose(fp);
  }
#endif
  
  DB_MSG(("<--radialGrad3D"));
  
  return nPro;
}


/** Calculate trajectory for 3D radial imaging and store in file.

  For each projection a vector with sampling positions is created.
  
  @param matrixSize     matrix size of final image
  @param useOrigin      Yes, if the first projection is additional without gradients
  @param nPro           number of projections (without origin)
  @param gradR          array with read gradient values
  @param gradP          array with phase gradient values
  @param gradS          array with slice gradient values
  @param fileName       name of binary file where the trajectory will be stored
    
  @return 0 = success, -1 = error 
*/
int radialTraj3D(
  int    matrixSize,
  YesNo  useOrigin,
  int    nPro, 
  double *gradR, 
  double *gradP, 
  double *gradS, 
  const  char *fileName)
{
  DB_MSG(("-->radialTraj3D"));
  
  const int offsetPro = useOrigin ? 1 : 0;
  FILE *fp = NULL;
  
  fp = fopen(fileName, "w");
  if (fp == NULL)
  {
    DB_MSG(("<--radialTraj3D with error"));
    return -1;
  }

  for (int iPro = offsetPro; iPro < nPro+offsetPro; iPro++)
  {
    for (int iSamp = 0; iSamp < matrixSize; iSamp++)
    {
      double samp = (double)iSamp / (double)matrixSize - 0.5;
      double sampVal[3];
      
      sampVal[0] = samp * gradR[iPro];
      sampVal[1] = samp * gradP[iPro];
      sampVal[2] = samp * gradS[iPro];
      
      fwrite(sampVal, 3, sizeof(double), fp);
    }
  }
  
  fclose(fp);
  
  DB_MSG(("<--radialTraj3D"));
  
  return 0;
}

