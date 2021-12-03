// Koka generated module: "operators", koka version: 2.3.2, platform: 64-bit
#include "operators.h"
extern kk_box_t kk_operators__lp__ques__fun1031_1_rp_(kk_function_t _fself, kk_box_t b, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1031__t_1_rp_* _self = kk_function_as(struct kk_operators__lp__ques__fun1031__t_1_rp_*, _fself);
  kk_box_t a = _self->a; /* 192 */
  kk_function_t h = _self->h; /* (192, 193) -> 194 195 */
  kk_drop_match(_self, {kk_box_dup(a);kk_function_dup(h);}, {}, _ctx)
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), h, (h, a, b, _ctx));
}
extern kk_box_t kk_operators__lp__ques__fun1032_2_rp_(kk_function_t _fself, kk_box_t b, kk_box_t c, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1032__t_2_rp_* _self = kk_function_as(struct kk_operators__lp__ques__fun1032__t_2_rp_*, _fself);
  kk_box_t a = _self->a; /* 245 */
  kk_function_t h = _self->h; /* (245, 246, 247) -> 248 249 */
  kk_drop_match(_self, {kk_box_dup(a);kk_function_dup(h);}, {}, _ctx)
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), h, (h, a, b, c, _ctx));
}
extern kk_box_t kk_operators__lp__ques__fun1033_3_rp_(kk_function_t _fself, kk_box_t b, kk_box_t c, kk_box_t d, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1033__t_3_rp_* _self = kk_function_as(struct kk_operators__lp__ques__fun1033__t_3_rp_*, _fself);
  kk_box_t a = _self->a; /* 327 */
  kk_function_t h = _self->h; /* (327, 328, 329, 330) -> 331 332 */
  kk_drop_match(_self, {kk_box_dup(a);kk_function_dup(h);}, {}, _ctx)
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), h, (h, a, b, c, d, _ctx));
}
extern kk_box_t kk_operators__lp__ques__fun1034_4_rp_(kk_function_t _fself, kk_box_t b, kk_box_t c, kk_box_t d, kk_box_t e, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1034__t_4_rp_* _self = kk_function_as(struct kk_operators__lp__ques__fun1034__t_4_rp_*, _fself);
  kk_box_t a = _self->a; /* 444 */
  kk_function_t h = _self->h; /* (444, 445, 446, 447, 448) -> 449 450 */
  kk_drop_match(_self, {kk_box_dup(a);kk_function_dup(h);}, {}, _ctx)
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), h, (h, a, b, c, d, e, _ctx));
}
extern kk_box_t kk_operators__lp__ques__fun1035_5_rp_(kk_function_t _fself, kk_box_t b, kk_box_t c, kk_box_t d, kk_box_t e, kk_box_t f, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1035__t_5_rp_* _self = kk_function_as(struct kk_operators__lp__ques__fun1035__t_5_rp_*, _fself);
  kk_box_t a = _self->a; /* 602 */
  kk_function_t h = _self->h; /* (602, 603, 604, 605, 606, 607) -> 608 609 */
  kk_drop_match(_self, {kk_box_dup(a);kk_function_dup(h);}, {}, _ctx)
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), h, (h, a, b, c, d, e, f, _ctx));
}
extern kk_box_t kk_operators__lp__ques__fun1036_6_rp_(kk_function_t _fself, kk_box_t b, kk_box_t c, kk_box_t d, kk_box_t e, kk_box_t f, kk_box_t g, kk_context_t* _ctx) {
  struct kk_operators__lp__ques__fun1036__t_6_rp_* _self = kk_function_as(struct kk_operators__lp__ques__fun1036__t_6_rp_*, _fself);
  kk_box_t a = _self->a; /* 807 */
  kk_function_t h = _self->h; /* (807, 808, 809, 810, 811, 812, 813) -> 814 815 */
  kk_drop_match(_self, {kk_box_dup(a);kk_function_dup(h);}, {}, _ctx)
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), h, (h, a, b, c, d, e, f, g, _ctx));
}

// initialization
void kk_operators__init(kk_context_t* _ctx){
  static bool _kk_initialized = false;
  if (_kk_initialized) return;
  _kk_initialized = true;
  kk_std_core_types__init(_ctx);
  kk_std_core__init(_ctx);
  #if defined(KK_CUSTOM_INIT)
    KK_CUSTOM_INIT (_ctx);
  #endif
}

// termination
void kk_operators__done(kk_context_t* _ctx){
  static bool _kk_done = false;
  if (_kk_done) return;
  _kk_done = true;
  #if defined(KK_CUSTOM_DONE)
    KK_CUSTOM_DONE (_ctx);
  #endif
  kk_std_core__done(_ctx);
  kk_std_core_types__done(_ctx);
}
