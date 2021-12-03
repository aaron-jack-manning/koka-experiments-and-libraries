// Koka generated module: "std/core/types", koka version: 2.3.2, platform: 64-bit
#include "std_core_types.h"

kk_std_core_types__tuple2_ kk_std_core_types__copy_1(kk_std_core_types__tuple2_ _this, kk_std_core_types__optional fst0, kk_std_core_types__optional snd0, kk_context_t* _ctx) { /* forall<a,b> ((a, b), fst : optional<a>, snd : optional<b>) -> (a, b) */ 
  kk_box_t _x2469;
  if (kk_std_core_types__is_Optional(fst0)) {
    kk_box_t _fst_407 = fst0._cons.Optional.value;
    _x2469 = _fst_407; /*428*/
  }
  else {
    kk_box_t _x = _this.fst;
    kk_box_dup(_x);
    _x2469 = _x; /*428*/
  }
  kk_box_t _x2470;
  if (kk_std_core_types__is_Optional(snd0)) {
    kk_box_t _snd_415 = snd0._cons.Optional.value;
    kk_std_core_types__tuple2__drop(_this, _ctx);
    _x2470 = _snd_415; /*429*/
  }
  else {
    kk_box_t _x0 = _this.snd;
    kk_box_dup(_x0);
    kk_std_core_types__tuple2__drop(_this, _ctx);
    _x2470 = _x0; /*429*/
  }
  return kk_std_core_types__new_dash__lp__comma__rp_(_x2469, _x2470, _ctx);
}

kk_std_core_types__tuple3_ kk_std_core_types__copy_2(kk_std_core_types__tuple3_ _this, kk_std_core_types__optional fst0, kk_std_core_types__optional snd0, kk_std_core_types__optional thd0, kk_context_t* _ctx) { /* forall<a,b,c> ((a, b, c), fst : optional<a>, snd : optional<b>, thd : optional<c>) -> (a, b, c) */ 
  kk_box_t _x2471;
  if (kk_std_core_types__is_Optional(fst0)) {
    kk_box_t _fst_589 = fst0._cons.Optional.value;
    _x2471 = _fst_589; /*652*/
  }
  else {
    kk_box_t _x = _this.fst;
    kk_box_dup(_x);
    _x2471 = _x; /*652*/
  }
  kk_box_t _x2472;
  if (kk_std_core_types__is_Optional(snd0)) {
    kk_box_t _snd_626 = snd0._cons.Optional.value;
    _x2472 = _snd_626; /*653*/
  }
  else {
    kk_box_t _x0 = _this.snd;
    kk_box_dup(_x0);
    _x2472 = _x0; /*653*/
  }
  kk_box_t _x2473;
  if (kk_std_core_types__is_Optional(thd0)) {
    kk_box_t _thd_635 = thd0._cons.Optional.value;
    kk_std_core_types__tuple3__drop(_this, _ctx);
    _x2473 = _thd_635; /*654*/
  }
  else {
    kk_box_t _x1 = _this.thd;
    kk_box_dup(_x1);
    kk_std_core_types__tuple3__drop(_this, _ctx);
    _x2473 = _x1; /*654*/
  }
  return kk_std_core_types__new_dash__lp__comma__comma__rp_(_x2471, _x2472, _x2473, _ctx);
}

