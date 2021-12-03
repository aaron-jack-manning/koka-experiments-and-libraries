// Koka generated module: "bitmap", koka version: 2.3.2, platform: 64-bit
#include "bitmap.h"

kk_bitmap__colour kk_bitmap__copy(kk_bitmap__colour _this, kk_std_core_types__optional red0, kk_std_core_types__optional green0, kk_std_core_types__optional blue0, kk_context_t* _ctx) { /* (colour, red : optional<int>, green : optional<int>, blue : optional<int>) -> colour */ 
  kk_integer_t _x1572;
  if (kk_std_core_types__is_Optional(red0)) {
    kk_box_t _box_x1320 = red0._cons.Optional.value;
    kk_integer_t _red_52 = kk_integer_unbox(_box_x1320);
    kk_integer_dup(_red_52);
    kk_std_core_types__optional_drop(red0, _ctx);
    _x1572 = _red_52; /*int*/
    goto _match1573;
  }
  {
    kk_integer_t _x = _this.red;
    kk_integer_dup(_x);
    _x1572 = _x; /*int*/
  }
  _match1573: ;
  kk_integer_t _x1575;
  if (kk_std_core_types__is_Optional(green0)) {
    kk_box_t _box_x1321 = green0._cons.Optional.value;
    kk_integer_t _green_58 = kk_integer_unbox(_box_x1321);
    kk_integer_dup(_green_58);
    kk_std_core_types__optional_drop(green0, _ctx);
    _x1575 = _green_58; /*int*/
    goto _match1576;
  }
  {
    kk_integer_t _x0 = _this.green;
    kk_integer_dup(_x0);
    _x1575 = _x0; /*int*/
  }
  _match1576: ;
  kk_integer_t _x1578;
  if (kk_std_core_types__is_Optional(blue0)) {
    kk_box_t _box_x1322 = blue0._cons.Optional.value;
    kk_integer_t _blue_64 = kk_integer_unbox(_box_x1322);
    kk_integer_dup(_blue_64);
    kk_std_core_types__optional_drop(blue0, _ctx);
    kk_bitmap__colour_drop(_this, _ctx);
    _x1578 = _blue_64; /*int*/
    goto _match1579;
  }
  {
    kk_integer_t _x1 = _this.blue;
    kk_integer_dup(_x1);
    kk_bitmap__colour_drop(_this, _ctx);
    _x1578 = _x1; /*int*/
  }
  _match1579: ;
  return kk_bitmap__new_Colour(_x1572, _x1575, _x1578, _ctx);
}
 
// lifted

kk_std_core__list kk_bitmap__lift1237_bytes(kk_integer_t current, kk_integer_t base, kk_context_t* _ctx) { /* (current : int, base : int) -> div list<int> */ 
  kk_std_core_types__tuple2_ _match_1571;
  kk_integer_t _x1581 = kk_integer_dup(base); /*int*/
  _match_1571 = kk_integer_div_mod_tuple(current,_x1581,kk_context()); /*(int, int)*/
  {
    kk_box_t _box_x1323 = _match_1571.fst;
    kk_box_t _box_x1324 = _match_1571.snd;
    kk_integer_t quotient = kk_integer_unbox(_box_x1323);
    kk_integer_t remainder = kk_integer_unbox(_box_x1324);
    kk_integer_dup(quotient);
    kk_integer_dup(remainder);
    kk_std_core_types__tuple2__drop(_match_1571, _ctx);
    if (kk_integer_eq_borrow(quotient, kk_integer_from_small(0), _ctx)) {
      kk_integer_drop(quotient, _ctx);
      kk_integer_drop(base, _ctx);
      kk_std_core__list _x1584 = kk_std_core__new_Cons(kk_reuse_null, kk_integer_box(remainder), kk_std_core__new_Nil(_ctx), _ctx); /*list<1009>*/
      return kk_std_core_append(_x1584, kk_std_core__new_Nil(_ctx), _ctx);
    }
    {
      kk_std_core__list ys0_1221 = kk_bitmap__lift1237_bytes(quotient, base, _ctx); /*list<int>*/;
      kk_std_core__list _x1585 = kk_std_core__new_Cons(kk_reuse_null, kk_integer_box(remainder), kk_std_core__new_Nil(_ctx), _ctx); /*list<1009>*/
      return kk_std_core_append(_x1585, ys0_1221, _ctx);
    }
  }
}


