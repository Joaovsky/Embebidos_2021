/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: eml_rand_mt19937ar_stateful.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Dec-2020 16:16:46
 */

/* Include Files */
#include "eml_rand_mt19937ar_stateful.h"
#include "environment_data.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void c_eml_rand_mt19937ar_stateful_i(void)
{
  int mti;
  unsigned int r;

  /*@>35834*/
  memset(&                             /*@>3451e*/
         state                         /*@>3451f*/
         [                             /*@>3451b*/
         0],                           /*@>407f8*/
         0,                            /*@>3451d*/
         625U                          /*@>34525*/
         *                             /*@>34524*/
         sizeof(unsigned int));

  /*@>275ec*/
  r                                    /*@>275eb*/
    =                                  /*@>2807f*/
    5489U;

  /*@>2ca38*/
  state                                /*@>27618*/
    [                                  /*@>27629*/
    0]                                 /*@>275ef*/
    =                                  /*@>28080*/
    5489U;

  /*@>27622*/
  for (                                /*@>407fd*/
       mti =                           /*@>2760a*/
       0;                              /*@>40805*/
       mti <                           /*@>27608*/
       623;                            /*@>40802*/
       mti                             /*@>40809*/
       ++) {
    /*@>275f4*/
    r                                  /*@>275f3*/
      = ((                             /*@>275f5*/
          r                            /*@>3034e*/
          ^                            /*@>275f7*/
          r                            /*@>27615*/
          >>                           /*@>2762a*/
          30U)                         /*@>3034b*/
         *                             /*@>2762b*/
         1812433253U                   /*@>304e0*/
         +                             /*@>28fc8*/
         mti)                          /*@>304e1*/
      +                                /*@>275fb*/
      1U;

    /*@>2ca48*/
    state                              /*@>27619*/
      [                                /*@>28fd1*/
      mti                              /*@>291d2*/
      +                                /*@>2761b*/
      1]                               /*@>275fc*/
      =                                /*@>27600*/
      r;
  }

  /*@>2ca3e*/
  state                                /*@>2761d*/
    [                                  /*@>2762c*/
    624]                               /*@>27602*/
    =                                  /*@>2762d*/
    624U;
}

/*
 * File trailer for eml_rand_mt19937ar_stateful.c
 *
 * [EOF]
 */
