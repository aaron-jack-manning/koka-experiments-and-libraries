// Koka generated module: "display", koka version: 2.3.2, platform: 64-bit
#include "display.h"

kk_unit_t kk_display_println(kk_std_core__list ls, kk_context_t* _ctx) { /* (ls : list<int>) -> console () */ 
  kk_unit_t __1 = kk_Unit;
  if (kk_std_core__is_Nil(ls)) {
    
  }
  {
    struct kk_std_core_Cons* _con94 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x89 = _con94->head;
    kk_std_core__list _pat1 = _con94->tail;
    if (kk_std_core__is_Nil(_pat1)) {
      kk_integer_t x = kk_integer_unbox(_box_x89);
      if (kk_likely(kk_std_core__list_is_unique(ls))) {
        kk_std_core__list_free(ls);
      }
      else {
        kk_integer_dup(x);
        kk_std_core__list_decref(ls, _ctx);
      }
      kk_string_t _x96 = kk_std_core_show(x, _ctx); /*string*/
      kk_std_core_printsln(_x96, _ctx);
      goto _match93;
    }
  }
  {
    struct kk_std_core_Cons* _con97 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x90 = _con97->head;
    kk_std_core__list xs = _con97->tail;
    kk_integer_t x0 = kk_integer_unbox(_box_x90);
    if (kk_likely(kk_std_core__list_is_unique(ls))) {
      kk_std_core__list_free(ls);
    }
    else {
      kk_integer_dup(x0);
      kk_std_core__list_dup(xs);
      kk_std_core__list_decref(ls, _ctx);
    }
    kk_unit_t __ = kk_Unit;
    kk_string_t _x99 = kk_std_core_show(x0, _ctx); /*string*/
    kk_std_core_prints(_x99, _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_string_t _x100;
    kk_define_string_literal(, _s101, 2, ", ")
    _x100 = kk_string_dup(_s101); /*string*/
    kk_std_core_prints(_x100, _ctx);
    kk_display_println(xs, _ctx);
  }
  _match93: ;
  kk_Unit; return kk_Unit;
}

// initialization
void kk_display__init(kk_context_t* _ctx){
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
void kk_display__done(kk_context_t* _ctx){
  static bool _kk_done = false;
  if (_kk_done) return;
  _kk_done = true;
  #if defined(KK_CUSTOM_DONE)
    KK_CUSTOM_DONE (_ctx);
  #endif
  kk_std_core__done(_ctx);
  kk_std_core_types__done(_ctx);
}
