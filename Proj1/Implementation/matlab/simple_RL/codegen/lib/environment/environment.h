/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: environment.h
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Dec-2020 16:16:46
 */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  extern void environment(double action, double last_temp_state, double ref_temp,
    double *cur_temp, double *b_state, double *rewards);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for environment.h
 *
 * [EOF]
 */
