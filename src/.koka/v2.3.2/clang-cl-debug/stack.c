// Koka generated module: "stack", koka version: 2.3.2, platform: 64-bit
#include "stack.h"

kk_std_core_types__maybe kk_stack_peek(kk_stack__stack st, kk_context_t* _ctx) { /* forall<a> (st : stack<a>) -> maybe<a> */ 
  if (kk_stack__is_Empty(st)) {
    return kk_std_core_types__new_Nothing(_ctx);
  }
  {
    struct kk_stack_Top* _con130 = kk_stack__as_Top(st);
    kk_box_t x = _con130->head;
    kk_stack__stack _pat1 = _con130->tail;
    if (kk_likely(kk_stack__stack_is_unique(st))) {
      kk_stack__stack_drop(_pat1, _ctx);
      kk_stack__stack_free(st);
    }
    else {
      kk_box_dup(x);
      kk_stack__stack_decref(st, _ctx);
    }
    return kk_std_core_types__new_Just(x, _ctx);
  }
}

kk_std_core_types__tuple2_ kk_stack_pop(kk_stack__stack st, kk_context_t* _ctx) { /* forall<a> (st : stack<a>) -> (maybe<a>, stack<a>) */ 
  if (kk_stack__is_Empty(st)) {
    return kk_std_core_types__new_dash__lp__comma__rp_(kk_std_core_types__maybe_box(kk_std_core_types__new_Nothing(_ctx), _ctx), kk_stack__stack_box(kk_stack__new_Empty(_ctx), _ctx), _ctx);
  }
  {
    struct kk_stack_Top* _con131 = kk_stack__as_Top(st);
    kk_box_t x = _con131->head;
    kk_stack__stack xs = _con131->tail;
    if (kk_likely(kk_stack__stack_is_unique(st))) {
      kk_stack__stack_free(st);
    }
    else {
      kk_box_dup(x);
      kk_stack__stack_dup(xs);
      kk_stack__stack_decref(st, _ctx);
    }
    kk_box_t _x132;
    kk_std_core_types__maybe _x133 = kk_std_core_types__new_Just(x, _ctx); /*maybe<1034>*/
    _x132 = kk_std_core_types__maybe_box(_x133, _ctx); /*1004*/
    return kk_std_core_types__new_dash__lp__comma__rp_(_x132, kk_stack__stack_box(xs, _ctx), _ctx);
  }
}

// initialization
void kk_stack__init(kk_context_t* _ctx){
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
void kk_stack__done(kk_context_t* _ctx){
  static bool _kk_done = false;
  if (_kk_done) return;
  _kk_done = true;
  #if defined(KK_CUSTOM_DONE)
    KK_CUSTOM_DONE (_ctx);
  #endif
  kk_std_core__done(_ctx);
  kk_std_core_types__done(_ctx);
}