kk_std_core_types__tuple4_ kk_std_core_types__copy_3(kk_std_core_types__tuple4_ _this, kk_std_core_types__optional fst0, kk_std_core_types__optional snd0, kk_std_core_types__optional thd0, kk_std_core_types__optional field40, kk_context_t* _ctx) { /* forall<a,b,c,d> ((a, b, c, d), fst : optional<a>, snd : optional<b>, thd : optional<c>, field4 : optional<d>) -> (a, b, c, d) */ 
  kk_box_t _x2478;
  if (kk_std_core_types__is_Optional(fst0)) {
    kk_box_t _fst_962 = fst0._cons.Optional.value;
    _x2478 = _fst_962; /*1098*/
  }
  else {
    struct kk_std_core_types__Tuple4_* _con2479 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(_this);
    kk_box_t _x = _con2479->fst;
    kk_box_dup(_x);
    _x2478 = _x; /*1098*/
  }
  kk_box_t _x2480;
  if (kk_std_core_types__is_Optional(snd0)) {
    kk_box_t _snd_1020 = snd0._cons.Optional.value;
    _x2480 = _snd_1020; /*1099*/
  }
  else {
    struct kk_std_core_types__Tuple4_* _con2481 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(_this);
    kk_box_t _x0 = _con2481->snd;
    kk_box_dup(_x0);
    _x2480 = _x0; /*1099*/
  }
  kk_box_t _x2482;
  if (kk_std_core_types__is_Optional(thd0)) {
    kk_box_t _thd_1067 = thd0._cons.Optional.value;
    _x2482 = _thd_1067; /*1100*/
  }
  else {
    struct kk_std_core_types__Tuple4_* _con2483 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(_this);
    kk_box_t _x1 = _con2483->thd;
    kk_box_dup(_x1);
    _x2482 = _x1; /*1100*/
  }
  kk_box_t _x2484;
  if (kk_std_core_types__is_Optional(field40)) {
    kk_box_t _field4_1077 = field40._cons.Optional.value;
    kk_std_core_types__tuple4__dropn(_this, ((int32_t)KI32(4)), _ctx);
    _x2484 = _field4_1077; /*1101*/
  }
  else {
    struct kk_std_core_types__Tuple4_* _con2485 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(_this);
    kk_box_t _pat03 = _con2485->fst;
    kk_box_t _pat14 = _con2485->snd;
    kk_box_t _pat23 = _con2485->thd;
    kk_box_t _x2 = _con2485->field4;
    if (kk_likely(kk_std_core_types__tuple4__is_unique(_this))) {
      kk_box_drop(_pat23, _ctx);
      kk_box_drop(_pat14, _ctx);
      kk_box_drop(_pat03, _ctx);
      kk_std_core_types__tuple4__free(_this);
    }
    else {
      kk_box_dup(_x2);
      kk_std_core_types__tuple4__decref(_this, _ctx);
    }
    _x2484 = _x2; /*1101*/
  }
  return kk_std_core_types__new_dash__lp__comma__comma__comma__rp_(kk_reuse_null, _x2478, _x2480, _x2482, _x2484, _ctx);
}

kk_std_core_types___lp__comma__comma__comma__comma__rp_ kk_std_core_types__copy_4(kk_std_core_types___lp__comma__comma__comma__comma__rp_ _this, kk_std_core_types__optional fst0, kk_std_core_types__optional snd0, kk_std_core_types__optional thd0, kk_std_core_types__optional field40, kk_std_core_types__optional field50, kk_context_t* _ctx) { /* forall<a,b,c,d,a1> ((a, b, c, d, a1), fst : optional<a>, snd : optional<b>, thd : optional<c>, field4 : optional<d>, field5 : optional<a1>) -> (a, b, c, d, a1) */ 
  kk_box_t _x2491;
  if (kk_std_core_types__is_Optional(fst0)) {
    kk_box_t _fst_1621 = fst0._cons.Optional.value;
    _x2491 = _fst_1621; /*1870*/
  }
  else {
    struct kk_std_core_types__lp__comma__comma__comma__comma__rp_* _con2492 = kk_std_core_types__as_dash__lp__comma__comma__comma__comma__rp_(_this);
    kk_box_t _x = _con2492->fst;
    kk_box_dup(_x);
    _x2491 = _x; /*1870*/
  }
  kk_box_t _x2493;
  if (kk_std_core_types__is_Optional(snd0)) {
    kk_box_t _snd_1705 = snd0._cons.Optional.value;
    _x2493 = _snd_1705; /*1871*/
  }
  else {
    struct kk_std_core_types__lp__comma__comma__comma__comma__rp_* _con2494 = kk_std_core_types__as_dash__lp__comma__comma__comma__comma__rp_(_this);
    kk_box_t _x0 = _con2494->snd;
    kk_box_dup(_x0);
    _x2493 = _x0; /*1871*/
  }
  kk_box_t _x2495;
  if (kk_std_core_types__is_Optional(thd0)) {
    kk_box_t _thd_1777 = thd0._cons.Optional.value;
    _x2495 = _thd_1777; /*1872*/
  }
  else {
    struct kk_std_core_types__lp__comma__comma__comma__comma__rp_* _con2496 = kk_std_core_types__as_dash__lp__comma__comma__comma__comma__rp_(_this);
    kk_box_t _x1 = _con2496->thd;
    kk_box_dup(_x1);
    _x2495 = _x1; /*1872*/
  }
  kk_box_t _x2497;
  if (kk_std_core_types__is_Optional(field40)) {
    kk_box_t _field4_1834 = field40._cons.Optional.value;
    _x2497 = _field4_1834; /*1873*/
  }
  else {
    struct kk_std_core_types__lp__comma__comma__comma__comma__rp_* _con2498 = kk_std_core_types__as_dash__lp__comma__comma__comma__comma__rp_(_this);
    kk_box_t _x2 = _con2498->field4;
    kk_box_dup(_x2);
    _x2497 = _x2; /*1873*/
  }
  kk_box_t _x2499;
  if (kk_std_core_types__is_Optional(field50)) {
    kk_box_t _field5_1845 = field50._cons.Optional.value;
    kk_std_core_types___lp__comma__comma__comma__comma__rp__dropn(_this, ((int32_t)KI32(5)), _ctx);
    _x2499 = _field5_1845; /*1874*/
  }
  else {
    struct kk_std_core_types__lp__comma__comma__comma__comma__rp_* _con2500 = kk_std_core_types__as_dash__lp__comma__comma__comma__comma__rp_(_this);
    kk_box_t _pat04 = _con2500->fst;
    kk_box_t _pat15 = _con2500->snd;
    kk_box_t _pat24 = _con2500->thd;
    kk_box_t _pat34 = _con2500->field4;
    kk_box_t _x3 = _con2500->field5;
    if (kk_likely(kk_std_core_types___lp__comma__comma__comma__comma__rp__is_unique(_this))) {
      kk_box_drop(_pat34, _ctx);
      kk_box_drop(_pat24, _ctx);
      kk_box_drop(_pat15, _ctx);
      kk_box_drop(_pat04, _ctx);
      kk_std_core_types___lp__comma__comma__comma__comma__rp__free(_this);
    }
    else {
      kk_box_dup(_x3);
      kk_std_core_types___lp__comma__comma__comma__comma__rp__decref(_this, _ctx);
    }
    _x2499 = _x3; /*1874*/
  }
  return kk_std_core_types__new_dash__lp__comma__comma__comma__comma__rp_(kk_reuse_null, _x2491, _x2493, _x2495, _x2497, _x2499, _ctx);
}