// lift anonymous function
struct kk_bitmap_bytes_fun1590__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap_bytes_fun1590(kk_function_t _fself, kk_integer_t _b_1336, kk_context_t* _ctx);
static kk_function_t kk_bitmap_new_bytes_fun1590(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap_bytes_fun1590, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap_bytes_fun1590(kk_function_t _fself, kk_integer_t _b_1336, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_integer_drop(_b_1336, _ctx);
  return kk_integer_box(kk_integer_from_small(0));
}

kk_std_core__list kk_bitmap_bytes(kk_integer_t number, kk_context_t* _ctx) { /* (number : int) -> div list<int> */ 
  kk_std_core__list digits = kk_bitmap__lift1237_bytes(number, kk_integer_from_small(256), _ctx); /*list<int>*/;
  kk_integer_t padding;
  kk_integer_t _x1586;
  kk_integer_t _x1587;
  kk_std_core__list _x1588 = kk_std_core__list_dup(digits); /*list<int>*/
  _x1587 = kk_std_core__lift17270_length_1(_x1588, kk_integer_from_small(0), _ctx); /*int*/
  _x1586 = kk_integer_mod(_x1587,(kk_integer_from_small(4)),kk_context()); /*int*/
  padding = kk_integer_sub((kk_integer_from_small(4)),_x1586,kk_context()); /*int*/
  kk_std_core__list _x1589 = kk_std_core_list_2(kk_integer_from_small(1), padding, kk_bitmap_new_bytes_fun1590(_ctx), _ctx); /*list<1001>*/
  return kk_std_core_append(digits, _x1589, _ctx);
}


// lift anonymous function
struct kk_bitmap_colour_to_bytes_fun1591__t {
  struct kk_function_s _base;
  kk_bitmap__colour colour;
};
static kk_box_t kk_bitmap_colour_to_bytes_fun1591(kk_function_t _fself, kk_ssize_t _b_1343, kk_context_t* _ctx);
static kk_function_t kk_bitmap_new_colour_to_bytes_fun1591(kk_bitmap__colour colour, kk_context_t* _ctx) {
  struct kk_bitmap_colour_to_bytes_fun1591__t* _self = kk_function_alloc_as(struct kk_bitmap_colour_to_bytes_fun1591__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap_colour_to_bytes_fun1591, kk_context());
  _self->colour = colour;
  return &_self->_base;
}

static kk_box_t kk_bitmap_colour_to_bytes_fun1591(kk_function_t _fself, kk_ssize_t _b_1343, kk_context_t* _ctx) {
  struct kk_bitmap_colour_to_bytes_fun1591__t* _self = kk_function_as(struct kk_bitmap_colour_to_bytes_fun1591__t*, _fself);
  kk_bitmap__colour colour = _self->colour; /* bitmap/colour */
  kk_drop_match(_self, {kk_bitmap__colour_dup(colour);}, {}, _ctx)
  kk_integer_t _x1592;
  kk_integer_t i0_1236 = kk_integer_from_ssize_t(_b_1343,kk_context()); /*int*/;
  if (kk_integer_eq_borrow(i0_1236, kk_integer_from_small(0), _ctx)) {
    kk_integer_drop(i0_1236, _ctx);
    {
      kk_integer_t _x = colour.blue;
      kk_integer_dup(_x);
      kk_bitmap__colour_drop(colour, _ctx);
      _x1592 = _x; /*int*/
      goto _match1593;
    }
  }
  if (kk_integer_eq_borrow(i0_1236, kk_integer_from_small(1), _ctx)) {
    kk_integer_drop(i0_1236, _ctx);
    {
      kk_integer_t _x0 = colour.green;
      kk_integer_dup(_x0);
      kk_bitmap__colour_drop(colour, _ctx);
      _x1592 = _x0; /*int*/
      goto _match1593;
    }
  }
  if (kk_integer_eq_borrow(i0_1236, kk_integer_from_small(2), _ctx)) {
    kk_integer_drop(i0_1236, _ctx);
    {
      kk_integer_t _x1 = colour.red;
      kk_integer_dup(_x1);
      kk_bitmap__colour_drop(colour, _ctx);
      _x1592 = _x1; /*int*/
      goto _match1593;
    }
  }
  {
    kk_integer_drop(i0_1236, _ctx);
    kk_bitmap__colour_drop(colour, _ctx);
    _x1592 = kk_integer_from_small(0); /*int*/
  }
  _match1593: ;
  return kk_integer_box(_x1592);
}

kk_vector_t kk_bitmap_colour_to_bytes(kk_bitmap__colour colour, kk_context_t* _ctx) { /* (colour : colour) -> vector<int> */ 
  kk_ssize_t _b_1344_1341 = ((kk_ssize_t)3); /*ssize_t*/;
  return kk_std_core_vector_initz(_b_1344_1341, kk_bitmap_new_colour_to_bytes_fun1591(colour, _ctx), _ctx);
}
 
// monadic lift

kk_unit_t kk_bitmap__mlift1265_get_dib_header(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_unit_t wild__4, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, i : int, print-resolution-bytes : list<int>, wild_4 : ()) -> <local<h>,exn,div> () */ 
  kk_integer_t _b_1353_1350;
  kk_integer_t _x1594 = kk_integer_dup(i); /*int*/
  _b_1353_1350 = kk_integer_add((kk_integer_from_small(28)),_x1594,kk_context()); /*int*/
  kk_std_core_types__maybe _b_1355_1347 = kk_std_core__lp__lb__rb__2_rp_(print_resolution_bytes, i, _ctx); /*maybe<int>*/;
  kk_integer_t _b_1354_1351;
  kk_box_t _x1595 = kk_std_core_maybe_1(_b_1355_1347, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_1354_1351 = kk_integer_unbox(_x1595); /*int*/
  kk_unit_t _brw_1570 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_b_1353_1350,(kk_integer_box(_b_1354_1351)),kk_context());
  kk_ref_drop(dib_header, _ctx);
  kk_integer_drop(_b_1353_1350, _ctx);
  _brw_1570; return kk_Unit;
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift1266_get_dib_header_fun1600__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_integer_t i;
  kk_std_core__list print_resolution_bytes;
};
static kk_unit_t kk_bitmap__mlift1266_get_dib_header_fun1600(kk_function_t _fself, kk_unit_t wild__4, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1266_get_dib_header_fun1600(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1266_get_dib_header_fun1600__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1266_get_dib_header_fun1600__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1266_get_dib_header_fun1600, kk_context());
  _self->dib_header = dib_header;
  _self->i = i;
  _self->print_resolution_bytes = print_resolution_bytes;
  return &_self->_base;
}

static kk_unit_t kk_bitmap__mlift1266_get_dib_header_fun1600(kk_function_t _fself, kk_unit_t wild__4, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1266_get_dib_header_fun1600__t* _self = kk_function_as(struct kk_bitmap__mlift1266_get_dib_header_fun1600__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<933,vector<int>> */
  kk_integer_t i = _self->i; /* int */
  kk_std_core__list print_resolution_bytes = _self->print_resolution_bytes; /* list<int> */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_integer_dup(i);kk_std_core__list_dup(print_resolution_bytes);}, {}, _ctx)
  kk_integer_t _b_1373_1370;
  kk_integer_t _x1601 = kk_integer_dup(i); /*int*/
  _b_1373_1370 = kk_integer_add((kk_integer_from_small(28)),_x1601,kk_context()); /*int*/
  kk_std_core_types__maybe _b_1375_1367 = kk_std_core__lp__lb__rb__2_rp_(print_resolution_bytes, i, _ctx); /*maybe<int>*/;
  kk_integer_t _b_1374_1371;
  kk_box_t _x1602 = kk_std_core_maybe_1(_b_1375_1367, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_1374_1371 = kk_integer_unbox(_x1602); /*int*/
  kk_unit_t _brw_1568 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_b_1373_1370,(kk_integer_box(_b_1374_1371)),kk_context());
  kk_ref_drop(dib_header, _ctx);
  kk_integer_drop(_b_1373_1370, _ctx);
  return _brw_1568;
}


