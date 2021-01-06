/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_environment_api.h
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Dec-2020 16:16:46
 */

#ifndef _CODER_ENVIRONMENT_API_H
#define _CODER_ENVIRONMENT_API_H

/* Include Files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void environment(real_T action, real_T last_temp_state, real_T ref_temp,
                   real_T *cur_temp, real_T *state, real_T *rewards);
  void environment_api(const mxArray * const prhs[3], int32_T nlhs, const
                       mxArray *plhs[3]);
  void environment_atexit(void);
  void environment_initialize(void);
  void environment_terminate(void);
  void environment_xil_shutdown(void);
  void environment_xil_terminate(void);
  void exec(void);
  void exec_api(void);
  real_T getState(real_T ref, real_T cur);
  void getState_api(const mxArray * const prhs[2], const mxArray *plhs[1]);
  real_T reward(real_T state, real_T action);
  void reward_api(const mxArray * const prhs[2], const mxArray *plhs[1]);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for _coder_environment_api.h
 *
 * [EOF]
 */
