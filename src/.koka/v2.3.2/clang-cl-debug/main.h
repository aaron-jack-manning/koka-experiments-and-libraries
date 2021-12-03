#pragma once
#ifndef kk_main_H
#define kk_main_H
// Koka generated module: "main", koka version: 2.3.2, platform: 64-bit
#include <kklib.h>
#include "std_core_types.h"
#include "std_core.h"

// type declarations

// value declarations

static inline kk_unit_t kk_main_main(kk_context_t* _ctx) { /* () -> console () */ 
  kk_unit_t __ = kk_Unit;
  kk_string_t _x30;
  kk_define_string_literal(, _s31, 13, "Hello, World!")
  _x30 = kk_string_dup(_s31); /*string*/
  kk_std_core_printsln(_x30, _ctx);
  kk_Unit; return kk_Unit;
}

void kk_main__init(kk_context_t* _ctx);


void kk_main__done(kk_context_t* _ctx);

#endif // header
