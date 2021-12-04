// Koka generated module: "display", koka version: 2.3.2, platform: 64-bit
#include "display.h"
 
// Display lists of primitives

kk_unit_t kk_display_println(kk_std_core__list ls, kk_context_t* _ctx) { /* (ls : list<int>) -> console () */ 
  kk_unit_t __1 = kk_Unit;
  if (kk_std_core__is_Nil(ls)) {
    
  }
  {
    struct kk_std_core_Cons* _con1432 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x1350 = _con1432->head;
    kk_std_core__list _pat1 = _con1432->tail;
    if (kk_std_core__is_Nil(_pat1)) {
      kk_integer_t x = kk_integer_unbox(_box_x1350);
      if (kk_likely(kk_std_core__list_is_unique(ls))) {
        kk_std_core__list_free(ls);
      }
      else {
        kk_integer_dup(x);
        kk_std_core__list_decref(ls, _ctx);
      }
      kk_string_t _x1434 = kk_std_core_show(x, _ctx); /*string*/
      kk_std_core_printsln(_x1434, _ctx);
      goto _match1431;
    }
  }
  {
    struct kk_std_core_Cons* _con1435 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x1351 = _con1435->head;
    kk_std_core__list xs = _con1435->tail;
    kk_integer_t x0 = kk_integer_unbox(_box_x1351);
    if (kk_likely(kk_std_core__list_is_unique(ls))) {
      kk_std_core__list_free(ls);
    }
    else {
      kk_integer_dup(x0);
      kk_std_core__list_dup(xs);
      kk_std_core__list_decref(ls, _ctx);
    }
    kk_unit_t __ = kk_Unit;
    kk_string_t _x1437 = kk_std_core_show(x0, _ctx); /*string*/
    kk_std_core_prints(_x1437, _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_string_t _x1438;
    kk_define_string_literal(, _s1439, 2, ", ")
    _x1438 = kk_string_dup(_s1439); /*string*/
    kk_std_core_prints(_x1438, _ctx);
    kk_display_println(xs, _ctx);
  }
  _match1431: ;
  kk_Unit; return kk_Unit;
}

kk_unit_t kk_display_println_1(kk_std_core__list ls, kk_context_t* _ctx) { /* (ls : list<char>) -> console () */ 
  kk_unit_t __1 = kk_Unit;
  if (kk_std_core__is_Nil(ls)) {
    
  }
  {
    struct kk_std_core_Cons* _con1441 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x1352 = _con1441->head;
    kk_std_core__list _pat1 = _con1441->tail;
    if (kk_std_core__is_Nil(_pat1)) {
      kk_char_t x = kk_char_unbox(_box_x1352, NULL);
      if (kk_likely(kk_std_core__list_is_unique(ls))) {
        kk_std_core__list_free(ls);
      }
      else {
        kk_std_core__list_decref(ls, _ctx);
      }
      kk_string_t _x1443 = kk_std_core_string(x, _ctx); /*string*/
      kk_std_core_printsln(_x1443, _ctx);
      goto _match1440;
    }
  }
  {
    struct kk_std_core_Cons* _con1444 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x1353 = _con1444->head;
    kk_std_core__list xs = _con1444->tail;
    kk_char_t x0 = kk_char_unbox(_box_x1353, NULL);
    if (kk_likely(kk_std_core__list_is_unique(ls))) {
      kk_std_core__list_free(ls);
    }
    else {
      kk_std_core__list_dup(xs);
      kk_std_core__list_decref(ls, _ctx);
    }
    kk_unit_t __ = kk_Unit;
    kk_string_t _x1446 = kk_std_core_string(x0, _ctx); /*string*/
    kk_std_core_prints(_x1446, _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_string_t _x1447;
    kk_define_string_literal(, _s1448, 2, ", ")
    _x1447 = kk_string_dup(_s1448); /*string*/
    kk_std_core_prints(_x1447, _ctx);
    kk_display_println_1(xs, _ctx);
  }
  _match1440: ;
  kk_Unit; return kk_Unit;
}

kk_unit_t kk_display_println_2(kk_std_core__list ls, kk_context_t* _ctx) { /* (ls : list<bool>) -> console () */ 
  kk_unit_t __1 = kk_Unit;
  if (kk_std_core__is_Nil(ls)) {
    
  }
  {
    struct kk_std_core_Cons* _con1450 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x1354 = _con1450->head;
    kk_std_core__list _pat1 = _con1450->tail;
    if (kk_std_core__is_Nil(_pat1)) {
      bool x = kk_bool_unbox(_box_x1354);
      if (kk_likely(kk_std_core__list_is_unique(ls))) {
        kk_box_drop(_box_x1354, _ctx);
        kk_std_core__list_free(ls);
      }
      else {
        kk_std_core__list_decref(ls, _ctx);
      }
      kk_string_t _x1452;
      if (x) {
        kk_define_string_literal(, _s1453, 4, "True")
        _x1452 = kk_string_dup(_s1453); /*string*/
      }
      else {
        kk_define_string_literal(, _s1454, 5, "False")
        _x1452 = kk_string_dup(_s1454); /*string*/
      }
      kk_std_core_printsln(_x1452, _ctx);
      goto _match1449;
    }
  }
  {
    struct kk_std_core_Cons* _con1455 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x1355 = _con1455->head;
    kk_std_core__list xs = _con1455->tail;
    bool x0 = kk_bool_unbox(_box_x1355);
    if (kk_likely(kk_std_core__list_is_unique(ls))) {
      kk_box_drop(_box_x1355, _ctx);
      kk_std_core__list_free(ls);
    }
    else {
      kk_std_core__list_dup(xs);
      kk_std_core__list_decref(ls, _ctx);
    }
    kk_unit_t __ = kk_Unit;
    kk_string_t _x1457;
    if (x0) {
      kk_define_string_literal(, _s1458, 4, "True")
      _x1457 = kk_string_dup(_s1458); /*string*/
    }
    else {
      kk_define_string_literal(, _s1459, 5, "False")
      _x1457 = kk_string_dup(_s1459); /*string*/
    }
    kk_std_core_prints(_x1457, _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_string_t _x1460;
    kk_define_string_literal(, _s1461, 2, ", ")
    _x1460 = kk_string_dup(_s1461); /*string*/
    kk_std_core_prints(_x1460, _ctx);
    kk_display_println_2(xs, _ctx);
  }
  _match1449: ;
  kk_Unit; return kk_Unit;
}

kk_unit_t kk_display_println_3(kk_std_core__list ls, kk_context_t* _ctx) { /* (ls : list<double>) -> console () */ 
  kk_unit_t __1 = kk_Unit;
  if (kk_std_core__is_Nil(ls)) {
    
  }
  {
    struct kk_std_core_Cons* _con1463 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x1356 = _con1463->head;
    kk_std_core__list _pat1 = _con1463->tail;
    if (kk_std_core__is_Nil(_pat1)) {
      double x = kk_double_unbox(_box_x1356, NULL);
      if (kk_likely(kk_std_core__list_is_unique(ls))) {
        kk_box_drop(_box_x1356, _ctx);
        kk_std_core__list_free(ls);
      }
      else {
        kk_std_core__list_decref(ls, _ctx);
      }
      kk_string_t _x1465 = kk_std_core_show_1(x, kk_std_core_types__new_None(_ctx), _ctx); /*string*/
      kk_std_core_printsln(_x1465, _ctx);
      goto _match1462;
    }
  }
  {
    struct kk_std_core_Cons* _con1466 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x1357 = _con1466->head;
    kk_std_core__list xs = _con1466->tail;
    double x0 = kk_double_unbox(_box_x1357, NULL);
    if (kk_likely(kk_std_core__list_is_unique(ls))) {
      kk_box_drop(_box_x1357, _ctx);
      kk_std_core__list_free(ls);
    }
    else {
      kk_std_core__list_dup(xs);
      kk_std_core__list_decref(ls, _ctx);
    }
    kk_unit_t __ = kk_Unit;
    kk_string_t _x1468 = kk_std_core_show_1(x0, kk_std_core_types__new_None(_ctx), _ctx); /*string*/
    kk_std_core_prints(_x1468, _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_string_t _x1469;
    kk_define_string_literal(, _s1470, 2, ", ")
    _x1469 = kk_string_dup(_s1470); /*string*/
    kk_std_core_prints(_x1469, _ctx);
    kk_display_println_3(xs, _ctx);
  }
  _match1462: ;
  kk_Unit; return kk_Unit;
}

kk_unit_t kk_display_println_4(kk_std_core__list ls, kk_context_t* _ctx) { /* (ls : list<string>) -> console () */ 
  kk_unit_t __1 = kk_Unit;
  if (kk_std_core__is_Nil(ls)) {
    
  }
  {
    struct kk_std_core_Cons* _con1472 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x1358 = _con1472->head;
    kk_std_core__list _pat1 = _con1472->tail;
    if (kk_std_core__is_Nil(_pat1)) {
      kk_string_t x = kk_string_unbox(_box_x1358);
      if (kk_likely(kk_std_core__list_is_unique(ls))) {
        kk_std_core__list_free(ls);
      }
      else {
        kk_string_dup(x);
        kk_std_core__list_decref(ls, _ctx);
      }
      kk_std_core_printsln(x, _ctx);
      goto _match1471;
    }
  }
  {
    struct kk_std_core_Cons* _con1474 = kk_std_core__as_Cons(ls);
    kk_box_t _box_x1359 = _con1474->head;
    kk_std_core__list xs = _con1474->tail;
    kk_string_t x0 = kk_string_unbox(_box_x1359);
    if (kk_likely(kk_std_core__list_is_unique(ls))) {
      kk_std_core__list_free(ls);
    }
    else {
      kk_string_dup(x0);
      kk_std_core__list_dup(xs);
      kk_std_core__list_decref(ls, _ctx);
    }
    kk_unit_t __ = kk_Unit;
    kk_std_core_prints(x0, _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_string_t _x1476;
    kk_define_string_literal(, _s1477, 2, ", ")
    _x1476 = kk_string_dup(_s1477); /*string*/
    kk_std_core_prints(_x1476, _ctx);
    kk_display_println_4(xs, _ctx);
  }
  _match1471: ;
  kk_Unit; return kk_Unit;
}
 
// Display vectors of primitives


// lift anonymous function
struct kk_display_println_fun1479__t_5 {
  struct kk_function_s _base;
  kk_vector_t vec;
};
static kk_unit_t kk_display_println_fun1479_5(kk_function_t _fself, kk_box_t _b_1362, kk_ssize_t _b_1363, kk_context_t* _ctx);
static kk_function_t kk_display_new_println_fun1479_5(kk_vector_t vec, kk_context_t* _ctx) {
  struct kk_display_println_fun1479__t_5* _self = kk_function_alloc_as(struct kk_display_println_fun1479__t_5, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_display_println_fun1479_5, kk_context());
  _self->vec = vec;
  return &_self->_base;
}

static kk_unit_t kk_display_println_fun1479_5(kk_function_t _fself, kk_box_t _b_1362, kk_ssize_t _b_1363, kk_context_t* _ctx) {
  struct kk_display_println_fun1479__t_5* _self = kk_function_as(struct kk_display_println_fun1479__t_5*, _fself);
  kk_vector_t vec = _self->vec; /* vector<int> */
  kk_drop_match(_self, {kk_vector_dup(vec);}, {}, _ctx)
  kk_integer_t index_1341 = kk_integer_from_ssize_t(_b_1363,kk_context()); /*int*/;
  kk_unit_t __ = kk_Unit;
  kk_string_t _x1480;
  kk_integer_t _x1481 = kk_integer_unbox(_b_1362); /*int*/
  _x1480 = kk_std_core_show(_x1481, _ctx); /*string*/
  kk_std_core_prints(_x1480, _ctx);
  bool _match_1417;
  kk_integer_t _brw_1418;
  kk_integer_t _x1482;
  kk_ssize_t _x1483;
  kk_ssize_t _brw_1419 = kk_vector_len_borrow(vec); /*ssize_t*/;
  kk_vector_drop(vec, _ctx);
  _x1483 = _brw_1419; /*ssize_t*/
  _x1482 = kk_integer_from_ssize_t(_x1483,kk_context()); /*int*/
  _brw_1418 = kk_integer_sub(_x1482,(kk_integer_from_small(1)),kk_context()); /*int*/
  bool _brw_1420 = kk_integer_eq_borrow(index_1341,_brw_1418,kk_context()); /*bool*/;
  kk_integer_drop(index_1341, _ctx);
  kk_integer_drop(_brw_1418, _ctx);
  _match_1417 = _brw_1420; /*bool*/
  if (_match_1417) {
    kk_string_t _x1484 = kk_string_empty(); /*string*/
    return kk_std_core_printsln(_x1484, _ctx);
  }
  {
    kk_string_t _x1486;
    kk_define_string_literal(, _s1487, 2, ", ")
    _x1486 = kk_string_dup(_s1487); /*string*/
    return kk_std_core_prints(_x1486, _ctx);
  }
}

kk_unit_t kk_display_println_5(kk_vector_t vec, kk_context_t* _ctx) { /* (vec : vector<int>) -> console () */ 
  kk_unit_t __0 = kk_Unit;
  kk_vector_t _x1478 = kk_vector_dup(vec); /*vector<int>*/
  kk_std_core_foreach_indexedz(_x1478, kk_display_new_println_fun1479_5(vec, _ctx), _ctx);
  kk_Unit; return kk_Unit;
}


// lift anonymous function
struct kk_display_println_fun1489__t_6 {
  struct kk_function_s _base;
  kk_vector_t vec;
};
static kk_unit_t kk_display_println_fun1489_6(kk_function_t _fself, kk_box_t _b_1370, kk_ssize_t _b_1371, kk_context_t* _ctx);
static kk_function_t kk_display_new_println_fun1489_6(kk_vector_t vec, kk_context_t* _ctx) {
  struct kk_display_println_fun1489__t_6* _self = kk_function_alloc_as(struct kk_display_println_fun1489__t_6, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_display_println_fun1489_6, kk_context());
  _self->vec = vec;
  return &_self->_base;
}

static kk_unit_t kk_display_println_fun1489_6(kk_function_t _fself, kk_box_t _b_1370, kk_ssize_t _b_1371, kk_context_t* _ctx) {
  struct kk_display_println_fun1489__t_6* _self = kk_function_as(struct kk_display_println_fun1489__t_6*, _fself);
  kk_vector_t vec = _self->vec; /* vector<char> */
  kk_drop_match(_self, {kk_vector_dup(vec);}, {}, _ctx)
  kk_integer_t index_1343 = kk_integer_from_ssize_t(_b_1371,kk_context()); /*int*/;
  kk_unit_t __ = kk_Unit;
  kk_string_t _x1490;
  kk_char_t _x1491 = kk_char_unbox(_b_1370, _ctx); /*char*/
  _x1490 = kk_std_core_string(_x1491, _ctx); /*string*/
  kk_std_core_prints(_x1490, _ctx);
  bool _match_1413;
  kk_integer_t _brw_1414;
  kk_integer_t _x1492;
  kk_ssize_t _x1493;
  kk_ssize_t _brw_1415 = kk_vector_len_borrow(vec); /*ssize_t*/;
  kk_vector_drop(vec, _ctx);
  _x1493 = _brw_1415; /*ssize_t*/
  _x1492 = kk_integer_from_ssize_t(_x1493,kk_context()); /*int*/
  _brw_1414 = kk_integer_sub(_x1492,(kk_integer_from_small(1)),kk_context()); /*int*/
  bool _brw_1416 = kk_integer_eq_borrow(index_1343,_brw_1414,kk_context()); /*bool*/;
  kk_integer_drop(index_1343, _ctx);
  kk_integer_drop(_brw_1414, _ctx);
  _match_1413 = _brw_1416; /*bool*/
  if (_match_1413) {
    kk_string_t _x1494 = kk_string_empty(); /*string*/
    return kk_std_core_printsln(_x1494, _ctx);
  }
  {
    kk_string_t _x1496;
    kk_define_string_literal(, _s1497, 2, ", ")
    _x1496 = kk_string_dup(_s1497); /*string*/
    return kk_std_core_prints(_x1496, _ctx);
  }
}

kk_unit_t kk_display_println_6(kk_vector_t vec, kk_context_t* _ctx) { /* (vec : vector<char>) -> console () */ 
  kk_unit_t __0 = kk_Unit;
  kk_vector_t _x1488 = kk_vector_dup(vec); /*vector<char>*/
  kk_std_core_foreach_indexedz(_x1488, kk_display_new_println_fun1489_6(vec, _ctx), _ctx);
  kk_Unit; return kk_Unit;
}


// lift anonymous function
struct kk_display_println_fun1499__t_7 {
  struct kk_function_s _base;
  kk_vector_t vec;
};
static kk_unit_t kk_display_println_fun1499_7(kk_function_t _fself, kk_box_t _b_1378, kk_ssize_t _b_1379, kk_context_t* _ctx);
static kk_function_t kk_display_new_println_fun1499_7(kk_vector_t vec, kk_context_t* _ctx) {
  struct kk_display_println_fun1499__t_7* _self = kk_function_alloc_as(struct kk_display_println_fun1499__t_7, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_display_println_fun1499_7, kk_context());
  _self->vec = vec;
  return &_self->_base;
}

static kk_unit_t kk_display_println_fun1499_7(kk_function_t _fself, kk_box_t _b_1378, kk_ssize_t _b_1379, kk_context_t* _ctx) {
  struct kk_display_println_fun1499__t_7* _self = kk_function_as(struct kk_display_println_fun1499__t_7*, _fself);
  kk_vector_t vec = _self->vec; /* vector<bool> */
  kk_drop_match(_self, {kk_vector_dup(vec);}, {}, _ctx)
  kk_integer_t index_1345 = kk_integer_from_ssize_t(_b_1379,kk_context()); /*int*/;
  kk_unit_t __ = kk_Unit;
  kk_string_t _x1500;
  bool _match_1412 = kk_bool_unbox(_b_1378); /*bool*/;
  if (_match_1412) {
    kk_define_string_literal(, _s1501, 4, "True")
    _x1500 = kk_string_dup(_s1501); /*string*/
  }
  else {
    kk_define_string_literal(, _s1502, 5, "False")
    _x1500 = kk_string_dup(_s1502); /*string*/
  }
  kk_std_core_prints(_x1500, _ctx);
  bool _match_1408;
  kk_integer_t _brw_1409;
  kk_integer_t _x1503;
  kk_ssize_t _x1504;
  kk_ssize_t _brw_1410 = kk_vector_len_borrow(vec); /*ssize_t*/;
  kk_vector_drop(vec, _ctx);
  _x1504 = _brw_1410; /*ssize_t*/
  _x1503 = kk_integer_from_ssize_t(_x1504,kk_context()); /*int*/
  _brw_1409 = kk_integer_sub(_x1503,(kk_integer_from_small(1)),kk_context()); /*int*/
  bool _brw_1411 = kk_integer_eq_borrow(index_1345,_brw_1409,kk_context()); /*bool*/;
  kk_integer_drop(index_1345, _ctx);
  kk_integer_drop(_brw_1409, _ctx);
  _match_1408 = _brw_1411; /*bool*/
  if (_match_1408) {
    kk_string_t _x1505 = kk_string_empty(); /*string*/
    return kk_std_core_printsln(_x1505, _ctx);
  }
  {
    kk_string_t _x1507;
    kk_define_string_literal(, _s1508, 2, ", ")
    _x1507 = kk_string_dup(_s1508); /*string*/
    return kk_std_core_prints(_x1507, _ctx);
  }
}

kk_unit_t kk_display_println_7(kk_vector_t vec, kk_context_t* _ctx) { /* (vec : vector<bool>) -> console () */ 
  kk_unit_t __0 = kk_Unit;
  kk_vector_t _x1498 = kk_vector_dup(vec); /*vector<bool>*/
  kk_std_core_foreach_indexedz(_x1498, kk_display_new_println_fun1499_7(vec, _ctx), _ctx);
  kk_Unit; return kk_Unit;
}


// lift anonymous function
struct kk_display_println_fun1510__t_8 {
  struct kk_function_s _base;
  kk_vector_t vec;
};
static kk_unit_t kk_display_println_fun1510_8(kk_function_t _fself, kk_box_t _b_1386, kk_ssize_t _b_1387, kk_context_t* _ctx);
static kk_function_t kk_display_new_println_fun1510_8(kk_vector_t vec, kk_context_t* _ctx) {
  struct kk_display_println_fun1510__t_8* _self = kk_function_alloc_as(struct kk_display_println_fun1510__t_8, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_display_println_fun1510_8, kk_context());
  _self->vec = vec;
  return &_self->_base;
}

static kk_unit_t kk_display_println_fun1510_8(kk_function_t _fself, kk_box_t _b_1386, kk_ssize_t _b_1387, kk_context_t* _ctx) {
  struct kk_display_println_fun1510__t_8* _self = kk_function_as(struct kk_display_println_fun1510__t_8*, _fself);
  kk_vector_t vec = _self->vec; /* vector<double> */
  kk_drop_match(_self, {kk_vector_dup(vec);}, {}, _ctx)
  kk_integer_t index_1347 = kk_integer_from_ssize_t(_b_1387,kk_context()); /*int*/;
  kk_unit_t __ = kk_Unit;
  kk_string_t _x1511;
  double _x1512 = kk_double_unbox(_b_1386, _ctx); /*double*/
  _x1511 = kk_std_core_show_1(_x1512, kk_std_core_types__new_None(_ctx), _ctx); /*string*/
  kk_std_core_prints(_x1511, _ctx);
  bool _match_1404;
  kk_integer_t _brw_1405;
  kk_integer_t _x1513;
  kk_ssize_t _x1514;
  kk_ssize_t _brw_1406 = kk_vector_len_borrow(vec); /*ssize_t*/;
  kk_vector_drop(vec, _ctx);
  _x1514 = _brw_1406; /*ssize_t*/
  _x1513 = kk_integer_from_ssize_t(_x1514,kk_context()); /*int*/
  _brw_1405 = kk_integer_sub(_x1513,(kk_integer_from_small(1)),kk_context()); /*int*/
  bool _brw_1407 = kk_integer_eq_borrow(index_1347,_brw_1405,kk_context()); /*bool*/;
  kk_integer_drop(index_1347, _ctx);
  kk_integer_drop(_brw_1405, _ctx);
  _match_1404 = _brw_1407; /*bool*/
  if (_match_1404) {
    kk_string_t _x1515 = kk_string_empty(); /*string*/
    return kk_std_core_printsln(_x1515, _ctx);
  }
  {
    kk_string_t _x1517;
    kk_define_string_literal(, _s1518, 2, ", ")
    _x1517 = kk_string_dup(_s1518); /*string*/
    return kk_std_core_prints(_x1517, _ctx);
  }
}

kk_unit_t kk_display_println_8(kk_vector_t vec, kk_context_t* _ctx) { /* (vec : vector<double>) -> console () */ 
  kk_unit_t __0 = kk_Unit;
  kk_vector_t _x1509 = kk_vector_dup(vec); /*vector<double>*/
  kk_std_core_foreach_indexedz(_x1509, kk_display_new_println_fun1510_8(vec, _ctx), _ctx);
  kk_Unit; return kk_Unit;
}


// lift anonymous function
struct kk_display_println_fun1520__t_9 {
  struct kk_function_s _base;
  kk_vector_t vec;
};
static kk_unit_t kk_display_println_fun1520_9(kk_function_t _fself, kk_box_t _b_1394, kk_ssize_t _b_1395, kk_context_t* _ctx);
static kk_function_t kk_display_new_println_fun1520_9(kk_vector_t vec, kk_context_t* _ctx) {
  struct kk_display_println_fun1520__t_9* _self = kk_function_alloc_as(struct kk_display_println_fun1520__t_9, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_display_println_fun1520_9, kk_context());
  _self->vec = vec;
  return &_self->_base;
}

static kk_unit_t kk_display_println_fun1520_9(kk_function_t _fself, kk_box_t _b_1394, kk_ssize_t _b_1395, kk_context_t* _ctx) {
  struct kk_display_println_fun1520__t_9* _self = kk_function_as(struct kk_display_println_fun1520__t_9*, _fself);
  kk_vector_t vec = _self->vec; /* vector<string> */
  kk_drop_match(_self, {kk_vector_dup(vec);}, {}, _ctx)
  kk_integer_t index_1349 = kk_integer_from_ssize_t(_b_1395,kk_context()); /*int*/;
  kk_unit_t __ = kk_Unit;
  kk_string_t _x1521 = kk_string_unbox(_b_1394); /*string*/
  kk_std_core_prints(_x1521, _ctx);
  bool _match_1400;
  kk_integer_t _brw_1401;
  kk_integer_t _x1522;
  kk_ssize_t _x1523;
  kk_ssize_t _brw_1402 = kk_vector_len_borrow(vec); /*ssize_t*/;
  kk_vector_drop(vec, _ctx);
  _x1523 = _brw_1402; /*ssize_t*/
  _x1522 = kk_integer_from_ssize_t(_x1523,kk_context()); /*int*/
  _brw_1401 = kk_integer_sub(_x1522,(kk_integer_from_small(1)),kk_context()); /*int*/
  bool _brw_1403 = kk_integer_eq_borrow(index_1349,_brw_1401,kk_context()); /*bool*/;
  kk_integer_drop(index_1349, _ctx);
  kk_integer_drop(_brw_1401, _ctx);
  _match_1400 = _brw_1403; /*bool*/
  if (_match_1400) {
    kk_string_t _x1524 = kk_string_empty(); /*string*/
    return kk_std_core_printsln(_x1524, _ctx);
  }
  {
    kk_string_t _x1526;
    kk_define_string_literal(, _s1527, 2, ", ")
    _x1526 = kk_string_dup(_s1527); /*string*/
    return kk_std_core_prints(_x1526, _ctx);
  }
}

kk_unit_t kk_display_println_9(kk_vector_t vec, kk_context_t* _ctx) { /* (vec : vector<string>) -> console () */ 
  kk_unit_t __0 = kk_Unit;
  kk_vector_t _x1519 = kk_vector_dup(vec); /*vector<string>*/
  kk_std_core_foreach_indexedz(_x1519, kk_display_new_println_fun1520_9(vec, _ctx), _ctx);
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
