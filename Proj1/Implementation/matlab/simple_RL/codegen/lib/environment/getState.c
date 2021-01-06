/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: getState.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Dec-2020 16:16:46
 */

/* Include Files */
#include "getState.h"
#include "environment_data.h"
#include "environment_initialize.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <float.h>
#include <math.h>

/* Function Declarations */
static double rt_remd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_remd_snf(double u0, double u1)
{
  double q;
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1) || rtIsInf(u0)) {
    y = rtNaN;
  } else if (rtIsInf(u1)) {
    y = u0;
  } else if ((u1 != 0.0) && (u1 != trunc(u1))) {
    q = fabs(u0 / u1);
    if (!(fabs(q - floor(q + 0.5)) > DBL_EPSILON * q)) {
      y = 0.0 * u0;
    } else {
      y = fmod(u0, u1);
    }
  } else {
    y = fmod(u0, u1);
  }

  return y;
}

/*
 * If temperature difference surpasses 15:
 * Arguments    : double ref
 *                double cur
 * Return Type  : double
 */
double getState(double ref, double cur)
{
  static const signed char lookup_table[45] = { 5, 5, 5, 5, 5, 0, 0, 0, 0, 6, 6,
    6, 6, 6, 0, 0, 0, 0, 7, 7, 7, 7, 1, 2, 2, 2, 2, 0, 0, 0, 0, 3, 3, 3, 3, 3, 0,
    0, 0, 0, 4, 4, 4, 4, 4 };

  double d;
  double s;
  if (!isInitialized_environment) {
    environment_initialize();
  }

  d = ref - cur;
  if (d >= 15.0) {
    s = 4.0;
  } else if (cur - ref >= 15.0) {
    s = 5.0;
  } else {
    /* Get State */
    s = lookup_table[((int)(trunc(rt_remd_snf(d, 5.0)) + 5.0) + 9 * ((int)(trunc
      (d / 5.0) + 3.0) - 1)) - 1];
  }

  return s;
}

/*
 * File trailer for getState.c
 *
 * [EOF]
 */
