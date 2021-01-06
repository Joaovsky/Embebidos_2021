/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: exec.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 30-Dec-2020 16:16:46
 */

/* Include Files */
#include "exec.h"
#include "environment.h"
#include "environment_data.h"
#include "environment_initialize.h"
#include "rand.h"
#include "randn.h"
#include "reward.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_roundd_snf(double u);

/* Function Definitions */
/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/*
 * Initialize Q table
 * Arguments    : void
 * Return Type  : void
 */
void exec(void)
{
  double Q_table[35];
  double Action;
  double Greed;
  double cs;
  double cur_temp;
  double init_cur_temp;
  double ns;
  double r;
  double ref_temp;
  int Episode;
  int iindx;
  if (!isInitialized_environment) {
    environment_initialize();
  }

  /* Randomize Q initial values */
  randn(Q_table);

  /* Set initial parameters */
  Action = 3.0;

  /* All Actuators OFF */
  /* 0.9 */
  Greed = 0.3;

  /* 0.8                %Using Greedy for exploration vs exploitation */
  /* 0.7           % */
  /* 100 */
  /* Loop for each episode */
  for (Episode = 0; Episode < 1000; Episode++) {
    /* Set new initial current and reference temperatures  */
    init_cur_temp = 12.0 * b_rand() + 15.0;
    ref_temp = 12.0 * b_rand() + 15.0;
    ref_temp = rt_roundd_snf(ref_temp);

    /* ref_temp = 21; */
    /* Initialize S */
    environment(Action, init_cur_temp, ref_temp, &cur_temp, &cs, &r);

    /* Loop for each step of the episode */
    do {
      /* Choose A from S using policy derived from Q using  */
      b_rand();
      if (b_rand() > Greed) {
        init_cur_temp = Q_table[(int)cs - 1];
        iindx = -1;
        r = Q_table[(int)cs + 6];
        if (init_cur_temp < r) {
          init_cur_temp = r;
          iindx = 0;
        }

        r = Q_table[(int)cs + 13];
        if (init_cur_temp < r) {
          init_cur_temp = r;
          iindx = 1;
        }

        r = Q_table[(int)cs + 20];
        if (init_cur_temp < r) {
          init_cur_temp = r;
          iindx = 2;
        }

        if (init_cur_temp < Q_table[(int)cs + 27]) {
          iindx = 3;
        }

        Action = (double)iindx + 2.0;
      } else {
        Action = b_rand();
        Action = floor(Action * 5.0) + 1.0;
      }

      /* Take action A, observe R, S' */
      environment(Action, cur_temp, ref_temp, &cur_temp, &ns, &r);

      /* Determine max Q value of given action and next state */
      init_cur_temp = Q_table[(int)ns - 1];
      if (init_cur_temp < Q_table[(int)ns + 6]) {
        init_cur_temp = Q_table[(int)ns + 6];
      }

      if (init_cur_temp < Q_table[(int)ns + 13]) {
        init_cur_temp = Q_table[(int)ns + 13];
      }

      r = Q_table[(int)ns + 20];
      if (init_cur_temp < r) {
        init_cur_temp = r;
      }

      r = Q_table[(int)ns + 27];
      if (init_cur_temp < r) {
        init_cur_temp = r;
      }

      /* Update Q value from current state and action */
      iindx = ((int)cs + 7 * ((int)Action - 1)) - 1;
      Q_table[iindx] = 0.7 * Q_table[iindx] + 0.3 * (reward(cs, Action) + 0.5 *
        init_cur_temp);

      /* S <- S' */
      cs = ns;

      /* If S = 1, temperature converged, break */
    } while (!(ns == 1.0));

    /* Decrease exploration as Episode number increases. Exploit only when half */
    /* of the total episodes are executed */
    if (500 >= Episode + 1) {
      Greed -= 0.00060120240480961921;
    }
  }

  /* Plot Average Reward for the first n episodes, 1<=n<=Total_episodes */
  /* for n=1:Total_episodes */
  /* hold on */
  /* plot(n,sum(Episode_reward(1:n))/n, 'c+'); */
  /* Plot(episode, cur_temp, 'o'); */
  /* hold off */
  /* end */
  /* Save QTable with otimized values */
  /* save('QTable_S7_A5.mat','Q_table'); */
}

/*
 * File trailer for exec.c
 *
 * [EOF]
 */
