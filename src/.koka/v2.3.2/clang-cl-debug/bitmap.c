// Koka generated module: "bitmap", koka version: 2.3.2, platform: 64-bit
#include "bitmap.h"

kk_bitmap__colour kk_bitmap__copy(kk_bitmap__colour _this, kk_std_core_types__optional red0, kk_std_core_types__optional green0, kk_std_core_types__optional blue0, kk_context_t* _ctx) { /* (colour, red : optional<int>, green : optional<int>, blue : optional<int>) -> colour */ 
  kk_integer_t _x3426;
  if (kk_std_core_types__is_Optional(red0)) {
    kk_box_t _box_x2873 = red0._cons.Optional.value;
    kk_integer_t _red_59 = kk_integer_unbox(_box_x2873);
    kk_integer_dup(_red_59);
    kk_std_core_types__optional_drop(red0, _ctx);
    _x3426 = _red_59; /*int*/
    goto _match3427;
  }
  {
    kk_integer_t _x = _this.red;
    kk_integer_dup(_x);
    _x3426 = _x; /*int*/
  }
  _match3427: ;
  kk_integer_t _x3429;
  if (kk_std_core_types__is_Optional(green0)) {
    kk_box_t _box_x2874 = green0._cons.Optional.value;
    kk_integer_t _green_65 = kk_integer_unbox(_box_x2874);
    kk_integer_dup(_green_65);
    kk_std_core_types__optional_drop(green0, _ctx);
    _x3429 = _green_65; /*int*/
    goto _match3430;
  }
  {
    kk_integer_t _x0 = _this.green;
    kk_integer_dup(_x0);
    _x3429 = _x0; /*int*/
  }
  _match3430: ;
  kk_integer_t _x3432;
  if (kk_std_core_types__is_Optional(blue0)) {
    kk_box_t _box_x2875 = blue0._cons.Optional.value;
    kk_integer_t _blue_71 = kk_integer_unbox(_box_x2875);
    kk_integer_dup(_blue_71);
    kk_std_core_types__optional_drop(blue0, _ctx);
    kk_bitmap__colour_drop(_this, _ctx);
    _x3432 = _blue_71; /*int*/
    goto _match3433;
  }
  {
    kk_integer_t _x1 = _this.blue;
    kk_integer_dup(_x1);
    kk_bitmap__colour_drop(_this, _ctx);
    _x3432 = _x1; /*int*/
  }
  _match3433: ;
  return kk_bitmap__new_Colour(_x3426, _x3429, _x3432, _ctx);
}
 
// lifted

kk_std_core__list kk_bitmap__lift2648_bytes(kk_integer_t current, kk_integer_t base, kk_context_t* _ctx) { /* (current : int, base : int) -> div list<int> */ 
  kk_std_core_types__tuple2_ _match_3425;
  kk_integer_t _x3435 = kk_integer_dup(base); /*int*/
  _match_3425 = kk_integer_div_mod_tuple(current,_x3435,kk_context()); /*(int, int)*/
  {
    kk_box_t _box_x2876 = _match_3425.fst;
    kk_box_t _box_x2877 = _match_3425.snd;
    kk_integer_t quotient = kk_integer_unbox(_box_x2876);
    kk_integer_t remainder = kk_integer_unbox(_box_x2877);
    kk_integer_dup(quotient);
    kk_integer_dup(remainder);
    kk_std_core_types__tuple2__drop(_match_3425, _ctx);
    if (kk_integer_eq_borrow(quotient, kk_integer_from_small(0), _ctx)) {
      kk_integer_drop(quotient, _ctx);
      kk_integer_drop(base, _ctx);
      kk_std_core__list _x3438 = kk_std_core__new_Cons(kk_reuse_null, kk_integer_box(remainder), kk_std_core__new_Nil(_ctx), _ctx); /*list<1009>*/
      return kk_std_core_append(_x3438, kk_std_core__new_Nil(_ctx), _ctx);
    }
    {
      kk_std_core__list ys0_2610 = kk_bitmap__lift2648_bytes(quotient, base, _ctx); /*list<int>*/;
      kk_std_core__list _x3439 = kk_std_core__new_Cons(kk_reuse_null, kk_integer_box(remainder), kk_std_core__new_Nil(_ctx), _ctx); /*list<1009>*/
      return kk_std_core_append(_x3439, ys0_2610, _ctx);
    }
  }
}


// lift anonymous function
struct kk_bitmap_bytes_fun3444__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap_bytes_fun3444(kk_function_t _fself, kk_integer_t _b_2889, kk_context_t* _ctx);
static kk_function_t kk_bitmap_new_bytes_fun3444(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap_bytes_fun3444, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap_bytes_fun3444(kk_function_t _fself, kk_integer_t _b_2889, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_integer_drop(_b_2889, _ctx);
  return kk_integer_box(kk_integer_from_small(0));
}

kk_std_core__list kk_bitmap_bytes(kk_integer_t number, kk_context_t* _ctx) { /* (number : int) -> div list<int> */ 
  kk_std_core__list digits = kk_bitmap__lift2648_bytes(number, kk_integer_from_small(256), _ctx); /*list<int>*/;
  kk_integer_t padding;
  kk_integer_t _x3440;
  kk_integer_t _x3441;
  kk_std_core__list _x3442 = kk_std_core__list_dup(digits); /*list<int>*/
  _x3441 = kk_std_core__lift17270_length_1(_x3442, kk_integer_from_small(0), _ctx); /*int*/
  _x3440 = kk_integer_mod(_x3441,(kk_integer_from_small(4)),kk_context()); /*int*/
  padding = kk_integer_sub((kk_integer_from_small(4)),_x3440,kk_context()); /*int*/
  kk_std_core__list _x3443 = kk_std_core_list_2(kk_integer_from_small(1), padding, kk_bitmap_new_bytes_fun3444(_ctx), _ctx); /*list<1001>*/
  return kk_std_core_append(digits, _x3443, _ctx);
}


// lift anonymous function
struct kk_bitmap_colour_to_bytes_fun3445__t {
  struct kk_function_s _base;
  kk_bitmap__colour colour;
};
static kk_box_t kk_bitmap_colour_to_bytes_fun3445(kk_function_t _fself, kk_ssize_t _b_2896, kk_context_t* _ctx);
static kk_function_t kk_bitmap_new_colour_to_bytes_fun3445(kk_bitmap__colour colour, kk_context_t* _ctx) {
  struct kk_bitmap_colour_to_bytes_fun3445__t* _self = kk_function_alloc_as(struct kk_bitmap_colour_to_bytes_fun3445__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap_colour_to_bytes_fun3445, kk_context());
  _self->colour = colour;
  return &_self->_base;
}

static kk_box_t kk_bitmap_colour_to_bytes_fun3445(kk_function_t _fself, kk_ssize_t _b_2896, kk_context_t* _ctx) {
  struct kk_bitmap_colour_to_bytes_fun3445__t* _self = kk_function_as(struct kk_bitmap_colour_to_bytes_fun3445__t*, _fself);
  kk_bitmap__colour colour = _self->colour; /* bitmap/colour */
  kk_drop_match(_self, {kk_bitmap__colour_dup(colour);}, {}, _ctx)
  kk_integer_t _x3446;
  kk_integer_t i0_2646 = kk_integer_from_ssize_t(_b_2896,kk_context()); /*int*/;
  if (kk_integer_eq_borrow(i0_2646, kk_integer_from_small(0), _ctx)) {
    kk_integer_drop(i0_2646, _ctx);
    {
      kk_integer_t _x = colour.blue;
      kk_integer_dup(_x);
      kk_bitmap__colour_drop(colour, _ctx);
      _x3446 = _x; /*int*/
      goto _match3447;
    }
  }
  if (kk_integer_eq_borrow(i0_2646, kk_integer_from_small(1), _ctx)) {
    kk_integer_drop(i0_2646, _ctx);
    {
      kk_integer_t _x0 = colour.green;
      kk_integer_dup(_x0);
      kk_bitmap__colour_drop(colour, _ctx);
      _x3446 = _x0; /*int*/
      goto _match3447;
    }
  }
  if (kk_integer_eq_borrow(i0_2646, kk_integer_from_small(2), _ctx)) {
    kk_integer_drop(i0_2646, _ctx);
    {
      kk_integer_t _x1 = colour.red;
      kk_integer_dup(_x1);
      kk_bitmap__colour_drop(colour, _ctx);
      _x3446 = _x1; /*int*/
      goto _match3447;
    }
  }
  {
    kk_integer_drop(i0_2646, _ctx);
    kk_bitmap__colour_drop(colour, _ctx);
    _x3446 = kk_integer_from_small(0); /*int*/
  }
  _match3447: ;
  return kk_integer_box(_x3446);
}

kk_vector_t kk_bitmap_colour_to_bytes(kk_bitmap__colour colour, kk_context_t* _ctx) { /* (colour : colour) -> vector<int> */ 
  kk_ssize_t _b_2897_2894 = ((kk_ssize_t)3); /*ssize_t*/;
  return kk_std_core_vector_initz(_b_2897_2894, kk_bitmap_new_colour_to_bytes_fun3445(colour, _ctx), _ctx);
}

kk_integer_t kk_bitmap_round_up_to_four(kk_integer_t number, kk_context_t* _ctx) { /* (number : int) -> int */ 
  bool _match_3421;
  kk_integer_t _brw_3423;
  kk_integer_t _x3448 = kk_integer_dup(number); /*int*/
  _brw_3423 = kk_integer_mod(_x3448,(kk_integer_from_small(4)),kk_context()); /*int*/
  kk_integer_t _brw_3422 = kk_integer_from_small(0); /*int*/;
  bool _brw_3424 = kk_integer_eq_borrow(_brw_3423,_brw_3422,kk_context()); /*bool*/;
  kk_integer_drop(_brw_3423, _ctx);
  kk_integer_drop(_brw_3422, _ctx);
  _match_3421 = _brw_3424; /*bool*/
  if (_match_3421) {
    return number;
  }
  {
    kk_integer_t _x3449;
    kk_integer_t _x3450 = kk_integer_dup(number); /*int*/
    _x3449 = kk_integer_add(_x3450,(kk_integer_from_small(4)),kk_context()); /*int*/
    kk_integer_t _x3451 = kk_integer_mod(number,(kk_integer_from_small(4)),kk_context()); /*int*/
    return kk_integer_sub(_x3449,_x3451,kk_context());
  }
}
 
// monadic lift

kk_unit_t kk_bitmap__mlift2714_get_dib_header(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_unit_t wild__4, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, i : int, print-resolution-bytes : list<int>, wild_4 : ()) -> <local<h>,exn,div> () */ 
  kk_integer_t _b_2906_2903;
  kk_integer_t _x3452 = kk_integer_dup(i); /*int*/
  _b_2906_2903 = kk_integer_add((kk_integer_from_small(28)),_x3452,kk_context()); /*int*/
  kk_std_core_types__maybe _b_2908_2900 = kk_std_core__lp__lb__rb__2_rp_(print_resolution_bytes, i, _ctx); /*maybe<int>*/;
  kk_integer_t _b_2907_2904;
  kk_box_t _x3453 = kk_std_core_maybe_1(_b_2908_2900, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_2907_2904 = kk_integer_unbox(_x3453); /*int*/
  kk_unit_t _brw_3420 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_b_2906_2903,(kk_integer_box(_b_2907_2904)),kk_context());
  kk_ref_drop(dib_header, _ctx);
  kk_integer_drop(_b_2906_2903, _ctx);
  _brw_3420; return kk_Unit;
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2715_get_dib_header_fun3458__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_integer_t i;
  kk_std_core__list print_resolution_bytes;
};
static kk_unit_t kk_bitmap__mlift2715_get_dib_header_fun3458(kk_function_t _fself, kk_unit_t wild__4, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2715_get_dib_header_fun3458(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2715_get_dib_header_fun3458__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2715_get_dib_header_fun3458__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2715_get_dib_header_fun3458, kk_context());
  _self->dib_header = dib_header;
  _self->i = i;
  _self->print_resolution_bytes = print_resolution_bytes;
  return &_self->_base;
}

static kk_unit_t kk_bitmap__mlift2715_get_dib_header_fun3458(kk_function_t _fself, kk_unit_t wild__4, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2715_get_dib_header_fun3458__t* _self = kk_function_as(struct kk_bitmap__mlift2715_get_dib_header_fun3458__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<1011,vector<int>> */
  kk_integer_t i = _self->i; /* int */
  kk_std_core__list print_resolution_bytes = _self->print_resolution_bytes; /* list<int> */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_integer_dup(i);kk_std_core__list_dup(print_resolution_bytes);}, {}, _ctx)
  kk_integer_t _b_2926_2923;
  kk_integer_t _x3459 = kk_integer_dup(i); /*int*/
  _b_2926_2923 = kk_integer_add((kk_integer_from_small(28)),_x3459,kk_context()); /*int*/
  kk_std_core_types__maybe _b_2928_2920 = kk_std_core__lp__lb__rb__2_rp_(print_resolution_bytes, i, _ctx); /*maybe<int>*/;
  kk_integer_t _b_2927_2924;
  kk_box_t _x3460 = kk_std_core_maybe_1(_b_2928_2920, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_2927_2924 = kk_integer_unbox(_x3460); /*int*/
  kk_unit_t _brw_3418 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_b_2926_2923,(kk_integer_box(_b_2927_2924)),kk_context());
  kk_ref_drop(dib_header, _ctx);
  kk_integer_drop(_b_2926_2923, _ctx);
  return _brw_3418;
}


// lift anonymous function
struct kk_bitmap__mlift2715_get_dib_header_fun3462__t {
  struct kk_function_s _base;
  kk_function_t next_2748;
};
static kk_box_t kk_bitmap__mlift2715_get_dib_header_fun3462(kk_function_t _fself, kk_box_t _b_2931, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2715_get_dib_header_fun3462(kk_function_t next_2748, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2715_get_dib_header_fun3462__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2715_get_dib_header_fun3462__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2715_get_dib_header_fun3462, kk_context());
  _self->next_2748 = next_2748;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2715_get_dib_header_fun3462(kk_function_t _fself, kk_box_t _b_2931, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2715_get_dib_header_fun3462__t* _self = kk_function_as(struct kk_bitmap__mlift2715_get_dib_header_fun3462__t*, _fself);
  kk_function_t next_2748 = _self->next_2748; /* (()) -> <local<1011>,exn,div> () */
  kk_drop_match(_self, {kk_function_dup(next_2748);}, {}, _ctx)
  kk_unit_t _x3463 = kk_Unit;
  kk_unit_t _x3464 = kk_Unit;
  kk_unit_unbox(_b_2931);
  kk_function_call(kk_unit_t, (kk_function_t, kk_unit_t, kk_context_t*), next_2748, (next_2748, _x3464, _ctx));
  return kk_unit_box(_x3463);
}

kk_unit_t kk_bitmap__mlift2715_get_dib_header(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_unit_t wild__3, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, i : int, print-resolution-bytes : list<int>, wild_3 : ()) -> <local<h>,exn,div> () */ 
  kk_integer_t _b_2916_2913;
  kk_integer_t _x3454 = kk_integer_dup(i); /*int*/
  _b_2916_2913 = kk_integer_add((kk_integer_from_small(24)),_x3454,kk_context()); /*int*/
  kk_std_core_types__maybe _b_2918_2910;
  kk_std_core__list _x3455 = kk_std_core__list_dup(print_resolution_bytes); /*list<int>*/
  kk_integer_t _x3456 = kk_integer_dup(i); /*int*/
  _b_2918_2910 = kk_std_core__lp__lb__rb__2_rp_(_x3455, _x3456, _ctx); /*maybe<int>*/
  kk_integer_t _b_2917_2914;
  kk_box_t _x3457 = kk_std_core_maybe_1(_b_2918_2910, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_2917_2914 = kk_integer_unbox(_x3457); /*int*/
  kk_unit_t x_2747 = kk_Unit;
  kk_unit_t _brw_3419 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_b_2916_2913,(kk_integer_box(_b_2917_2914)),kk_context());
  kk_integer_drop(_b_2916_2913, _ctx);
  _brw_3419;
  kk_function_t next_2748 = kk_bitmap__new_mlift2715_get_dib_header_fun3458(dib_header, i, print_resolution_bytes, _ctx); /*(()) -> <local<1011>,exn,div> ()*/;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3461 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2715_get_dib_header_fun3462(next_2748, _ctx), _ctx); /*1002*/
    kk_unit_unbox(_x3461); return kk_Unit;
  }
  {
    kk_function_call(kk_unit_t, (kk_function_t, kk_unit_t, kk_context_t*), next_2748, (next_2748, x_2747, _ctx)); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2716_get_dib_header_fun3469__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_integer_t i;
  kk_std_core__list print_resolution_bytes;
};
static kk_box_t kk_bitmap__mlift2716_get_dib_header_fun3469(kk_function_t _fself, kk_box_t _b_2944, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2716_get_dib_header_fun3469(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2716_get_dib_header_fun3469__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2716_get_dib_header_fun3469__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2716_get_dib_header_fun3469, kk_context());
  _self->dib_header = dib_header;
  _self->i = i;
  _self->print_resolution_bytes = print_resolution_bytes;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2716_get_dib_header_fun3469(kk_function_t _fself, kk_box_t _b_2944, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2716_get_dib_header_fun3469__t* _self = kk_function_as(struct kk_bitmap__mlift2716_get_dib_header_fun3469__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<1011,vector<int>> */
  kk_integer_t i = _self->i; /* int */
  kk_std_core__list print_resolution_bytes = _self->print_resolution_bytes; /* list<int> */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_integer_dup(i);kk_std_core__list_dup(print_resolution_bytes);}, {}, _ctx)
  kk_unit_t _x3470 = kk_Unit;
  kk_unit_t _x3471 = kk_Unit;
  kk_unit_unbox(_b_2944);
  kk_bitmap__mlift2715_get_dib_header(dib_header, i, print_resolution_bytes, _x3471, _ctx);
  return kk_unit_box(_x3470);
}

