/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: environment.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Dec-2020 16:16:46
 */

/* Include Files */
#include "environment.h"
#include "environment_data.h"
#include "environment_initialize.h"
#include "getState.h"
#include "rand.h"
#include "reward.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Calculate current temperature as function of last temperature state,
 * temperature variation given as consequence of action and stochastic noise
 * representing outside perturbations
 * Arguments    : double action
 *                double last_temp_state
 *                double ref_temp
 *                double *cur_temp
 *                double *b_state
 *                double *rewards
 * Return Type  : void
 */
void environment(double action, double last_temp_state, double ref_temp, double *
                 cur_temp, double *b_state, double *rewards)
{
  if (!isInitialized_environment) {
    environment_initialize();
  }

  *cur_temp = (last_temp_state + 2.0 * (b_rand() - 0.5)) + (0.5 * (double)((int)
    action - 1) + -1.0);

  /* Determine state and reward */
  *b_state = getState(ref_temp, *cur_temp);
  *rewards = reward(*b_state, action);
}

/*
 * File trailer for environment.c
 *
 * [EOF]
 */
