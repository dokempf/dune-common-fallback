// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef __GRAPE_COMMON_H__
#define __GRAPE_COMMON_H__

/* global variables for leaf iterator display */
static BUTTON * leafButton=0;
/* global variables for maxlevel use */
static BUTTON * maxlevelButton=0;

static TIMESCENE * globalTsc = 0;

/* info about data on one mesh */
typedef struct datainfo DATAINFO;
struct datainfo
{
  const char * name;
  const char * base_name;
  DATAINFO *next;

  int dimVal; /* length of vector (dimVal = 1 --> scalar, otherwise vector  */
  int * comp; /* number of each component */
};

/* info about one mesh */
typedef struct info INFO;
struct info
{
  int fix_mesh; /* if no dynamic grid 1 : else 0 */
  const char  *name;
  DATAINFO * datinf;
  void  *tsc;
};

void setupLeafButton(MANAGER *mgr, void *sc, int yesTimeScene);

#endif
