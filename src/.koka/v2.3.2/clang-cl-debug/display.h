#pragma once
#ifndef kk_display_H
#define kk_display_H
// Koka generated module: "display", koka version: 2.3.2, platform: 64-bit
#include <kklib.h>
#include "std_core_types.h"
#include "std_core.h"

// type declarations

// value declarations

kk_unit_t kk_display_println(kk_std_core__list ls, kk_context_t* _ctx); /* (ls : list<int>) -> console () */ 

void kk_display__init(kk_context_t* _ctx);


void kk_display__done(kk_context_t* _ctx);

#endif // header
