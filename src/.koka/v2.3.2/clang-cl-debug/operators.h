#pragma once
#ifndef kk_operators_H
#define kk_operators_H
// Koka generated module: "operators", koka version: 2.3.2, platform: 64-bit
#include <kklib.h>
#include "std_core_types.h"
#include "std_core.h"

// type declarations

// value declarations
 
// List construction (note that literals can be created with [,] syntax, but this is poorly documented)

static inline kk_std_core__list kk_operators__lp__colon__colon__rp_(kk_box_t a, kk_std_core__list ls, kk_context_t* _ctx) { /* forall<a> (a : a, ls : list<a>) -> list<a> */ 
  return kk_std_core__new_Cons(kk_reuse_null, a, ls, _ctx);
}

static inline kk_std_core__list kk_operators__lp__colon__colon__1_rp_(kk_box_t a, kk_box_t b, kk_context_t* _ctx) { /* forall<a> (a : a, b : a) -> list<a> */ 
  kk_std_core__list _x1030 = kk_std_core__new_Cons(kk_reuse_null, b, kk_std_core__new_Nil(_ctx), _ctx); /*list<1009>*/
  return kk_std_core__new_Cons(kk_reuse_null, a, _x1030, _ctx);
}
 
// Currying

static inline kk_box_t kk_operators__lp__ques__rp_(kk_function_t h, kk_box_t a, kk_context_t* _ctx) { /* forall<a,e,b> (h : (a) -> e b, a : a) -> e b */ 
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), h, (h, a, _ctx));
}


// lift anonymous function
struct kk_operators__lp__ques__fun1031__t_1_rp_ {
  struct kk_function_s _base;
  kk_box_t a;
  kk_function_t h;
};
extern kk_box_t kk_operators__lp__ques__fun1031_1_rp_(kk_function_t _fself, kk_box_t b, kk_context_t* _ctx);
static inline kk_function_t kk_operators_new_dash__ques__fun1031_1(kk_box_t a, kk_function_t h, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1031__t_1_rp_* _self = kk_function_alloc_as(struct kk_operators__lp__ques__fun1031__t_1_rp_, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_operators__lp__ques__fun1031_1_rp_, kk_context());
  _self->a = a;
  _self->h = h;
  return &_self->_base;
}


static inline kk_function_t kk_operators__lp__ques__1_rp_(kk_function_t h, kk_box_t a, kk_context_t* _ctx) { /* forall<a,b,e,c> (h : (a, b) -> e c, a : a) -> ((b) -> e c) */ 
  return kk_operators_new_dash__ques__fun1031_1(a, h, _ctx);
}


// lift anonymous function
struct kk_operators__lp__ques__fun1032__t_2_rp_ {
  struct kk_function_s _base;
  kk_box_t a;
  kk_function_t h;
};
extern kk_box_t kk_operators__lp__ques__fun1032_2_rp_(kk_function_t _fself, kk_box_t b, kk_box_t c, kk_context_t* _ctx);
static inline kk_function_t kk_operators_new_dash__ques__fun1032_2(kk_box_t a, kk_function_t h, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1032__t_2_rp_* _self = kk_function_alloc_as(struct kk_operators__lp__ques__fun1032__t_2_rp_, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_operators__lp__ques__fun1032_2_rp_, kk_context());
  _self->a = a;
  _self->h = h;
  return &_self->_base;
}


static inline kk_function_t kk_operators__lp__ques__2_rp_(kk_function_t h, kk_box_t a, kk_context_t* _ctx) { /* forall<a,b,c,e,d> (h : (a, b, c) -> e d, a : a) -> ((b, c) -> e d) */ 
  return kk_operators_new_dash__ques__fun1032_2(a, h, _ctx);
}


// lift anonymous function
struct kk_operators__lp__ques__fun1033__t_3_rp_ {
  struct kk_function_s _base;
  kk_box_t a;
  kk_function_t h;
};
extern kk_box_t kk_operators__lp__ques__fun1033_3_rp_(kk_function_t _fself, kk_box_t b, kk_box_t c, kk_box_t d, kk_context_t* _ctx);
static inline kk_function_t kk_operators_new_dash__ques__fun1033_3(kk_box_t a, kk_function_t h, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1033__t_3_rp_* _self = kk_function_alloc_as(struct kk_operators__lp__ques__fun1033__t_3_rp_, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_operators__lp__ques__fun1033_3_rp_, kk_context());
  _self->a = a;
  _self->h = h;
  return &_self->_base;
}


