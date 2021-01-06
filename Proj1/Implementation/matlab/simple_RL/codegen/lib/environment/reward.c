/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: reward.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Dec-2020 16:16:46
 */

/* Include Files */
#include "reward.h"
#include "environment_data.h"
#include "environment_initialize.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Give reward according to the respective state
 * Arguments    : double b_state
 *                double action
 * Return Type  : double
 */
double reward(double b_state, double action)
{
  static const signed char iv[7] = { 100, -1, -2, -5, -5, -2, -1 };

  static const signed char iv1[5] = { -2, -1, 0, -1, -2 };

  if (!isInitialized_environment) {
    environment_initialize();
  }

  /* Reward is a function of state and action */
  return (double)iv[(int)b_state - 1] - 0.5 * (double)iv1[(int)action - 1];
}

/*
 * File trailer for reward.c
 *
 * [EOF]
 */
