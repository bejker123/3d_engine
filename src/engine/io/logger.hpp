#pragma once
#include <stdio.h>

#define LOGGER_PREFIX "[LOGGER] "

#define DEBUG

#ifdef DEBUG
#define LOG                                                                    \
  printf(LOGGER_PREFIX);                                                       \
  printf
#define LOG_VAR(P, X) printf(LOGGER_PREFIX #X " = " P " @ %p\n", X, &X)
#define SLOG_VAR(X) printf(LOGGER_PREFIX #X " = \"%s\" @ %p\n", X, &X, X)
#else
#define LOG(...) ((void)0)
#define LOG_VAR(...) ((void)0)
#define SLOG_VAR(...) ((void)0)
#endif