// lift anonymous function
struct kk_bitmap__mlift1266_get_dib_header_fun1604__t {
  struct kk_function_s _base;
  kk_function_t next_1277;
};
static kk_box_t kk_bitmap__mlift1266_get_dib_header_fun1604(kk_function_t _fself, kk_box_t _b_1378, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1266_get_dib_header_fun1604(kk_function_t next_1277, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1266_get_dib_header_fun1604__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1266_get_dib_header_fun1604__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1266_get_dib_header_fun1604, kk_context());
  _self->next_1277 = next_1277;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift1266_get_dib_header_fun1604(kk_function_t _fself, kk_box_t _b_1378, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1266_get_dib_header_fun1604__t* _self = kk_function_as(struct kk_bitmap__mlift1266_get_dib_header_fun1604__t*, _fself);
  kk_function_t next_1277 = _self->next_1277; /* (()) -> <local<933>,exn,div> () */
  kk_drop_match(_self, {kk_function_dup(next_1277);}, {}, _ctx)
  kk_unit_t _x1605 = kk_Unit;
  kk_unit_t _x1606 = kk_Unit;
  kk_unit_unbox(_b_1378);
  kk_function_call(kk_unit_t, (kk_function_t, kk_unit_t, kk_context_t*), next_1277, (next_1277, _x1606, _ctx));
  return kk_unit_box(_x1605);
}

kk_unit_t kk_bitmap__mlift1266_get_dib_header(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_unit_t wild__3, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, i : int, print-resolution-bytes : list<int>, wild_3 : ()) -> <local<h>,exn,div> () */ 
  kk_integer_t _b_1363_1360;
  kk_integer_t _x1596 = kk_integer_dup(i); /*int*/
  _b_1363_1360 = kk_integer_add((kk_integer_from_small(24)),_x1596,kk_context()); /*int*/
  kk_std_core_types__maybe _b_1365_1357;
  kk_std_core__list _x1597 = kk_std_core__list_dup(print_resolution_bytes); /*list<int>*/
  kk_integer_t _x1598 = kk_integer_dup(i); /*int*/
  _b_1365_1357 = kk_std_core__lp__lb__rb__2_rp_(_x1597, _x1598, _ctx); /*maybe<int>*/
  kk_integer_t _b_1364_1361;
  kk_box_t _x1599 = kk_std_core_maybe_1(_b_1365_1357, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_1364_1361 = kk_integer_unbox(_x1599); /*int*/
  kk_unit_t x_1276 = kk_Unit;
  kk_unit_t _brw_1569 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_b_1363_1360,(kk_integer_box(_b_1364_1361)),kk_context());
  kk_integer_drop(_b_1363_1360, _ctx);
  _brw_1569;
  kk_function_t next_1277 = kk_bitmap__new_mlift1266_get_dib_header_fun1600(dib_header, i, print_resolution_bytes, _ctx); /*(()) -> <local<933>,exn,div> ()*/;
  if (kk_yielding(kk_context())) {
    kk_box_t _x1603 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift1266_get_dib_header_fun1604(next_1277, _ctx), _ctx); /*1002*/
    kk_unit_unbox(_x1603); return kk_Unit;
  }
  {
    kk_function_call(kk_unit_t, (kk_function_t, kk_unit_t, kk_context_t*), next_1277, (next_1277, x_1276, _ctx)); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift1267_get_dib_header_fun1611__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_integer_t i;
  kk_std_core__list print_resolution_bytes;
};
static kk_box_t kk_bitmap__mlift1267_get_dib_header_fun1611(kk_function_t _fself, kk_box_t _b_1391, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1267_get_dib_header_fun1611(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1267_get_dib_header_fun1611__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1267_get_dib_header_fun1611__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1267_get_dib_header_fun1611, kk_context());
  _self->dib_header = dib_header;
  _self->i = i;
  _self->print_resolution_bytes = print_resolution_bytes;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift1267_get_dib_header_fun1611(kk_function_t _fself, kk_box_t _b_1391, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1267_get_dib_header_fun1611__t* _self = kk_function_as(struct kk_bitmap__mlift1267_get_dib_header_fun1611__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<933,vector<int>> */
  kk_integer_t i = _self->i; /* int */
  kk_std_core__list print_resolution_bytes = _self->print_resolution_bytes; /* list<int> */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_integer_dup(i);kk_std_core__list_dup(print_resolution_bytes);}, {}, _ctx)
  kk_unit_t _x1612 = kk_Unit;
  kk_unit_t _x1613 = kk_Unit;
  kk_unit_unbox(_b_1391);
  kk_bitmap__mlift1266_get_dib_header(dib_header, i, print_resolution_bytes, _x1613, _ctx);
  return kk_unit_box(_x1612);
}