static inline kk_function_t kk_operators__lp__ques__3_rp_(kk_function_t h, kk_box_t a, kk_context_t* _ctx) { /* forall<a,b,c,d,e,a1> (h : (a, b, c, d) -> e a1, a : a) -> ((b, c, d) -> e a1) */ 
  return kk_operators_new_dash__ques__fun1033_3(a, h, _ctx);
}


// lift anonymous function
struct kk_operators__lp__ques__fun1034__t_4_rp_ {
  struct kk_function_s _base;
  kk_box_t a;
  kk_function_t h;
};
extern kk_box_t kk_operators__lp__ques__fun1034_4_rp_(kk_function_t _fself, kk_box_t b, kk_box_t c, kk_box_t d, kk_box_t e, kk_context_t* _ctx);
static inline kk_function_t kk_operators_new_dash__ques__fun1034_4(kk_box_t a, kk_function_t h, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1034__t_4_rp_* _self = kk_function_alloc_as(struct kk_operators__lp__ques__fun1034__t_4_rp_, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_operators__lp__ques__fun1034_4_rp_, kk_context());
  _self->a = a;
  _self->h = h;
  return &_self->_base;
}


static inline kk_function_t kk_operators__lp__ques__4_rp_(kk_function_t h, kk_box_t a, kk_context_t* _ctx) { /* forall<a,b,c,d,a1,e,b1> (h : (a, b, c, d, a1) -> e b1, a : a) -> ((b, c, d, a1) -> e b1) */ 
  return kk_operators_new_dash__ques__fun1034_4(a, h, _ctx);
}


// lift anonymous function
struct kk_operators__lp__ques__fun1035__t_5_rp_ {
  struct kk_function_s _base;
  kk_box_t a;
  kk_function_t h;
};
extern kk_box_t kk_operators__lp__ques__fun1035_5_rp_(kk_function_t _fself, kk_box_t b, kk_box_t c, kk_box_t d, kk_box_t e, kk_box_t f, kk_context_t* _ctx);
static inline kk_function_t kk_operators_new_dash__ques__fun1035_5(kk_box_t a, kk_function_t h, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1035__t_5_rp_* _self = kk_function_alloc_as(struct kk_operators__lp__ques__fun1035__t_5_rp_, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_operators__lp__ques__fun1035_5_rp_, kk_context());
  _self->a = a;
  _self->h = h;
  return &_self->_base;
}


static inline kk_function_t kk_operators__lp__ques__5_rp_(kk_function_t h, kk_box_t a, kk_context_t* _ctx) { /* forall<a,b,c,d,a1,b1,e,c1> (h : (a, b, c, d, a1, b1) -> e c1, a : a) -> ((b, c, d, a1, b1) -> e c1) */ 
  return kk_operators_new_dash__ques__fun1035_5(a, h, _ctx);
}


// lift anonymous function
struct kk_operators__lp__ques__fun1036__t_6_rp_ {
  struct kk_function_s _base;
  kk_box_t a;
  kk_function_t h;
};
extern kk_box_t kk_operators__lp__ques__fun1036_6_rp_(kk_function_t _fself, kk_box_t b, kk_box_t c, kk_box_t d, kk_box_t e, kk_box_t f, kk_box_t g, kk_context_t* _ctx);
static inline kk_function_t kk_operators_new_dash__ques__fun1036_6(kk_box_t a, kk_function_t h, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1036__t_6_rp_* _self = kk_function_alloc_as(struct kk_operators__lp__ques__fun1036__t_6_rp_, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_operators__lp__ques__fun1036_6_rp_, kk_context());
  _self->a = a;
  _self->h = h;
  return &_self->_base;
}


static inline kk_function_t kk_operators__lp__ques__6_rp_(kk_function_t h, kk_box_t a, kk_context_t* _ctx) { /* forall<a,b,c,d,a1,b1,c1,e,d1> (h : (a, b, c, d, a1, b1, c1) -> e d1, a : a) -> ((b, c, d, a1, b1, c1) -> e d1) */ 
  return kk_operators_new_dash__ques__fun1036_6(a, h, _ctx);
}

void kk_operators__init(kk_context_t* _ctx);


void kk_operators__done(kk_context_t* _ctx);

#endif // header
