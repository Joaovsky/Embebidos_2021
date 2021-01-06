/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: environment_initialize.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Dec-2020 16:16:46
 */

/* Include Files */
#include "environment_initialize.h"
#include "eml_rand_mt19937ar_stateful.h"
#include "environment_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void environment_initialize(void)
{
  c_eml_rand_mt19937ar_stateful_i();
  isInitialized_environment = true;
}

/*
 * File trailer for environment_initialize.c
 *
 * [EOF]
 */
