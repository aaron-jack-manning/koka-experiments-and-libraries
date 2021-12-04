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

kk_unit_t kk_display_println_1(kk_std_core__list ls, kk_context_t* _ctx); /* (ls : list<char>) -> console () */ 

kk_unit_t kk_display_println_2(kk_std_core__list ls, kk_context_t* _ctx); /* (ls : list<bool>) -> console () */ 

kk_unit_t kk_display_println_3(kk_std_core__list ls, kk_context_t* _ctx); /* (ls : list<double>) -> console () */ 

kk_unit_t kk_display_println_4(kk_std_core__list ls, kk_context_t* _ctx); /* (ls : list<string>) -> console () */ 

kk_unit_t kk_display_println_5(kk_vector_t vec, kk_context_t* _ctx); /* (vec : vector<int>) -> console () */ 

kk_unit_t kk_display_println_6(kk_vector_t vec, kk_context_t* _ctx); /* (vec : vector<char>) -> console () */ 

kk_unit_t kk_display_println_7(kk_vector_t vec, kk_context_t* _ctx); /* (vec : vector<bool>) -> console () */ 

kk_unit_t kk_display_println_8(kk_vector_t vec, kk_context_t* _ctx); /* (vec : vector<double>) -> console () */ 

kk_unit_t kk_display_println_9(kk_vector_t vec, kk_context_t* _ctx); /* (vec : vector<string>) -> console () */ 

void kk_display__init(kk_context_t* _ctx);


void kk_display__done(kk_context_t* _ctx);

#endif // header
