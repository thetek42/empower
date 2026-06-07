/*
 * Template for Empower tests.
 * :%s/<TEMPLATE>/MODULE_NAME
 * :%s/<template>/module_name
 * :%s/<description>/description
 */

#define E_<TEMPLATE>_IMPL
#include "e_<template>.h"
#include "e_test.h"

void test_<template>(void) {
    /* ... tests ... */
}
