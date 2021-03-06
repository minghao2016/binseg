/* -*- compile-command: "R CMD INSTALL .." -*- */

#include "binseg_normal.h"
#include "binseg_normal_cost.h"
#include <R.h>//for error
#include <R_ext/Rdynload.h>//for registering routines.
#include <Rinternals.h>//*SXP

static int binseg_normal_nargs = 11;
static R_NativePrimitiveArgType binseg_normal_types[] =
  {REALSXP, INTSXP,
   INTSXP, INTSXP, REALSXP,
   REALSXP, REALSXP,
   INTSXP, INTSXP,
   INTSXP, INTSXP};
void binseg_normal_interface
(double *data_vec, int *n_data,
 int *max_segments, int *seg_end, double *cost,
 double *before_mean, double *after_mean,
 int *before_size, int *after_size, 
 int *invalidates_index, int *invalidates_after){
  int status = binseg_normal
    (data_vec, *n_data, *max_segments, seg_end, cost,
     before_mean, after_mean,
     before_size, after_size,
     invalidates_index, invalidates_after);
  if(status == ERROR_NO_DATA){
    error("no data"); 
  }
  if(status == ERROR_NO_SEGMENTS){
    error("no segments"); 
  }
  if(status == ERROR_TOO_MANY_SEGMENTS){
    error("too many segments"); 
  }
  if(status != 0){
    error("non-zero status from binseg_normal");
  }
}
  
static int binseg_normal_cost_nargs = 4;
static R_NativePrimitiveArgType binseg_normal_cost_types[] =
  {REALSXP, INTSXP,
   INTSXP, REALSXP};
void binseg_normal_cost_interface
(double *data_vec, int *n_data,
   int *max_segments, double *cost){
  binseg_normal_cost
    (data_vec, *n_data, *max_segments, cost);
}
  
R_CMethodDef cMethods[] = {
  {"binseg_normal_interface",
   (DL_FUNC) &binseg_normal_interface, binseg_normal_nargs, binseg_normal_types
  },
  {"binseg_normal_cost_interface",
   (DL_FUNC) &binseg_normal_cost_interface,
     binseg_normal_cost_nargs,
     binseg_normal_cost_types
  },
  {NULL, NULL, 0, NULL}
};

extern "C" {
  void R_init_binseg(DllInfo *info) {
    R_registerRoutines(info, cMethods, NULL, NULL, NULL);
    //R_useDynamicSymbols call says the DLL is not to be searched for
    //entry points specified by character strings so .C etc calls will
    //only find registered symbols.
    R_useDynamicSymbols(info, FALSE);
  }
}
