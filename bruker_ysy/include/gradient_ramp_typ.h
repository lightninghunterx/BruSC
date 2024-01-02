/*
 *******************************************************************
 *
 * $Source: /sc/CvsTree/sc/gen/h/gradient_ramp_typ.h,v $
 *
 * Copyright (c) 2003
 * BRUKER BioSpin GmbH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: gradient_ramp_typ.h,v 1.5 2017/12/18 14:05:27 mah Exp $
 *******************************************************************
 */

#ifndef GRADIENT_RAMP_TYPE_H
#define GRADIENT_RAMP_TYPE_H
/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type GRADIENT_TYPE

  @ptypePurpose		
      Defines, how a gradient is switched
    GradientRamped:
      Switch gradient via default ramp to reach the given value
    GradientRampedByDelay:
      Switch gradient with linear ramp during given delay 
      to reach the given value
    GradientShaped:
      Switch gradient via given shapes.
      Normally the final value is zero.
    GradientShapedWithGroff:
      Switch gradient via given shapes.
      It is ensured, that the gradients are swiched off
    ShimRamped:
      Switch shims with special shim_ramp.
      It is ensured, that the gradients are swiched off
  @ptypeAuthor      MAH                                 */
/*------------------------------------------------------*/

typedef enum
{
  GradientRamped,
  GradientRampedByDelay,
  GradientShaped,
  GradientShapedWithGroff,
  GradientTypeUnknown,
  ShimRamped 
} GRADIENT_TYPE;
/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type GRADIENT_RAMP_TYPE

  @ptypePurpose		
      Defines the gradient switching mode of unshaped gradients.
    Constant_time:
      Switching a gradient by a gradient program for one
      value to another will result into several linear inter-
      mediate steps (ramp) to reduce the noise generated
      during gradient switching and/or to avoid an overshoot
      of the preemphasis unit. The ramp time may be sepecified
      by the parameter @see PREEMP_ramp_time.
    Constant_slope:
      It has similar effects like above, but the time required
      to perform the gradient ramp depends on the difference
      of two subsequent gradient values. The slope may be
      specified by the parameter @see PREEMP_ramp_slope.
    Constant_time_xyz:
      It is used to select a ramp time for all three gradients
      interpendently. The ramp time is specified by the 
      parameters PREEMP_ramp_time_x, _y, _z.
    Ramp_off:
      Switches of any interpolation between two gradient 
      switching points.
    Shaped_ramp:
      Indicates, that the shepe @see PREEMP_ramp_shape is used, 
      to switch the gradients.
    Shaped_ramp_xyz:
      Indicates, that the shape @see PREEMP_ramp_shape_x, _y, or z
      is used, to switch the gradients Gx, Gy or Gz..
  @ptypeAuthor      MAH                                 */
/*------------------------------------------------------*/

typedef enum
{
  Constant_time,
  Constant_slope,
  Constant_time_xyz,
  Ramp_off,
  Shaped_ramp,
  Shaped_ramp_xyz
} GRADIENT_RAMP_TYPE;
#endif
