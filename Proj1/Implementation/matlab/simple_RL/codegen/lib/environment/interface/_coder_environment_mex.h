/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_environment_mex.h
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Dec-2020 16:16:46
 */

#ifndef _CODER_ENVIRONMENT_MEX_H
#define _CODER_ENVIRONMENT_MEX_H

/* Include Files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#ifdef __cplusplus

extern "C" {

#endif

  /* Function Declarations */
  void environment_mexFunction(int32_T nlhs, mxArray *plhs[3], int32_T nrhs,
    const mxArray *prhs[3]);
  void exec_mexFunction(int32_T nlhs, int32_T nrhs);
  void getState_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs, const
    mxArray *prhs[2]);
  MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T
    nrhs, const mxArray *prhs[]);
  emlrtCTX mexFunctionCreateRootTLS(void);
  void reward_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs, const
    mxArray *prhs[2]);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for _coder_environment_mex.h
 *
 * [EOF]
 */
