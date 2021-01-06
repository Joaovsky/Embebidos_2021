/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Dec-2020 16:16:46
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

/* Include Files */
#include "main.h"
#include "environment.h"
#include "environment_terminate.h"
#include "exec.h"
#include "getState.h"
#include "reward.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static double argInit_real_T(void);
static void main_environment(void);
static void main_exec(void);
static void main_getState(void);
static void main_reward(void);

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_environment(void)
{
  double action_tmp;
  double b_state;
  double cur_temp;
  double rewards;

  /* Initialize function 'environment' input arguments. */
  action_tmp = argInit_real_T();

  /* Call the entry-point 'environment'. */
  environment(action_tmp, action_tmp, action_tmp, &cur_temp, &b_state, &rewards);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_exec(void)
{
  /* Call the entry-point 'exec'. */
  exec();
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_getState(void)
{
  double ref_tmp;

  /* Initialize function 'getState' input arguments. */
  ref_tmp = argInit_real_T();

  /* Call the entry-point 'getState'. */
  ref_tmp = getState(ref_tmp, ref_tmp);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_reward(void)
{
  double state_tmp;

  /* Initialize function 'reward' input arguments. */
  state_tmp = argInit_real_T();

  /* Call the entry-point 'reward'. */
  state_tmp = reward(state_tmp, state_tmp);
}

/*
 * Arguments    : int argc
 *                const char * const argv[]
 * Return Type  : int
 */
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* The initialize function is being called automatically from your entry-point function. So, a call to initialize is not included here. */
  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_environment();
  main_exec();
  main_getState();
  main_reward();

  /* Terminate the application.
     You do not need to do this more than one time. */
  environment_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