kk_unit_t kk_bitmap__mlift2716_get_dib_header(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_std_core__list vertical_resolution_bytes, kk_unit_t wild__2, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, i : int, print-resolution-bytes : list<int>, vertical-resolution-bytes : list<int>, wild_2 : ()) -> <local<h>,exn,div> () */ 
  kk_integer_t _b_2939_2936;
  kk_integer_t _x3465 = kk_integer_dup(i); /*int*/
  _b_2939_2936 = kk_integer_add((kk_integer_from_small(8)),_x3465,kk_context()); /*int*/
  kk_std_core_types__maybe _b_2941_2933;
  kk_integer_t _x3466 = kk_integer_dup(i); /*int*/
  _b_2941_2933 = kk_std_core__lp__lb__rb__2_rp_(vertical_resolution_bytes, _x3466, _ctx); /*maybe<int>*/
  kk_integer_t _b_2940_2937;
  kk_box_t _x3467 = kk_std_core_maybe_1(_b_2941_2933, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_2940_2937 = kk_integer_unbox(_x3467); /*int*/
  kk_unit_t x_2753 = kk_Unit;
  kk_unit_t _brw_3416 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_b_2939_2936,(kk_integer_box(_b_2940_2937)),kk_context());
  kk_integer_drop(_b_2939_2936, _ctx);
  _brw_3416;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3468 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2716_get_dib_header_fun3469(dib_header, i, print_resolution_bytes, _ctx), _ctx); /*1002*/
    kk_unit_unbox(_x3468); return kk_Unit;
  }
  {
    kk_bitmap__mlift2715_get_dib_header(dib_header, i, print_resolution_bytes, x_2753, _ctx); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2718_get_dib_header_fun3474__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap__mlift2718_get_dib_header_fun3474(kk_function_t _fself, kk_box_t _b_2951, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2718_get_dib_header_fun3474(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap__mlift2718_get_dib_header_fun3474, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap__mlift2718_get_dib_header_fun3474(kk_function_t _fself, kk_box_t _b_2951, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core__list _x3475;
  kk_integer_t _x3476 = kk_integer_unbox(_b_2951); /*int*/
  _x3475 = kk_bitmap_bytes(_x3476, _ctx); /*list<int>*/
  return kk_std_core__list_box(_x3475, _ctx);
}


// lift anonymous function
struct kk_bitmap__mlift2718_get_dib_header_fun3478__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap__mlift2718_get_dib_header_fun3478(kk_function_t _fself, kk_box_t _b_2956, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2718_get_dib_header_fun3478(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap__mlift2718_get_dib_header_fun3478, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap__mlift2718_get_dib_header_fun3478(kk_function_t _fself, kk_box_t _b_2956, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core__list _x3479;
  kk_integer_t _x3480 = kk_integer_unbox(_b_2956); /*int*/
  _x3479 = kk_bitmap_bytes(_x3480, _ctx); /*list<int>*/
  return kk_std_core__list_box(_x3479, _ctx);
}


// lift anonymous function
struct kk_bitmap__mlift2718_get_dib_header_fun3482__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap__mlift2718_get_dib_header_fun3482(kk_function_t _fself, kk_box_t _b_2961, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2718_get_dib_header_fun3482(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap__mlift2718_get_dib_header_fun3482, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap__mlift2718_get_dib_header_fun3482(kk_function_t _fself, kk_box_t _b_2961, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core__list _x3483;
  kk_integer_t _x3484 = kk_integer_unbox(_b_2961); /*int*/
  _x3483 = kk_bitmap_bytes(_x3484, _ctx); /*list<int>*/
  return kk_std_core__list_box(_x3483, _ctx);
}


// lift anonymous function
struct kk_bitmap__mlift2718_get_dib_header_fun3486__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_std_core__list horizontal_resolution_bytes;
  kk_std_core__list print_resolution_bytes;
  kk_std_core__list vertical_resolution_bytes;
};
static kk_unit_t kk_bitmap__mlift2718_get_dib_header_fun3486(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2718_get_dib_header_fun3486(kk_ref_t dib_header, kk_std_core__list horizontal_resolution_bytes, kk_std_core__list print_resolution_bytes, kk_std_core__list vertical_resolution_bytes, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2718_get_dib_header_fun3486__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2718_get_dib_header_fun3486__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2718_get_dib_header_fun3486, kk_context());
  _self->dib_header = dib_header;
  _self->horizontal_resolution_bytes = horizontal_resolution_bytes;
  _self->print_resolution_bytes = print_resolution_bytes;
  _self->vertical_resolution_bytes = vertical_resolution_bytes;
  return &_self->_base;
}



// lift anonymous function
struct kk_bitmap__mlift2718_get_dib_header_fun3491__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_integer_t i;
  kk_std_core__list print_resolution_bytes;
  kk_std_core__list vertical_resolution_bytes;
};
static kk_box_t kk_bitmap__mlift2718_get_dib_header_fun3491(kk_function_t _fself, kk_box_t _b_2975, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2718_get_dib_header_fun3491(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_std_core__list vertical_resolution_bytes, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2718_get_dib_header_fun3491__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2718_get_dib_header_fun3491__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2718_get_dib_header_fun3491, kk_context());
  _self->dib_header = dib_header;
  _self->i = i;
  _self->print_resolution_bytes = print_resolution_bytes;
  _self->vertical_resolution_bytes = vertical_resolution_bytes;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2718_get_dib_header_fun3491(kk_function_t _fself, kk_box_t _b_2975, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2718_get_dib_header_fun3491__t* _self = kk_function_as(struct kk_bitmap__mlift2718_get_dib_header_fun3491__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<1011,vector<int>> */
  kk_integer_t i = _self->i; /* int */
  kk_std_core__list print_resolution_bytes = _self->print_resolution_bytes; /* list<int> */
  kk_std_core__list vertical_resolution_bytes = _self->vertical_resolution_bytes; /* list<int> */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_integer_dup(i);kk_std_core__list_dup(print_resolution_bytes);kk_std_core__list_dup(vertical_resolution_bytes);}, {}, _ctx)
  kk_unit_t _x3492 = kk_Unit;
  kk_unit_t _x3493 = kk_Unit;
  kk_unit_unbox(_b_2975);
  kk_bitmap__mlift2716_get_dib_header(dib_header, i, print_resolution_bytes, vertical_resolution_bytes, _x3493, _ctx);
  return kk_unit_box(_x3492);
}
static kk_unit_t kk_bitmap__mlift2718_get_dib_header_fun3486(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2718_get_dib_header_fun3486__t* _self = kk_function_as(struct kk_bitmap__mlift2718_get_dib_header_fun3486__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<1011,vector<int>> */
  kk_std_core__list horizontal_resolution_bytes = _self->horizontal_resolution_bytes; /* list<int> */
  kk_std_core__list print_resolution_bytes = _self->print_resolution_bytes; /* list<int> */
  kk_std_core__list vertical_resolution_bytes = _self->vertical_resolution_bytes; /* list<int> */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_std_core__list_dup(horizontal_resolution_bytes);kk_std_core__list_dup(print_resolution_bytes);kk_std_core__list_dup(vertical_resolution_bytes);}, {}, _ctx)
  kk_integer_t _b_2970_2967;
  kk_integer_t _x3487 = kk_integer_dup(i); /*int*/
  _b_2970_2967 = kk_integer_add((kk_integer_from_small(4)),_x3487,kk_context()); /*int*/
  kk_std_core_types__maybe _b_2972_2964;
  kk_integer_t _x3488 = kk_integer_dup(i); /*int*/
  _b_2972_2964 = kk_std_core__lp__lb__rb__2_rp_(horizontal_resolution_bytes, _x3488, _ctx); /*maybe<int>*/
  kk_integer_t _b_2971_2968;
  kk_box_t _x3489 = kk_std_core_maybe_1(_b_2972_2964, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_2971_2968 = kk_integer_unbox(_x3489); /*int*/
  kk_unit_t x0_2757 = kk_Unit;
  kk_unit_t _brw_3414 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_b_2970_2967,(kk_integer_box(_b_2971_2968)),kk_context());
  kk_integer_drop(_b_2970_2967, _ctx);
  _brw_3414;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3490 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2718_get_dib_header_fun3491(dib_header, i, print_resolution_bytes, vertical_resolution_bytes, _ctx), _ctx); /*1002*/
    return kk_unit_unbox(_x3490);
  }
  {
    return kk_bitmap__mlift2716_get_dib_header(dib_header, i, print_resolution_bytes, vertical_resolution_bytes, x0_2757, _ctx);
  }
}


// lift anonymous function
struct kk_bitmap__mlift2718_get_dib_header_fun3495__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
};
static kk_box_t kk_bitmap__mlift2718_get_dib_header_fun3495(kk_function_t _fself, kk_box_t _b_2980, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2718_get_dib_header_fun3495(kk_ref_t dib_header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2718_get_dib_header_fun3495__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2718_get_dib_header_fun3495__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2718_get_dib_header_fun3495, kk_context());
  _self->dib_header = dib_header;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2718_get_dib_header_fun3495(kk_function_t _fself, kk_box_t _b_2980, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2718_get_dib_header_fun3495__t* _self = kk_function_as(struct kk_bitmap__mlift2718_get_dib_header_fun3495__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<1011,vector<int>> */
  kk_drop_match(_self, {kk_ref_dup(dib_header);}, {}, _ctx)
  kk_box_drop(_b_2980, _ctx);
  return (kk_ref_get(dib_header,kk_context()));
}

kk_vector_t kk_bitmap__mlift2718_get_dib_header(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_unit_t wild__1, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, dpcm : int, horizontal-resolution : int, vertical-resolution : int, wild_1 : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_std_core__list horizontal_resolution_bytes;
  kk_box_t _x3473 = kk_std_core_hnd__open_none1(kk_bitmap__new_mlift2718_get_dib_header_fun3474(_ctx), kk_integer_box(horizontal_resolution), _ctx); /*1002*/
  horizontal_resolution_bytes = kk_std_core__list_unbox(_x3473, _ctx); /*list<int>*/
  kk_std_core__list vertical_resolution_bytes;
  kk_box_t _x3477 = kk_std_core_hnd__open_none1(kk_bitmap__new_mlift2718_get_dib_header_fun3478(_ctx), kk_integer_box(vertical_resolution), _ctx); /*1002*/
  vertical_resolution_bytes = kk_std_core__list_unbox(_x3477, _ctx); /*list<int>*/
  kk_integer_t _x11_2710 = kk_integer_mul(dpcm,(kk_integer_from_small(100)),kk_context()); /*int*/;
  kk_std_core__list print_resolution_bytes;
  kk_box_t _x3481 = kk_std_core_hnd__open_none1(kk_bitmap__new_mlift2718_get_dib_header_fun3482(_ctx), kk_integer_box(_x11_2710), _ctx); /*1002*/
  print_resolution_bytes = kk_std_core__list_unbox(_x3481, _ctx); /*list<int>*/
  kk_unit_t x_2755 = kk_Unit;
  kk_function_t _x3485;
  kk_ref_dup(dib_header);
  _x3485 = kk_bitmap__new_mlift2718_get_dib_header_fun3486(dib_header, horizontal_resolution_bytes, print_resolution_bytes, vertical_resolution_bytes, _ctx); /*(i : int) -> <exn,local<1011>,div> ()*/
  kk_std_core__lift17275_for(_x3485, kk_integer_from_small(3), kk_integer_from_small(0), _ctx);
  kk_box_t _x3494;
  if (kk_yielding(kk_context())) {
    _x3494 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2718_get_dib_header_fun3495(dib_header, _ctx), _ctx); /*1002*/
  }
  else {
    _x3494 = (kk_ref_get(dib_header,kk_context())); /*1002*/
  }
  return kk_vector_unbox(_x3494, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2719_get_dib_header_fun3497__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_integer_t dpcm;
  kk_integer_t horizontal_resolution;
  kk_integer_t vertical_resolution;
};
static kk_box_t kk_bitmap__mlift2719_get_dib_header_fun3497(kk_function_t _fself, kk_box_t _b_2993, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2719_get_dib_header_fun3497(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2719_get_dib_header_fun3497__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2719_get_dib_header_fun3497__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2719_get_dib_header_fun3497, kk_context());
  _self->dib_header = dib_header;
  _self->dpcm = dpcm;
  _self->horizontal_resolution = horizontal_resolution;
  _self->vertical_resolution = vertical_resolution;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2719_get_dib_header_fun3497(kk_function_t _fself, kk_box_t _b_2993, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2719_get_dib_header_fun3497__t* _self = kk_function_as(struct kk_bitmap__mlift2719_get_dib_header_fun3497__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<1011,vector<int>> */
  kk_integer_t dpcm = _self->dpcm; /* int */
  kk_integer_t horizontal_resolution = _self->horizontal_resolution; /* int */
  kk_integer_t vertical_resolution = _self->vertical_resolution; /* int */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_integer_dup(dpcm);kk_integer_dup(horizontal_resolution);kk_integer_dup(vertical_resolution);}, {}, _ctx)
  kk_vector_t _x3498;
  kk_unit_t _x3499 = kk_Unit;
  kk_unit_unbox(_b_2993);
  _x3498 = kk_bitmap__mlift2718_get_dib_header(dib_header, dpcm, horizontal_resolution, vertical_resolution, _x3499, _ctx); /*vector<int>*/
  return kk_vector_box(_x3498, _ctx);
}

kk_vector_t kk_bitmap__mlift2719_get_dib_header(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_unit_t wild__0, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, dpcm : int, horizontal-resolution : int, vertical-resolution : int, wild_0 : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_unit_t x_2761 = kk_Unit;
  kk_integer_t _brw_3410 = kk_integer_from_small(14); /*int*/;
  kk_unit_t _brw_3411 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_brw_3410,(kk_integer_box(kk_integer_from_small(24))),kk_context());
  kk_integer_drop(_brw_3410, _ctx);
  _brw_3411;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3496 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2719_get_dib_header_fun3497(dib_header, dpcm, horizontal_resolution, vertical_resolution, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x3496, _ctx);
  }
  {
    return kk_bitmap__mlift2718_get_dib_header(dib_header, dpcm, horizontal_resolution, vertical_resolution, x_2761, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2720_get_dib_header_fun3501__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_integer_t dpcm;
  kk_integer_t horizontal_resolution;
  kk_integer_t vertical_resolution;
};
static kk_box_t kk_bitmap__mlift2720_get_dib_header_fun3501(kk_function_t _fself, kk_box_t _b_3003, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2720_get_dib_header_fun3501(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2720_get_dib_header_fun3501__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2720_get_dib_header_fun3501__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2720_get_dib_header_fun3501, kk_context());
  _self->dib_header = dib_header;
  _self->dpcm = dpcm;
  _self->horizontal_resolution = horizontal_resolution;
  _self->vertical_resolution = vertical_resolution;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2720_get_dib_header_fun3501(kk_function_t _fself, kk_box_t _b_3003, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2720_get_dib_header_fun3501__t* _self = kk_function_as(struct kk_bitmap__mlift2720_get_dib_header_fun3501__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<1011,vector<int>> */
  kk_integer_t dpcm = _self->dpcm; /* int */
  kk_integer_t horizontal_resolution = _self->horizontal_resolution; /* int */
  kk_integer_t vertical_resolution = _self->vertical_resolution; /* int */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_integer_dup(dpcm);kk_integer_dup(horizontal_resolution);kk_integer_dup(vertical_resolution);}, {}, _ctx)
  kk_vector_t _x3502;
  kk_unit_t _x3503 = kk_Unit;
  kk_unit_unbox(_b_3003);
  _x3502 = kk_bitmap__mlift2719_get_dib_header(dib_header, dpcm, horizontal_resolution, vertical_resolution, _x3503, _ctx); /*vector<int>*/
  return kk_vector_box(_x3502, _ctx);
}

kk_vector_t kk_bitmap__mlift2720_get_dib_header(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_unit_t wild__, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, dpcm : int, horizontal-resolution : int, vertical-resolution : int, wild_ : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_unit_t x_2763 = kk_Unit;
  kk_integer_t _brw_3407 = kk_integer_from_small(12); /*int*/;
  kk_unit_t _brw_3408 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_brw_3407,(kk_integer_box(kk_integer_from_small(1))),kk_context());
  kk_integer_drop(_brw_3407, _ctx);
  _brw_3408;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3500 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2720_get_dib_header_fun3501(dib_header, dpcm, horizontal_resolution, vertical_resolution, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x3500, _ctx);
  }
  {
    return kk_bitmap__mlift2719_get_dib_header(dib_header, dpcm, horizontal_resolution, vertical_resolution, x_2763, _ctx);
  }
}


// lift anonymous function
struct kk_bitmap_get_dib_header_fun3506__t {
  struct kk_function_s _base;
  kk_integer_t dpcm;
  kk_integer_t horizontal_resolution;
  kk_ref_t loc;
  kk_integer_t vertical_resolution;
};
static kk_box_t kk_bitmap_get_dib_header_fun3506(kk_function_t _fself, kk_box_t _b_3019, kk_context_t* _ctx);
static kk_function_t kk_bitmap_new_get_dib_header_fun3506(kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_ref_t loc, kk_integer_t vertical_resolution, kk_context_t* _ctx) {
  struct kk_bitmap_get_dib_header_fun3506__t* _self = kk_function_alloc_as(struct kk_bitmap_get_dib_header_fun3506__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap_get_dib_header_fun3506, kk_context());
  _self->dpcm = dpcm;
  _self->horizontal_resolution = horizontal_resolution;
  _self->loc = loc;
  _self->vertical_resolution = vertical_resolution;
  return &_self->_base;
}

static kk_box_t kk_bitmap_get_dib_header_fun3506(kk_function_t _fself, kk_box_t _b_3019, kk_context_t* _ctx) {
  struct kk_bitmap_get_dib_header_fun3506__t* _self = kk_function_as(struct kk_bitmap_get_dib_header_fun3506__t*, _fself);
  kk_integer_t dpcm = _self->dpcm; /* int */
  kk_integer_t horizontal_resolution = _self->horizontal_resolution; /* int */
  kk_ref_t loc = _self->loc; /* local-var<1011,vector<int>> */
  kk_integer_t vertical_resolution = _self->vertical_resolution; /* int */
  kk_drop_match(_self, {kk_integer_dup(dpcm);kk_integer_dup(horizontal_resolution);kk_ref_dup(loc);kk_integer_dup(vertical_resolution);}, {}, _ctx)
  kk_vector_t _x3507;
  kk_unit_t _x3508 = kk_Unit;
  kk_unit_unbox(_b_3019);
  _x3507 = kk_bitmap__mlift2720_get_dib_header(loc, dpcm, horizontal_resolution, vertical_resolution, _x3508, _ctx); /*vector<int>*/
  return kk_vector_box(_x3507, _ctx);
}

kk_vector_t kk_bitmap_get_dib_header(kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_integer_t dpcm, kk_context_t* _ctx) { /* (horizontal-resolution : int, vertical-resolution : int, dpcm : int) -> pure vector<int> */ 
  kk_vector_t init_2765;
  kk_integer_t _brw_3404 = kk_integer_from_small(50); /*int*/;
  kk_vector_t _brw_3405 = kk_std_core_vector_2(_brw_3404, kk_integer_box(kk_integer_from_small(0)), _ctx); /*vector<1001>*/;
  kk_integer_drop(_brw_3404, _ctx);
  init_2765 = _brw_3405; /*vector<int>*/
  kk_ref_t loc = kk_ref_alloc((kk_vector_box(init_2765, _ctx)),kk_context()); /*local-var<1011,vector<int>>*/;
  kk_unit_t x_2767 = kk_Unit;
  kk_integer_t _brw_3402 = kk_integer_from_small(0); /*int*/;
  kk_unit_t _brw_3403 = kk_Unit;
  kk_ref_vector_assign_borrow(loc,_brw_3402,(kk_integer_box(kk_integer_from_small(40))),kk_context());
  kk_integer_drop(_brw_3402, _ctx);
  _brw_3403;
  kk_vector_t res;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3504;
    kk_function_t _x3505;
    kk_ref_dup(loc);
    _x3505 = kk_bitmap_new_get_dib_header_fun3506(dpcm, horizontal_resolution, loc, vertical_resolution, _ctx); /*(1001) -> 1003 1002*/
    _x3504 = kk_std_core_hnd_yield_extend(_x3505, _ctx); /*1002*/
    res = kk_vector_unbox(_x3504, _ctx); /*vector<int>*/
  }
  else {
    kk_ref_t _x3509 = kk_ref_dup(loc); /*local-var<1011,vector<int>>*/
    res = kk_bitmap__mlift2720_get_dib_header(_x3509, dpcm, horizontal_resolution, vertical_resolution, x_2767, _ctx); /*vector<int>*/
  }
  kk_box_t _x3510 = kk_std_core_hnd_prompt_local_var(loc, kk_vector_box(res, _ctx), _ctx); /*1002*/
  return kk_vector_unbox(_x3510, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2722_get_header_fun3513__t {
  struct kk_function_s _base;
  kk_ref_t header;
};
static kk_box_t kk_bitmap__mlift2722_get_header_fun3513(kk_function_t _fself, kk_box_t _b_3036, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2722_get_header_fun3513(kk_ref_t header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2722_get_header_fun3513__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2722_get_header_fun3513__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2722_get_header_fun3513, kk_context());
  _self->header = header;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2722_get_header_fun3513(kk_function_t _fself, kk_box_t _b_3036, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2722_get_header_fun3513__t* _self = kk_function_as(struct kk_bitmap__mlift2722_get_header_fun3513__t*, _fself);
  kk_ref_t header = _self->header; /* local-var<1287,vector<int>> */
  kk_drop_match(_self, {kk_ref_dup(header);}, {}, _ctx)
  kk_box_drop(_b_3036, _ctx);
  return (kk_ref_get(header,kk_context()));
}

kk_vector_t kk_bitmap__mlift2722_get_header(kk_ref_t header, kk_unit_t wild__1, kk_context_t* _ctx) { /* forall<h> (header : local-var<h,vector<int>>, wild_1 : ()) -> <exn,local<h>,div> vector<int> */ 
  kk_unit_t x_2769 = kk_Unit;
  kk_integer_t _brw_3399 = kk_integer_from_small(10); /*int*/;
  kk_unit_t _brw_3400 = kk_Unit;
  kk_ref_vector_assign_borrow(header,_brw_3399,(kk_integer_box(kk_integer_from_small(54))),kk_context());
  kk_integer_drop(_brw_3399, _ctx);
  _brw_3400;
  kk_box_t _x3512;
  if (kk_yielding(kk_context())) {
    _x3512 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2722_get_header_fun3513(header, _ctx), _ctx); /*1002*/
  }
  else {
    _x3512 = (kk_ref_get(header,kk_context())); /*1002*/
  }
  return kk_vector_unbox(_x3512, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2723_get_header_fun3515__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap__mlift2723_get_header_fun3515(kk_function_t _fself, kk_box_t _b_3044, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2723_get_header_fun3515(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap__mlift2723_get_header_fun3515, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap__mlift2723_get_header_fun3515(kk_function_t _fself, kk_box_t _b_3044, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core__list _x3516;
  kk_integer_t _x3517 = kk_integer_unbox(_b_3044); /*int*/
  _x3516 = kk_bitmap_bytes(_x3517, _ctx); /*list<int>*/
  return kk_std_core__list_box(_x3516, _ctx);
}


// lift anonymous function
struct kk_bitmap__mlift2723_get_header_fun3519__t {
  struct kk_function_s _base;
  kk_std_core__list file_size_bytes;
  kk_ref_t header;
};
static kk_unit_t kk_bitmap__mlift2723_get_header_fun3519(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2723_get_header_fun3519(kk_std_core__list file_size_bytes, kk_ref_t header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2723_get_header_fun3519__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2723_get_header_fun3519__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2723_get_header_fun3519, kk_context());
  _self->file_size_bytes = file_size_bytes;
  _self->header = header;
  return &_self->_base;
}

static kk_unit_t kk_bitmap__mlift2723_get_header_fun3519(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2723_get_header_fun3519__t* _self = kk_function_as(struct kk_bitmap__mlift2723_get_header_fun3519__t*, _fself);
  kk_std_core__list file_size_bytes = _self->file_size_bytes; /* list<int> */
  kk_ref_t header = _self->header; /* local-var<1287,vector<int>> */
  kk_drop_match(_self, {kk_std_core__list_dup(file_size_bytes);kk_ref_dup(header);}, {}, _ctx)
  kk_integer_t _b_3053_3050;
  kk_integer_t _x3520 = kk_integer_dup(i); /*int*/
  _b_3053_3050 = kk_integer_add(_x3520,(kk_integer_from_small(2)),kk_context()); /*int*/
  kk_std_core_types__maybe _b_3055_3047 = kk_std_core__lp__lb__rb__2_rp_(file_size_bytes, i, _ctx); /*maybe<int>*/;
  kk_integer_t _b_3054_3051;
  kk_box_t _x3521 = kk_std_core_maybe_1(_b_3055_3047, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_3054_3051 = kk_integer_unbox(_x3521); /*int*/
  kk_unit_t _brw_3397 = kk_Unit;
  kk_ref_vector_assign_borrow(header,_b_3053_3050,(kk_integer_box(_b_3054_3051)),kk_context());
  kk_ref_drop(header, _ctx);
  kk_integer_drop(_b_3053_3050, _ctx);
  return _brw_3397;
}


// lift anonymous function
struct kk_bitmap__mlift2723_get_header_fun3523__t {
  struct kk_function_s _base;
  kk_ref_t header;
};
static kk_box_t kk_bitmap__mlift2723_get_header_fun3523(kk_function_t _fself, kk_box_t _b_3058, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2723_get_header_fun3523(kk_ref_t header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2723_get_header_fun3523__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2723_get_header_fun3523__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2723_get_header_fun3523, kk_context());
  _self->header = header;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2723_get_header_fun3523(kk_function_t _fself, kk_box_t _b_3058, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2723_get_header_fun3523__t* _self = kk_function_as(struct kk_bitmap__mlift2723_get_header_fun3523__t*, _fself);
  kk_ref_t header = _self->header; /* local-var<1287,vector<int>> */
  kk_drop_match(_self, {kk_ref_dup(header);}, {}, _ctx)
  kk_vector_t _x3524;
  kk_unit_t _x3525 = kk_Unit;
  kk_unit_unbox(_b_3058);
  _x3524 = kk_bitmap__mlift2722_get_header(header, _x3525, _ctx); /*vector<int>*/
  return kk_vector_box(_x3524, _ctx);
}

kk_vector_t kk_bitmap__mlift2723_get_header(kk_integer_t file_size, kk_ref_t header, kk_unit_t wild__0, kk_context_t* _ctx) { /* forall<h> (file-size : int, header : local-var<h,vector<int>>, wild_0 : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_std_core__list file_size_bytes;
  kk_box_t _x3514 = kk_std_core_hnd__open_none1(kk_bitmap__new_mlift2723_get_header_fun3515(_ctx), kk_integer_box(file_size), _ctx); /*1002*/
  file_size_bytes = kk_std_core__list_unbox(_x3514, _ctx); /*list<int>*/
  kk_unit_t x_2773 = kk_Unit;
  kk_function_t _x3518;
  kk_ref_dup(header);
  _x3518 = kk_bitmap__new_mlift2723_get_header_fun3519(file_size_bytes, header, _ctx); /*(i : int) -> <exn,local<1287>,div> ()*/
  kk_std_core__lift17275_for(_x3518, kk_integer_from_small(11), kk_integer_from_small(0), _ctx);
  if (kk_yielding(kk_context())) {
    kk_box_t _x3522 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2723_get_header_fun3523(header, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x3522, _ctx);
  }
  {
    return kk_bitmap__mlift2722_get_header(header, x_2773, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2724_get_header_fun3527__t {
  struct kk_function_s _base;
  kk_integer_t file_size;
  kk_ref_t header;
};
static kk_box_t kk_bitmap__mlift2724_get_header_fun3527(kk_function_t _fself, kk_box_t _b_3068, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2724_get_header_fun3527(kk_integer_t file_size, kk_ref_t header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2724_get_header_fun3527__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2724_get_header_fun3527__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2724_get_header_fun3527, kk_context());
  _self->file_size = file_size;
  _self->header = header;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2724_get_header_fun3527(kk_function_t _fself, kk_box_t _b_3068, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2724_get_header_fun3527__t* _self = kk_function_as(struct kk_bitmap__mlift2724_get_header_fun3527__t*, _fself);
  kk_integer_t file_size = _self->file_size; /* int */
  kk_ref_t header = _self->header; /* local-var<1287,vector<int>> */
  kk_drop_match(_self, {kk_integer_dup(file_size);kk_ref_dup(header);}, {}, _ctx)
  kk_vector_t _x3528;
  kk_unit_t _x3529 = kk_Unit;
  kk_unit_unbox(_b_3068);
  _x3528 = kk_bitmap__mlift2723_get_header(file_size, header, _x3529, _ctx); /*vector<int>*/
  return kk_vector_box(_x3528, _ctx);
}

kk_vector_t kk_bitmap__mlift2724_get_header(kk_integer_t file_size, kk_ref_t header, kk_unit_t wild__, kk_context_t* _ctx) { /* forall<h> (file-size : int, header : local-var<h,vector<int>>, wild_ : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_unit_t x_2775 = kk_Unit;
  kk_integer_t _brw_3394 = kk_integer_from_small(1); /*int*/;
  kk_unit_t _brw_3395 = kk_Unit;
  kk_ref_vector_assign_borrow(header,_brw_3394,(kk_integer_box(kk_integer_from_small(77))),kk_context());
  kk_integer_drop(_brw_3394, _ctx);
  _brw_3395;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3526 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2724_get_header_fun3527(file_size, header, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x3526, _ctx);
  }
  {
    return kk_bitmap__mlift2723_get_header(file_size, header, x_2775, _ctx);
  }
}


// lift anonymous function
struct kk_bitmap_get_header_fun3532__t {
  struct kk_function_s _base;
  kk_integer_t file_size;
  kk_ref_t loc;
};
static kk_box_t kk_bitmap_get_header_fun3532(kk_function_t _fself, kk_box_t _b_3084, kk_context_t* _ctx);
static kk_function_t kk_bitmap_new_get_header_fun3532(kk_integer_t file_size, kk_ref_t loc, kk_context_t* _ctx) {
  struct kk_bitmap_get_header_fun3532__t* _self = kk_function_alloc_as(struct kk_bitmap_get_header_fun3532__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap_get_header_fun3532, kk_context());
  _self->file_size = file_size;
  _self->loc = loc;
  return &_self->_base;
}

static kk_box_t kk_bitmap_get_header_fun3532(kk_function_t _fself, kk_box_t _b_3084, kk_context_t* _ctx) {
  struct kk_bitmap_get_header_fun3532__t* _self = kk_function_as(struct kk_bitmap_get_header_fun3532__t*, _fself);
  kk_integer_t file_size = _self->file_size; /* int */
  kk_ref_t loc = _self->loc; /* local-var<1287,vector<int>> */
  kk_drop_match(_self, {kk_integer_dup(file_size);kk_ref_dup(loc);}, {}, _ctx)
  kk_vector_t _x3533;
  kk_unit_t _x3534 = kk_Unit;
  kk_unit_unbox(_b_3084);
  _x3533 = kk_bitmap__mlift2724_get_header(file_size, loc, _x3534, _ctx); /*vector<int>*/
  return kk_vector_box(_x3533, _ctx);
}

kk_vector_t kk_bitmap_get_header(kk_integer_t file_size, kk_context_t* _ctx) { /* (file-size : int) -> pure vector<int> */ 
  kk_vector_t init_2777;
  kk_integer_t _brw_3391 = kk_integer_from_small(14); /*int*/;
  kk_vector_t _brw_3392 = kk_std_core_vector_2(_brw_3391, kk_integer_box(kk_integer_from_small(0)), _ctx); /*vector<1001>*/;
  kk_integer_drop(_brw_3391, _ctx);
  init_2777 = _brw_3392; /*vector<int>*/
  kk_ref_t loc = kk_ref_alloc((kk_vector_box(init_2777, _ctx)),kk_context()); /*local-var<1287,vector<int>>*/;
  kk_unit_t x_2779 = kk_Unit;
  kk_integer_t _brw_3389 = kk_integer_from_small(0); /*int*/;
  kk_unit_t _brw_3390 = kk_Unit;
  kk_ref_vector_assign_borrow(loc,_brw_3389,(kk_integer_box(kk_integer_from_small(66))),kk_context());
  kk_integer_drop(_brw_3389, _ctx);
  _brw_3390;
  kk_vector_t res;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3530;
    kk_function_t _x3531;
    kk_ref_dup(loc);
    _x3531 = kk_bitmap_new_get_header_fun3532(file_size, loc, _ctx); /*(1001) -> 1003 1002*/
    _x3530 = kk_std_core_hnd_yield_extend(_x3531, _ctx); /*1002*/
    res = kk_vector_unbox(_x3530, _ctx); /*vector<int>*/
  }
  else {
    kk_ref_t _x3535 = kk_ref_dup(loc); /*local-var<1287,vector<int>>*/
    res = kk_bitmap__mlift2724_get_header(file_size, _x3535, x_2779, _ctx); /*vector<int>*/
  }
  kk_box_t _x3536 = kk_std_core_hnd_prompt_local_var(loc, kk_vector_box(res, _ctx), _ctx); /*1002*/
  return kk_vector_unbox(_x3536, _ctx);
}
 
// monadic lift

kk_unit_t kk_bitmap__mlift2725_get_bitmap_bytes(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_integer_t i2, kk_integer_t _y_2694, kk_context_t* _ctx) { /* forall<h> (dib-header : vector<int>, final-bitmap : local-var<h,vector<int>>, header : vector<int>, i2 : int, int) -> exn () */ 
  kk_integer_t _b_3095_3092;
  kk_integer_t _x3537;
  kk_integer_t _x3538;
  kk_integer_t _brw_3387 = kk_std_core_length_2(header, _ctx); /*int*/;
  kk_vector_drop(header, _ctx);
  _x3538 = _brw_3387; /*int*/
  kk_integer_t _x3539;
  kk_integer_t _brw_3386 = kk_std_core_length_2(dib_header, _ctx); /*int*/;
  kk_vector_drop(dib_header, _ctx);
  _x3539 = _brw_3386; /*int*/
  _x3537 = kk_integer_add(_x3538,_x3539,kk_context()); /*int*/
  _b_3095_3092 = kk_integer_add(_x3537,i2,kk_context()); /*int*/
  kk_unit_t _brw_3385 = kk_Unit;
  kk_ref_vector_assign_borrow(final_bitmap,_b_3095_3092,(kk_integer_box(_y_2694)),kk_context());
  kk_ref_drop(final_bitmap, _ctx);
  kk_integer_drop(_b_3095_3092, _ctx);
  _brw_3385; return kk_Unit;
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2726_get_bitmap_bytes_fun3542__t {
  struct kk_function_s _base;
  kk_vector_t dib_header;
  kk_ref_t final_bitmap;
  kk_vector_t header;
  kk_integer_t i2;
};
static kk_box_t kk_bitmap__mlift2726_get_bitmap_bytes_fun3542(kk_function_t _fself, kk_box_t _b_3102, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2726_get_bitmap_bytes_fun3542(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_integer_t i2, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2726_get_bitmap_bytes_fun3542__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2726_get_bitmap_bytes_fun3542__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2726_get_bitmap_bytes_fun3542, kk_context());
  _self->dib_header = dib_header;
  _self->final_bitmap = final_bitmap;
  _self->header = header;
  _self->i2 = i2;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2726_get_bitmap_bytes_fun3542(kk_function_t _fself, kk_box_t _b_3102, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2726_get_bitmap_bytes_fun3542__t* _self = kk_function_as(struct kk_bitmap__mlift2726_get_bitmap_bytes_fun3542__t*, _fself);
  kk_vector_t dib_header = _self->dib_header; /* vector<int> */
  kk_ref_t final_bitmap = _self->final_bitmap; /* local-var<2600,vector<int>> */
  kk_vector_t header = _self->header; /* vector<int> */
  kk_integer_t i2 = _self->i2; /* int */
  kk_drop_match(_self, {kk_vector_dup(dib_header);kk_ref_dup(final_bitmap);kk_vector_dup(header);kk_integer_dup(i2);}, {}, _ctx)
  kk_unit_t _x3543 = kk_Unit;
  kk_integer_t _x3544 = kk_integer_unbox(_b_3102); /*int*/
  kk_bitmap__mlift2725_get_bitmap_bytes(dib_header, final_bitmap, header, i2, _x3544, _ctx);
  return kk_unit_box(_x3543);
}

kk_unit_t kk_bitmap__mlift2726_get_bitmap_bytes(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_integer_t i2, kk_vector_t _y_2693, kk_context_t* _ctx) { /* forall<h> (dib-header : vector<int>, final-bitmap : local-var<h,vector<int>>, header : vector<int>, i2 : int, vector<int>) -> <local<h>,exn,div> () */ 
  kk_integer_t x_2781;
  kk_box_t _x3540;
  kk_box_t _brw_3384 = kk_vector_at_int_borrow(_y_2693, i2); /*1001*/;
  kk_vector_drop(_y_2693, _ctx);
  _x3540 = _brw_3384; /*1001*/
  x_2781 = kk_integer_unbox(_x3540); /*int*/
  if (kk_yielding(kk_context())) {
    kk_integer_drop(x_2781, _ctx);
    kk_box_t _x3541 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2726_get_bitmap_bytes_fun3542(dib_header, final_bitmap, header, i2, _ctx), _ctx); /*1002*/
    kk_unit_unbox(_x3541); return kk_Unit;
  }
  {
    kk_bitmap__mlift2725_get_bitmap_bytes(dib_header, final_bitmap, header, i2, x_2781, _ctx); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3548__t {
  struct kk_function_s _base;
  kk_vector_t dib_header;
  kk_ref_t final_bitmap;
  kk_vector_t header;
  kk_ref_t pixels;
};
static kk_unit_t kk_bitmap__mlift2728_get_bitmap_bytes_fun3548(kk_function_t _fself, kk_integer_t i2, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2728_get_bitmap_bytes_fun3548(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_ref_t pixels, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3548__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3548__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2728_get_bitmap_bytes_fun3548, kk_context());
  _self->dib_header = dib_header;
  _self->final_bitmap = final_bitmap;
  _self->header = header;
  _self->pixels = pixels;
  return &_self->_base;
}



// lift anonymous function
struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3551__t {
  struct kk_function_s _base;
  kk_vector_t dib_header;
  kk_ref_t final_bitmap;
  kk_vector_t header;
  kk_integer_t i2;
};
static kk_box_t kk_bitmap__mlift2728_get_bitmap_bytes_fun3551(kk_function_t _fself, kk_box_t _b_3110, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2728_get_bitmap_bytes_fun3551(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_integer_t i2, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3551__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3551__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2728_get_bitmap_bytes_fun3551, kk_context());
  _self->dib_header = dib_header;
  _self->final_bitmap = final_bitmap;
  _self->header = header;
  _self->i2 = i2;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2728_get_bitmap_bytes_fun3551(kk_function_t _fself, kk_box_t _b_3110, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3551__t* _self = kk_function_as(struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3551__t*, _fself);
  kk_vector_t dib_header = _self->dib_header; /* vector<int> */
  kk_ref_t final_bitmap = _self->final_bitmap; /* local-var<2600,vector<int>> */
  kk_vector_t header = _self->header; /* vector<int> */
  kk_integer_t i2 = _self->i2; /* int */
  kk_drop_match(_self, {kk_vector_dup(dib_header);kk_ref_dup(final_bitmap);kk_vector_dup(header);kk_integer_dup(i2);}, {}, _ctx)
  kk_unit_t _x3552 = kk_Unit;
  kk_vector_t _x3553 = kk_vector_unbox(_b_3110, _ctx); /*vector<int>*/
  kk_bitmap__mlift2726_get_bitmap_bytes(dib_header, final_bitmap, header, i2, _x3553, _ctx);
  return kk_unit_box(_x3552);
}
static kk_unit_t kk_bitmap__mlift2728_get_bitmap_bytes_fun3548(kk_function_t _fself, kk_integer_t i2, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3548__t* _self = kk_function_as(struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3548__t*, _fself);
  kk_vector_t dib_header = _self->dib_header; /* vector<int> */
  kk_ref_t final_bitmap = _self->final_bitmap; /* local-var<2600,vector<int>> */
  kk_vector_t header = _self->header; /* vector<int> */
  kk_ref_t pixels = _self->pixels; /* local-var<2600,vector<int>> */
  kk_drop_match(_self, {kk_vector_dup(dib_header);kk_ref_dup(final_bitmap);kk_vector_dup(header);kk_ref_dup(pixels);}, {}, _ctx)
  kk_vector_t x0_2785;
  kk_box_t _x3549 = (kk_ref_get(pixels,kk_context())); /*1000*/
  x0_2785 = kk_vector_unbox(_x3549, _ctx); /*vector<int>*/
  if (kk_yielding(kk_context())) {
    kk_vector_drop(x0_2785, _ctx);
    kk_box_t _x3550 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2728_get_bitmap_bytes_fun3551(dib_header, final_bitmap, header, i2, _ctx), _ctx); /*1002*/
    return kk_unit_unbox(_x3550);
  }
  {
    return kk_bitmap__mlift2726_get_bitmap_bytes(dib_header, final_bitmap, header, i2, x0_2785, _ctx);
  }
}


// lift anonymous function
struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3555__t {
  struct kk_function_s _base;
  kk_ref_t final_bitmap;
};
static kk_box_t kk_bitmap__mlift2728_get_bitmap_bytes_fun3555(kk_function_t _fself, kk_box_t _b_3115, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2728_get_bitmap_bytes_fun3555(kk_ref_t final_bitmap, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3555__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3555__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2728_get_bitmap_bytes_fun3555, kk_context());
  _self->final_bitmap = final_bitmap;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2728_get_bitmap_bytes_fun3555(kk_function_t _fself, kk_box_t _b_3115, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3555__t* _self = kk_function_as(struct kk_bitmap__mlift2728_get_bitmap_bytes_fun3555__t*, _fself);
  kk_ref_t final_bitmap = _self->final_bitmap; /* local-var<2600,vector<int>> */
  kk_drop_match(_self, {kk_ref_dup(final_bitmap);}, {}, _ctx)
  kk_box_drop(_b_3115, _ctx);
  return (kk_ref_get(final_bitmap,kk_context()));
}

kk_vector_t kk_bitmap__mlift2728_get_bitmap_bytes(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_ref_t pixels, kk_vector_t _y_2692, kk_context_t* _ctx) { /* forall<h> (dib-header : vector<int>, final-bitmap : local-var<h,vector<int>>, header : vector<int>, pixels : local-var<h,vector<int>>, vector<int>) -> <local<h>,exn,div> vector<int> */ 
  kk_integer_t end4_2644;
  kk_integer_t _x3546;
  kk_integer_t _brw_3382 = kk_std_core_length_2(_y_2692, _ctx); /*int*/;
  kk_vector_drop(_y_2692, _ctx);
  _x3546 = _brw_3382; /*int*/
  end4_2644 = kk_integer_sub(_x3546,(kk_integer_from_small(1)),kk_context()); /*int*/
  kk_unit_t x_2783 = kk_Unit;
  kk_function_t _x3547;
  kk_ref_dup(final_bitmap);
  _x3547 = kk_bitmap__new_mlift2728_get_bitmap_bytes_fun3548(dib_header, final_bitmap, header, pixels, _ctx); /*(i2 : int) -> <exn,local<2600>,div> ()*/
  kk_std_core__lift17275_for(_x3547, end4_2644, kk_integer_from_small(0), _ctx);
  kk_box_t _x3554;
  if (kk_yielding(kk_context())) {
    _x3554 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2728_get_bitmap_bytes_fun3555(final_bitmap, _ctx), _ctx); /*1002*/
  }
  else {
    _x3554 = (kk_ref_get(final_bitmap,kk_context())); /*1002*/
  }
  return kk_vector_unbox(_x3554, _ctx);
}
 
// monadic lift

kk_unit_t kk_bitmap__mlift2729_get_bitmap_bytes(kk_ref_t final_bitmap, kk_vector_t header, kk_integer_t i1, kk_integer_t _y_2689, kk_context_t* _ctx) { /* forall<h> (final-bitmap : local-var<h,vector<int>>, header : vector<int>, i1 : int, int) -> exn () */ 
  kk_integer_t _b_3125_3122;
  kk_integer_t _x3556;
  kk_integer_t _brw_3379 = kk_std_core_length_2(header, _ctx); /*int*/;
  kk_vector_drop(header, _ctx);
  _x3556 = _brw_3379; /*int*/
  _b_3125_3122 = kk_integer_add(_x3556,i1,kk_context()); /*int*/
  kk_unit_t _brw_3378 = kk_Unit;
  kk_ref_vector_assign_borrow(final_bitmap,_b_3125_3122,(kk_integer_box(_y_2689)),kk_context());
  kk_ref_drop(final_bitmap, _ctx);
  kk_integer_drop(_b_3125_3122, _ctx);
  _brw_3378; return kk_Unit;
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2730_get_bitmap_bytes_fun3560__t {
  struct kk_function_s _base;
  kk_vector_t dib_header;
  kk_ref_t final_bitmap;
  kk_vector_t header;
  kk_ref_t pixels;
};
static kk_box_t kk_bitmap__mlift2730_get_bitmap_bytes_fun3560(kk_function_t _fself, kk_box_t _b_3130, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2730_get_bitmap_bytes_fun3560(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_ref_t pixels, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2730_get_bitmap_bytes_fun3560__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2730_get_bitmap_bytes_fun3560__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2730_get_bitmap_bytes_fun3560, kk_context());
  _self->dib_header = dib_header;
  _self->final_bitmap = final_bitmap;
  _self->header = header;
  _self->pixels = pixels;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2730_get_bitmap_bytes_fun3560(kk_function_t _fself, kk_box_t _b_3130, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2730_get_bitmap_bytes_fun3560__t* _self = kk_function_as(struct kk_bitmap__mlift2730_get_bitmap_bytes_fun3560__t*, _fself);
  kk_vector_t dib_header = _self->dib_header; /* vector<int> */
  kk_ref_t final_bitmap = _self->final_bitmap; /* local-var<2600,vector<int>> */
  kk_vector_t header = _self->header; /* vector<int> */
  kk_ref_t pixels = _self->pixels; /* local-var<2600,vector<int>> */
  kk_drop_match(_self, {kk_vector_dup(dib_header);kk_ref_dup(final_bitmap);kk_vector_dup(header);kk_ref_dup(pixels);}, {}, _ctx)
  kk_vector_t _x3561;
  kk_vector_t _x3562 = kk_vector_unbox(_b_3130, _ctx); /*vector<int>*/
  _x3561 = kk_bitmap__mlift2728_get_bitmap_bytes(dib_header, final_bitmap, header, pixels, _x3562, _ctx); /*vector<int>*/
  return kk_vector_box(_x3561, _ctx);
}

kk_vector_t kk_bitmap__mlift2730_get_bitmap_bytes(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_ref_t pixels, kk_unit_t wild__4, kk_context_t* _ctx) { /* forall<h> (dib-header : vector<int>, final-bitmap : local-var<h,vector<int>>, header : vector<int>, pixels : local-var<h,vector<int>>, wild_4 : ()) -> <exn,local<h>,div> vector<int> */ 
  kk_vector_t x_2789;
  kk_box_t _x3557;
  kk_ref_t _x3558 = kk_ref_dup(pixels); /*local-var<2600,vector<int>>*/
  _x3557 = (kk_ref_get(_x3558,kk_context())); /*1000*/
  x_2789 = kk_vector_unbox(_x3557, _ctx); /*vector<int>*/
  if (kk_yielding(kk_context())) {
    kk_vector_drop(x_2789, _ctx);
    kk_box_t _x3559 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2730_get_bitmap_bytes_fun3560(dib_header, final_bitmap, header, pixels, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x3559, _ctx);
  }
  {
    return kk_bitmap__mlift2728_get_bitmap_bytes(dib_header, final_bitmap, header, pixels, x_2789, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3565__t {
  struct kk_function_s _base;
  kk_vector_t dib_header;
  kk_ref_t final_bitmap;
  kk_vector_t header;
};
static kk_unit_t kk_bitmap__mlift2732_get_bitmap_bytes_fun3565(kk_function_t _fself, kk_integer_t i1, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2732_get_bitmap_bytes_fun3565(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3565__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3565__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2732_get_bitmap_bytes_fun3565, kk_context());
  _self->dib_header = dib_header;
  _self->final_bitmap = final_bitmap;
  _self->header = header;
  return &_self->_base;
}



// lift anonymous function
struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3567__t {
  struct kk_function_s _base;
  kk_ref_t final_bitmap;
  kk_vector_t header;
  kk_integer_t i1;
};
static kk_unit_t kk_bitmap__mlift2732_get_bitmap_bytes_fun3567(kk_function_t _fself, kk_integer_t _y_2689, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2732_get_bitmap_bytes_fun3567(kk_ref_t final_bitmap, kk_vector_t header, kk_integer_t i1, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3567__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3567__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2732_get_bitmap_bytes_fun3567, kk_context());
  _self->final_bitmap = final_bitmap;
  _self->header = header;
  _self->i1 = i1;
  return &_self->_base;
}

static kk_unit_t kk_bitmap__mlift2732_get_bitmap_bytes_fun3567(kk_function_t _fself, kk_integer_t _y_2689, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3567__t* _self = kk_function_as(struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3567__t*, _fself);
  kk_ref_t final_bitmap = _self->final_bitmap; /* local-var<2600,vector<int>> */
  kk_vector_t header = _self->header; /* vector<int> */
  kk_integer_t i1 = _self->i1; /* int */
  kk_drop_match(_self, {kk_ref_dup(final_bitmap);kk_vector_dup(header);kk_integer_dup(i1);}, {}, _ctx)
  kk_integer_t _b_3147_3144;
  kk_integer_t _x3568;
  kk_integer_t _brw_3374 = kk_std_core_length_2(header, _ctx); /*int*/;
  kk_vector_drop(header, _ctx);
  _x3568 = _brw_3374; /*int*/
  _b_3147_3144 = kk_integer_add(_x3568,i1,kk_context()); /*int*/
  kk_unit_t _brw_3373 = kk_Unit;
  kk_ref_vector_assign_borrow(final_bitmap,_b_3147_3144,(kk_integer_box(_y_2689)),kk_context());
  kk_ref_drop(final_bitmap, _ctx);
  kk_integer_drop(_b_3147_3144, _ctx);
  return _brw_3373;
}


// lift anonymous function
struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3570__t {
  struct kk_function_s _base;
  kk_function_t next0_2794;
};
static kk_box_t kk_bitmap__mlift2732_get_bitmap_bytes_fun3570(kk_function_t _fself, kk_box_t _b_3150, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2732_get_bitmap_bytes_fun3570(kk_function_t next0_2794, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3570__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3570__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2732_get_bitmap_bytes_fun3570, kk_context());
  _self->next0_2794 = next0_2794;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2732_get_bitmap_bytes_fun3570(kk_function_t _fself, kk_box_t _b_3150, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3570__t* _self = kk_function_as(struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3570__t*, _fself);
  kk_function_t next0_2794 = _self->next0_2794; /* (int) -> exn () */
  kk_drop_match(_self, {kk_function_dup(next0_2794);}, {}, _ctx)
  kk_unit_t _x3571 = kk_Unit;
  kk_integer_t _x3572 = kk_integer_unbox(_b_3150); /*int*/
  kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), next0_2794, (next0_2794, _x3572, _ctx));
  return kk_unit_box(_x3571);
}
static kk_unit_t kk_bitmap__mlift2732_get_bitmap_bytes_fun3565(kk_function_t _fself, kk_integer_t i1, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3565__t* _self = kk_function_as(struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3565__t*, _fself);
  kk_vector_t dib_header = _self->dib_header; /* vector<int> */
  kk_ref_t final_bitmap = _self->final_bitmap; /* local-var<2600,vector<int>> */
  kk_vector_t header = _self->header; /* vector<int> */
  kk_drop_match(_self, {kk_vector_dup(dib_header);kk_ref_dup(final_bitmap);kk_vector_dup(header);}, {}, _ctx)
  kk_integer_t x0_2793;
  kk_box_t _x3566;
  kk_box_t _brw_3375 = kk_vector_at_int_borrow(dib_header, i1); /*1001*/;
  kk_vector_drop(dib_header, _ctx);
  _x3566 = _brw_3375; /*1001*/
  x0_2793 = kk_integer_unbox(_x3566); /*int*/
  kk_function_t next0_2794 = kk_bitmap__new_mlift2732_get_bitmap_bytes_fun3567(final_bitmap, header, i1, _ctx); /*(int) -> exn ()*/;
  if (kk_yielding(kk_context())) {
    kk_integer_drop(x0_2793, _ctx);
    kk_box_t _x3569 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2732_get_bitmap_bytes_fun3570(next0_2794, _ctx), _ctx); /*1002*/
    return kk_unit_unbox(_x3569);
  }
  {
    return kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), next0_2794, (next0_2794, x0_2793, _ctx));
  }
}


// lift anonymous function
struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3574__t {
  struct kk_function_s _base;
  kk_vector_t dib_header;
  kk_ref_t final_bitmap;
  kk_vector_t header;
  kk_ref_t pixels;
};
static kk_box_t kk_bitmap__mlift2732_get_bitmap_bytes_fun3574(kk_function_t _fself, kk_box_t _b_3153, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2732_get_bitmap_bytes_fun3574(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_ref_t pixels, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3574__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3574__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2732_get_bitmap_bytes_fun3574, kk_context());
  _self->dib_header = dib_header;
  _self->final_bitmap = final_bitmap;
  _self->header = header;
  _self->pixels = pixels;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2732_get_bitmap_bytes_fun3574(kk_function_t _fself, kk_box_t _b_3153, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3574__t* _self = kk_function_as(struct kk_bitmap__mlift2732_get_bitmap_bytes_fun3574__t*, _fself);
  kk_vector_t dib_header = _self->dib_header; /* vector<int> */
  kk_ref_t final_bitmap = _self->final_bitmap; /* local-var<2600,vector<int>> */
  kk_vector_t header = _self->header; /* vector<int> */
  kk_ref_t pixels = _self->pixels; /* local-var<2600,vector<int>> */
  kk_drop_match(_self, {kk_vector_dup(dib_header);kk_ref_dup(final_bitmap);kk_vector_dup(header);kk_ref_dup(pixels);}, {}, _ctx)
  kk_vector_t _x3575;
  kk_unit_t _x3576 = kk_Unit;
  kk_unit_unbox(_b_3153);
  _x3575 = kk_bitmap__mlift2730_get_bitmap_bytes(dib_header, final_bitmap, header, pixels, _x3576, _ctx); /*vector<int>*/
  return kk_vector_box(_x3575, _ctx);
}

kk_vector_t kk_bitmap__mlift2732_get_bitmap_bytes(kk_vector_t dib_header, kk_ref_t final_bitmap, kk_vector_t header, kk_ref_t pixels, kk_unit_t wild__3, kk_context_t* _ctx) { /* forall<h> (dib-header : vector<int>, final-bitmap : local-var<h,vector<int>>, header : vector<int>, pixels : local-var<h,vector<int>>, wild_3 : ()) -> <exn,local<h>,div> vector<int> */ 
  kk_integer_t end3_2641;
  kk_integer_t _x3563 = kk_std_core_length_2(dib_header, _ctx); /*int*/
  end3_2641 = kk_integer_sub(_x3563,(kk_integer_from_small(1)),kk_context()); /*int*/
  kk_unit_t x_2791 = kk_Unit;
  kk_function_t _x3564;
  kk_vector_dup(dib_header);
  kk_ref_dup(final_bitmap);
  kk_vector_dup(header);
  _x3564 = kk_bitmap__new_mlift2732_get_bitmap_bytes_fun3565(dib_header, final_bitmap, header, _ctx); /*(i1 : int) -> <exn,local<2600>,div> ()*/
  kk_std_core__lift17275_for(_x3564, end3_2641, kk_integer_from_small(0), _ctx);
  if (kk_yielding(kk_context())) {
    kk_box_t _x3573 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2732_get_bitmap_bytes_fun3574(dib_header, final_bitmap, header, pixels, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x3573, _ctx);
  }
  {
    return kk_bitmap__mlift2730_get_bitmap_bytes(dib_header, final_bitmap, header, pixels, x_2791, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3583__t {
  struct kk_function_s _base;
  kk_vector_t header;
  kk_ref_t loc;
};
static kk_unit_t kk_bitmap__mlift2733_get_bitmap_bytes_fun3583(kk_function_t _fself, kk_integer_t i00, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2733_get_bitmap_bytes_fun3583(kk_vector_t header, kk_ref_t loc, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3583__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3583__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2733_get_bitmap_bytes_fun3583, kk_context());
  _self->header = header;
  _self->loc = loc;
  return &_self->_base;
}



// lift anonymous function
struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3586__t {
  struct kk_function_s _base;
  kk_integer_t i00;
  kk_ref_t loc;
};
static kk_box_t kk_bitmap__mlift2733_get_bitmap_bytes_fun3586(kk_function_t _fself, kk_box_t _b_3170, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2733_get_bitmap_bytes_fun3586(kk_integer_t i00, kk_ref_t loc, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3586__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3586__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2733_get_bitmap_bytes_fun3586, kk_context());
  _self->i00 = i00;
  _self->loc = loc;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2733_get_bitmap_bytes_fun3586(kk_function_t _fself, kk_box_t _b_3170, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3586__t* _self = kk_function_as(struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3586__t*, _fself);
  kk_integer_t i00 = _self->i00; /* int */
  kk_ref_t loc = _self->loc; /* local-var<2600,vector<int>> */
  kk_drop_match(_self, {kk_integer_dup(i00);kk_ref_dup(loc);}, {}, _ctx)
  kk_unit_t _x3587 = kk_Unit;
  kk_unit_t _brw_3367 = kk_Unit;
  kk_ref_vector_assign_borrow(loc,i00,_b_3170,kk_context());
  kk_ref_drop(loc, _ctx);
  kk_integer_drop(i00, _ctx);
  _brw_3367;
  return kk_unit_box(_x3587);
}
static kk_unit_t kk_bitmap__mlift2733_get_bitmap_bytes_fun3583(kk_function_t _fself, kk_integer_t i00, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3583__t* _self = kk_function_as(struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3583__t*, _fself);
  kk_vector_t header = _self->header; /* vector<int> */
  kk_ref_t loc = _self->loc; /* local-var<2600,vector<int>> */
  kk_drop_match(_self, {kk_vector_dup(header);kk_ref_dup(loc);}, {}, _ctx)
  kk_integer_t x0_2803;
  kk_box_t _x3584;
  kk_box_t _brw_3368 = kk_vector_at_int_borrow(header, i00); /*1001*/;
  kk_vector_drop(header, _ctx);
  _x3584 = _brw_3368; /*1001*/
  x0_2803 = kk_integer_unbox(_x3584); /*int*/
  if (kk_yielding(kk_context())) {
    kk_integer_drop(x0_2803, _ctx);
    kk_box_t _x3585 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2733_get_bitmap_bytes_fun3586(i00, loc, _ctx), _ctx); /*1002*/
    return kk_unit_unbox(_x3585);
  }
  {
    kk_unit_t _brw_3366 = kk_Unit;
    kk_ref_vector_assign_borrow(loc,i00,(kk_integer_box(x0_2803)),kk_context());
    kk_ref_drop(loc, _ctx);
    kk_integer_drop(i00, _ctx);
    return _brw_3366;
  }
}


// lift anonymous function
struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3590__t {
  struct kk_function_s _base;
  kk_vector_t dib_header;
  kk_vector_t header;
  kk_ref_t loc;
  kk_ref_t pixels;
};
static kk_box_t kk_bitmap__mlift2733_get_bitmap_bytes_fun3590(kk_function_t _fself, kk_box_t _b_3183, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2733_get_bitmap_bytes_fun3590(kk_vector_t dib_header, kk_vector_t header, kk_ref_t loc, kk_ref_t pixels, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3590__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3590__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2733_get_bitmap_bytes_fun3590, kk_context());
  _self->dib_header = dib_header;
  _self->header = header;
  _self->loc = loc;
  _self->pixels = pixels;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2733_get_bitmap_bytes_fun3590(kk_function_t _fself, kk_box_t _b_3183, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3590__t* _self = kk_function_as(struct kk_bitmap__mlift2733_get_bitmap_bytes_fun3590__t*, _fself);
  kk_vector_t dib_header = _self->dib_header; /* vector<int> */
  kk_vector_t header = _self->header; /* vector<int> */
  kk_ref_t loc = _self->loc; /* local-var<2600,vector<int>> */
  kk_ref_t pixels = _self->pixels; /* local-var<2600,vector<int>> */
  kk_drop_match(_self, {kk_vector_dup(dib_header);kk_vector_dup(header);kk_ref_dup(loc);kk_ref_dup(pixels);}, {}, _ctx)
  kk_vector_t _x3591;
  kk_unit_t _x3592 = kk_Unit;
  kk_unit_unbox(_b_3183);
  _x3591 = kk_bitmap__mlift2732_get_bitmap_bytes(dib_header, loc, header, pixels, _x3592, _ctx); /*vector<int>*/
  return kk_vector_box(_x3591, _ctx);
}

kk_vector_t kk_bitmap__mlift2733_get_bitmap_bytes(kk_vector_t dib_header, kk_vector_t header, kk_ref_t pixels, kk_vector_t _y_2685, kk_context_t* _ctx) { /* forall<h> (dib-header : vector<int>, header : vector<int>, pixels : local-var<h,vector<int>>, vector<int>) -> <local<h>,exn,div> vector<int> */ 
  kk_integer_t _b_3158_3156;
  kk_integer_t _x3577;
  kk_integer_t _x3578 = kk_std_core_length_2(header, _ctx); /*int*/
  kk_integer_t _x3579 = kk_std_core_length_2(dib_header, _ctx); /*int*/
  _x3577 = kk_integer_add(_x3578,_x3579,kk_context()); /*int*/
  kk_integer_t _x3580;
  kk_integer_t _brw_3370 = kk_std_core_length_2(_y_2685, _ctx); /*int*/;
  kk_vector_drop(_y_2685, _ctx);
  _x3580 = _brw_3370; /*int*/
  _b_3158_3156 = kk_integer_add(_x3577,_x3580,kk_context()); /*int*/
  kk_vector_t init_2799;
  kk_vector_t _brw_3369 = kk_std_core_vector_2(_b_3158_3156, kk_integer_box(kk_integer_from_small(0)), _ctx); /*vector<1001>*/;
  kk_integer_drop(_b_3158_3156, _ctx);
  init_2799 = _brw_3369; /*vector<int>*/
  kk_ref_t loc = kk_ref_alloc((kk_vector_box(init_2799, _ctx)),kk_context()); /*local-var<2600,vector<int>>*/;
  kk_integer_t end2_2638;
  kk_integer_t _x3581 = kk_std_core_length_2(header, _ctx); /*int*/
  end2_2638 = kk_integer_sub(_x3581,(kk_integer_from_small(1)),kk_context()); /*int*/
  kk_unit_t x_2801 = kk_Unit;
  kk_function_t _x3582;
  kk_vector_dup(header);
  kk_ref_dup(loc);
  _x3582 = kk_bitmap__new_mlift2733_get_bitmap_bytes_fun3583(header, loc, _ctx); /*(i00 : int) -> <exn,local<2600>,div> ()*/
  kk_std_core__lift17275_for(_x3582, end2_2638, kk_integer_from_small(0), _ctx);
  kk_vector_t res;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3588;
    kk_function_t _x3589;
    kk_ref_dup(loc);
    _x3589 = kk_bitmap__new_mlift2733_get_bitmap_bytes_fun3590(dib_header, header, loc, pixels, _ctx); /*(1001) -> 1003 1002*/
    _x3588 = kk_std_core_hnd_yield_extend(_x3589, _ctx); /*1002*/
    res = kk_vector_unbox(_x3588, _ctx); /*vector<int>*/
  }
  else {
    kk_ref_t _x3593 = kk_ref_dup(loc); /*local-var<2600,vector<int>>*/
    res = kk_bitmap__mlift2732_get_bitmap_bytes(dib_header, _x3593, header, pixels, x_2801, _ctx); /*vector<int>*/
  }
  kk_box_t _x3594 = kk_std_core_hnd_prompt_local_var(loc, kk_vector_box(res, _ctx), _ctx); /*1002*/
  return kk_vector_unbox(_x3594, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2735_get_bitmap_bytes_fun3597__t {
  struct kk_function_s _base;
  kk_ref_t pixels_index;
};
static kk_unit_t kk_bitmap__mlift2735_get_bitmap_bytes_fun3597(kk_function_t _fself, kk_integer_t _y_2681, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2735_get_bitmap_bytes_fun3597(kk_ref_t pixels_index, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2735_get_bitmap_bytes_fun3597__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2735_get_bitmap_bytes_fun3597__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2735_get_bitmap_bytes_fun3597, kk_context());
  _self->pixels_index = pixels_index;
  return &_self->_base;
}

static kk_unit_t kk_bitmap__mlift2735_get_bitmap_bytes_fun3597(kk_function_t _fself, kk_integer_t _y_2681, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2735_get_bitmap_bytes_fun3597__t* _self = kk_function_as(struct kk_bitmap__mlift2735_get_bitmap_bytes_fun3597__t*, _fself);
  kk_ref_t pixels_index = _self->pixels_index; /* local-var<2600,int> */
  kk_drop_match(_self, {kk_ref_dup(pixels_index);}, {}, _ctx)
  kk_integer_t _b_3199_3197 = kk_integer_add(_y_2681,(kk_integer_from_small(1)),kk_context()); /*int*/;
  kk_unit_t _brw_3362 = kk_Unit;
  (kk_ref_set_borrow(pixels_index,(kk_integer_box(_b_3199_3197)),kk_context()));
  kk_ref_drop(pixels_index, _ctx);
  return _brw_3362;
}


// lift anonymous function
struct kk_bitmap__mlift2735_get_bitmap_bytes_fun3599__t {
  struct kk_function_s _base;
  kk_function_t next_2809;
};
static kk_box_t kk_bitmap__mlift2735_get_bitmap_bytes_fun3599(kk_function_t _fself, kk_box_t _b_3201, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2735_get_bitmap_bytes_fun3599(kk_function_t next_2809, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2735_get_bitmap_bytes_fun3599__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2735_get_bitmap_bytes_fun3599__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2735_get_bitmap_bytes_fun3599, kk_context());
  _self->next_2809 = next_2809;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2735_get_bitmap_bytes_fun3599(kk_function_t _fself, kk_box_t _b_3201, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2735_get_bitmap_bytes_fun3599__t* _self = kk_function_as(struct kk_bitmap__mlift2735_get_bitmap_bytes_fun3599__t*, _fself);
  kk_function_t next_2809 = _self->next_2809; /* (int) -> <local<2600>,exn,div> () */
  kk_drop_match(_self, {kk_function_dup(next_2809);}, {}, _ctx)
  kk_unit_t _x3600 = kk_Unit;
  kk_integer_t _x3601 = kk_integer_unbox(_b_3201); /*int*/
  kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), next_2809, (next_2809, _x3601, _ctx));
  return kk_unit_box(_x3600);
}

kk_unit_t kk_bitmap__mlift2735_get_bitmap_bytes(kk_ref_t pixels_index, kk_unit_t wild__1, kk_context_t* _ctx) { /* forall<h> (pixels-index : local-var<h,int>, wild_1 : ()) -> <local<h>,exn,div> () */ 
  kk_integer_t x_2808;
  kk_box_t _x3595;
  kk_ref_t _x3596 = kk_ref_dup(pixels_index); /*local-var<2600,int>*/
  _x3595 = (kk_ref_get(_x3596,kk_context())); /*1000*/
  x_2808 = kk_integer_unbox(_x3595); /*int*/
  kk_function_t next_2809 = kk_bitmap__new_mlift2735_get_bitmap_bytes_fun3597(pixels_index, _ctx); /*(int) -> <local<2600>,exn,div> ()*/;
  if (kk_yielding(kk_context())) {
    kk_integer_drop(x_2808, _ctx);
    kk_box_t _x3598 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2735_get_bitmap_bytes_fun3599(next_2809, _ctx), _ctx); /*1002*/
    kk_unit_unbox(_x3598); return kk_Unit;
  }
  {
    kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), next_2809, (next_2809, x_2808, _ctx)); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2736_get_bitmap_bytes_fun3603__t {
  struct kk_function_s _base;
  kk_ref_t pixels_index;
};
static kk_box_t kk_bitmap__mlift2736_get_bitmap_bytes_fun3603(kk_function_t _fself, kk_box_t _b_3210, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2736_get_bitmap_bytes_fun3603(kk_ref_t pixels_index, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2736_get_bitmap_bytes_fun3603__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2736_get_bitmap_bytes_fun3603__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2736_get_bitmap_bytes_fun3603, kk_context());
  _self->pixels_index = pixels_index;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2736_get_bitmap_bytes_fun3603(kk_function_t _fself, kk_box_t _b_3210, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2736_get_bitmap_bytes_fun3603__t* _self = kk_function_as(struct kk_bitmap__mlift2736_get_bitmap_bytes_fun3603__t*, _fself);
  kk_ref_t pixels_index = _self->pixels_index; /* local-var<2600,int> */
  kk_drop_match(_self, {kk_ref_dup(pixels_index);}, {}, _ctx)
  kk_unit_t _x3604 = kk_Unit;
  kk_unit_t _x3605 = kk_Unit;
  kk_unit_unbox(_b_3210);
  kk_bitmap__mlift2735_get_bitmap_bytes(pixels_index, _x3605, _ctx);
  return kk_unit_box(_x3604);
}

kk_unit_t kk_bitmap__mlift2736_get_bitmap_bytes(kk_ref_t pixels, kk_ref_t pixels_index, kk_integer_t _y_2679, kk_context_t* _ctx) { /* forall<h> (pixels : local-var<h,vector<int>>, pixels-index : local-var<h,int>, int) -> <local<h>,exn,div> () */ 
  kk_unit_t x_2812 = kk_Unit;
  kk_unit_t _brw_3360 = kk_Unit;
  kk_ref_vector_assign_borrow(pixels,_y_2679,(kk_integer_box(kk_integer_from_small(0))),kk_context());
  kk_ref_drop(pixels, _ctx);
  kk_integer_drop(_y_2679, _ctx);
  _brw_3360;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3602 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2736_get_bitmap_bytes_fun3603(pixels_index, _ctx), _ctx); /*1002*/
    kk_unit_unbox(_x3602); return kk_Unit;
  }
  {
    kk_bitmap__mlift2735_get_bitmap_bytes(pixels_index, x_2812, _ctx); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2738_get_bitmap_bytes_fun3608__t {
  struct kk_function_s _base;
  kk_ref_t pixels_index;
};
static kk_unit_t kk_bitmap__mlift2738_get_bitmap_bytes_fun3608(kk_function_t _fself, kk_integer_t _y_2675, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2738_get_bitmap_bytes_fun3608(kk_ref_t pixels_index, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2738_get_bitmap_bytes_fun3608__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2738_get_bitmap_bytes_fun3608__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2738_get_bitmap_bytes_fun3608, kk_context());
  _self->pixels_index = pixels_index;
  return &_self->_base;
}

static kk_unit_t kk_bitmap__mlift2738_get_bitmap_bytes_fun3608(kk_function_t _fself, kk_integer_t _y_2675, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2738_get_bitmap_bytes_fun3608__t* _self = kk_function_as(struct kk_bitmap__mlift2738_get_bitmap_bytes_fun3608__t*, _fself);
  kk_ref_t pixels_index = _self->pixels_index; /* local-var<2600,int> */
  kk_drop_match(_self, {kk_ref_dup(pixels_index);}, {}, _ctx)
  kk_integer_t _b_3222_3220 = kk_integer_add(_y_2675,(kk_integer_from_small(1)),kk_context()); /*int*/;
  kk_unit_t _brw_3357 = kk_Unit;
  (kk_ref_set_borrow(pixels_index,(kk_integer_box(_b_3222_3220)),kk_context()));
  kk_ref_drop(pixels_index, _ctx);
  return _brw_3357;
}


// lift anonymous function
struct kk_bitmap__mlift2738_get_bitmap_bytes_fun3610__t {
  struct kk_function_s _base;
  kk_function_t next_2815;
};
static kk_box_t kk_bitmap__mlift2738_get_bitmap_bytes_fun3610(kk_function_t _fself, kk_box_t _b_3224, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2738_get_bitmap_bytes_fun3610(kk_function_t next_2815, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2738_get_bitmap_bytes_fun3610__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2738_get_bitmap_bytes_fun3610__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2738_get_bitmap_bytes_fun3610, kk_context());
  _self->next_2815 = next_2815;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2738_get_bitmap_bytes_fun3610(kk_function_t _fself, kk_box_t _b_3224, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2738_get_bitmap_bytes_fun3610__t* _self = kk_function_as(struct kk_bitmap__mlift2738_get_bitmap_bytes_fun3610__t*, _fself);
  kk_function_t next_2815 = _self->next_2815; /* (int) -> <local<2600>,exn,div> () */
  kk_drop_match(_self, {kk_function_dup(next_2815);}, {}, _ctx)
  kk_unit_t _x3611 = kk_Unit;
  kk_integer_t _x3612 = kk_integer_unbox(_b_3224); /*int*/
  kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), next_2815, (next_2815, _x3612, _ctx));
  return kk_unit_box(_x3611);
}

kk_unit_t kk_bitmap__mlift2738_get_bitmap_bytes(kk_ref_t pixels_index, kk_unit_t wild__, kk_context_t* _ctx) { /* forall<h> (pixels-index : local-var<h,int>, wild_ : ()) -> <local<h>,exn,div> () */ 
  kk_integer_t x_2814;
  kk_box_t _x3606;
  kk_ref_t _x3607 = kk_ref_dup(pixels_index); /*local-var<2600,int>*/
  _x3606 = (kk_ref_get(_x3607,kk_context())); /*1000*/
  x_2814 = kk_integer_unbox(_x3606); /*int*/
  kk_function_t next_2815 = kk_bitmap__new_mlift2738_get_bitmap_bytes_fun3608(pixels_index, _ctx); /*(int) -> <local<2600>,exn,div> ()*/;
  if (kk_yielding(kk_context())) {
    kk_integer_drop(x_2814, _ctx);
    kk_box_t _x3609 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2738_get_bitmap_bytes_fun3610(next_2815, _ctx), _ctx); /*1002*/
    kk_unit_unbox(_x3609); return kk_Unit;
  }
  {
    kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), next_2815, (next_2815, x_2814, _ctx)); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2739_get_bitmap_bytes_fun3614__t {
  struct kk_function_s _base;
  kk_ref_t pixels_index;
};
static kk_box_t kk_bitmap__mlift2739_get_bitmap_bytes_fun3614(kk_function_t _fself, kk_box_t _b_3233, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2739_get_bitmap_bytes_fun3614(kk_ref_t pixels_index, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2739_get_bitmap_bytes_fun3614__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2739_get_bitmap_bytes_fun3614__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2739_get_bitmap_bytes_fun3614, kk_context());
  _self->pixels_index = pixels_index;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2739_get_bitmap_bytes_fun3614(kk_function_t _fself, kk_box_t _b_3233, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2739_get_bitmap_bytes_fun3614__t* _self = kk_function_as(struct kk_bitmap__mlift2739_get_bitmap_bytes_fun3614__t*, _fself);
  kk_ref_t pixels_index = _self->pixels_index; /* local-var<2600,int> */
  kk_drop_match(_self, {kk_ref_dup(pixels_index);}, {}, _ctx)
  kk_unit_t _x3615 = kk_Unit;
  kk_unit_t _x3616 = kk_Unit;
  kk_unit_unbox(_b_3233);
  kk_bitmap__mlift2738_get_bitmap_bytes(pixels_index, _x3616, _ctx);
  return kk_unit_box(_x3615);
}

kk_unit_t kk_bitmap__mlift2739_get_bitmap_bytes(kk_ref_t pixels, kk_ref_t pixels_index, kk_integer_t x, kk_integer_t _y_2673, kk_context_t* _ctx) { /* forall<h> (pixels : local-var<h,vector<int>>, pixels-index : local-var<h,int>, x : int, int) -> <local<h>,exn,div> () */ 
  kk_unit_t x0_2818 = kk_Unit;
  kk_unit_t _brw_3355 = kk_Unit;
  kk_ref_vector_assign_borrow(pixels,_y_2673,(kk_integer_box(x)),kk_context());
  kk_ref_drop(pixels, _ctx);
  kk_integer_drop(_y_2673, _ctx);
  _brw_3355;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3613 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2739_get_bitmap_bytes_fun3614(pixels_index, _ctx), _ctx); /*1002*/
    kk_unit_unbox(_x3613); return kk_Unit;
  }
  {
    kk_bitmap__mlift2738_get_bitmap_bytes(pixels_index, x0_2818, _ctx); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3618__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap__mlift2740_get_bitmap_bytes_fun3618(kk_function_t _fself, kk_box_t _b_3241, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2740_get_bitmap_bytes_fun3618(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap__mlift2740_get_bitmap_bytes_fun3618, _ctx)
  return kk_function_dup(_fself);
}



// lift anonymous function
struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3620__t {
  struct kk_function_s _base;
  kk_box_t _b_3241;
};
static kk_box_t kk_bitmap__mlift2740_get_bitmap_bytes_fun3620(kk_function_t _fself, kk_ssize_t _b_3238, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2740_get_bitmap_bytes_fun3620(kk_box_t _b_3241, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3620__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3620__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2740_get_bitmap_bytes_fun3620, kk_context());
  _self->_b_3241 = _b_3241;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2740_get_bitmap_bytes_fun3620(kk_function_t _fself, kk_ssize_t _b_3238, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3620__t* _self = kk_function_as(struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3620__t*, _fself);
  kk_box_t _b_3241 = _self->_b_3241; /* 1001 */
  kk_drop_match(_self, {kk_box_dup(_b_3241);}, {}, _ctx)
  kk_integer_t _x3621;
  kk_integer_t i0_2646 = kk_integer_from_ssize_t(_b_3238,kk_context()); /*int*/;
  if (kk_integer_eq_borrow(i0_2646, kk_integer_from_small(0), _ctx)) {
    kk_integer_drop(i0_2646, _ctx);
    kk_bitmap__colour _match_3353 = kk_bitmap__colour_unbox(_b_3241, _ctx); /*bitmap/colour*/;
    {
      kk_integer_t _x = _match_3353.blue;
      kk_integer_dup(_x);
      kk_bitmap__colour_drop(_match_3353, _ctx);
      _x3621 = _x; /*int*/
      goto _match3622;
    }
  }
  if (kk_integer_eq_borrow(i0_2646, kk_integer_from_small(1), _ctx)) {
    kk_integer_drop(i0_2646, _ctx);
    kk_bitmap__colour _match_3352 = kk_bitmap__colour_unbox(_b_3241, _ctx); /*bitmap/colour*/;
    {
      kk_integer_t _x01 = _match_3352.green;
      kk_integer_dup(_x01);
      kk_bitmap__colour_drop(_match_3352, _ctx);
      _x3621 = _x01; /*int*/
      goto _match3622;
    }
  }
  if (kk_integer_eq_borrow(i0_2646, kk_integer_from_small(2), _ctx)) {
    kk_integer_drop(i0_2646, _ctx);
    kk_bitmap__colour _match_3351 = kk_bitmap__colour_unbox(_b_3241, _ctx); /*bitmap/colour*/;
    {
      kk_integer_t _x11 = _match_3351.red;
      kk_integer_dup(_x11);
      kk_bitmap__colour_drop(_match_3351, _ctx);
      _x3621 = _x11; /*int*/
      goto _match3622;
    }
  }
  {
    kk_integer_drop(i0_2646, _ctx);
    kk_box_drop(_b_3241, _ctx);
    _x3621 = kk_integer_from_small(0); /*int*/
  }
  _match3622: ;
  return kk_integer_box(_x3621);
}
static kk_box_t kk_bitmap__mlift2740_get_bitmap_bytes_fun3618(kk_function_t _fself, kk_box_t _b_3241, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_vector_t _x3619;
  kk_ssize_t _b_3244_3236 = ((kk_ssize_t)3); /*ssize_t*/;
  _x3619 = kk_std_core_vector_initz(_b_3244_3236, kk_bitmap__new_mlift2740_get_bitmap_bytes_fun3620(_b_3241, _ctx), _ctx); /*vector<1001>*/
  return kk_vector_box(_x3619, _ctx);
}


// lift anonymous function
struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3623__t {
  struct kk_function_s _base;
  kk_ref_t pixels;
  kk_ref_t pixels_index;
};
static kk_unit_t kk_bitmap__mlift2740_get_bitmap_bytes_fun3623(kk_function_t _fself, kk_box_t _b_3254, kk_ssize_t _b_3255, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2740_get_bitmap_bytes_fun3623(kk_ref_t pixels, kk_ref_t pixels_index, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3623__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3623__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2740_get_bitmap_bytes_fun3623, kk_context());
  _self->pixels = pixels;
  _self->pixels_index = pixels_index;
  return &_self->_base;
}



// lift anonymous function
struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3627__t {
  struct kk_function_s _base;
  kk_box_t _b_3254;
  kk_ref_t pixels;
  kk_ref_t pixels_index;
};
static kk_box_t kk_bitmap__mlift2740_get_bitmap_bytes_fun3627(kk_function_t _fself, kk_box_t _b_3251, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2740_get_bitmap_bytes_fun3627(kk_box_t _b_3254, kk_ref_t pixels, kk_ref_t pixels_index, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3627__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3627__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2740_get_bitmap_bytes_fun3627, kk_context());
  _self->_b_3254 = _b_3254;
  _self->pixels = pixels;
  _self->pixels_index = pixels_index;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2740_get_bitmap_bytes_fun3627(kk_function_t _fself, kk_box_t _b_3251, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3627__t* _self = kk_function_as(struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3627__t*, _fself);
  kk_box_t _b_3254 = _self->_b_3254; /* 1001 */
  kk_ref_t pixels = _self->pixels; /* local-var<2600,vector<int>> */
  kk_ref_t pixels_index = _self->pixels_index; /* local-var<2600,int> */
  kk_drop_match(_self, {kk_box_dup(_b_3254);kk_ref_dup(pixels);kk_ref_dup(pixels_index);}, {}, _ctx)
  kk_unit_t _x3628 = kk_Unit;
  kk_integer_t _x3629 = kk_integer_unbox(_b_3254); /*int*/
  kk_integer_t _x3630 = kk_integer_unbox(_b_3251); /*int*/
  kk_bitmap__mlift2739_get_bitmap_bytes(pixels, pixels_index, _x3629, _x3630, _ctx);
  return kk_unit_box(_x3628);
}
static kk_unit_t kk_bitmap__mlift2740_get_bitmap_bytes_fun3623(kk_function_t _fself, kk_box_t _b_3254, kk_ssize_t _b_3255, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3623__t* _self = kk_function_as(struct kk_bitmap__mlift2740_get_bitmap_bytes_fun3623__t*, _fself);
  kk_ref_t pixels = _self->pixels; /* local-var<2600,vector<int>> */
  kk_ref_t pixels_index = _self->pixels_index; /* local-var<2600,int> */
  kk_drop_match(_self, {kk_ref_dup(pixels);kk_ref_dup(pixels_index);}, {}, _ctx)
  kk_integer_t x0_2820;
  kk_box_t _x3624;
  kk_ref_t _x3625 = kk_ref_dup(pixels_index); /*local-var<2600,int>*/
  _x3624 = (kk_ref_get(_x3625,kk_context())); /*1000*/
  x0_2820 = kk_integer_unbox(_x3624); /*int*/
  if (kk_yielding(kk_context())) {
    kk_integer_drop(x0_2820, _ctx);
    kk_box_t _x3626 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2740_get_bitmap_bytes_fun3627(_b_3254, pixels, pixels_index, _ctx), _ctx); /*1002*/
    return kk_unit_unbox(_x3626);
  }
  {
    kk_integer_t _x3631 = kk_integer_unbox(_b_3254); /*int*/
    return kk_bitmap__mlift2739_get_bitmap_bytes(pixels, pixels_index, _x3631, x0_2820, _ctx);
  }
}

kk_unit_t kk_bitmap__mlift2740_get_bitmap_bytes(kk_ref_t pixels, kk_ref_t pixels_index, kk_bitmap__colour _y_2672, kk_context_t* _ctx) { /* forall<h> (pixels : local-var<h,vector<int>>, pixels-index : local-var<h,int>, colour) -> exn () */ 
  kk_vector_t colour0;
  kk_box_t _x3617 = kk_std_core_hnd__open_none1(kk_bitmap__new_mlift2740_get_bitmap_bytes_fun3618(_ctx), kk_bitmap__colour_box(_y_2672, _ctx), _ctx); /*1002*/
  colour0 = kk_vector_unbox(_x3617, _ctx); /*vector<int>*/
  kk_std_core_foreach_indexedz(colour0, kk_bitmap__new_mlift2740_get_bitmap_bytes_fun3623(pixels, pixels_index, _ctx), _ctx); return kk_Unit;
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2741_get_bitmap_bytes_fun3634__t {
  struct kk_function_s _base;
  kk_ref_t pixels;
  kk_ref_t pixels_index;
};
static kk_box_t kk_bitmap__mlift2741_get_bitmap_bytes_fun3634(kk_function_t _fself, kk_box_t _b_3267, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2741_get_bitmap_bytes_fun3634(kk_ref_t pixels, kk_ref_t pixels_index, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2741_get_bitmap_bytes_fun3634__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2741_get_bitmap_bytes_fun3634__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2741_get_bitmap_bytes_fun3634, kk_context());
  _self->pixels = pixels;
  _self->pixels_index = pixels_index;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2741_get_bitmap_bytes_fun3634(kk_function_t _fself, kk_box_t _b_3267, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2741_get_bitmap_bytes_fun3634__t* _self = kk_function_as(struct kk_bitmap__mlift2741_get_bitmap_bytes_fun3634__t*, _fself);
  kk_ref_t pixels = _self->pixels; /* local-var<2600,vector<int>> */
  kk_ref_t pixels_index = _self->pixels_index; /* local-var<2600,int> */
  kk_drop_match(_self, {kk_ref_dup(pixels);kk_ref_dup(pixels_index);}, {}, _ctx)
  kk_unit_t _x3635 = kk_Unit;
  kk_bitmap__colour _x3636 = kk_bitmap__colour_unbox(_b_3267, _ctx); /*bitmap/colour*/
  kk_bitmap__mlift2740_get_bitmap_bytes(pixels, pixels_index, _x3636, _ctx);
  return kk_unit_box(_x3635);
}

kk_unit_t kk_bitmap__mlift2741_get_bitmap_bytes(kk_integer_t j, kk_ref_t pixels, kk_ref_t pixels_index, kk_vector_t _y_2671, kk_context_t* _ctx) { /* forall<h> (j : int, pixels : local-var<h,vector<int>>, pixels-index : local-var<h,int>, vector<colour>) -> exn () */ 
  kk_bitmap__colour x_2822;
  kk_box_t _x3632;
  kk_box_t _brw_3349 = kk_vector_at_int_borrow(_y_2671, j); /*1001*/;
  kk_vector_drop(_y_2671, _ctx);
  kk_integer_drop(j, _ctx);
  _x3632 = _brw_3349; /*1001*/
  x_2822 = kk_bitmap__colour_unbox(_x3632, _ctx); /*bitmap/colour*/
  if (kk_yielding(kk_context())) {
    kk_bitmap__colour_drop(x_2822, _ctx);
    kk_box_t _x3633 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2741_get_bitmap_bytes_fun3634(pixels, pixels_index, _ctx), _ctx); /*1002*/
    kk_unit_unbox(_x3633); return kk_Unit;
  }
  {
    kk_bitmap__mlift2740_get_bitmap_bytes(pixels, pixels_index, x_2822, _ctx); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2742_get_bitmap_bytes_fun3637__t {
  struct kk_function_s _base;
  kk_ref_t pixels;
  kk_ref_t pixels_index;
};
static kk_unit_t kk_bitmap__mlift2742_get_bitmap_bytes_fun3637(kk_function_t _fself, kk_integer_t padding_index, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2742_get_bitmap_bytes_fun3637(kk_ref_t pixels, kk_ref_t pixels_index, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2742_get_bitmap_bytes_fun3637__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2742_get_bitmap_bytes_fun3637__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2742_get_bitmap_bytes_fun3637, kk_context());
  _self->pixels = pixels;
  _self->pixels_index = pixels_index;
  return &_self->_base;
}



// lift anonymous function
struct kk_bitmap__mlift2742_get_bitmap_bytes_fun3641__t {
  struct kk_function_s _base;
  kk_ref_t pixels;
  kk_ref_t pixels_index;
};
static kk_box_t kk_bitmap__mlift2742_get_bitmap_bytes_fun3641(kk_function_t _fself, kk_box_t _b_3273, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2742_get_bitmap_bytes_fun3641(kk_ref_t pixels, kk_ref_t pixels_index, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2742_get_bitmap_bytes_fun3641__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2742_get_bitmap_bytes_fun3641__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2742_get_bitmap_bytes_fun3641, kk_context());
  _self->pixels = pixels;
  _self->pixels_index = pixels_index;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2742_get_bitmap_bytes_fun3641(kk_function_t _fself, kk_box_t _b_3273, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2742_get_bitmap_bytes_fun3641__t* _self = kk_function_as(struct kk_bitmap__mlift2742_get_bitmap_bytes_fun3641__t*, _fself);
  kk_ref_t pixels = _self->pixels; /* local-var<2600,vector<int>> */
  kk_ref_t pixels_index = _self->pixels_index; /* local-var<2600,int> */
  kk_drop_match(_self, {kk_ref_dup(pixels);kk_ref_dup(pixels_index);}, {}, _ctx)
  kk_unit_t _x3642 = kk_Unit;
  kk_integer_t _x3643 = kk_integer_unbox(_b_3273); /*int*/
  kk_bitmap__mlift2736_get_bitmap_bytes(pixels, pixels_index, _x3643, _ctx);
  return kk_unit_box(_x3642);
}
static kk_unit_t kk_bitmap__mlift2742_get_bitmap_bytes_fun3637(kk_function_t _fself, kk_integer_t padding_index, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2742_get_bitmap_bytes_fun3637__t* _self = kk_function_as(struct kk_bitmap__mlift2742_get_bitmap_bytes_fun3637__t*, _fself);
  kk_ref_t pixels = _self->pixels; /* local-var<2600,vector<int>> */
  kk_ref_t pixels_index = _self->pixels_index; /* local-var<2600,int> */
  kk_drop_match(_self, {kk_ref_dup(pixels);kk_ref_dup(pixels_index);}, {}, _ctx)
  kk_integer_drop(padding_index, _ctx);
  kk_integer_t x_2824;
  kk_box_t _x3638;
  kk_ref_t _x3639 = kk_ref_dup(pixels_index); /*local-var<2600,int>*/
  _x3638 = (kk_ref_get(_x3639,kk_context())); /*1000*/
  x_2824 = kk_integer_unbox(_x3638); /*int*/
  if (kk_yielding(kk_context())) {
    kk_integer_drop(x_2824, _ctx);
    kk_box_t _x3640 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2742_get_bitmap_bytes_fun3641(pixels, pixels_index, _ctx), _ctx); /*1002*/
    return kk_unit_unbox(_x3640);
  }
  {
    return kk_bitmap__mlift2736_get_bitmap_bytes(pixels, pixels_index, x_2824, _ctx);
  }
}

kk_unit_t kk_bitmap__mlift2742_get_bitmap_bytes(kk_integer_t padding, kk_ref_t pixels, kk_ref_t pixels_index, kk_unit_t wild__0, kk_context_t* _ctx) { /* forall<h> (padding : int, pixels : local-var<h,vector<int>>, pixels-index : local-var<h,int>, wild_0 : ()) -> <exn,local<h>,div> () */ 
  kk_std_core__lift17275_for(kk_bitmap__new_mlift2742_get_bitmap_bytes_fun3637(pixels, pixels_index, _ctx), padding, kk_integer_from_small(1), _ctx); return kk_Unit;
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2743_get_bitmap_bytes_fun3647__t {
  struct kk_function_s _base;
  kk_vector_t dib_header;
  kk_vector_t header;
  kk_ref_t pixels;
};
static kk_box_t kk_bitmap__mlift2743_get_bitmap_bytes_fun3647(kk_function_t _fself, kk_box_t _b_3279, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2743_get_bitmap_bytes_fun3647(kk_vector_t dib_header, kk_vector_t header, kk_ref_t pixels, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2743_get_bitmap_bytes_fun3647__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2743_get_bitmap_bytes_fun3647__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2743_get_bitmap_bytes_fun3647, kk_context());
  _self->dib_header = dib_header;
  _self->header = header;
  _self->pixels = pixels;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2743_get_bitmap_bytes_fun3647(kk_function_t _fself, kk_box_t _b_3279, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2743_get_bitmap_bytes_fun3647__t* _self = kk_function_as(struct kk_bitmap__mlift2743_get_bitmap_bytes_fun3647__t*, _fself);
  kk_vector_t dib_header = _self->dib_header; /* vector<int> */
  kk_vector_t header = _self->header; /* vector<int> */
  kk_ref_t pixels = _self->pixels; /* local-var<2600,vector<int>> */
  kk_drop_match(_self, {kk_vector_dup(dib_header);kk_vector_dup(header);kk_ref_dup(pixels);}, {}, _ctx)
  kk_vector_t _x3648;
  kk_vector_t _x3649 = kk_vector_unbox(_b_3279, _ctx); /*vector<int>*/
  _x3648 = kk_bitmap__mlift2733_get_bitmap_bytes(dib_header, header, pixels, _x3649, _ctx); /*vector<int>*/
  return kk_vector_box(_x3648, _ctx);
}

kk_vector_t kk_bitmap__mlift2743_get_bitmap_bytes(kk_vector_t dib_header, kk_vector_t header, kk_ref_t pixels, kk_unit_t wild__2, kk_context_t* _ctx) { /* forall<h> (dib-header : vector<int>, header : vector<int>, pixels : local-var<h,vector<int>>, wild_2 : ()) -> <exn,local<h>,div> vector<int> */ 
  kk_vector_t x_2826;
  kk_box_t _x3644;
  kk_ref_t _x3645 = kk_ref_dup(pixels); /*local-var<2600,vector<int>>*/
  _x3644 = (kk_ref_get(_x3645,kk_context())); /*1000*/
  x_2826 = kk_vector_unbox(_x3644, _ctx); /*vector<int>*/
  if (kk_yielding(kk_context())) {
    kk_vector_drop(x_2826, _ctx);
    kk_box_t _x3646 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2743_get_bitmap_bytes_fun3647(dib_header, header, pixels, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x3646, _ctx);
  }
  {
    return kk_bitmap__mlift2733_get_bitmap_bytes(dib_header, header, pixels, x_2826, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3659__t {
  struct kk_function_s _base;
  kk_integer_t horizontal_resolution;
  kk_ref_t loc;
  kk_ref_t loc0;
  kk_integer_t padding;
  kk_vector_t screen;
};
static kk_unit_t kk_bitmap__mlift2744_get_bitmap_bytes_fun3659(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2744_get_bitmap_bytes_fun3659(kk_integer_t horizontal_resolution, kk_ref_t loc, kk_ref_t loc0, kk_integer_t padding, kk_vector_t screen, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3659__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3659__t, 6, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2744_get_bitmap_bytes_fun3659, kk_context());
  _self->horizontal_resolution = horizontal_resolution;
  _self->loc = loc;
  _self->loc0 = loc0;
  _self->padding = padding;
  _self->screen = screen;
  return &_self->_base;
}



// lift anonymous function
struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3661__t {
  struct kk_function_s _base;
  kk_integer_t i;
  kk_ref_t loc;
  kk_ref_t loc0;
  kk_vector_t screen;
};
static kk_unit_t kk_bitmap__mlift2744_get_bitmap_bytes_fun3661(kk_function_t _fself, kk_integer_t j, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2744_get_bitmap_bytes_fun3661(kk_integer_t i, kk_ref_t loc, kk_ref_t loc0, kk_vector_t screen, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3661__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3661__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2744_get_bitmap_bytes_fun3661, kk_context());
  _self->i = i;
  _self->loc = loc;
  _self->loc0 = loc0;
  _self->screen = screen;
  return &_self->_base;
}



// lift anonymous function
struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3664__t {
  struct kk_function_s _base;
  kk_integer_t j;
  kk_ref_t loc;
  kk_ref_t loc0;
};
static kk_box_t kk_bitmap__mlift2744_get_bitmap_bytes_fun3664(kk_function_t _fself, kk_box_t _b_3295, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2744_get_bitmap_bytes_fun3664(kk_integer_t j, kk_ref_t loc, kk_ref_t loc0, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3664__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3664__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2744_get_bitmap_bytes_fun3664, kk_context());
  _self->j = j;
  _self->loc = loc;
  _self->loc0 = loc0;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2744_get_bitmap_bytes_fun3664(kk_function_t _fself, kk_box_t _b_3295, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3664__t* _self = kk_function_as(struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3664__t*, _fself);
  kk_integer_t j = _self->j; /* int */
  kk_ref_t loc = _self->loc; /* local-var<2600,vector<int>> */
  kk_ref_t loc0 = _self->loc0; /* local-var<2600,int> */
  kk_drop_match(_self, {kk_integer_dup(j);kk_ref_dup(loc);kk_ref_dup(loc0);}, {}, _ctx)
  kk_unit_t _x3665 = kk_Unit;
  kk_vector_t _x3666 = kk_vector_unbox(_b_3295, _ctx); /*vector<bitmap/colour>*/
  kk_bitmap__mlift2741_get_bitmap_bytes(j, loc, loc0, _x3666, _ctx);
  return kk_unit_box(_x3665);
}
static kk_unit_t kk_bitmap__mlift2744_get_bitmap_bytes_fun3661(kk_function_t _fself, kk_integer_t j, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3661__t* _self = kk_function_as(struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3661__t*, _fself);
  kk_integer_t i = _self->i; /* int */
  kk_ref_t loc = _self->loc; /* local-var<2600,vector<int>> */
  kk_ref_t loc0 = _self->loc0; /* local-var<2600,int> */
  kk_vector_t screen = _self->screen; /* vector<vector<bitmap/colour>> */
  kk_drop_match(_self, {kk_integer_dup(i);kk_ref_dup(loc);kk_ref_dup(loc0);kk_vector_dup(screen);}, {}, _ctx)
  kk_vector_t x1_2836;
  kk_box_t _x3662;
  kk_box_t _brw_3344 = kk_vector_at_int_borrow(screen, i); /*1001*/;
  kk_vector_drop(screen, _ctx);
  kk_integer_drop(i, _ctx);
  _x3662 = _brw_3344; /*1001*/
  x1_2836 = kk_vector_unbox(_x3662, _ctx); /*vector<bitmap/colour>*/
  if (kk_yielding(kk_context())) {
    kk_vector_drop(x1_2836, _ctx);
    kk_box_t _x3663 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2744_get_bitmap_bytes_fun3664(j, loc, loc0, _ctx), _ctx); /*1002*/
    return kk_unit_unbox(_x3663);
  }
  {
    return kk_bitmap__mlift2741_get_bitmap_bytes(j, loc, loc0, x1_2836, _ctx);
  }
}


// lift anonymous function
struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3668__t {
  struct kk_function_s _base;
  kk_ref_t loc;
  kk_ref_t loc0;
  kk_integer_t padding;
};
static kk_box_t kk_bitmap__mlift2744_get_bitmap_bytes_fun3668(kk_function_t _fself, kk_box_t _b_3299, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2744_get_bitmap_bytes_fun3668(kk_ref_t loc, kk_ref_t loc0, kk_integer_t padding, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3668__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3668__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2744_get_bitmap_bytes_fun3668, kk_context());
  _self->loc = loc;
  _self->loc0 = loc0;
  _self->padding = padding;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2744_get_bitmap_bytes_fun3668(kk_function_t _fself, kk_box_t _b_3299, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3668__t* _self = kk_function_as(struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3668__t*, _fself);
  kk_ref_t loc = _self->loc; /* local-var<2600,vector<int>> */
  kk_ref_t loc0 = _self->loc0; /* local-var<2600,int> */
  kk_integer_t padding = _self->padding; /* int */
  kk_drop_match(_self, {kk_ref_dup(loc);kk_ref_dup(loc0);kk_integer_dup(padding);}, {}, _ctx)
  kk_unit_t _x3669 = kk_Unit;
  kk_unit_t _x3670 = kk_Unit;
  kk_unit_unbox(_b_3299);
  kk_bitmap__mlift2742_get_bitmap_bytes(padding, loc, loc0, _x3670, _ctx);
  return kk_unit_box(_x3669);
}
static kk_unit_t kk_bitmap__mlift2744_get_bitmap_bytes_fun3659(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3659__t* _self = kk_function_as(struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3659__t*, _fself);
  kk_integer_t horizontal_resolution = _self->horizontal_resolution; /* int */
  kk_ref_t loc = _self->loc; /* local-var<2600,vector<int>> */
  kk_ref_t loc0 = _self->loc0; /* local-var<2600,int> */
  kk_integer_t padding = _self->padding; /* int */
  kk_vector_t screen = _self->screen; /* vector<vector<bitmap/colour>> */
  kk_drop_match(_self, {kk_integer_dup(horizontal_resolution);kk_ref_dup(loc);kk_ref_dup(loc0);kk_integer_dup(padding);kk_vector_dup(screen);}, {}, _ctx)
  kk_integer_t end0_2630 = kk_integer_sub(horizontal_resolution,(kk_integer_from_small(1)),kk_context()); /*int*/;
  kk_unit_t x0_2834 = kk_Unit;
  kk_function_t _x3660;
  kk_ref_dup(loc);
  kk_ref_dup(loc0);
  _x3660 = kk_bitmap__new_mlift2744_get_bitmap_bytes_fun3661(i, loc, loc0, screen, _ctx); /*(j : int) -> <exn,local<2600>,div> ()*/
  kk_std_core__lift17275_for(_x3660, end0_2630, kk_integer_from_small(0), _ctx);
  if (kk_yielding(kk_context())) {
    kk_box_t _x3667 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2744_get_bitmap_bytes_fun3668(loc, loc0, padding, _ctx), _ctx); /*1002*/
    return kk_unit_unbox(_x3667);
  }
  {
    return kk_bitmap__mlift2742_get_bitmap_bytes(padding, loc, loc0, x0_2834, _ctx);
  }
}


// lift anonymous function
struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3673__t {
  struct kk_function_s _base;
  kk_vector_t dib_header;
  kk_vector_t header;
  kk_ref_t loc;
};
static kk_box_t kk_bitmap__mlift2744_get_bitmap_bytes_fun3673(kk_function_t _fself, kk_box_t _b_3303, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2744_get_bitmap_bytes_fun3673(kk_vector_t dib_header, kk_vector_t header, kk_ref_t loc, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3673__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3673__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2744_get_bitmap_bytes_fun3673, kk_context());
  _self->dib_header = dib_header;
  _self->header = header;
  _self->loc = loc;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2744_get_bitmap_bytes_fun3673(kk_function_t _fself, kk_box_t _b_3303, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3673__t* _self = kk_function_as(struct kk_bitmap__mlift2744_get_bitmap_bytes_fun3673__t*, _fself);
  kk_vector_t dib_header = _self->dib_header; /* vector<int> */
  kk_vector_t header = _self->header; /* vector<int> */
  kk_ref_t loc = _self->loc; /* local-var<2600,vector<int>> */
  kk_drop_match(_self, {kk_vector_dup(dib_header);kk_vector_dup(header);kk_ref_dup(loc);}, {}, _ctx)
  kk_vector_t _x3674;
  kk_unit_t _x3675 = kk_Unit;
  kk_unit_unbox(_b_3303);
  _x3674 = kk_bitmap__mlift2743_get_bitmap_bytes(dib_header, header, loc, _x3675, _ctx); /*vector<int>*/
  return kk_vector_box(_x3674, _ctx);
}

kk_vector_t kk_bitmap__mlift2744_get_bitmap_bytes(kk_vector_t header, kk_integer_t horizontal_resolution, kk_vector_t screen, kk_integer_t vertical_resolution, kk_vector_t dib_header, kk_context_t* _ctx) { /* forall<h> (header : vector<int>, horizontal-resolution : int, screen : vector<vector<colour>>, vertical-resolution : int, dib-header : vector<int>) -> pure vector<int> */ 
  kk_integer_t padding;
  kk_integer_t _x3650 = kk_integer_dup(horizontal_resolution); /*int*/
  padding = kk_integer_mod(_x3650,(kk_integer_from_small(4)),kk_context()); /*int*/
  kk_integer_t pixels_length;
  kk_integer_t _x3651;
  kk_integer_t _x3652;
  kk_integer_t _x3653 = kk_integer_dup(horizontal_resolution); /*int*/
  kk_integer_t _x3654 = kk_integer_dup(vertical_resolution); /*int*/
  _x3652 = kk_integer_mul(_x3653,_x3654,kk_context()); /*int*/
  _x3651 = kk_integer_mul((kk_integer_from_small(3)),_x3652,kk_context()); /*int*/
  kk_integer_t _x3655;
  kk_integer_t _x3656 = kk_integer_dup(padding); /*int*/
  kk_integer_t _x3657 = kk_integer_dup(vertical_resolution); /*int*/
  _x3655 = kk_integer_mul(_x3656,_x3657,kk_context()); /*int*/
  pixels_length = kk_integer_add(_x3651,_x3655,kk_context()); /*int*/
  kk_vector_t init_2828;
  kk_vector_t _brw_3345 = kk_std_core_vector_2(pixels_length, kk_integer_box(kk_integer_from_small(0)), _ctx); /*vector<1001>*/;
  kk_integer_drop(pixels_length, _ctx);
  init_2828 = _brw_3345; /*vector<int>*/
  kk_ref_t loc = kk_ref_alloc((kk_vector_box(init_2828, _ctx)),kk_context()); /*local-var<2600,vector<int>>*/;
  kk_ref_t loc0 = kk_ref_alloc((kk_integer_box(kk_integer_from_small(0))),kk_context()); /*local-var<2600,int>*/;
  kk_integer_t end_2627 = kk_integer_sub(vertical_resolution,(kk_integer_from_small(1)),kk_context()); /*int*/;
  kk_unit_t x_2832 = kk_Unit;
  kk_function_t _x3658;
  kk_ref_dup(loc);
  kk_ref_dup(loc0);
  _x3658 = kk_bitmap__new_mlift2744_get_bitmap_bytes_fun3659(horizontal_resolution, loc, loc0, padding, screen, _ctx); /*(i : int) -> <exn,local<2600>,div> ()*/
  kk_std_core__lift17275_for(_x3658, end_2627, kk_integer_from_small(0), _ctx);
  kk_vector_t res0;
  if (kk_yielding(kk_context())) {
    kk_box_t _x3671;
    kk_function_t _x3672;
    kk_ref_dup(loc);
    _x3672 = kk_bitmap__new_mlift2744_get_bitmap_bytes_fun3673(dib_header, header, loc, _ctx); /*(1001) -> 1003 1002*/
    _x3671 = kk_std_core_hnd_yield_extend(_x3672, _ctx); /*1002*/
    res0 = kk_vector_unbox(_x3671, _ctx); /*vector<int>*/
  }
  else {
    kk_ref_t _x3676 = kk_ref_dup(loc); /*local-var<2600,vector<int>>*/
    res0 = kk_bitmap__mlift2743_get_bitmap_bytes(dib_header, header, _x3676, x_2832, _ctx); /*vector<int>*/
  }
  kk_vector_t res;
  kk_box_t _x3677 = kk_std_core_hnd_prompt_local_var(loc0, kk_vector_box(res0, _ctx), _ctx); /*1002*/
  res = kk_vector_unbox(_x3677, _ctx); /*vector<int>*/
  kk_box_t _x3678 = kk_std_core_hnd_prompt_local_var(loc, kk_vector_box(res, _ctx), _ctx); /*1002*/
  return kk_vector_unbox(_x3678, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2745_get_bitmap_bytes_fun3682__t {
  struct kk_function_s _base;
  kk_vector_t header;
  kk_integer_t horizontal_resolution;
  kk_vector_t screen;
  kk_integer_t vertical_resolution;
};
static kk_box_t kk_bitmap__mlift2745_get_bitmap_bytes_fun3682(kk_function_t _fself, kk_box_t _b_3315, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2745_get_bitmap_bytes_fun3682(kk_vector_t header, kk_integer_t horizontal_resolution, kk_vector_t screen, kk_integer_t vertical_resolution, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2745_get_bitmap_bytes_fun3682__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2745_get_bitmap_bytes_fun3682__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2745_get_bitmap_bytes_fun3682, kk_context());
  _self->header = header;
  _self->horizontal_resolution = horizontal_resolution;
  _self->screen = screen;
  _self->vertical_resolution = vertical_resolution;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2745_get_bitmap_bytes_fun3682(kk_function_t _fself, kk_box_t _b_3315, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2745_get_bitmap_bytes_fun3682__t* _self = kk_function_as(struct kk_bitmap__mlift2745_get_bitmap_bytes_fun3682__t*, _fself);
  kk_vector_t header = _self->header; /* vector<int> */
  kk_integer_t horizontal_resolution = _self->horizontal_resolution; /* int */
  kk_vector_t screen = _self->screen; /* vector<vector<bitmap/colour>> */
  kk_integer_t vertical_resolution = _self->vertical_resolution; /* int */
  kk_drop_match(_self, {kk_vector_dup(header);kk_integer_dup(horizontal_resolution);kk_vector_dup(screen);kk_integer_dup(vertical_resolution);}, {}, _ctx)
  kk_vector_t _x3683;
  kk_vector_t _x3684 = kk_vector_unbox(_b_3315, _ctx); /*vector<int>*/
  _x3683 = kk_bitmap__mlift2744_get_bitmap_bytes(header, horizontal_resolution, screen, vertical_resolution, _x3684, _ctx); /*vector<int>*/
  return kk_vector_box(_x3683, _ctx);
}

kk_vector_t kk_bitmap__mlift2745_get_bitmap_bytes(kk_integer_t horizontal_resolution, kk_vector_t screen, kk_integer_t vertical_resolution, kk_vector_t header, kk_context_t* _ctx) { /* forall<h> (horizontal-resolution : int, screen : vector<vector<colour>>, vertical-resolution : int, header : vector<int>) -> pure vector<int> */ 
  kk_vector_t x_2838;
  kk_integer_t _x3679 = kk_integer_dup(horizontal_resolution); /*int*/
  kk_integer_t _x3680 = kk_integer_dup(vertical_resolution); /*int*/
  x_2838 = kk_bitmap_get_dib_header(_x3679, _x3680, kk_integer_from_small(40), _ctx); /*vector<int>*/
  if (kk_yielding(kk_context())) {
    kk_vector_drop(x_2838, _ctx);
    kk_box_t _x3681 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2745_get_bitmap_bytes_fun3682(header, horizontal_resolution, screen, vertical_resolution, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x3681, _ctx);
  }
  {
    return kk_bitmap__mlift2744_get_bitmap_bytes(header, horizontal_resolution, screen, vertical_resolution, x_2838, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift2746_get_bitmap_bytes_fun3689__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap__mlift2746_get_bitmap_bytes_fun3689(kk_function_t _fself, kk_box_t _b_3320, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2746_get_bitmap_bytes_fun3689(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap__mlift2746_get_bitmap_bytes_fun3689, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap__mlift2746_get_bitmap_bytes_fun3689(kk_function_t _fself, kk_box_t _b_3320, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_integer_t _x3690;
  kk_integer_t _x3691 = kk_integer_unbox(_b_3320); /*int*/
  _x3690 = kk_bitmap_round_up_to_four(_x3691, _ctx); /*int*/
  return kk_integer_box(_x3690);
}


// lift anonymous function
struct kk_bitmap__mlift2746_get_bitmap_bytes_fun3695__t {
  struct kk_function_s _base;
  kk_integer_t horizontal_resolution;
  kk_vector_t screen;
  kk_integer_t vertical_resolution;
};
static kk_box_t kk_bitmap__mlift2746_get_bitmap_bytes_fun3695(kk_function_t _fself, kk_box_t _b_3324, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift2746_get_bitmap_bytes_fun3695(kk_integer_t horizontal_resolution, kk_vector_t screen, kk_integer_t vertical_resolution, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2746_get_bitmap_bytes_fun3695__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift2746_get_bitmap_bytes_fun3695__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift2746_get_bitmap_bytes_fun3695, kk_context());
  _self->horizontal_resolution = horizontal_resolution;
  _self->screen = screen;
  _self->vertical_resolution = vertical_resolution;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift2746_get_bitmap_bytes_fun3695(kk_function_t _fself, kk_box_t _b_3324, kk_context_t* _ctx) {
  struct kk_bitmap__mlift2746_get_bitmap_bytes_fun3695__t* _self = kk_function_as(struct kk_bitmap__mlift2746_get_bitmap_bytes_fun3695__t*, _fself);
  kk_integer_t horizontal_resolution = _self->horizontal_resolution; /* int */
  kk_vector_t screen = _self->screen; /* vector<vector<bitmap/colour>> */
  kk_integer_t vertical_resolution = _self->vertical_resolution; /* int */
  kk_drop_match(_self, {kk_integer_dup(horizontal_resolution);kk_vector_dup(screen);kk_integer_dup(vertical_resolution);}, {}, _ctx)
  kk_vector_t _x3696;
  kk_vector_t _x3697 = kk_vector_unbox(_b_3324, _ctx); /*vector<int>*/
  _x3696 = kk_bitmap__mlift2745_get_bitmap_bytes(horizontal_resolution, screen, vertical_resolution, _x3697, _ctx); /*vector<int>*/
  return kk_vector_box(_x3696, _ctx);
}

kk_vector_t kk_bitmap__mlift2746_get_bitmap_bytes(kk_integer_t horizontal_resolution, kk_vector_t screen, kk_vector_t _y_2668, kk_context_t* _ctx) { /* forall<h> (horizontal-resolution : int, screen : vector<vector<colour>>, vector<colour>) -> exn vector<int> */ 
  kk_integer_t vertical_resolution;
  kk_integer_t _brw_3339 = kk_std_core_length_2(_y_2668, _ctx); /*int*/;
  kk_vector_drop(_y_2668, _ctx);
  vertical_resolution = _brw_3339; /*int*/
  kk_integer_t filesize;
  kk_integer_t _x3685;
  kk_integer_t _x3686 = kk_integer_dup(vertical_resolution); /*int*/
  kk_integer_t _x3687;
  kk_box_t _x3688;
  kk_box_t _x3692;
  kk_integer_t _x3693 = kk_integer_dup(horizontal_resolution); /*int*/
  _x3692 = kk_integer_box(_x3693); /*1001*/
  _x3688 = kk_std_core_hnd__open_none1(kk_bitmap__new_mlift2746_get_bitmap_bytes_fun3689(_ctx), _x3692, _ctx); /*1002*/
  _x3687 = kk_integer_unbox(_x3688); /*int*/
  _x3685 = kk_integer_mul(_x3686,_x3687,kk_context()); /*int*/
  filesize = kk_integer_add((kk_integer_from_small(54)),_x3685,kk_context()); /*int*/
  kk_vector_t x_2840 = kk_bitmap_get_header(filesize, _ctx); /*vector<int>*/;
  if (kk_yielding(kk_context())) {
    kk_vector_drop(x_2840, _ctx);
    kk_box_t _x3694 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift2746_get_bitmap_bytes_fun3695(horizontal_resolution, screen, vertical_resolution, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x3694, _ctx);
  }
  {
    return kk_bitmap__mlift2745_get_bitmap_bytes(horizontal_resolution, screen, vertical_resolution, x_2840, _ctx);
  }
}


// lift anonymous function
struct kk_bitmap_get_bitmap_bytes_fun3700__t {
  struct kk_function_s _base;
  kk_integer_t horizontal_resolution;
  kk_vector_t screen;
};
static kk_box_t kk_bitmap_get_bitmap_bytes_fun3700(kk_function_t _fself, kk_box_t _b_3332, kk_context_t* _ctx);
static kk_function_t kk_bitmap_new_get_bitmap_bytes_fun3700(kk_integer_t horizontal_resolution, kk_vector_t screen, kk_context_t* _ctx) {
  struct kk_bitmap_get_bitmap_bytes_fun3700__t* _self = kk_function_alloc_as(struct kk_bitmap_get_bitmap_bytes_fun3700__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap_get_bitmap_bytes_fun3700, kk_context());
  _self->horizontal_resolution = horizontal_resolution;
  _self->screen = screen;
  return &_self->_base;
}

static kk_box_t kk_bitmap_get_bitmap_bytes_fun3700(kk_function_t _fself, kk_box_t _b_3332, kk_context_t* _ctx) {
  struct kk_bitmap_get_bitmap_bytes_fun3700__t* _self = kk_function_as(struct kk_bitmap_get_bitmap_bytes_fun3700__t*, _fself);
  kk_integer_t horizontal_resolution = _self->horizontal_resolution; /* int */
  kk_vector_t screen = _self->screen; /* vector<vector<bitmap/colour>> */
  kk_drop_match(_self, {kk_integer_dup(horizontal_resolution);kk_vector_dup(screen);}, {}, _ctx)
  kk_vector_t _x3701;
  kk_vector_t _x3702 = kk_vector_unbox(_b_3332, _ctx); /*vector<bitmap/colour>*/
  _x3701 = kk_bitmap__mlift2746_get_bitmap_bytes(horizontal_resolution, screen, _x3702, _ctx); /*vector<int>*/
  return kk_vector_box(_x3701, _ctx);
}

kk_vector_t kk_bitmap_get_bitmap_bytes(kk_vector_t screen, kk_context_t* _ctx) { /* (screen : vector<vector<colour>>) -> pure vector<int> */ 
  kk_integer_t horizontal_resolution = kk_std_core_length_2(screen, _ctx); /*int*/;
  kk_vector_t x_2842;
  kk_box_t _x3698;
  kk_integer_t _brw_3336 = kk_integer_from_small(0); /*int*/;
  kk_box_t _brw_3337 = kk_vector_at_int_borrow(screen, _brw_3336); /*1001*/;
  kk_integer_drop(_brw_3336, _ctx);
  _x3698 = _brw_3337; /*1001*/
  x_2842 = kk_vector_unbox(_x3698, _ctx); /*vector<bitmap/colour>*/
  if (kk_yielding(kk_context())) {
    kk_vector_drop(x_2842, _ctx);
    kk_box_t _x3699 = kk_std_core_hnd_yield_extend(kk_bitmap_new_get_bitmap_bytes_fun3700(horizontal_resolution, screen, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x3699, _ctx);
  }
  {
    return kk_bitmap__mlift2746_get_bitmap_bytes(horizontal_resolution, screen, x_2842, _ctx);
  }
}

// initialization
void kk_bitmap__init(kk_context_t* _ctx){
  static bool _kk_initialized = false;
  if (_kk_initialized) return;
  _kk_initialized = true;
  kk_std_core_types__init(_ctx);
  kk_std_core_hnd__init(_ctx);
  kk_std_core__init(_ctx);
  #if defined(KK_CUSTOM_INIT)
    KK_CUSTOM_INIT (_ctx);
  #endif
}

// termination
void kk_bitmap__done(kk_context_t* _ctx){
  static bool _kk_done = false;
  if (_kk_done) return;
  _kk_done = true;
  #if defined(KK_CUSTOM_DONE)
    KK_CUSTOM_DONE (_ctx);
  #endif
  kk_std_core__done(_ctx);
  kk_std_core_hnd__done(_ctx);
  kk_std_core_types__done(_ctx);
}