kk_std_core_types__hbox kk_std_core_types__copy_6(kk_std_core_types__hbox _this, kk_std_core_types__optional unhbox0, kk_context_t* _ctx) { /* forall<a> (hbox<a>, unhbox : optional<a>) -> hbox<a> */ 
  kk_box_t _x2503;
  if (kk_std_core_types__is_Optional(unhbox0)) {
    kk_box_t _unhbox_2118 = unhbox0._cons.Optional.value;
    kk_std_core_types__hbox_dropn(_this, ((int32_t)KI32(1)), _ctx);
    _x2503 = _unhbox_2118; /*2127*/
  }
  else {
    struct kk_std_core_types_Hbox* _con2504 = kk_std_core_types__as_Hbox(_this);
    kk_box_t _x = _con2504->unhbox;
    if (kk_likely(kk_std_core_types__hbox_is_unique(_this))) {
      kk_std_core_types__hbox_free(_this);
    }
    else {
      kk_box_dup(_x);
      kk_std_core_types__hbox_decref(_this, _ctx);
    }
    _x2503 = _x; /*2127*/
  }
  return kk_std_core_types__new_Hbox(kk_reuse_null, _x2503, _ctx);
}
 
// Generated by type inference and later refined into one of the `open` variants in `std/core/hnd`.

kk_box_t kk_std_core_types__open(kk_box_t x, kk_context_t* _ctx) { /* forall<e,e1,a,b> (x : a) -> e1 b */ 
  return (x);
}

kk_box_t kk_std_core_types_keep(kk_box_t x, kk_context_t* _ctx) { /* forall<a> (x : a) -> a */ 
  return x;
}

kk_reuse_t kk_std_core_types_no_reuse(kk_context_t* _ctx) { /* () -> reuse */ 
  return NULL;
}
 
// If a heap effect is unobservable, the heap effect can be erased by using the `run` fun.
// See also: _State in Haskell, by Simon Peyton Jones and John Launchbury_.

kk_box_t kk_std_core_types_run(kk_function_t action, kk_context_t* _ctx) { /* forall<e,a> (action : forall<h> () -> <alloc<h>,read<h>,write<h>|e> a) -> e a */ 
  return (kk_function_call(kk_box_t,(kk_function_t,kk_context_t*),action,(action,kk_context())));
}

// initialization
void kk_std_core_types__init(kk_context_t* _ctx){
  static bool _kk_initialized = false;
  if (_kk_initialized) return;
  _kk_initialized = true;
  #if defined(KK_CUSTOM_INIT)
    KK_CUSTOM_INIT (_ctx);
  #endif
}

// termination
void kk_std_core_types__done(kk_context_t* _ctx){
  static bool _kk_done = false;
  if (_kk_done) return;
  _kk_done = true;
  #if defined(KK_CUSTOM_DONE)
    KK_CUSTOM_DONE (_ctx);
  #endif
}