kk_unit_t kk_bitmap__mlift1267_get_dib_header(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_std_core__list vertical_resolution_bytes, kk_unit_t wild__2, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, i : int, print-resolution-bytes : list<int>, vertical-resolution-bytes : list<int>, wild_2 : ()) -> <local<h>,exn,div> () */ 
  kk_integer_t _b_1386_1383;
  kk_integer_t _x1607 = kk_integer_dup(i); /*int*/
  _b_1386_1383 = kk_integer_add((kk_integer_from_small(8)),_x1607,kk_context()); /*int*/
  kk_std_core_types__maybe _b_1388_1380;
  kk_integer_t _x1608 = kk_integer_dup(i); /*int*/
  _b_1388_1380 = kk_std_core__lp__lb__rb__2_rp_(vertical_resolution_bytes, _x1608, _ctx); /*maybe<int>*/
  kk_integer_t _b_1387_1384;
  kk_box_t _x1609 = kk_std_core_maybe_1(_b_1388_1380, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_1387_1384 = kk_integer_unbox(_x1609); /*int*/
  kk_unit_t x_1282 = kk_Unit;
  kk_unit_t _brw_1566 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_b_1386_1383,(kk_integer_box(_b_1387_1384)),kk_context());
  kk_integer_drop(_b_1386_1383, _ctx);
  _brw_1566;
  if (kk_yielding(kk_context())) {
    kk_box_t _x1610 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift1267_get_dib_header_fun1611(dib_header, i, print_resolution_bytes, _ctx), _ctx); /*1002*/
    kk_unit_unbox(_x1610); return kk_Unit;
  }
  {
    kk_bitmap__mlift1266_get_dib_header(dib_header, i, print_resolution_bytes, x_1282, _ctx); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift1269_get_dib_header_fun1616__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap__mlift1269_get_dib_header_fun1616(kk_function_t _fself, kk_box_t _b_1398, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1269_get_dib_header_fun1616(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap__mlift1269_get_dib_header_fun1616, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap__mlift1269_get_dib_header_fun1616(kk_function_t _fself, kk_box_t _b_1398, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core__list _x1617;
  kk_integer_t _x1618 = kk_integer_unbox(_b_1398); /*int*/
  _x1617 = kk_bitmap_bytes(_x1618, _ctx); /*list<int>*/
  return kk_std_core__list_box(_x1617, _ctx);
}


// lift anonymous function
struct kk_bitmap__mlift1269_get_dib_header_fun1620__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap__mlift1269_get_dib_header_fun1620(kk_function_t _fself, kk_box_t _b_1403, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1269_get_dib_header_fun1620(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap__mlift1269_get_dib_header_fun1620, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap__mlift1269_get_dib_header_fun1620(kk_function_t _fself, kk_box_t _b_1403, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core__list _x1621;
  kk_integer_t _x1622 = kk_integer_unbox(_b_1403); /*int*/
  _x1621 = kk_bitmap_bytes(_x1622, _ctx); /*list<int>*/
  return kk_std_core__list_box(_x1621, _ctx);
}


// lift anonymous function
struct kk_bitmap__mlift1269_get_dib_header_fun1624__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap__mlift1269_get_dib_header_fun1624(kk_function_t _fself, kk_box_t _b_1408, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1269_get_dib_header_fun1624(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap__mlift1269_get_dib_header_fun1624, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap__mlift1269_get_dib_header_fun1624(kk_function_t _fself, kk_box_t _b_1408, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core__list _x1625;
  kk_integer_t _x1626 = kk_integer_unbox(_b_1408); /*int*/
  _x1625 = kk_bitmap_bytes(_x1626, _ctx); /*list<int>*/
  return kk_std_core__list_box(_x1625, _ctx);
}


// lift anonymous function
struct kk_bitmap__mlift1269_get_dib_header_fun1628__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_std_core__list horizontal_resolution_bytes;
  kk_std_core__list print_resolution_bytes;
  kk_std_core__list vertical_resolution_bytes;
};
static kk_unit_t kk_bitmap__mlift1269_get_dib_header_fun1628(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1269_get_dib_header_fun1628(kk_ref_t dib_header, kk_std_core__list horizontal_resolution_bytes, kk_std_core__list print_resolution_bytes, kk_std_core__list vertical_resolution_bytes, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1269_get_dib_header_fun1628__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1269_get_dib_header_fun1628__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1269_get_dib_header_fun1628, kk_context());
  _self->dib_header = dib_header;
  _self->horizontal_resolution_bytes = horizontal_resolution_bytes;
  _self->print_resolution_bytes = print_resolution_bytes;
  _self->vertical_resolution_bytes = vertical_resolution_bytes;
  return &_self->_base;
}



// lift anonymous function
struct kk_bitmap__mlift1269_get_dib_header_fun1633__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_integer_t i;
  kk_std_core__list print_resolution_bytes;
  kk_std_core__list vertical_resolution_bytes;
};
static kk_box_t kk_bitmap__mlift1269_get_dib_header_fun1633(kk_function_t _fself, kk_box_t _b_1422, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1269_get_dib_header_fun1633(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_std_core__list vertical_resolution_bytes, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1269_get_dib_header_fun1633__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1269_get_dib_header_fun1633__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1269_get_dib_header_fun1633, kk_context());
  _self->dib_header = dib_header;
  _self->i = i;
  _self->print_resolution_bytes = print_resolution_bytes;
  _self->vertical_resolution_bytes = vertical_resolution_bytes;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift1269_get_dib_header_fun1633(kk_function_t _fself, kk_box_t _b_1422, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1269_get_dib_header_fun1633__t* _self = kk_function_as(struct kk_bitmap__mlift1269_get_dib_header_fun1633__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<933,vector<int>> */
  kk_integer_t i = _self->i; /* int */
  kk_std_core__list print_resolution_bytes = _self->print_resolution_bytes; /* list<int> */
  kk_std_core__list vertical_resolution_bytes = _self->vertical_resolution_bytes; /* list<int> */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_integer_dup(i);kk_std_core__list_dup(print_resolution_bytes);kk_std_core__list_dup(vertical_resolution_bytes);}, {}, _ctx)
  kk_unit_t _x1634 = kk_Unit;
  kk_unit_t _x1635 = kk_Unit;
  kk_unit_unbox(_b_1422);
  kk_bitmap__mlift1267_get_dib_header(dib_header, i, print_resolution_bytes, vertical_resolution_bytes, _x1635, _ctx);
  return kk_unit_box(_x1634);
}
static kk_unit_t kk_bitmap__mlift1269_get_dib_header_fun1628(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1269_get_dib_header_fun1628__t* _self = kk_function_as(struct kk_bitmap__mlift1269_get_dib_header_fun1628__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<933,vector<int>> */
  kk_std_core__list horizontal_resolution_bytes = _self->horizontal_resolution_bytes; /* list<int> */
  kk_std_core__list print_resolution_bytes = _self->print_resolution_bytes; /* list<int> */
  kk_std_core__list vertical_resolution_bytes = _self->vertical_resolution_bytes; /* list<int> */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_std_core__list_dup(horizontal_resolution_bytes);kk_std_core__list_dup(print_resolution_bytes);kk_std_core__list_dup(vertical_resolution_bytes);}, {}, _ctx)
  kk_integer_t _b_1417_1414;
  kk_integer_t _x1629 = kk_integer_dup(i); /*int*/
  _b_1417_1414 = kk_integer_add((kk_integer_from_small(4)),_x1629,kk_context()); /*int*/
  kk_std_core_types__maybe _b_1419_1411;
  kk_integer_t _x1630 = kk_integer_dup(i); /*int*/
  _b_1419_1411 = kk_std_core__lp__lb__rb__2_rp_(horizontal_resolution_bytes, _x1630, _ctx); /*maybe<int>*/
  kk_integer_t _b_1418_1415;
  kk_box_t _x1631 = kk_std_core_maybe_1(_b_1419_1411, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_1418_1415 = kk_integer_unbox(_x1631); /*int*/
  kk_unit_t x0_1286 = kk_Unit;
  kk_unit_t _brw_1564 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_b_1417_1414,(kk_integer_box(_b_1418_1415)),kk_context());
  kk_integer_drop(_b_1417_1414, _ctx);
  _brw_1564;
  if (kk_yielding(kk_context())) {
    kk_box_t _x1632 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift1269_get_dib_header_fun1633(dib_header, i, print_resolution_bytes, vertical_resolution_bytes, _ctx), _ctx); /*1002*/
    return kk_unit_unbox(_x1632);
  }
  {
    return kk_bitmap__mlift1267_get_dib_header(dib_header, i, print_resolution_bytes, vertical_resolution_bytes, x0_1286, _ctx);
  }
}


// lift anonymous function
struct kk_bitmap__mlift1269_get_dib_header_fun1637__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
};
static kk_box_t kk_bitmap__mlift1269_get_dib_header_fun1637(kk_function_t _fself, kk_box_t _b_1427, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1269_get_dib_header_fun1637(kk_ref_t dib_header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1269_get_dib_header_fun1637__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1269_get_dib_header_fun1637__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1269_get_dib_header_fun1637, kk_context());
  _self->dib_header = dib_header;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift1269_get_dib_header_fun1637(kk_function_t _fself, kk_box_t _b_1427, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1269_get_dib_header_fun1637__t* _self = kk_function_as(struct kk_bitmap__mlift1269_get_dib_header_fun1637__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<933,vector<int>> */
  kk_drop_match(_self, {kk_ref_dup(dib_header);}, {}, _ctx)
  kk_box_drop(_b_1427, _ctx);
  return (kk_ref_get(dib_header,kk_context()));
}

kk_vector_t kk_bitmap__mlift1269_get_dib_header(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_unit_t wild__1, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, dpcm : int, horizontal-resolution : int, vertical-resolution : int, wild_1 : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_std_core__list horizontal_resolution_bytes;
  kk_box_t _x1615 = kk_std_core_hnd__open_none1(kk_bitmap__new_mlift1269_get_dib_header_fun1616(_ctx), kk_integer_box(horizontal_resolution), _ctx); /*1002*/
  horizontal_resolution_bytes = kk_std_core__list_unbox(_x1615, _ctx); /*list<int>*/
  kk_std_core__list vertical_resolution_bytes;
  kk_box_t _x1619 = kk_std_core_hnd__open_none1(kk_bitmap__new_mlift1269_get_dib_header_fun1620(_ctx), kk_integer_box(vertical_resolution), _ctx); /*1002*/
  vertical_resolution_bytes = kk_std_core__list_unbox(_x1619, _ctx); /*list<int>*/
  kk_integer_t _x11_1263 = kk_integer_mul(dpcm,(kk_integer_from_small(100)),kk_context()); /*int*/;
  kk_std_core__list print_resolution_bytes;
  kk_box_t _x1623 = kk_std_core_hnd__open_none1(kk_bitmap__new_mlift1269_get_dib_header_fun1624(_ctx), kk_integer_box(_x11_1263), _ctx); /*1002*/
  print_resolution_bytes = kk_std_core__list_unbox(_x1623, _ctx); /*list<int>*/
  kk_unit_t x_1284 = kk_Unit;
  kk_function_t _x1627;
  kk_ref_dup(dib_header);
  _x1627 = kk_bitmap__new_mlift1269_get_dib_header_fun1628(dib_header, horizontal_resolution_bytes, print_resolution_bytes, vertical_resolution_bytes, _ctx); /*(i : int) -> <exn,local<933>,div> ()*/
  kk_std_core__lift17275_for(_x1627, kk_integer_from_small(3), kk_integer_from_small(0), _ctx);
  kk_box_t _x1636;
  if (kk_yielding(kk_context())) {
    _x1636 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift1269_get_dib_header_fun1637(dib_header, _ctx), _ctx); /*1002*/
  }
  else {
    _x1636 = (kk_ref_get(dib_header,kk_context())); /*1002*/
  }
  return kk_vector_unbox(_x1636, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift1270_get_dib_header_fun1639__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_integer_t dpcm;
  kk_integer_t horizontal_resolution;
  kk_integer_t vertical_resolution;
};
static kk_box_t kk_bitmap__mlift1270_get_dib_header_fun1639(kk_function_t _fself, kk_box_t _b_1440, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1270_get_dib_header_fun1639(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1270_get_dib_header_fun1639__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1270_get_dib_header_fun1639__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1270_get_dib_header_fun1639, kk_context());
  _self->dib_header = dib_header;
  _self->dpcm = dpcm;
  _self->horizontal_resolution = horizontal_resolution;
  _self->vertical_resolution = vertical_resolution;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift1270_get_dib_header_fun1639(kk_function_t _fself, kk_box_t _b_1440, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1270_get_dib_header_fun1639__t* _self = kk_function_as(struct kk_bitmap__mlift1270_get_dib_header_fun1639__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<933,vector<int>> */
  kk_integer_t dpcm = _self->dpcm; /* int */
  kk_integer_t horizontal_resolution = _self->horizontal_resolution; /* int */
  kk_integer_t vertical_resolution = _self->vertical_resolution; /* int */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_integer_dup(dpcm);kk_integer_dup(horizontal_resolution);kk_integer_dup(vertical_resolution);}, {}, _ctx)
  kk_vector_t _x1640;
  kk_unit_t _x1641 = kk_Unit;
  kk_unit_unbox(_b_1440);
  _x1640 = kk_bitmap__mlift1269_get_dib_header(dib_header, dpcm, horizontal_resolution, vertical_resolution, _x1641, _ctx); /*vector<int>*/
  return kk_vector_box(_x1640, _ctx);
}

kk_vector_t kk_bitmap__mlift1270_get_dib_header(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_unit_t wild__0, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, dpcm : int, horizontal-resolution : int, vertical-resolution : int, wild_0 : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_unit_t x_1290 = kk_Unit;
  kk_integer_t _brw_1560 = kk_integer_from_small(14); /*int*/;
  kk_unit_t _brw_1561 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_brw_1560,(kk_integer_box(kk_integer_from_small(24))),kk_context());
  kk_integer_drop(_brw_1560, _ctx);
  _brw_1561;
  if (kk_yielding(kk_context())) {
    kk_box_t _x1638 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift1270_get_dib_header_fun1639(dib_header, dpcm, horizontal_resolution, vertical_resolution, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x1638, _ctx);
  }
  {
    return kk_bitmap__mlift1269_get_dib_header(dib_header, dpcm, horizontal_resolution, vertical_resolution, x_1290, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift1271_get_dib_header_fun1643__t {
  struct kk_function_s _base;
  kk_ref_t dib_header;
  kk_integer_t dpcm;
  kk_integer_t horizontal_resolution;
  kk_integer_t vertical_resolution;
};
static kk_box_t kk_bitmap__mlift1271_get_dib_header_fun1643(kk_function_t _fself, kk_box_t _b_1450, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1271_get_dib_header_fun1643(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1271_get_dib_header_fun1643__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1271_get_dib_header_fun1643__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1271_get_dib_header_fun1643, kk_context());
  _self->dib_header = dib_header;
  _self->dpcm = dpcm;
  _self->horizontal_resolution = horizontal_resolution;
  _self->vertical_resolution = vertical_resolution;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift1271_get_dib_header_fun1643(kk_function_t _fself, kk_box_t _b_1450, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1271_get_dib_header_fun1643__t* _self = kk_function_as(struct kk_bitmap__mlift1271_get_dib_header_fun1643__t*, _fself);
  kk_ref_t dib_header = _self->dib_header; /* local-var<933,vector<int>> */
  kk_integer_t dpcm = _self->dpcm; /* int */
  kk_integer_t horizontal_resolution = _self->horizontal_resolution; /* int */
  kk_integer_t vertical_resolution = _self->vertical_resolution; /* int */
  kk_drop_match(_self, {kk_ref_dup(dib_header);kk_integer_dup(dpcm);kk_integer_dup(horizontal_resolution);kk_integer_dup(vertical_resolution);}, {}, _ctx)
  kk_vector_t _x1644;
  kk_unit_t _x1645 = kk_Unit;
  kk_unit_unbox(_b_1450);
  _x1644 = kk_bitmap__mlift1270_get_dib_header(dib_header, dpcm, horizontal_resolution, vertical_resolution, _x1645, _ctx); /*vector<int>*/
  return kk_vector_box(_x1644, _ctx);
}

kk_vector_t kk_bitmap__mlift1271_get_dib_header(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_unit_t wild__, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, dpcm : int, horizontal-resolution : int, vertical-resolution : int, wild_ : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_unit_t x_1292 = kk_Unit;
  kk_integer_t _brw_1557 = kk_integer_from_small(12); /*int*/;
  kk_unit_t _brw_1558 = kk_Unit;
  kk_ref_vector_assign_borrow(dib_header,_brw_1557,(kk_integer_box(kk_integer_from_small(1))),kk_context());
  kk_integer_drop(_brw_1557, _ctx);
  _brw_1558;
  if (kk_yielding(kk_context())) {
    kk_box_t _x1642 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift1271_get_dib_header_fun1643(dib_header, dpcm, horizontal_resolution, vertical_resolution, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x1642, _ctx);
  }
  {
    return kk_bitmap__mlift1270_get_dib_header(dib_header, dpcm, horizontal_resolution, vertical_resolution, x_1292, _ctx);
  }
}


// lift anonymous function
struct kk_bitmap_get_dib_header_fun1648__t {
  struct kk_function_s _base;
  kk_integer_t dpcm;
  kk_integer_t horizontal_resolution;
  kk_ref_t loc;
  kk_integer_t vertical_resolution;
};
static kk_box_t kk_bitmap_get_dib_header_fun1648(kk_function_t _fself, kk_box_t _b_1466, kk_context_t* _ctx);
static kk_function_t kk_bitmap_new_get_dib_header_fun1648(kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_ref_t loc, kk_integer_t vertical_resolution, kk_context_t* _ctx) {
  struct kk_bitmap_get_dib_header_fun1648__t* _self = kk_function_alloc_as(struct kk_bitmap_get_dib_header_fun1648__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap_get_dib_header_fun1648, kk_context());
  _self->dpcm = dpcm;
  _self->horizontal_resolution = horizontal_resolution;
  _self->loc = loc;
  _self->vertical_resolution = vertical_resolution;
  return &_self->_base;
}

static kk_box_t kk_bitmap_get_dib_header_fun1648(kk_function_t _fself, kk_box_t _b_1466, kk_context_t* _ctx) {
  struct kk_bitmap_get_dib_header_fun1648__t* _self = kk_function_as(struct kk_bitmap_get_dib_header_fun1648__t*, _fself);
  kk_integer_t dpcm = _self->dpcm; /* int */
  kk_integer_t horizontal_resolution = _self->horizontal_resolution; /* int */
  kk_ref_t loc = _self->loc; /* local-var<933,vector<int>> */
  kk_integer_t vertical_resolution = _self->vertical_resolution; /* int */
  kk_drop_match(_self, {kk_integer_dup(dpcm);kk_integer_dup(horizontal_resolution);kk_ref_dup(loc);kk_integer_dup(vertical_resolution);}, {}, _ctx)
  kk_vector_t _x1649;
  kk_unit_t _x1650 = kk_Unit;
  kk_unit_unbox(_b_1466);
  _x1649 = kk_bitmap__mlift1271_get_dib_header(loc, dpcm, horizontal_resolution, vertical_resolution, _x1650, _ctx); /*vector<int>*/
  return kk_vector_box(_x1649, _ctx);
}

kk_vector_t kk_bitmap_get_dib_header(kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_integer_t dpcm, kk_context_t* _ctx) { /* (horizontal-resolution : int, vertical-resolution : int, dpcm : int) -> pure vector<int> */ 
  kk_vector_t init_1294;
  kk_integer_t _brw_1554 = kk_integer_from_small(50); /*int*/;
  kk_vector_t _brw_1555 = kk_std_core_vector_2(_brw_1554, kk_integer_box(kk_integer_from_small(0)), _ctx); /*vector<1001>*/;
  kk_integer_drop(_brw_1554, _ctx);
  init_1294 = _brw_1555; /*vector<int>*/
  kk_ref_t loc = kk_ref_alloc((kk_vector_box(init_1294, _ctx)),kk_context()); /*local-var<933,vector<int>>*/;
  kk_unit_t x_1296 = kk_Unit;
  kk_integer_t _brw_1552 = kk_integer_from_small(0); /*int*/;
  kk_unit_t _brw_1553 = kk_Unit;
  kk_ref_vector_assign_borrow(loc,_brw_1552,(kk_integer_box(kk_integer_from_small(40))),kk_context());
  kk_integer_drop(_brw_1552, _ctx);
  _brw_1553;
  kk_vector_t res;
  if (kk_yielding(kk_context())) {
    kk_box_t _x1646;
    kk_function_t _x1647;
    kk_ref_dup(loc);
    _x1647 = kk_bitmap_new_get_dib_header_fun1648(dpcm, horizontal_resolution, loc, vertical_resolution, _ctx); /*(1001) -> 1003 1002*/
    _x1646 = kk_std_core_hnd_yield_extend(_x1647, _ctx); /*1002*/
    res = kk_vector_unbox(_x1646, _ctx); /*vector<int>*/
  }
  else {
    kk_ref_t _x1651 = kk_ref_dup(loc); /*local-var<933,vector<int>>*/
    res = kk_bitmap__mlift1271_get_dib_header(_x1651, dpcm, horizontal_resolution, vertical_resolution, x_1296, _ctx); /*vector<int>*/
  }
  kk_box_t _x1652 = kk_std_core_hnd_prompt_local_var(loc, kk_vector_box(res, _ctx), _ctx); /*1002*/
  return kk_vector_unbox(_x1652, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift1273_get_header_fun1655__t {
  struct kk_function_s _base;
};
static kk_box_t kk_bitmap__mlift1273_get_header_fun1655(kk_function_t _fself, kk_box_t _b_1477, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1273_get_header_fun1655(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_bitmap__mlift1273_get_header_fun1655, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_bitmap__mlift1273_get_header_fun1655(kk_function_t _fself, kk_box_t _b_1477, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core__list _x1656;
  kk_integer_t _x1657 = kk_integer_unbox(_b_1477); /*int*/
  _x1656 = kk_bitmap_bytes(_x1657, _ctx); /*list<int>*/
  return kk_std_core__list_box(_x1656, _ctx);
}


// lift anonymous function
struct kk_bitmap__mlift1273_get_header_fun1659__t {
  struct kk_function_s _base;
  kk_std_core__list file_size_bytes;
  kk_ref_t header;
};
static kk_unit_t kk_bitmap__mlift1273_get_header_fun1659(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1273_get_header_fun1659(kk_std_core__list file_size_bytes, kk_ref_t header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1273_get_header_fun1659__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1273_get_header_fun1659__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1273_get_header_fun1659, kk_context());
  _self->file_size_bytes = file_size_bytes;
  _self->header = header;
  return &_self->_base;
}

static kk_unit_t kk_bitmap__mlift1273_get_header_fun1659(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1273_get_header_fun1659__t* _self = kk_function_as(struct kk_bitmap__mlift1273_get_header_fun1659__t*, _fself);
  kk_std_core__list file_size_bytes = _self->file_size_bytes; /* list<int> */
  kk_ref_t header = _self->header; /* local-var<1211,vector<int>> */
  kk_drop_match(_self, {kk_std_core__list_dup(file_size_bytes);kk_ref_dup(header);}, {}, _ctx)
  kk_integer_t _b_1486_1483;
  kk_integer_t _x1660 = kk_integer_dup(i); /*int*/
  _b_1486_1483 = kk_integer_add(_x1660,(kk_integer_from_small(2)),kk_context()); /*int*/
  kk_std_core_types__maybe _b_1488_1480 = kk_std_core__lp__lb__rb__2_rp_(file_size_bytes, i, _ctx); /*maybe<int>*/;
  kk_integer_t _b_1487_1484;
  kk_box_t _x1661 = kk_std_core_maybe_1(_b_1488_1480, kk_integer_box(kk_integer_from_small(0)), _ctx); /*1001*/
  _b_1487_1484 = kk_integer_unbox(_x1661); /*int*/
  kk_unit_t _brw_1550 = kk_Unit;
  kk_ref_vector_assign_borrow(header,_b_1486_1483,(kk_integer_box(_b_1487_1484)),kk_context());
  kk_ref_drop(header, _ctx);
  kk_integer_drop(_b_1486_1483, _ctx);
  return _brw_1550;
}


// lift anonymous function
struct kk_bitmap__mlift1273_get_header_fun1663__t {
  struct kk_function_s _base;
  kk_ref_t header;
};
static kk_box_t kk_bitmap__mlift1273_get_header_fun1663(kk_function_t _fself, kk_box_t _b_1492, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1273_get_header_fun1663(kk_ref_t header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1273_get_header_fun1663__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1273_get_header_fun1663__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1273_get_header_fun1663, kk_context());
  _self->header = header;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift1273_get_header_fun1663(kk_function_t _fself, kk_box_t _b_1492, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1273_get_header_fun1663__t* _self = kk_function_as(struct kk_bitmap__mlift1273_get_header_fun1663__t*, _fself);
  kk_ref_t header = _self->header; /* local-var<1211,vector<int>> */
  kk_drop_match(_self, {kk_ref_dup(header);}, {}, _ctx)
  kk_box_drop(_b_1492, _ctx);
  return (kk_ref_get(header,kk_context()));
}

kk_vector_t kk_bitmap__mlift1273_get_header(kk_integer_t file_size, kk_ref_t header, kk_unit_t wild__1, kk_context_t* _ctx) { /* forall<h> (file-size : int, header : local-var<h,vector<int>>, wild_1 : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_std_core__list file_size_bytes;
  kk_box_t _x1654 = kk_std_core_hnd__open_none1(kk_bitmap__new_mlift1273_get_header_fun1655(_ctx), kk_integer_box(file_size), _ctx); /*1002*/
  file_size_bytes = kk_std_core__list_unbox(_x1654, _ctx); /*list<int>*/
  kk_unit_t x_1298 = kk_Unit;
  kk_function_t _x1658;
  kk_ref_dup(header);
  _x1658 = kk_bitmap__new_mlift1273_get_header_fun1659(file_size_bytes, header, _ctx); /*(i : int) -> <exn,local<1211>,div> ()*/
  kk_std_core__lift17275_for(_x1658, kk_integer_from_small(13), kk_integer_from_small(0), _ctx);
  kk_box_t _x1662;
  if (kk_yielding(kk_context())) {
    _x1662 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift1273_get_header_fun1663(header, _ctx), _ctx); /*1002*/
  }
  else {
    _x1662 = (kk_ref_get(header,kk_context())); /*1002*/
  }
  return kk_vector_unbox(_x1662, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift1274_get_header_fun1665__t {
  struct kk_function_s _base;
  kk_integer_t file_size;
  kk_ref_t header;
};
static kk_box_t kk_bitmap__mlift1274_get_header_fun1665(kk_function_t _fself, kk_box_t _b_1505, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1274_get_header_fun1665(kk_integer_t file_size, kk_ref_t header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1274_get_header_fun1665__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1274_get_header_fun1665__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1274_get_header_fun1665, kk_context());
  _self->file_size = file_size;
  _self->header = header;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift1274_get_header_fun1665(kk_function_t _fself, kk_box_t _b_1505, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1274_get_header_fun1665__t* _self = kk_function_as(struct kk_bitmap__mlift1274_get_header_fun1665__t*, _fself);
  kk_integer_t file_size = _self->file_size; /* int */
  kk_ref_t header = _self->header; /* local-var<1211,vector<int>> */
  kk_drop_match(_self, {kk_integer_dup(file_size);kk_ref_dup(header);}, {}, _ctx)
  kk_vector_t _x1666;
  kk_unit_t _x1667 = kk_Unit;
  kk_unit_unbox(_b_1505);
  _x1666 = kk_bitmap__mlift1273_get_header(file_size, header, _x1667, _ctx); /*vector<int>*/
  return kk_vector_box(_x1666, _ctx);
}

kk_vector_t kk_bitmap__mlift1274_get_header(kk_integer_t file_size, kk_ref_t header, kk_unit_t wild__0, kk_context_t* _ctx) { /* forall<h> (file-size : int, header : local-var<h,vector<int>>, wild_0 : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_unit_t x_1302 = kk_Unit;
  kk_integer_t _brw_1547 = kk_integer_from_small(10); /*int*/;
  kk_unit_t _brw_1548 = kk_Unit;
  kk_ref_vector_assign_borrow(header,_brw_1547,(kk_integer_box(kk_integer_from_small(54))),kk_context());
  kk_integer_drop(_brw_1547, _ctx);
  _brw_1548;
  if (kk_yielding(kk_context())) {
    kk_box_t _x1664 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift1274_get_header_fun1665(file_size, header, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x1664, _ctx);
  }
  {
    return kk_bitmap__mlift1273_get_header(file_size, header, x_1302, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_bitmap__mlift1275_get_header_fun1669__t {
  struct kk_function_s _base;
  kk_integer_t file_size;
  kk_ref_t header;
};
static kk_box_t kk_bitmap__mlift1275_get_header_fun1669(kk_function_t _fself, kk_box_t _b_1515, kk_context_t* _ctx);
static kk_function_t kk_bitmap__new_mlift1275_get_header_fun1669(kk_integer_t file_size, kk_ref_t header, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1275_get_header_fun1669__t* _self = kk_function_alloc_as(struct kk_bitmap__mlift1275_get_header_fun1669__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap__mlift1275_get_header_fun1669, kk_context());
  _self->file_size = file_size;
  _self->header = header;
  return &_self->_base;
}

static kk_box_t kk_bitmap__mlift1275_get_header_fun1669(kk_function_t _fself, kk_box_t _b_1515, kk_context_t* _ctx) {
  struct kk_bitmap__mlift1275_get_header_fun1669__t* _self = kk_function_as(struct kk_bitmap__mlift1275_get_header_fun1669__t*, _fself);
  kk_integer_t file_size = _self->file_size; /* int */
  kk_ref_t header = _self->header; /* local-var<1211,vector<int>> */
  kk_drop_match(_self, {kk_integer_dup(file_size);kk_ref_dup(header);}, {}, _ctx)
  kk_vector_t _x1670;
  kk_unit_t _x1671 = kk_Unit;
  kk_unit_unbox(_b_1515);
  _x1670 = kk_bitmap__mlift1274_get_header(file_size, header, _x1671, _ctx); /*vector<int>*/
  return kk_vector_box(_x1670, _ctx);
}

kk_vector_t kk_bitmap__mlift1275_get_header(kk_integer_t file_size, kk_ref_t header, kk_unit_t wild__, kk_context_t* _ctx) { /* forall<h> (file-size : int, header : local-var<h,vector<int>>, wild_ : ()) -> <local<h>,exn,div> vector<int> */ 
  kk_unit_t x_1304 = kk_Unit;
  kk_integer_t _brw_1544 = kk_integer_from_small(1); /*int*/;
  kk_unit_t _brw_1545 = kk_Unit;
  kk_ref_vector_assign_borrow(header,_brw_1544,(kk_integer_box(kk_integer_from_small(77))),kk_context());
  kk_integer_drop(_brw_1544, _ctx);
  _brw_1545;
  if (kk_yielding(kk_context())) {
    kk_box_t _x1668 = kk_std_core_hnd_yield_extend(kk_bitmap__new_mlift1275_get_header_fun1669(file_size, header, _ctx), _ctx); /*1002*/
    return kk_vector_unbox(_x1668, _ctx);
  }
  {
    return kk_bitmap__mlift1274_get_header(file_size, header, x_1304, _ctx);
  }
}


// lift anonymous function
struct kk_bitmap_get_header_fun1674__t {
  struct kk_function_s _base;
  kk_integer_t file_size;
  kk_ref_t loc;
};
static kk_box_t kk_bitmap_get_header_fun1674(kk_function_t _fself, kk_box_t _b_1531, kk_context_t* _ctx);
static kk_function_t kk_bitmap_new_get_header_fun1674(kk_integer_t file_size, kk_ref_t loc, kk_context_t* _ctx) {
  struct kk_bitmap_get_header_fun1674__t* _self = kk_function_alloc_as(struct kk_bitmap_get_header_fun1674__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_bitmap_get_header_fun1674, kk_context());
  _self->file_size = file_size;
  _self->loc = loc;
  return &_self->_base;
}

static kk_box_t kk_bitmap_get_header_fun1674(kk_function_t _fself, kk_box_t _b_1531, kk_context_t* _ctx) {
  struct kk_bitmap_get_header_fun1674__t* _self = kk_function_as(struct kk_bitmap_get_header_fun1674__t*, _fself);
  kk_integer_t file_size = _self->file_size; /* int */
  kk_ref_t loc = _self->loc; /* local-var<1211,vector<int>> */
  kk_drop_match(_self, {kk_integer_dup(file_size);kk_ref_dup(loc);}, {}, _ctx)
  kk_vector_t _x1675;
  kk_unit_t _x1676 = kk_Unit;
  kk_unit_unbox(_b_1531);
  _x1675 = kk_bitmap__mlift1275_get_header(file_size, loc, _x1676, _ctx); /*vector<int>*/
  return kk_vector_box(_x1675, _ctx);
}

kk_vector_t kk_bitmap_get_header(kk_integer_t file_size, kk_context_t* _ctx) { /* (file-size : int) -> pure vector<int> */ 
  kk_vector_t init_1306;
  kk_integer_t _brw_1541 = kk_integer_from_small(14); /*int*/;
  kk_vector_t _brw_1542 = kk_std_core_vector_2(_brw_1541, kk_integer_box(kk_integer_from_small(0)), _ctx); /*vector<1001>*/;
  kk_integer_drop(_brw_1541, _ctx);
  init_1306 = _brw_1542; /*vector<int>*/
  kk_ref_t loc = kk_ref_alloc((kk_vector_box(init_1306, _ctx)),kk_context()); /*local-var<1211,vector<int>>*/;
  kk_unit_t x_1308 = kk_Unit;
  kk_integer_t _brw_1539 = kk_integer_from_small(0); /*int*/;
  kk_unit_t _brw_1540 = kk_Unit;
  kk_ref_vector_assign_borrow(loc,_brw_1539,(kk_integer_box(kk_integer_from_small(66))),kk_context());
  kk_integer_drop(_brw_1539, _ctx);
  _brw_1540;
  kk_vector_t res;
  if (kk_yielding(kk_context())) {
    kk_box_t _x1672;
    kk_function_t _x1673;
    kk_ref_dup(loc);
    _x1673 = kk_bitmap_new_get_header_fun1674(file_size, loc, _ctx); /*(1001) -> 1003 1002*/
    _x1672 = kk_std_core_hnd_yield_extend(_x1673, _ctx); /*1002*/
    res = kk_vector_unbox(_x1672, _ctx); /*vector<int>*/
  }
  else {
    kk_ref_t _x1677 = kk_ref_dup(loc); /*local-var<1211,vector<int>>*/
    res = kk_bitmap__mlift1275_get_header(file_size, _x1677, x_1308, _ctx); /*vector<int>*/
  }
  kk_box_t _x1678 = kk_std_core_hnd_prompt_local_var(loc, kk_vector_box(res, _ctx), _ctx); /*1002*/
  return kk_vector_unbox(_x1678, _ctx);
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
