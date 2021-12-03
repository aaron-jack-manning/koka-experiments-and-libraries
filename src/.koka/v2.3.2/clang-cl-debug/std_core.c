// Koka generated module: "std/core", koka version: 2.3.2, platform: 64-bit
#include "std_core.h"
/*---------------------------------------------------------------------------
  Copyright 2020-2021, Microsoft Research, Daan Leijen.

  This is free software; you can redistribute it and/or modify it under the
  terms of the Apache License, Version 2.0. A copy of the License can be
  found in the LICENSE file at the root of this distribution.
---------------------------------------------------------------------------*/

kk_std_core__list kk_vector_to_list(kk_vector_t v, kk_std_core__list tail, kk_context_t* ctx) {
  // todo: avoid boxed_dup if v is unique
  kk_ssize_t n;
  kk_box_t* p = kk_vector_buf_borrow(v, &n);
  if (n <= 0) {
    kk_vector_drop(v,ctx);
    return tail;
  }
  kk_std_core__list nil  = kk_std_core__new_Nil(ctx);
  struct kk_std_core_Cons* cons = NULL;
  kk_std_core__list list = kk_std_core__new_Nil(ctx);
  for( kk_ssize_t i = 0; i < n; i++ ) {
    kk_std_core__list hd = kk_std_core__new_Cons(kk_reuse_null,kk_box_dup(p[i]), nil, ctx);
    if (cons==NULL) {
      list = hd;
    }
    else {
      cons->tail = hd;
    }
    cons = kk_std_core__as_Cons(hd);
  }
  if (cons == NULL) { list = tail; } 
               else { cons->tail = tail; }
  kk_vector_drop(v,ctx);
  return list;
}

kk_vector_t kk_list_to_vector(kk_std_core__list xs, kk_context_t* ctx) {
  // todo: avoid boxed_dup if xs is unique
  // find the length
  kk_ssize_t len = 0;
  kk_std_core__list ys = xs;
  while (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* cons = kk_std_core__as_Cons(ys);
    len++;
    ys = cons->tail;
  }
  // alloc the vector and copy
  kk_box_t* p;
  kk_vector_t v = kk_vector_alloc_uninit(len, &p, ctx);  
  ys = xs;
  for( kk_ssize_t i = 0; i < len; i++) {
    struct kk_std_core_Cons* cons = kk_std_core__as_Cons(ys);
    ys = cons->tail;
    p[i] = kk_box_dup(cons->head);
  }
  kk_std_core__list_drop(xs,ctx);  // todo: drop while visiting?
  return v;
}

kk_vector_t kk_vector_init( kk_ssize_t n, kk_function_t init, kk_context_t* ctx) {
  kk_box_t* p;
  kk_vector_t v = kk_vector_alloc_uninit(n, &p, ctx);  
  for(kk_ssize_t i = 0; i < n; i++) {
    kk_function_dup(init);
    p[i] = kk_function_call(kk_box_t,(kk_function_t,kk_ssize_t,kk_context_t*),init,(init,i,ctx));
  }
  kk_function_drop(init,ctx);
  return v;
}

kk_box_t kk_main_console( kk_function_t action, kk_context_t* ctx ) {
  return kk_function_call(kk_box_t,(kk_function_t,kk_unit_t,kk_context_t*),action,(action,kk_Unit,ctx));
}


kk_std_core__list kk_string_to_list(kk_string_t s, kk_context_t* ctx) {
  kk_ssize_t len;
  const uint8_t* p = kk_string_buf_borrow(s,&len);
  const uint8_t* const end = p + len;
  kk_std_core__list nil  = kk_std_core__new_Nil(ctx);
  kk_std_core__list list = nil;
  struct kk_std_core_Cons* tl = NULL;
  kk_ssize_t count;
  while( p < end ) {
    kk_char_t c = kk_utf8_read(p,&count);
    p += count;
    kk_std_core__list cons = kk_std_core__new_Cons(kk_reuse_null,kk_char_box(c,ctx), nil, ctx);
    if (tl!=NULL) {
      tl->tail = cons;
    }
    else {
      list = cons;
    }
    tl = kk_std_core__as_Cons(cons);
  }
  kk_string_drop(s,ctx);
  return list;
}

kk_string_t kk_string_from_list(kk_std_core__list cs, kk_context_t* ctx) {
  // TODO: optimize for short strings to write directly into a local buffer?
  // find total UTF8 length
  kk_ssize_t len = 0;
  kk_std_core__list xs = cs;
  while (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* cons = kk_std_core__as_Cons(xs);
    len += kk_utf8_len(kk_char_unbox(cons->head,ctx));
    xs = cons->tail;
  }
  // allocate and copy the characters
  uint8_t* p;
  kk_string_t s = kk_unsafe_string_alloc_buf(len,&p,ctx);  // must be initialized
  xs = cs;
  while (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* cons = kk_std_core__as_Cons(xs);
    kk_ssize_t count;
    kk_utf8_write( kk_char_unbox(cons->head,ctx), p, &count );
    p += count;
    xs = cons->tail;
  }
  kk_assert_internal(*p == 0 && (p - kk_string_buf_borrow(s,NULL)) == len);
  kk_std_core__list_drop(cs,ctx);  // todo: drop while visiting?
  return s;
}

static inline void kk_sslice_start_end_borrowx( kk_std_core__sslice sslice, const uint8_t** start, const uint8_t** end, const uint8_t** sstart, const uint8_t** send) {
  kk_ssize_t slen;
  const uint8_t* s = kk_string_buf_borrow(sslice.str,&slen);
  *start = s + sslice.start;
  *end = s + sslice.start + sslice.len;
  if (sstart != NULL) *sstart = s;
  if (send != NULL) *send = s + slen;
  kk_assert_internal(*start >= s && *start <= *end);
  kk_assert_internal(*end >= *start && *end <= s + slen);
}

static inline void kk_sslice_start_end_borrow( kk_std_core__sslice sslice, const uint8_t** start, const uint8_t** end) {
  kk_sslice_start_end_borrowx(sslice,start,end,NULL,NULL);
}

kk_integer_t kk_slice_count( kk_std_core__sslice sslice, kk_context_t* ctx ) {
  // TODO: optimize this by extending kk_string_count
  const uint8_t* start;
  const uint8_t* end;
  kk_sslice_start_end_borrow(sslice, &start, &end);
  kk_ssize_t count = 0;
  while( start < end && *start != 0 ) {
    const uint8_t* next = kk_utf8_next(start);
    count++;
    start = next;
  }
  kk_std_core__sslice_drop(sslice,ctx);
  return kk_integer_from_ssize_t(count,ctx);
}

kk_string_t kk_slice_to_string( kk_std_core__sslice  sslice, kk_context_t* ctx ) {
  const uint8_t* start;
  const uint8_t* end;
  kk_sslice_start_end_borrow(sslice, &start, &end);
  // is it the full string?
  if (sslice.start == 0 && sslice.len == kk_string_len_borrow(sslice.str)) {
    // TODO: drop sslice and dup sslice.str?
    return sslice.str;
  }
  else {
    // if not, we copy len bytes
    kk_string_t s = kk_string_alloc_dupn_valid_utf8(sslice.len, start, ctx);
    kk_std_core__sslice_drop(sslice,ctx);
    return s;
  }
}

kk_std_core__sslice kk_slice_first( kk_string_t str, kk_context_t* ctx ) {
  kk_ssize_t slen;
  const uint8_t* s = kk_string_buf_borrow(str,&slen);
  const uint8_t* next = (slen > 0 ? kk_utf8_next(s) : s);
  return kk_std_core__new_Sslice(str, 0, (next - s), ctx);
}

kk_std_core__sslice kk_slice_last( kk_string_t str, kk_context_t* ctx ) {
  kk_ssize_t slen;
  const uint8_t* s = kk_string_buf_borrow(str,&slen);
  const uint8_t* end = s + slen;
  const uint8_t* prev = (s==end ? s : kk_utf8_prev(end));
  return kk_std_core__new_Sslice(str, (prev - s), (end - prev), ctx);
}

kk_std_core__sslice kk_slice_between( struct kk_std_core_Sslice slice1, struct kk_std_core_Sslice slice2, kk_context_t* ctx ) {
  const uint8_t* s1 = kk_string_buf_borrow( slice1.str, NULL );
  const uint8_t* s2 = kk_string_buf_borrow( slice2.str, NULL );
  if (s1 != s2) {
    kk_info_message("between: not equal slices: %p vs. %p\n", s1, s2);
    return kk_std_core__new_Sslice(kk_string_empty(), 0, -1, ctx); // invalid slice
  }
  kk_ssize_t start = (slice1.start <= slice2.start ? slice1.start : slice2.start);
  kk_ssize_t len   = (slice1.start <= slice2.start ? slice2.start - slice1.start : slice1.start - slice2.start);
  return kk_std_core__new_Sslice(slice1.str, start, len, ctx);
}

kk_std_core_types__maybe kk_slice_next( struct kk_std_core_Sslice slice, kk_context_t* ctx ) {
  if (slice.len <= 0) {
    kk_std_core__sslice_drop(slice,ctx);
    return kk_std_core_types__new_Nothing(ctx);
  }
  const uint8_t* start;
  const uint8_t* end;
  kk_sslice_start_end_borrow(slice, &start, &end);
  kk_ssize_t clen;
  const kk_char_t c = kk_utf8_read(start,&clen);
  kk_assert_internal(clen > 0 && clen <= slice.len);
  if (clen > slice.len) clen = slice.len;
  // TODO: specialize type to avoid boxing
  kk_std_core__sslice snext = kk_std_core__new_Sslice(slice.str, slice.start + clen, slice.len - clen, ctx);
  kk_std_core_types__tuple2_ res = kk_std_core_types__new_dash__lp__comma__rp_( kk_char_box(c,ctx), kk_std_core__sslice_box(snext,ctx), ctx);
  return kk_std_core_types__new_Just( kk_std_core_types__tuple2__box(res,ctx), ctx );
}

/* Borrow count */
struct kk_std_core_Sslice kk_slice_extend_borrow( struct kk_std_core_Sslice slice, kk_integer_t count, kk_context_t* ctx ) {
  kk_ssize_t cnt = kk_integer_clamp_borrow(count);
  if (cnt==0 || (slice.len <= 0 && cnt<0)) return slice;
  const uint8_t* s0;
  const uint8_t* s1;
  kk_sslice_start_end_borrow(slice,&s0,&s1);
  const uint8_t* t  = s1;
  if (cnt >= 0) {
    do {
      t = kk_utf8_next(t);
      cnt--;
    } while (cnt > 0 && *t != 0);
  }
  else {  // cnt < 0
    const uint8_t* sstart = s0 - slice.start;
    do {
      t = kk_utf8_prev(t);
      cnt++;
    } while (cnt < 0 && t > sstart);
  }
  if (t == s1) return slice;  // length is unchanged
  return kk_std_core__new_Sslice(slice.str, slice.start, (t < s0 ? 0 : (t - s0)), ctx);
}

/* Borrow count */
struct kk_std_core_Sslice kk_slice_advance_borrow( struct kk_std_core_Sslice slice, kk_integer_t count, kk_context_t* ctx ) {
  const kk_ssize_t cnt0 = kk_integer_clamp_borrow(count);
  kk_ssize_t cnt = cnt0;
  if (cnt==0 || (slice.start == 0 && cnt<0)) return slice;
  const uint8_t* sstart;
  const uint8_t* s0;
  const uint8_t* s1;
  const uint8_t* send;
  kk_sslice_start_end_borrowx(slice,&s0,&s1,&sstart,&send);
  // advance the start
  const uint8_t* t0  = s0;
  if (cnt >= 0) {
    do {
      t0 = kk_utf8_next(t0);
      cnt--;
    } while (cnt > 0 && t0 < send);
  }
  else {  // cnt < 0
    do {
      t0 = kk_utf8_prev(t0);
      cnt++;
    } while (cnt < 0 && t0 > sstart);
  }
  if (t0 == s0) return slice;  // start is unchanged
  // "t0" points to the new start, now advance the end by the same amount of codepoints
  const uint8_t* t1 = s1;
  cnt = cnt0;
  if (cnt >= 0) {
    do {
      t1 = kk_utf8_next(t1);
      cnt--;
    } while (cnt > 0 && t1 < send);
  }
  else {  // cnt < 0
    do {
      t1 = kk_utf8_prev(t1);
      cnt++;
    } while (cnt < 0 && t1 > sstart);
  }
  // t1 points to the new end
  kk_assert_internal(t1 >= t0);
  return kk_std_core__new_Sslice(slice.str, (t0 - sstart), (t1 - t0), ctx);
}

/* Borrow iupto */
struct kk_std_core_Sslice kk_slice_common_prefix_borrow( kk_string_t str1, kk_string_t str2, kk_integer_t iupto, kk_context_t* ctx ) {
  const uint8_t* s1 = kk_string_buf_borrow(str1,NULL);
  const uint8_t* s2 = kk_string_buf_borrow(str2,NULL);
  kk_ssize_t upto = kk_integer_clamp_ssize_t_borrow(iupto);
  kk_ssize_t count;
  for(count = 0; count < upto && *s1 != 0 && *s2 != 0; count++, s1++, s2++ ) {
    if (*s1 != *s2) break;
  }
  kk_string_drop(str2,ctx);
  return kk_std_core__new_Sslice(str1, 0, count, ctx);
}


kk_std_core__error kk_error_ok( kk_box_t result, kk_context_t* ctx ) {
  return kk_std_core__new_Ok( result, ctx );
}

kk_std_core__error kk_error_from_errno( int err, kk_context_t* ctx ) {  
  kk_string_t msg;
  #if defined(__GLIBC__) && !defined(WIN32) && !defined(__APPLE__) && !defined(__FreeBSD__)
    // GNU version of strerror_r
    char buf[256];
    char* serr = strerror_r(err, buf, 255); buf[255] = 0;
    msg = kk_string_alloc_from_qutf8( serr, ctx );
  #elif (/* _POSIX_C_SOURCE >= 200112L ||*/ !defined(WIN32) && (_XOPEN_SOURCE >= 600 || defined(__APPLE__) || defined(__FreeBSD__) || defined(__MUSL__)))
    // XSI version of strerror_r
    char buf[256];
    strerror_r(err, buf, 255); buf[255] = 0;
    msg = kk_string_alloc_from_qutf8( buf, ctx );
  #elif defined(_MSC_VER) || (__STDC_VERSION__ >= 201112L || __cplusplus >= 201103L)
    // MSVC, or C/C++ 11
    char buf[256];
    strerror_s(buf, 255, err); buf[255] = 0;
    msg = kk_string_alloc_from_qutf8( buf, ctx );
  #else
    // Old style
    msg = kk_string_alloc_from_qutf8( strerror(err), ctx );
  #endif
  return kk_std_core__new_Error( kk_std_core__new_Exception( msg, kk_std_core__new_ExnSystem(kk_reuse_null, kk_integer_from_int(err,ctx), ctx), ctx), ctx );  
}


kk_unit_t kk_assert_fail( kk_string_t msg, kk_context_t* ctx ) {
  kk_fatal_error(EINVAL, "assertion failed: %s\n", kk_string_cbuf_borrow(msg,NULL));
  kk_string_drop(msg,ctx);
  return kk_Unit;
}
kk_define_string_literal(, kk_std_core__tag_ExnError, 17, "std/core/ExnError")
kk_define_string_literal(, kk_std_core__tag_ExnAssert, 18, "std/core/ExnAssert")
kk_define_string_literal(, kk_std_core__tag_ExnTodo, 16, "std/core/ExnTodo")
kk_define_string_literal(, kk_std_core__tag_ExnRange, 17, "std/core/ExnRange")
kk_define_string_literal(, kk_std_core__tag_ExnPattern, 19, "std/core/ExnPattern")
kk_define_string_literal(, kk_std_core__tag_ExnSystem, 18, "std/core/ExnSystem")
kk_define_string_literal(, kk_std_core__tag_ExnInternal, 20, "std/core/ExnInternal")

kk_std_core__exception kk_std_core__copy(kk_std_core__exception _this, kk_std_core_types__optional message0, kk_std_core_types__optional info0, kk_context_t* _ctx) { /* (exception, message : optional<string>, info : optional<exception-info>) -> exception */ 
  kk_string_t _x20042;
  if (kk_std_core_types__is_Optional(message0)) {
    kk_box_t _box_x18220 = message0._cons.Optional.value;
    kk_string_t _message_1690 = kk_string_unbox(_box_x18220);
    kk_string_dup(_message_1690);
    kk_std_core_types__optional_drop(message0, _ctx);
    _x20042 = _message_1690; /*string*/
    goto _match20043;
  }
  {
    kk_string_t _x = _this.message;
    kk_string_dup(_x);
    _x20042 = _x; /*string*/
  }
  _match20043: ;
  kk_std_core__exception_info _x20045;
  if (kk_std_core_types__is_Optional(info0)) {
    kk_box_t _box_x18221 = info0._cons.Optional.value;
    kk_std_core__exception_info _info_1696 = kk_std_core__exception_info_unbox(_box_x18221, NULL);
    kk_std_core__exception_info_dup(_info_1696);
    kk_std_core_types__optional_drop(info0, _ctx);
    kk_std_core__exception_drop(_this, _ctx);
    _x20045 = _info_1696; /*exception-info*/
    goto _match20046;
  }
  {
    kk_std_core__exception_info _x0 = _this.info;
    kk_std_core__exception_info_dup(_x0);
    kk_std_core__exception_drop(_this, _ctx);
    _x20045 = _x0; /*exception-info*/
  }
  _match20046: ;
  return kk_std_core__new_Exception(_x20042, _x20045, _ctx);
}

kk_std_core__sslice kk_std_core__copy_2(kk_std_core__sslice _this, kk_std_core_types__optional str0, kk_std_core_types__optional start0, kk_std_core_types__optional len0, kk_context_t* _ctx) { /* (sslice, str : optional<string>, start : optional<ssize_t>, len : optional<ssize_t>) -> sslice */ 
  kk_string_t _x20052;
  if (kk_std_core_types__is_Optional(str0)) {
    kk_box_t _box_x18223 = str0._cons.Optional.value;
    kk_string_t _str_1828 = kk_string_unbox(_box_x18223);
    kk_string_dup(_str_1828);
    kk_std_core_types__optional_drop(str0, _ctx);
    _x20052 = _str_1828; /*string*/
    goto _match20053;
  }
  {
    kk_string_t _x = _this.str;
    kk_string_dup(_x);
    _x20052 = _x; /*string*/
  }
  _match20053: ;
  kk_ssize_t _x20055;
  if (kk_std_core_types__is_Optional(start0)) {
    kk_box_t _box_x18224 = start0._cons.Optional.value;
    kk_ssize_t _start_1834 = kk_ssize_unbox(_box_x18224, NULL);
    kk_std_core_types__optional_drop(start0, _ctx);
    _x20055 = _start_1834; /*ssize_t*/
    goto _match20056;
  }
  {
    kk_ssize_t _x0 = _this.start;
    _x20055 = _x0; /*ssize_t*/
  }
  _match20056: ;
  kk_ssize_t _x20058;
  if (kk_std_core_types__is_Optional(len0)) {
    kk_box_t _box_x18225 = len0._cons.Optional.value;
    kk_ssize_t _len_1840 = kk_ssize_unbox(_box_x18225, NULL);
    kk_std_core_types__optional_drop(len0, _ctx);
    kk_std_core__sslice_drop(_this, _ctx);
    _x20058 = _len_1840; /*ssize_t*/
    goto _match20059;
  }
  {
    kk_ssize_t _x1 = _this.len;
    kk_std_core__sslice_drop(_this, _ctx);
    _x20058 = _x1; /*ssize_t*/
  }
  _match20059: ;
  return kk_std_core__new_Sslice(_x20052, _x20055, _x20058, _ctx);
}

kk_std_core__stream kk_std_core__copy_3(kk_std_core__stream _this, kk_std_core_types__optional head0, kk_std_core_types__optional tail0, kk_context_t* _ctx) { /* forall<a> (stream<a>, head : optional<a>, tail : optional<stream<a>>) -> stream<a> */ 
  kk_box_t _x20063;
  if (kk_std_core_types__is_Optional(head0)) {
    kk_box_t _head_1886 = head0._cons.Optional.value;
    _x20063 = _head_1886; /*1903*/
  }
  else {
    struct kk_std_core_Next* _con20064 = kk_std_core__as_Next(_this);
    kk_box_t _x = _con20064->head;
    kk_box_dup(_x);
    _x20063 = _x; /*1903*/
  }
  kk_std_core__stream _x20065;
  if (kk_std_core_types__is_Optional(tail0)) {
    kk_box_t _box_x18226 = tail0._cons.Optional.value;
    kk_std_core__stream _tail_1893 = kk_std_core__stream_unbox(_box_x18226, NULL);
    kk_std_core__stream_dup(_tail_1893);
    kk_std_core_types__optional_drop(tail0, _ctx);
    kk_std_core__stream_drop(_this, _ctx);
    _x20065 = _tail_1893; /*stream<1903>*/
    goto _match20066;
  }
  {
    struct kk_std_core_Next* _con20068 = kk_std_core__as_Next(_this);
    kk_box_t _pat01 = _con20068->head;
    kk_std_core__stream _x0 = _con20068->tail;
    if (kk_likely(kk_std_core__stream_is_unique(_this))) {
      kk_box_drop(_pat01, _ctx);
      kk_std_core__stream_free(_this);
    }
    else {
      kk_std_core__stream_dup(_x0);
      kk_std_core__stream_decref(_this, _ctx);
    }
    _x20065 = _x0; /*stream<1903>*/
  }
  _match20066: ;
  return kk_std_core__new_Next(kk_reuse_null, _x20063, _x20065, _ctx);
}
 
// runtime tag for the `:exn` effect

kk_std_core_hnd__htag kk_std_core__tag_exn;
 
// handler for the `:exn` effect

kk_box_t kk_std_core__handle_exn(int32_t cfc, kk_std_core__hnd_exn hnd, kk_function_t ret, kk_function_t action, kk_context_t* _ctx) { /* forall<a,e,b> (cfc : int32, hnd : .hnd-exn<e,b>, ret : (res : a) -> e b, action : () -> <exn|e> a) -> e b */ 
  kk_std_core_hnd__htag _x20072 = kk_std_core_hnd__htag_dup(kk_std_core__tag_exn); /*std/core/hnd/htag<.hnd-exn>*/
  return kk_std_core_hnd__hhandle(_x20072, cfc, kk_std_core__hnd_exn_box(hnd, _ctx), ret, action, _ctx);
}
 
// Unsafe: transform any type to a `null` type; used internally by the compiler.

kk_std_core__null kk_std_core__null_any(kk_box_t x, kk_context_t* _ctx) { /* forall<a> (x : a) -> null<a> */ 
  return ((x).box == kk_box_null.box ? kk_datatype_from_ptr(NULL) : kk_datatype_unbox(x));
}

kk_ref_t kk_std_core_redirect;
 
// Print a string to the console, including a final newline character.

kk_unit_t kk_std_core_xprintsln(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> console () */ 
  kk_println(s,kk_context()); return kk_Unit;
}

kk_integer_t kk_std_core_string_compare(kk_string_t x, kk_string_t y, kk_context_t* _ctx) { /* (x : string, y : string) -> int */ 
  return kk_string_cmp_int(x,y,kk_context());
}
 
// Convert an integer to an `:int32`. The number is _clamped_ to the maximal or minimum `:int32`
// value if it is outside the range of an `:int32`.

int32_t kk_std_core_int32(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> int32 */ 
  return kk_integer_clamp32(i,kk_context());
}
 
// Convert an integer to an `:ssize_t`. The number is _clamped_ to the maximal or minimum `:ssize_t`
// value if it is outside the range of an `:ssize_t`.

kk_ssize_t kk_std_core_ssize__t(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> ssize_t */ 
  return kk_integer_clamp_ssize_t(i,kk_context());
}
 
// Convert a character to a string

kk_string_t kk_std_core_string(kk_char_t c, kk_context_t* _ctx) { /* (c : char) -> string */ 
  return kk_string_from_char(c,kk_context());
}
 
// Convert a vector of characters to a string.

kk_string_t kk_std_core_string_1(kk_vector_t _arg1, kk_context_t* _ctx) { /* (vector<char>) -> string */ 
  return kk_string_from_chars(_arg1,kk_context());
}
 
// Convert a list of characters to a string

kk_string_t kk_std_core_string_2(kk_std_core__list cs, kk_context_t* _ctx) { /* (cs : list<char>) -> total string */ 
  return kk_string_from_list(cs,kk_context());
}
 
// O(n). Copy the `slice` argument into a fresh string.
// Takes O(1) time if the slice covers the entire string.

kk_string_t kk_std_core_string_3(kk_std_core__sslice slice0, kk_context_t* _ctx) { /* (slice : sslice) -> string */ 
  return kk_slice_to_string(slice0,kk_context());
}
 
// Convert a vector to a list with an optional tail.

kk_std_core__list kk_std_core_vlist(kk_vector_t v, kk_std_core_types__optional tail0, kk_context_t* _ctx) { /* forall<a> (v : vector<a>, tail : optional<list<a>>) -> list<a> */ 
  kk_std_core__list _x20075;
  if (kk_std_core_types__is_Optional(tail0)) {
    kk_box_t _box_x18240 = tail0._cons.Optional.value;
    kk_std_core__list _tail_2097 = kk_std_core__list_unbox(_box_x18240, NULL);
    kk_std_core__list_dup(_tail_2097);
    kk_std_core_types__optional_drop(tail0, _ctx);
    _x20075 = _tail_2097; /*list<2107>*/
    goto _match20076;
  }
  {
    _x20075 = kk_std_core__new_Nil(_ctx); /*list<2107>*/
  }
  _match20076: ;
  return kk_vector_to_list(v,_x20075,kk_context());
}

kk_string_t kk_std_core_int_show_hex(kk_integer_t i, bool use_capitals, kk_context_t* _ctx) { /* (i : int, use-capitals : bool) -> string */ 
  return kk_integer_to_hex_string(i,use_capitals,kk_context());
}

kk_string_t kk_std_core_repeatz(kk_string_t s, kk_ssize_t n, kk_context_t* _ctx) { /* (s : string, n : ssize_t) -> string */ 
  return kk_string_repeat(s,n,kk_context());
}

kk_string_t kk_std_core_show_expx(double d, int32_t prec, kk_context_t* _ctx) { /* (d : double, prec : int32) -> string */ 
  return kk_double_show_exp(d,prec,kk_context());
}

kk_string_t kk_std_core_show_fixedx(double d, int32_t prec, kk_context_t* _ctx) { /* (d : double, prec : int32) -> string */ 
  return kk_double_show_fixed(d,prec,kk_context());
}
 
// Print a string to the console

kk_unit_t kk_std_core_xprints(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> console () */ 
  kk_print(s,kk_context()); return kk_Unit;
}
 
// Raise an integer `i` to the power of `exp`.

kk_integer_t kk_std_core_pow(kk_integer_t i, kk_integer_t exp, kk_context_t* _ctx) { /* (i : int, exp : int) -> int */ 
  return kk_integer_pow(i,exp,kk_context());
}
 
// O(`count`). Advance the start position of a string slice by `count` characters
// up to the end of the string.
// A negative `count` advances the start position backwards upto the first position
// in a string.
// Maintains the character count of the original slice upto the end of the string.
// For example:
//
// * `"abc".first.advance(1).string == "b"`,
// * `"abc".first.advance(3).string == ""`,
// * `"abc".last.advance(-1).string == "b"`.
//

kk_std_core__sslice kk_std_core_advance(kk_std_core__sslice slice0, kk_integer_t count, kk_context_t* _ctx) { /* (slice : sslice, count : int) -> sslice */ 
  return kk_slice_advance_borrow(slice0,count,kk_context());
}

kk_unit_t kk_std_core_unsafe_assert_fail(kk_string_t msg, kk_context_t* _ctx) { /* (msg : string) -> () */ 
  kk_assert_fail(msg,kk_context()); return kk_Unit;
}
 
// clamp an `:int` to fit in a `:byte`.

uint8_t kk_std_core_byte(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> byte */ 
  return kk_integer_clamp_byte(i,kk_context());
}
 
// O(`count`). Extend a string slice by `count` characters up to the end of the string.
// A negative `count` shrinks the slice up to the empty slice.
// For example:
//
// * `"abc".first.extend(1).string == "ab"`
// * `"abc".last.extend(-1).string == ""`
//

kk_std_core__sslice kk_std_core_extend(kk_std_core__sslice slice0, kk_integer_t count, kk_context_t* _ctx) { /* (slice : sslice, count : int) -> sslice */ 
  return kk_slice_extend_borrow(slice0,count,kk_context());
}

kk_std_core__sslice kk_std_core_first1(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> sslice */ 
  return kk_slice_first(s,kk_context());
}
 
// Convert a string to upper-case

kk_string_t kk_std_core_to_upper(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> string */ 
  return kk_string_to_upper(s,kk_context());
}

kk_integer_t kk_std_core_cdiv_exp10(kk_integer_t i, kk_integer_t n, kk_context_t* _ctx) { /* (i : int, n : int) -> int */ 
  return kk_integer_cdiv_pow10(i,n,kk_context());
}

kk_integer_t kk_std_core_mul_exp10(kk_integer_t i, kk_integer_t n, kk_context_t* _ctx) { /* (i : int, n : int) -> int */ 
  return kk_integer_mul_pow10(i,n,kk_context());
}
 
// Return the common prefix of two strings (upto `upto` characters (default is minimum length of the two strings))

kk_std_core__sslice kk_std_core_common_prefix(kk_string_t s, kk_string_t t, kk_std_core_types__optional upto, kk_context_t* _ctx) { /* (s : string, t : string, upto : optional<int>) -> sslice */ 
  kk_integer_t _brw_19924;
  if (kk_std_core_types__is_Optional(upto)) {
    kk_box_t _box_x18241 = upto._cons.Optional.value;
    kk_integer_t _upto_2201 = kk_integer_unbox(_box_x18241);
    _brw_19924 = kk_integer_dup(_upto_2201); /*int*/
    goto _match20078;
  }
  {
    _brw_19924 = kk_integer_from_small(-1); /*int*/
  }
  _match20078: ;
  kk_std_core__sslice _brw_19925 = kk_slice_common_prefix_borrow(s,t,_brw_19924,kk_context()); /*sslice*/;
  kk_integer_drop(_brw_19924, _ctx);
  return _brw_19925;
}
 
// lifted

kk_std_core__list kk_std_core__ctail_lift17262_concat(kk_std_core__list ys, kk_std_core__list zss, kk_std_core_types__ctail _acc, kk_context_t* _ctx) { /* forall<a> (ys : list<a>, zss : list<list<a>>, ctail<list<a>>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20080 = kk_std_core__as_Cons(ys);
    kk_box_t y = _con20080->head;
    kk_std_core__list yy = _con20080->tail;
    kk_reuse_t _ru_19935 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      _ru_19935 = (kk_std_core__list_reuse(ys));
    }
    else {
      kk_box_dup(y);
      kk_std_core__list_dup(yy);
      kk_std_core__list_decref(ys, _ctx);
    }
    kk_std_core__list _ctail_17313 = kk_std_core__list_hole(); /*list<2249>*/;
    kk_std_core__list _ctail_17314 = kk_std_core__new_Cons(_ru_19935, y, _ctail_17313, _ctx); /*list<2249>*/;
    { // tailcall
      kk_std_core_types__ctail _x20081;
      kk_box_t* _b_18253_18247 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17314)->tail)); /*cfield<list<2249>>*/;
      _x20081 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17314, _ctx)),_b_18253_18247); /*ctail<0>*/
      ys = yy;
      _acc = _x20081;
      goto kk__tailcall;
    }
  }
  if (kk_std_core__is_Cons(zss)) {
    struct kk_std_core_Cons* _con20082 = kk_std_core__as_Cons(zss);
    kk_box_t _box_x18248 = _con20082->head;
    kk_std_core__list zzs = _con20082->tail;
    kk_std_core__list zs = kk_std_core__list_unbox(_box_x18248, NULL);
    if (kk_likely(kk_std_core__list_is_unique(zss))) {
      kk_std_core__list_free(zss);
    }
    else {
      kk_std_core__list_dup(zs);
      kk_std_core__list_dup(zzs);
      kk_std_core__list_decref(zss, _ctx);
    }
    { // tailcall
      ys = zs;
      zss = zzs;
      goto kk__tailcall;
    }
  }
  {
    kk_box_t _x20084 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20084, _ctx);
  }
}
 
// lifted

kk_std_core__list kk_std_core__lift17262_concat(kk_std_core__list ys0, kk_std_core__list zss0, kk_context_t* _ctx) { /* forall<a> (ys : list<a>, zss : list<list<a>>) -> list<a> */ 
  kk_std_core_types__ctail _x20085 = kk_ctail_nil(); /*ctail<0>*/
  return kk_std_core__ctail_lift17262_concat(ys0, zss0, _x20085, _ctx);
}
extern kk_box_t kk_std_core_const_fun20086_1(kk_function_t _fself, kk_box_t ___wildcard__122__7, kk_context_t* _ctx) {
  struct kk_std_core_const_fun20086__t_1* _self = kk_function_as(struct kk_std_core_const_fun20086__t_1*, _fself);
  kk_box_t default0 = _self->default0; /* 2277 */
  kk_drop_match(_self, {kk_box_dup(default0);}, {}, _ctx)
  kk_box_drop(___wildcard__122__7, _ctx);
  return default0;
}
 
// If the slice is not empty,
// return the first character, and a new slice that is advanced by 1.

kk_std_core_types__maybe kk_std_core_next(kk_std_core__sslice slice0, kk_context_t* _ctx) { /* (slice : sslice) -> maybe<(char, sslice)> */ 
  return kk_slice_next(slice0,kk_context());
}
 
// Return the number of decimal digits of `i`. Return `0` when `i==0`.

kk_integer_t kk_std_core_count_digits(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> int */ 
  return kk_integer_count_digits(i,kk_context());
}
 
// Transform an `:error` type to an `:either` value.

kk_std_core_types__either kk_std_core_either(kk_std_core__error t, kk_context_t* _ctx) { /* forall<a> (t : error<a>) -> either<exception,a> */ 
  if (kk_std_core__is_Error(t)) {
    kk_std_core__exception exn0 = t._cons.Error.exception;
    kk_std_core__exception_dup(exn0);
    kk_std_core__error_drop(t, _ctx);
    return kk_std_core_types__new_Left(kk_std_core__exception_box(exn0, _ctx), _ctx);
  }
  {
    kk_box_t x = t._cons.Ok.result;
    return kk_std_core_types__new_Right(x, _ctx);
  }
}

bool kk_std_core_xends_with(kk_string_t s, kk_string_t post, kk_context_t* _ctx) { /* (s : string, post : string) -> bool */ 
  return kk_string_ends_with(s,post,kk_context());
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17691_op(kk_std_core_types__ctail _acc, kk_function_t f, kk_std_core__list zz, kk_std_core__list ys1_17289, kk_context_t* _ctx) { /* forall<a,b,e> (ctail<list<b>>, f : (a) -> e list<b>, zz : list<a>, ys1.17289 : list<b>) -> e list<b> */ 
  return kk_std_core__ctail_lift17263_flatmap(f, ys1_17289, zz, _acc, _ctx);
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17692_op(kk_function_t _accm, kk_function_t f0, kk_std_core__list zz0, kk_std_core__list ys1_172890, kk_context_t* _ctx) { /* forall<a,b,e> ((list<b>) -> list<b>, f : (a) -> e list<b>, zz : list<a>, ys1.17289 : list<b>) -> e list<b> */ 
  return kk_std_core__ctailm_lift17263_flatmap(f0, ys1_172890, zz0, _accm, _ctx);
}
 
// lifted


// lift anonymous function
struct kk_std_core__ctail_lift17263_flatmap_fun20092__t {
  struct kk_function_s _base;
  kk_function_t f1;
  kk_std_core__list zz1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_lift17263_flatmap_fun20092(kk_function_t _fself, kk_box_t _b_18268, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_lift17263_flatmap_fun20092(kk_function_t f1, kk_std_core__list zz1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_lift17263_flatmap_fun20092__t* _self = kk_function_alloc_as(struct kk_std_core__ctail_lift17263_flatmap_fun20092__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_lift17263_flatmap_fun20092, kk_context());
  _self->f1 = f1;
  _self->zz1 = zz1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_lift17263_flatmap_fun20092(kk_function_t _fself, kk_box_t _b_18268, kk_context_t* _ctx) {
  struct kk_std_core__ctail_lift17263_flatmap_fun20092__t* _self = kk_function_as(struct kk_std_core__ctail_lift17263_flatmap_fun20092__t*, _fself);
  kk_function_t f1 = _self->f1; /* (2426) -> 2428 list<2427> */
  kk_std_core__list zz1 = _self->zz1; /* list<2426> */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<2427>> */
  kk_drop_match(_self, {kk_function_dup(f1);kk_std_core__list_dup(zz1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x20093;
  kk_std_core__list _x20094 = kk_std_core__list_unbox(_b_18268, _ctx); /*list<2427>*/
  _x20093 = kk_std_core__mlift17691_op(_acc0, f1, zz1, _x20094, _ctx); /*list<2427>*/
  return kk_std_core__list_box(_x20093, _ctx);
}

kk_std_core__list kk_std_core__ctail_lift17263_flatmap(kk_function_t f1, kk_std_core__list ys, kk_std_core__list zs, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,b,e> (f : (a) -> e list<b>, ys : list<b>, zs : list<a>, ctail<list<b>>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20087 = kk_std_core__as_Cons(ys);
    kk_box_t y = _con20087->head;
    kk_std_core__list yy = _con20087->tail;
    kk_reuse_t _ru_19937 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      _ru_19937 = (kk_std_core__list_reuse(ys));
    }
    else {
      kk_box_dup(y);
      kk_std_core__list_dup(yy);
      kk_std_core__list_decref(ys, _ctx);
    }
    kk_std_core__list _ctail_17315 = kk_std_core__list_hole(); /*list<2427>*/;
    kk_std_core__list _ctail_17316 = kk_std_core__new_Cons(_ru_19937, y, _ctail_17315, _ctx); /*list<2427>*/;
    { // tailcall
      kk_std_core_types__ctail _x20088;
      kk_box_t* _b_18273_18266 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17316)->tail)); /*cfield<list<2427>>*/;
      _x20088 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_17316, _ctx)),_b_18273_18266); /*ctail<0>*/
      ys = yy;
      _acc0 = _x20088;
      goto kk__tailcall;
    }
  }
  if (kk_std_core__is_Cons(zs)) {
    struct kk_std_core_Cons* _con20089 = kk_std_core__as_Cons(zs);
    kk_box_t z = _con20089->head;
    kk_std_core__list zz1 = _con20089->tail;
    if (kk_likely(kk_std_core__list_is_unique(zs))) {
      kk_std_core__list_free(zs);
    }
    else {
      kk_box_dup(z);
      kk_std_core__list_dup(zz1);
      kk_std_core__list_decref(zs, _ctx);
    }
    kk_std_core__list x_17767;
    kk_function_t _x20090 = kk_function_dup(f1); /*(2426) -> 2428 list<2427>*/
    x_17767 = kk_function_call(kk_std_core__list, (kk_function_t, kk_box_t, kk_context_t*), _x20090, (_x20090, z, _ctx)); /*list<2427>*/
    if (kk_yielding(kk_context())) {
      kk_std_core__list_drop(x_17767, _ctx);
      kk_box_t _x20091 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_lift17263_flatmap_fun20092(f1, zz1, _acc0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20091, _ctx);
    }
    { // tailcall
      ys = x_17767;
      zs = zz1;
      goto kk__tailcall;
    }
  }
  {
    kk_function_drop(f1, _ctx);
    kk_box_t _x20095 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20095, _ctx);
  }
}
 
// lifted


// lift anonymous function
struct kk_std_core__ctailm_lift17263_flatmap_fun20098__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t y0;
};
static kk_std_core__list kk_std_core__ctailm_lift17263_flatmap_fun20098(kk_function_t _fself, kk_std_core__list _ctail_17318, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_lift17263_flatmap_fun20098(kk_function_t _accm0, kk_box_t y0, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_lift17263_flatmap_fun20098__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_lift17263_flatmap_fun20098__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_lift17263_flatmap_fun20098, kk_context());
  _self->_accm0 = _accm0;
  _self->y0 = y0;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_lift17263_flatmap_fun20098(kk_function_t _fself, kk_std_core__list _ctail_17318, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_lift17263_flatmap_fun20098__t* _self = kk_function_as(struct kk_std_core__ctailm_lift17263_flatmap_fun20098__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<2427>) -> list<2427> */
  kk_box_t y0 = _self->y0; /* 2427 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(y0);}, {}, _ctx)
  kk_std_core__list _x20099 = kk_std_core__new_Cons(kk_reuse_null, y0, _ctail_17318, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x20099, _ctx));
}


// lift anonymous function
struct kk_std_core__ctailm_lift17263_flatmap_fun20103__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t f2;
  kk_std_core__list zz2;
};
static kk_box_t kk_std_core__ctailm_lift17263_flatmap_fun20103(kk_function_t _fself, kk_box_t _b_18282, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_lift17263_flatmap_fun20103(kk_function_t _accm0, kk_function_t f2, kk_std_core__list zz2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_lift17263_flatmap_fun20103__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_lift17263_flatmap_fun20103__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_lift17263_flatmap_fun20103, kk_context());
  _self->_accm0 = _accm0;
  _self->f2 = f2;
  _self->zz2 = zz2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_lift17263_flatmap_fun20103(kk_function_t _fself, kk_box_t _b_18282, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_lift17263_flatmap_fun20103__t* _self = kk_function_as(struct kk_std_core__ctailm_lift17263_flatmap_fun20103__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<2427>) -> list<2427> */
  kk_function_t f2 = _self->f2; /* (2426) -> 2428 list<2427> */
  kk_std_core__list zz2 = _self->zz2; /* list<2426> */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(f2);kk_std_core__list_dup(zz2);}, {}, _ctx)
  kk_std_core__list _x20104;
  kk_std_core__list _x20105 = kk_std_core__list_unbox(_b_18282, _ctx); /*list<2427>*/
  _x20104 = kk_std_core__mlift17692_op(_accm0, f2, zz2, _x20105, _ctx); /*list<2427>*/
  return kk_std_core__list_box(_x20104, _ctx);
}

kk_std_core__list kk_std_core__ctailm_lift17263_flatmap(kk_function_t f2, kk_std_core__list ys0, kk_std_core__list zs0, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,b,e> (f : (a) -> e list<b>, ys : list<b>, zs : list<a>, (list<b>) -> list<b>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys0)) {
    struct kk_std_core_Cons* _con20096 = kk_std_core__as_Cons(ys0);
    kk_box_t y0 = _con20096->head;
    kk_std_core__list yy0 = _con20096->tail;
    if (kk_likely(kk_std_core__list_is_unique(ys0))) {
      kk_std_core__list_free(ys0);
    }
    else {
      kk_box_dup(y0);
      kk_std_core__list_dup(yy0);
      kk_std_core__list_decref(ys0, _ctx);
    }
    { // tailcall
      kk_function_t _x20097 = kk_std_core__new_ctailm_lift17263_flatmap_fun20098(_accm0, y0, _ctx); /*(list<2427>) -> list<2427>*/
      ys0 = yy0;
      _accm0 = _x20097;
      goto kk__tailcall;
    }
  }
  if (kk_std_core__is_Cons(zs0)) {
    struct kk_std_core_Cons* _con20100 = kk_std_core__as_Cons(zs0);
    kk_box_t z0 = _con20100->head;
    kk_std_core__list zz2 = _con20100->tail;
    if (kk_likely(kk_std_core__list_is_unique(zs0))) {
      kk_std_core__list_free(zs0);
    }
    else {
      kk_box_dup(z0);
      kk_std_core__list_dup(zz2);
      kk_std_core__list_decref(zs0, _ctx);
    }
    kk_std_core__list x0_17770;
    kk_function_t _x20101 = kk_function_dup(f2); /*(2426) -> 2428 list<2427>*/
    x0_17770 = kk_function_call(kk_std_core__list, (kk_function_t, kk_box_t, kk_context_t*), _x20101, (_x20101, z0, _ctx)); /*list<2427>*/
    if (kk_yielding(kk_context())) {
      kk_std_core__list_drop(x0_17770, _ctx);
      kk_box_t _x20102 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_lift17263_flatmap_fun20103(_accm0, f2, zz2, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20102, _ctx);
    }
    { // tailcall
      ys0 = x0_17770;
      zs0 = zz2;
      goto kk__tailcall;
    }
  }
  {
    kk_function_drop(f2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// lifted


// lift anonymous function
struct kk_std_core__lift17263_flatmap_fun20107__t {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core__lift17263_flatmap_fun20107(kk_function_t _fself, kk_std_core__list _ctail_17317, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17263_flatmap_fun20107(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core__lift17263_flatmap_fun20107, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core__lift17263_flatmap_fun20107(kk_function_t _fself, kk_std_core__list _ctail_17317, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17317;
}

kk_std_core__list kk_std_core__lift17263_flatmap(kk_function_t f3, kk_std_core__list ys1, kk_std_core__list zs1, kk_context_t* _ctx) { /* forall<a,b,e> (f : (a) -> e list<b>, ys : list<b>, zs : list<a>) -> e list<b> */ 
  bool _match_19921 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19921) {
    kk_std_core_types__ctail _x20106 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_lift17263_flatmap(f3, ys1, zs1, _x20106, _ctx);
  }
  {
    return kk_std_core__ctailm_lift17263_flatmap(f3, ys1, zs1, kk_std_core__new_lift17263_flatmap_fun20107(_ctx), _ctx);
  }
}
 
// lifted

kk_std_core__list kk_std_core__lift17264_reverse_append(kk_std_core__list acc, kk_std_core__list ys, kk_context_t* _ctx) { /* forall<a> (acc : list<a>, ys : list<a>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20108 = kk_std_core__as_Cons(ys);
    kk_box_t x = _con20108->head;
    kk_std_core__list xx = _con20108->tail;
    kk_reuse_t _ru_19941 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      _ru_19941 = (kk_std_core__list_reuse(ys));
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(ys, _ctx);
    }
    { // tailcall
      kk_std_core__list _x20109 = kk_std_core__new_Cons(_ru_19941, x, acc, _ctx); /*list<61>*/
      acc = _x20109;
      ys = xx;
      goto kk__tailcall;
    }
  }
  {
    return acc;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17694_force_fun20113__t {
  struct kk_function_s _base;
  kk_ref_t r;
};
static kk_box_t kk_std_core__mlift17694_force_fun20113(kk_function_t _fself, kk_box_t x0, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17694_force_fun20113(kk_ref_t r, kk_context_t* _ctx) {
  struct kk_std_core__mlift17694_force_fun20113__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17694_force_fun20113__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17694_force_fun20113, kk_context());
  _self->r = r;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17694_force_fun20113(kk_function_t _fself, kk_box_t x0, kk_context_t* _ctx) {
  struct kk_std_core__mlift17694_force_fun20113__t* _self = kk_function_as(struct kk_std_core__mlift17694_force_fun20113__t*, _fself);
  kk_ref_t r = _self->r; /* ref<global,either<() -> 2563 2562,2562>> */
  kk_drop_match(_self, {kk_ref_dup(r);}, {}, _ctx)
  kk_unit_t __ = kk_Unit;
  kk_unit_t _brw_19919 = kk_Unit;
  kk_box_t _x20114;
  kk_std_core_types__either _x20115;
  kk_box_t _x20116 = kk_box_dup(x0); /*2562*/
  _x20115 = kk_std_core_types__new_Right(_x20116, _ctx); /*either<72,73>*/
  _x20114 = kk_std_core_types__either_box(_x20115, _ctx); /*170*/
  kk_ref_set_borrow(r,_x20114,kk_context());
  kk_ref_drop(r, _ctx);
  _brw_19919;
  return x0;
}

kk_box_t kk_std_core__mlift17694_force(kk_ref_t r, kk_function_t _y_17405, kk_context_t* _ctx) { /* forall<a,e> (r : ref<global,either<() -> e a,a>>, () -> <st<global>,div|e> a) -> <alloc<global>,div,read<global>,write<global>|e> a */ 
  kk_box_t x_17773 = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), _y_17405, (_y_17405, _ctx)); /*2562*/;
  kk_function_t next0_17774 = kk_std_core__new_mlift17694_force_fun20113(r, _ctx); /*(2562) -> <st<global>,div|2563> 2562*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(x_17773, _ctx);
    return kk_std_core_hnd_yield_extend(next0_17774, _ctx);
  }
  {
    return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), next0_17774, (next0_17774, x_17773, _ctx));
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17695_force_fun20117__t {
  struct kk_function_s _base;
  kk_ref_t r;
};
static kk_box_t kk_std_core__mlift17695_force_fun20117(kk_function_t _fself, kk_box_t _b_18297, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17695_force_fun20117(kk_ref_t r, kk_context_t* _ctx) {
  struct kk_std_core__mlift17695_force_fun20117__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17695_force_fun20117__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17695_force_fun20117, kk_context());
  _self->r = r;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17695_force_fun20117(kk_function_t _fself, kk_box_t _b_18297, kk_context_t* _ctx) {
  struct kk_std_core__mlift17695_force_fun20117__t* _self = kk_function_as(struct kk_std_core__mlift17695_force_fun20117__t*, _fself);
  kk_ref_t r = _self->r; /* ref<global,either<() -> 2563 2562,2562>> */
  kk_drop_match(_self, {kk_ref_dup(r);}, {}, _ctx)
  kk_function_t _x20118 = kk_function_unbox(_b_18297); /*() -> <st<global>,div|2563> 18298*/
  return kk_std_core__mlift17694_force(r, _x20118, _ctx);
}

kk_box_t kk_std_core__mlift17695_force(kk_ref_t r, kk_std_core_types__either _y_17403, kk_context_t* _ctx) { /* forall<a,e> (r : ref<global,either<() -> e a,a>>, either<() -> e a,a>) -> <read<global>,div,alloc<global>,write<global>|e> a */ 
  if (kk_std_core_types__is_Right(_y_17403)) {
    kk_box_t x = _y_17403._cons.Right.right;
    kk_ref_drop(r, _ctx);
    return x;
  }
  {
    kk_box_t _fun_unbox_x18294 = _y_17403._cons.Left.left;
    if (kk_yielding(kk_context())) {
      kk_box_drop(_fun_unbox_x18294, _ctx);
      return kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17695_force_fun20117(r, _ctx), _ctx);
    }
    {
      kk_function_t _x20119 = kk_function_unbox(_fun_unbox_x18294); /*() -> 2563 18295*/
      return kk_std_core__mlift17694_force(r, _x20119, _ctx);
    }
  }
}
 
// Force a delayed value; the value is computed only on the first
// call to `force` and cached afterwards.


// lift anonymous function
struct kk_std_core_force_fun20122__t {
  struct kk_function_s _base;
  kk_std_core__delayed delayed;
};
static kk_box_t kk_std_core_force_fun20122(kk_function_t _fself, kk_box_t _b_18304, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_force_fun20122(kk_std_core__delayed delayed, kk_context_t* _ctx) {
  struct kk_std_core_force_fun20122__t* _self = kk_function_alloc_as(struct kk_std_core_force_fun20122__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_force_fun20122, kk_context());
  _self->delayed = delayed;
  return &_self->_base;
}

static kk_box_t kk_std_core_force_fun20122(kk_function_t _fself, kk_box_t _b_18304, kk_context_t* _ctx) {
  struct kk_std_core_force_fun20122__t* _self = kk_function_as(struct kk_std_core_force_fun20122__t*, _fself);
  kk_std_core__delayed delayed = _self->delayed; /* delayed<2563,2562> */
  kk_drop_match(_self, {kk_std_core__delayed_dup(delayed);}, {}, _ctx)
  kk_std_core_types__either _y_18306_17403 = kk_std_core_types__either_unbox(_b_18304, _ctx); /*either<() -> 2563 2562,2562>*/;
  kk_ref_t _x20123;
  {
    kk_ref_t _x0 = delayed.dref;
    _x20123 = _x0; /*ref<global,either<() -> 2563 2562,2562>>*/
  }
  return kk_std_core__mlift17695_force(_x20123, _y_18306_17403, _ctx);
}

kk_box_t kk_std_core_force(kk_std_core__delayed delayed, kk_context_t* _ctx) { /* forall<a,e> (delayed : delayed<e,a>) -> e a */ 
  kk_std_core_types__either x_17782;
  kk_box_t _x20120;
  kk_ref_t _x20121;
  {
    kk_ref_t _x = delayed.dref;
    kk_ref_dup(_x);
    _x20121 = _x; /*ref<global,either<() -> 2563 2562,2562>>*/
  }
  _x20120 = kk_ref_get(_x20121,kk_context()); /*184*/
  x_17782 = kk_std_core_types__either_unbox(_x20120, _ctx); /*either<() -> 2563 2562,2562>*/
  if (kk_yielding(kk_context())) {
    kk_std_core_types__either_drop(x_17782, _ctx);
    return kk_std_core_hnd_yield_extend(kk_std_core_new_force_fun20122(delayed, _ctx), _ctx);
  }
  {
    kk_std_core_types__either _y_18307_17403 = x_17782; /*either<() -> 2563 2562,2562>*/;
    kk_ref_t _x20124;
    {
      kk_ref_t _x0 = delayed.dref;
      _x20124 = _x0; /*ref<global,either<() -> 2563 2562,2562>>*/
    }
    return kk_std_core__mlift17695_force(_x20124, _y_18307_17403, _ctx);
  }
}
 
// Generic show: shows the internal representation of an object as a string
// Note: this breaks parametricity so it should not be public

kk_string_t kk_std_core_gshow(kk_box_t _arg1, kk_context_t* _ctx) { /* forall<a> (a) -> string */ 
  return kk_show_any(_arg1,kk_context());
}
 
// Return the host environment: `dotnet`, `browser`, `webworker`, `node`, or `libc`.

kk_string_t kk_std_core_host(kk_context_t* _ctx) { /* () -> ndet string */ 
  return kk_get_host(kk_context());
}
 
// clamp an `:int` to fit in an `:int64_t`.

int64_t kk_std_core_int64(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> int64 */ 
  return kk_integer_clamp64(i,kk_context());
}
 
// lifted

kk_std_core__list kk_std_core__ctail_lift17265_intersperse(kk_std_core__list ys, kk_box_t s, kk_std_core_types__ctail _acc, kk_context_t* _ctx) { /* forall<a> (ys : list<a>, s : a, ctail<list<a>>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20125 = kk_std_core__as_Cons(ys);
    kk_box_t y = _con20125->head;
    kk_std_core__list yy = _con20125->tail;
    kk_reuse_t _ru_19942 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      _ru_19942 = (kk_std_core__list_reuse(ys));
    }
    else {
      kk_box_dup(y);
      kk_std_core__list_dup(yy);
      kk_std_core__list_decref(ys, _ctx);
    }
    kk_std_core__list _ctail_17319;
    kk_std_core__list _x20126 = kk_std_core__list_hole(); /*list<2594>*/
    _ctail_17319 = kk_std_core__new_Cons(_ru_19942, y, _x20126, _ctx); /*list<2594>*/
    { // tailcall
      kk_box_t _x20127 = kk_box_dup(s); /*2594*/
      kk_std_core_types__ctail _x20128;
      kk_box_t* _b_18318_18313 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17319)->tail)); /*cfield<list<2594>>*/;
      kk_box_t _x20129;
      kk_std_core__list _x20130 = kk_std_core__new_Cons(kk_reuse_null, s, _ctail_17319, _ctx); /*list<61>*/
      _x20129 = kk_std_core__list_box(_x20130, _ctx); /*0*/
      _x20128 = kk_ctail_link(_acc,_x20129,_b_18318_18313); /*ctail<0>*/
      ys = yy;
      s = _x20127;
      _acc = _x20128;
      goto kk__tailcall;
    }
  }
  {
    kk_box_drop(s, _ctx);
    kk_box_t _x20131 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20131, _ctx);
  }
}
 
// lifted

kk_std_core__list kk_std_core__lift17265_intersperse(kk_std_core__list ys0, kk_box_t s0, kk_context_t* _ctx) { /* forall<a> (ys : list<a>, s : a) -> list<a> */ 
  kk_std_core_types__ctail _x20132 = kk_ctail_nil(); /*ctail<0>*/
  return kk_std_core__ctail_lift17265_intersperse(ys0, s0, _x20132, _ctx);
}
 
// Insert a separator `sep`  between all elements of a list `xs` .

kk_std_core__list kk_std_core_intersperse(kk_std_core__list xs, kk_box_t sep, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, sep : a) -> list<a> */ 
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20133 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20133->head;
    kk_std_core__list xx = _con20133->tail;
    kk_reuse_t _ru_19943 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      _ru_19943 = (kk_std_core__list_reuse(xs));
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_std_core__list _x20134 = kk_std_core__lift17265_intersperse(xx, sep, _ctx); /*list<2594>*/
    return kk_std_core__new_Cons(_ru_19943, x, _x20134, _ctx);
  }
  {
    kk_box_drop(sep, _ctx);
    return kk_std_core__new_Nil(_ctx);
  }
}
 
// clamp an `:int` to fit in an `:intptr_t`.

intptr_t kk_std_core_intptr__t(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> intptr_t */ 
  return kk_integer_clamp_intptr_t(i,kk_context());
}
 
// Return the number of ending `0` digits of `i`. Return `0` when `i==0`.

kk_integer_t kk_std_core_is_exp10(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> int */ 
  return kk_integer_ctz(i,kk_context());
}

kk_std_core__sslice kk_std_core_last1(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> sslice */ 
  return kk_slice_last(s,kk_context());
}
 
// Used by the compiler to wrap main console applications

kk_box_t kk_std_core_main_console(kk_function_t main, kk_context_t* _ctx) { /* forall<a,e> (main : () -> e a) -> e a */ 
  return kk_main_console(main,kk_context());
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17696_op(kk_std_core_types__ctail _acc, kk_function_t f, kk_std_core__list yy, kk_box_t _ctail_17321, kk_context_t* _ctx) { /* forall<a,b,e> (ctail<list<b>>, f : (value : a, rest : list<a>) -> e b, yy : list<a>, b) -> e list<b> */ 
  kk_std_core__list _ctail_17322 = kk_std_core__list_hole(); /*list<2735>*/;
  kk_std_core__list _ctail_17323 = kk_std_core__new_Cons(kk_reuse_null, _ctail_17321, _ctail_17322, _ctx); /*list<2735>*/;
  kk_std_core_types__ctail _x20135;
  kk_box_t* _b_18332_18329 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17323)->tail)); /*cfield<list<2735>>*/;
  _x20135 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17323, _ctx)),_b_18332_18329); /*ctail<0>*/
  return kk_std_core__ctail_lift17266_map_peek(f, yy, _x20135, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17697_op_fun20136__t {
  struct kk_function_s _base;
  kk_function_t _accm;
  kk_box_t _ctail_17326;
};
static kk_std_core__list kk_std_core__mlift17697_op_fun20136(kk_function_t _fself, kk_std_core__list _ctail_17325, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17697_op_fun20136(kk_function_t _accm, kk_box_t _ctail_17326, kk_context_t* _ctx) {
  struct kk_std_core__mlift17697_op_fun20136__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17697_op_fun20136__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17697_op_fun20136, kk_context());
  _self->_accm = _accm;
  _self->_ctail_17326 = _ctail_17326;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__mlift17697_op_fun20136(kk_function_t _fself, kk_std_core__list _ctail_17325, kk_context_t* _ctx) {
  struct kk_std_core__mlift17697_op_fun20136__t* _self = kk_function_as(struct kk_std_core__mlift17697_op_fun20136__t*, _fself);
  kk_function_t _accm = _self->_accm; /* (list<2735>) -> list<2735> */
  kk_box_t _ctail_17326 = _self->_ctail_17326; /* 2735 */
  kk_drop_match(_self, {kk_function_dup(_accm);kk_box_dup(_ctail_17326);}, {}, _ctx)
  kk_std_core__list _x20137 = kk_std_core__new_Cons(kk_reuse_null, _ctail_17326, _ctail_17325, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, _x20137, _ctx));
}

kk_std_core__list kk_std_core__mlift17697_op(kk_function_t _accm, kk_function_t f0, kk_std_core__list yy0, kk_box_t _ctail_17326, kk_context_t* _ctx) { /* forall<a,b,e> ((list<b>) -> list<b>, f : (value : a, rest : list<a>) -> e b, yy : list<a>, b) -> e list<b> */ 
  return kk_std_core__ctailm_lift17266_map_peek(f0, yy0, kk_std_core__new_mlift17697_op_fun20136(_accm, _ctail_17326, _ctx), _ctx);
}
 
// lifted


// lift anonymous function
struct kk_std_core__ctail_lift17266_map_peek_fun20142__t {
  struct kk_function_s _base;
  kk_function_t f1;
  kk_std_core__list yy1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_lift17266_map_peek_fun20142(kk_function_t _fself, kk_box_t _b_18337, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_lift17266_map_peek_fun20142(kk_function_t f1, kk_std_core__list yy1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_lift17266_map_peek_fun20142__t* _self = kk_function_alloc_as(struct kk_std_core__ctail_lift17266_map_peek_fun20142__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_lift17266_map_peek_fun20142, kk_context());
  _self->f1 = f1;
  _self->yy1 = yy1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_lift17266_map_peek_fun20142(kk_function_t _fself, kk_box_t _b_18337, kk_context_t* _ctx) {
  struct kk_std_core__ctail_lift17266_map_peek_fun20142__t* _self = kk_function_as(struct kk_std_core__ctail_lift17266_map_peek_fun20142__t*, _fself);
  kk_function_t f1 = _self->f1; /* (value : 2734, rest : list<2734>) -> 2736 2735 */
  kk_std_core__list yy1 = _self->yy1; /* list<2734> */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<2735>> */
  kk_drop_match(_self, {kk_function_dup(f1);kk_std_core__list_dup(yy1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x20143 = kk_std_core__mlift17696_op(_acc0, f1, yy1, _b_18337, _ctx); /*list<2735>*/
  return kk_std_core__list_box(_x20143, _ctx);
}

kk_std_core__list kk_std_core__ctail_lift17266_map_peek(kk_function_t f1, kk_std_core__list ys, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,b,e> (f : (value : a, rest : list<a>) -> e b, ys : list<a>, ctail<list<b>>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20138 = kk_std_core__as_Cons(ys);
    kk_box_t y = _con20138->head;
    kk_std_core__list yy1 = _con20138->tail;
    kk_reuse_t _ru_19944 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      _ru_19944 = (kk_std_core__list_reuse(ys));
    }
    else {
      kk_box_dup(y);
      kk_std_core__list_dup(yy1);
      kk_std_core__list_decref(ys, _ctx);
    }
    kk_box_t x_17784;
    kk_function_t _x20140 = kk_function_dup(f1); /*(value : 2734, rest : list<2734>) -> 2736 2735*/
    kk_std_core__list _x20139 = kk_std_core__list_dup(yy1); /*list<2734>*/
    x_17784 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_std_core__list, kk_context_t*), _x20140, (_x20140, y, _x20139, _ctx)); /*2735*/
    if (kk_yielding(kk_context())) {
      kk_reuse_drop(_ru_19944);
      kk_box_drop(x_17784, _ctx);
      kk_box_t _x20141 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_lift17266_map_peek_fun20142(f1, yy1, _acc0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20141, _ctx);
    }
    {
      kk_std_core__list _ctail_173220 = kk_std_core__list_hole(); /*list<2735>*/;
      kk_std_core__list _ctail_173230 = kk_std_core__new_Cons(_ru_19944, x_17784, _ctail_173220, _ctx); /*list<2735>*/;
      { // tailcall
        kk_std_core_types__ctail _x20144;
        kk_box_t* _b_18349_18343 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_173230)->tail)); /*cfield<list<2735>>*/;
        _x20144 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_173230, _ctx)),_b_18349_18343); /*ctail<0>*/
        ys = yy1;
        _acc0 = _x20144;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_function_drop(f1, _ctx);
    kk_box_t _x20145 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20145, _ctx);
  }
}
 
// lifted


// lift anonymous function
struct kk_std_core__ctailm_lift17266_map_peek_fun20150__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t f2;
  kk_std_core__list yy2;
};
static kk_box_t kk_std_core__ctailm_lift17266_map_peek_fun20150(kk_function_t _fself, kk_box_t _b_18357, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_lift17266_map_peek_fun20150(kk_function_t _accm0, kk_function_t f2, kk_std_core__list yy2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_lift17266_map_peek_fun20150__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_lift17266_map_peek_fun20150__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_lift17266_map_peek_fun20150, kk_context());
  _self->_accm0 = _accm0;
  _self->f2 = f2;
  _self->yy2 = yy2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_lift17266_map_peek_fun20150(kk_function_t _fself, kk_box_t _b_18357, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_lift17266_map_peek_fun20150__t* _self = kk_function_as(struct kk_std_core__ctailm_lift17266_map_peek_fun20150__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<2735>) -> list<2735> */
  kk_function_t f2 = _self->f2; /* (value : 2734, rest : list<2734>) -> 2736 2735 */
  kk_std_core__list yy2 = _self->yy2; /* list<2734> */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(f2);kk_std_core__list_dup(yy2);}, {}, _ctx)
  kk_std_core__list _x20151 = kk_std_core__mlift17697_op(_accm0, f2, yy2, _b_18357, _ctx); /*list<2735>*/
  return kk_std_core__list_box(_x20151, _ctx);
}


// lift anonymous function
struct kk_std_core__ctailm_lift17266_map_peek_fun20153__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t x0_17787;
};
static kk_std_core__list kk_std_core__ctailm_lift17266_map_peek_fun20153(kk_function_t _fself, kk_std_core__list _ctail_173250, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_lift17266_map_peek_fun20153(kk_function_t _accm0, kk_box_t x0_17787, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_lift17266_map_peek_fun20153__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_lift17266_map_peek_fun20153__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_lift17266_map_peek_fun20153, kk_context());
  _self->_accm0 = _accm0;
  _self->x0_17787 = x0_17787;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_lift17266_map_peek_fun20153(kk_function_t _fself, kk_std_core__list _ctail_173250, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_lift17266_map_peek_fun20153__t* _self = kk_function_as(struct kk_std_core__ctailm_lift17266_map_peek_fun20153__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<2735>) -> list<2735> */
  kk_box_t x0_17787 = _self->x0_17787; /* 2735 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(x0_17787);}, {}, _ctx)
  kk_std_core__list _x20154 = kk_std_core__new_Cons(kk_reuse_null, x0_17787, _ctail_173250, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x20154, _ctx));
}

kk_std_core__list kk_std_core__ctailm_lift17266_map_peek(kk_function_t f2, kk_std_core__list ys0, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,b,e> (f : (value : a, rest : list<a>) -> e b, ys : list<a>, (list<b>) -> list<b>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys0)) {
    struct kk_std_core_Cons* _con20146 = kk_std_core__as_Cons(ys0);
    kk_box_t y0 = _con20146->head;
    kk_std_core__list yy2 = _con20146->tail;
    if (kk_likely(kk_std_core__list_is_unique(ys0))) {
      kk_std_core__list_free(ys0);
    }
    else {
      kk_box_dup(y0);
      kk_std_core__list_dup(yy2);
      kk_std_core__list_decref(ys0, _ctx);
    }
    kk_box_t x0_17787;
    kk_function_t _x20148 = kk_function_dup(f2); /*(value : 2734, rest : list<2734>) -> 2736 2735*/
    kk_std_core__list _x20147 = kk_std_core__list_dup(yy2); /*list<2734>*/
    x0_17787 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_std_core__list, kk_context_t*), _x20148, (_x20148, y0, _x20147, _ctx)); /*2735*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x0_17787, _ctx);
      kk_box_t _x20149 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_lift17266_map_peek_fun20150(_accm0, f2, yy2, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20149, _ctx);
    }
    { // tailcall
      kk_function_t _x20152 = kk_std_core__new_ctailm_lift17266_map_peek_fun20153(_accm0, x0_17787, _ctx); /*(list<2735>) -> list<2735>*/
      ys0 = yy2;
      _accm0 = _x20152;
      goto kk__tailcall;
    }
  }
  {
    kk_function_drop(f2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// lifted


// lift anonymous function
struct kk_std_core__lift17266_map_peek_fun20156__t {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core__lift17266_map_peek_fun20156(kk_function_t _fself, kk_std_core__list _ctail_17324, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17266_map_peek_fun20156(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core__lift17266_map_peek_fun20156, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core__lift17266_map_peek_fun20156(kk_function_t _fself, kk_std_core__list _ctail_17324, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17324;
}

kk_std_core__list kk_std_core__lift17266_map_peek(kk_function_t f3, kk_std_core__list ys1, kk_context_t* _ctx) { /* forall<a,b,e> (f : (value : a, rest : list<a>) -> e b, ys : list<a>) -> e list<b> */ 
  bool _match_19913 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19913) {
    kk_std_core_types__ctail _x20155 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_lift17266_map_peek(f3, ys1, _x20155, _ctx);
  }
  {
    return kk_std_core__ctailm_lift17266_map_peek(f3, ys1, kk_std_core__new_lift17266_map_peek_fun20156(_ctx), _ctx);
  }
}

kk_integer_t kk_std_core_maxListStack;

kk_ref_t kk_std_core_trace_enabled;
 
// Compose two funs `f` and `g`.


// lift anonymous function
struct kk_std_core_o_fun20158__t {
  struct kk_function_s _base;
  kk_function_t f;
  kk_function_t g;
};
static kk_box_t kk_std_core_o_fun20158(kk_function_t _fself, kk_box_t x, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_o_fun20158(kk_function_t f, kk_function_t g, kk_context_t* _ctx) {
  struct kk_std_core_o_fun20158__t* _self = kk_function_alloc_as(struct kk_std_core_o_fun20158__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_o_fun20158, kk_context());
  _self->f = f;
  _self->g = g;
  return &_self->_base;
}

static kk_box_t kk_std_core_o_fun20158(kk_function_t _fself, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_o_fun20158__t* _self = kk_function_as(struct kk_std_core_o_fun20158__t*, _fself);
  kk_function_t f = _self->f; /* (2778) -> 2775 2776 */
  kk_function_t g = _self->g; /* (2774) -> 2775 2778 */
  kk_drop_match(_self, {kk_function_dup(f);kk_function_dup(g);}, {}, _ctx)
  kk_box_t x0_17790 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), g, (g, x, _ctx)); /*2778*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(x0_17790, _ctx);
    return kk_std_core_hnd_yield_extend(f, _ctx);
  }
  {
    return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, x0_17790, _ctx));
  }
}

kk_function_t kk_std_core_o(kk_function_t f, kk_function_t g, kk_context_t* _ctx) { /* forall<a,b,c,e> (f : (a) -> e b, g : (c) -> e a) -> ((x : c) -> e b) */ 
  return kk_std_core_new_o_fun20158(f, g, _ctx);
}
 
// monadic lift

kk_box_t kk_std_core__mlift17698_once(kk_function_t calc, kk_ref_t r, kk_std_core_types__maybe _y_17422, kk_context_t* _ctx) { /* forall<_h,_e,a> (calc : () -> a, r : ref<_h,maybe<a>>, maybe<a>) -> <read<_h>,write<_h>,div|_e> a */ 
  if (kk_std_core_types__is_Just(_y_17422)) {
    kk_box_t x = _y_17422._cons.Just.value;
    kk_ref_drop(r, _ctx);
    kk_function_drop(calc, _ctx);
    return x;
  }
  {
    kk_box_t x0 = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), calc, (calc, _ctx)); /*2899*/;
    kk_unit_t __ = kk_Unit;
    kk_unit_t _brw_19911 = kk_Unit;
    kk_box_t _x20160;
    kk_std_core_types__maybe _x20161;
    kk_box_t _x20162 = kk_box_dup(x0); /*2899*/
    _x20161 = kk_std_core_types__new_Just(_x20162, _ctx); /*maybe<105>*/
    _x20160 = kk_std_core_types__maybe_box(_x20161, _ctx); /*170*/
    kk_ref_set_borrow(r,_x20160,kk_context());
    kk_ref_drop(r, _ctx);
    _brw_19911;
    return x0;
  }
}
 
// Given a total function to calculate a value `:a`, return
// a total function that only calculates the value once and then
// returns the cached result.


// lift anonymous function
struct kk_std_core_once_fun20163__t {
  struct kk_function_s _base;
  kk_function_t calc;
  kk_ref_t r;
};
static kk_box_t kk_std_core_once_fun20163(kk_function_t _fself, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_once_fun20163(kk_function_t calc, kk_ref_t r, kk_context_t* _ctx) {
  struct kk_std_core_once_fun20163__t* _self = kk_function_alloc_as(struct kk_std_core_once_fun20163__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_once_fun20163, kk_context());
  _self->calc = calc;
  _self->r = r;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_once_fun20166__t {
  struct kk_function_s _base;
  kk_function_t calc;
  kk_ref_t r;
};
static kk_box_t kk_std_core_once_fun20166(kk_function_t _fself, kk_std_core_types__maybe _y_17422, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_once_fun20166(kk_function_t calc, kk_ref_t r, kk_context_t* _ctx) {
  struct kk_std_core_once_fun20166__t* _self = kk_function_alloc_as(struct kk_std_core_once_fun20166__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_once_fun20166, kk_context());
  _self->calc = calc;
  _self->r = r;
  return &_self->_base;
}

static kk_box_t kk_std_core_once_fun20166(kk_function_t _fself, kk_std_core_types__maybe _y_17422, kk_context_t* _ctx) {
  struct kk_std_core_once_fun20166__t* _self = kk_function_as(struct kk_std_core_once_fun20166__t*, _fself);
  kk_function_t calc = _self->calc; /* () -> 2899 */
  kk_ref_t r = _self->r; /* ref<_2808,maybe<2899>> */
  kk_drop_match(_self, {kk_function_dup(calc);kk_ref_dup(r);}, {}, _ctx)
  if (kk_std_core_types__is_Just(_y_17422)) {
    kk_box_t x0 = _y_17422._cons.Just.value;
    kk_ref_drop(r, _ctx);
    kk_function_drop(calc, _ctx);
    return x0;
  }
  {
    kk_box_t x00 = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), calc, (calc, _ctx)); /*2899*/;
    kk_unit_t __ = kk_Unit;
    kk_unit_t _brw_19910 = kk_Unit;
    kk_box_t _x20167;
    kk_std_core_types__maybe _x20168;
    kk_box_t _x20169 = kk_box_dup(x00); /*2899*/
    _x20168 = kk_std_core_types__new_Just(_x20169, _ctx); /*maybe<105>*/
    _x20167 = kk_std_core_types__maybe_box(_x20168, _ctx); /*170*/
    kk_ref_set_borrow(r,_x20167,kk_context());
    kk_ref_drop(r, _ctx);
    _brw_19910;
    return x00;
  }
}


// lift anonymous function
struct kk_std_core_once_fun20170__t {
  struct kk_function_s _base;
  kk_function_t next0_17795;
};
static kk_box_t kk_std_core_once_fun20170(kk_function_t _fself, kk_box_t _b_18376, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_once_fun20170(kk_function_t next0_17795, kk_context_t* _ctx) {
  struct kk_std_core_once_fun20170__t* _self = kk_function_alloc_as(struct kk_std_core_once_fun20170__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_once_fun20170, kk_context());
  _self->next0_17795 = next0_17795;
  return &_self->_base;
}

static kk_box_t kk_std_core_once_fun20170(kk_function_t _fself, kk_box_t _b_18376, kk_context_t* _ctx) {
  struct kk_std_core_once_fun20170__t* _self = kk_function_as(struct kk_std_core_once_fun20170__t*, _fself);
  kk_function_t next0_17795 = _self->next0_17795; /* (maybe<2899>) -> <read<_2808>,write<_2808>,div|_2891> 2899 */
  kk_drop_match(_self, {kk_function_dup(next0_17795);}, {}, _ctx)
  kk_std_core_types__maybe _x20171 = kk_std_core_types__maybe_unbox(_b_18376, _ctx); /*maybe<2899>*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_types__maybe, kk_context_t*), next0_17795, (next0_17795, _x20171, _ctx));
}
static kk_box_t kk_std_core_once_fun20163(kk_function_t _fself, kk_context_t* _ctx) {
  struct kk_std_core_once_fun20163__t* _self = kk_function_as(struct kk_std_core_once_fun20163__t*, _fself);
  kk_function_t calc = _self->calc; /* () -> 2899 */
  kk_ref_t r = _self->r; /* ref<_2808,maybe<2899>> */
  kk_drop_match(_self, {kk_function_dup(calc);kk_ref_dup(r);}, {}, _ctx)
  kk_std_core_types__maybe x_17794;
  kk_box_t _x20164;
  kk_ref_t _x20165 = kk_ref_dup(r); /*ref<_2808,maybe<2899>>*/
  _x20164 = kk_ref_get(_x20165,kk_context()); /*184*/
  x_17794 = kk_std_core_types__maybe_unbox(_x20164, _ctx); /*maybe<2899>*/
  kk_function_t next0_17795 = kk_std_core_new_once_fun20166(calc, r, _ctx); /*(maybe<2899>) -> <read<_2808>,write<_2808>,div|_2891> 2899*/;
  if (kk_yielding(kk_context())) {
    kk_std_core_types__maybe_drop(x_17794, _ctx);
    return kk_std_core_hnd_yield_extend(kk_std_core_new_once_fun20170(next0_17795, _ctx), _ctx);
  }
  {
    return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_types__maybe, kk_context_t*), next0_17795, (next0_17795, x_17794, _ctx));
  }
}

kk_function_t kk_std_core_once(kk_function_t calc, kk_context_t* _ctx) { /* forall<a> (calc : () -> a) -> (() -> a) */ 
  kk_ref_t r = kk_ref_alloc((kk_std_core_types__maybe_box(kk_std_core_types__new_Nothing(_ctx), _ctx)),kk_context()); /*ref<_2808,maybe<2899>>*/;
  return kk_std_core_new_once_fun20163(calc, r, _ctx);
}

kk_std_core_types__maybe kk_std_core_xparse_int(kk_string_t s, bool hex, kk_context_t* _ctx) { /* (s : string, hex : bool) -> maybe<int> */ 
  return kk_integer_xparse(s,hex,kk_context());
}

kk_box_t kk_std_core_phantom(kk_context_t* _ctx) { /* forall<a> () -> a */ 
  return kk_box_null;
}
 
// Convert a string to lower-case

kk_string_t kk_std_core_to_lower(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> string */ 
  return kk_string_to_lower(s,kk_context());
}

kk_unit_t kk_std_core_xtrace(kk_string_t message0, kk_context_t* _ctx) { /* (message : string) -> () */ 
  kk_trace(message0,kk_context()); return kk_Unit;
}

kk_unit_t kk_std_core_xtrace_any(kk_string_t message0, kk_box_t x, kk_context_t* _ctx) { /* forall<a> (message : string, x : a) -> () */ 
  kk_trace_any(message0,x,kk_context()); return kk_Unit;
}

kk_ref_t kk_std_core_unique_count;

kk_vector_t kk_std_core_unvlist(kk_std_core__list xs, kk_context_t* _ctx) { /* forall<a> (xs : list<a>) -> vector<a> */ 
  return kk_list_to_vector(xs,kk_context());
}
 
// Create a new vector of length `n`  with initial elements given by function `f` .

kk_vector_t kk_std_core_vector_initz(kk_ssize_t n, kk_function_t f, kk_context_t* _ctx) { /* forall<a> (n : ssize_t, f : (ssize_t) -> a) -> vector<a> */ 
  return kk_vector_init(n,f,kk_context());
}

bool kk_std_core__lp__excl__eq__4_rp_(kk_std_core_types__order x, kk_std_core_types__order y, kk_context_t* _ctx) { /* (x : order, y : order) -> bool */ 
  kk_integer_t _brw_19905;
  if (kk_std_core_types__is_Lt(x)) {
    _brw_19905 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20174;
  }
  if (kk_std_core_types__is_Eq(x)) {
    _brw_19905 = kk_integer_from_small(0); /*int*/
    goto _match20174;
  }
  {
    _brw_19905 = kk_integer_from_small(1); /*int*/
  }
  _match20174: ;
  kk_integer_t _brw_19904;
  if (kk_std_core_types__is_Lt(y)) {
    _brw_19904 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20175;
  }
  if (kk_std_core_types__is_Eq(y)) {
    _brw_19904 = kk_integer_from_small(0); /*int*/
    goto _match20175;
  }
  {
    _brw_19904 = kk_integer_from_small(1); /*int*/
  }
  _match20175: ;
  bool _brw_19906 = kk_integer_neq_borrow(_brw_19905,_brw_19904,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19905, _ctx);
  kk_integer_drop(_brw_19904, _ctx);
  return _brw_19906;
}
 
// Append two lists.

kk_std_core__list kk_std_core__ctail_append(kk_std_core__list xs, kk_std_core__list ys, kk_std_core_types__ctail _acc, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, ys : list<a>, ctail<list<a>>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20178 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20178->head;
    kk_std_core__list xx = _con20178->tail;
    kk_reuse_t _ru_19946 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      _ru_19946 = (kk_std_core__list_reuse(xs));
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_std_core__list _ctail_17327 = kk_std_core__list_hole(); /*list<3291>*/;
    kk_std_core__list _ctail_17328 = kk_std_core__new_Cons(_ru_19946, x, _ctail_17327, _ctx); /*list<3291>*/;
    { // tailcall
      kk_std_core_types__ctail _x20179;
      kk_box_t* _b_18390_18385 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17328)->tail)); /*cfield<list<3291>>*/;
      _x20179 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17328, _ctx)),_b_18390_18385); /*ctail<0>*/
      xs = xx;
      _acc = _x20179;
      goto kk__tailcall;
    }
  }
  {
    kk_box_t _x20180 = kk_ctail_resolve(_acc,(kk_std_core__list_box(ys, _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20180, _ctx);
  }
}
 
// Append two lists.

kk_std_core__list kk_std_core_append(kk_std_core__list xs0, kk_std_core__list ys0, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, ys : list<a>) -> list<a> */ 
  kk_std_core_types__ctail _x20181 = kk_ctail_nil(); /*ctail<0>*/
  return kk_std_core__ctail_append(xs0, ys0, _x20181, _ctx);
}
 
// Append two strings.

kk_string_t kk_std_core__lp__plus__plus__1_rp_(kk_string_t x, kk_string_t y, kk_context_t* _ctx) { /* (x : string, y : string) -> string */ 
  return kk_string_cat(x,y,kk_context());
}

kk_unit_t kk_std_core_printsln(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> console () */ 
  kk_std_core_types__maybe _match_19901;
  kk_box_t _x20182;
  kk_ref_t _x20183 = kk_ref_dup(kk_std_core_redirect); /*ref<global,maybe<(string) -> console ()>>*/
  _x20182 = kk_ref_get(_x20183,kk_context()); /*184*/
  _match_19901 = kk_std_core_types__maybe_unbox(_x20182, _ctx); /*maybe<(string) -> console ()>*/
  if (kk_std_core_types__is_Nothing(_match_19901)) {
    kk_std_core_xprintsln(s, _ctx); return kk_Unit;
  }
  {
    kk_box_t _fun_unbox_x18400 = _match_19901._cons.Just.value;
    kk_string_t _b_18403;
    kk_string_t _x20184;
    kk_define_string_literal(, _s20185, 1, "\n")
    _x20184 = kk_string_dup(_s20185); /*string*/
    _b_18403 = kk_std_core__lp__plus__plus__1_rp_(s, _x20184, _ctx); /*string*/
    kk_box_t _x20186;
    kk_function_t _x20187 = kk_function_unbox(_fun_unbox_x18400); /*(18401) -> console 18402*/
    _x20186 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20187, (_x20187, kk_string_box(_b_18403), _ctx)); /*18402*/
    kk_unit_unbox(_x20186); return kk_Unit;
  }
}

bool kk_std_core__lp__eq__eq__4_rp_(kk_std_core_types__order x, kk_std_core_types__order y, kk_context_t* _ctx) { /* (x : order, y : order) -> bool */ 
  kk_integer_t _brw_19899;
  if (kk_std_core_types__is_Lt(x)) {
    _brw_19899 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20188;
  }
  if (kk_std_core_types__is_Eq(x)) {
    _brw_19899 = kk_integer_from_small(0); /*int*/
    goto _match20188;
  }
  {
    _brw_19899 = kk_integer_from_small(1); /*int*/
  }
  _match20188: ;
  kk_integer_t _brw_19898;
  if (kk_std_core_types__is_Lt(y)) {
    _brw_19898 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20189;
  }
  if (kk_std_core_types__is_Eq(y)) {
    _brw_19898 = kk_integer_from_small(0); /*int*/
    goto _match20189;
  }
  {
    _brw_19898 = kk_integer_from_small(1); /*int*/
  }
  _match20189: ;
  bool _brw_19900 = kk_integer_eq_borrow(_brw_19899,_brw_19898,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19899, _ctx);
  kk_integer_drop(_brw_19898, _ctx);
  return _brw_19900;
}

bool kk_std_core__lp__lt__5_rp_(kk_std_core_types__order x, kk_std_core_types__order y, kk_context_t* _ctx) { /* (x : order, y : order) -> bool */ 
  kk_integer_t _brw_19896;
  if (kk_std_core_types__is_Lt(x)) {
    _brw_19896 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20190;
  }
  if (kk_std_core_types__is_Eq(x)) {
    _brw_19896 = kk_integer_from_small(0); /*int*/
    goto _match20190;
  }
  {
    _brw_19896 = kk_integer_from_small(1); /*int*/
  }
  _match20190: ;
  kk_integer_t _brw_19895;
  if (kk_std_core_types__is_Lt(y)) {
    _brw_19895 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20191;
  }
  if (kk_std_core_types__is_Eq(y)) {
    _brw_19895 = kk_integer_from_small(0); /*int*/
    goto _match20191;
  }
  {
    _brw_19895 = kk_integer_from_small(1); /*int*/
  }
  _match20191: ;
  bool _brw_19897 = kk_integer_lt_borrow(_brw_19896,_brw_19895,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19896, _ctx);
  kk_integer_drop(_brw_19895, _ctx);
  return _brw_19897;
}

kk_std_core_types__order kk_std_core_order(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> order */ 
  bool _match_19889;
  kk_integer_t _brw_19893 = kk_integer_from_small(0); /*int*/;
  bool _brw_19894 = kk_integer_lt_borrow(i,_brw_19893,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19893, _ctx);
  _match_19889 = _brw_19894; /*bool*/
  if (_match_19889) {
    kk_integer_drop(i, _ctx);
    return kk_std_core_types__new_Lt(_ctx);
  }
  {
    bool _match_19890;
    kk_integer_t _brw_19891 = kk_integer_from_small(0); /*int*/;
    bool _brw_19892 = kk_integer_gt_borrow(i,_brw_19891,kk_context()); /*bool*/;
    kk_integer_drop(i, _ctx);
    kk_integer_drop(_brw_19891, _ctx);
    _match_19890 = _brw_19892; /*bool*/
    if (_match_19890) {
      return kk_std_core_types__new_Gt(_ctx);
    }
    {
      return kk_std_core_types__new_Eq(_ctx);
    }
  }
}
 
// Compare two strings.
// Uses the character codes directly for comparison

kk_std_core_types__order kk_std_core_compare_4(kk_string_t x, kk_string_t y, kk_context_t* _ctx) { /* (x : string, y : string) -> order */ 
  kk_integer_t i_17109 = kk_std_core_string_compare(x, y, _ctx); /*int*/;
  bool _match_19883;
  kk_integer_t _brw_19887 = kk_integer_from_small(0); /*int*/;
  bool _brw_19888 = kk_integer_lt_borrow(i_17109,_brw_19887,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19887, _ctx);
  _match_19883 = _brw_19888; /*bool*/
  if (_match_19883) {
    kk_integer_drop(i_17109, _ctx);
    return kk_std_core_types__new_Lt(_ctx);
  }
  {
    bool _match_19884;
    kk_integer_t _brw_19885 = kk_integer_from_small(0); /*int*/;
    bool _brw_19886 = kk_integer_gt_borrow(i_17109,_brw_19885,kk_context()); /*bool*/;
    kk_integer_drop(i_17109, _ctx);
    kk_integer_drop(_brw_19885, _ctx);
    _match_19884 = _brw_19886; /*bool*/
    if (_match_19884) {
      return kk_std_core_types__new_Gt(_ctx);
    }
    {
      return kk_std_core_types__new_Eq(_ctx);
    }
  }
}

bool kk_std_core__lp__lt__7_rp_(kk_string_t x, kk_string_t y, kk_context_t* _ctx) { /* (x : string, y : string) -> bool */ 
  kk_std_core_types__order x0_17243 = kk_std_core_compare_4(x, y, _ctx); /*order*/;
  kk_integer_t _brw_19881;
  if (kk_std_core_types__is_Lt(x0_17243)) {
    _brw_19881 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20192;
  }
  if (kk_std_core_types__is_Eq(x0_17243)) {
    _brw_19881 = kk_integer_from_small(0); /*int*/
    goto _match20192;
  }
  {
    _brw_19881 = kk_integer_from_small(1); /*int*/
  }
  _match20192: ;
  kk_integer_t _brw_19880 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/;
  bool _brw_19882 = kk_integer_eq_borrow(_brw_19881,_brw_19880,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19881, _ctx);
  kk_integer_drop(_brw_19880, _ctx);
  return _brw_19882;
}

bool kk_std_core__lp__gt__3_rp_(kk_std_core_types__order x, kk_std_core_types__order y, kk_context_t* _ctx) { /* (x : order, y : order) -> bool */ 
  kk_integer_t _brw_19878;
  if (kk_std_core_types__is_Lt(x)) {
    _brw_19878 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20193;
  }
  if (kk_std_core_types__is_Eq(x)) {
    _brw_19878 = kk_integer_from_small(0); /*int*/
    goto _match20193;
  }
  {
    _brw_19878 = kk_integer_from_small(1); /*int*/
  }
  _match20193: ;
  kk_integer_t _brw_19877;
  if (kk_std_core_types__is_Lt(y)) {
    _brw_19877 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20194;
  }
  if (kk_std_core_types__is_Eq(y)) {
    _brw_19877 = kk_integer_from_small(0); /*int*/
    goto _match20194;
  }
  {
    _brw_19877 = kk_integer_from_small(1); /*int*/
  }
  _match20194: ;
  bool _brw_19879 = kk_integer_gt_borrow(_brw_19878,_brw_19877,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19878, _ctx);
  kk_integer_drop(_brw_19877, _ctx);
  return _brw_19879;
}

bool kk_std_core__lp__gt__5_rp_(kk_string_t x, kk_string_t y, kk_context_t* _ctx) { /* (x : string, y : string) -> bool */ 
  kk_std_core_types__order x0_17245 = kk_std_core_compare_4(x, y, _ctx); /*order*/;
  kk_integer_t _brw_19875;
  if (kk_std_core_types__is_Lt(x0_17245)) {
    _brw_19875 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20195;
  }
  if (kk_std_core_types__is_Eq(x0_17245)) {
    _brw_19875 = kk_integer_from_small(0); /*int*/
    goto _match20195;
  }
  {
    _brw_19875 = kk_integer_from_small(1); /*int*/
  }
  _match20195: ;
  kk_integer_t _brw_19874 = kk_integer_from_small(1); /*int*/;
  bool _brw_19876 = kk_integer_eq_borrow(_brw_19875,_brw_19874,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19875, _ctx);
  kk_integer_drop(_brw_19874, _ctx);
  return _brw_19876;
}

kk_std_core_types__order kk_std_core_compare_2(bool x, bool y, kk_context_t* _ctx) { /* (x : bool, y : bool) -> order */ 
  if (x) {
    if (x) {
      if (y) {
        return kk_std_core_types__new_Eq(_ctx);
      }
      {
        return kk_std_core_types__new_Gt(_ctx);
      }
    }
    {
      return kk_std_core_types__new_Eq(_ctx);
    }
  }
  if (y) {
    return kk_std_core_types__new_Lt(_ctx);
  }
  if (x) {
    if (y) {
      return kk_std_core_types__new_Eq(_ctx);
    }
    {
      return kk_std_core_types__new_Gt(_ctx);
    }
  }
  {
    return kk_std_core_types__new_Eq(_ctx);
  }
}

bool kk_std_core__lp__gt__eq__4_rp_(kk_std_core_types__order x, kk_std_core_types__order y, kk_context_t* _ctx) { /* (x : order, y : order) -> bool */ 
  kk_integer_t _brw_19868;
  if (kk_std_core_types__is_Lt(x)) {
    _brw_19868 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20196;
  }
  if (kk_std_core_types__is_Eq(x)) {
    _brw_19868 = kk_integer_from_small(0); /*int*/
    goto _match20196;
  }
  {
    _brw_19868 = kk_integer_from_small(1); /*int*/
  }
  _match20196: ;
  kk_integer_t _brw_19867;
  if (kk_std_core_types__is_Lt(y)) {
    _brw_19867 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20197;
  }
  if (kk_std_core_types__is_Eq(y)) {
    _brw_19867 = kk_integer_from_small(0); /*int*/
    goto _match20197;
  }
  {
    _brw_19867 = kk_integer_from_small(1); /*int*/
  }
  _match20197: ;
  bool _brw_19869 = kk_integer_gte_borrow(_brw_19868,_brw_19867,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19868, _ctx);
  kk_integer_drop(_brw_19867, _ctx);
  return _brw_19869;
}

bool kk_std_core__lp__gt__eq__6_rp_(kk_string_t x, kk_string_t y, kk_context_t* _ctx) { /* (x : string, y : string) -> bool */ 
  kk_std_core_types__order x0_17247 = kk_std_core_compare_4(x, y, _ctx); /*order*/;
  kk_integer_t _brw_19865;
  if (kk_std_core_types__is_Lt(x0_17247)) {
    _brw_19865 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20198;
  }
  if (kk_std_core_types__is_Eq(x0_17247)) {
    _brw_19865 = kk_integer_from_small(0); /*int*/
    goto _match20198;
  }
  {
    _brw_19865 = kk_integer_from_small(1); /*int*/
  }
  _match20198: ;
  kk_integer_t _brw_19864 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/;
  bool _brw_19866 = kk_integer_gt_borrow(_brw_19865,_brw_19864,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19865, _ctx);
  kk_integer_drop(_brw_19864, _ctx);
  return _brw_19866;
}
 
// lifted

kk_string_t kk_std_core__lift17267_joinsep(kk_string_t sep, kk_std_core__list ys, kk_string_t acc, kk_context_t* _ctx) { /* (sep : string, ys : list<string>, acc : string) -> string */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20199 = kk_std_core__as_Cons(ys);
    kk_box_t _box_x18405 = _con20199->head;
    kk_std_core__list yy = _con20199->tail;
    kk_string_t y = kk_string_unbox(_box_x18405);
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      kk_std_core__list_free(ys);
    }
    else {
      kk_string_dup(y);
      kk_std_core__list_dup(yy);
      kk_std_core__list_decref(ys, _ctx);
    }
    kk_string_t acc0_17293;
    kk_string_t _x20201;
    kk_string_t _x20202 = kk_string_dup(sep); /*string*/
    _x20201 = kk_std_core__lp__plus__plus__1_rp_(_x20202, y, _ctx); /*string*/
    acc0_17293 = kk_std_core__lp__plus__plus__1_rp_(acc, _x20201, _ctx); /*string*/
    { // tailcall
      ys = yy;
      acc = acc0_17293;
      goto kk__tailcall;
    }
  }
  {
    kk_string_drop(sep, _ctx);
    return acc;
  }
}
 
// Concatenate all strings in a list

kk_string_t kk_std_core_joinsep(kk_std_core__list xs, kk_string_t sep, kk_context_t* _ctx) { /* (xs : list<string>, sep : string) -> string */ 
  if (kk_std_core__is_Nil(xs)) {
    kk_string_drop(sep, _ctx);
    return kk_string_empty();
  }
  {
    struct kk_std_core_Cons* _con20204 = kk_std_core__as_Cons(xs);
    kk_box_t _box_x18406 = _con20204->head;
    kk_std_core__list xx = _con20204->tail;
    kk_string_t x = kk_string_unbox(_box_x18406);
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_string_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    return kk_std_core__lift17267_joinsep(sep, xx, x, _ctx);
  }
}
 
// lifted

kk_string_t kk_std_core__lift17268_join_2(kk_std_core__list ys, kk_string_t acc, kk_context_t* _ctx) { /* (ys : list<string>, acc : string) -> string */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20206 = kk_std_core__as_Cons(ys);
    kk_box_t _box_x18407 = _con20206->head;
    kk_std_core__list yy = _con20206->tail;
    kk_string_t y = kk_string_unbox(_box_x18407);
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      kk_std_core__list_free(ys);
    }
    else {
      kk_string_dup(y);
      kk_std_core__list_dup(yy);
      kk_std_core__list_decref(ys, _ctx);
    }
    { // tailcall
      kk_string_t _x20208;
      kk_string_t _x20209;
      kk_string_t _x20210 = kk_string_empty(); /*string*/
      _x20209 = kk_std_core__lp__plus__plus__1_rp_(_x20210, y, _ctx); /*string*/
      _x20208 = kk_std_core__lp__plus__plus__1_rp_(acc, _x20209, _ctx); /*string*/
      ys = yy;
      acc = _x20208;
      goto kk__tailcall;
    }
  }
  {
    return acc;
  }
}
 
// Concatenate all strings in a list

kk_string_t kk_std_core_join_2(kk_std_core__list xs, kk_context_t* _ctx) { /* (xs : list<string>) -> string */ 
  if (kk_std_core__is_Nil(xs)) {
    return kk_string_empty();
  }
  {
    struct kk_std_core_Cons* _con20213 = kk_std_core__as_Cons(xs);
    kk_box_t _box_x18408 = _con20213->head;
    kk_std_core__list xx = _con20213->tail;
    kk_string_t x = kk_string_unbox(_box_x18408);
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_string_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    return kk_std_core__lift17268_join_2(xx, x, _ctx);
  }
}
 
// Concatenate all strings in a list using a specific separator

kk_string_t kk_std_core_join_3(kk_std_core__list xs, kk_string_t sep, kk_context_t* _ctx) { /* (xs : list<string>, sep : string) -> string */ 
  if (kk_std_core__is_Nil(xs)) {
    kk_string_drop(sep, _ctx);
    return kk_string_empty();
  }
  {
    struct kk_std_core_Cons* _con20216 = kk_std_core__as_Cons(xs);
    kk_box_t _box_x18409 = _con20216->head;
    kk_std_core__list xx = _con20216->tail;
    kk_string_t x = kk_string_unbox(_box_x18409);
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_string_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    return kk_std_core__lift17267_joinsep(sep, xx, x, _ctx);
  }
}

bool kk_std_core__lp__lt__eq__5_rp_(kk_std_core_types__order x, kk_std_core_types__order y, kk_context_t* _ctx) { /* (x : order, y : order) -> bool */ 
  kk_integer_t _brw_19862;
  if (kk_std_core_types__is_Lt(x)) {
    _brw_19862 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20218;
  }
  if (kk_std_core_types__is_Eq(x)) {
    _brw_19862 = kk_integer_from_small(0); /*int*/
    goto _match20218;
  }
  {
    _brw_19862 = kk_integer_from_small(1); /*int*/
  }
  _match20218: ;
  kk_integer_t _brw_19861;
  if (kk_std_core_types__is_Lt(y)) {
    _brw_19861 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20219;
  }
  if (kk_std_core_types__is_Eq(y)) {
    _brw_19861 = kk_integer_from_small(0); /*int*/
    goto _match20219;
  }
  {
    _brw_19861 = kk_integer_from_small(1); /*int*/
  }
  _match20219: ;
  bool _brw_19863 = kk_integer_lte_borrow(_brw_19862,_brw_19861,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19862, _ctx);
  kk_integer_drop(_brw_19861, _ctx);
  return _brw_19863;
}

bool kk_std_core__lp__lt__eq__7_rp_(kk_string_t x, kk_string_t y, kk_context_t* _ctx) { /* (x : string, y : string) -> bool */ 
  kk_std_core_types__order x0_17249 = kk_std_core_compare_4(x, y, _ctx); /*order*/;
  kk_integer_t _brw_19859;
  if (kk_std_core_types__is_Lt(x0_17249)) {
    _brw_19859 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20220;
  }
  if (kk_std_core_types__is_Eq(x0_17249)) {
    _brw_19859 = kk_integer_from_small(0); /*int*/
    goto _match20220;
  }
  {
    _brw_19859 = kk_integer_from_small(1); /*int*/
  }
  _match20220: ;
  kk_integer_t _brw_19858 = kk_integer_from_small(1); /*int*/;
  bool _brw_19860 = kk_integer_lt_borrow(_brw_19859,_brw_19858,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19859, _ctx);
  kk_integer_drop(_brw_19858, _ctx);
  return _brw_19860;
}

kk_ssize_t kk_std_core_decr_1(kk_ssize_t i, kk_context_t* _ctx) { /* (i : ssize_t) -> ssize_t */ 
  return (i - 1);
}

kk_ssize_t kk_std_core_incr_1(kk_ssize_t i, kk_context_t* _ctx) { /* (i : ssize_t) -> ssize_t */ 
  return (i + 1);
}
 
// monadic lift

kk_unit_t kk_std_core__mlift17699_op(kk_function_t action, kk_ssize_t end, kk_ssize_t i, kk_unit_t wild__, kk_context_t* _ctx) { /* forall<e> (action : (ssize_t) -> e (), end : ssize_t, i : ssize_t, wild_ : ()) -> e () */ 
  kk_ssize_t i0_17296 = kk_std_core_incr_1(i, _ctx); /*ssize_t*/;
  kk_std_core__lift17269_forz(action, end, i0_17296, _ctx); return kk_Unit;
}
 
// lifted


// lift anonymous function
struct kk_std_core__lift17269_forz_fun20223__t {
  struct kk_function_s _base;
  kk_function_t action0;
  kk_ssize_t end0;
  kk_ssize_t i0;
};
static kk_box_t kk_std_core__lift17269_forz_fun20223(kk_function_t _fself, kk_box_t _b_18411, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17269_forz_fun20223(kk_function_t action0, kk_ssize_t end0, kk_ssize_t i0, kk_context_t* _ctx) {
  struct kk_std_core__lift17269_forz_fun20223__t* _self = kk_function_alloc_as(struct kk_std_core__lift17269_forz_fun20223__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17269_forz_fun20223, kk_context());
  _self->action0 = action0;
  _self->end0 = end0;
  _self->i0 = i0;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17269_forz_fun20223(kk_function_t _fself, kk_box_t _b_18411, kk_context_t* _ctx) {
  struct kk_std_core__lift17269_forz_fun20223__t* _self = kk_function_as(struct kk_std_core__lift17269_forz_fun20223__t*, _fself);
  kk_function_t action0 = _self->action0; /* (ssize_t) -> 4429 () */
  kk_ssize_t end0 = _self->end0; /* ssize_t */
  kk_ssize_t i0 = _self->i0; /* ssize_t */
  kk_drop_match(_self, {kk_function_dup(action0);;;}, {}, _ctx)
  kk_unit_t _x20224 = kk_Unit;
  kk_unit_t _x20225 = kk_Unit;
  kk_unit_unbox(_b_18411);
  kk_std_core__mlift17699_op(action0, end0, i0, _x20225, _ctx);
  return kk_unit_box(_x20224);
}

kk_unit_t kk_std_core__lift17269_forz(kk_function_t action0, kk_ssize_t end0, kk_ssize_t i0, kk_context_t* _ctx) { /* forall<e> (action : (ssize_t) -> e (), end : ssize_t, i : ssize_t) -> e () */ 
  kk__tailcall: ;
  bool _match_19856 = (i0 <= end0); /*bool*/;
  if (_match_19856) {
    kk_unit_t x_17803 = kk_Unit;
    kk_function_t _x20221 = kk_function_dup(action0); /*(ssize_t) -> 4429 ()*/
    kk_function_call(kk_unit_t, (kk_function_t, kk_ssize_t, kk_context_t*), _x20221, (_x20221, i0, _ctx));
    if (kk_yielding(kk_context())) {
      kk_box_t _x20222 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17269_forz_fun20223(action0, end0, i0, _ctx), _ctx); /*3860*/
      kk_unit_unbox(_x20222); return kk_Unit;
    }
    {
      kk_ssize_t i0_172960 = kk_std_core_incr_1(i0, _ctx); /*ssize_t*/;
      { // tailcall
        i0 = i0_172960;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_function_drop(action0, _ctx);
    kk_Unit; return kk_Unit;
  }
}


// lift anonymous function
struct kk_std_core_foreach_indexedz_fun20227__t {
  struct kk_function_s _base;
  kk_function_t f;
  kk_vector_t v;
};
static kk_unit_t kk_std_core_foreach_indexedz_fun20227(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_indexedz_fun20227(kk_function_t f, kk_vector_t v, kk_context_t* _ctx) {
  struct kk_std_core_foreach_indexedz_fun20227__t* _self = kk_function_alloc_as(struct kk_std_core_foreach_indexedz_fun20227__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_indexedz_fun20227, kk_context());
  _self->f = f;
  _self->v = v;
  return &_self->_base;
}

static kk_unit_t kk_std_core_foreach_indexedz_fun20227(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_foreach_indexedz_fun20227__t* _self = kk_function_as(struct kk_std_core_foreach_indexedz_fun20227__t*, _fself);
  kk_function_t f = _self->f; /* (4463, ssize_t) -> 4464 () */
  kk_vector_t v = _self->v; /* vector<4463> */
  kk_drop_match(_self, {kk_function_dup(f);kk_vector_dup(v);}, {}, _ctx)
  kk_box_t _x20228;
  kk_box_t _brw_19855 = kk_vector_at_borrow(v, i); /*223*/;
  kk_vector_drop(v, _ctx);
  _x20228 = _brw_19855; /*223*/
  return kk_function_call(kk_unit_t, (kk_function_t, kk_box_t, kk_ssize_t, kk_context_t*), f, (f, _x20228, i, _ctx));
}

kk_unit_t kk_std_core_foreach_indexedz(kk_vector_t v, kk_function_t f, kk_context_t* _ctx) { /* forall<a,e> (v : vector<a>, f : (a, ssize_t) -> e ()) -> e () */ 
  kk_ssize_t start0_17806 = ((kk_ssize_t)0); /*ssize_t*/;
  kk_ssize_t end_17807;
  kk_ssize_t _x20226 = kk_vector_len_borrow(v); /*ssize_t*/
  end_17807 = kk_std_core_decr_1(_x20226, _ctx); /*ssize_t*/
  kk_std_core__lift17269_forz(kk_std_core_new_foreach_indexedz_fun20227(f, v, _ctx), end_17807, start0_17806, _ctx); return kk_Unit;
}
 
// lifted

kk_integer_t kk_std_core__lift17270_length_1(kk_std_core__list ys, kk_integer_t acc, kk_context_t* _ctx) { /* forall<a> (ys : list<a>, acc : int) -> int */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20229 = kk_std_core__as_Cons(ys);
    kk_box_t _pat0 = _con20229->head;
    kk_std_core__list yy = _con20229->tail;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      kk_box_drop(_pat0, _ctx);
      kk_std_core__list_free(ys);
    }
    else {
      kk_std_core__list_dup(yy);
      kk_std_core__list_decref(ys, _ctx);
    }
    { // tailcall
      kk_integer_t _x20230 = kk_integer_add(acc,(kk_integer_from_small(1)),kk_context()); /*int*/
      ys = yy;
      acc = _x20230;
      goto kk__tailcall;
    }
  }
  {
    return acc;
  }
}
 
// Returns an integer list of increasing elements from `lo`  to `hi`
// (including both `lo`  and `hi` ).
// If `lo > hi`  the function returns the empty list.

kk_std_core__list kk_std_core__ctail_list(kk_integer_t lo, kk_integer_t hi, kk_std_core_types__ctail _acc, kk_context_t* _ctx) { /* (lo : int, hi : int, ctail<list<int>>) -> total list<int> */ 
  kk__tailcall: ;
  bool _match_19854 = kk_integer_lte_borrow(lo,hi,kk_context()); /*bool*/;
  if (_match_19854) {
    kk_std_core__list _ctail_17329 = kk_std_core__list_hole(); /*list<int>*/;
    kk_std_core__list _ctail_17330;
    kk_box_t _x20232;
    kk_integer_t _x20233 = kk_integer_dup(lo); /*int*/
    _x20232 = kk_integer_box(_x20233); /*61*/
    _ctail_17330 = kk_std_core__new_Cons(kk_reuse_null, _x20232, _ctail_17329, _ctx); /*list<int>*/
    { // tailcall
      kk_integer_t _x20234 = kk_integer_add(lo,(kk_integer_from_small(1)),kk_context()); /*int*/
      kk_std_core_types__ctail _x20235;
      kk_box_t* _b_18428_18423 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17330)->tail)); /*cfield<list<int>>*/;
      _x20235 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17330, _ctx)),_b_18428_18423); /*ctail<0>*/
      lo = _x20234;
      _acc = _x20235;
      goto kk__tailcall;
    }
  }
  {
    kk_integer_drop(lo, _ctx);
    kk_integer_drop(hi, _ctx);
    kk_box_t _x20236 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20236, _ctx);
  }
}
 
// Returns an integer list of increasing elements from `lo`  to `hi`
// (including both `lo`  and `hi` ).
// If `lo > hi`  the function returns the empty list.

kk_std_core__list kk_std_core_list(kk_integer_t lo0, kk_integer_t hi0, kk_context_t* _ctx) { /* (lo : int, hi : int) -> total list<int> */ 
  kk_std_core_types__ctail _x20237 = kk_ctail_nil(); /*ctail<0>*/
  return kk_std_core__ctail_list(lo0, hi0, _x20237, _ctx);
}
 
// Returns an integer list of increasing elements from `lo`  to `hi` with stride `stride`.
// If `lo > hi`  the function returns the empty list.

kk_std_core__list kk_std_core__ctail_list_1(kk_integer_t lo, kk_integer_t hi, kk_integer_t stride, kk_std_core_types__ctail _acc, kk_context_t* _ctx) { /* (lo : int, hi : int, stride : int, ctail<list<int>>) -> total list<int> */ 
  kk__tailcall: ;
  bool _match_19853 = kk_integer_lte_borrow(lo,hi,kk_context()); /*bool*/;
  if (_match_19853) {
    kk_std_core__list _ctail_17331 = kk_std_core__list_hole(); /*list<int>*/;
    kk_std_core__list _ctail_17332;
    kk_box_t _x20238;
    kk_integer_t _x20239 = kk_integer_dup(lo); /*int*/
    _x20238 = kk_integer_box(_x20239); /*61*/
    _ctail_17332 = kk_std_core__new_Cons(kk_reuse_null, _x20238, _ctail_17331, _ctx); /*list<int>*/
    { // tailcall
      kk_integer_t _x20240;
      kk_integer_t _x20241 = kk_integer_dup(stride); /*int*/
      _x20240 = kk_integer_add(lo,_x20241,kk_context()); /*int*/
      kk_std_core_types__ctail _x20242;
      kk_box_t* _b_18448_18443 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17332)->tail)); /*cfield<list<int>>*/;
      _x20242 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17332, _ctx)),_b_18448_18443); /*ctail<0>*/
      lo = _x20240;
      _acc = _x20242;
      goto kk__tailcall;
    }
  }
  {
    kk_integer_drop(stride, _ctx);
    kk_integer_drop(lo, _ctx);
    kk_integer_drop(hi, _ctx);
    kk_box_t _x20243 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20243, _ctx);
  }
}
 
// Returns an integer list of increasing elements from `lo`  to `hi` with stride `stride`.
// If `lo > hi`  the function returns the empty list.

kk_std_core__list kk_std_core_list_1(kk_integer_t lo0, kk_integer_t hi0, kk_integer_t stride0, kk_context_t* _ctx) { /* (lo : int, hi : int, stride : int) -> total list<int> */ 
  kk_std_core_types__ctail _x20244 = kk_ctail_nil(); /*ctail<0>*/
  return kk_std_core__ctail_list_1(lo0, hi0, stride0, _x20244, _ctx);
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17700_op(kk_std_core_types__ctail _acc, kk_function_t f, kk_integer_t hi, kk_integer_t lo, kk_box_t _ctail_17333, kk_context_t* _ctx) { /* forall<a,e> (ctail<list<a>>, f : (int) -> e a, hi : int, lo : int, a) -> e list<a> */ 
  kk_std_core__list _ctail_17334 = kk_std_core__list_hole(); /*list<5016>*/;
  kk_std_core__list _ctail_17335 = kk_std_core__new_Cons(kk_reuse_null, _ctail_17333, _ctail_17334, _ctx); /*list<5016>*/;
  kk_integer_t _x20245 = kk_integer_add(lo,(kk_integer_from_small(1)),kk_context()); /*int*/
  kk_std_core_types__ctail _x20246;
  kk_box_t* _b_18462_18459 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17335)->tail)); /*cfield<list<5016>>*/;
  _x20246 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17335, _ctx)),_b_18462_18459); /*ctail<0>*/
  return kk_std_core__ctail_list_2(_x20245, hi, f, _x20246, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17701_op_fun20248__t {
  struct kk_function_s _base;
  kk_function_t _accm;
  kk_box_t _ctail_17338;
};
static kk_std_core__list kk_std_core__mlift17701_op_fun20248(kk_function_t _fself, kk_std_core__list _ctail_17337, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17701_op_fun20248(kk_function_t _accm, kk_box_t _ctail_17338, kk_context_t* _ctx) {
  struct kk_std_core__mlift17701_op_fun20248__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17701_op_fun20248__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17701_op_fun20248, kk_context());
  _self->_accm = _accm;
  _self->_ctail_17338 = _ctail_17338;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__mlift17701_op_fun20248(kk_function_t _fself, kk_std_core__list _ctail_17337, kk_context_t* _ctx) {
  struct kk_std_core__mlift17701_op_fun20248__t* _self = kk_function_as(struct kk_std_core__mlift17701_op_fun20248__t*, _fself);
  kk_function_t _accm = _self->_accm; /* (list<5016>) -> list<5016> */
  kk_box_t _ctail_17338 = _self->_ctail_17338; /* 5016 */
  kk_drop_match(_self, {kk_function_dup(_accm);kk_box_dup(_ctail_17338);}, {}, _ctx)
  kk_std_core__list _x20249 = kk_std_core__new_Cons(kk_reuse_null, _ctail_17338, _ctail_17337, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, _x20249, _ctx));
}

kk_std_core__list kk_std_core__mlift17701_op(kk_function_t _accm, kk_function_t f0, kk_integer_t hi0, kk_integer_t lo0, kk_box_t _ctail_17338, kk_context_t* _ctx) { /* forall<a,e> ((list<a>) -> list<a>, f : (int) -> e a, hi : int, lo : int, a) -> e list<a> */ 
  kk_integer_t _x20247 = kk_integer_add(lo0,(kk_integer_from_small(1)),kk_context()); /*int*/
  return kk_std_core__ctailm_list_2(_x20247, hi0, f0, kk_std_core__new_mlift17701_op_fun20248(_accm, _ctail_17338, _ctx), _ctx);
}
 
// Applies a function `f` to list of increasing elements from `lo`  to `hi`
// (including both `lo`  and `hi` ).
// If `lo > hi`  the function returns the empty list.


// lift anonymous function
struct kk_std_core__ctail_list_fun20253__t_2 {
  struct kk_function_s _base;
  kk_function_t f1;
  kk_integer_t hi1;
  kk_integer_t lo1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_list_fun20253_2(kk_function_t _fself, kk_box_t _b_18467, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_list_fun20253_2(kk_function_t f1, kk_integer_t hi1, kk_integer_t lo1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_list_fun20253__t_2* _self = kk_function_alloc_as(struct kk_std_core__ctail_list_fun20253__t_2, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_list_fun20253_2, kk_context());
  _self->f1 = f1;
  _self->hi1 = hi1;
  _self->lo1 = lo1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_list_fun20253_2(kk_function_t _fself, kk_box_t _b_18467, kk_context_t* _ctx) {
  struct kk_std_core__ctail_list_fun20253__t_2* _self = kk_function_as(struct kk_std_core__ctail_list_fun20253__t_2*, _fself);
  kk_function_t f1 = _self->f1; /* (int) -> 5017 5016 */
  kk_integer_t hi1 = _self->hi1; /* int */
  kk_integer_t lo1 = _self->lo1; /* int */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<5016>> */
  kk_drop_match(_self, {kk_function_dup(f1);kk_integer_dup(hi1);kk_integer_dup(lo1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x20254 = kk_std_core__mlift17700_op(_acc0, f1, hi1, lo1, _b_18467, _ctx); /*list<5016>*/
  return kk_std_core__list_box(_x20254, _ctx);
}

kk_std_core__list kk_std_core__ctail_list_2(kk_integer_t lo1, kk_integer_t hi1, kk_function_t f1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,e> (lo : int, hi : int, f : (int) -> e a, ctail<list<a>>) -> e list<a> */ 
  kk__tailcall: ;
  bool _match_19851 = kk_integer_lte_borrow(lo1,hi1,kk_context()); /*bool*/;
  if (_match_19851) {
    kk_box_t x_17809;
    kk_function_t _x20251 = kk_function_dup(f1); /*(int) -> 5017 5016*/
    kk_integer_t _x20250 = kk_integer_dup(lo1); /*int*/
    x_17809 = kk_function_call(kk_box_t, (kk_function_t, kk_integer_t, kk_context_t*), _x20251, (_x20251, _x20250, _ctx)); /*5016*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x_17809, _ctx);
      kk_box_t _x20252 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_list_fun20253_2(f1, hi1, lo1, _acc0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20252, _ctx);
    }
    {
      kk_std_core__list _ctail_173340 = kk_std_core__list_hole(); /*list<5016>*/;
      kk_std_core__list _ctail_173350 = kk_std_core__new_Cons(kk_reuse_null, x_17809, _ctail_173340, _ctx); /*list<5016>*/;
      { // tailcall
        kk_integer_t _x20255 = kk_integer_add(lo1,(kk_integer_from_small(1)),kk_context()); /*int*/
        kk_std_core_types__ctail _x20256;
        kk_box_t* _b_18479_18473 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_173350)->tail)); /*cfield<list<5016>>*/;
        _x20256 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_173350, _ctx)),_b_18479_18473); /*ctail<0>*/
        lo1 = _x20255;
        _acc0 = _x20256;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_integer_drop(lo1, _ctx);
    kk_integer_drop(hi1, _ctx);
    kk_function_drop(f1, _ctx);
    kk_box_t _x20257 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20257, _ctx);
  }
}
 
// Applies a function `f` to list of increasing elements from `lo`  to `hi`
// (including both `lo`  and `hi` ).
// If `lo > hi`  the function returns the empty list.


// lift anonymous function
struct kk_std_core__ctailm_list_fun20261__t_2 {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t f2;
  kk_integer_t hi2;
  kk_integer_t lo2;
};
static kk_box_t kk_std_core__ctailm_list_fun20261_2(kk_function_t _fself, kk_box_t _b_18487, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_list_fun20261_2(kk_function_t _accm0, kk_function_t f2, kk_integer_t hi2, kk_integer_t lo2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_list_fun20261__t_2* _self = kk_function_alloc_as(struct kk_std_core__ctailm_list_fun20261__t_2, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_list_fun20261_2, kk_context());
  _self->_accm0 = _accm0;
  _self->f2 = f2;
  _self->hi2 = hi2;
  _self->lo2 = lo2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_list_fun20261_2(kk_function_t _fself, kk_box_t _b_18487, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_list_fun20261__t_2* _self = kk_function_as(struct kk_std_core__ctailm_list_fun20261__t_2*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<5016>) -> list<5016> */
  kk_function_t f2 = _self->f2; /* (int) -> 5017 5016 */
  kk_integer_t hi2 = _self->hi2; /* int */
  kk_integer_t lo2 = _self->lo2; /* int */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(f2);kk_integer_dup(hi2);kk_integer_dup(lo2);}, {}, _ctx)
  kk_std_core__list _x20262 = kk_std_core__mlift17701_op(_accm0, f2, hi2, lo2, _b_18487, _ctx); /*list<5016>*/
  return kk_std_core__list_box(_x20262, _ctx);
}


// lift anonymous function
struct kk_std_core__ctailm_list_fun20265__t_2 {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t x0_17812;
};
static kk_std_core__list kk_std_core__ctailm_list_fun20265_2(kk_function_t _fself, kk_std_core__list _ctail_173370, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_list_fun20265_2(kk_function_t _accm0, kk_box_t x0_17812, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_list_fun20265__t_2* _self = kk_function_alloc_as(struct kk_std_core__ctailm_list_fun20265__t_2, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_list_fun20265_2, kk_context());
  _self->_accm0 = _accm0;
  _self->x0_17812 = x0_17812;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_list_fun20265_2(kk_function_t _fself, kk_std_core__list _ctail_173370, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_list_fun20265__t_2* _self = kk_function_as(struct kk_std_core__ctailm_list_fun20265__t_2*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<5016>) -> list<5016> */
  kk_box_t x0_17812 = _self->x0_17812; /* 5016 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(x0_17812);}, {}, _ctx)
  kk_std_core__list _x20266 = kk_std_core__new_Cons(kk_reuse_null, x0_17812, _ctail_173370, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x20266, _ctx));
}

kk_std_core__list kk_std_core__ctailm_list_2(kk_integer_t lo2, kk_integer_t hi2, kk_function_t f2, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,e> (lo : int, hi : int, f : (int) -> e a, (list<a>) -> list<a>) -> e list<a> */ 
  kk__tailcall: ;
  bool _match_19849 = kk_integer_lte_borrow(lo2,hi2,kk_context()); /*bool*/;
  if (_match_19849) {
    kk_box_t x0_17812;
    kk_function_t _x20259 = kk_function_dup(f2); /*(int) -> 5017 5016*/
    kk_integer_t _x20258 = kk_integer_dup(lo2); /*int*/
    x0_17812 = kk_function_call(kk_box_t, (kk_function_t, kk_integer_t, kk_context_t*), _x20259, (_x20259, _x20258, _ctx)); /*5016*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x0_17812, _ctx);
      kk_box_t _x20260 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_list_fun20261_2(_accm0, f2, hi2, lo2, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20260, _ctx);
    }
    { // tailcall
      kk_integer_t _x20263 = kk_integer_add(lo2,(kk_integer_from_small(1)),kk_context()); /*int*/
      kk_function_t _x20264 = kk_std_core__new_ctailm_list_fun20265_2(_accm0, x0_17812, _ctx); /*(list<5016>) -> list<5016>*/
      lo2 = _x20263;
      _accm0 = _x20264;
      goto kk__tailcall;
    }
  }
  {
    kk_integer_drop(lo2, _ctx);
    kk_integer_drop(hi2, _ctx);
    kk_function_drop(f2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// Applies a function `f` to list of increasing elements from `lo`  to `hi`
// (including both `lo`  and `hi` ).
// If `lo > hi`  the function returns the empty list.


// lift anonymous function
struct kk_std_core_list_fun20268__t_2 {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core_list_fun20268_2(kk_function_t _fself, kk_std_core__list _ctail_17336, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_list_fun20268_2(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_list_fun20268_2, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core_list_fun20268_2(kk_function_t _fself, kk_std_core__list _ctail_17336, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17336;
}

kk_std_core__list kk_std_core_list_2(kk_integer_t lo3, kk_integer_t hi3, kk_function_t f3, kk_context_t* _ctx) { /* forall<a,e> (lo : int, hi : int, f : (int) -> e a) -> e list<a> */ 
  bool _match_19848 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19848) {
    kk_std_core_types__ctail _x20267 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_list_2(lo3, hi3, f3, _x20267, _ctx);
  }
  {
    return kk_std_core__ctailm_list_2(lo3, hi3, f3, kk_std_core_new_list_fun20268_2(_ctx), _ctx);
  }
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17702_op(kk_std_core_types__ctail _acc, kk_function_t f, kk_integer_t hi, kk_integer_t lo, kk_integer_t stride, kk_box_t _ctail_17339, kk_context_t* _ctx) { /* forall<a,e> (ctail<list<a>>, f : (int) -> e a, hi : int, lo : int, stride : int, a) -> e list<a> */ 
  kk_std_core__list _ctail_17340 = kk_std_core__list_hole(); /*list<5107>*/;
  kk_std_core__list _ctail_17341 = kk_std_core__new_Cons(kk_reuse_null, _ctail_17339, _ctail_17340, _ctx); /*list<5107>*/;
  kk_integer_t _x20269;
  kk_integer_t _x20270 = kk_integer_dup(stride); /*int*/
  _x20269 = kk_integer_add(lo,_x20270,kk_context()); /*int*/
  kk_std_core_types__ctail _x20271;
  kk_box_t* _b_18498_18495 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17341)->tail)); /*cfield<list<5107>>*/;
  _x20271 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17341, _ctx)),_b_18498_18495); /*ctail<0>*/
  return kk_std_core__ctail_list_3(_x20269, hi, stride, f, _x20271, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17703_op_fun20274__t {
  struct kk_function_s _base;
  kk_function_t _accm;
  kk_box_t _ctail_17344;
};
static kk_std_core__list kk_std_core__mlift17703_op_fun20274(kk_function_t _fself, kk_std_core__list _ctail_17343, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17703_op_fun20274(kk_function_t _accm, kk_box_t _ctail_17344, kk_context_t* _ctx) {
  struct kk_std_core__mlift17703_op_fun20274__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17703_op_fun20274__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17703_op_fun20274, kk_context());
  _self->_accm = _accm;
  _self->_ctail_17344 = _ctail_17344;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__mlift17703_op_fun20274(kk_function_t _fself, kk_std_core__list _ctail_17343, kk_context_t* _ctx) {
  struct kk_std_core__mlift17703_op_fun20274__t* _self = kk_function_as(struct kk_std_core__mlift17703_op_fun20274__t*, _fself);
  kk_function_t _accm = _self->_accm; /* (list<5107>) -> list<5107> */
  kk_box_t _ctail_17344 = _self->_ctail_17344; /* 5107 */
  kk_drop_match(_self, {kk_function_dup(_accm);kk_box_dup(_ctail_17344);}, {}, _ctx)
  kk_std_core__list _x20275 = kk_std_core__new_Cons(kk_reuse_null, _ctail_17344, _ctail_17343, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, _x20275, _ctx));
}

kk_std_core__list kk_std_core__mlift17703_op(kk_function_t _accm, kk_function_t f0, kk_integer_t hi0, kk_integer_t lo0, kk_integer_t stride0, kk_box_t _ctail_17344, kk_context_t* _ctx) { /* forall<a,e> ((list<a>) -> list<a>, f : (int) -> e a, hi : int, lo : int, stride : int, a) -> e list<a> */ 
  kk_integer_t _x20272;
  kk_integer_t _x20273 = kk_integer_dup(stride0); /*int*/
  _x20272 = kk_integer_add(lo0,_x20273,kk_context()); /*int*/
  return kk_std_core__ctailm_list_3(_x20272, hi0, stride0, f0, kk_std_core__new_mlift17703_op_fun20274(_accm, _ctail_17344, _ctx), _ctx);
}
 
// Returns an integer list of increasing elements from `lo`  to `hi` with stride `stride`.
// If `lo > hi`  the function returns the empty list.


// lift anonymous function
struct kk_std_core__ctail_list_fun20279__t_3 {
  struct kk_function_s _base;
  kk_function_t f1;
  kk_integer_t hi1;
  kk_integer_t lo1;
  kk_integer_t stride1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_list_fun20279_3(kk_function_t _fself, kk_box_t _b_18503, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_list_fun20279_3(kk_function_t f1, kk_integer_t hi1, kk_integer_t lo1, kk_integer_t stride1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_list_fun20279__t_3* _self = kk_function_alloc_as(struct kk_std_core__ctail_list_fun20279__t_3, 6, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_list_fun20279_3, kk_context());
  _self->f1 = f1;
  _self->hi1 = hi1;
  _self->lo1 = lo1;
  _self->stride1 = stride1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_list_fun20279_3(kk_function_t _fself, kk_box_t _b_18503, kk_context_t* _ctx) {
  struct kk_std_core__ctail_list_fun20279__t_3* _self = kk_function_as(struct kk_std_core__ctail_list_fun20279__t_3*, _fself);
  kk_function_t f1 = _self->f1; /* (int) -> 5108 5107 */
  kk_integer_t hi1 = _self->hi1; /* int */
  kk_integer_t lo1 = _self->lo1; /* int */
  kk_integer_t stride1 = _self->stride1; /* int */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<5107>> */
  kk_drop_match(_self, {kk_function_dup(f1);kk_integer_dup(hi1);kk_integer_dup(lo1);kk_integer_dup(stride1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x20280 = kk_std_core__mlift17702_op(_acc0, f1, hi1, lo1, stride1, _b_18503, _ctx); /*list<5107>*/
  return kk_std_core__list_box(_x20280, _ctx);
}

kk_std_core__list kk_std_core__ctail_list_3(kk_integer_t lo1, kk_integer_t hi1, kk_integer_t stride1, kk_function_t f1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,e> (lo : int, hi : int, stride : int, f : (int) -> e a, ctail<list<a>>) -> e list<a> */ 
  kk__tailcall: ;
  bool _match_19846 = kk_integer_lte_borrow(lo1,hi1,kk_context()); /*bool*/;
  if (_match_19846) {
    kk_box_t x_17815;
    kk_function_t _x20277 = kk_function_dup(f1); /*(int) -> 5108 5107*/
    kk_integer_t _x20276 = kk_integer_dup(lo1); /*int*/
    x_17815 = kk_function_call(kk_box_t, (kk_function_t, kk_integer_t, kk_context_t*), _x20277, (_x20277, _x20276, _ctx)); /*5107*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x_17815, _ctx);
      kk_box_t _x20278 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_list_fun20279_3(f1, hi1, lo1, stride1, _acc0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20278, _ctx);
    }
    {
      kk_std_core__list _ctail_173400 = kk_std_core__list_hole(); /*list<5107>*/;
      kk_std_core__list _ctail_173410 = kk_std_core__new_Cons(kk_reuse_null, x_17815, _ctail_173400, _ctx); /*list<5107>*/;
      { // tailcall
        kk_integer_t _x20281;
        kk_integer_t _x20282 = kk_integer_dup(stride1); /*int*/
        _x20281 = kk_integer_add(lo1,_x20282,kk_context()); /*int*/
        kk_std_core_types__ctail _x20283;
        kk_box_t* _b_18515_18509 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_173410)->tail)); /*cfield<list<5107>>*/;
        _x20283 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_173410, _ctx)),_b_18515_18509); /*ctail<0>*/
        lo1 = _x20281;
        _acc0 = _x20283;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_integer_drop(stride1, _ctx);
    kk_integer_drop(lo1, _ctx);
    kk_integer_drop(hi1, _ctx);
    kk_function_drop(f1, _ctx);
    kk_box_t _x20284 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20284, _ctx);
  }
}
 
// Returns an integer list of increasing elements from `lo`  to `hi` with stride `stride`.
// If `lo > hi`  the function returns the empty list.


// lift anonymous function
struct kk_std_core__ctailm_list_fun20288__t_3 {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t f2;
  kk_integer_t hi2;
  kk_integer_t lo2;
  kk_integer_t stride2;
};
static kk_box_t kk_std_core__ctailm_list_fun20288_3(kk_function_t _fself, kk_box_t _b_18523, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_list_fun20288_3(kk_function_t _accm0, kk_function_t f2, kk_integer_t hi2, kk_integer_t lo2, kk_integer_t stride2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_list_fun20288__t_3* _self = kk_function_alloc_as(struct kk_std_core__ctailm_list_fun20288__t_3, 6, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_list_fun20288_3, kk_context());
  _self->_accm0 = _accm0;
  _self->f2 = f2;
  _self->hi2 = hi2;
  _self->lo2 = lo2;
  _self->stride2 = stride2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_list_fun20288_3(kk_function_t _fself, kk_box_t _b_18523, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_list_fun20288__t_3* _self = kk_function_as(struct kk_std_core__ctailm_list_fun20288__t_3*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<5107>) -> list<5107> */
  kk_function_t f2 = _self->f2; /* (int) -> 5108 5107 */
  kk_integer_t hi2 = _self->hi2; /* int */
  kk_integer_t lo2 = _self->lo2; /* int */
  kk_integer_t stride2 = _self->stride2; /* int */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(f2);kk_integer_dup(hi2);kk_integer_dup(lo2);kk_integer_dup(stride2);}, {}, _ctx)
  kk_std_core__list _x20289 = kk_std_core__mlift17703_op(_accm0, f2, hi2, lo2, stride2, _b_18523, _ctx); /*list<5107>*/
  return kk_std_core__list_box(_x20289, _ctx);
}


// lift anonymous function
struct kk_std_core__ctailm_list_fun20293__t_3 {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t x0_17818;
};
static kk_std_core__list kk_std_core__ctailm_list_fun20293_3(kk_function_t _fself, kk_std_core__list _ctail_173430, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_list_fun20293_3(kk_function_t _accm0, kk_box_t x0_17818, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_list_fun20293__t_3* _self = kk_function_alloc_as(struct kk_std_core__ctailm_list_fun20293__t_3, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_list_fun20293_3, kk_context());
  _self->_accm0 = _accm0;
  _self->x0_17818 = x0_17818;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_list_fun20293_3(kk_function_t _fself, kk_std_core__list _ctail_173430, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_list_fun20293__t_3* _self = kk_function_as(struct kk_std_core__ctailm_list_fun20293__t_3*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<5107>) -> list<5107> */
  kk_box_t x0_17818 = _self->x0_17818; /* 5107 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(x0_17818);}, {}, _ctx)
  kk_std_core__list _x20294 = kk_std_core__new_Cons(kk_reuse_null, x0_17818, _ctail_173430, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x20294, _ctx));
}

kk_std_core__list kk_std_core__ctailm_list_3(kk_integer_t lo2, kk_integer_t hi2, kk_integer_t stride2, kk_function_t f2, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,e> (lo : int, hi : int, stride : int, f : (int) -> e a, (list<a>) -> list<a>) -> e list<a> */ 
  kk__tailcall: ;
  bool _match_19844 = kk_integer_lte_borrow(lo2,hi2,kk_context()); /*bool*/;
  if (_match_19844) {
    kk_box_t x0_17818;
    kk_function_t _x20286 = kk_function_dup(f2); /*(int) -> 5108 5107*/
    kk_integer_t _x20285 = kk_integer_dup(lo2); /*int*/
    x0_17818 = kk_function_call(kk_box_t, (kk_function_t, kk_integer_t, kk_context_t*), _x20286, (_x20286, _x20285, _ctx)); /*5107*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x0_17818, _ctx);
      kk_box_t _x20287 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_list_fun20288_3(_accm0, f2, hi2, lo2, stride2, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20287, _ctx);
    }
    { // tailcall
      kk_integer_t _x20290;
      kk_integer_t _x20291 = kk_integer_dup(stride2); /*int*/
      _x20290 = kk_integer_add(lo2,_x20291,kk_context()); /*int*/
      kk_function_t _x20292 = kk_std_core__new_ctailm_list_fun20293_3(_accm0, x0_17818, _ctx); /*(list<5107>) -> list<5107>*/
      lo2 = _x20290;
      _accm0 = _x20292;
      goto kk__tailcall;
    }
  }
  {
    kk_integer_drop(stride2, _ctx);
    kk_integer_drop(lo2, _ctx);
    kk_integer_drop(hi2, _ctx);
    kk_function_drop(f2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// Returns an integer list of increasing elements from `lo`  to `hi` with stride `stride`.
// If `lo > hi`  the function returns the empty list.


// lift anonymous function
struct kk_std_core_list_fun20296__t_3 {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core_list_fun20296_3(kk_function_t _fself, kk_std_core__list _ctail_17342, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_list_fun20296_3(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_list_fun20296_3, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core_list_fun20296_3(kk_function_t _fself, kk_std_core__list _ctail_17342, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17342;
}

kk_std_core__list kk_std_core_list_3(kk_integer_t lo3, kk_integer_t hi3, kk_integer_t stride3, kk_function_t f3, kk_context_t* _ctx) { /* forall<a,e> (lo : int, hi : int, stride : int, f : (int) -> e a) -> e list<a> */ 
  bool _match_19843 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19843) {
    kk_std_core_types__ctail _x20295 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_list_3(lo3, hi3, stride3, f3, _x20295, _ctx);
  }
  {
    return kk_std_core__ctailm_list_3(lo3, hi3, stride3, f3, kk_std_core_new_list_fun20296_3(_ctx), _ctx);
  }
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17704_op(kk_std_core_types__ctail _acc, kk_function_t f, kk_std_core__list xx, kk_box_t _ctail_17345, kk_context_t* _ctx) { /* forall<a,b,e> (ctail<list<b>>, f : (a) -> e b, xx : list<a>, b) -> e list<b> */ 
  kk_std_core__list _ctail_17346 = kk_std_core__list_hole(); /*list<6225>*/;
  kk_std_core__list _ctail_17347 = kk_std_core__new_Cons(kk_reuse_null, _ctail_17345, _ctail_17346, _ctx); /*list<6225>*/;
  kk_std_core_types__ctail _x20297;
  kk_box_t* _b_18534_18531 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17347)->tail)); /*cfield<list<6225>>*/;
  _x20297 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17347, _ctx)),_b_18534_18531); /*ctail<0>*/
  return kk_std_core__ctail_map_5(xx, f, _x20297, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17705_op_fun20298__t {
  struct kk_function_s _base;
  kk_function_t _accm;
  kk_box_t _ctail_17350;
};
static kk_std_core__list kk_std_core__mlift17705_op_fun20298(kk_function_t _fself, kk_std_core__list _ctail_17349, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17705_op_fun20298(kk_function_t _accm, kk_box_t _ctail_17350, kk_context_t* _ctx) {
  struct kk_std_core__mlift17705_op_fun20298__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17705_op_fun20298__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17705_op_fun20298, kk_context());
  _self->_accm = _accm;
  _self->_ctail_17350 = _ctail_17350;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__mlift17705_op_fun20298(kk_function_t _fself, kk_std_core__list _ctail_17349, kk_context_t* _ctx) {
  struct kk_std_core__mlift17705_op_fun20298__t* _self = kk_function_as(struct kk_std_core__mlift17705_op_fun20298__t*, _fself);
  kk_function_t _accm = _self->_accm; /* (list<6225>) -> list<6225> */
  kk_box_t _ctail_17350 = _self->_ctail_17350; /* 6225 */
  kk_drop_match(_self, {kk_function_dup(_accm);kk_box_dup(_ctail_17350);}, {}, _ctx)
  kk_std_core__list _x20299 = kk_std_core__new_Cons(kk_reuse_null, _ctail_17350, _ctail_17349, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, _x20299, _ctx));
}

kk_std_core__list kk_std_core__mlift17705_op(kk_function_t _accm, kk_function_t f0, kk_std_core__list xx0, kk_box_t _ctail_17350, kk_context_t* _ctx) { /* forall<a,b,e> ((list<b>) -> list<b>, f : (a) -> e b, xx : list<a>, b) -> e list<b> */ 
  return kk_std_core__ctailm_map_5(xx0, f0, kk_std_core__new_mlift17705_op_fun20298(_accm, _ctail_17350, _ctx), _ctx);
}
 
// Apply a function `f`  to each element of the input list in sequence.


// lift anonymous function
struct kk_std_core__ctail_map_fun20303__t_5 {
  struct kk_function_s _base;
  kk_function_t f1;
  kk_std_core__list xx1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_map_fun20303_5(kk_function_t _fself, kk_box_t _b_18539, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_map_fun20303_5(kk_function_t f1, kk_std_core__list xx1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_map_fun20303__t_5* _self = kk_function_alloc_as(struct kk_std_core__ctail_map_fun20303__t_5, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_map_fun20303_5, kk_context());
  _self->f1 = f1;
  _self->xx1 = xx1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_map_fun20303_5(kk_function_t _fself, kk_box_t _b_18539, kk_context_t* _ctx) {
  struct kk_std_core__ctail_map_fun20303__t_5* _self = kk_function_as(struct kk_std_core__ctail_map_fun20303__t_5*, _fself);
  kk_function_t f1 = _self->f1; /* (6224) -> 6226 6225 */
  kk_std_core__list xx1 = _self->xx1; /* list<6224> */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<6225>> */
  kk_drop_match(_self, {kk_function_dup(f1);kk_std_core__list_dup(xx1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x20304 = kk_std_core__mlift17704_op(_acc0, f1, xx1, _b_18539, _ctx); /*list<6225>*/
  return kk_std_core__list_box(_x20304, _ctx);
}

kk_std_core__list kk_std_core__ctail_map_5(kk_std_core__list xs, kk_function_t f1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, f : (a) -> e b, ctail<list<b>>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20300 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20300->head;
    kk_std_core__list xx1 = _con20300->tail;
    kk_reuse_t _ru_19953 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      _ru_19953 = (kk_std_core__list_reuse(xs));
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx1);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_box_t x0_17821;
    kk_function_t _x20301 = kk_function_dup(f1); /*(6224) -> 6226 6225*/
    x0_17821 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20301, (_x20301, x, _ctx)); /*6225*/
    if (kk_yielding(kk_context())) {
      kk_reuse_drop(_ru_19953);
      kk_box_drop(x0_17821, _ctx);
      kk_box_t _x20302 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_map_fun20303_5(f1, xx1, _acc0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20302, _ctx);
    }
    {
      kk_std_core__list _ctail_173460 = kk_std_core__list_hole(); /*list<6225>*/;
      kk_std_core__list _ctail_173470 = kk_std_core__new_Cons(_ru_19953, x0_17821, _ctail_173460, _ctx); /*list<6225>*/;
      { // tailcall
        kk_std_core_types__ctail _x20305;
        kk_box_t* _b_18551_18545 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_173470)->tail)); /*cfield<list<6225>>*/;
        _x20305 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_173470, _ctx)),_b_18551_18545); /*ctail<0>*/
        xs = xx1;
        _acc0 = _x20305;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_function_drop(f1, _ctx);
    kk_box_t _x20306 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20306, _ctx);
  }
}
 
// Apply a function `f`  to each element of the input list in sequence.


// lift anonymous function
struct kk_std_core__ctailm_map_fun20310__t_5 {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t f2;
  kk_std_core__list xx2;
};
static kk_box_t kk_std_core__ctailm_map_fun20310_5(kk_function_t _fself, kk_box_t _b_18559, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_map_fun20310_5(kk_function_t _accm0, kk_function_t f2, kk_std_core__list xx2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_map_fun20310__t_5* _self = kk_function_alloc_as(struct kk_std_core__ctailm_map_fun20310__t_5, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_map_fun20310_5, kk_context());
  _self->_accm0 = _accm0;
  _self->f2 = f2;
  _self->xx2 = xx2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_map_fun20310_5(kk_function_t _fself, kk_box_t _b_18559, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_map_fun20310__t_5* _self = kk_function_as(struct kk_std_core__ctailm_map_fun20310__t_5*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<6225>) -> list<6225> */
  kk_function_t f2 = _self->f2; /* (6224) -> 6226 6225 */
  kk_std_core__list xx2 = _self->xx2; /* list<6224> */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(f2);kk_std_core__list_dup(xx2);}, {}, _ctx)
  kk_std_core__list _x20311 = kk_std_core__mlift17705_op(_accm0, f2, xx2, _b_18559, _ctx); /*list<6225>*/
  return kk_std_core__list_box(_x20311, _ctx);
}


// lift anonymous function
struct kk_std_core__ctailm_map_fun20313__t_5 {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t x2_17824;
};
static kk_std_core__list kk_std_core__ctailm_map_fun20313_5(kk_function_t _fself, kk_std_core__list _ctail_173490, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_map_fun20313_5(kk_function_t _accm0, kk_box_t x2_17824, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_map_fun20313__t_5* _self = kk_function_alloc_as(struct kk_std_core__ctailm_map_fun20313__t_5, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_map_fun20313_5, kk_context());
  _self->_accm0 = _accm0;
  _self->x2_17824 = x2_17824;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_map_fun20313_5(kk_function_t _fself, kk_std_core__list _ctail_173490, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_map_fun20313__t_5* _self = kk_function_as(struct kk_std_core__ctailm_map_fun20313__t_5*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<6225>) -> list<6225> */
  kk_box_t x2_17824 = _self->x2_17824; /* 6225 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(x2_17824);}, {}, _ctx)
  kk_std_core__list _x20314 = kk_std_core__new_Cons(kk_reuse_null, x2_17824, _ctail_173490, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x20314, _ctx));
}

kk_std_core__list kk_std_core__ctailm_map_5(kk_std_core__list xs0, kk_function_t f2, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, f : (a) -> e b, (list<b>) -> list<b>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs0)) {
    struct kk_std_core_Cons* _con20307 = kk_std_core__as_Cons(xs0);
    kk_box_t x1 = _con20307->head;
    kk_std_core__list xx2 = _con20307->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs0))) {
      kk_std_core__list_free(xs0);
    }
    else {
      kk_box_dup(x1);
      kk_std_core__list_dup(xx2);
      kk_std_core__list_decref(xs0, _ctx);
    }
    kk_box_t x2_17824;
    kk_function_t _x20308 = kk_function_dup(f2); /*(6224) -> 6226 6225*/
    x2_17824 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20308, (_x20308, x1, _ctx)); /*6225*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x2_17824, _ctx);
      kk_box_t _x20309 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_map_fun20310_5(_accm0, f2, xx2, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20309, _ctx);
    }
    { // tailcall
      kk_function_t _x20312 = kk_std_core__new_ctailm_map_fun20313_5(_accm0, x2_17824, _ctx); /*(list<6225>) -> list<6225>*/
      xs0 = xx2;
      _accm0 = _x20312;
      goto kk__tailcall;
    }
  }
  {
    kk_function_drop(f2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// Apply a function `f`  to each element of the input list in sequence.


// lift anonymous function
struct kk_std_core_map_fun20316__t_5 {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core_map_fun20316_5(kk_function_t _fself, kk_std_core__list _ctail_17348, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20316_5(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_map_fun20316_5, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core_map_fun20316_5(kk_function_t _fself, kk_std_core__list _ctail_17348, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17348;
}

kk_std_core__list kk_std_core_map_5(kk_std_core__list xs1, kk_function_t f3, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, f : (a) -> e b) -> e list<b> */ 
  bool _match_19840 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19840) {
    kk_std_core_types__ctail _x20315 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_map_5(xs1, f3, _x20315, _ctx);
  }
  {
    return kk_std_core__ctailm_map_5(xs1, f3, kk_std_core_new_map_fun20316_5(_ctx), _ctx);
  }
}
 
// Create a list of characters from `lo`  to `hi`  (inclusive).


// lift anonymous function
struct kk_std_core_list_fun20319__t_4 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_list_fun20319_4(kk_function_t _fself, kk_box_t _b_18564, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_list_fun20319_4(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_list_fun20319_4, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_list_fun20319_4(kk_function_t _fself, kk_box_t _b_18564, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_char_t _x20320;
  kk_integer_t _brw_19838 = kk_integer_unbox(_b_18564); /*int*/;
  kk_char_t _brw_19839 = kk_integer_clamp32_borrow(_brw_19838); /*char*/;
  kk_integer_drop(_brw_19838, _ctx);
  _x20320 = _brw_19839; /*char*/
  return kk_char_box(_x20320, _ctx);
}

kk_std_core__list kk_std_core_list_4(kk_char_t lo, kk_char_t hi, kk_context_t* _ctx) { /* (lo : char, hi : char) -> total list<char> */ 
  kk_std_core__list _b_18565_18562;
  kk_integer_t _x20317 = kk_integer_from_int(lo,kk_context()); /*int*/
  kk_integer_t _x20318 = kk_integer_from_int(hi,kk_context()); /*int*/
  _b_18565_18562 = kk_std_core_list(_x20317, _x20318, _ctx); /*list<int>*/
  return kk_std_core_map_5(_b_18565_18562, kk_std_core_new_list_fun20319_4(_ctx), _ctx);
}
 
// Convert a string to a list of characters

kk_std_core__list kk_std_core_list_6(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> total list<char> */ 
  return kk_string_to_list(s,kk_context());
}


// lift anonymous function
struct kk_std_core_map_fun20322__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_map_fun20322(kk_function_t _fself, kk_box_t _b_18568, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20322(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_map_fun20322, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_map_fun20322(kk_function_t _fself, kk_box_t _b_18568, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core_types__maybe _x20323 = kk_std_core_types__new_Just(_b_18568, _ctx); /*maybe<105>*/
  return kk_std_core_types__maybe_box(_x20323, _ctx);
}

kk_std_core_types__maybe kk_std_core_map(kk_std_core_types__maybe m, kk_function_t f, kk_context_t* _ctx) { /* forall<a,b,e> (m : maybe<a>, f : (a) -> e b) -> e maybe<b> */ 
  if (kk_std_core_types__is_Nothing(m)) {
    kk_function_drop(f, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
  {
    kk_box_t x = m._cons.Just.value;
    kk_box_t x0_17827 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, x, _ctx)); /*5405*/;
    if (kk_yielding(kk_context())) {
      kk_box_drop(x0_17827, _ctx);
      kk_box_t _x20321 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20322(_ctx), _ctx); /*3860*/
      return kk_std_core_types__maybe_unbox(_x20321, _ctx);
    }
    {
      return kk_std_core_types__new_Just(x0_17827, _ctx);
    }
  }
}
 
// Map over the `Right` component of an `:either` type.


// lift anonymous function
struct kk_std_core_map_fun20325__t_1 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_map_fun20325_1(kk_function_t _fself, kk_box_t _b_18572, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20325_1(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_map_fun20325_1, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_map_fun20325_1(kk_function_t _fself, kk_box_t _b_18572, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core_types__either _x20326 = kk_std_core_types__new_Right(_b_18572, _ctx); /*either<72,73>*/
  return kk_std_core_types__either_box(_x20326, _ctx);
}

kk_std_core_types__either kk_std_core_map_1(kk_std_core_types__either e, kk_function_t f, kk_context_t* _ctx) { /* forall<a,b,c,e> (e : either<a,b>, f : (b) -> e c) -> e either<a,c> */ 
  if (kk_std_core_types__is_Right(e)) {
    kk_box_t x = e._cons.Right.right;
    kk_box_t x0_17831 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, x, _ctx)); /*5443*/;
    if (kk_yielding(kk_context())) {
      kk_box_drop(x0_17831, _ctx);
      kk_box_t _x20324 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20325_1(_ctx), _ctx); /*3860*/
      return kk_std_core_types__either_unbox(_x20324, _ctx);
    }
    {
      return kk_std_core_types__new_Right(x0_17831, _ctx);
    }
  }
  {
    kk_box_t x00 = e._cons.Left.left;
    kk_function_drop(f, _ctx);
    return kk_std_core_types__new_Left(x00, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17709_map_fun20329__t_2 {
  struct kk_function_s _base;
  kk_box_t _y_17461;
};
static kk_box_t kk_std_core__mlift17709_map_fun20329_2(kk_function_t _fself, kk_box_t _b_18576, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17709_map_fun20329_2(kk_box_t _y_17461, kk_context_t* _ctx) {
  struct kk_std_core__mlift17709_map_fun20329__t_2* _self = kk_function_alloc_as(struct kk_std_core__mlift17709_map_fun20329__t_2, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17709_map_fun20329_2, kk_context());
  _self->_y_17461 = _y_17461;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17709_map_fun20329_2(kk_function_t _fself, kk_box_t _b_18576, kk_context_t* _ctx) {
  struct kk_std_core__mlift17709_map_fun20329__t_2* _self = kk_function_as(struct kk_std_core__mlift17709_map_fun20329__t_2*, _fself);
  kk_box_t _y_17461 = _self->_y_17461; /* 5588 */
  kk_drop_match(_self, {kk_box_dup(_y_17461);}, {}, _ctx)
  kk_std_core_types__tuple2_ _x20330 = kk_std_core_types__new_dash__lp__comma__rp_(_y_17461, _b_18576, _ctx); /*(6, 7)*/
  return kk_std_core_types__tuple2__box(_x20330, _ctx);
}

kk_std_core_types__tuple2_ kk_std_core__mlift17709_map_2(kk_function_t f, kk_std_core_types__tuple2_ t, kk_box_t _y_17461, kk_context_t* _ctx) { /* forall<a,b,e> (f : (a) -> e b, t : (a, a), b) -> e (b, b) */ 
  kk_box_t x_17835;
  kk_box_t _x20327;
  {
    kk_box_t _x = t.snd;
    kk_box_dup(_x);
    kk_std_core_types__tuple2__drop(t, _ctx);
    _x20327 = _x; /*5587*/
  }
  x_17835 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, _x20327, _ctx)); /*5588*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(x_17835, _ctx);
    kk_box_t _x20328 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17709_map_fun20329_2(_y_17461, _ctx), _ctx); /*3860*/
    return kk_std_core_types__tuple2__unbox(_x20328, _ctx);
  }
  {
    return kk_std_core_types__new_dash__lp__comma__rp_(_y_17461, x_17835, _ctx);
  }
}


// lift anonymous function
struct kk_std_core_map_fun20334__t_2 {
  struct kk_function_s _base;
  kk_function_t f;
  kk_std_core_types__tuple2_ t;
};
static kk_box_t kk_std_core_map_fun20334_2(kk_function_t _fself, kk_box_t _b_18580, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20334_2(kk_function_t f, kk_std_core_types__tuple2_ t, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20334__t_2* _self = kk_function_alloc_as(struct kk_std_core_map_fun20334__t_2, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20334_2, kk_context());
  _self->f = f;
  _self->t = t;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20334_2(kk_function_t _fself, kk_box_t _b_18580, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20334__t_2* _self = kk_function_as(struct kk_std_core_map_fun20334__t_2*, _fself);
  kk_function_t f = _self->f; /* (5587) -> 5589 5588 */
  kk_std_core_types__tuple2_ t = _self->t; /* (5587, 5587) */
  kk_drop_match(_self, {kk_function_dup(f);kk_std_core_types__tuple2__dup(t);}, {}, _ctx)
  kk_std_core_types__tuple2_ _x20335 = kk_std_core__mlift17709_map_2(f, t, _b_18580, _ctx); /*(5588, 5588)*/
  return kk_std_core_types__tuple2__box(_x20335, _ctx);
}


// lift anonymous function
struct kk_std_core_map_fun20338__t_2 {
  struct kk_function_s _base;
  kk_box_t x_17840;
};
static kk_box_t kk_std_core_map_fun20338_2(kk_function_t _fself, kk_box_t _b_18582, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20338_2(kk_box_t x_17840, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20338__t_2* _self = kk_function_alloc_as(struct kk_std_core_map_fun20338__t_2, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20338_2, kk_context());
  _self->x_17840 = x_17840;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20338_2(kk_function_t _fself, kk_box_t _b_18582, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20338__t_2* _self = kk_function_as(struct kk_std_core_map_fun20338__t_2*, _fself);
  kk_box_t x_17840 = _self->x_17840; /* 5588 */
  kk_drop_match(_self, {kk_box_dup(x_17840);}, {}, _ctx)
  kk_std_core_types__tuple2_ _x20339 = kk_std_core_types__new_dash__lp__comma__rp_(x_17840, _b_18582, _ctx); /*(6, 7)*/
  return kk_std_core_types__tuple2__box(_x20339, _ctx);
}

kk_std_core_types__tuple2_ kk_std_core_map_2(kk_std_core_types__tuple2_ t, kk_function_t f, kk_context_t* _ctx) { /* forall<a,b,e> (t : (a, a), f : (a) -> e b) -> e (b, b) */ 
  kk_box_t x_17840;
  kk_function_t _x20332 = kk_function_dup(f); /*(5587) -> 5589 5588*/
  kk_box_t _x20331;
  {
    kk_box_t _x = t.fst;
    kk_box_dup(_x);
    _x20331 = _x; /*5587*/
  }
  x_17840 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20332, (_x20332, _x20331, _ctx)); /*5588*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(x_17840, _ctx);
    kk_box_t _x20333 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20334_2(f, t, _ctx), _ctx); /*3860*/
    return kk_std_core_types__tuple2__unbox(_x20333, _ctx);
  }
  {
    kk_box_t x0_17844;
    kk_box_t _x20336;
    {
      kk_box_t _x0 = t.snd;
      kk_box_dup(_x0);
      kk_std_core_types__tuple2__drop(t, _ctx);
      _x20336 = _x0; /*5587*/
    }
    x0_17844 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, _x20336, _ctx)); /*5588*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x0_17844, _ctx);
      kk_box_t _x20337 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20338_2(x_17840, _ctx), _ctx); /*3860*/
      return kk_std_core_types__tuple2__unbox(_x20337, _ctx);
    }
    {
      return kk_std_core_types__new_dash__lp__comma__rp_(x_17840, x0_17844, _ctx);
    }
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17711_map_fun20342__t_3 {
  struct kk_function_s _base;
  kk_box_t _y_17463;
  kk_box_t _y_17464;
};
static kk_box_t kk_std_core__mlift17711_map_fun20342_3(kk_function_t _fself, kk_box_t _b_18588, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17711_map_fun20342_3(kk_box_t _y_17463, kk_box_t _y_17464, kk_context_t* _ctx) {
  struct kk_std_core__mlift17711_map_fun20342__t_3* _self = kk_function_alloc_as(struct kk_std_core__mlift17711_map_fun20342__t_3, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17711_map_fun20342_3, kk_context());
  _self->_y_17463 = _y_17463;
  _self->_y_17464 = _y_17464;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17711_map_fun20342_3(kk_function_t _fself, kk_box_t _b_18588, kk_context_t* _ctx) {
  struct kk_std_core__mlift17711_map_fun20342__t_3* _self = kk_function_as(struct kk_std_core__mlift17711_map_fun20342__t_3*, _fself);
  kk_box_t _y_17463 = _self->_y_17463; /* 5806 */
  kk_box_t _y_17464 = _self->_y_17464; /* 5806 */
  kk_drop_match(_self, {kk_box_dup(_y_17463);kk_box_dup(_y_17464);}, {}, _ctx)
  kk_std_core_types__tuple3_ _x20343 = kk_std_core_types__new_dash__lp__comma__comma__rp_(_y_17463, _y_17464, _b_18588, _ctx); /*(13, 14, 15)*/
  return kk_std_core_types__tuple3__box(_x20343, _ctx);
}

kk_std_core_types__tuple3_ kk_std_core__mlift17711_map_3(kk_box_t _y_17463, kk_function_t f, kk_std_core_types__tuple3_ t, kk_box_t _y_17464, kk_context_t* _ctx) { /* forall<a,b,e> (b, f : (a) -> e b, t : (a, a, a), b) -> e (b, b, b) */ 
  kk_box_t x_17850;
  kk_box_t _x20340;
  {
    kk_box_t _x = t.thd;
    kk_box_dup(_x);
    kk_std_core_types__tuple3__drop(t, _ctx);
    _x20340 = _x; /*5805*/
  }
  x_17850 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, _x20340, _ctx)); /*5806*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(x_17850, _ctx);
    kk_box_t _x20341 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17711_map_fun20342_3(_y_17463, _y_17464, _ctx), _ctx); /*3860*/
    return kk_std_core_types__tuple3__unbox(_x20341, _ctx);
  }
  {
    return kk_std_core_types__new_dash__lp__comma__comma__rp_(_y_17463, _y_17464, x_17850, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17712_map_fun20347__t_3 {
  struct kk_function_s _base;
  kk_box_t _y_17463;
  kk_function_t f;
  kk_std_core_types__tuple3_ t;
};
static kk_box_t kk_std_core__mlift17712_map_fun20347_3(kk_function_t _fself, kk_box_t _b_18592, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17712_map_fun20347_3(kk_box_t _y_17463, kk_function_t f, kk_std_core_types__tuple3_ t, kk_context_t* _ctx) {
  struct kk_std_core__mlift17712_map_fun20347__t_3* _self = kk_function_alloc_as(struct kk_std_core__mlift17712_map_fun20347__t_3, 6, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17712_map_fun20347_3, kk_context());
  _self->_y_17463 = _y_17463;
  _self->f = f;
  _self->t = t;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17712_map_fun20347_3(kk_function_t _fself, kk_box_t _b_18592, kk_context_t* _ctx) {
  struct kk_std_core__mlift17712_map_fun20347__t_3* _self = kk_function_as(struct kk_std_core__mlift17712_map_fun20347__t_3*, _fself);
  kk_box_t _y_17463 = _self->_y_17463; /* 5806 */
  kk_function_t f = _self->f; /* (5805) -> 5807 5806 */
  kk_std_core_types__tuple3_ t = _self->t; /* (5805, 5805, 5805) */
  kk_drop_match(_self, {kk_box_dup(_y_17463);kk_function_dup(f);kk_std_core_types__tuple3__dup(t);}, {}, _ctx)
  kk_std_core_types__tuple3_ _x20348 = kk_std_core__mlift17711_map_3(_y_17463, f, t, _b_18592, _ctx); /*(5806, 5806, 5806)*/
  return kk_std_core_types__tuple3__box(_x20348, _ctx);
}

kk_std_core_types__tuple3_ kk_std_core__mlift17712_map_3(kk_function_t f, kk_std_core_types__tuple3_ t, kk_box_t _y_17463, kk_context_t* _ctx) { /* forall<a,b,e> (f : (a) -> e b, t : (a, a, a), b) -> e (b, b, b) */ 
  kk_box_t x_17856;
  kk_function_t _x20345 = kk_function_dup(f); /*(5805) -> 5807 5806*/
  kk_box_t _x20344;
  {
    kk_box_t _x = t.snd;
    kk_box_dup(_x);
    _x20344 = _x; /*5805*/
  }
  x_17856 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20345, (_x20345, _x20344, _ctx)); /*5806*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(x_17856, _ctx);
    kk_box_t _x20346 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17712_map_fun20347_3(_y_17463, f, t, _ctx), _ctx); /*3860*/
    return kk_std_core_types__tuple3__unbox(_x20346, _ctx);
  }
  {
    return kk_std_core__mlift17711_map_3(_y_17463, f, t, x_17856, _ctx);
  }
}


// lift anonymous function
struct kk_std_core_map_fun20352__t_3 {
  struct kk_function_s _base;
  kk_function_t f;
  kk_std_core_types__tuple3_ t;
};
static kk_box_t kk_std_core_map_fun20352_3(kk_function_t _fself, kk_box_t _b_18596, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20352_3(kk_function_t f, kk_std_core_types__tuple3_ t, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20352__t_3* _self = kk_function_alloc_as(struct kk_std_core_map_fun20352__t_3, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20352_3, kk_context());
  _self->f = f;
  _self->t = t;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20352_3(kk_function_t _fself, kk_box_t _b_18596, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20352__t_3* _self = kk_function_as(struct kk_std_core_map_fun20352__t_3*, _fself);
  kk_function_t f = _self->f; /* (5805) -> 5807 5806 */
  kk_std_core_types__tuple3_ t = _self->t; /* (5805, 5805, 5805) */
  kk_drop_match(_self, {kk_function_dup(f);kk_std_core_types__tuple3__dup(t);}, {}, _ctx)
  kk_std_core_types__tuple3_ _x20353 = kk_std_core__mlift17712_map_3(f, t, _b_18596, _ctx); /*(5806, 5806, 5806)*/
  return kk_std_core_types__tuple3__box(_x20353, _ctx);
}


// lift anonymous function
struct kk_std_core_map_fun20357__t_3 {
  struct kk_function_s _base;
  kk_function_t f;
  kk_std_core_types__tuple3_ t;
  kk_box_t x_17859;
};
static kk_box_t kk_std_core_map_fun20357_3(kk_function_t _fself, kk_box_t _b_18598, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20357_3(kk_function_t f, kk_std_core_types__tuple3_ t, kk_box_t x_17859, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20357__t_3* _self = kk_function_alloc_as(struct kk_std_core_map_fun20357__t_3, 6, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20357_3, kk_context());
  _self->f = f;
  _self->t = t;
  _self->x_17859 = x_17859;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20357_3(kk_function_t _fself, kk_box_t _b_18598, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20357__t_3* _self = kk_function_as(struct kk_std_core_map_fun20357__t_3*, _fself);
  kk_function_t f = _self->f; /* (5805) -> 5807 5806 */
  kk_std_core_types__tuple3_ t = _self->t; /* (5805, 5805, 5805) */
  kk_box_t x_17859 = _self->x_17859; /* 5806 */
  kk_drop_match(_self, {kk_function_dup(f);kk_std_core_types__tuple3__dup(t);kk_box_dup(x_17859);}, {}, _ctx)
  kk_std_core_types__tuple3_ _x20358 = kk_std_core__mlift17711_map_3(x_17859, f, t, _b_18598, _ctx); /*(5806, 5806, 5806)*/
  return kk_std_core_types__tuple3__box(_x20358, _ctx);
}


// lift anonymous function
struct kk_std_core_map_fun20361__t_3 {
  struct kk_function_s _base;
  kk_box_t x_17859;
  kk_box_t x0_17863;
};
static kk_box_t kk_std_core_map_fun20361_3(kk_function_t _fself, kk_box_t _b_18600, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20361_3(kk_box_t x_17859, kk_box_t x0_17863, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20361__t_3* _self = kk_function_alloc_as(struct kk_std_core_map_fun20361__t_3, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20361_3, kk_context());
  _self->x_17859 = x_17859;
  _self->x0_17863 = x0_17863;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20361_3(kk_function_t _fself, kk_box_t _b_18600, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20361__t_3* _self = kk_function_as(struct kk_std_core_map_fun20361__t_3*, _fself);
  kk_box_t x_17859 = _self->x_17859; /* 5806 */
  kk_box_t x0_17863 = _self->x0_17863; /* 5806 */
  kk_drop_match(_self, {kk_box_dup(x_17859);kk_box_dup(x0_17863);}, {}, _ctx)
  kk_std_core_types__tuple3_ _x20362 = kk_std_core_types__new_dash__lp__comma__comma__rp_(x_17859, x0_17863, _b_18600, _ctx); /*(13, 14, 15)*/
  return kk_std_core_types__tuple3__box(_x20362, _ctx);
}

kk_std_core_types__tuple3_ kk_std_core_map_3(kk_std_core_types__tuple3_ t, kk_function_t f, kk_context_t* _ctx) { /* forall<a,b,e> (t : (a, a, a), f : (a) -> e b) -> e (b, b, b) */ 
  kk_box_t x_17859;
  kk_function_t _x20350 = kk_function_dup(f); /*(5805) -> 5807 5806*/
  kk_box_t _x20349;
  {
    kk_box_t _x = t.fst;
    kk_box_dup(_x);
    _x20349 = _x; /*5805*/
  }
  x_17859 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20350, (_x20350, _x20349, _ctx)); /*5806*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(x_17859, _ctx);
    kk_box_t _x20351 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20352_3(f, t, _ctx), _ctx); /*3860*/
    return kk_std_core_types__tuple3__unbox(_x20351, _ctx);
  }
  {
    kk_box_t x0_17863;
    kk_function_t _x20355 = kk_function_dup(f); /*(5805) -> 5807 5806*/
    kk_box_t _x20354;
    {
      kk_box_t _x0 = t.snd;
      kk_box_dup(_x0);
      _x20354 = _x0; /*5805*/
    }
    x0_17863 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20355, (_x20355, _x20354, _ctx)); /*5806*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x0_17863, _ctx);
      kk_box_t _x20356 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20357_3(f, t, x_17859, _ctx), _ctx); /*3860*/
      return kk_std_core_types__tuple3__unbox(_x20356, _ctx);
    }
    {
      kk_box_t x1_17867;
      kk_box_t _x20359;
      {
        kk_box_t _x1 = t.thd;
        kk_box_dup(_x1);
        kk_std_core_types__tuple3__drop(t, _ctx);
        _x20359 = _x1; /*5805*/
      }
      x1_17867 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, _x20359, _ctx)); /*5806*/
      if (kk_yielding(kk_context())) {
        kk_box_drop(x1_17867, _ctx);
        kk_box_t _x20360 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20361_3(x_17859, x0_17863, _ctx), _ctx); /*3860*/
        return kk_std_core_types__tuple3__unbox(_x20360, _ctx);
      }
      {
        return kk_std_core_types__new_dash__lp__comma__comma__rp_(x_17859, x0_17863, x1_17867, _ctx);
      }
    }
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17714_map_fun20366__t_4 {
  struct kk_function_s _base;
  kk_box_t _y_17466;
  kk_box_t _y_17467;
  kk_box_t _y_17468;
};
static kk_box_t kk_std_core__mlift17714_map_fun20366_4(kk_function_t _fself, kk_box_t _b_18608, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17714_map_fun20366_4(kk_box_t _y_17466, kk_box_t _y_17467, kk_box_t _y_17468, kk_context_t* _ctx) {
  struct kk_std_core__mlift17714_map_fun20366__t_4* _self = kk_function_alloc_as(struct kk_std_core__mlift17714_map_fun20366__t_4, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17714_map_fun20366_4, kk_context());
  _self->_y_17466 = _y_17466;
  _self->_y_17467 = _y_17467;
  _self->_y_17468 = _y_17468;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17714_map_fun20366_4(kk_function_t _fself, kk_box_t _b_18608, kk_context_t* _ctx) {
  struct kk_std_core__mlift17714_map_fun20366__t_4* _self = kk_function_as(struct kk_std_core__mlift17714_map_fun20366__t_4*, _fself);
  kk_box_t _y_17466 = _self->_y_17466; /* 6093 */
  kk_box_t _y_17467 = _self->_y_17467; /* 6093 */
  kk_box_t _y_17468 = _self->_y_17468; /* 6093 */
  kk_drop_match(_self, {kk_box_dup(_y_17466);kk_box_dup(_y_17467);kk_box_dup(_y_17468);}, {}, _ctx)
  kk_std_core_types__tuple4_ _x20367 = kk_std_core_types__new_dash__lp__comma__comma__comma__rp_(kk_reuse_null, _y_17466, _y_17467, _y_17468, _b_18608, _ctx); /*(22, 23, 24, 25)*/
  return kk_std_core_types__tuple4__box(_x20367, _ctx);
}

kk_std_core_types__tuple4_ kk_std_core__mlift17714_map_4(kk_box_t _y_17466, kk_box_t _y_17467, kk_function_t f, kk_std_core_types__tuple4_ t, kk_box_t _y_17468, kk_context_t* _ctx) { /* forall<a,b,e> (b, b, f : (a) -> e b, t : (a, a, a, a), b) -> e (b, b, b, b) */ 
  kk_box_t x_17874;
  kk_box_t _x20363;
  {
    struct kk_std_core_types__Tuple4_* _con20364 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(t);
    kk_box_t _pat00 = _con20364->fst;
    kk_box_t _pat10 = _con20364->snd;
    kk_box_t _pat2 = _con20364->thd;
    kk_box_t _x = _con20364->field4;
    if (kk_likely(kk_std_core_types__tuple4__is_unique(t))) {
      kk_box_drop(_pat2, _ctx);
      kk_box_drop(_pat10, _ctx);
      kk_box_drop(_pat00, _ctx);
      kk_std_core_types__tuple4__free(t);
    }
    else {
      kk_box_dup(_x);
      kk_std_core_types__tuple4__decref(t, _ctx);
    }
    _x20363 = _x; /*6092*/
  }
  x_17874 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, _x20363, _ctx)); /*6093*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(x_17874, _ctx);
    kk_box_t _x20365 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17714_map_fun20366_4(_y_17466, _y_17467, _y_17468, _ctx), _ctx); /*3860*/
    return kk_std_core_types__tuple4__unbox(_x20365, _ctx);
  }
  {
    return kk_std_core_types__new_dash__lp__comma__comma__comma__rp_(kk_reuse_null, _y_17466, _y_17467, _y_17468, x_17874, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17715_map_fun20372__t_4 {
  struct kk_function_s _base;
  kk_box_t _y_17466;
  kk_box_t _y_17467;
  kk_function_t f;
  kk_std_core_types__tuple4_ t;
};
static kk_box_t kk_std_core__mlift17715_map_fun20372_4(kk_function_t _fself, kk_box_t _b_18612, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17715_map_fun20372_4(kk_box_t _y_17466, kk_box_t _y_17467, kk_function_t f, kk_std_core_types__tuple4_ t, kk_context_t* _ctx) {
  struct kk_std_core__mlift17715_map_fun20372__t_4* _self = kk_function_alloc_as(struct kk_std_core__mlift17715_map_fun20372__t_4, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17715_map_fun20372_4, kk_context());
  _self->_y_17466 = _y_17466;
  _self->_y_17467 = _y_17467;
  _self->f = f;
  _self->t = t;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17715_map_fun20372_4(kk_function_t _fself, kk_box_t _b_18612, kk_context_t* _ctx) {
  struct kk_std_core__mlift17715_map_fun20372__t_4* _self = kk_function_as(struct kk_std_core__mlift17715_map_fun20372__t_4*, _fself);
  kk_box_t _y_17466 = _self->_y_17466; /* 6093 */
  kk_box_t _y_17467 = _self->_y_17467; /* 6093 */
  kk_function_t f = _self->f; /* (6092) -> 6094 6093 */
  kk_std_core_types__tuple4_ t = _self->t; /* (6092, 6092, 6092, 6092) */
  kk_drop_match(_self, {kk_box_dup(_y_17466);kk_box_dup(_y_17467);kk_function_dup(f);kk_std_core_types__tuple4__dup(t);}, {}, _ctx)
  kk_std_core_types__tuple4_ _x20373 = kk_std_core__mlift17714_map_4(_y_17466, _y_17467, f, t, _b_18612, _ctx); /*(6093, 6093, 6093, 6093)*/
  return kk_std_core_types__tuple4__box(_x20373, _ctx);
}

kk_std_core_types__tuple4_ kk_std_core__mlift17715_map_4(kk_box_t _y_17466, kk_function_t f, kk_std_core_types__tuple4_ t, kk_box_t _y_17467, kk_context_t* _ctx) { /* forall<a,b,e> (b, f : (a) -> e b, t : (a, a, a, a), b) -> e (b, b, b, b) */ 
  kk_box_t x_17881;
  kk_function_t _x20370 = kk_function_dup(f); /*(6092) -> 6094 6093*/
  kk_box_t _x20368;
  {
    struct kk_std_core_types__Tuple4_* _con20369 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(t);
    kk_box_t _x = _con20369->thd;
    kk_box_dup(_x);
    _x20368 = _x; /*6092*/
  }
  x_17881 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20370, (_x20370, _x20368, _ctx)); /*6093*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(x_17881, _ctx);
    kk_box_t _x20371 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17715_map_fun20372_4(_y_17466, _y_17467, f, t, _ctx), _ctx); /*3860*/
    return kk_std_core_types__tuple4__unbox(_x20371, _ctx);
  }
  {
    return kk_std_core__mlift17714_map_4(_y_17466, _y_17467, f, t, x_17881, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17716_map_fun20378__t_4 {
  struct kk_function_s _base;
  kk_box_t _y_17466;
  kk_function_t f;
  kk_std_core_types__tuple4_ t;
};
static kk_box_t kk_std_core__mlift17716_map_fun20378_4(kk_function_t _fself, kk_box_t _b_18616, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17716_map_fun20378_4(kk_box_t _y_17466, kk_function_t f, kk_std_core_types__tuple4_ t, kk_context_t* _ctx) {
  struct kk_std_core__mlift17716_map_fun20378__t_4* _self = kk_function_alloc_as(struct kk_std_core__mlift17716_map_fun20378__t_4, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17716_map_fun20378_4, kk_context());
  _self->_y_17466 = _y_17466;
  _self->f = f;
  _self->t = t;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17716_map_fun20378_4(kk_function_t _fself, kk_box_t _b_18616, kk_context_t* _ctx) {
  struct kk_std_core__mlift17716_map_fun20378__t_4* _self = kk_function_as(struct kk_std_core__mlift17716_map_fun20378__t_4*, _fself);
  kk_box_t _y_17466 = _self->_y_17466; /* 6093 */
  kk_function_t f = _self->f; /* (6092) -> 6094 6093 */
  kk_std_core_types__tuple4_ t = _self->t; /* (6092, 6092, 6092, 6092) */
  kk_drop_match(_self, {kk_box_dup(_y_17466);kk_function_dup(f);kk_std_core_types__tuple4__dup(t);}, {}, _ctx)
  kk_std_core_types__tuple4_ _x20379 = kk_std_core__mlift17715_map_4(_y_17466, f, t, _b_18616, _ctx); /*(6093, 6093, 6093, 6093)*/
  return kk_std_core_types__tuple4__box(_x20379, _ctx);
}

kk_std_core_types__tuple4_ kk_std_core__mlift17716_map_4(kk_function_t f, kk_std_core_types__tuple4_ t, kk_box_t _y_17466, kk_context_t* _ctx) { /* forall<a,b,e> (f : (a) -> e b, t : (a, a, a, a), b) -> e (b, b, b, b) */ 
  kk_box_t x_17884;
  kk_function_t _x20376 = kk_function_dup(f); /*(6092) -> 6094 6093*/
  kk_box_t _x20374;
  {
    struct kk_std_core_types__Tuple4_* _con20375 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(t);
    kk_box_t _x = _con20375->snd;
    kk_box_dup(_x);
    _x20374 = _x; /*6092*/
  }
  x_17884 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20376, (_x20376, _x20374, _ctx)); /*6093*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(x_17884, _ctx);
    kk_box_t _x20377 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17716_map_fun20378_4(_y_17466, f, t, _ctx), _ctx); /*3860*/
    return kk_std_core_types__tuple4__unbox(_x20377, _ctx);
  }
  {
    return kk_std_core__mlift17715_map_4(_y_17466, f, t, x_17884, _ctx);
  }
}


// lift anonymous function
struct kk_std_core_map_fun20384__t_4 {
  struct kk_function_s _base;
  kk_function_t f;
  kk_std_core_types__tuple4_ t;
};
static kk_box_t kk_std_core_map_fun20384_4(kk_function_t _fself, kk_box_t _b_18620, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20384_4(kk_function_t f, kk_std_core_types__tuple4_ t, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20384__t_4* _self = kk_function_alloc_as(struct kk_std_core_map_fun20384__t_4, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20384_4, kk_context());
  _self->f = f;
  _self->t = t;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20384_4(kk_function_t _fself, kk_box_t _b_18620, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20384__t_4* _self = kk_function_as(struct kk_std_core_map_fun20384__t_4*, _fself);
  kk_function_t f = _self->f; /* (6092) -> 6094 6093 */
  kk_std_core_types__tuple4_ t = _self->t; /* (6092, 6092, 6092, 6092) */
  kk_drop_match(_self, {kk_function_dup(f);kk_std_core_types__tuple4__dup(t);}, {}, _ctx)
  kk_std_core_types__tuple4_ _x20385 = kk_std_core__mlift17716_map_4(f, t, _b_18620, _ctx); /*(6093, 6093, 6093, 6093)*/
  return kk_std_core_types__tuple4__box(_x20385, _ctx);
}


// lift anonymous function
struct kk_std_core_map_fun20390__t_4 {
  struct kk_function_s _base;
  kk_function_t f;
  kk_std_core_types__tuple4_ t;
  kk_box_t x_17887;
};
static kk_box_t kk_std_core_map_fun20390_4(kk_function_t _fself, kk_box_t _b_18622, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20390_4(kk_function_t f, kk_std_core_types__tuple4_ t, kk_box_t x_17887, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20390__t_4* _self = kk_function_alloc_as(struct kk_std_core_map_fun20390__t_4, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20390_4, kk_context());
  _self->f = f;
  _self->t = t;
  _self->x_17887 = x_17887;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20390_4(kk_function_t _fself, kk_box_t _b_18622, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20390__t_4* _self = kk_function_as(struct kk_std_core_map_fun20390__t_4*, _fself);
  kk_function_t f = _self->f; /* (6092) -> 6094 6093 */
  kk_std_core_types__tuple4_ t = _self->t; /* (6092, 6092, 6092, 6092) */
  kk_box_t x_17887 = _self->x_17887; /* 6093 */
  kk_drop_match(_self, {kk_function_dup(f);kk_std_core_types__tuple4__dup(t);kk_box_dup(x_17887);}, {}, _ctx)
  kk_std_core_types__tuple4_ _x20391 = kk_std_core__mlift17715_map_4(x_17887, f, t, _b_18622, _ctx); /*(6093, 6093, 6093, 6093)*/
  return kk_std_core_types__tuple4__box(_x20391, _ctx);
}


// lift anonymous function
struct kk_std_core_map_fun20396__t_4 {
  struct kk_function_s _base;
  kk_function_t f;
  kk_std_core_types__tuple4_ t;
  kk_box_t x_17887;
  kk_box_t x0_17891;
};
static kk_box_t kk_std_core_map_fun20396_4(kk_function_t _fself, kk_box_t _b_18624, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20396_4(kk_function_t f, kk_std_core_types__tuple4_ t, kk_box_t x_17887, kk_box_t x0_17891, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20396__t_4* _self = kk_function_alloc_as(struct kk_std_core_map_fun20396__t_4, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20396_4, kk_context());
  _self->f = f;
  _self->t = t;
  _self->x_17887 = x_17887;
  _self->x0_17891 = x0_17891;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20396_4(kk_function_t _fself, kk_box_t _b_18624, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20396__t_4* _self = kk_function_as(struct kk_std_core_map_fun20396__t_4*, _fself);
  kk_function_t f = _self->f; /* (6092) -> 6094 6093 */
  kk_std_core_types__tuple4_ t = _self->t; /* (6092, 6092, 6092, 6092) */
  kk_box_t x_17887 = _self->x_17887; /* 6093 */
  kk_box_t x0_17891 = _self->x0_17891; /* 6093 */
  kk_drop_match(_self, {kk_function_dup(f);kk_std_core_types__tuple4__dup(t);kk_box_dup(x_17887);kk_box_dup(x0_17891);}, {}, _ctx)
  kk_std_core_types__tuple4_ _x20397 = kk_std_core__mlift17714_map_4(x_17887, x0_17891, f, t, _b_18624, _ctx); /*(6093, 6093, 6093, 6093)*/
  return kk_std_core_types__tuple4__box(_x20397, _ctx);
}


// lift anonymous function
struct kk_std_core_map_fun20401__t_4 {
  struct kk_function_s _base;
  kk_box_t x_17887;
  kk_box_t x0_17891;
  kk_box_t x1_17895;
};
static kk_box_t kk_std_core_map_fun20401_4(kk_function_t _fself, kk_box_t _b_18626, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20401_4(kk_box_t x_17887, kk_box_t x0_17891, kk_box_t x1_17895, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20401__t_4* _self = kk_function_alloc_as(struct kk_std_core_map_fun20401__t_4, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20401_4, kk_context());
  _self->x_17887 = x_17887;
  _self->x0_17891 = x0_17891;
  _self->x1_17895 = x1_17895;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20401_4(kk_function_t _fself, kk_box_t _b_18626, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20401__t_4* _self = kk_function_as(struct kk_std_core_map_fun20401__t_4*, _fself);
  kk_box_t x_17887 = _self->x_17887; /* 6093 */
  kk_box_t x0_17891 = _self->x0_17891; /* 6093 */
  kk_box_t x1_17895 = _self->x1_17895; /* 6093 */
  kk_drop_match(_self, {kk_box_dup(x_17887);kk_box_dup(x0_17891);kk_box_dup(x1_17895);}, {}, _ctx)
  kk_std_core_types__tuple4_ _x20402 = kk_std_core_types__new_dash__lp__comma__comma__comma__rp_(kk_reuse_null, x_17887, x0_17891, x1_17895, _b_18626, _ctx); /*(22, 23, 24, 25)*/
  return kk_std_core_types__tuple4__box(_x20402, _ctx);
}

kk_std_core_types__tuple4_ kk_std_core_map_4(kk_std_core_types__tuple4_ t, kk_function_t f, kk_context_t* _ctx) { /* forall<a,b,e> (t : (a, a, a, a), f : (a) -> e b) -> e (b, b, b, b) */ 
  kk_box_t x_17887;
  kk_function_t _x20382 = kk_function_dup(f); /*(6092) -> 6094 6093*/
  kk_box_t _x20380;
  {
    struct kk_std_core_types__Tuple4_* _con20381 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(t);
    kk_box_t _x = _con20381->fst;
    kk_box_dup(_x);
    _x20380 = _x; /*6092*/
  }
  x_17887 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20382, (_x20382, _x20380, _ctx)); /*6093*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(x_17887, _ctx);
    kk_box_t _x20383 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20384_4(f, t, _ctx), _ctx); /*3860*/
    return kk_std_core_types__tuple4__unbox(_x20383, _ctx);
  }
  {
    kk_box_t x0_17891;
    kk_function_t _x20388 = kk_function_dup(f); /*(6092) -> 6094 6093*/
    kk_box_t _x20386;
    {
      struct kk_std_core_types__Tuple4_* _con20387 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(t);
      kk_box_t _x0 = _con20387->snd;
      kk_box_dup(_x0);
      _x20386 = _x0; /*6092*/
    }
    x0_17891 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20388, (_x20388, _x20386, _ctx)); /*6093*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x0_17891, _ctx);
      kk_box_t _x20389 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20390_4(f, t, x_17887, _ctx), _ctx); /*3860*/
      return kk_std_core_types__tuple4__unbox(_x20389, _ctx);
    }
    {
      kk_box_t x1_17895;
      kk_function_t _x20394 = kk_function_dup(f); /*(6092) -> 6094 6093*/
      kk_box_t _x20392;
      {
        struct kk_std_core_types__Tuple4_* _con20393 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(t);
        kk_box_t _x1 = _con20393->thd;
        kk_box_dup(_x1);
        _x20392 = _x1; /*6092*/
      }
      x1_17895 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20394, (_x20394, _x20392, _ctx)); /*6093*/
      if (kk_yielding(kk_context())) {
        kk_box_drop(x1_17895, _ctx);
        kk_box_t _x20395 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20396_4(f, t, x_17887, x0_17891, _ctx), _ctx); /*3860*/
        return kk_std_core_types__tuple4__unbox(_x20395, _ctx);
      }
      {
        kk_box_t x2_17899;
        kk_box_t _x20398;
        {
          struct kk_std_core_types__Tuple4_* _con20399 = kk_std_core_types__as_dash__lp__comma__comma__comma__rp_(t);
          kk_box_t _pat06 = _con20399->fst;
          kk_box_t _pat13 = _con20399->snd;
          kk_box_t _pat22 = _con20399->thd;
          kk_box_t _x2 = _con20399->field4;
          if (kk_likely(kk_std_core_types__tuple4__is_unique(t))) {
            kk_box_drop(_pat22, _ctx);
            kk_box_drop(_pat13, _ctx);
            kk_box_drop(_pat06, _ctx);
            kk_std_core_types__tuple4__free(t);
          }
          else {
            kk_box_dup(_x2);
            kk_std_core_types__tuple4__decref(t, _ctx);
          }
          _x20398 = _x2; /*6092*/
        }
        x2_17899 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, _x20398, _ctx)); /*6093*/
        if (kk_yielding(kk_context())) {
          kk_box_drop(x2_17899, _ctx);
          kk_box_t _x20400 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20401_4(x_17887, x0_17891, x1_17895, _ctx), _ctx); /*3860*/
          return kk_std_core_types__tuple4__unbox(_x20400, _ctx);
        }
        {
          return kk_std_core_types__new_dash__lp__comma__comma__comma__rp_(kk_reuse_null, x_17887, x0_17891, x1_17895, x2_17899, _ctx);
        }
      }
    }
  }
}
 
// Apply a function `f` to each character in a string


// lift anonymous function
struct kk_std_core_map_fun20403__t_6 {
  struct kk_function_s _base;
  kk_function_t f;
};
static kk_box_t kk_std_core_map_fun20403_6(kk_function_t _fself, kk_box_t _b_18637, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20403_6(kk_function_t f, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20403__t_6* _self = kk_function_alloc_as(struct kk_std_core_map_fun20403__t_6, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20403_6, kk_context());
  _self->f = f;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20403_6(kk_function_t _fself, kk_box_t _b_18637, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20403__t_6* _self = kk_function_as(struct kk_std_core_map_fun20403__t_6*, _fself);
  kk_function_t f = _self->f; /* (char) -> 6561 char */
  kk_drop_match(_self, {kk_function_dup(f);}, {}, _ctx)
  kk_char_t _x20404;
  kk_char_t _x20405 = kk_char_unbox(_b_18637, _ctx); /*char*/
  _x20404 = kk_function_call(kk_char_t, (kk_function_t, kk_char_t, kk_context_t*), f, (f, _x20405, _ctx)); /*char*/
  return kk_char_box(_x20404, _ctx);
}


// lift anonymous function
struct kk_std_core_map_fun20407__t_6 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_map_fun20407_6(kk_function_t _fself, kk_box_t _b_18641, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20407_6(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_map_fun20407_6, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_map_fun20407_6(kk_function_t _fself, kk_box_t _b_18641, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_string_t _x20408;
  kk_std_core__list _x20409 = kk_std_core__list_unbox(_b_18641, _ctx); /*list<char>*/
  _x20408 = kk_std_core_string_2(_x20409, _ctx); /*string*/
  return kk_string_box(_x20408);
}

kk_string_t kk_std_core_map_6(kk_string_t s, kk_function_t f, kk_context_t* _ctx) { /* forall<e> (s : string, f : (char) -> e char) -> e string */ 
  kk_std_core__list _b_18638_18635 = kk_std_core_list_6(s, _ctx); /*list<char>*/;
  kk_std_core__list x_17907 = kk_std_core_map_5(_b_18638_18635, kk_std_core_new_map_fun20403_6(f, _ctx), _ctx); /*list<char>*/;
  if (kk_yielding(kk_context())) {
    kk_std_core__list_drop(x_17907, _ctx);
    kk_box_t _x20406 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20407_6(_ctx), _ctx); /*3860*/
    return kk_string_unbox(_x20406);
  }
  {
    return kk_std_core_string_2(x_17907, _ctx);
  }
}
 
// Apply a total function `f` to each element in a vector `v`


// lift anonymous function
struct kk_std_core_map_fun20415__t_7 {
  struct kk_function_s _base;
  kk_function_t f;
  kk_vector_t v;
  kk_vector_t w;
};
static kk_unit_t kk_std_core_map_fun20415_7(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20415_7(kk_function_t f, kk_vector_t v, kk_vector_t w, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20415__t_7* _self = kk_function_alloc_as(struct kk_std_core_map_fun20415__t_7, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20415_7, kk_context());
  _self->f = f;
  _self->v = v;
  _self->w = w;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_map_fun20417__t_7 {
  struct kk_function_s _base;
  kk_vector_t w;
  kk_ssize_t i;
};
static kk_box_t kk_std_core_map_fun20417_7(kk_function_t _fself, kk_box_t _b_18644, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20417_7(kk_vector_t w, kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20417__t_7* _self = kk_function_alloc_as(struct kk_std_core_map_fun20417__t_7, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20417_7, kk_context());
  _self->w = w;
  _self->i = i;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20417_7(kk_function_t _fself, kk_box_t _b_18644, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20417__t_7* _self = kk_function_as(struct kk_std_core_map_fun20417__t_7*, _fself);
  kk_vector_t w = _self->w; /* vector<6610> */
  kk_ssize_t i = _self->i; /* ssize_t */
  kk_drop_match(_self, {kk_vector_dup(w);;}, {}, _ctx)
  kk_unit_t _x20418 = kk_Unit;
  kk_vector_unsafe_assign(w,i,_b_18644,kk_context());
  return kk_unit_box(_x20418);
}
static kk_unit_t kk_std_core_map_fun20415_7(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20415__t_7* _self = kk_function_as(struct kk_std_core_map_fun20415__t_7*, _fself);
  kk_function_t f = _self->f; /* (6609) -> 6611 6610 */
  kk_vector_t v = _self->v; /* vector<6609> */
  kk_vector_t w = _self->w; /* vector<6610> */
  kk_drop_match(_self, {kk_function_dup(f);kk_vector_dup(v);kk_vector_dup(w);}, {}, _ctx)
  kk_box_t x0_18151;
  kk_box_t _brw_19819 = kk_vector_at_borrow(v, i); /*223*/;
  kk_vector_drop(v, _ctx);
  x0_18151 = _brw_19819; /*6609*/
  kk_box_t x1_17915 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, x0_18151, _ctx)); /*6610*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(x1_17915, _ctx);
    kk_box_t _x20416 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20417_7(w, i, _ctx), _ctx); /*3860*/
    return kk_unit_unbox(_x20416);
  }
  {
    return kk_vector_unsafe_assign(w,i,x1_17915,kk_context());
  }
}


// lift anonymous function
struct kk_std_core_map_fun20420__t_7 {
  struct kk_function_s _base;
  kk_vector_t w;
};
static kk_box_t kk_std_core_map_fun20420_7(kk_function_t _fself, kk_box_t _b_18648, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_fun20420_7(kk_vector_t w, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20420__t_7* _self = kk_function_alloc_as(struct kk_std_core_map_fun20420__t_7, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_map_fun20420_7, kk_context());
  _self->w = w;
  return &_self->_base;
}

static kk_box_t kk_std_core_map_fun20420_7(kk_function_t _fself, kk_box_t _b_18648, kk_context_t* _ctx) {
  struct kk_std_core_map_fun20420__t_7* _self = kk_function_as(struct kk_std_core_map_fun20420__t_7*, _fself);
  kk_vector_t w = _self->w; /* vector<6610> */
  kk_drop_match(_self, {kk_vector_dup(w);}, {}, _ctx)
  kk_box_drop(_b_18648, _ctx);
  return kk_vector_box(w, _ctx);
}

kk_vector_t kk_std_core_map_7(kk_vector_t v, kk_function_t f, kk_context_t* _ctx) { /* forall<a,b,e> (v : vector<a>, f : (a) -> e b) -> e vector<b> */ 
  kk_vector_t w;
  kk_ssize_t _x20410;
  kk_integer_t _x20411;
  kk_ssize_t _x20412 = kk_vector_len_borrow(v); /*ssize_t*/
  _x20411 = kk_integer_from_ssize_t(_x20412,kk_context()); /*int*/
  _x20410 = kk_std_core_ssize__t(_x20411, _ctx); /*ssize_t*/
  w = kk_vector_alloc_uninit(_x20410,NULL,kk_context()); /*vector<6610>*/
  kk_ssize_t start0_17920 = ((kk_ssize_t)0); /*ssize_t*/;
  kk_ssize_t end_17921;
  kk_ssize_t _x20413 = kk_vector_len_borrow(v); /*ssize_t*/
  end_17921 = kk_std_core_decr_1(_x20413, _ctx); /*ssize_t*/
  kk_unit_t x_17910 = kk_Unit;
  kk_function_t _x20414;
  kk_vector_dup(w);
  _x20414 = kk_std_core_new_map_fun20415_7(f, v, w, _ctx); /*(i : ssize_t) -> 6611 ()*/
  kk_std_core__lift17269_forz(_x20414, end_17921, start0_17920, _ctx);
  if (kk_yielding(kk_context())) {
    kk_box_t _x20419 = kk_std_core_hnd_yield_extend(kk_std_core_new_map_fun20420_7(w, _ctx), _ctx); /*3860*/
    return kk_vector_unbox(_x20419, _ctx);
  }
  {
    return w;
  }
}
 
// Right-align a string to width `width`  using `fill`  (default is a space) to fill from the left.

kk_string_t kk_std_core_pad_left(kk_string_t s, kk_integer_t width, kk_std_core_types__optional fill, kk_context_t* _ctx) { /* (s : string, width : int, fill : optional<char>) -> string */ 
  kk_ssize_t w;
  kk_integer_t _x20421 = kk_integer_dup(width); /*int*/
  w = kk_std_core_ssize__t(_x20421, _ctx); /*ssize_t*/
  kk_ssize_t n;
  kk_string_t _x20422 = kk_string_dup(s); /*string*/
  n = kk_string_len(_x20422,kk_context()); /*ssize_t*/
  bool _match_19816 = (w <= n); /*bool*/;
  if (_match_19816) {
    kk_std_core_types__optional_drop(fill, _ctx);
    return s;
  }
  {
    kk_string_t _x20423;
    kk_string_t _x20424;
    kk_char_t _x20425;
    if (kk_std_core_types__is_Optional(fill)) {
      kk_box_t _box_x18651 = fill._cons.Optional.value;
      kk_char_t _fill_7359 = kk_char_unbox(_box_x18651, NULL);
      kk_std_core_types__optional_drop(fill, _ctx);
      _x20425 = _fill_7359; /*char*/
      goto _match20426;
    }
    {
      _x20425 = ' '; /*char*/
    }
    _match20426: ;
    _x20424 = kk_std_core_string(_x20425, _ctx); /*string*/
    kk_ssize_t _x20428 = (w - n); /*ssize_t*/
    _x20423 = kk_std_core_repeatz(_x20424, _x20428, _ctx); /*string*/
    return kk_std_core__lp__plus__plus__1_rp_(_x20423, s, _ctx);
  }
}
 
// Show an `:int` as a hexadecimal value.
// The `width`  parameter specifies how wide the hex value is where `"0"`  is used to align.
// The `use-capitals` parameter (= `True`) determines if captical letters should be used to display the hexadecimal digits.
// The `pre` (=`"0x"`) is an optional prefix for the number (goes between the sign and the number).

kk_string_t kk_std_core_show_hex(kk_integer_t i, kk_std_core_types__optional width, kk_std_core_types__optional use_capitals, kk_std_core_types__optional pre, kk_context_t* _ctx) { /* (i : int, width : optional<int>, use-capitals : optional<bool>, pre : optional<string>) -> string */ 
  kk_string_t _x20429;
  bool _match_19813;
  kk_integer_t _brw_19814 = kk_integer_from_small(0); /*int*/;
  bool _brw_19815 = kk_integer_lt_borrow(i,_brw_19814,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19814, _ctx);
  _match_19813 = _brw_19815; /*bool*/
  if (_match_19813) {
    kk_define_string_literal(, _s20430, 1, "-")
    _x20429 = kk_string_dup(_s20430); /*string*/
  }
  else {
    _x20429 = kk_string_empty(); /*string*/
  }
  kk_string_t _x20432;
  kk_string_t _x20433;
  if (kk_std_core_types__is_Optional(pre)) {
    kk_box_t _box_x18652 = pre._cons.Optional.value;
    kk_string_t _pre_7467 = kk_string_unbox(_box_x18652);
    kk_string_dup(_pre_7467);
    kk_std_core_types__optional_drop(pre, _ctx);
    _x20433 = _pre_7467; /*string*/
    goto _match20434;
  }
  {
    kk_define_string_literal(, _s20436, 2, "0x")
    _x20433 = kk_string_dup(_s20436); /*string*/
  }
  _match20434: ;
  kk_string_t _x20437;
  kk_string_t _own_19811;
  kk_integer_t _x20438 = kk_integer_abs(i,kk_context()); /*int*/
  bool _x20439;
  if (kk_std_core_types__is_Optional(use_capitals)) {
    kk_box_t _box_x18653 = use_capitals._cons.Optional.value;
    bool _use_capitals_7463 = kk_bool_unbox(_box_x18653);
    kk_std_core_types__optional_drop(use_capitals, _ctx);
    _x20439 = _use_capitals_7463; /*bool*/
    goto _match20440;
  }
  {
    _x20439 = true; /*bool*/
  }
  _match20440: ;
  _own_19811 = kk_std_core_int_show_hex(_x20438, _x20439, _ctx); /*string*/
  kk_integer_t _brw_19810;
  if (kk_std_core_types__is_Optional(width)) {
    kk_box_t _box_x18654 = width._cons.Optional.value;
    kk_integer_t _width_7459 = kk_integer_unbox(_box_x18654);
    kk_integer_dup(_width_7459);
    kk_std_core_types__optional_drop(width, _ctx);
    _brw_19810 = _width_7459; /*int*/
    goto _match20442;
  }
  {
    _brw_19810 = kk_integer_from_small(1); /*int*/
  }
  _match20442: ;
  kk_string_t _brw_19812;
  kk_std_core_types__optional _x20444 = kk_std_core_types__new_Optional(kk_char_box('0', _ctx), _ctx); /*optional<110>*/
  _brw_19812 = kk_std_core_pad_left(_own_19811, _brw_19810, _x20444, _ctx); /*string*/
  kk_integer_drop(_brw_19810, _ctx);
  _x20437 = _brw_19812; /*string*/
  _x20432 = kk_std_core__lp__plus__plus__1_rp_(_x20433, _x20437, _ctx); /*string*/
  return kk_std_core__lp__plus__plus__1_rp_(_x20429, _x20432, _ctx);
}
 
// Is the integer positive (stricly greater than zero)

bool kk_std_core_is_pos_2(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> bool */ 
  kk_std_core_types__order x_17251;
  kk_std_core_types__order _brw_19807 = kk_int_as_order(kk_integer_signum_borrow(i),kk_context()); /*order*/;
  kk_integer_drop(i, _ctx);
  x_17251 = _brw_19807; /*order*/
  kk_integer_t _brw_19805;
  if (kk_std_core_types__is_Lt(x_17251)) {
    _brw_19805 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20445;
  }
  if (kk_std_core_types__is_Eq(x_17251)) {
    _brw_19805 = kk_integer_from_small(0); /*int*/
    goto _match20445;
  }
  {
    _brw_19805 = kk_integer_from_small(1); /*int*/
  }
  _match20445: ;
  kk_integer_t _brw_19804 = kk_integer_from_small(1); /*int*/;
  bool _brw_19806 = kk_integer_eq_borrow(_brw_19805,_brw_19804,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19805, _ctx);
  kk_integer_drop(_brw_19804, _ctx);
  return _brw_19806;
}
 
// Show a character as a string

kk_string_t kk_std_core_show_char(kk_char_t c, kk_context_t* _ctx) { /* (c : char) -> string */ 
  bool _match_19776 = (c < (' ')); /*bool*/;
  if (_match_19776) {
    bool _match_19792 = (c == 0x000A); /*bool*/;
    if (_match_19792) {
      kk_define_string_literal(, _s20452, 2, "\\n")
      return kk_string_dup(_s20452);
    }
    {
      bool _match_19793 = (c == 0x000D); /*bool*/;
      if (_match_19793) {
        kk_define_string_literal(, _s20453, 2, "\\r")
        return kk_string_dup(_s20453);
      }
      {
        bool _match_19794 = (c == 0x0009); /*bool*/;
        if (_match_19794) {
          kk_define_string_literal(, _s20454, 2, "\\t")
          return kk_string_dup(_s20454);
        }
        {
          bool _match_19795;
          kk_integer_t _brw_19801 = kk_integer_from_int(c,kk_context()); /*int*/;
          kk_integer_t _brw_19800 = kk_integer_from_small(255); /*int*/;
          bool _brw_19802 = kk_integer_lte_borrow(_brw_19801,_brw_19800,kk_context()); /*bool*/;
          kk_integer_drop(_brw_19801, _ctx);
          kk_integer_drop(_brw_19800, _ctx);
          _match_19795 = _brw_19802; /*bool*/
          if (_match_19795) {
            kk_string_t _x20455;
            kk_define_string_literal(, _s20456, 2, "\\x")
            _x20455 = kk_string_dup(_s20456); /*string*/
            kk_string_t _x20457;
            kk_integer_t _arg_7933 = kk_integer_from_int(c,kk_context()); /*int*/;
            kk_std_core_types__optional _x20458 = kk_std_core_types__new_Optional(kk_integer_box(kk_integer_from_small(2)), _ctx); /*optional<110>*/
            kk_std_core_types__optional _x20459;
            kk_box_t _x20460;
            kk_string_t _x20461 = kk_string_empty(); /*string*/
            _x20460 = kk_string_box(_x20461); /*110*/
            _x20459 = kk_std_core_types__new_Optional(_x20460, _ctx); /*optional<110>*/
            _x20457 = kk_std_core_show_hex(_arg_7933, _x20458, kk_std_core_types__new_None(_ctx), _x20459, _ctx); /*string*/
            return kk_std_core__lp__plus__plus__1_rp_(_x20455, _x20457, _ctx);
          }
          {
            bool _match_19796;
            kk_integer_t _brw_19798 = kk_integer_from_int(c,kk_context()); /*int*/;
            kk_integer_t _brw_19797 = kk_integer_from_int(65535, _ctx); /*int*/;
            bool _brw_19799 = kk_integer_lte_borrow(_brw_19798,_brw_19797,kk_context()); /*bool*/;
            kk_integer_drop(_brw_19798, _ctx);
            kk_integer_drop(_brw_19797, _ctx);
            _match_19796 = _brw_19799; /*bool*/
            if (_match_19796) {
              kk_string_t _x20463;
              kk_define_string_literal(, _s20464, 2, "\\u")
              _x20463 = kk_string_dup(_s20464); /*string*/
              kk_string_t _x20465;
              kk_integer_t _arg_8018 = kk_integer_from_int(c,kk_context()); /*int*/;
              kk_std_core_types__optional _x20466 = kk_std_core_types__new_Optional(kk_integer_box(kk_integer_from_small(4)), _ctx); /*optional<110>*/
              kk_std_core_types__optional _x20467;
              kk_box_t _x20468;
              kk_string_t _x20469 = kk_string_empty(); /*string*/
              _x20468 = kk_string_box(_x20469); /*110*/
              _x20467 = kk_std_core_types__new_Optional(_x20468, _ctx); /*optional<110>*/
              _x20465 = kk_std_core_show_hex(_arg_8018, _x20466, kk_std_core_types__new_None(_ctx), _x20467, _ctx); /*string*/
              return kk_std_core__lp__plus__plus__1_rp_(_x20463, _x20465, _ctx);
            }
            {
              kk_string_t _x20471;
              kk_define_string_literal(, _s20472, 2, "\\U")
              _x20471 = kk_string_dup(_s20472); /*string*/
              kk_string_t _x20473;
              kk_integer_t _arg_8061 = kk_integer_from_int(c,kk_context()); /*int*/;
              kk_std_core_types__optional _x20474 = kk_std_core_types__new_Optional(kk_integer_box(kk_integer_from_small(6)), _ctx); /*optional<110>*/
              kk_std_core_types__optional _x20475;
              kk_box_t _x20476;
              kk_string_t _x20477 = kk_string_empty(); /*string*/
              _x20476 = kk_string_box(_x20477); /*110*/
              _x20475 = kk_std_core_types__new_Optional(_x20476, _ctx); /*optional<110>*/
              _x20473 = kk_std_core_show_hex(_arg_8061, _x20474, kk_std_core_types__new_None(_ctx), _x20475, _ctx); /*string*/
              return kk_std_core__lp__plus__plus__1_rp_(_x20471, _x20473, _ctx);
            }
          }
        }
      }
    }
  }
  {
    bool _match_19777 = (c > ('~')); /*bool*/;
    if (_match_19777) {
      bool _match_19781 = (c == 0x000A); /*bool*/;
      if (_match_19781) {
        kk_define_string_literal(, _s20479, 2, "\\n")
        return kk_string_dup(_s20479);
      }
      {
        bool _match_19782 = (c == 0x000D); /*bool*/;
        if (_match_19782) {
          kk_define_string_literal(, _s20480, 2, "\\r")
          return kk_string_dup(_s20480);
        }
        {
          bool _match_19783 = (c == 0x0009); /*bool*/;
          if (_match_19783) {
            kk_define_string_literal(, _s20481, 2, "\\t")
            return kk_string_dup(_s20481);
          }
          {
            bool _match_19784;
            kk_integer_t _brw_19790 = kk_integer_from_int(c,kk_context()); /*int*/;
            kk_integer_t _brw_19789 = kk_integer_from_small(255); /*int*/;
            bool _brw_19791 = kk_integer_lte_borrow(_brw_19790,_brw_19789,kk_context()); /*bool*/;
            kk_integer_drop(_brw_19790, _ctx);
            kk_integer_drop(_brw_19789, _ctx);
            _match_19784 = _brw_19791; /*bool*/
            if (_match_19784) {
              kk_string_t _x20482;
              kk_define_string_literal(, _s20483, 2, "\\x")
              _x20482 = kk_string_dup(_s20483); /*string*/
              kk_string_t _x20484;
              kk_integer_t _arg_79330 = kk_integer_from_int(c,kk_context()); /*int*/;
              kk_std_core_types__optional _x20485 = kk_std_core_types__new_Optional(kk_integer_box(kk_integer_from_small(2)), _ctx); /*optional<110>*/
              kk_std_core_types__optional _x20486;
              kk_box_t _x20487;
              kk_string_t _x20488 = kk_string_empty(); /*string*/
              _x20487 = kk_string_box(_x20488); /*110*/
              _x20486 = kk_std_core_types__new_Optional(_x20487, _ctx); /*optional<110>*/
              _x20484 = kk_std_core_show_hex(_arg_79330, _x20485, kk_std_core_types__new_None(_ctx), _x20486, _ctx); /*string*/
              return kk_std_core__lp__plus__plus__1_rp_(_x20482, _x20484, _ctx);
            }
            {
              bool _match_19785;
              kk_integer_t _brw_19787 = kk_integer_from_int(c,kk_context()); /*int*/;
              kk_integer_t _brw_19786 = kk_integer_from_int(65535, _ctx); /*int*/;
              bool _brw_19788 = kk_integer_lte_borrow(_brw_19787,_brw_19786,kk_context()); /*bool*/;
              kk_integer_drop(_brw_19787, _ctx);
              kk_integer_drop(_brw_19786, _ctx);
              _match_19785 = _brw_19788; /*bool*/
              if (_match_19785) {
                kk_string_t _x20490;
                kk_define_string_literal(, _s20491, 2, "\\u")
                _x20490 = kk_string_dup(_s20491); /*string*/
                kk_string_t _x20492;
                kk_integer_t _arg_80180 = kk_integer_from_int(c,kk_context()); /*int*/;
                kk_std_core_types__optional _x20493 = kk_std_core_types__new_Optional(kk_integer_box(kk_integer_from_small(4)), _ctx); /*optional<110>*/
                kk_std_core_types__optional _x20494;
                kk_box_t _x20495;
                kk_string_t _x20496 = kk_string_empty(); /*string*/
                _x20495 = kk_string_box(_x20496); /*110*/
                _x20494 = kk_std_core_types__new_Optional(_x20495, _ctx); /*optional<110>*/
                _x20492 = kk_std_core_show_hex(_arg_80180, _x20493, kk_std_core_types__new_None(_ctx), _x20494, _ctx); /*string*/
                return kk_std_core__lp__plus__plus__1_rp_(_x20490, _x20492, _ctx);
              }
              {
                kk_string_t _x20498;
                kk_define_string_literal(, _s20499, 2, "\\U")
                _x20498 = kk_string_dup(_s20499); /*string*/
                kk_string_t _x20500;
                kk_integer_t _arg_80610 = kk_integer_from_int(c,kk_context()); /*int*/;
                kk_std_core_types__optional _x20501 = kk_std_core_types__new_Optional(kk_integer_box(kk_integer_from_small(6)), _ctx); /*optional<110>*/
                kk_std_core_types__optional _x20502;
                kk_box_t _x20503;
                kk_string_t _x20504 = kk_string_empty(); /*string*/
                _x20503 = kk_string_box(_x20504); /*110*/
                _x20502 = kk_std_core_types__new_Optional(_x20503, _ctx); /*optional<110>*/
                _x20500 = kk_std_core_show_hex(_arg_80610, _x20501, kk_std_core_types__new_None(_ctx), _x20502, _ctx); /*string*/
                return kk_std_core__lp__plus__plus__1_rp_(_x20498, _x20500, _ctx);
              }
            }
          }
        }
      }
    }
    {
      bool _match_19778 = (c == ('\'')); /*bool*/;
      if (_match_19778) {
        kk_define_string_literal(, _s20506, 2, "\\\'")
        return kk_string_dup(_s20506);
      }
      {
        bool _match_19779 = (c == ('"')); /*bool*/;
        if (_match_19779) {
          kk_define_string_literal(, _s20507, 2, "\\\"")
          return kk_string_dup(_s20507);
        }
        {
          bool _match_19780 = (c == ('\\')); /*bool*/;
          if (_match_19780) {
            kk_define_string_literal(, _s20508, 2, "\\\\")
            return kk_string_dup(_s20508);
          }
          {
            return kk_std_core_string(c, _ctx);
          }
        }
      }
    }
  }
}
 
// Show a `:double` fixed-point notation.
// The optional `precision` (= `-2`) specifies the maximum precision.
// If `>=0` it specifies the number of digits behind the dot (up to `20` max).
// If negative, then at most the absolute value of `precision` digits behind the dot are used.
// This may still show a number in exponential notation if the it is too small or large,
// in particular, for  a `d` where `d > 1.0e21` or `d < 1.0e-15`, or if
// `precision.abs > 17`, the `show-exp` routine is used.

kk_string_t kk_std_core_show_fixed(double d, kk_std_core_types__optional precision, kk_context_t* _ctx) { /* (d : double, precision : optional<int>) -> string */ 
  double dabs = kk_double_abs(d); /*double*/;
  bool _match_19774 = (dabs < (1.0e-15)); /*bool*/;
  if (_match_19774) {
    int32_t _x20513;
    kk_integer_t _x20514;
    if (kk_std_core_types__is_Optional(precision)) {
      kk_box_t _box_x18682 = precision._cons.Optional.value;
      kk_integer_t _precision_8150 = kk_integer_unbox(_box_x18682);
      kk_integer_dup(_precision_8150);
      kk_std_core_types__optional_drop(precision, _ctx);
      _x20514 = _precision_8150; /*int*/
      goto _match20515;
    }
    {
      _x20514 = kk_integer_from_small(-2); /*int*/
    }
    _match20515: ;
    _x20513 = kk_std_core_int32(_x20514, _ctx); /*int32*/
    return kk_std_core_show_expx(d, _x20513, _ctx);
  }
  {
    bool _match_19775 = (dabs > (1.0e21)); /*bool*/;
    if (_match_19775) {
      int32_t _x20517;
      kk_integer_t _x20518;
      if (kk_std_core_types__is_Optional(precision)) {
        kk_box_t _box_x18683 = precision._cons.Optional.value;
        kk_integer_t _precision_81500 = kk_integer_unbox(_box_x18683);
        kk_integer_dup(_precision_81500);
        kk_std_core_types__optional_drop(precision, _ctx);
        _x20518 = _precision_81500; /*int*/
        goto _match20519;
      }
      {
        _x20518 = kk_integer_from_small(-2); /*int*/
      }
      _match20519: ;
      _x20517 = kk_std_core_int32(_x20518, _ctx); /*int32*/
      return kk_std_core_show_expx(d, _x20517, _ctx);
    }
    {
      int32_t _x20521;
      kk_integer_t _x20522;
      if (kk_std_core_types__is_Optional(precision)) {
        kk_box_t _box_x18684 = precision._cons.Optional.value;
        kk_integer_t _precision_81501 = kk_integer_unbox(_box_x18684);
        kk_integer_dup(_precision_81501);
        kk_std_core_types__optional_drop(precision, _ctx);
        _x20522 = _precision_81501; /*int*/
        goto _match20523;
      }
      {
        _x20522 = kk_integer_from_small(-2); /*int*/
      }
      _match20523: ;
      _x20521 = kk_std_core_int32(_x20522, _ctx); /*int32*/
      return kk_std_core_show_fixedx(d, _x20521, _ctx);
    }
  }
}
 
// lifted

kk_string_t kk_std_core__lift17271_show_list(kk_string_t sep, kk_std_core__list ys, kk_string_t acc, kk_context_t* _ctx) { /* (sep : string, ys : list<string>, acc : string) -> string */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20525 = kk_std_core__as_Cons(ys);
    kk_box_t _box_x18685 = _con20525->head;
    kk_std_core__list yy = _con20525->tail;
    kk_string_t y = kk_string_unbox(_box_x18685);
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      kk_std_core__list_free(ys);
    }
    else {
      kk_string_dup(y);
      kk_std_core__list_dup(yy);
      kk_std_core__list_decref(ys, _ctx);
    }
    kk_string_t acc0_17298;
    kk_string_t _x20527;
    kk_string_t _x20528 = kk_string_dup(sep); /*string*/
    _x20527 = kk_std_core__lp__plus__plus__1_rp_(_x20528, y, _ctx); /*string*/
    acc0_17298 = kk_std_core__lp__plus__plus__1_rp_(acc, _x20527, _ctx); /*string*/
    { // tailcall
      ys = yy;
      acc = acc0_17298;
      goto kk__tailcall;
    }
  }
  {
    kk_string_drop(sep, _ctx);
    return acc;
  }
}
 
// monadic lift

kk_string_t kk_std_core__mlift17719_show_list(kk_std_core__list _y_17473, kk_context_t* _ctx) { /* forall<e> (list<string>) -> e string */ 
  kk_string_t _x20529;
  kk_define_string_literal(, _s20530, 1, "[")
  _x20529 = kk_string_dup(_s20530); /*string*/
  kk_string_t _x20531;
  kk_string_t _x20532;
  if (kk_std_core__is_Nil(_y_17473)) {
    _x20532 = kk_string_empty(); /*string*/
  }
  else {
    struct kk_std_core_Cons* _con20534 = kk_std_core__as_Cons(_y_17473);
    kk_box_t _box_x18686 = _con20534->head;
    kk_std_core__list xx = _con20534->tail;
    kk_string_t x = kk_string_unbox(_box_x18686);
    if (kk_likely(kk_std_core__list_is_unique(_y_17473))) {
      kk_std_core__list_free(_y_17473);
    }
    else {
      kk_string_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(_y_17473, _ctx);
    }
    kk_string_t _x20536;
    kk_define_string_literal(, _s20537, 1, ",")
    _x20536 = kk_string_dup(_s20537); /*string*/
    _x20532 = kk_std_core__lift17271_show_list(_x20536, xx, x, _ctx); /*string*/
  }
  kk_string_t _x20538;
  kk_define_string_literal(, _s20539, 1, "]")
  _x20538 = kk_string_dup(_s20539); /*string*/
  _x20531 = kk_std_core__lp__plus__plus__1_rp_(_x20532, _x20538, _ctx); /*string*/
  return kk_std_core__lp__plus__plus__1_rp_(_x20529, _x20531, _ctx);
}
 
// Convert a list to a string


// lift anonymous function
struct kk_std_core_show_list_fun20540__t {
  struct kk_function_s _base;
  kk_function_t show_elem;
};
static kk_box_t kk_std_core_show_list_fun20540(kk_function_t _fself, kk_box_t _b_18689, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_show_list_fun20540(kk_function_t show_elem, kk_context_t* _ctx) {
  struct kk_std_core_show_list_fun20540__t* _self = kk_function_alloc_as(struct kk_std_core_show_list_fun20540__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_show_list_fun20540, kk_context());
  _self->show_elem = show_elem;
  return &_self->_base;
}

static kk_box_t kk_std_core_show_list_fun20540(kk_function_t _fself, kk_box_t _b_18689, kk_context_t* _ctx) {
  struct kk_std_core_show_list_fun20540__t* _self = kk_function_as(struct kk_std_core_show_list_fun20540__t*, _fself);
  kk_function_t show_elem = _self->show_elem; /* (8695) -> 8696 string */
  kk_drop_match(_self, {kk_function_dup(show_elem);}, {}, _ctx)
  kk_string_t _x20541 = kk_function_call(kk_string_t, (kk_function_t, kk_box_t, kk_context_t*), show_elem, (show_elem, _b_18689, _ctx)); /*string*/
  return kk_string_box(_x20541);
}


// lift anonymous function
struct kk_std_core_show_list_fun20543__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_show_list_fun20543(kk_function_t _fself, kk_box_t _b_18694, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_show_list_fun20543(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_show_list_fun20543, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_show_list_fun20543(kk_function_t _fself, kk_box_t _b_18694, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_string_t _x20544;
  kk_string_t _x20545;
  kk_define_string_literal(, _s20546, 1, "[")
  _x20545 = kk_string_dup(_s20546); /*string*/
  kk_string_t _x20547;
  kk_string_t _x20548;
  kk_std_core__list _match_19773 = kk_std_core__list_unbox(_b_18694, _ctx); /*list<string>*/;
  if (kk_std_core__is_Nil(_match_19773)) {
    _x20548 = kk_string_empty(); /*string*/
  }
  else {
    struct kk_std_core_Cons* _con20550 = kk_std_core__as_Cons(_match_19773);
    kk_box_t _box_x18692 = _con20550->head;
    kk_std_core__list xx = _con20550->tail;
    kk_string_t x0 = kk_string_unbox(_box_x18692);
    if (kk_likely(kk_std_core__list_is_unique(_match_19773))) {
      kk_std_core__list_free(_match_19773);
    }
    else {
      kk_string_dup(x0);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(_match_19773, _ctx);
    }
    kk_string_t _x20552;
    kk_define_string_literal(, _s20553, 1, ",")
    _x20552 = kk_string_dup(_s20553); /*string*/
    _x20548 = kk_std_core__lift17271_show_list(_x20552, xx, x0, _ctx); /*string*/
  }
  kk_string_t _x20554;
  kk_define_string_literal(, _s20555, 1, "]")
  _x20554 = kk_string_dup(_s20555); /*string*/
  _x20547 = kk_std_core__lp__plus__plus__1_rp_(_x20548, _x20554, _ctx); /*string*/
  _x20544 = kk_std_core__lp__plus__plus__1_rp_(_x20545, _x20547, _ctx); /*string*/
  return kk_string_box(_x20544);
}

kk_string_t kk_std_core_show_list(kk_std_core__list xs, kk_function_t show_elem, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, show-elem : (a) -> e string) -> e string */ 
  kk_std_core__list x_17925 = kk_std_core_map_5(xs, kk_std_core_new_show_list_fun20540(show_elem, _ctx), _ctx); /*list<string>*/;
  if (kk_yielding(kk_context())) {
    kk_std_core__list_drop(x_17925, _ctx);
    kk_box_t _x20542 = kk_std_core_hnd_yield_extend(kk_std_core_new_show_list_fun20543(_ctx), _ctx); /*3860*/
    return kk_string_unbox(_x20542);
  }
  {
    kk_string_t _x20556;
    kk_define_string_literal(, _s20557, 1, "[")
    _x20556 = kk_string_dup(_s20557); /*string*/
    kk_string_t _x20558;
    kk_string_t _x20559;
    if (kk_std_core__is_Nil(x_17925)) {
      _x20559 = kk_string_empty(); /*string*/
    }
    else {
      struct kk_std_core_Cons* _con20561 = kk_std_core__as_Cons(x_17925);
      kk_box_t _box_x18695 = _con20561->head;
      kk_std_core__list xx0 = _con20561->tail;
      kk_string_t x1 = kk_string_unbox(_box_x18695);
      if (kk_likely(kk_std_core__list_is_unique(x_17925))) {
        kk_std_core__list_free(x_17925);
      }
      else {
        kk_string_dup(x1);
        kk_std_core__list_dup(xx0);
        kk_std_core__list_decref(x_17925, _ctx);
      }
      kk_string_t _x20563;
      kk_define_string_literal(, _s20564, 1, ",")
      _x20563 = kk_string_dup(_s20564); /*string*/
      _x20559 = kk_std_core__lift17271_show_list(_x20563, xx0, x1, _ctx); /*string*/
    }
    kk_string_t _x20565;
    kk_define_string_literal(, _s20566, 1, "]")
    _x20565 = kk_string_dup(_s20566); /*string*/
    _x20558 = kk_std_core__lp__plus__plus__1_rp_(_x20559, _x20565, _ctx); /*string*/
    return kk_std_core__lp__plus__plus__1_rp_(_x20556, _x20558, _ctx);
  }
}
 
// Convert an `:int` to a string

kk_string_t kk_std_core_show(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> string */ 
  return kk_integer_to_string(i,kk_context());
}
 
// Show a `:double` as a string.
// If `d >= 1.0e-5` and `d < 1.0e+21`, `show-fixed` is used and otherwise `show-exp`.
// Default `precision` is `-17`.

kk_string_t kk_std_core_show_1(double d, kk_std_core_types__optional precision, kk_context_t* _ctx) { /* (d : double, precision : optional<int>) -> string */ 
  double dabs = kk_double_abs(d); /*double*/;
  bool _match_19770 = (dabs >= (1.0e-5)); /*bool*/;
  if (_match_19770) {
    bool _match_19771 = (dabs < (1.0e21)); /*bool*/;
    if (_match_19771) {
      kk_std_core_types__optional _x20567;
      kk_box_t _x20568;
      kk_integer_t _x20569;
      if (kk_std_core_types__is_Optional(precision)) {
        kk_box_t _box_x18698 = precision._cons.Optional.value;
        kk_integer_t _precision_8705 = kk_integer_unbox(_box_x18698);
        kk_integer_dup(_precision_8705);
        kk_std_core_types__optional_drop(precision, _ctx);
        _x20569 = _precision_8705; /*int*/
        goto _match20570;
      }
      {
        _x20569 = kk_integer_from_small(-17); /*int*/
      }
      _match20570: ;
      _x20568 = kk_integer_box(_x20569); /*110*/
      _x20567 = kk_std_core_types__new_Optional(_x20568, _ctx); /*optional<110>*/
      return kk_std_core_show_fixed(d, _x20567, _ctx);
    }
    {
      int32_t _x20572;
      kk_integer_t _x20573;
      if (kk_std_core_types__is_Optional(precision)) {
        kk_box_t _box_x18700 = precision._cons.Optional.value;
        kk_integer_t _precision_87050 = kk_integer_unbox(_box_x18700);
        kk_integer_dup(_precision_87050);
        kk_std_core_types__optional_drop(precision, _ctx);
        _x20573 = _precision_87050; /*int*/
        goto _match20574;
      }
      {
        _x20573 = kk_integer_from_small(-17); /*int*/
      }
      _match20574: ;
      _x20572 = kk_std_core_int32(_x20573, _ctx); /*int32*/
      return kk_std_core_show_expx(d, _x20572, _ctx);
    }
  }
  {
    int32_t _x20576;
    kk_integer_t _x20577;
    if (kk_std_core_types__is_Optional(precision)) {
      kk_box_t _box_x18701 = precision._cons.Optional.value;
      kk_integer_t _precision_87051 = kk_integer_unbox(_box_x18701);
      kk_integer_dup(_precision_87051);
      kk_std_core_types__optional_drop(precision, _ctx);
      _x20577 = _precision_87051; /*int*/
      goto _match20578;
    }
    {
      _x20577 = kk_integer_from_small(-17); /*int*/
    }
    _match20578: ;
    _x20576 = kk_std_core_int32(_x20577, _ctx); /*int32*/
    return kk_std_core_show_expx(d, _x20576, _ctx);
  }
}
 
// lifted

kk_string_t kk_std_core__lift17272_show_3(kk_std_core__list ys, kk_string_t acc, kk_context_t* _ctx) { /* (ys : list<string>, acc : string) -> string */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20586 = kk_std_core__as_Cons(ys);
    kk_box_t _box_x18703 = _con20586->head;
    kk_std_core__list yy = _con20586->tail;
    kk_string_t y = kk_string_unbox(_box_x18703);
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      kk_std_core__list_free(ys);
    }
    else {
      kk_string_dup(y);
      kk_std_core__list_dup(yy);
      kk_std_core__list_decref(ys, _ctx);
    }
    { // tailcall
      kk_string_t _x20588;
      kk_string_t _x20589;
      kk_string_t _x20590 = kk_string_empty(); /*string*/
      _x20589 = kk_std_core__lp__plus__plus__1_rp_(_x20590, y, _ctx); /*string*/
      _x20588 = kk_std_core__lp__plus__plus__1_rp_(acc, _x20589, _ctx); /*string*/
      ys = yy;
      acc = _x20588;
      goto kk__tailcall;
    }
  }
  {
    return acc;
  }
}
 
// Show a string as a string literal


// lift anonymous function
struct kk_std_core_show_fun20596__t_3 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_show_fun20596_3(kk_function_t _fself, kk_box_t _b_18706, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_show_fun20596_3(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_show_fun20596_3, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_show_fun20596_3(kk_function_t _fself, kk_box_t _b_18706, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_string_t _x20597;
  kk_char_t _x20598 = kk_char_unbox(_b_18706, _ctx); /*char*/
  _x20597 = kk_std_core_show_char(_x20598, _ctx); /*string*/
  return kk_string_box(_x20597);
}

kk_string_t kk_std_core_show_3(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> string */ 
  kk_string_t _x20592;
  kk_define_string_literal(, _s20593, 1, "\"")
  _x20592 = kk_string_dup(_s20593); /*string*/
  kk_string_t _x20594;
  kk_string_t _x20595;
  kk_std_core__list _b_18707_18704 = kk_std_core_list_6(s, _ctx); /*list<char>*/;
  kk_std_core__list xs_17146 = kk_std_core_map_5(_b_18707_18704, kk_std_core_new_show_fun20596_3(_ctx), _ctx); /*list<string>*/;
  if (kk_std_core__is_Nil(xs_17146)) {
    _x20595 = kk_string_empty(); /*string*/
  }
  else {
    struct kk_std_core_Cons* _con20600 = kk_std_core__as_Cons(xs_17146);
    kk_box_t _box_x18709 = _con20600->head;
    kk_std_core__list xx = _con20600->tail;
    kk_string_t x = kk_string_unbox(_box_x18709);
    if (kk_likely(kk_std_core__list_is_unique(xs_17146))) {
      kk_std_core__list_free(xs_17146);
    }
    else {
      kk_string_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs_17146, _ctx);
    }
    _x20595 = kk_std_core__lift17272_show_3(xx, x, _ctx); /*string*/
  }
  kk_string_t _x20602;
  kk_define_string_literal(, _s20603, 1, "\"")
  _x20602 = kk_string_dup(_s20603); /*string*/
  _x20594 = kk_std_core__lp__plus__plus__1_rp_(_x20595, _x20602, _ctx); /*string*/
  return kk_std_core__lp__plus__plus__1_rp_(_x20592, _x20594, _ctx);
}
extern kk_string_t kk_std_core_show_fun20608_7(kk_function_t _fself, kk_box_t _b_18712, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_string_t _x20609 = kk_string_unbox(_b_18712); /*string*/
  return kk_std_core_show_3(_x20609, _ctx);
}
extern kk_string_t kk_std_core_show_fun20610_8(kk_function_t _fself, kk_box_t _b_18717, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_integer_t _x20611 = kk_integer_unbox(_b_18717); /*int*/
  return kk_std_core_show(_x20611, _ctx);
}
extern kk_string_t kk_std_core_show_fun20612_9(kk_function_t _fself, kk_box_t _b_18722, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  bool _x20613 = kk_bool_unbox(_b_18722); /*bool*/
  return kk_std_core_show_4(_x20613, _ctx);
}

kk_unit_t kk_std_core_prints(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> console () */ 
  kk_std_core_types__maybe _match_19769;
  kk_box_t _x20622;
  kk_ref_t _x20623 = kk_ref_dup(kk_std_core_redirect); /*ref<global,maybe<(string) -> console ()>>*/
  _x20622 = kk_ref_get(_x20623,kk_context()); /*184*/
  _match_19769 = kk_std_core_types__maybe_unbox(_x20622, _ctx); /*maybe<(string) -> console ()>*/
  if (kk_std_core_types__is_Nothing(_match_19769)) {
    kk_std_core_xprints(s, _ctx); return kk_Unit;
  }
  {
    kk_box_t _fun_unbox_x18729 = _match_19769._cons.Just.value;
    kk_box_t _x20624;
    kk_function_t _x20625 = kk_function_unbox(_fun_unbox_x18729); /*(18730) -> console 18731*/
    _x20624 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x20625, (_x20625, kk_string_box(s), _ctx)); /*18731*/
    kk_unit_unbox(_x20624); return kk_Unit;
  }
}


// lift anonymous function
struct kk_std_core__default_exn_fun20632__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core__default_exn_fun20632(kk_function_t _fself, kk_std_core_hnd__marker _b_18747, kk_std_core_hnd__ev _b_18748, kk_box_t _b_18749, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_default_exn_fun20632(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core__default_exn_fun20632, _ctx)
  return kk_function_dup(_fself);
}



// lift anonymous function
struct kk_std_core__default_exn_fun20633__t {
  struct kk_function_s _base;
  kk_box_t _b_18749;
};
static kk_box_t kk_std_core__default_exn_fun20633(kk_function_t _fself, kk_function_t _b_18744, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_default_exn_fun20633(kk_box_t _b_18749, kk_context_t* _ctx) {
  struct kk_std_core__default_exn_fun20633__t* _self = kk_function_alloc_as(struct kk_std_core__default_exn_fun20633__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__default_exn_fun20633, kk_context());
  _self->_b_18749 = _b_18749;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core__default_exn_fun20635__t {
  struct kk_function_s _base;
  kk_function_t _b_18744;
};
static kk_unit_t kk_std_core__default_exn_fun20635(kk_function_t _fself, kk_std_core_hnd__resume_result _b_18745, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_default_exn_fun20635(kk_function_t _b_18744, kk_context_t* _ctx) {
  struct kk_std_core__default_exn_fun20635__t* _self = kk_function_alloc_as(struct kk_std_core__default_exn_fun20635__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__default_exn_fun20635, kk_context());
  _self->_b_18744 = _b_18744;
  return &_self->_base;
}

static kk_unit_t kk_std_core__default_exn_fun20635(kk_function_t _fself, kk_std_core_hnd__resume_result _b_18745, kk_context_t* _ctx) {
  struct kk_std_core__default_exn_fun20635__t* _self = kk_function_as(struct kk_std_core__default_exn_fun20635__t*, _fself);
  kk_function_t _b_18744 = _self->_b_18744; /* (std/core/hnd/resume-result<3924,3927>) -> 3926 3927 */
  kk_drop_match(_self, {kk_function_dup(_b_18744);}, {}, _ctx)
  kk_box_t _x20636 = kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__resume_result, kk_context_t*), _b_18744, (_b_18744, _b_18745, _ctx)); /*3927*/
  return kk_unit_unbox(_x20636);
}
static kk_box_t kk_std_core__default_exn_fun20633(kk_function_t _fself, kk_function_t _b_18744, kk_context_t* _ctx) {
  struct kk_std_core__default_exn_fun20633__t* _self = kk_function_as(struct kk_std_core__default_exn_fun20633__t*, _fself);
  kk_box_t _b_18749 = _self->_b_18749; /* 51 */
  kk_drop_match(_self, {kk_box_dup(_b_18749);}, {}, _ctx)
  kk_unit_t _x20634 = kk_Unit;
  kk_function_t ___wildcard__585__45_18766 = kk_std_core__new_default_exn_fun20635(_b_18744, _ctx); /*(std/core/hnd/resume-result<9759,()>) -> <console|9757> ()*/;
  kk_function_drop(___wildcard__585__45_18766, _ctx);
  kk_unit_t __ = kk_Unit;
  kk_string_t _x20637;
  kk_define_string_literal(, _s20638, 20, "uncaught exception: ")
  _x20637 = kk_string_dup(_s20638); /*string*/
  kk_std_core_prints(_x20637, _ctx);
  kk_string_t _x20639;
  kk_std_core__exception _match_19766 = kk_std_core__exception_unbox(_b_18749, _ctx); /*exception*/;
  {
    kk_string_t _x = _match_19766.message;
    kk_string_dup(_x);
    kk_std_core__exception_drop(_match_19766, _ctx);
    _x20639 = _x; /*string*/
  }
  kk_std_core_printsln(_x20639, _ctx);
  return kk_unit_box(_x20634);
}
static kk_box_t kk_std_core__default_exn_fun20632(kk_function_t _fself, kk_std_core_hnd__marker _b_18747, kk_std_core_hnd__ev _b_18748, kk_box_t _b_18749, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core_hnd__ev_dropn(_b_18748, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to_final(_b_18747, kk_std_core__new_default_exn_fun20633(_b_18749, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core__default_exn_fun20640__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core__default_exn_fun20640(kk_function_t _fself, kk_box_t _b_18754, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_default_exn_fun20640(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core__default_exn_fun20640, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core__default_exn_fun20640(kk_function_t _fself, kk_box_t _b_18754, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _b_18754;
}


// lift anonymous function
struct kk_std_core__default_exn_fun20641__t {
  struct kk_function_s _base;
  kk_function_t action;
};
static kk_box_t kk_std_core__default_exn_fun20641(kk_function_t _fself, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_default_exn_fun20641(kk_function_t action, kk_context_t* _ctx) {
  struct kk_std_core__default_exn_fun20641__t* _self = kk_function_alloc_as(struct kk_std_core__default_exn_fun20641__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__default_exn_fun20641, kk_context());
  _self->action = action;
  return &_self->_base;
}

static kk_box_t kk_std_core__default_exn_fun20641(kk_function_t _fself, kk_context_t* _ctx) {
  struct kk_std_core__default_exn_fun20641__t* _self = kk_function_as(struct kk_std_core__default_exn_fun20641__t*, _fself);
  kk_function_t action = _self->action; /* () -> <exn,console|9757> () */
  kk_drop_match(_self, {kk_function_dup(action);}, {}, _ctx)
  kk_unit_t _x20642 = kk_Unit;
  kk_function_call(kk_unit_t, (kk_function_t, kk_context_t*), action, (action, _ctx));
  return kk_unit_box(_x20642);
}

kk_unit_t kk_std_core__default_exn(kk_function_t action, kk_context_t* _ctx) { /* forall<e> (action : () -> <console,exn|e> ()) -> <console|e> () */ 
  int32_t _b_18755_18750 = ((int32_t)KI32(0)); /*int32*/;
  kk_box_t _x20629;
  kk_std_core__hnd_exn _x20630;
  kk_std_core_hnd__clause1 _x20631 = kk_std_core_hnd__new_Clause1(kk_std_core__new_default_exn_fun20632(_ctx), _ctx); /*std/core/hnd/clause1<51,52,53,54,55>*/
  _x20630 = kk_std_core__new_Hnd_exn(kk_reuse_null, _x20631, _ctx); /*.hnd-exn<11,12>*/
  _x20629 = kk_std_core__handle_exn(_b_18755_18750, _x20630, kk_std_core__new_default_exn_fun20640(_ctx), kk_std_core__new_default_exn_fun20641(action, _ctx), _ctx); /*1971*/
  kk_unit_unbox(_x20629); return kk_Unit;
}
 
// Get (zero-based) element `n`  of a list. Return a `:maybe` type.

kk_std_core_types__maybe kk_std_core__lp__lb__rb__2_rp_(kk_std_core__list xs, kk_integer_t n, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, n : int) -> maybe<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20643 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20643->head;
    kk_std_core__list xx = _con20643->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    bool _match_19760;
    kk_integer_t _brw_19764 = kk_integer_from_small(0); /*int*/;
    bool _brw_19765 = kk_integer_gt_borrow(n,_brw_19764,kk_context()); /*bool*/;
    kk_integer_drop(_brw_19764, _ctx);
    _match_19760 = _brw_19765; /*bool*/
    if (_match_19760) {
      kk_box_drop(x, _ctx);
      { // tailcall
        kk_integer_t _x20644 = kk_integer_sub(n,(kk_integer_from_small(1)),kk_context()); /*int*/
        xs = xx;
        n = _x20644;
        goto kk__tailcall;
      }
    }
    {
      kk_std_core__list_drop(xx, _ctx);
      bool _match_19761;
      kk_integer_t _brw_19762 = kk_integer_from_small(0); /*int*/;
      bool _brw_19763 = kk_integer_eq_borrow(n,_brw_19762,kk_context()); /*bool*/;
      kk_integer_drop(n, _ctx);
      kk_integer_drop(_brw_19762, _ctx);
      _match_19761 = _brw_19763; /*bool*/
      if (_match_19761) {
        return kk_std_core_types__new_Just(x, _ctx);
      }
      {
        kk_box_drop(x, _ctx);
        return kk_std_core_types__new_Nothing(_ctx);
      }
    }
  }
  {
    kk_integer_drop(n, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
}
 
// O(1). Return the string slice from the end of `slice` argument
// to the end of the string.

kk_std_core__sslice kk_std_core_after(kk_std_core__sslice slice0, kk_context_t* _ctx) { /* (slice : sslice) -> sslice */ 
  {
    kk_string_t s = slice0.str;
    kk_ssize_t start0 = slice0.start;
    kk_ssize_t len0 = slice0.len;
    kk_string_dup(s);
    kk_std_core__sslice_drop(slice0, _ctx);
    kk_string_t _x20645 = kk_string_dup(s); /*string*/
    kk_ssize_t _x20646 = (start0 + len0); /*ssize_t*/
    kk_ssize_t _x20647;
    kk_ssize_t _x20648 = kk_string_len(s,kk_context()); /*ssize_t*/
    kk_ssize_t _x20649 = (start0 + len0); /*ssize_t*/
    _x20647 = (_x20648 - _x20649); /*ssize_t*/
    return kk_std_core__new_Sslice(_x20645, _x20646, _x20647, _ctx);
  }
}
 
// monadic lift

bool kk_std_core__mlift17720_all(kk_function_t predicate, kk_std_core__list xx, bool _y_17476, kk_context_t* _ctx) { /* forall<a,e> (predicate : (a) -> e bool, xx : list<a>, bool) -> e bool */ 
  if (_y_17476) {
    return kk_std_core_all(xx, predicate, _ctx);
  }
  {
    kk_std_core__list_drop(xx, _ctx);
    kk_function_drop(predicate, _ctx);
    return false;
  }
}
 
// Do all elements satisfy a predicate ?


// lift anonymous function
struct kk_std_core_all_fun20653__t {
  struct kk_function_s _base;
  kk_function_t predicate0;
  kk_std_core__list xx0;
};
static kk_box_t kk_std_core_all_fun20653(kk_function_t _fself, kk_box_t _b_18768, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_all_fun20653(kk_function_t predicate0, kk_std_core__list xx0, kk_context_t* _ctx) {
  struct kk_std_core_all_fun20653__t* _self = kk_function_alloc_as(struct kk_std_core_all_fun20653__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_all_fun20653, kk_context());
  _self->predicate0 = predicate0;
  _self->xx0 = xx0;
  return &_self->_base;
}

static kk_box_t kk_std_core_all_fun20653(kk_function_t _fself, kk_box_t _b_18768, kk_context_t* _ctx) {
  struct kk_std_core_all_fun20653__t* _self = kk_function_as(struct kk_std_core_all_fun20653__t*, _fself);
  kk_function_t predicate0 = _self->predicate0; /* (9994) -> 9995 bool */
  kk_std_core__list xx0 = _self->xx0; /* list<9994> */
  kk_drop_match(_self, {kk_function_dup(predicate0);kk_std_core__list_dup(xx0);}, {}, _ctx)
  bool _x20654;
  bool _x20655 = kk_bool_unbox(_b_18768); /*bool*/
  _x20654 = kk_std_core__mlift17720_all(predicate0, xx0, _x20655, _ctx); /*bool*/
  return kk_bool_box(_x20654);
}

bool kk_std_core_all(kk_std_core__list xs, kk_function_t predicate0, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, predicate : (a) -> e bool) -> e bool */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20650 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20650->head;
    kk_std_core__list xx0 = _con20650->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx0);
      kk_std_core__list_decref(xs, _ctx);
    }
    bool x0_17933;
    kk_function_t _x20651 = kk_function_dup(predicate0); /*(9994) -> 9995 bool*/
    x0_17933 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), _x20651, (_x20651, x, _ctx)); /*bool*/
    if (kk_yielding(kk_context())) {
      kk_box_t _x20652 = kk_std_core_hnd_yield_extend(kk_std_core_new_all_fun20653(predicate0, xx0, _ctx), _ctx); /*3860*/
      return kk_bool_unbox(_x20652);
    }
    if (x0_17933) { // tailcall
                    xs = xx0;
                    goto kk__tailcall;
    }
    {
      kk_std_core__list_drop(xx0, _ctx);
      kk_function_drop(predicate0, _ctx);
      return false;
    }
  }
  {
    kk_function_drop(predicate0, _ctx);
    return true;
  }
}
 
// monadic lift

bool kk_std_core__mlift17721_any(kk_function_t predicate, kk_std_core__list xx, bool _y_17480, kk_context_t* _ctx) { /* forall<a,e> (predicate : (a) -> e bool, xx : list<a>, bool) -> e bool */ 
  if (_y_17480) {
    kk_std_core__list_drop(xx, _ctx);
    kk_function_drop(predicate, _ctx);
    return true;
  }
  {
    return kk_std_core_any(xx, predicate, _ctx);
  }
}
 
// Are there any elements in a list that satisfy a predicate ?


// lift anonymous function
struct kk_std_core_any_fun20659__t {
  struct kk_function_s _base;
  kk_function_t predicate0;
  kk_std_core__list xx0;
};
static kk_box_t kk_std_core_any_fun20659(kk_function_t _fself, kk_box_t _b_18772, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_any_fun20659(kk_function_t predicate0, kk_std_core__list xx0, kk_context_t* _ctx) {
  struct kk_std_core_any_fun20659__t* _self = kk_function_alloc_as(struct kk_std_core_any_fun20659__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_any_fun20659, kk_context());
  _self->predicate0 = predicate0;
  _self->xx0 = xx0;
  return &_self->_base;
}

static kk_box_t kk_std_core_any_fun20659(kk_function_t _fself, kk_box_t _b_18772, kk_context_t* _ctx) {
  struct kk_std_core_any_fun20659__t* _self = kk_function_as(struct kk_std_core_any_fun20659__t*, _fself);
  kk_function_t predicate0 = _self->predicate0; /* (10022) -> 10023 bool */
  kk_std_core__list xx0 = _self->xx0; /* list<10022> */
  kk_drop_match(_self, {kk_function_dup(predicate0);kk_std_core__list_dup(xx0);}, {}, _ctx)
  bool _x20660;
  bool _x20661 = kk_bool_unbox(_b_18772); /*bool*/
  _x20660 = kk_std_core__mlift17721_any(predicate0, xx0, _x20661, _ctx); /*bool*/
  return kk_bool_box(_x20660);
}

bool kk_std_core_any(kk_std_core__list xs, kk_function_t predicate0, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, predicate : (a) -> e bool) -> e bool */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20656 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20656->head;
    kk_std_core__list xx0 = _con20656->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx0);
      kk_std_core__list_decref(xs, _ctx);
    }
    bool x0_17936;
    kk_function_t _x20657 = kk_function_dup(predicate0); /*(10022) -> 10023 bool*/
    x0_17936 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), _x20657, (_x20657, x, _ctx)); /*bool*/
    if (kk_yielding(kk_context())) {
      kk_box_t _x20658 = kk_std_core_hnd_yield_extend(kk_std_core_new_any_fun20659(predicate0, xx0, _ctx), _ctx); /*3860*/
      return kk_bool_unbox(_x20658);
    }
    if (x0_17936) {
      kk_std_core__list_drop(xx0, _ctx);
      kk_function_drop(predicate0, _ctx);
      return true;
    }
    { // tailcall
      xs = xx0;
      goto kk__tailcall;
    }
  }
  {
    kk_function_drop(predicate0, _ctx);
    return false;
  }
}
 
// Return the element at position `index` in vector `v`, or `Nothing` if out of bounds

kk_std_core_types__maybe kk_std_core_at(kk_vector_t v, kk_integer_t index, kk_context_t* _ctx) { /* forall<a> (v : vector<a>, index : int) -> maybe<a> */ 
  kk_ssize_t idx;
  kk_integer_t _x20662 = kk_integer_dup(index); /*int*/
  idx = kk_std_core_ssize__t(_x20662, _ctx); /*ssize_t*/
  bool _match_19757;
  kk_ssize_t _x20663 = kk_vector_len_borrow(v); /*ssize_t*/
  _match_19757 = (_x20663 <= idx); /*bool*/
  if (_match_19757) {
    return kk_std_core_types__new_Nothing(_ctx);
  }
  {
    kk_box_t _x20664 = kk_vector_at_borrow(v, idx); /*223*/
    return kk_std_core_types__new_Just(_x20664, _ctx);
  }
}
 
// O(`n`). The first `n` (default = `1`) characters in a string.

kk_std_core__sslice kk_std_core_first(kk_string_t s, kk_std_core_types__optional n, kk_context_t* _ctx) { /* (s : string, n : optional<int>) -> sslice */ 
  kk_std_core__sslice slice0 = kk_std_core_first1(s, _ctx); /*sslice*/;
  bool _match_19749;
  kk_integer_t _brw_19753;
  if (kk_std_core_types__is_Optional(n)) {
    kk_box_t _box_x18775 = n._cons.Optional.value;
    kk_integer_t _n_10165 = kk_integer_unbox(_box_x18775);
    kk_integer_dup(_n_10165);
    _brw_19753 = _n_10165; /*int*/
    goto _match20668;
  }
  {
    _brw_19753 = kk_integer_from_small(1); /*int*/
  }
  _match20668: ;
  kk_integer_t _brw_19752 = kk_integer_from_small(1); /*int*/;
  bool _brw_19754 = kk_integer_eq_borrow(_brw_19753,_brw_19752,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19753, _ctx);
  kk_integer_drop(_brw_19752, _ctx);
  _match_19749 = _brw_19754; /*bool*/
  if (_match_19749) {
    kk_std_core_types__optional_drop(n, _ctx);
    return slice0;
  }
  {
    kk_integer_t _brw_19750;
    kk_integer_t _x20670;
    if (kk_std_core_types__is_Optional(n)) {
      kk_box_t _box_x18776 = n._cons.Optional.value;
      kk_integer_t _n_101650 = kk_integer_unbox(_box_x18776);
      kk_integer_dup(_n_101650);
      kk_std_core_types__optional_drop(n, _ctx);
      _x20670 = _n_101650; /*int*/
      goto _match20671;
    }
    {
      _x20670 = kk_integer_from_small(1); /*int*/
    }
    _match20671: ;
    _brw_19750 = kk_integer_sub(_x20670,(kk_integer_from_small(1)),kk_context()); /*int*/
    kk_std_core__sslice _brw_19751 = kk_std_core_extend(slice0, _brw_19750, _ctx); /*sslice*/;
    kk_integer_drop(_brw_19750, _ctx);
    return _brw_19751;
  }
}
 
// Convert the first character of a string to uppercase.

kk_string_t kk_std_core_capitalize(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> string */ 
  kk_string_t _x20673;
  kk_string_t _x20674;
  kk_std_core__sslice _x20675;
  kk_std_core__sslice slice0;
  kk_string_t _x20676 = kk_string_dup(s); /*string*/
  slice0 = kk_std_core_first1(_x20676, _ctx); /*sslice*/
  bool _match_19741;
  kk_integer_t _brw_19746;
  kk_std_core_types__optional _match_19747 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
  if (kk_std_core_types__is_Optional(_match_19747)) {
    kk_box_t _box_x18777 = _match_19747._cons.Optional.value;
    kk_integer_t _n_10165 = kk_integer_unbox(_box_x18777);
    kk_integer_dup(_n_10165);
    kk_std_core_types__optional_drop(_match_19747, _ctx);
    _brw_19746 = _n_10165; /*int*/
    goto _match20677;
  }
  {
    _brw_19746 = kk_integer_from_small(1); /*int*/
  }
  _match20677: ;
  kk_integer_t _brw_19745 = kk_integer_from_small(1); /*int*/;
  bool _brw_19748 = kk_integer_eq_borrow(_brw_19746,_brw_19745,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19746, _ctx);
  kk_integer_drop(_brw_19745, _ctx);
  _match_19741 = _brw_19748; /*bool*/
  if (_match_19741) {
    _x20675 = slice0; /*sslice*/
  }
  else {
    kk_integer_t _brw_19742;
    kk_integer_t _x20679;
    kk_std_core_types__optional _match_19743 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
    if (kk_std_core_types__is_Optional(_match_19743)) {
      kk_box_t _box_x18778 = _match_19743._cons.Optional.value;
      kk_integer_t _n_101650 = kk_integer_unbox(_box_x18778);
      kk_integer_dup(_n_101650);
      kk_std_core_types__optional_drop(_match_19743, _ctx);
      _x20679 = _n_101650; /*int*/
      goto _match20680;
    }
    {
      _x20679 = kk_integer_from_small(1); /*int*/
    }
    _match20680: ;
    _brw_19742 = kk_integer_sub(_x20679,(kk_integer_from_small(1)),kk_context()); /*int*/
    kk_std_core__sslice _brw_19744 = kk_std_core_extend(slice0, _brw_19742, _ctx); /*sslice*/;
    kk_integer_drop(_brw_19742, _ctx);
    _x20675 = _brw_19744; /*sslice*/
  }
  _x20674 = kk_std_core_string_3(_x20675, _ctx); /*string*/
  _x20673 = kk_std_core_to_upper(_x20674, _ctx); /*string*/
  kk_string_t _x20682;
  kk_std_core__sslice _x20683;
  kk_std_core__sslice slice2 = kk_std_core_first1(s, _ctx); /*sslice*/;
  kk_std_core__sslice slice1_17155;
  bool _match_19733;
  kk_integer_t _brw_19738;
  kk_std_core_types__optional _match_19739 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
  if (kk_std_core_types__is_Optional(_match_19739)) {
    kk_box_t _box_x18779 = _match_19739._cons.Optional.value;
    kk_integer_t _n_1016500 = kk_integer_unbox(_box_x18779);
    kk_integer_dup(_n_1016500);
    kk_std_core_types__optional_drop(_match_19739, _ctx);
    _brw_19738 = _n_1016500; /*int*/
    goto _match20684;
  }
  {
    _brw_19738 = kk_integer_from_small(1); /*int*/
  }
  _match20684: ;
  kk_integer_t _brw_19737 = kk_integer_from_small(1); /*int*/;
  bool _brw_19740 = kk_integer_eq_borrow(_brw_19738,_brw_19737,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19738, _ctx);
  kk_integer_drop(_brw_19737, _ctx);
  _match_19733 = _brw_19740; /*bool*/
  if (_match_19733) {
    slice1_17155 = slice2; /*sslice*/
  }
  else {
    kk_integer_t _brw_19734;
    kk_integer_t _x20686;
    kk_std_core_types__optional _match_19735 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
    if (kk_std_core_types__is_Optional(_match_19735)) {
      kk_box_t _box_x18780 = _match_19735._cons.Optional.value;
      kk_integer_t _n_1016501 = kk_integer_unbox(_box_x18780);
      kk_integer_dup(_n_1016501);
      kk_std_core_types__optional_drop(_match_19735, _ctx);
      _x20686 = _n_1016501; /*int*/
      goto _match20687;
    }
    {
      _x20686 = kk_integer_from_small(1); /*int*/
    }
    _match20687: ;
    _brw_19734 = kk_integer_sub(_x20686,(kk_integer_from_small(1)),kk_context()); /*int*/
    kk_std_core__sslice _brw_19736 = kk_std_core_extend(slice2, _brw_19734, _ctx); /*sslice*/;
    kk_integer_drop(_brw_19734, _ctx);
    slice1_17155 = _brw_19736; /*sslice*/
  }
  {
    kk_string_t s1 = slice1_17155.str;
    kk_ssize_t start0 = slice1_17155.start;
    kk_ssize_t len0 = slice1_17155.len;
    kk_string_dup(s1);
    kk_std_core__sslice_drop(slice1_17155, _ctx);
    kk_string_t _x20689 = kk_string_dup(s1); /*string*/
    kk_ssize_t _x20690 = (start0 + len0); /*ssize_t*/
    kk_ssize_t _x20691;
    kk_ssize_t _x20692 = kk_string_len(s1,kk_context()); /*ssize_t*/
    kk_ssize_t _x20693 = (start0 + len0); /*ssize_t*/
    _x20691 = (_x20692 - _x20693); /*ssize_t*/
    _x20683 = kk_std_core__new_Sslice(_x20689, _x20690, _x20691, _ctx); /*sslice*/
  }
  _x20682 = kk_std_core_string_3(_x20683, _ctx); /*string*/
  return kk_std_core__lp__plus__plus__1_rp_(_x20673, _x20682, _ctx);
}
 
// Catch any exception raised in `action` and handle it.
// Use `on-exn` or `on-exit` when appropiate.


// lift anonymous function
struct kk_std_core_try_fun20696__t {
  struct kk_function_s _base;
  kk_function_t hndl;
};
static kk_box_t kk_std_core_try_fun20696(kk_function_t _fself, kk_std_core_hnd__marker _b_18782, kk_std_core_hnd__ev _b_18783, kk_box_t _b_18784, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_fun20696(kk_function_t hndl, kk_context_t* _ctx) {
  struct kk_std_core_try_fun20696__t* _self = kk_function_alloc_as(struct kk_std_core_try_fun20696__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_try_fun20696, kk_context());
  _self->hndl = hndl;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_try_fun20697__t {
  struct kk_function_s _base;
  kk_box_t _b_18784;
  kk_function_t hndl;
};
static kk_box_t kk_std_core_try_fun20697(kk_function_t _fself, kk_function_t ___wildcard__585__45, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_fun20697(kk_box_t _b_18784, kk_function_t hndl, kk_context_t* _ctx) {
  struct kk_std_core_try_fun20697__t* _self = kk_function_alloc_as(struct kk_std_core_try_fun20697__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_try_fun20697, kk_context());
  _self->_b_18784 = _b_18784;
  _self->hndl = hndl;
  return &_self->_base;
}

static kk_box_t kk_std_core_try_fun20697(kk_function_t _fself, kk_function_t ___wildcard__585__45, kk_context_t* _ctx) {
  struct kk_std_core_try_fun20697__t* _self = kk_function_as(struct kk_std_core_try_fun20697__t*, _fself);
  kk_box_t _b_18784 = _self->_b_18784; /* 51 */
  kk_function_t hndl = _self->hndl; /* (exception) -> 10332 10331 */
  kk_drop_match(_self, {kk_box_dup(_b_18784);kk_function_dup(hndl);}, {}, _ctx)
  kk_function_drop(___wildcard__585__45, _ctx);
  kk_std_core__exception _x20698 = kk_std_core__exception_unbox(_b_18784, _ctx); /*exception*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_std_core__exception, kk_context_t*), hndl, (hndl, _x20698, _ctx));
}
static kk_box_t kk_std_core_try_fun20696(kk_function_t _fself, kk_std_core_hnd__marker _b_18782, kk_std_core_hnd__ev _b_18783, kk_box_t _b_18784, kk_context_t* _ctx) {
  struct kk_std_core_try_fun20696__t* _self = kk_function_as(struct kk_std_core_try_fun20696__t*, _fself);
  kk_function_t hndl = _self->hndl; /* (exception) -> 10332 10331 */
  kk_drop_match(_self, {kk_function_dup(hndl);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(_b_18783, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to_final(_b_18782, kk_std_core_new_try_fun20697(_b_18784, hndl, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_try_fun20699__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_try_fun20699(kk_function_t _fself, kk_box_t _x, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_fun20699(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_try_fun20699, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_try_fun20699(kk_function_t _fself, kk_box_t _x, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _x;
}

kk_box_t kk_std_core_try(kk_function_t action, kk_function_t hndl, kk_context_t* _ctx) { /* forall<a,e> (action : () -> <exn|e> a, hndl : (exception) -> e a) -> e a */ 
  kk_std_core__hnd_exn _x20694;
  kk_std_core_hnd__clause1 _x20695 = kk_std_core_hnd__new_Clause1(kk_std_core_new_try_fun20696(hndl, _ctx), _ctx); /*std/core/hnd/clause1<51,52,53,54,55>*/
  _x20694 = kk_std_core__new_Hnd_exn(kk_reuse_null, _x20695, _ctx); /*.hnd-exn<11,12>*/
  return kk_std_core__handle_exn(((int32_t)KI32(0)), _x20694, kk_std_core_new_try_fun20699(_ctx), action, _ctx);
}
 
// Transform an exception effect to an  `:error` type.


// lift anonymous function
struct kk_std_core_try_fun20703__t_1 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_try_fun20703_1(kk_function_t _fself, kk_std_core_hnd__marker _b_18794, kk_std_core_hnd__ev _b_18795, kk_box_t _b_18796, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_fun20703_1(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_try_fun20703_1, _ctx)
  return kk_function_dup(_fself);
}



// lift anonymous function
struct kk_std_core_try_fun20704__t_1 {
  struct kk_function_s _base;
  kk_box_t _b_18796;
};
static kk_box_t kk_std_core_try_fun20704_1(kk_function_t _fself, kk_function_t _b_18791, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_fun20704_1(kk_box_t _b_18796, kk_context_t* _ctx) {
  struct kk_std_core_try_fun20704__t_1* _self = kk_function_alloc_as(struct kk_std_core_try_fun20704__t_1, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_try_fun20704_1, kk_context());
  _self->_b_18796 = _b_18796;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_try_fun20706__t_1 {
  struct kk_function_s _base;
  kk_function_t _b_18791;
};
static kk_std_core__error kk_std_core_try_fun20706_1(kk_function_t _fself, kk_std_core_hnd__resume_result _b_18792, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_fun20706_1(kk_function_t _b_18791, kk_context_t* _ctx) {
  struct kk_std_core_try_fun20706__t_1* _self = kk_function_alloc_as(struct kk_std_core_try_fun20706__t_1, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_try_fun20706_1, kk_context());
  _self->_b_18791 = _b_18791;
  return &_self->_base;
}

static kk_std_core__error kk_std_core_try_fun20706_1(kk_function_t _fself, kk_std_core_hnd__resume_result _b_18792, kk_context_t* _ctx) {
  struct kk_std_core_try_fun20706__t_1* _self = kk_function_as(struct kk_std_core_try_fun20706__t_1*, _fself);
  kk_function_t _b_18791 = _self->_b_18791; /* (std/core/hnd/resume-result<3924,3927>) -> 3926 3927 */
  kk_drop_match(_self, {kk_function_dup(_b_18791);}, {}, _ctx)
  kk_box_t _x20707 = kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__resume_result, kk_context_t*), _b_18791, (_b_18791, _b_18792, _ctx)); /*3927*/
  return kk_std_core__error_unbox(_x20707, _ctx);
}
static kk_box_t kk_std_core_try_fun20704_1(kk_function_t _fself, kk_function_t _b_18791, kk_context_t* _ctx) {
  struct kk_std_core_try_fun20704__t_1* _self = kk_function_as(struct kk_std_core_try_fun20704__t_1*, _fself);
  kk_box_t _b_18796 = _self->_b_18796; /* 51 */
  kk_drop_match(_self, {kk_box_dup(_b_18796);}, {}, _ctx)
  kk_std_core__error _x20705;
  kk_function_t ___wildcard__585__45_18817 = kk_std_core_new_try_fun20706_1(_b_18791, _ctx); /*(std/core/hnd/resume-result<10318,error<10370>>) -> 10371 error<10370>*/;
  kk_function_drop(___wildcard__585__45_18817, _ctx);
  kk_std_core__exception _x20708 = kk_std_core__exception_unbox(_b_18796, _ctx); /*exception*/
  _x20705 = kk_std_core__new_Error(_x20708, _ctx); /*error<30>*/
  return kk_std_core__error_box(_x20705, _ctx);
}
static kk_box_t kk_std_core_try_fun20703_1(kk_function_t _fself, kk_std_core_hnd__marker _b_18794, kk_std_core_hnd__ev _b_18795, kk_box_t _b_18796, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core_hnd__ev_dropn(_b_18795, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to_final(_b_18794, kk_std_core_new_try_fun20704_1(_b_18796, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_try_fun20709__t_1 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_try_fun20709_1(kk_function_t _fself, kk_box_t _b_18803, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_fun20709_1(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_try_fun20709_1, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_try_fun20709_1(kk_function_t _fself, kk_box_t _b_18803, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _b_18803;
}


// lift anonymous function
struct kk_std_core_try_fun20710__t_1 {
  struct kk_function_s _base;
  kk_function_t action;
};
static kk_box_t kk_std_core_try_fun20710_1(kk_function_t _fself, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_fun20710_1(kk_function_t action, kk_context_t* _ctx) {
  struct kk_std_core_try_fun20710__t_1* _self = kk_function_alloc_as(struct kk_std_core_try_fun20710__t_1, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_try_fun20710_1, kk_context());
  _self->action = action;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_try_fun20713__t_1 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_try_fun20713_1(kk_function_t _fself, kk_box_t _b_18798, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_fun20713_1(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_try_fun20713_1, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_try_fun20713_1(kk_function_t _fself, kk_box_t _b_18798, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core__error _x20714 = kk_std_core__new_Ok(_b_18798, _ctx); /*error<30>*/
  return kk_std_core__error_box(_x20714, _ctx);
}
static kk_box_t kk_std_core_try_fun20710_1(kk_function_t _fself, kk_context_t* _ctx) {
  struct kk_std_core_try_fun20710__t_1* _self = kk_function_as(struct kk_std_core_try_fun20710__t_1*, _fself);
  kk_function_t action = _self->action; /* () -> <exn|10371> 10370 */
  kk_drop_match(_self, {kk_function_dup(action);}, {}, _ctx)
  kk_std_core__error _x20711;
  kk_box_t x0_17941 = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), action, (action, _ctx)); /*10370*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(x0_17941, _ctx);
    kk_box_t _x20712 = kk_std_core_hnd_yield_extend(kk_std_core_new_try_fun20713_1(_ctx), _ctx); /*3860*/
    _x20711 = kk_std_core__error_unbox(_x20712, _ctx); /*error<10370>*/
  }
  else {
    _x20711 = kk_std_core__new_Ok(x0_17941, _ctx); /*error<10370>*/
  }
  return kk_std_core__error_box(_x20711, _ctx);
}

kk_std_core__error kk_std_core_try_1(kk_function_t action, kk_context_t* _ctx) { /* forall<a,e> (action : () -> <exn|e> a) -> e error<a> */ 
  int32_t _b_18804_18799 = ((int32_t)KI32(0)); /*int32*/;
  kk_box_t _x20700;
  kk_std_core__hnd_exn _x20701;
  kk_std_core_hnd__clause1 _x20702 = kk_std_core_hnd__new_Clause1(kk_std_core_new_try_fun20703_1(_ctx), _ctx); /*std/core/hnd/clause1<51,52,53,54,55>*/
  _x20701 = kk_std_core__new_Hnd_exn(kk_reuse_null, _x20702, _ctx); /*.hnd-exn<11,12>*/
  _x20700 = kk_std_core__handle_exn(_b_18804_18799, _x20701, kk_std_core_new_try_fun20709_1(_ctx), kk_std_core_new_try_fun20710_1(action, _ctx), _ctx); /*1971*/
  return kk_std_core__error_unbox(_x20700, _ctx);
}

kk_std_core_types__tuple2_ kk_std_core_cdivmod_exp10(kk_integer_t i, kk_integer_t n, kk_context_t* _ctx) { /* (i : int, n : int) -> (int, int) */ 
  bool _match_19729;
  kk_integer_t _brw_19730 = kk_integer_from_small(0); /*int*/;
  bool _brw_19731 = kk_integer_lte_borrow(n,_brw_19730,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19730, _ctx);
  _match_19729 = _brw_19731; /*bool*/
  if (_match_19729) {
    kk_integer_drop(n, _ctx);
    return kk_std_core_types__new_dash__lp__comma__rp_(kk_integer_box(i), kk_integer_box(kk_integer_from_small(0)), _ctx);
  }
  {
    kk_integer_t cq;
    kk_integer_t _x20715 = kk_integer_dup(i); /*int*/
    kk_integer_t _x20716 = kk_integer_dup(n); /*int*/
    cq = kk_std_core_cdiv_exp10(_x20715, _x20716, _ctx); /*int*/
    kk_integer_t cr;
    kk_integer_t _x20717;
    kk_integer_t _x20718 = kk_integer_dup(cq); /*int*/
    _x20717 = kk_std_core_mul_exp10(_x20718, n, _ctx); /*int*/
    cr = kk_integer_sub(i,_x20717,kk_context()); /*int*/
    return kk_std_core_types__new_dash__lp__comma__rp_(kk_integer_box(cq), kk_integer_box(cr), _ctx);
  }
}
 
// Concatenate a list of `:maybe` values

kk_std_core__list kk_std_core__ctail_concat_maybe(kk_std_core__list xs, kk_std_core_types__ctail _acc, kk_context_t* _ctx) { /* forall<a> (xs : list<maybe<a>>, ctail<list<a>>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20719 = kk_std_core__as_Cons(xs);
    kk_box_t _box_x18826 = _con20719->head;
    kk_std_core__list xx = _con20719->tail;
    kk_std_core_types__maybe x = kk_std_core_types__maybe_unbox(_box_x18826, NULL);
    kk_reuse_t _ru_19972 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core_types__maybe_dup(x);
      kk_box_drop(_box_x18826, _ctx);
      _ru_19972 = (kk_std_core__list_reuse(xs));
    }
    else {
      kk_std_core_types__maybe_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    if (kk_std_core_types__is_Just(x)) {
      kk_box_t y = x._cons.Just.value;
      kk_std_core__list _ctail_17351 = kk_std_core__list_hole(); /*list<10510>*/;
      kk_std_core__list _ctail_17352 = kk_std_core__new_Cons(_ru_19972, y, _ctail_17351, _ctx); /*list<10510>*/;
      { // tailcall
        kk_std_core_types__ctail _x20721;
        kk_box_t* _b_18837_18832 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17352)->tail)); /*cfield<list<10510>>*/;
        _x20721 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17352, _ctx)),_b_18837_18832); /*ctail<0>*/
        xs = xx;
        _acc = _x20721;
        goto kk__tailcall;
      }
    }
    {
      kk_reuse_drop(_ru_19972);
      { // tailcall
        xs = xx;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_box_t _x20722 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20722, _ctx);
  }
}
 
// Concatenate a list of `:maybe` values

kk_std_core__list kk_std_core_concat_maybe(kk_std_core__list xs0, kk_context_t* _ctx) { /* forall<a> (xs : list<maybe<a>>) -> list<a> */ 
  kk_std_core_types__ctail _x20723 = kk_ctail_nil(); /*ctail<0>*/
  return kk_std_core__ctail_concat_maybe(xs0, _x20723, _ctx);
}
 
// monadic lift

kk_std_core_types__maybe kk_std_core__mlift17723_op(kk_function_t action, kk_ssize_t end, kk_ssize_t i, kk_std_core_types__maybe _y_17488, kk_context_t* _ctx) { /* forall<a,e> (action : (ssize_t) -> e maybe<a>, end : ssize_t, i : ssize_t, maybe<a>) -> e maybe<a> */ 
  if (kk_std_core_types__is_Nothing(_y_17488)) {
    kk_ssize_t i0_17301 = kk_std_core_incr_1(i, _ctx); /*ssize_t*/;
    return kk_std_core__lift17273_for_whilez(action, end, i0_17301, _ctx);
  }
  {
    kk_box_t x = _y_17488._cons.Just.value;
    kk_function_drop(action, _ctx);
    return kk_std_core_types__new_Just(x, _ctx);
  }
}
 
// lifted


// lift anonymous function
struct kk_std_core__lift17273_for_whilez_fun20726__t {
  struct kk_function_s _base;
  kk_function_t action0;
  kk_ssize_t end0;
  kk_ssize_t i0;
};
static kk_box_t kk_std_core__lift17273_for_whilez_fun20726(kk_function_t _fself, kk_box_t _b_18844, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17273_for_whilez_fun20726(kk_function_t action0, kk_ssize_t end0, kk_ssize_t i0, kk_context_t* _ctx) {
  struct kk_std_core__lift17273_for_whilez_fun20726__t* _self = kk_function_alloc_as(struct kk_std_core__lift17273_for_whilez_fun20726__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17273_for_whilez_fun20726, kk_context());
  _self->action0 = action0;
  _self->end0 = end0;
  _self->i0 = i0;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17273_for_whilez_fun20726(kk_function_t _fself, kk_box_t _b_18844, kk_context_t* _ctx) {
  struct kk_std_core__lift17273_for_whilez_fun20726__t* _self = kk_function_as(struct kk_std_core__lift17273_for_whilez_fun20726__t*, _fself);
  kk_function_t action0 = _self->action0; /* (ssize_t) -> 10565 maybe<10564> */
  kk_ssize_t end0 = _self->end0; /* ssize_t */
  kk_ssize_t i0 = _self->i0; /* ssize_t */
  kk_drop_match(_self, {kk_function_dup(action0);;;}, {}, _ctx)
  kk_std_core_types__maybe _x20727;
  kk_std_core_types__maybe _x20728 = kk_std_core_types__maybe_unbox(_b_18844, _ctx); /*maybe<10564>*/
  _x20727 = kk_std_core__mlift17723_op(action0, end0, i0, _x20728, _ctx); /*maybe<10564>*/
  return kk_std_core_types__maybe_box(_x20727, _ctx);
}

kk_std_core_types__maybe kk_std_core__lift17273_for_whilez(kk_function_t action0, kk_ssize_t end0, kk_ssize_t i0, kk_context_t* _ctx) { /* forall<a,e> (action : (ssize_t) -> e maybe<a>, end : ssize_t, i : ssize_t) -> e maybe<a> */ 
  kk__tailcall: ;
  bool _match_19727 = (i0 <= end0); /*bool*/;
  if (_match_19727) {
    kk_std_core_types__maybe x0_17944;
    kk_function_t _x20724 = kk_function_dup(action0); /*(ssize_t) -> 10565 maybe<10564>*/
    x0_17944 = kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_ssize_t, kk_context_t*), _x20724, (_x20724, i0, _ctx)); /*maybe<10564>*/
    if (kk_yielding(kk_context())) {
      kk_std_core_types__maybe_drop(x0_17944, _ctx);
      kk_box_t _x20725 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17273_for_whilez_fun20726(action0, end0, i0, _ctx), _ctx); /*3860*/
      return kk_std_core_types__maybe_unbox(_x20725, _ctx);
    }
    if (kk_std_core_types__is_Nothing(x0_17944)) {
      kk_ssize_t i0_173010 = kk_std_core_incr_1(i0, _ctx); /*ssize_t*/;
      { // tailcall
        i0 = i0_173010;
        goto kk__tailcall;
      }
    }
    {
      kk_box_t x1 = x0_17944._cons.Just.value;
      kk_function_drop(action0, _ctx);
      return kk_std_core_types__new_Just(x1, _ctx);
    }
  }
  {
    kk_function_drop(action0, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
}
 
// monadic lift

kk_std_core_types__maybe kk_std_core__mlift17724_foreach_while(kk_function_t action, kk_std_core__list xx, kk_std_core_types__maybe _y_17493, kk_context_t* _ctx) { /* forall<a,b,e> (action : (a) -> e maybe<b>, xx : list<a>, maybe<b>) -> e maybe<b> */ 
  if (kk_std_core_types__is_Nothing(_y_17493)) {
    return kk_std_core_foreach_while(xx, action, _ctx);
  }
  {
    kk_std_core__list_drop(xx, _ctx);
    kk_function_drop(action, _ctx);
    return _y_17493;
  }
}
 
// Invoke `action` for each element of a list while `action` return `Nothing`


// lift anonymous function
struct kk_std_core_foreach_while_fun20735__t {
  struct kk_function_s _base;
  kk_function_t action0;
  kk_std_core__list xx0;
};
static kk_box_t kk_std_core_foreach_while_fun20735(kk_function_t _fself, kk_box_t _b_18848, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_while_fun20735(kk_function_t action0, kk_std_core__list xx0, kk_context_t* _ctx) {
  struct kk_std_core_foreach_while_fun20735__t* _self = kk_function_alloc_as(struct kk_std_core_foreach_while_fun20735__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_while_fun20735, kk_context());
  _self->action0 = action0;
  _self->xx0 = xx0;
  return &_self->_base;
}

static kk_box_t kk_std_core_foreach_while_fun20735(kk_function_t _fself, kk_box_t _b_18848, kk_context_t* _ctx) {
  struct kk_std_core_foreach_while_fun20735__t* _self = kk_function_as(struct kk_std_core_foreach_while_fun20735__t*, _fself);
  kk_function_t action0 = _self->action0; /* (10698) -> 10700 maybe<10699> */
  kk_std_core__list xx0 = _self->xx0; /* list<10698> */
  kk_drop_match(_self, {kk_function_dup(action0);kk_std_core__list_dup(xx0);}, {}, _ctx)
  kk_std_core_types__maybe _x20736;
  kk_std_core_types__maybe _x20737 = kk_std_core_types__maybe_unbox(_b_18848, _ctx); /*maybe<10699>*/
  _x20736 = kk_std_core__mlift17724_foreach_while(action0, xx0, _x20737, _ctx); /*maybe<10699>*/
  return kk_std_core_types__maybe_box(_x20736, _ctx);
}

kk_std_core_types__maybe kk_std_core_foreach_while(kk_std_core__list xs, kk_function_t action0, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, action : (a) -> e maybe<b>) -> e maybe<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Nil(xs)) {
    kk_function_drop(action0, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
  {
    struct kk_std_core_Cons* _con20732 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20732->head;
    kk_std_core__list xx0 = _con20732->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx0);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_std_core_types__maybe x0_17947;
    kk_function_t _x20733 = kk_function_dup(action0); /*(10698) -> 10700 maybe<10699>*/
    x0_17947 = kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_box_t, kk_context_t*), _x20733, (_x20733, x, _ctx)); /*maybe<10699>*/
    if (kk_yielding(kk_context())) {
      kk_std_core_types__maybe_drop(x0_17947, _ctx);
      kk_box_t _x20734 = kk_std_core_hnd_yield_extend(kk_std_core_new_foreach_while_fun20735(action0, xx0, _ctx), _ctx); /*3860*/
      return kk_std_core_types__maybe_unbox(_x20734, _ctx);
    }
    if (kk_std_core_types__is_Nothing(x0_17947)) { // tailcall
                                                   xs = xx0;
                                                   goto kk__tailcall;
    }
    {
      kk_std_core__list_drop(xx0, _ctx);
      kk_function_drop(action0, _ctx);
      return x0_17947;
    }
  }
}
 
// monadic lift

kk_std_core_types__maybe kk_std_core__mlift17725_foreach_while_1(kk_function_t action, kk_std_core__sslice rest, kk_std_core_types__maybe _y_17497, kk_context_t* _ctx) { /* forall<a,e> (action : (c : char) -> e maybe<a>, rest : sslice, maybe<a>) -> e maybe<a> */ 
  if (kk_std_core_types__is_Nothing(_y_17497)) {
    return kk_std_core_foreach_while_1(rest, action, _ctx);
  }
  {
    kk_std_core__sslice_drop(rest, _ctx);
    kk_function_drop(action, _ctx);
    return _y_17497;
  }
}
 
// Apply a function for each character in a string slice.
// If `action` returns `Just`, the function returns immediately with that result.


// lift anonymous function
struct kk_std_core_foreach_while_fun20743__t_1 {
  struct kk_function_s _base;
  kk_function_t action0;
  kk_std_core__sslice rest0;
};
static kk_box_t kk_std_core_foreach_while_fun20743_1(kk_function_t _fself, kk_box_t _b_18855, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_while_fun20743_1(kk_function_t action0, kk_std_core__sslice rest0, kk_context_t* _ctx) {
  struct kk_std_core_foreach_while_fun20743__t_1* _self = kk_function_alloc_as(struct kk_std_core_foreach_while_fun20743__t_1, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_while_fun20743_1, kk_context());
  _self->action0 = action0;
  _self->rest0 = rest0;
  return &_self->_base;
}

static kk_box_t kk_std_core_foreach_while_fun20743_1(kk_function_t _fself, kk_box_t _b_18855, kk_context_t* _ctx) {
  struct kk_std_core_foreach_while_fun20743__t_1* _self = kk_function_as(struct kk_std_core_foreach_while_fun20743__t_1*, _fself);
  kk_function_t action0 = _self->action0; /* (c : char) -> 10785 maybe<10784> */
  kk_std_core__sslice rest0 = _self->rest0; /* sslice */
  kk_drop_match(_self, {kk_function_dup(action0);kk_std_core__sslice_dup(rest0);}, {}, _ctx)
  kk_std_core_types__maybe _x20744;
  kk_std_core_types__maybe _x20745 = kk_std_core_types__maybe_unbox(_b_18855, _ctx); /*maybe<10784>*/
  _x20744 = kk_std_core__mlift17725_foreach_while_1(action0, rest0, _x20745, _ctx); /*maybe<10784>*/
  return kk_std_core_types__maybe_box(_x20744, _ctx);
}

kk_std_core_types__maybe kk_std_core_foreach_while_1(kk_std_core__sslice slice0, kk_function_t action0, kk_context_t* _ctx) { /* forall<a,e> (slice : sslice, action : (c : char) -> e maybe<a>) -> e maybe<a> */ 
  kk__tailcall: ;
  kk_std_core_types__maybe _match_19724 = kk_std_core_next(slice0, _ctx); /*maybe<(char, sslice)>*/;
  if (kk_std_core_types__is_Nothing(_match_19724)) {
    kk_function_drop(action0, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
  {
    kk_box_t _box_x18851 = _match_19724._cons.Just.value;
    kk_std_core_types__tuple2_ _pat1 = kk_std_core_types__tuple2__unbox(_box_x18851, NULL);
    kk_box_t _box_x18852 = _pat1.fst;
    kk_box_t _box_x18853 = _pat1.snd;
    kk_char_t c = kk_char_unbox(_box_x18852, NULL);
    kk_std_core__sslice rest0 = kk_std_core__sslice_unbox(_box_x18853, NULL);
    kk_std_core__sslice_dup(rest0);
    kk_std_core_types__maybe_drop(_match_19724, _ctx);
    kk_std_core_types__maybe x_17950;
    kk_function_t _x20741 = kk_function_dup(action0); /*(c : char) -> 10785 maybe<10784>*/
    x_17950 = kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_char_t, kk_context_t*), _x20741, (_x20741, c, _ctx)); /*maybe<10784>*/
    if (kk_yielding(kk_context())) {
      kk_std_core_types__maybe_drop(x_17950, _ctx);
      kk_box_t _x20742 = kk_std_core_hnd_yield_extend(kk_std_core_new_foreach_while_fun20743_1(action0, rest0, _ctx), _ctx); /*3860*/
      return kk_std_core_types__maybe_unbox(_x20742, _ctx);
    }
    if (kk_std_core_types__is_Nothing(x_17950)) { // tailcall
                                                  slice0 = rest0;
                                                  goto kk__tailcall;
    }
    {
      kk_std_core__sslice_drop(rest0, _ctx);
      kk_function_drop(action0, _ctx);
      return x_17950;
    }
  }
}
 
// Invoke a function `f` for each element in a vector `v`.
// If `f` returns `Just`, the iteration is stopped early and the result is returned.


// lift anonymous function
struct kk_std_core_foreach_while_fun20751__t_3 {
  struct kk_function_s _base;
  kk_function_t f;
  kk_vector_t v;
};
static kk_std_core_types__maybe kk_std_core_foreach_while_fun20751_3(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_while_fun20751_3(kk_function_t f, kk_vector_t v, kk_context_t* _ctx) {
  struct kk_std_core_foreach_while_fun20751__t_3* _self = kk_function_alloc_as(struct kk_std_core_foreach_while_fun20751__t_3, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_while_fun20751_3, kk_context());
  _self->f = f;
  _self->v = v;
  return &_self->_base;
}

static kk_std_core_types__maybe kk_std_core_foreach_while_fun20751_3(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_foreach_while_fun20751__t_3* _self = kk_function_as(struct kk_std_core_foreach_while_fun20751__t_3*, _fself);
  kk_function_t f = _self->f; /* (10883) -> 10885 maybe<10884> */
  kk_vector_t v = _self->v; /* vector<10883> */
  kk_drop_match(_self, {kk_function_dup(f);kk_vector_dup(v);}, {}, _ctx)
  kk_box_t _x20752;
  kk_box_t _brw_19723 = kk_vector_at_borrow(v, i); /*223*/;
  kk_vector_drop(v, _ctx);
  _x20752 = _brw_19723; /*223*/
  return kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_box_t, kk_context_t*), f, (f, _x20752, _ctx));
}

kk_std_core_types__maybe kk_std_core_foreach_while_3(kk_vector_t v, kk_function_t f, kk_context_t* _ctx) { /* forall<a,b,e> (v : vector<a>, f : (a) -> e maybe<b>) -> e maybe<b> */ 
  kk_ssize_t start0_17953 = ((kk_ssize_t)0); /*ssize_t*/;
  kk_ssize_t end_17954;
  kk_ssize_t _x20750 = kk_vector_len_borrow(v); /*ssize_t*/
  end_17954 = kk_std_core_decr_1(_x20750, _ctx); /*ssize_t*/
  return kk_std_core__lift17273_for_whilez(kk_std_core_new_foreach_while_fun20751_3(f, v, _ctx), end_17954, start0_17953, _ctx);
}
 
// monadic lift

kk_unit_t kk_std_core__mlift17726_foreach(kk_function_t action, kk_std_core__list xx, kk_unit_t wild__, kk_context_t* _ctx) { /* forall<a,e> (action : (a) -> e (), xx : list<a>, wild_ : ()) -> e () */ 
  kk_std_core_foreach(xx, action, _ctx); return kk_Unit;
}
 
// Invoke `action` for each element of a list


// lift anonymous function
struct kk_std_core_foreach_fun20756__t {
  struct kk_function_s _base;
  kk_function_t action0;
  kk_std_core__list xx0;
};
static kk_box_t kk_std_core_foreach_fun20756(kk_function_t _fself, kk_box_t _b_18859, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_fun20756(kk_function_t action0, kk_std_core__list xx0, kk_context_t* _ctx) {
  struct kk_std_core_foreach_fun20756__t* _self = kk_function_alloc_as(struct kk_std_core_foreach_fun20756__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_fun20756, kk_context());
  _self->action0 = action0;
  _self->xx0 = xx0;
  return &_self->_base;
}

static kk_box_t kk_std_core_foreach_fun20756(kk_function_t _fself, kk_box_t _b_18859, kk_context_t* _ctx) {
  struct kk_std_core_foreach_fun20756__t* _self = kk_function_as(struct kk_std_core_foreach_fun20756__t*, _fself);
  kk_function_t action0 = _self->action0; /* (11078) -> 11079 () */
  kk_std_core__list xx0 = _self->xx0; /* list<11078> */
  kk_drop_match(_self, {kk_function_dup(action0);kk_std_core__list_dup(xx0);}, {}, _ctx)
  kk_unit_t _x20757 = kk_Unit;
  kk_unit_t _x20758 = kk_Unit;
  kk_unit_unbox(_b_18859);
  kk_std_core__mlift17726_foreach(action0, xx0, _x20758, _ctx);
  return kk_unit_box(_x20757);
}

kk_unit_t kk_std_core_foreach(kk_std_core__list xs, kk_function_t action0, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, action : (a) -> e ()) -> e () */ 
  kk__tailcall: ;
  if (kk_std_core__is_Nil(xs)) {
    kk_function_drop(action0, _ctx);
    kk_Unit; return kk_Unit;
  }
  {
    struct kk_std_core_Cons* _con20753 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20753->head;
    kk_std_core__list xx0 = _con20753->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx0);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_unit_t x0_17956 = kk_Unit;
    kk_function_t _x20754 = kk_function_dup(action0); /*(11078) -> 11079 ()*/
    kk_function_call(kk_unit_t, (kk_function_t, kk_box_t, kk_context_t*), _x20754, (_x20754, x, _ctx));
    if (kk_yielding(kk_context())) {
      kk_box_t _x20755 = kk_std_core_hnd_yield_extend(kk_std_core_new_foreach_fun20756(action0, xx0, _ctx), _ctx); /*3860*/
      kk_unit_unbox(_x20755); return kk_Unit;
    }
    { // tailcall
      xs = xx0;
      goto kk__tailcall;
    }
  }
}
 
// Apply a function for each character in a string slice.


// lift anonymous function
struct kk_std_core_foreach_fun20759__t_1 {
  struct kk_function_s _base;
  kk_function_t action;
};
static kk_std_core_types__maybe kk_std_core_foreach_fun20759_1(kk_function_t _fself, kk_char_t c, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_fun20759_1(kk_function_t action, kk_context_t* _ctx) {
  struct kk_std_core_foreach_fun20759__t_1* _self = kk_function_alloc_as(struct kk_std_core_foreach_fun20759__t_1, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_fun20759_1, kk_context());
  _self->action = action;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_foreach_fun20761__t_1 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_foreach_fun20761_1(kk_function_t _fself, kk_box_t _b_18863, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_fun20761_1(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_foreach_fun20761_1, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_foreach_fun20761_1(kk_function_t _fself, kk_box_t _b_18863, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_box_drop(_b_18863, _ctx);
  return kk_std_core_types__maybe_box(kk_std_core_types__new_Nothing(_ctx), _ctx);
}
static kk_std_core_types__maybe kk_std_core_foreach_fun20759_1(kk_function_t _fself, kk_char_t c, kk_context_t* _ctx) {
  struct kk_std_core_foreach_fun20759__t_1* _self = kk_function_as(struct kk_std_core_foreach_fun20759__t_1*, _fself);
  kk_function_t action = _self->action; /* (c : char) -> 11137 () */
  kk_drop_match(_self, {kk_function_dup(action);}, {}, _ctx)
  kk_unit_t x0_17962 = kk_Unit;
  kk_function_call(kk_unit_t, (kk_function_t, kk_char_t, kk_context_t*), action, (action, c, _ctx));
  if (kk_yielding(kk_context())) {
    kk_box_t _x20760 = kk_std_core_hnd_yield_extend(kk_std_core_new_foreach_fun20761_1(_ctx), _ctx); /*3860*/
    return kk_std_core_types__maybe_unbox(_x20760, _ctx);
  }
  {
    return kk_std_core_types__new_Nothing(_ctx);
  }
}


// lift anonymous function
struct kk_std_core_foreach_fun20763__t_1 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_foreach_fun20763_1(kk_function_t _fself, kk_box_t _b_18867, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_fun20763_1(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_foreach_fun20763_1, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_foreach_fun20763_1(kk_function_t _fself, kk_box_t _b_18867, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_box_drop(_b_18867, _ctx);
  return kk_unit_box(kk_Unit);
}

kk_unit_t kk_std_core_foreach_1(kk_std_core__sslice slice0, kk_function_t action, kk_context_t* _ctx) { /* forall<e> (slice : sslice, action : (c : char) -> e ()) -> e () */ 
  kk_std_core_types__maybe x_17959 = kk_std_core_foreach_while_1(slice0, kk_std_core_new_foreach_fun20759_1(action, _ctx), _ctx); /*maybe<_11124>*/;
  kk_std_core_types__maybe_drop(x_17959, _ctx);
  if (kk_yielding(kk_context())) {
    kk_box_t _x20762 = kk_std_core_hnd_yield_extend(kk_std_core_new_foreach_fun20763_1(_ctx), _ctx); /*3860*/
    kk_unit_unbox(_x20762); return kk_Unit;
  }
  {
    kk_Unit; return kk_Unit;
  }
}
 
// Invoke a function for each character in a string


// lift anonymous function
struct kk_std_core_foreach_fun20767__t_2 {
  struct kk_function_s _base;
  kk_function_t action;
};
static kk_std_core_types__maybe kk_std_core_foreach_fun20767_2(kk_function_t _fself, kk_char_t c, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_fun20767_2(kk_function_t action, kk_context_t* _ctx) {
  struct kk_std_core_foreach_fun20767__t_2* _self = kk_function_alloc_as(struct kk_std_core_foreach_fun20767__t_2, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_fun20767_2, kk_context());
  _self->action = action;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_foreach_fun20769__t_2 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_foreach_fun20769_2(kk_function_t _fself, kk_box_t _b_18871, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_fun20769_2(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_foreach_fun20769_2, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_foreach_fun20769_2(kk_function_t _fself, kk_box_t _b_18871, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_box_drop(_b_18871, _ctx);
  return kk_std_core_types__maybe_box(kk_std_core_types__new_Nothing(_ctx), _ctx);
}
static kk_std_core_types__maybe kk_std_core_foreach_fun20767_2(kk_function_t _fself, kk_char_t c, kk_context_t* _ctx) {
  struct kk_std_core_foreach_fun20767__t_2* _self = kk_function_as(struct kk_std_core_foreach_fun20767__t_2*, _fself);
  kk_function_t action = _self->action; /* (c : char) -> 11176 () */
  kk_drop_match(_self, {kk_function_dup(action);}, {}, _ctx)
  kk_unit_t x0_17969 = kk_Unit;
  kk_function_call(kk_unit_t, (kk_function_t, kk_char_t, kk_context_t*), action, (action, c, _ctx));
  if (kk_yielding(kk_context())) {
    kk_box_t _x20768 = kk_std_core_hnd_yield_extend(kk_std_core_new_foreach_fun20769_2(_ctx), _ctx); /*3860*/
    return kk_std_core_types__maybe_unbox(_x20768, _ctx);
  }
  {
    return kk_std_core_types__new_Nothing(_ctx);
  }
}


// lift anonymous function
struct kk_std_core_foreach_fun20771__t_2 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_foreach_fun20771_2(kk_function_t _fself, kk_box_t _b_18875, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_fun20771_2(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_foreach_fun20771_2, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_foreach_fun20771_2(kk_function_t _fself, kk_box_t _b_18875, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_box_drop(_b_18875, _ctx);
  return kk_unit_box(kk_Unit);
}

kk_unit_t kk_std_core_foreach_2(kk_string_t s, kk_function_t action, kk_context_t* _ctx) { /* forall<e> (s : string, action : (c : char) -> e ()) -> e () */ 
  kk_std_core__sslice slice0_17160;
  kk_string_t _x20764 = kk_string_dup(s); /*string*/
  kk_ssize_t _x20765 = ((kk_ssize_t)0); /*ssize_t*/
  kk_ssize_t _x20766 = kk_string_len(s,kk_context()); /*ssize_t*/
  slice0_17160 = kk_std_core__new_Sslice(_x20764, _x20765, _x20766, _ctx); /*sslice*/
  kk_std_core_types__maybe x_17966 = kk_std_core_foreach_while_1(slice0_17160, kk_std_core_new_foreach_fun20767_2(action, _ctx), _ctx); /*maybe<_11124>*/;
  kk_std_core_types__maybe_drop(x_17966, _ctx);
  if (kk_yielding(kk_context())) {
    kk_box_t _x20770 = kk_std_core_hnd_yield_extend(kk_std_core_new_foreach_fun20771_2(_ctx), _ctx); /*3860*/
    kk_unit_unbox(_x20770); return kk_Unit;
  }
  {
    kk_Unit; return kk_Unit;
  }
}
 
// Invoke a function `f` for each element in a vector `v`


// lift anonymous function
struct kk_std_core_foreach_fun20773__t_3 {
  struct kk_function_s _base;
  kk_function_t f;
  kk_vector_t v;
};
static kk_unit_t kk_std_core_foreach_fun20773_3(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_fun20773_3(kk_function_t f, kk_vector_t v, kk_context_t* _ctx) {
  struct kk_std_core_foreach_fun20773__t_3* _self = kk_function_alloc_as(struct kk_std_core_foreach_fun20773__t_3, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_fun20773_3, kk_context());
  _self->f = f;
  _self->v = v;
  return &_self->_base;
}

static kk_unit_t kk_std_core_foreach_fun20773_3(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_foreach_fun20773__t_3* _self = kk_function_as(struct kk_std_core_foreach_fun20773__t_3*, _fself);
  kk_function_t f = _self->f; /* (11191) -> 11192 () */
  kk_vector_t v = _self->v; /* vector<11191> */
  kk_drop_match(_self, {kk_function_dup(f);kk_vector_dup(v);}, {}, _ctx)
  kk_box_t x_18165;
  kk_box_t _brw_19717 = kk_vector_at_borrow(v, i); /*223*/;
  kk_vector_drop(v, _ctx);
  x_18165 = _brw_19717; /*11191*/
  return kk_function_call(kk_unit_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, x_18165, _ctx));
}

kk_unit_t kk_std_core_foreach_3(kk_vector_t v, kk_function_t f, kk_context_t* _ctx) { /* forall<a,e> (v : vector<a>, f : (a) -> e ()) -> e () */ 
  kk_ssize_t start0_17975 = ((kk_ssize_t)0); /*ssize_t*/;
  kk_ssize_t end_17976;
  kk_ssize_t _x20772 = kk_vector_len_borrow(v); /*ssize_t*/
  end_17976 = kk_std_core_decr_1(_x20772, _ctx); /*ssize_t*/
  kk_std_core__lift17269_forz(kk_std_core_new_foreach_fun20773_3(f, v, _ctx), end_17976, start0_17975, _ctx); return kk_Unit;
}
 
// O(n). Return the number of characters in a string.

kk_integer_t kk_std_core_count_1(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> int */ 
  return kk_string_count_int(s,kk_context());
}
 
// O(n). Return the number of characters in a string slice

kk_integer_t kk_std_core_count_2(kk_std_core__sslice slice0, kk_context_t* _ctx) { /* (slice : sslice) -> int */ 
  return kk_slice_count(slice0,kk_context());
}
 
// Count the number of times a predicate is true for each character in a string


// lift anonymous function
struct kk_std_core_count_fun20778__t_3 {
  struct kk_function_s _base;
  kk_ref_t loc;
  kk_function_t pred;
};
static kk_std_core_types__maybe kk_std_core_count_fun20778_3(kk_function_t _fself, kk_char_t c, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_count_fun20778_3(kk_ref_t loc, kk_function_t pred, kk_context_t* _ctx) {
  struct kk_std_core_count_fun20778__t_3* _self = kk_function_alloc_as(struct kk_std_core_count_fun20778__t_3, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_count_fun20778_3, kk_context());
  _self->loc = loc;
  _self->pred = pred;
  return &_self->_base;
}

static kk_std_core_types__maybe kk_std_core_count_fun20778_3(kk_function_t _fself, kk_char_t c, kk_context_t* _ctx) {
  struct kk_std_core_count_fun20778__t_3* _self = kk_function_as(struct kk_std_core_count_fun20778__t_3*, _fself);
  kk_ref_t loc = _self->loc; /* local-var<11385,int> */
  kk_function_t pred = _self->pred; /* (char) -> bool */
  kk_drop_match(_self, {kk_ref_dup(loc);kk_function_dup(pred);}, {}, _ctx)
  kk_unit_t __ = kk_Unit;
  bool _match_19715 = kk_function_call(bool, (kk_function_t, kk_char_t, kk_context_t*), pred, (pred, c, _ctx)); /*bool*/;
  if (_match_19715) {
    kk_integer_t _b_18884_18882;
    kk_integer_t _x20779;
    kk_box_t _x20780;
    kk_ref_t _x20781 = kk_ref_dup(loc); /*local-var<11385,int>*/
    _x20780 = (kk_ref_get(_x20781,kk_context())); /*258*/
    _x20779 = kk_integer_unbox(_x20780); /*int*/
    _b_18884_18882 = kk_integer_add(_x20779,(kk_integer_from_small(1)),kk_context()); /*int*/
    kk_unit_t _brw_19716 = kk_Unit;
    (kk_ref_set_borrow(loc,(kk_integer_box(_b_18884_18882)),kk_context()));
    kk_ref_drop(loc, _ctx);
    _brw_19716;
  }
  else {
    kk_ref_drop(loc, _ctx);
  }
  return kk_std_core_types__new_Nothing(_ctx);
}

kk_integer_t kk_std_core_count_3(kk_string_t s, kk_function_t pred, kk_context_t* _ctx) { /* (s : string, pred : (char) -> bool) -> int */ 
  kk_ref_t loc = kk_ref_alloc((kk_integer_box(kk_integer_from_small(0))),kk_context()); /*local-var<11385,int>*/;
  kk_std_core__sslice slice0_17165;
  kk_string_t _x20774 = kk_string_dup(s); /*string*/
  kk_ssize_t _x20775 = ((kk_ssize_t)0); /*ssize_t*/
  kk_ssize_t _x20776 = kk_string_len(s,kk_context()); /*ssize_t*/
  slice0_17165 = kk_std_core__new_Sslice(_x20774, _x20775, _x20776, _ctx); /*sslice*/
  kk_std_core_types__maybe __0;
  kk_function_t _x20777;
  kk_ref_dup(loc);
  _x20777 = kk_std_core_new_count_fun20778_3(loc, pred, _ctx); /*(c : char) -> (local<11385>) (forall<a> maybe<a>)*/
  __0 = kk_std_core_foreach_while_1(slice0_17165, _x20777, _ctx); /*maybe<_11124>*/
  kk_std_core_types__maybe_drop(__0, _ctx);
  kk_integer_t res;
  kk_box_t _x20782;
  kk_ref_t _x20783 = kk_ref_dup(loc); /*local-var<11385,int>*/
  _x20782 = (kk_ref_get(_x20783,kk_context())); /*258*/
  res = kk_integer_unbox(_x20782); /*int*/
  kk_box_t _x20784 = kk_std_core_hnd_prompt_local_var(loc, kk_integer_box(res), _ctx); /*9897*/
  return kk_integer_unbox(_x20784);
}
 
// Is the integer negative (stricly smaller than zero)

bool kk_std_core_is_neg_2(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> bool */ 
  kk_std_core_types__order x_17253;
  kk_std_core_types__order _brw_19714 = kk_int_as_order(kk_integer_signum_borrow(i),kk_context()); /*order*/;
  kk_integer_drop(i, _ctx);
  x_17253 = _brw_19714; /*order*/
  kk_integer_t _brw_19712;
  if (kk_std_core_types__is_Lt(x_17253)) {
    _brw_19712 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
    goto _match20788;
  }
  if (kk_std_core_types__is_Eq(x_17253)) {
    _brw_19712 = kk_integer_from_small(0); /*int*/
    goto _match20788;
  }
  {
    _brw_19712 = kk_integer_from_small(1); /*int*/
  }
  _match20788: ;
  kk_integer_t _brw_19711 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/;
  bool _brw_19713 = kk_integer_eq_borrow(_brw_19712,_brw_19711,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19712, _ctx);
  kk_integer_drop(_brw_19711, _ctx);
  return _brw_19713;
}

kk_std_core_types__tuple2_ kk_std_core_divmod_exp10(kk_integer_t i, kk_integer_t n, kk_context_t* _ctx) { /* (i : int, n : int) -> (int, int) */ 
  kk_std_core_types__tuple2_ _match_19707;
  kk_integer_t _x20789 = kk_integer_dup(n); /*int*/
  _match_19707 = kk_std_core_cdivmod_exp10(i, _x20789, _ctx); /*(int, int)*/
  {
    kk_box_t _box_x18897 = _match_19707.fst;
    kk_box_t _box_x18898 = _match_19707.snd;
    kk_integer_t cq = kk_integer_unbox(_box_x18897);
    kk_integer_t cr = kk_integer_unbox(_box_x18898);
    kk_integer_dup(cq);
    kk_integer_dup(cr);
    kk_std_core_types__tuple2__drop(_match_19707, _ctx);
    kk_std_core_types__order x_17255 = kk_int_as_order(kk_integer_signum_borrow(cr),kk_context()); /*order*/;
    bool b_17168;
    kk_integer_t _brw_19709;
    if (kk_std_core_types__is_Lt(x_17255)) {
      _brw_19709 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/
      goto _match20792;
    }
    if (kk_std_core_types__is_Eq(x_17255)) {
      _brw_19709 = kk_integer_from_small(0); /*int*/
      goto _match20792;
    }
    {
      _brw_19709 = kk_integer_from_small(1); /*int*/
    }
    _match20792: ;
    kk_integer_t _brw_19708 = kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context()); /*int*/;
    bool _brw_19710 = kk_integer_eq_borrow(_brw_19709,_brw_19708,kk_context()); /*bool*/;
    kk_integer_drop(_brw_19709, _ctx);
    kk_integer_drop(_brw_19708, _ctx);
    b_17168 = _brw_19710; /*bool*/
    if (b_17168) {
      kk_integer_t _b_18903_18899 = kk_integer_sub(cq,(kk_integer_from_small(1)),kk_context()); /*int*/;
      kk_integer_t _b_18904_18900;
      kk_integer_t _x20793 = kk_std_core_mul_exp10(kk_integer_from_small(1), n, _ctx); /*int*/
      _b_18904_18900 = kk_integer_add(cr,_x20793,kk_context()); /*int*/
      return kk_std_core_types__new_dash__lp__comma__rp_(kk_integer_box(_b_18903_18899), kk_integer_box(_b_18904_18900), _ctx);
    }
    {
      kk_integer_drop(n, _ctx);
      return kk_std_core_types__new_dash__lp__comma__rp_(kk_integer_box(cq), kk_integer_box(cr), _ctx);
    }
  }
}
 
// Drop the first `n` elements of a list (or fewer if the list is shorter than `n`)

kk_std_core__list kk_std_core_drop(kk_std_core__list xs, kk_integer_t n, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, n : int) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20794 = kk_std_core__as_Cons(xs);
    kk_box_t _pat0 = _con20794->head;
    kk_std_core__list xx = _con20794->tail;
    kk_integer_t _brw_19705 = kk_integer_from_small(0); /*int*/;
    bool _brw_19706 = kk_integer_gt_borrow(n,_brw_19705,kk_context()); /*bool*/;
    kk_integer_drop(_brw_19705, _ctx);
    if (_brw_19706) {
      if (kk_likely(kk_std_core__list_is_unique(xs))) {
        kk_box_drop(_pat0, _ctx);
        kk_std_core__list_free(xs);
      }
      else {
        kk_std_core__list_dup(xx);
        kk_std_core__list_decref(xs, _ctx);
      }
      { // tailcall
        kk_integer_t _x20795 = kk_integer_sub(n,(kk_integer_from_small(1)),kk_context()); /*int*/
        xs = xx;
        n = _x20795;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_integer_drop(n, _ctx);
    return xs;
  }
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17731_drop_while(kk_function_t predicate, kk_std_core__list xs, kk_std_core__list xx, bool _y_17513, kk_context_t* _ctx) { /* forall<a,e> (predicate : (a) -> e bool, xs : list<a>, xx : list<a>, bool) -> e list<a> */ 
  if (_y_17513) {
    kk_std_core__list_drop(xs, _ctx);
    return kk_std_core_drop_while(xx, predicate, _ctx);
  }
  {
    kk_std_core__list_drop(xx, _ctx);
    kk_function_drop(predicate, _ctx);
    return xs;
  }
}
 
// Drop all initial elements that satisfy `predicate`


// lift anonymous function
struct kk_std_core_drop_while_fun20799__t {
  struct kk_function_s _base;
  kk_function_t predicate0;
  kk_std_core__list xs0;
  kk_std_core__list xx0;
};
static kk_box_t kk_std_core_drop_while_fun20799(kk_function_t _fself, kk_box_t _b_18908, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_drop_while_fun20799(kk_function_t predicate0, kk_std_core__list xs0, kk_std_core__list xx0, kk_context_t* _ctx) {
  struct kk_std_core_drop_while_fun20799__t* _self = kk_function_alloc_as(struct kk_std_core_drop_while_fun20799__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_drop_while_fun20799, kk_context());
  _self->predicate0 = predicate0;
  _self->xs0 = xs0;
  _self->xx0 = xx0;
  return &_self->_base;
}

static kk_box_t kk_std_core_drop_while_fun20799(kk_function_t _fself, kk_box_t _b_18908, kk_context_t* _ctx) {
  struct kk_std_core_drop_while_fun20799__t* _self = kk_function_as(struct kk_std_core_drop_while_fun20799__t*, _fself);
  kk_function_t predicate0 = _self->predicate0; /* (11653) -> 11654 bool */
  kk_std_core__list xs0 = _self->xs0; /* list<11653> */
  kk_std_core__list xx0 = _self->xx0; /* list<11653> */
  kk_drop_match(_self, {kk_function_dup(predicate0);kk_std_core__list_dup(xs0);kk_std_core__list_dup(xx0);}, {}, _ctx)
  kk_std_core__list _x20800;
  bool _x20801 = kk_bool_unbox(_b_18908); /*bool*/
  _x20800 = kk_std_core__mlift17731_drop_while(predicate0, xs0, xx0, _x20801, _ctx); /*list<11653>*/
  return kk_std_core__list_box(_x20800, _ctx);
}

kk_std_core__list kk_std_core_drop_while(kk_std_core__list xs0, kk_function_t predicate0, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, predicate : (a) -> e bool) -> e list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs0)) {
    struct kk_std_core_Cons* _con20796 = kk_std_core__as_Cons(xs0);
    kk_box_t x = _con20796->head;
    kk_std_core__list xx0 = _con20796->tail;
    kk_box_dup(x);
    kk_std_core__list_dup(xx0);
    bool x0_17980;
    kk_function_t _x20797 = kk_function_dup(predicate0); /*(11653) -> 11654 bool*/
    x0_17980 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), _x20797, (_x20797, x, _ctx)); /*bool*/
    if (kk_yielding(kk_context())) {
      kk_box_t _x20798 = kk_std_core_hnd_yield_extend(kk_std_core_new_drop_while_fun20799(predicate0, xs0, xx0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20798, _ctx);
    }
    if (x0_17980) {
      if (kk_likely(kk_std_core__list_is_unique(xs0))) {
        kk_std_core__list_drop(xx0, _ctx);
        kk_box_drop(x, _ctx);
        kk_std_core__list_free(xs0);
      }
      else {
        kk_std_core__list_decref(xs0, _ctx);
      }
      { // tailcall
        xs0 = xx0;
        goto kk__tailcall;
      }
    }
    {
      kk_std_core__list_drop(xx0, _ctx);
      kk_function_drop(predicate0, _ctx);
      return xs0;
    }
  }
  {
    kk_function_drop(predicate0, _ctx);
    return kk_std_core__new_Nil(_ctx);
  }
}
 
// An empty slice

kk_std_core__sslice kk_std_core_empty;
 
// Does string `s`  end with `post`?
// If so, returns a slice of `s` from the start up to the `post` string at the end.

kk_std_core_types__maybe kk_std_core_ends_with(kk_string_t s, kk_string_t post, kk_context_t* _ctx) { /* (s : string, post : string) -> maybe<sslice> */ 
  bool _match_19703;
  kk_string_t _x20806 = kk_string_dup(s); /*string*/
  kk_string_t _x20807 = kk_string_dup(post); /*string*/
  _match_19703 = kk_std_core_xends_with(_x20806, _x20807, _ctx); /*bool*/
  if (_match_19703) {
    kk_std_core__sslice _b_18912_18911;
    kk_string_t _x20808 = kk_string_dup(s); /*string*/
    kk_ssize_t _x20809 = ((kk_ssize_t)0); /*ssize_t*/
    kk_ssize_t _x20810;
    kk_ssize_t _x20811 = kk_string_len(s,kk_context()); /*ssize_t*/
    kk_ssize_t _x20812 = kk_string_len(post,kk_context()); /*ssize_t*/
    _x20810 = (_x20811 - _x20812); /*ssize_t*/
    _b_18912_18911 = kk_std_core__new_Sslice(_x20808, _x20809, _x20810, _ctx); /*sslice*/
    return kk_std_core_types__new_Just(kk_std_core__sslice_box(_b_18912_18911, _ctx), _ctx);
  }
  {
    kk_string_drop(s, _ctx);
    kk_string_drop(post, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
}
 
// Throw an exception with a specified message.

kk_box_t kk_std_core_throw(kk_string_t message0, kk_std_core_types__optional info0, kk_context_t* _ctx) { /* forall<a> (message : string, info : optional<exception-info>) -> exn a */ 
  kk_std_core_hnd__ev ev_17983;
  kk_ssize_t _x20813 = ((kk_ssize_t)0); /*ssize_t*/
  ev_17983 = kk_evv_at(_x20813,kk_context()); /*std/core/hnd/ev<.hnd-exn>*/
  {
    struct kk_std_core_hnd_Ev* _con20814 = kk_std_core_hnd__as_Ev(ev_17983);
    kk_std_core_hnd__marker m0 = _con20814->marker;
    kk_box_t _box_x18913 = _con20814->hnd;
    kk_std_core__hnd_exn h = kk_std_core__hnd_exn_unbox(_box_x18913, NULL);
    kk_std_core__hnd_exn_dup(h);
    kk_std_core_hnd__clause1 _match_19701;
    kk_std_core_hnd__clause1 _brw_19702 = kk_std_core__select_throw_exn(h, _ctx); /*std/core/hnd/clause1<exception,1940,.hnd-exn,1941,1942>*/;
    kk_std_core__hnd_exn_drop(h, _ctx);
    _match_19701 = _brw_19702; /*std/core/hnd/clause1<exception,1940,.hnd-exn,1941,1942>*/
    {
      kk_function_t _fun_unbox_x18917 = _match_19701.clause;
      kk_box_t _x20816;
      kk_std_core__exception _x20817;
      kk_std_core__exception_info _x20818;
      if (kk_std_core_types__is_Optional(info0)) {
        kk_box_t _box_x18921 = info0._cons.Optional.value;
        kk_std_core__exception_info _info_11739 = kk_std_core__exception_info_unbox(_box_x18921, NULL);
        kk_std_core__exception_info_dup(_info_11739);
        kk_std_core_types__optional_drop(info0, _ctx);
        _x20818 = _info_11739; /*exception-info*/
        goto _match20819;
      }
      {
        _x20818 = kk_std_core__new_ExnError(_ctx); /*exception-info*/
      }
      _match20819: ;
      _x20817 = kk_std_core__new_Exception(message0, _x20818, _ctx); /*exception*/
      _x20816 = kk_std_core__exception_box(_x20817, _ctx); /*51*/
      return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__marker, kk_std_core_hnd__ev, kk_box_t, kk_context_t*), _fun_unbox_x18917, (_fun_unbox_x18917, m0, ev_17983, _x20816, _ctx));
    }
  }
}
 
// Transform an `:error` type back to an `exn` effect.

kk_box_t kk_std_core_throw_1(kk_std_core__error err, kk_context_t* _ctx) { /* forall<a> (err : error<a>) -> exn a */ 
  if (kk_std_core__is_Error(err)) {
    kk_std_core__exception exn0 = err._cons.Error.exception;
    kk_std_core__exception_dup(exn0);
    kk_std_core__error_drop(err, _ctx);
    kk_std_core_hnd__ev ev_17986;
    kk_ssize_t _x20821 = ((kk_ssize_t)0); /*ssize_t*/
    ev_17986 = kk_evv_at(_x20821,kk_context()); /*std/core/hnd/ev<.hnd-exn>*/
    {
      struct kk_std_core_hnd_Ev* _con20822 = kk_std_core_hnd__as_Ev(ev_17986);
      kk_std_core_hnd__marker m0 = _con20822->marker;
      kk_box_t _box_x18922 = _con20822->hnd;
      kk_std_core__hnd_exn h = kk_std_core__hnd_exn_unbox(_box_x18922, NULL);
      kk_std_core__hnd_exn_dup(h);
      kk_std_core_hnd__clause1 _match_19699;
      kk_std_core_hnd__clause1 _brw_19700 = kk_std_core__select_throw_exn(h, _ctx); /*std/core/hnd/clause1<exception,1940,.hnd-exn,1941,1942>*/;
      kk_std_core__hnd_exn_drop(h, _ctx);
      _match_19699 = _brw_19700; /*std/core/hnd/clause1<exception,1940,.hnd-exn,1941,1942>*/
      {
        kk_function_t _fun_unbox_x18926 = _match_19699.clause;
        return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__marker, kk_std_core_hnd__ev, kk_box_t, kk_context_t*), _fun_unbox_x18926, (_fun_unbox_x18926, m0, ev_17986, kk_std_core__exception_box(exn0, _ctx), _ctx));
      }
    }
  }
  {
    kk_box_t x0 = err._cons.Ok.result;
    return x0;
  }
}
 
// monadic lift

kk_box_t kk_std_core__mlift17732_error_pattern(kk_string_t definition, kk_string_t location, kk_string_t _c_17520, kk_context_t* _ctx) { /* forall<a> (definition : string, location : string, string) -> a */ 
  kk_string_t message0_17174;
  kk_string_t _x20824 = kk_string_dup(location); /*string*/
  kk_string_t _x20825;
  kk_string_t _x20826;
  kk_define_string_literal(, _s20827, 23, ": pattern match failure")
  _x20826 = kk_string_dup(_s20827); /*string*/
  _x20825 = kk_std_core__lp__plus__plus__1_rp_(_c_17520, _x20826, _ctx); /*string*/
  message0_17174 = kk_std_core__lp__plus__plus__1_rp_(_x20824, _x20825, _ctx); /*string*/
  kk_std_core_hnd__ev ev_17989;
  kk_ssize_t _x20828 = ((kk_ssize_t)0); /*ssize_t*/
  ev_17989 = kk_evv_at(_x20828,kk_context()); /*std/core/hnd/ev<.hnd-exn>*/
  {
    struct kk_std_core_hnd_Ev* _con20829 = kk_std_core_hnd__as_Ev(ev_17989);
    kk_std_core_hnd__marker m0 = _con20829->marker;
    kk_box_t _box_x18930 = _con20829->hnd;
    kk_std_core__hnd_exn h = kk_std_core__hnd_exn_unbox(_box_x18930, NULL);
    kk_std_core__hnd_exn_dup(h);
    kk_std_core_hnd__clause1 _match_19697;
    kk_std_core_hnd__clause1 _brw_19698 = kk_std_core__select_throw_exn(h, _ctx); /*std/core/hnd/clause1<exception,1940,.hnd-exn,1941,1942>*/;
    kk_std_core__hnd_exn_drop(h, _ctx);
    _match_19697 = _brw_19698; /*std/core/hnd/clause1<exception,1940,.hnd-exn,1941,1942>*/
    {
      kk_function_t _fun_unbox_x18934 = _match_19697.clause;
      kk_box_t _x20831;
      kk_std_core__exception _x20832;
      kk_std_core__exception_info _x20833 = kk_std_core__new_ExnPattern(kk_reuse_null, location, definition, _ctx); /*exception-info*/
      _x20832 = kk_std_core__new_Exception(message0_17174, _x20833, _ctx); /*exception*/
      _x20831 = kk_std_core__exception_box(_x20832, _ctx); /*51*/
      return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__marker, kk_std_core_hnd__ev, kk_box_t, kk_context_t*), _fun_unbox_x18934, (_fun_unbox_x18934, m0, ev_17989, _x20831, _ctx));
    }
  }
}
 
// Raise a pattern match exception. This is function is used internally by the
// compiler to generate error messages on pattern match failures.


// lift anonymous function
struct kk_std_core_error_pattern_fun20841__t {
  struct kk_function_s _base;
  kk_string_t definition;
  kk_string_t location;
};
static kk_box_t kk_std_core_error_pattern_fun20841(kk_function_t _fself, kk_box_t _b_18939, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_error_pattern_fun20841(kk_string_t definition, kk_string_t location, kk_context_t* _ctx) {
  struct kk_std_core_error_pattern_fun20841__t* _self = kk_function_alloc_as(struct kk_std_core_error_pattern_fun20841__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_error_pattern_fun20841, kk_context());
  _self->definition = definition;
  _self->location = location;
  return &_self->_base;
}

static kk_box_t kk_std_core_error_pattern_fun20841(kk_function_t _fself, kk_box_t _b_18939, kk_context_t* _ctx) {
  struct kk_std_core_error_pattern_fun20841__t* _self = kk_function_as(struct kk_std_core_error_pattern_fun20841__t*, _fself);
  kk_string_t definition = _self->definition; /* string */
  kk_string_t location = _self->location; /* string */
  kk_drop_match(_self, {kk_string_dup(definition);kk_string_dup(location);}, {}, _ctx)
  kk_string_t _x20842 = kk_string_unbox(_b_18939); /*string*/
  return kk_std_core__mlift17732_error_pattern(definition, location, _x20842, _ctx);
}

kk_box_t kk_std_core_error_pattern(kk_string_t location, kk_string_t definition, kk_context_t* _ctx) { /* forall<a> (location : string, definition : string) -> exn a */ 
  kk_string_t x_17992;
  bool _match_19696;
  kk_string_t _x20834 = kk_string_dup(definition); /*string*/
  kk_string_t _x20835 = kk_string_empty(); /*string*/
  _match_19696 = kk_string_is_eq(_x20834,_x20835,kk_context()); /*bool*/
  if (_match_19696) {
    x_17992 = kk_string_empty(); /*string*/
  }
  else {
    kk_string_t _x20838;
    kk_define_string_literal(, _s20839, 2, ": ")
    _x20838 = kk_string_dup(_s20839); /*string*/
    kk_string_t _x20840 = kk_string_dup(definition); /*string*/
    x_17992 = kk_std_core__lp__plus__plus__1_rp_(_x20838, _x20840, _ctx); /*string*/
  }
  if (kk_yielding(kk_context())) {
    kk_string_drop(x_17992, _ctx);
    return kk_std_core_hnd_yield_extend(kk_std_core_new_error_pattern_fun20841(definition, location, _ctx), _ctx);
  }
  {
    kk_string_t message0_17174;
    kk_string_t _x20843 = kk_string_dup(location); /*string*/
    kk_string_t _x20844;
    kk_string_t _x20845;
    kk_define_string_literal(, _s20846, 23, ": pattern match failure")
    _x20845 = kk_string_dup(_s20846); /*string*/
    _x20844 = kk_std_core__lp__plus__plus__1_rp_(x_17992, _x20845, _ctx); /*string*/
    message0_17174 = kk_std_core__lp__plus__plus__1_rp_(_x20843, _x20844, _ctx); /*string*/
    kk_std_core_hnd__ev ev_17995;
    kk_ssize_t _x20847 = ((kk_ssize_t)0); /*ssize_t*/
    ev_17995 = kk_evv_at(_x20847,kk_context()); /*std/core/hnd/ev<.hnd-exn>*/
    {
      struct kk_std_core_hnd_Ev* _con20848 = kk_std_core_hnd__as_Ev(ev_17995);
      kk_std_core_hnd__marker m0 = _con20848->marker;
      kk_box_t _box_x18940 = _con20848->hnd;
      kk_std_core__hnd_exn h = kk_std_core__hnd_exn_unbox(_box_x18940, NULL);
      kk_std_core__hnd_exn_dup(h);
      kk_std_core_hnd__clause1 _match_19694;
      kk_std_core_hnd__clause1 _brw_19695 = kk_std_core__select_throw_exn(h, _ctx); /*std/core/hnd/clause1<exception,1940,.hnd-exn,1941,1942>*/;
      kk_std_core__hnd_exn_drop(h, _ctx);
      _match_19694 = _brw_19695; /*std/core/hnd/clause1<exception,1940,.hnd-exn,1941,1942>*/
      {
        kk_function_t _fun_unbox_x18944 = _match_19694.clause;
        kk_box_t _x20850;
        kk_std_core__exception _x20851;
        kk_std_core__exception_info _x20852 = kk_std_core__new_ExnPattern(kk_reuse_null, location, definition, _ctx); /*exception-info*/
        _x20851 = kk_std_core__new_Exception(message0_17174, _x20852, _ctx); /*exception*/
        _x20850 = kk_std_core__exception_box(_x20851, _ctx); /*51*/
        return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__marker, kk_std_core_hnd__ev, kk_box_t, kk_context_t*), _fun_unbox_x18944, (_fun_unbox_x18944, m0, ev_17995, _x20850, _ctx));
      }
    }
  }
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17733_op(kk_std_core_types__ctail _acc, kk_function_t pred, kk_box_t x, kk_std_core__list xx, bool _y_17523, kk_context_t* _ctx) { /* forall<a,e> (ctail<list<a>>, pred : (a) -> e bool, x : a, xx : list<a>, bool) -> e list<a> */ 
  if (_y_17523) {
    kk_std_core__list _ctail_17353 = kk_std_core__list_hole(); /*list<11950>*/;
    kk_std_core__list _ctail_17354 = kk_std_core__new_Cons(kk_reuse_null, x, _ctail_17353, _ctx); /*list<11950>*/;
    kk_std_core_types__ctail _x20853;
    kk_box_t* _b_18958_18955 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17354)->tail)); /*cfield<list<11950>>*/;
    _x20853 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17354, _ctx)),_b_18958_18955); /*ctail<0>*/
    return kk_std_core__ctail_filter(xx, pred, _x20853, _ctx);
  }
  {
    kk_box_drop(x, _ctx);
    return kk_std_core__ctail_filter(xx, pred, _acc, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17734_op_fun20854__t {
  struct kk_function_s _base;
  kk_function_t _accm;
  kk_box_t x0;
};
static kk_std_core__list kk_std_core__mlift17734_op_fun20854(kk_function_t _fself, kk_std_core__list _ctail_17356, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17734_op_fun20854(kk_function_t _accm, kk_box_t x0, kk_context_t* _ctx) {
  struct kk_std_core__mlift17734_op_fun20854__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17734_op_fun20854__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17734_op_fun20854, kk_context());
  _self->_accm = _accm;
  _self->x0 = x0;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__mlift17734_op_fun20854(kk_function_t _fself, kk_std_core__list _ctail_17356, kk_context_t* _ctx) {
  struct kk_std_core__mlift17734_op_fun20854__t* _self = kk_function_as(struct kk_std_core__mlift17734_op_fun20854__t*, _fself);
  kk_function_t _accm = _self->_accm; /* (list<11950>) -> list<11950> */
  kk_box_t x0 = _self->x0; /* 11950 */
  kk_drop_match(_self, {kk_function_dup(_accm);kk_box_dup(x0);}, {}, _ctx)
  kk_std_core__list _x20855 = kk_std_core__new_Cons(kk_reuse_null, x0, _ctail_17356, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, _x20855, _ctx));
}

kk_std_core__list kk_std_core__mlift17734_op(kk_function_t _accm, kk_function_t pred0, kk_box_t x0, kk_std_core__list xx0, bool _y_17528, kk_context_t* _ctx) { /* forall<a,e> ((list<a>) -> list<a>, pred : (a) -> e bool, x : a, xx : list<a>, bool) -> e list<a> */ 
  if (_y_17528) {
    return kk_std_core__ctailm_filter(xx0, pred0, kk_std_core__new_mlift17734_op_fun20854(_accm, x0, _ctx), _ctx);
  }
  {
    kk_box_drop(x0, _ctx);
    return kk_std_core__ctailm_filter(xx0, pred0, _accm, _ctx);
  }
}
 
// Retain only those elements of a list that satisfy the given predicate `pred`.
// For example: `filter([1,2,3],odd?) == [1,3]`


// lift anonymous function
struct kk_std_core__ctail_filter_fun20860__t {
  struct kk_function_s _base;
  kk_function_t pred1;
  kk_box_t x1;
  kk_std_core__list xx1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_filter_fun20860(kk_function_t _fself, kk_box_t _b_18963, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_filter_fun20860(kk_function_t pred1, kk_box_t x1, kk_std_core__list xx1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_filter_fun20860__t* _self = kk_function_alloc_as(struct kk_std_core__ctail_filter_fun20860__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_filter_fun20860, kk_context());
  _self->pred1 = pred1;
  _self->x1 = x1;
  _self->xx1 = xx1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_filter_fun20860(kk_function_t _fself, kk_box_t _b_18963, kk_context_t* _ctx) {
  struct kk_std_core__ctail_filter_fun20860__t* _self = kk_function_as(struct kk_std_core__ctail_filter_fun20860__t*, _fself);
  kk_function_t pred1 = _self->pred1; /* (11950) -> 11951 bool */
  kk_box_t x1 = _self->x1; /* 11950 */
  kk_std_core__list xx1 = _self->xx1; /* list<11950> */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<11950>> */
  kk_drop_match(_self, {kk_function_dup(pred1);kk_box_dup(x1);kk_std_core__list_dup(xx1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x20861;
  bool _x20862 = kk_bool_unbox(_b_18963); /*bool*/
  _x20861 = kk_std_core__mlift17733_op(_acc0, pred1, x1, xx1, _x20862, _ctx); /*list<11950>*/
  return kk_std_core__list_box(_x20861, _ctx);
}

kk_std_core__list kk_std_core__ctail_filter(kk_std_core__list xs, kk_function_t pred1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, pred : (a) -> e bool, ctail<list<a>>) -> e list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20856 = kk_std_core__as_Cons(xs);
    kk_box_t x1 = _con20856->head;
    kk_std_core__list xx1 = _con20856->tail;
    kk_reuse_t _ru_19981 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      _ru_19981 = (kk_std_core__list_reuse(xs));
    }
    else {
      kk_box_dup(x1);
      kk_std_core__list_dup(xx1);
      kk_std_core__list_decref(xs, _ctx);
    }
    bool x2_17998;
    kk_function_t _x20858 = kk_function_dup(pred1); /*(11950) -> 11951 bool*/
    kk_box_t _x20857 = kk_box_dup(x1); /*11950*/
    x2_17998 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), _x20858, (_x20858, _x20857, _ctx)); /*bool*/
    if (kk_yielding(kk_context())) {
      kk_reuse_drop(_ru_19981);
      kk_box_t _x20859 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_filter_fun20860(pred1, x1, xx1, _acc0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20859, _ctx);
    }
    if (x2_17998) {
      kk_std_core__list _ctail_173530 = kk_std_core__list_hole(); /*list<11950>*/;
      kk_std_core__list _ctail_173540 = kk_std_core__new_Cons(_ru_19981, x1, _ctail_173530, _ctx); /*list<11950>*/;
      { // tailcall
        kk_std_core_types__ctail _x20863;
        kk_box_t* _b_18975_18969 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_173540)->tail)); /*cfield<list<11950>>*/;
        _x20863 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_173540, _ctx)),_b_18975_18969); /*ctail<0>*/
        xs = xx1;
        _acc0 = _x20863;
        goto kk__tailcall;
      }
    }
    {
      kk_reuse_drop(_ru_19981);
      kk_box_drop(x1, _ctx);
      { // tailcall
        xs = xx1;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_function_drop(pred1, _ctx);
    kk_box_t _x20864 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20864, _ctx);
  }
}
 
// Retain only those elements of a list that satisfy the given predicate `pred`.
// For example: `filter([1,2,3],odd?) == [1,3]`


// lift anonymous function
struct kk_std_core__ctailm_filter_fun20869__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t pred2;
  kk_box_t x3;
  kk_std_core__list xx2;
};
static kk_box_t kk_std_core__ctailm_filter_fun20869(kk_function_t _fself, kk_box_t _b_18983, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_filter_fun20869(kk_function_t _accm0, kk_function_t pred2, kk_box_t x3, kk_std_core__list xx2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_filter_fun20869__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_filter_fun20869__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_filter_fun20869, kk_context());
  _self->_accm0 = _accm0;
  _self->pred2 = pred2;
  _self->x3 = x3;
  _self->xx2 = xx2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_filter_fun20869(kk_function_t _fself, kk_box_t _b_18983, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_filter_fun20869__t* _self = kk_function_as(struct kk_std_core__ctailm_filter_fun20869__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<11950>) -> list<11950> */
  kk_function_t pred2 = _self->pred2; /* (11950) -> 11951 bool */
  kk_box_t x3 = _self->x3; /* 11950 */
  kk_std_core__list xx2 = _self->xx2; /* list<11950> */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(pred2);kk_box_dup(x3);kk_std_core__list_dup(xx2);}, {}, _ctx)
  kk_std_core__list _x20870;
  bool _x20871 = kk_bool_unbox(_b_18983); /*bool*/
  _x20870 = kk_std_core__mlift17734_op(_accm0, pred2, x3, xx2, _x20871, _ctx); /*list<11950>*/
  return kk_std_core__list_box(_x20870, _ctx);
}


// lift anonymous function
struct kk_std_core__ctailm_filter_fun20873__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t x3;
};
static kk_std_core__list kk_std_core__ctailm_filter_fun20873(kk_function_t _fself, kk_std_core__list _ctail_173560, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_filter_fun20873(kk_function_t _accm0, kk_box_t x3, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_filter_fun20873__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_filter_fun20873__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_filter_fun20873, kk_context());
  _self->_accm0 = _accm0;
  _self->x3 = x3;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_filter_fun20873(kk_function_t _fself, kk_std_core__list _ctail_173560, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_filter_fun20873__t* _self = kk_function_as(struct kk_std_core__ctailm_filter_fun20873__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<11950>) -> list<11950> */
  kk_box_t x3 = _self->x3; /* 11950 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(x3);}, {}, _ctx)
  kk_std_core__list _x20874 = kk_std_core__new_Cons(kk_reuse_null, x3, _ctail_173560, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x20874, _ctx));
}

kk_std_core__list kk_std_core__ctailm_filter(kk_std_core__list xs0, kk_function_t pred2, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, pred : (a) -> e bool, (list<a>) -> list<a>) -> e list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs0)) {
    struct kk_std_core_Cons* _con20865 = kk_std_core__as_Cons(xs0);
    kk_box_t x3 = _con20865->head;
    kk_std_core__list xx2 = _con20865->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs0))) {
      kk_std_core__list_free(xs0);
    }
    else {
      kk_box_dup(x3);
      kk_std_core__list_dup(xx2);
      kk_std_core__list_decref(xs0, _ctx);
    }
    bool x4_18001;
    kk_function_t _x20867 = kk_function_dup(pred2); /*(11950) -> 11951 bool*/
    kk_box_t _x20866 = kk_box_dup(x3); /*11950*/
    x4_18001 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), _x20867, (_x20867, _x20866, _ctx)); /*bool*/
    if (kk_yielding(kk_context())) {
      kk_box_t _x20868 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_filter_fun20869(_accm0, pred2, x3, xx2, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20868, _ctx);
    }
    if (x4_18001) { // tailcall
                    kk_function_t _x20872 = kk_std_core__new_ctailm_filter_fun20873(_accm0, x3, _ctx); /*(list<11950>) -> list<11950>*/
                    xs0 = xx2;
                    _accm0 = _x20872;
                    goto kk__tailcall;
    }
    {
      kk_box_drop(x3, _ctx);
      { // tailcall
        xs0 = xx2;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_function_drop(pred2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// Retain only those elements of a list that satisfy the given predicate `pred`.
// For example: `filter([1,2,3],odd?) == [1,3]`


// lift anonymous function
struct kk_std_core_filter_fun20876__t {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core_filter_fun20876(kk_function_t _fself, kk_std_core__list _ctail_17355, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_filter_fun20876(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_filter_fun20876, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core_filter_fun20876(kk_function_t _fself, kk_std_core__list _ctail_17355, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17355;
}

kk_std_core__list kk_std_core_filter(kk_std_core__list xs1, kk_function_t pred3, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, pred : (a) -> e bool) -> e list<a> */ 
  bool _match_19690 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19690) {
    kk_std_core_types__ctail _x20875 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_filter(xs1, pred3, _x20875, _ctx);
  }
  {
    return kk_std_core__ctailm_filter(xs1, pred3, kk_std_core_new_filter_fun20876(_ctx), _ctx);
  }
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17735_op(kk_std_core_types__ctail _acc, kk_function_t pred, kk_std_core__list xx, kk_std_core_types__maybe _y_17536, kk_context_t* _ctx) { /* forall<a,b,e> (ctail<list<b>>, pred : (a) -> e maybe<b>, xx : list<a>, maybe<b>) -> e list<b> */ 
  if (kk_std_core_types__is_Nothing(_y_17536)) {
    return kk_std_core__ctail_filter_map(xx, pred, _acc, _ctx);
  }
  {
    kk_box_t y = _y_17536._cons.Just.value;
    kk_std_core__list _ctail_17357 = kk_std_core__list_hole(); /*list<11996>*/;
    kk_std_core__list _ctail_17358 = kk_std_core__new_Cons(kk_reuse_null, y, _ctail_17357, _ctx); /*list<11996>*/;
    kk_std_core_types__ctail _x20877;
    kk_box_t* _b_18994_18991 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17358)->tail)); /*cfield<list<11996>>*/;
    _x20877 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17358, _ctx)),_b_18994_18991); /*ctail<0>*/
    return kk_std_core__ctail_filter_map(xx, pred, _x20877, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17736_op_fun20878__t {
  struct kk_function_s _base;
  kk_function_t _accm;
  kk_box_t y0;
};
static kk_std_core__list kk_std_core__mlift17736_op_fun20878(kk_function_t _fself, kk_std_core__list _ctail_17360, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17736_op_fun20878(kk_function_t _accm, kk_box_t y0, kk_context_t* _ctx) {
  struct kk_std_core__mlift17736_op_fun20878__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17736_op_fun20878__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17736_op_fun20878, kk_context());
  _self->_accm = _accm;
  _self->y0 = y0;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__mlift17736_op_fun20878(kk_function_t _fself, kk_std_core__list _ctail_17360, kk_context_t* _ctx) {
  struct kk_std_core__mlift17736_op_fun20878__t* _self = kk_function_as(struct kk_std_core__mlift17736_op_fun20878__t*, _fself);
  kk_function_t _accm = _self->_accm; /* (list<11996>) -> list<11996> */
  kk_box_t y0 = _self->y0; /* 11996 */
  kk_drop_match(_self, {kk_function_dup(_accm);kk_box_dup(y0);}, {}, _ctx)
  kk_std_core__list _x20879 = kk_std_core__new_Cons(kk_reuse_null, y0, _ctail_17360, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, _x20879, _ctx));
}

kk_std_core__list kk_std_core__mlift17736_op(kk_function_t _accm, kk_function_t pred0, kk_std_core__list xx0, kk_std_core_types__maybe _y_17541, kk_context_t* _ctx) { /* forall<a,b,e> ((list<b>) -> list<b>, pred : (a) -> e maybe<b>, xx : list<a>, maybe<b>) -> e list<b> */ 
  if (kk_std_core_types__is_Nothing(_y_17541)) {
    return kk_std_core__ctailm_filter_map(xx0, pred0, _accm, _ctx);
  }
  {
    kk_box_t y0 = _y_17541._cons.Just.value;
    return kk_std_core__ctailm_filter_map(xx0, pred0, kk_std_core__new_mlift17736_op_fun20878(_accm, y0, _ctx), _ctx);
  }
}
 
// Retain only those elements of a list that satisfy the given predicate `pred`.
// For example: `filterMap([1,2,3],fn(i) { if (i.odd?) then Nothing else Just(i*i) }) == [4]`


// lift anonymous function
struct kk_std_core__ctail_filter_map_fun20884__t {
  struct kk_function_s _base;
  kk_function_t pred1;
  kk_std_core__list xx1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_filter_map_fun20884(kk_function_t _fself, kk_box_t _b_19001, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_filter_map_fun20884(kk_function_t pred1, kk_std_core__list xx1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_filter_map_fun20884__t* _self = kk_function_alloc_as(struct kk_std_core__ctail_filter_map_fun20884__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_filter_map_fun20884, kk_context());
  _self->pred1 = pred1;
  _self->xx1 = xx1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_filter_map_fun20884(kk_function_t _fself, kk_box_t _b_19001, kk_context_t* _ctx) {
  struct kk_std_core__ctail_filter_map_fun20884__t* _self = kk_function_as(struct kk_std_core__ctail_filter_map_fun20884__t*, _fself);
  kk_function_t pred1 = _self->pred1; /* (11995) -> 11997 maybe<11996> */
  kk_std_core__list xx1 = _self->xx1; /* list<11995> */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<11996>> */
  kk_drop_match(_self, {kk_function_dup(pred1);kk_std_core__list_dup(xx1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x20885;
  kk_std_core_types__maybe _x20886 = kk_std_core_types__maybe_unbox(_b_19001, _ctx); /*maybe<11996>*/
  _x20885 = kk_std_core__mlift17735_op(_acc0, pred1, xx1, _x20886, _ctx); /*list<11996>*/
  return kk_std_core__list_box(_x20885, _ctx);
}

kk_std_core__list kk_std_core__ctail_filter_map(kk_std_core__list xs, kk_function_t pred1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, pred : (a) -> e maybe<b>, ctail<list<b>>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Nil(xs)) {
    kk_function_drop(pred1, _ctx);
    kk_box_t _x20880 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20880, _ctx);
  }
  {
    struct kk_std_core_Cons* _con20881 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20881->head;
    kk_std_core__list xx1 = _con20881->tail;
    kk_reuse_t _ru_19983 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      _ru_19983 = (kk_std_core__list_reuse(xs));
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx1);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_std_core_types__maybe x0_18004;
    kk_function_t _x20882 = kk_function_dup(pred1); /*(11995) -> 11997 maybe<11996>*/
    x0_18004 = kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_box_t, kk_context_t*), _x20882, (_x20882, x, _ctx)); /*maybe<11996>*/
    if (kk_yielding(kk_context())) {
      kk_reuse_drop(_ru_19983);
      kk_std_core_types__maybe_drop(x0_18004, _ctx);
      kk_box_t _x20883 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_filter_map_fun20884(pred1, xx1, _acc0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20883, _ctx);
    }
    if (kk_std_core_types__is_Nothing(x0_18004)) {
      kk_reuse_drop(_ru_19983);
      { // tailcall
        xs = xx1;
        goto kk__tailcall;
      }
    }
    {
      kk_box_t y1 = x0_18004._cons.Just.value;
      kk_std_core__list _ctail_173570 = kk_std_core__list_hole(); /*list<11996>*/;
      kk_std_core__list _ctail_173580 = kk_std_core__new_Cons(_ru_19983, y1, _ctail_173570, _ctx); /*list<11996>*/;
      { // tailcall
        kk_std_core_types__ctail _x20887;
        kk_box_t* _b_19013_19007 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_173580)->tail)); /*cfield<list<11996>>*/;
        _x20887 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_173580, _ctx)),_b_19013_19007); /*ctail<0>*/
        xs = xx1;
        _acc0 = _x20887;
        goto kk__tailcall;
      }
    }
  }
}
 
// Retain only those elements of a list that satisfy the given predicate `pred`.
// For example: `filterMap([1,2,3],fn(i) { if (i.odd?) then Nothing else Just(i*i) }) == [4]`


// lift anonymous function
struct kk_std_core__ctailm_filter_map_fun20891__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t pred2;
  kk_std_core__list xx2;
};
static kk_box_t kk_std_core__ctailm_filter_map_fun20891(kk_function_t _fself, kk_box_t _b_19019, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_filter_map_fun20891(kk_function_t _accm0, kk_function_t pred2, kk_std_core__list xx2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_filter_map_fun20891__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_filter_map_fun20891__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_filter_map_fun20891, kk_context());
  _self->_accm0 = _accm0;
  _self->pred2 = pred2;
  _self->xx2 = xx2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_filter_map_fun20891(kk_function_t _fself, kk_box_t _b_19019, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_filter_map_fun20891__t* _self = kk_function_as(struct kk_std_core__ctailm_filter_map_fun20891__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<11996>) -> list<11996> */
  kk_function_t pred2 = _self->pred2; /* (11995) -> 11997 maybe<11996> */
  kk_std_core__list xx2 = _self->xx2; /* list<11995> */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(pred2);kk_std_core__list_dup(xx2);}, {}, _ctx)
  kk_std_core__list _x20892;
  kk_std_core_types__maybe _x20893 = kk_std_core_types__maybe_unbox(_b_19019, _ctx); /*maybe<11996>*/
  _x20892 = kk_std_core__mlift17736_op(_accm0, pred2, xx2, _x20893, _ctx); /*list<11996>*/
  return kk_std_core__list_box(_x20892, _ctx);
}


// lift anonymous function
struct kk_std_core__ctailm_filter_map_fun20895__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t y2;
};
static kk_std_core__list kk_std_core__ctailm_filter_map_fun20895(kk_function_t _fself, kk_std_core__list _ctail_173600, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_filter_map_fun20895(kk_function_t _accm0, kk_box_t y2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_filter_map_fun20895__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_filter_map_fun20895__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_filter_map_fun20895, kk_context());
  _self->_accm0 = _accm0;
  _self->y2 = y2;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_filter_map_fun20895(kk_function_t _fself, kk_std_core__list _ctail_173600, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_filter_map_fun20895__t* _self = kk_function_as(struct kk_std_core__ctailm_filter_map_fun20895__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<11996>) -> list<11996> */
  kk_box_t y2 = _self->y2; /* 11996 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(y2);}, {}, _ctx)
  kk_std_core__list _x20896 = kk_std_core__new_Cons(kk_reuse_null, y2, _ctail_173600, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x20896, _ctx));
}

kk_std_core__list kk_std_core__ctailm_filter_map(kk_std_core__list xs0, kk_function_t pred2, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, pred : (a) -> e maybe<b>, (list<b>) -> list<b>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Nil(xs0)) {
    kk_function_drop(pred2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
  {
    struct kk_std_core_Cons* _con20888 = kk_std_core__as_Cons(xs0);
    kk_box_t x1 = _con20888->head;
    kk_std_core__list xx2 = _con20888->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs0))) {
      kk_std_core__list_free(xs0);
    }
    else {
      kk_box_dup(x1);
      kk_std_core__list_dup(xx2);
      kk_std_core__list_decref(xs0, _ctx);
    }
    kk_std_core_types__maybe x2_18007;
    kk_function_t _x20889 = kk_function_dup(pred2); /*(11995) -> 11997 maybe<11996>*/
    x2_18007 = kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_box_t, kk_context_t*), _x20889, (_x20889, x1, _ctx)); /*maybe<11996>*/
    if (kk_yielding(kk_context())) {
      kk_std_core_types__maybe_drop(x2_18007, _ctx);
      kk_box_t _x20890 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_filter_map_fun20891(_accm0, pred2, xx2, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20890, _ctx);
    }
    if (kk_std_core_types__is_Nothing(x2_18007)) { // tailcall
                                                   xs0 = xx2;
                                                   goto kk__tailcall;
    }
    {
      kk_box_t y2 = x2_18007._cons.Just.value;
      { // tailcall
        kk_function_t _x20894 = kk_std_core__new_ctailm_filter_map_fun20895(_accm0, y2, _ctx); /*(list<11996>) -> list<11996>*/
        xs0 = xx2;
        _accm0 = _x20894;
        goto kk__tailcall;
      }
    }
  }
}
 
// Retain only those elements of a list that satisfy the given predicate `pred`.
// For example: `filterMap([1,2,3],fn(i) { if (i.odd?) then Nothing else Just(i*i) }) == [4]`


// lift anonymous function
struct kk_std_core_filter_map_fun20898__t {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core_filter_map_fun20898(kk_function_t _fself, kk_std_core__list _ctail_17359, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_filter_map_fun20898(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_filter_map_fun20898, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core_filter_map_fun20898(kk_function_t _fself, kk_std_core__list _ctail_17359, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17359;
}

kk_std_core__list kk_std_core_filter_map(kk_std_core__list xs1, kk_function_t pred3, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, pred : (a) -> e maybe<b>) -> e list<b> */ 
  bool _match_19687 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19687) {
    kk_std_core_types__ctail _x20897 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_filter_map(xs1, pred3, _x20897, _ctx);
  }
  {
    return kk_std_core__ctailm_filter_map(xs1, pred3, kk_std_core_new_filter_map_fun20898(_ctx), _ctx);
  }
}

bool kk_std_core_is_zero_1(kk_ssize_t i, kk_context_t* _ctx) { /* (i : ssize_t) -> bool */ 
  return (i == 0);
}
 
// Find the first element satisfying some predicate


// lift anonymous function
struct kk_std_core_find_fun20899__t {
  struct kk_function_s _base;
  kk_function_t pred;
};
static kk_std_core_types__maybe kk_std_core_find_fun20899(kk_function_t _fself, kk_box_t x, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_find_fun20899(kk_function_t pred, kk_context_t* _ctx) {
  struct kk_std_core_find_fun20899__t* _self = kk_function_alloc_as(struct kk_std_core_find_fun20899__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_find_fun20899, kk_context());
  _self->pred = pred;
  return &_self->_base;
}

static kk_std_core_types__maybe kk_std_core_find_fun20899(kk_function_t _fself, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_find_fun20899__t* _self = kk_function_as(struct kk_std_core_find_fun20899__t*, _fself);
  kk_function_t pred = _self->pred; /* (12094) -> bool */
  kk_drop_match(_self, {kk_function_dup(pred);}, {}, _ctx)
  bool _match_19686;
  kk_box_t _x20900 = kk_box_dup(x); /*12094*/
  _match_19686 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), pred, (pred, _x20900, _ctx)); /*bool*/
  if (_match_19686) {
    return kk_std_core_types__new_Just(x, _ctx);
  }
  {
    kk_box_drop(x, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
}

kk_std_core_types__maybe kk_std_core_find(kk_std_core__list xs, kk_function_t pred, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, pred : (a) -> bool) -> maybe<a> */ 
  return kk_std_core_foreach_while(xs, kk_std_core_new_find_fun20899(pred, _ctx), _ctx);
}
 
// O(n). If it occurs, return the position of substring `sub` in `s`, tupled with
// the position just following the substring `sub`.

kk_std_core_types__maybe kk_std_core_find_1(kk_string_t s, kk_string_t sub, kk_context_t* _ctx) { /* (s : string, sub : string) -> maybe<sslice> */ 
  kk_ssize_t i;
  kk_string_t _x20901 = kk_string_dup(s); /*string*/
  kk_string_t _x20902 = kk_string_dup(sub); /*string*/
  i = kk_string_index_of1(_x20901,_x20902,kk_context()); /*ssize_t*/
  bool _match_19685 = kk_std_core_is_zero_1(i, _ctx); /*bool*/;
  if (_match_19685) {
    kk_string_drop(sub, _ctx);
    kk_string_drop(s, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
  {
    kk_std_core__sslice _b_19023_19022;
    kk_ssize_t _x20903 = kk_std_core_decr_1(i, _ctx); /*ssize_t*/
    kk_ssize_t _x20904 = kk_string_len(sub,kk_context()); /*ssize_t*/
    _b_19023_19022 = kk_std_core__new_Sslice(s, _x20903, _x20904, _ctx); /*sslice*/
    return kk_std_core_types__new_Just(kk_std_core__sslice_box(_b_19023_19022, _ctx), _ctx);
  }
}
 
// Return the last index of substring `sub` in `s` if it occurs.

kk_std_core_types__maybe kk_std_core_find_last(kk_string_t s, kk_string_t sub, kk_context_t* _ctx) { /* (s : string, sub : string) -> maybe<sslice> */ 
  kk_ssize_t i;
  kk_string_t _x20905 = kk_string_dup(s); /*string*/
  kk_string_t _x20906 = kk_string_dup(sub); /*string*/
  i = kk_string_last_index_of1(_x20905,_x20906,kk_context()); /*ssize_t*/
  bool _match_19684 = kk_std_core_is_zero_1(i, _ctx); /*bool*/;
  if (_match_19684) {
    kk_string_drop(sub, _ctx);
    kk_string_drop(s, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
  {
    kk_std_core__sslice _b_19025_19024;
    kk_ssize_t _x20907 = kk_std_core_decr_1(i, _ctx); /*ssize_t*/
    kk_ssize_t _x20908 = kk_string_len(sub,kk_context()); /*ssize_t*/
    _b_19025_19024 = kk_std_core__new_Sslice(s, _x20907, _x20908, _ctx); /*sslice*/
    return kk_std_core_types__new_Just(kk_std_core__sslice_box(_b_19025_19024, _ctx), _ctx);
  }
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17737_op(kk_std_core_types__ctail _acc, kk_function_t f, kk_std_core__list xx, kk_std_core_types__maybe _y_17549, kk_context_t* _ctx) { /* forall<a,b,e> (ctail<list<b>>, f : (a) -> e maybe<b>, xx : list<a>, maybe<b>) -> e list<b> */ 
  if (kk_std_core_types__is_Just(_y_17549)) {
    kk_box_t y = _y_17549._cons.Just.value;
    kk_std_core__list _ctail_17361 = kk_std_core__list_hole(); /*list<12300>*/;
    kk_std_core__list _ctail_17362 = kk_std_core__new_Cons(kk_reuse_null, y, _ctail_17361, _ctx); /*list<12300>*/;
    kk_std_core_types__ctail _x20909;
    kk_box_t* _b_19034_19031 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17362)->tail)); /*cfield<list<12300>>*/;
    _x20909 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17362, _ctx)),_b_19034_19031); /*ctail<0>*/
    return kk_std_core__ctail_flatmap_maybe(xx, f, _x20909, _ctx);
  }
  {
    return kk_std_core__ctail_flatmap_maybe(xx, f, _acc, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17738_op_fun20910__t {
  struct kk_function_s _base;
  kk_function_t _accm;
  kk_box_t y0;
};
static kk_std_core__list kk_std_core__mlift17738_op_fun20910(kk_function_t _fself, kk_std_core__list _ctail_17364, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17738_op_fun20910(kk_function_t _accm, kk_box_t y0, kk_context_t* _ctx) {
  struct kk_std_core__mlift17738_op_fun20910__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17738_op_fun20910__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17738_op_fun20910, kk_context());
  _self->_accm = _accm;
  _self->y0 = y0;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__mlift17738_op_fun20910(kk_function_t _fself, kk_std_core__list _ctail_17364, kk_context_t* _ctx) {
  struct kk_std_core__mlift17738_op_fun20910__t* _self = kk_function_as(struct kk_std_core__mlift17738_op_fun20910__t*, _fself);
  kk_function_t _accm = _self->_accm; /* (list<12300>) -> list<12300> */
  kk_box_t y0 = _self->y0; /* 12300 */
  kk_drop_match(_self, {kk_function_dup(_accm);kk_box_dup(y0);}, {}, _ctx)
  kk_std_core__list _x20911 = kk_std_core__new_Cons(kk_reuse_null, y0, _ctail_17364, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, _x20911, _ctx));
}

kk_std_core__list kk_std_core__mlift17738_op(kk_function_t _accm, kk_function_t f0, kk_std_core__list xx0, kk_std_core_types__maybe _y_17554, kk_context_t* _ctx) { /* forall<a,b,e> ((list<b>) -> list<b>, f : (a) -> e maybe<b>, xx : list<a>, maybe<b>) -> e list<b> */ 
  if (kk_std_core_types__is_Just(_y_17554)) {
    kk_box_t y0 = _y_17554._cons.Just.value;
    return kk_std_core__ctailm_flatmap_maybe(xx0, f0, kk_std_core__new_mlift17738_op_fun20910(_accm, y0, _ctx), _ctx);
  }
  {
    return kk_std_core__ctailm_flatmap_maybe(xx0, f0, _accm, _ctx);
  }
}
 
// Concatenate the `Just` result elements from applying a function to all elements.


// lift anonymous function
struct kk_std_core__ctail_flatmap_maybe_fun20915__t {
  struct kk_function_s _base;
  kk_function_t f1;
  kk_std_core__list xx1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_flatmap_maybe_fun20915(kk_function_t _fself, kk_box_t _b_19039, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_flatmap_maybe_fun20915(kk_function_t f1, kk_std_core__list xx1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_flatmap_maybe_fun20915__t* _self = kk_function_alloc_as(struct kk_std_core__ctail_flatmap_maybe_fun20915__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_flatmap_maybe_fun20915, kk_context());
  _self->f1 = f1;
  _self->xx1 = xx1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_flatmap_maybe_fun20915(kk_function_t _fself, kk_box_t _b_19039, kk_context_t* _ctx) {
  struct kk_std_core__ctail_flatmap_maybe_fun20915__t* _self = kk_function_as(struct kk_std_core__ctail_flatmap_maybe_fun20915__t*, _fself);
  kk_function_t f1 = _self->f1; /* (12299) -> 12301 maybe<12300> */
  kk_std_core__list xx1 = _self->xx1; /* list<12299> */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<12300>> */
  kk_drop_match(_self, {kk_function_dup(f1);kk_std_core__list_dup(xx1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x20916;
  kk_std_core_types__maybe _x20917 = kk_std_core_types__maybe_unbox(_b_19039, _ctx); /*maybe<12300>*/
  _x20916 = kk_std_core__mlift17737_op(_acc0, f1, xx1, _x20917, _ctx); /*list<12300>*/
  return kk_std_core__list_box(_x20916, _ctx);
}

kk_std_core__list kk_std_core__ctail_flatmap_maybe(kk_std_core__list xs, kk_function_t f1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, f : (a) -> e maybe<b>, ctail<list<b>>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20912 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20912->head;
    kk_std_core__list xx1 = _con20912->tail;
    kk_reuse_t _ru_19985 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      _ru_19985 = (kk_std_core__list_reuse(xs));
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx1);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_std_core_types__maybe x0_18010;
    kk_function_t _x20913 = kk_function_dup(f1); /*(12299) -> 12301 maybe<12300>*/
    x0_18010 = kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_box_t, kk_context_t*), _x20913, (_x20913, x, _ctx)); /*maybe<12300>*/
    if (kk_yielding(kk_context())) {
      kk_reuse_drop(_ru_19985);
      kk_std_core_types__maybe_drop(x0_18010, _ctx);
      kk_box_t _x20914 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_flatmap_maybe_fun20915(f1, xx1, _acc0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20914, _ctx);
    }
    if (kk_std_core_types__is_Just(x0_18010)) {
      kk_box_t y1 = x0_18010._cons.Just.value;
      kk_std_core__list _ctail_173610 = kk_std_core__list_hole(); /*list<12300>*/;
      kk_std_core__list _ctail_173620 = kk_std_core__new_Cons(_ru_19985, y1, _ctail_173610, _ctx); /*list<12300>*/;
      { // tailcall
        kk_std_core_types__ctail _x20918;
        kk_box_t* _b_19051_19045 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_173620)->tail)); /*cfield<list<12300>>*/;
        _x20918 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_173620, _ctx)),_b_19051_19045); /*ctail<0>*/
        xs = xx1;
        _acc0 = _x20918;
        goto kk__tailcall;
      }
    }
    {
      kk_reuse_drop(_ru_19985);
      { // tailcall
        xs = xx1;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_function_drop(f1, _ctx);
    kk_box_t _x20919 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x20919, _ctx);
  }
}
 
// Concatenate the `Just` result elements from applying a function to all elements.


// lift anonymous function
struct kk_std_core__ctailm_flatmap_maybe_fun20923__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t f2;
  kk_std_core__list xx2;
};
static kk_box_t kk_std_core__ctailm_flatmap_maybe_fun20923(kk_function_t _fself, kk_box_t _b_19059, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_flatmap_maybe_fun20923(kk_function_t _accm0, kk_function_t f2, kk_std_core__list xx2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_flatmap_maybe_fun20923__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_flatmap_maybe_fun20923__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_flatmap_maybe_fun20923, kk_context());
  _self->_accm0 = _accm0;
  _self->f2 = f2;
  _self->xx2 = xx2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_flatmap_maybe_fun20923(kk_function_t _fself, kk_box_t _b_19059, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_flatmap_maybe_fun20923__t* _self = kk_function_as(struct kk_std_core__ctailm_flatmap_maybe_fun20923__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<12300>) -> list<12300> */
  kk_function_t f2 = _self->f2; /* (12299) -> 12301 maybe<12300> */
  kk_std_core__list xx2 = _self->xx2; /* list<12299> */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(f2);kk_std_core__list_dup(xx2);}, {}, _ctx)
  kk_std_core__list _x20924;
  kk_std_core_types__maybe _x20925 = kk_std_core_types__maybe_unbox(_b_19059, _ctx); /*maybe<12300>*/
  _x20924 = kk_std_core__mlift17738_op(_accm0, f2, xx2, _x20925, _ctx); /*list<12300>*/
  return kk_std_core__list_box(_x20924, _ctx);
}


// lift anonymous function
struct kk_std_core__ctailm_flatmap_maybe_fun20927__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t y2;
};
static kk_std_core__list kk_std_core__ctailm_flatmap_maybe_fun20927(kk_function_t _fself, kk_std_core__list _ctail_173640, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_flatmap_maybe_fun20927(kk_function_t _accm0, kk_box_t y2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_flatmap_maybe_fun20927__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_flatmap_maybe_fun20927__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_flatmap_maybe_fun20927, kk_context());
  _self->_accm0 = _accm0;
  _self->y2 = y2;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_flatmap_maybe_fun20927(kk_function_t _fself, kk_std_core__list _ctail_173640, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_flatmap_maybe_fun20927__t* _self = kk_function_as(struct kk_std_core__ctailm_flatmap_maybe_fun20927__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<12300>) -> list<12300> */
  kk_box_t y2 = _self->y2; /* 12300 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(y2);}, {}, _ctx)
  kk_std_core__list _x20928 = kk_std_core__new_Cons(kk_reuse_null, y2, _ctail_173640, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x20928, _ctx));
}

kk_std_core__list kk_std_core__ctailm_flatmap_maybe(kk_std_core__list xs0, kk_function_t f2, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, f : (a) -> e maybe<b>, (list<b>) -> list<b>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs0)) {
    struct kk_std_core_Cons* _con20920 = kk_std_core__as_Cons(xs0);
    kk_box_t x1 = _con20920->head;
    kk_std_core__list xx2 = _con20920->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs0))) {
      kk_std_core__list_free(xs0);
    }
    else {
      kk_box_dup(x1);
      kk_std_core__list_dup(xx2);
      kk_std_core__list_decref(xs0, _ctx);
    }
    kk_std_core_types__maybe x2_18013;
    kk_function_t _x20921 = kk_function_dup(f2); /*(12299) -> 12301 maybe<12300>*/
    x2_18013 = kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_box_t, kk_context_t*), _x20921, (_x20921, x1, _ctx)); /*maybe<12300>*/
    if (kk_yielding(kk_context())) {
      kk_std_core_types__maybe_drop(x2_18013, _ctx);
      kk_box_t _x20922 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_flatmap_maybe_fun20923(_accm0, f2, xx2, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x20922, _ctx);
    }
    if (kk_std_core_types__is_Just(x2_18013)) {
      kk_box_t y2 = x2_18013._cons.Just.value;
      { // tailcall
        kk_function_t _x20926 = kk_std_core__new_ctailm_flatmap_maybe_fun20927(_accm0, y2, _ctx); /*(list<12300>) -> list<12300>*/
        xs0 = xx2;
        _accm0 = _x20926;
        goto kk__tailcall;
      }
    }
    { // tailcall
      xs0 = xx2;
      goto kk__tailcall;
    }
  }
  {
    kk_function_drop(f2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// Concatenate the `Just` result elements from applying a function to all elements.


// lift anonymous function
struct kk_std_core_flatmap_maybe_fun20930__t {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core_flatmap_maybe_fun20930(kk_function_t _fself, kk_std_core__list _ctail_17363, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_flatmap_maybe_fun20930(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_flatmap_maybe_fun20930, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core_flatmap_maybe_fun20930(kk_function_t _fself, kk_std_core__list _ctail_17363, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17363;
}

kk_std_core__list kk_std_core_flatmap_maybe(kk_std_core__list xs1, kk_function_t f3, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, f : (a) -> e maybe<b>) -> e list<b> */ 
  bool _match_19681 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19681) {
    kk_std_core_types__ctail _x20929 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_flatmap_maybe(xs1, f3, _x20929, _ctx);
  }
  {
    return kk_std_core__ctailm_flatmap_maybe(xs1, f3, kk_std_core_new_flatmap_maybe_fun20930(_ctx), _ctx);
  }
}
 
// monadic lift

kk_box_t kk_std_core__mlift17739_fold_int(kk_integer_t end, kk_function_t f, kk_integer_t start0, kk_box_t x, kk_context_t* _ctx) { /* forall<a,e> (end : int, f : (int, a) -> e a, start0 : int, x : a) -> e a */ 
  kk_integer_t _x20931 = kk_integer_add(start0,(kk_integer_from_small(1)),kk_context()); /*int*/
  return kk_std_core_fold_int(_x20931, end, x, f, _ctx);
}
 
// fold over the integers between [`start`,`end`] (inclusive).


// lift anonymous function
struct kk_std_core_fold_int_fun20934__t {
  struct kk_function_s _base;
  kk_integer_t end0;
  kk_function_t f0;
  kk_integer_t start00;
};
static kk_box_t kk_std_core_fold_int_fun20934(kk_function_t _fself, kk_box_t x1, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_fold_int_fun20934(kk_integer_t end0, kk_function_t f0, kk_integer_t start00, kk_context_t* _ctx) {
  struct kk_std_core_fold_int_fun20934__t* _self = kk_function_alloc_as(struct kk_std_core_fold_int_fun20934__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_fold_int_fun20934, kk_context());
  _self->end0 = end0;
  _self->f0 = f0;
  _self->start00 = start00;
  return &_self->_base;
}

static kk_box_t kk_std_core_fold_int_fun20934(kk_function_t _fself, kk_box_t x1, kk_context_t* _ctx) {
  struct kk_std_core_fold_int_fun20934__t* _self = kk_function_as(struct kk_std_core_fold_int_fun20934__t*, _fself);
  kk_integer_t end0 = _self->end0; /* int */
  kk_function_t f0 = _self->f0; /* (int, 12360) -> 12361 12360 */
  kk_integer_t start00 = _self->start00; /* int */
  kk_drop_match(_self, {kk_integer_dup(end0);kk_function_dup(f0);kk_integer_dup(start00);}, {}, _ctx)
  return kk_std_core__mlift17739_fold_int(end0, f0, start00, x1, _ctx);
}

kk_box_t kk_std_core_fold_int(kk_integer_t start00, kk_integer_t end0, kk_box_t init0, kk_function_t f0, kk_context_t* _ctx) { /* forall<a,e> (start : int, end : int, init : a, f : (int, a) -> e a) -> e a */ 
  kk__tailcall: ;
  bool _match_19679 = kk_integer_gte_borrow(start00,end0,kk_context()); /*bool*/;
  if (_match_19679) {
    kk_integer_drop(start00, _ctx);
    kk_function_drop(f0, _ctx);
    kk_integer_drop(end0, _ctx);
    return init0;
  }
  {
    kk_box_t x0_18016;
    kk_function_t _x20933 = kk_function_dup(f0); /*(int, 12360) -> 12361 12360*/
    kk_integer_t _x20932 = kk_integer_dup(start00); /*int*/
    x0_18016 = kk_function_call(kk_box_t, (kk_function_t, kk_integer_t, kk_box_t, kk_context_t*), _x20933, (_x20933, _x20932, init0, _ctx)); /*12360*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x0_18016, _ctx);
      return kk_std_core_hnd_yield_extend(kk_std_core_new_fold_int_fun20934(end0, f0, start00, _ctx), _ctx);
    }
    { // tailcall
      kk_integer_t _x20935 = kk_integer_add(start00,(kk_integer_from_small(1)),kk_context()); /*int*/
      start00 = _x20935;
      init0 = x0_18016;
      goto kk__tailcall;
    }
  }
}
 
// monadic lift

kk_box_t kk_std_core__mlift17740_foldl(kk_function_t f, kk_std_core__list xx, kk_box_t _y_17566, kk_context_t* _ctx) { /* forall<a,e,b> (f : (a, b) -> e a, xx : list<b>, a) -> e a */ 
  return kk_std_core_foldl(xx, _y_17566, f, _ctx);
}
 
// Fold a list from the left, i.e. `foldl([1,2],0,(+)) == (0+1)+2`
// Since `foldl` is tail recursive, it is preferred over `foldr` when using an associative function `f`


// lift anonymous function
struct kk_std_core_foldl_fun20939__t {
  struct kk_function_s _base;
  kk_function_t f0;
  kk_std_core__list xx0;
};
static kk_box_t kk_std_core_foldl_fun20939(kk_function_t _fself, kk_box_t _y_175660, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foldl_fun20939(kk_function_t f0, kk_std_core__list xx0, kk_context_t* _ctx) {
  struct kk_std_core_foldl_fun20939__t* _self = kk_function_alloc_as(struct kk_std_core_foldl_fun20939__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foldl_fun20939, kk_context());
  _self->f0 = f0;
  _self->xx0 = xx0;
  return &_self->_base;
}

static kk_box_t kk_std_core_foldl_fun20939(kk_function_t _fself, kk_box_t _y_175660, kk_context_t* _ctx) {
  struct kk_std_core_foldl_fun20939__t* _self = kk_function_as(struct kk_std_core_foldl_fun20939__t*, _fself);
  kk_function_t f0 = _self->f0; /* (12420, 12425) -> 12422 12420 */
  kk_std_core__list xx0 = _self->xx0; /* list<12425> */
  kk_drop_match(_self, {kk_function_dup(f0);kk_std_core__list_dup(xx0);}, {}, _ctx)
  return kk_std_core__mlift17740_foldl(f0, xx0, _y_175660, _ctx);
}

kk_box_t kk_std_core_foldl(kk_std_core__list xs, kk_box_t z, kk_function_t f0, kk_context_t* _ctx) { /* forall<a,b,e> (list<a>, b, (b, a) -> e b) -> e b */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20937 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20937->head;
    kk_std_core__list xx0 = _con20937->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx0);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_box_t x0_18019;
    kk_function_t _x20938 = kk_function_dup(f0); /*(12420, 12425) -> 12422 12420*/
    x0_18019 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), _x20938, (_x20938, z, x, _ctx)); /*12420*/
    if (kk_yielding(kk_context())) {
      kk_box_drop(x0_18019, _ctx);
      return kk_std_core_hnd_yield_extend(kk_std_core_new_foldl_fun20939(f0, xx0, _ctx), _ctx);
    }
    { // tailcall
      xs = xx0;
      z = x0_18019;
      goto kk__tailcall;
    }
  }
  {
    kk_function_drop(f0, _ctx);
    return z;
  }
}


// lift anonymous function
struct kk_std_core_foldl1_fun20942__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_foldl1_fun20942(kk_function_t _fself, kk_box_t _b_19066, kk_box_t _b_19067, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foldl1_fun20942(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_foldl1_fun20942, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_foldl1_fun20942(kk_function_t _fself, kk_box_t _b_19066, kk_box_t _b_19067, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_string_t _x20943 = kk_string_unbox(_b_19066); /*string*/
  kk_std_core_types__optional _x20944 = kk_std_core_types__optional_unbox(_b_19067, _ctx); /*optional<exception-info>*/
  return kk_std_core_throw(_x20943, _x20944, _ctx);
}

kk_box_t kk_std_core_foldl1(kk_std_core__list xs, kk_function_t f, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, f : (a, a) -> <exn|e> a) -> <exn|e> a */ 
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con20940 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con20940->head;
    kk_std_core__list xx = _con20940->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    return kk_std_core_foldl(xx, x, f, _ctx);
  }
  {
    kk_function_drop(f, _ctx);
    kk_ssize_t _b_19068_19062;
    kk_std_core_hnd__htag _x20941 = kk_std_core_hnd__htag_dup(kk_std_core__tag_exn); /*std/core/hnd/htag<.hnd-exn>*/
    _b_19068_19062 = kk_std_core_hnd__evv_index(_x20941, _ctx); /*std/core/hnd/ev-index*/
    kk_box_t _x20945;
    kk_string_t _x20946;
    kk_define_string_literal(, _s20947, 33, "unexpected Nil in std/core/foldl1")
    _x20946 = kk_string_dup(_s20947); /*string*/
    _x20945 = kk_string_box(_x20946); /*5549*/
    return kk_std_core_hnd__open_at2(_b_19068_19062, kk_std_core_new_foldl1_fun20942(_ctx), _x20945, kk_std_core_types__optional_box(kk_std_core_types__new_None(_ctx), _ctx), _ctx);
  }
}
 
// lifted

kk_std_core__list kk_std_core__lift17274_reverse(kk_std_core__list acc, kk_std_core__list ys, kk_context_t* _ctx) { /* forall<a> (acc : list<a>, ys : list<a>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con20948 = kk_std_core__as_Cons(ys);
    kk_box_t x = _con20948->head;
    kk_std_core__list xx = _con20948->tail;
    kk_reuse_t _ru_19989 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      _ru_19989 = (kk_std_core__list_reuse(ys));
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(ys, _ctx);
    }
    { // tailcall
      kk_std_core__list _x20949 = kk_std_core__new_Cons(_ru_19989, x, acc, _ctx); /*list<61>*/
      acc = _x20949;
      ys = xx;
      goto kk__tailcall;
    }
  }
  {
    return acc;
  }
}
extern kk_box_t kk_std_core_foldr_fun20951(kk_function_t _fself, kk_box_t x, kk_box_t y, kk_context_t* _ctx) {
  struct kk_std_core_foldr_fun20951__t* _self = kk_function_as(struct kk_std_core_foldr_fun20951__t*, _fself);
  kk_function_t f = _self->f; /* (12511, 12507) -> 12509 12507 */
  kk_drop_match(_self, {kk_function_dup(f);}, {}, _ctx)
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), f, (f, y, x, _ctx));
}


// lift anonymous function
struct kk_std_core_foldr1_fun20954__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_foldr1_fun20954(kk_function_t _fself, kk_box_t _b_19076, kk_box_t _b_19077, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foldr1_fun20954(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_foldr1_fun20954, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_foldr1_fun20954(kk_function_t _fself, kk_box_t _b_19076, kk_box_t _b_19077, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_string_t _x20955 = kk_string_unbox(_b_19076); /*string*/
  kk_std_core_types__optional _x20956 = kk_std_core_types__optional_unbox(_b_19077, _ctx); /*optional<exception-info>*/
  return kk_std_core_throw(_x20955, _x20956, _ctx);
}

kk_box_t kk_std_core_foldr1(kk_std_core__list xs, kk_function_t f, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, f : (a, a) -> <exn|e> a) -> <exn|e> a */ 
  kk_std_core__list xs0_17179 = kk_std_core__lift17274_reverse(kk_std_core__new_Nil(_ctx), xs, _ctx); /*list<12545>*/;
  if (kk_std_core__is_Cons(xs0_17179)) {
    struct kk_std_core_Cons* _con20952 = kk_std_core__as_Cons(xs0_17179);
    kk_box_t x = _con20952->head;
    kk_std_core__list xx = _con20952->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs0_17179))) {
      kk_std_core__list_free(xs0_17179);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs0_17179, _ctx);
    }
    return kk_std_core_foldl(xx, x, f, _ctx);
  }
  {
    kk_function_drop(f, _ctx);
    kk_ssize_t _b_19078_19072;
    kk_std_core_hnd__htag _x20953 = kk_std_core_hnd__htag_dup(kk_std_core__tag_exn); /*std/core/hnd/htag<.hnd-exn>*/
    _b_19078_19072 = kk_std_core_hnd__evv_index(_x20953, _ctx); /*std/core/hnd/ev-index*/
    kk_box_t _x20957;
    kk_string_t _x20958;
    kk_define_string_literal(, _s20959, 33, "unexpected Nil in std/core/foldl1")
    _x20958 = kk_string_dup(_s20959); /*string*/
    _x20957 = kk_string_box(_x20958); /*5549*/
    return kk_std_core_hnd__open_at2(_b_19078_19072, kk_std_core_new_foldr1_fun20954(_ctx), _x20957, kk_std_core_types__optional_box(kk_std_core_types__new_None(_ctx), _ctx), _ctx);
  }
}
 
// monadic lift

kk_unit_t kk_std_core__mlift17741_op(kk_function_t action, kk_integer_t end, kk_integer_t i, kk_unit_t wild__, kk_context_t* _ctx) { /* forall<e> (action : (int) -> e (), end : int, i : int, wild_ : ()) -> e () */ 
  kk_integer_t i0_17302 = kk_integer_add(i,(kk_integer_from_small(1)),kk_context()); /*int*/;
  kk_std_core__lift17275_for(action, end, i0_17302, _ctx); return kk_Unit;
}
 
// lifted


// lift anonymous function
struct kk_std_core__lift17275_for_fun20963__t {
  struct kk_function_s _base;
  kk_function_t action0;
  kk_integer_t end0;
  kk_integer_t i0;
};
static kk_box_t kk_std_core__lift17275_for_fun20963(kk_function_t _fself, kk_box_t _b_19083, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17275_for_fun20963(kk_function_t action0, kk_integer_t end0, kk_integer_t i0, kk_context_t* _ctx) {
  struct kk_std_core__lift17275_for_fun20963__t* _self = kk_function_alloc_as(struct kk_std_core__lift17275_for_fun20963__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17275_for_fun20963, kk_context());
  _self->action0 = action0;
  _self->end0 = end0;
  _self->i0 = i0;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17275_for_fun20963(kk_function_t _fself, kk_box_t _b_19083, kk_context_t* _ctx) {
  struct kk_std_core__lift17275_for_fun20963__t* _self = kk_function_as(struct kk_std_core__lift17275_for_fun20963__t*, _fself);
  kk_function_t action0 = _self->action0; /* (int) -> 12582 () */
  kk_integer_t end0 = _self->end0; /* int */
  kk_integer_t i0 = _self->i0; /* int */
  kk_drop_match(_self, {kk_function_dup(action0);kk_integer_dup(end0);kk_integer_dup(i0);}, {}, _ctx)
  kk_unit_t _x20964 = kk_Unit;
  kk_unit_t _x20965 = kk_Unit;
  kk_unit_unbox(_b_19083);
  kk_std_core__mlift17741_op(action0, end0, i0, _x20965, _ctx);
  return kk_unit_box(_x20964);
}

kk_unit_t kk_std_core__lift17275_for(kk_function_t action0, kk_integer_t end0, kk_integer_t i0, kk_context_t* _ctx) { /* forall<e> (action : (int) -> e (), end : int, i : int) -> e () */ 
  kk__tailcall: ;
  bool _match_19676 = kk_integer_lte_borrow(i0,end0,kk_context()); /*bool*/;
  if (_match_19676) {
    kk_unit_t x_18024 = kk_Unit;
    kk_function_t _x20961 = kk_function_dup(action0); /*(int) -> 12582 ()*/
    kk_integer_t _x20960 = kk_integer_dup(i0); /*int*/
    kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), _x20961, (_x20961, _x20960, _ctx));
    if (kk_yielding(kk_context())) {
      kk_box_t _x20962 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17275_for_fun20963(action0, end0, i0, _ctx), _ctx); /*3860*/
      kk_unit_unbox(_x20962); return kk_Unit;
    }
    {
      kk_integer_t i0_173020 = kk_integer_add(i0,(kk_integer_from_small(1)),kk_context()); /*int*/;
      { // tailcall
        i0 = i0_173020;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_integer_drop(i0, _ctx);
    kk_integer_drop(end0, _ctx);
    kk_function_drop(action0, _ctx);
    kk_Unit; return kk_Unit;
  }
}
 
// monadic lift

kk_std_core_types__maybe kk_std_core__mlift17742_op(kk_function_t action, kk_integer_t end, kk_integer_t i, kk_std_core_types__maybe _y_17581, kk_context_t* _ctx) { /* forall<a,e> (action : (int) -> e maybe<a>, end : int, i : int, maybe<a>) -> e maybe<a> */ 
  if (kk_std_core_types__is_Nothing(_y_17581)) {
    kk_integer_t i0_17303 = kk_integer_add(i,(kk_integer_from_small(1)),kk_context()); /*int*/;
    return kk_std_core__lift17276_for_while(action, end, i0_17303, _ctx);
  }
  {
    kk_box_t x = _y_17581._cons.Just.value;
    kk_integer_drop(i, _ctx);
    kk_integer_drop(end, _ctx);
    kk_function_drop(action, _ctx);
    return kk_std_core_types__new_Just(x, _ctx);
  }
}
 
// lifted


// lift anonymous function
struct kk_std_core__lift17276_for_while_fun20970__t {
  struct kk_function_s _base;
  kk_function_t action0;
  kk_integer_t end0;
  kk_integer_t i0;
};
static kk_box_t kk_std_core__lift17276_for_while_fun20970(kk_function_t _fself, kk_box_t _b_19087, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17276_for_while_fun20970(kk_function_t action0, kk_integer_t end0, kk_integer_t i0, kk_context_t* _ctx) {
  struct kk_std_core__lift17276_for_while_fun20970__t* _self = kk_function_alloc_as(struct kk_std_core__lift17276_for_while_fun20970__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17276_for_while_fun20970, kk_context());
  _self->action0 = action0;
  _self->end0 = end0;
  _self->i0 = i0;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17276_for_while_fun20970(kk_function_t _fself, kk_box_t _b_19087, kk_context_t* _ctx) {
  struct kk_std_core__lift17276_for_while_fun20970__t* _self = kk_function_as(struct kk_std_core__lift17276_for_while_fun20970__t*, _fself);
  kk_function_t action0 = _self->action0; /* (int) -> 12633 maybe<12632> */
  kk_integer_t end0 = _self->end0; /* int */
  kk_integer_t i0 = _self->i0; /* int */
  kk_drop_match(_self, {kk_function_dup(action0);kk_integer_dup(end0);kk_integer_dup(i0);}, {}, _ctx)
  kk_std_core_types__maybe _x20971;
  kk_std_core_types__maybe _x20972 = kk_std_core_types__maybe_unbox(_b_19087, _ctx); /*maybe<12632>*/
  _x20971 = kk_std_core__mlift17742_op(action0, end0, i0, _x20972, _ctx); /*maybe<12632>*/
  return kk_std_core_types__maybe_box(_x20971, _ctx);
}

kk_std_core_types__maybe kk_std_core__lift17276_for_while(kk_function_t action0, kk_integer_t end0, kk_integer_t i0, kk_context_t* _ctx) { /* forall<a,e> (action : (int) -> e maybe<a>, end : int, i : int) -> e maybe<a> */ 
  kk__tailcall: ;
  bool _match_19674 = kk_integer_lte_borrow(i0,end0,kk_context()); /*bool*/;
  if (_match_19674) {
    kk_std_core_types__maybe x0_18027;
    kk_function_t _x20968 = kk_function_dup(action0); /*(int) -> 12633 maybe<12632>*/
    kk_integer_t _x20967 = kk_integer_dup(i0); /*int*/
    x0_18027 = kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_integer_t, kk_context_t*), _x20968, (_x20968, _x20967, _ctx)); /*maybe<12632>*/
    if (kk_yielding(kk_context())) {
      kk_std_core_types__maybe_drop(x0_18027, _ctx);
      kk_box_t _x20969 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17276_for_while_fun20970(action0, end0, i0, _ctx), _ctx); /*3860*/
      return kk_std_core_types__maybe_unbox(_x20969, _ctx);
    }
    if (kk_std_core_types__is_Nothing(x0_18027)) {
      kk_integer_t i0_173030 = kk_integer_add(i0,(kk_integer_from_small(1)),kk_context()); /*int*/;
      { // tailcall
        i0 = i0_173030;
        goto kk__tailcall;
      }
    }
    {
      kk_box_t x1 = x0_18027._cons.Just.value;
      kk_integer_drop(i0, _ctx);
      kk_integer_drop(end0, _ctx);
      kk_function_drop(action0, _ctx);
      return kk_std_core_types__new_Just(x1, _ctx);
    }
  }
  {
    kk_integer_drop(i0, _ctx);
    kk_integer_drop(end0, _ctx);
    kk_function_drop(action0, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17744_foreach_indexed_fun20975__t {
  struct kk_function_s _base;
  kk_ref_t i;
};
static kk_unit_t kk_std_core__mlift17744_foreach_indexed_fun20975(kk_function_t _fself, kk_integer_t _y_17589, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17744_foreach_indexed_fun20975(kk_ref_t i, kk_context_t* _ctx) {
  struct kk_std_core__mlift17744_foreach_indexed_fun20975__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17744_foreach_indexed_fun20975__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17744_foreach_indexed_fun20975, kk_context());
  _self->i = i;
  return &_self->_base;
}

static kk_unit_t kk_std_core__mlift17744_foreach_indexed_fun20975(kk_function_t _fself, kk_integer_t _y_17589, kk_context_t* _ctx) {
  struct kk_std_core__mlift17744_foreach_indexed_fun20975__t* _self = kk_function_as(struct kk_std_core__mlift17744_foreach_indexed_fun20975__t*, _fself);
  kk_ref_t i = _self->i; /* local-var<12737,int> */
  kk_drop_match(_self, {kk_ref_dup(i);}, {}, _ctx)
  kk_integer_t _b_19099_19097 = kk_integer_add(_y_17589,(kk_integer_from_small(1)),kk_context()); /*int*/;
  kk_unit_t _brw_19672 = kk_Unit;
  (kk_ref_set_borrow(i,(kk_integer_box(_b_19099_19097)),kk_context()));
  kk_ref_drop(i, _ctx);
  return _brw_19672;
}


// lift anonymous function
struct kk_std_core__mlift17744_foreach_indexed_fun20977__t {
  struct kk_function_s _base;
  kk_function_t next0_18031;
};
static kk_box_t kk_std_core__mlift17744_foreach_indexed_fun20977(kk_function_t _fself, kk_box_t _b_19101, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17744_foreach_indexed_fun20977(kk_function_t next0_18031, kk_context_t* _ctx) {
  struct kk_std_core__mlift17744_foreach_indexed_fun20977__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17744_foreach_indexed_fun20977__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17744_foreach_indexed_fun20977, kk_context());
  _self->next0_18031 = next0_18031;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17744_foreach_indexed_fun20977(kk_function_t _fself, kk_box_t _b_19101, kk_context_t* _ctx) {
  struct kk_std_core__mlift17744_foreach_indexed_fun20977__t* _self = kk_function_as(struct kk_std_core__mlift17744_foreach_indexed_fun20977__t*, _fself);
  kk_function_t next0_18031 = _self->next0_18031; /* (int) -> <local<12737>|12744> () */
  kk_drop_match(_self, {kk_function_dup(next0_18031);}, {}, _ctx)
  kk_unit_t _x20978 = kk_Unit;
  kk_integer_t _x20979 = kk_integer_unbox(_b_19101); /*int*/
  kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), next0_18031, (next0_18031, _x20979, _ctx));
  return kk_unit_box(_x20978);
}

kk_unit_t kk_std_core__mlift17744_foreach_indexed(kk_ref_t i, kk_unit_t wild__, kk_context_t* _ctx) { /* forall<h,e> (i : local-var<h,int>, wild_ : ()) -> <local<h>|e> () */ 
  kk_integer_t x_18030;
  kk_box_t _x20973;
  kk_ref_t _x20974 = kk_ref_dup(i); /*local-var<12737,int>*/
  _x20973 = (kk_ref_get(_x20974,kk_context())); /*258*/
  x_18030 = kk_integer_unbox(_x20973); /*int*/
  kk_function_t next0_18031 = kk_std_core__new_mlift17744_foreach_indexed_fun20975(i, _ctx); /*(int) -> <local<12737>|12744> ()*/;
  if (kk_yielding(kk_context())) {
    kk_integer_drop(x_18030, _ctx);
    kk_box_t _x20976 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17744_foreach_indexed_fun20977(next0_18031, _ctx), _ctx); /*3860*/
    kk_unit_unbox(_x20976); return kk_Unit;
  }
  {
    kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), next0_18031, (next0_18031, x_18030, _ctx)); return kk_Unit;
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17745_foreach_indexed_fun20981__t {
  struct kk_function_s _base;
  kk_ref_t i;
};
static kk_box_t kk_std_core__mlift17745_foreach_indexed_fun20981(kk_function_t _fself, kk_box_t _b_19104, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17745_foreach_indexed_fun20981(kk_ref_t i, kk_context_t* _ctx) {
  struct kk_std_core__mlift17745_foreach_indexed_fun20981__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17745_foreach_indexed_fun20981__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17745_foreach_indexed_fun20981, kk_context());
  _self->i = i;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17745_foreach_indexed_fun20981(kk_function_t _fself, kk_box_t _b_19104, kk_context_t* _ctx) {
  struct kk_std_core__mlift17745_foreach_indexed_fun20981__t* _self = kk_function_as(struct kk_std_core__mlift17745_foreach_indexed_fun20981__t*, _fself);
  kk_ref_t i = _self->i; /* local-var<12737,int> */
  kk_drop_match(_self, {kk_ref_dup(i);}, {}, _ctx)
  kk_unit_t _x20982 = kk_Unit;
  kk_unit_t _x20983 = kk_Unit;
  kk_unit_unbox(_b_19104);
  kk_std_core__mlift17744_foreach_indexed(i, _x20983, _ctx);
  return kk_unit_box(_x20982);
}

kk_unit_t kk_std_core__mlift17745_foreach_indexed(kk_function_t action, kk_ref_t i, kk_box_t x, kk_integer_t j, kk_context_t* _ctx) { /* forall<h,a,e> (action : (int, a) -> e (), i : local-var<h,int>, x : a, j : int) -> <local<h>|e> () */ 
  kk_unit_t x0_18034 = kk_Unit;
  kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_box_t, kk_context_t*), action, (action, j, x, _ctx));
  if (kk_yielding(kk_context())) {
    kk_box_t _x20980 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17745_foreach_indexed_fun20981(i, _ctx), _ctx); /*3860*/
    kk_unit_unbox(_x20980); return kk_Unit;
  }
  {
    kk_std_core__mlift17744_foreach_indexed(i, x0_18034, _ctx); return kk_Unit;
  }
}
 
// Invoke `action` for each element of a list, passing also the position of the element.


// lift anonymous function
struct kk_std_core_foreach_indexed_fun20985__t {
  struct kk_function_s _base;
  kk_function_t action;
  kk_ref_t loc;
};
static kk_unit_t kk_std_core_foreach_indexed_fun20985(kk_function_t _fself, kk_box_t x, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_indexed_fun20985(kk_function_t action, kk_ref_t loc, kk_context_t* _ctx) {
  struct kk_std_core_foreach_indexed_fun20985__t* _self = kk_function_alloc_as(struct kk_std_core_foreach_indexed_fun20985__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_indexed_fun20985, kk_context());
  _self->action = action;
  _self->loc = loc;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_foreach_indexed_fun20989__t {
  struct kk_function_s _base;
  kk_function_t action;
  kk_ref_t loc;
  kk_box_t x;
};
static kk_box_t kk_std_core_foreach_indexed_fun20989(kk_function_t _fself, kk_box_t _b_19112, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_indexed_fun20989(kk_function_t action, kk_ref_t loc, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_foreach_indexed_fun20989__t* _self = kk_function_alloc_as(struct kk_std_core_foreach_indexed_fun20989__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_indexed_fun20989, kk_context());
  _self->action = action;
  _self->loc = loc;
  _self->x = x;
  return &_self->_base;
}

static kk_box_t kk_std_core_foreach_indexed_fun20989(kk_function_t _fself, kk_box_t _b_19112, kk_context_t* _ctx) {
  struct kk_std_core_foreach_indexed_fun20989__t* _self = kk_function_as(struct kk_std_core_foreach_indexed_fun20989__t*, _fself);
  kk_function_t action = _self->action; /* (int, 12743) -> 12744 () */
  kk_ref_t loc = _self->loc; /* local-var<12737,int> */
  kk_box_t x = _self->x; /* 12743 */
  kk_drop_match(_self, {kk_function_dup(action);kk_ref_dup(loc);kk_box_dup(x);}, {}, _ctx)
  kk_unit_t _x20990 = kk_Unit;
  kk_integer_t _x20991 = kk_integer_unbox(_b_19112); /*int*/
  kk_std_core__mlift17745_foreach_indexed(action, loc, x, _x20991, _ctx);
  return kk_unit_box(_x20990);
}
static kk_unit_t kk_std_core_foreach_indexed_fun20985(kk_function_t _fself, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_foreach_indexed_fun20985__t* _self = kk_function_as(struct kk_std_core_foreach_indexed_fun20985__t*, _fself);
  kk_function_t action = _self->action; /* (int, 12743) -> 12744 () */
  kk_ref_t loc = _self->loc; /* local-var<12737,int> */
  kk_drop_match(_self, {kk_function_dup(action);kk_ref_dup(loc);}, {}, _ctx)
  kk_integer_t x0_18039;
  kk_box_t _x20986;
  kk_ref_t _x20987 = kk_ref_dup(loc); /*local-var<12737,int>*/
  _x20986 = (kk_ref_get(_x20987,kk_context())); /*258*/
  x0_18039 = kk_integer_unbox(_x20986); /*int*/
  if (kk_yielding(kk_context())) {
    kk_integer_drop(x0_18039, _ctx);
    kk_box_t _x20988 = kk_std_core_hnd_yield_extend(kk_std_core_new_foreach_indexed_fun20989(action, loc, x, _ctx), _ctx); /*3860*/
    return kk_unit_unbox(_x20988);
  }
  {
    return kk_std_core__mlift17745_foreach_indexed(action, loc, x, x0_18039, _ctx);
  }
}

kk_unit_t kk_std_core_foreach_indexed(kk_std_core__list xs, kk_function_t action, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, action : (int, a) -> e ()) -> e () */ 
  kk_ref_t loc = kk_ref_alloc((kk_integer_box(kk_integer_from_small(0))),kk_context()); /*local-var<12737,int>*/;
  kk_unit_t res = kk_Unit;
  kk_function_t _x20984;
  kk_ref_dup(loc);
  _x20984 = kk_std_core_new_foreach_indexed_fun20985(action, loc, _ctx); /*(x : 12743) -> <local<12737>|12744> ()*/
  kk_std_core_foreach(xs, _x20984, _ctx);
  kk_box_t _x20992 = kk_std_core_hnd_prompt_local_var(loc, kk_unit_box(res), _ctx); /*9897*/
  kk_unit_unbox(_x20992); return kk_Unit;
}
 
// Invoke a function `f` for each element in a vector `v`


// lift anonymous function
struct kk_std_core_foreach_indexed_fun20994__t_1 {
  struct kk_function_s _base;
  kk_function_t f;
  kk_vector_t v;
};
static kk_unit_t kk_std_core_foreach_indexed_fun20994_1(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_foreach_indexed_fun20994_1(kk_function_t f, kk_vector_t v, kk_context_t* _ctx) {
  struct kk_std_core_foreach_indexed_fun20994__t_1* _self = kk_function_alloc_as(struct kk_std_core_foreach_indexed_fun20994__t_1, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_foreach_indexed_fun20994_1, kk_context());
  _self->f = f;
  _self->v = v;
  return &_self->_base;
}

static kk_unit_t kk_std_core_foreach_indexed_fun20994_1(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_foreach_indexed_fun20994__t_1* _self = kk_function_as(struct kk_std_core_foreach_indexed_fun20994__t_1*, _fself);
  kk_function_t f = _self->f; /* (12773, int) -> 12774 () */
  kk_vector_t v = _self->v; /* vector<12773> */
  kk_drop_match(_self, {kk_function_dup(f);kk_vector_dup(v);}, {}, _ctx)
  kk_box_t x_18179;
  kk_box_t _brw_19668 = kk_vector_at_borrow(v, i); /*223*/;
  kk_vector_drop(v, _ctx);
  x_18179 = _brw_19668; /*12773*/
  kk_integer_t _x20995 = kk_integer_from_ssize_t(i,kk_context()); /*int*/
  return kk_function_call(kk_unit_t, (kk_function_t, kk_box_t, kk_integer_t, kk_context_t*), f, (f, x_18179, _x20995, _ctx));
}

kk_unit_t kk_std_core_foreach_indexed_1(kk_vector_t v, kk_function_t f, kk_context_t* _ctx) { /* forall<a,e> (v : vector<a>, f : (a, int) -> e ()) -> e () */ 
  kk_ssize_t start0_18043 = ((kk_ssize_t)0); /*ssize_t*/;
  kk_ssize_t end_18044;
  kk_ssize_t _x20993 = kk_vector_len_borrow(v); /*ssize_t*/
  end_18044 = kk_std_core_decr_1(_x20993, _ctx); /*ssize_t*/
  kk_std_core__lift17269_forz(kk_std_core_new_foreach_indexed_fun20994_1(f, v, _ctx), end_18044, start0_18043, _ctx); return kk_Unit;
}
 
// Return the head of list if the list is not empty.

kk_std_core_types__maybe kk_std_core_head_1(kk_std_core__list xs, kk_context_t* _ctx) { /* forall<a> (xs : list<a>) -> maybe<a> */ 
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21004 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21004->head;
    kk_std_core__list _pat0 = _con21004->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_drop(_pat0, _ctx);
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_decref(xs, _ctx);
    }
    return kk_std_core_types__new_Just(x, _ctx);
  }
  {
    return kk_std_core_types__new_Nothing(_ctx);
  }
}
 
// Return the head of list if the list is not empty, or use `default` otherwise

kk_box_t kk_std_core_head_2(kk_std_core__list xs, kk_box_t default0, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, default : a) -> a */ 
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21005 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21005->head;
    kk_std_core__list _pat0 = _con21005->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_drop(_pat0, _ctx);
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_box_drop(default0, _ctx);
    return x;
  }
  {
    return default0;
  }
}
 
// Return the first character of a string as a string (or the empty string)

kk_string_t kk_std_core_head_3(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> string */ 
  kk_std_core__sslice _x21006;
  kk_std_core__sslice slice0 = kk_std_core_first1(s, _ctx); /*sslice*/;
  bool _match_19660;
  kk_integer_t _brw_19665;
  kk_std_core_types__optional _match_19666 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
  if (kk_std_core_types__is_Optional(_match_19666)) {
    kk_box_t _box_x19119 = _match_19666._cons.Optional.value;
    kk_integer_t _n_10165 = kk_integer_unbox(_box_x19119);
    kk_integer_dup(_n_10165);
    kk_std_core_types__optional_drop(_match_19666, _ctx);
    _brw_19665 = _n_10165; /*int*/
    goto _match21007;
  }
  {
    _brw_19665 = kk_integer_from_small(1); /*int*/
  }
  _match21007: ;
  kk_integer_t _brw_19664 = kk_integer_from_small(1); /*int*/;
  bool _brw_19667 = kk_integer_eq_borrow(_brw_19665,_brw_19664,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19665, _ctx);
  kk_integer_drop(_brw_19664, _ctx);
  _match_19660 = _brw_19667; /*bool*/
  if (_match_19660) {
    _x21006 = slice0; /*sslice*/
  }
  else {
    kk_integer_t _brw_19661;
    kk_integer_t _x21009;
    kk_std_core_types__optional _match_19662 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
    if (kk_std_core_types__is_Optional(_match_19662)) {
      kk_box_t _box_x19120 = _match_19662._cons.Optional.value;
      kk_integer_t _n_101650 = kk_integer_unbox(_box_x19120);
      kk_integer_dup(_n_101650);
      kk_std_core_types__optional_drop(_match_19662, _ctx);
      _x21009 = _n_101650; /*int*/
      goto _match21010;
    }
    {
      _x21009 = kk_integer_from_small(1); /*int*/
    }
    _match21010: ;
    _brw_19661 = kk_integer_sub(_x21009,(kk_integer_from_small(1)),kk_context()); /*int*/
    kk_std_core__sslice _brw_19663 = kk_std_core_extend(slice0, _brw_19661, _ctx); /*sslice*/;
    kk_integer_drop(_brw_19661, _ctx);
    _x21006 = _brw_19663; /*sslice*/
  }
  return kk_std_core_string_3(_x21006, _ctx);
}
 
// Return the first character of a string (or `Nothing` for the empty string).


// lift anonymous function
struct kk_std_core_head_char_fun21016__t {
  struct kk_function_s _base;
};
static kk_std_core_types__maybe kk_std_core_head_char_fun21016(kk_function_t _fself, kk_char_t _b_19121, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_head_char_fun21016(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_head_char_fun21016, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core_types__maybe kk_std_core_head_char_fun21016(kk_function_t _fself, kk_char_t _b_19121, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return kk_std_core_types__new_Just(kk_char_box(_b_19121, _ctx), _ctx);
}

kk_std_core_types__maybe kk_std_core_head_char(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> maybe<char> */ 
  kk_std_core__sslice _x21012;
  kk_string_t _x21013 = kk_string_dup(s); /*string*/
  kk_ssize_t _x21014 = ((kk_ssize_t)0); /*ssize_t*/
  kk_ssize_t _x21015 = kk_string_len(s,kk_context()); /*ssize_t*/
  _x21012 = kk_std_core__new_Sslice(_x21013, _x21014, _x21015, _ctx); /*sslice*/
  return kk_std_core_foreach_while_1(_x21012, kk_std_core_new_head_char_fun21016(_ctx), _ctx);
}

kk_integer_t kk_std_core_index_of_acc(kk_std_core__list xs, kk_function_t pred, kk_integer_t idx, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, pred : (a) -> bool, idx : int) -> int */ 
  kk__tailcall: ;
  if (kk_std_core__is_Nil(xs)) {
    kk_function_drop(pred, _ctx);
    kk_integer_drop(idx, _ctx);
    return kk_integer_sub((kk_integer_from_small(0)),(kk_integer_from_small(1)),kk_context());
  }
  {
    struct kk_std_core_Cons* _con21017 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21017->head;
    kk_std_core__list xx = _con21017->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    bool _match_19659;
    kk_function_t _x21018 = kk_function_dup(pred); /*(13150) -> bool*/
    _match_19659 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), _x21018, (_x21018, x, _ctx)); /*bool*/
    if (_match_19659) {
      kk_std_core__list_drop(xx, _ctx);
      kk_function_drop(pred, _ctx);
      return idx;
    }
    { // tailcall
      kk_integer_t _x21019 = kk_integer_add(idx,(kk_integer_from_small(1)),kk_context()); /*int*/
      xs = xx;
      idx = _x21019;
      goto kk__tailcall;
    }
  }
}
 
// Return the list without its last element.
// Return an empty list for an empty list.

kk_std_core__list kk_std_core__ctail_init(kk_std_core__list xs, kk_std_core_types__ctail _acc, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, ctail<list<a>>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21020 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21020->head;
    kk_std_core__list xx = _con21020->tail;
    if (kk_std_core__is_Cons(xx)) {
      struct kk_std_core_Cons* _con21021 = kk_std_core__as_Cons(xx);
      kk_reuse_t _ru_19994 = kk_reuse_null; /*reuse*/;
      if (kk_likely(kk_std_core__list_is_unique(xs))) {
        _ru_19994 = (kk_std_core__list_reuse(xs));
      }
      else {
        kk_box_dup(x);
        kk_std_core__list_dup(xx);
        kk_std_core__list_decref(xs, _ctx);
      }
      kk_std_core__list _ctail_17365 = kk_std_core__list_hole(); /*list<13187>*/;
      kk_std_core__list _ctail_17366 = kk_std_core__new_Cons(_ru_19994, x, _ctail_17365, _ctx); /*list<13187>*/;
      { // tailcall
        kk_std_core_types__ctail _x21022;
        kk_box_t* _b_19132_19127 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17366)->tail)); /*cfield<list<13187>>*/;
        _x21022 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17366, _ctx)),_b_19132_19127); /*ctail<0>*/
        xs = xx;
        _acc = _x21022;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_std_core__list_drop(xs, _ctx);
    kk_box_t _x21023 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x21023, _ctx);
  }
}
 
// Return the list without its last element.
// Return an empty list for an empty list.

kk_std_core__list kk_std_core_init(kk_std_core__list xs0, kk_context_t* _ctx) { /* forall<a> (xs : list<a>) -> list<a> */ 
  kk_std_core_types__ctail _x21024 = kk_ctail_nil(); /*ctail<0>*/
  return kk_std_core__ctail_init(xs0, _x21024, _ctx);
}
 
// An invalid slice

kk_std_core__sslice kk_std_core_invalid;
 
// Is the character an ASCII letter is-

bool kk_std_core_is_alpha(kk_char_t c, kk_context_t* _ctx) { /* (c : char) -> bool */ 
  bool _match_19653 = (c >= ('a')); /*bool*/;
  if (_match_19653) {
    bool _match_19655 = (c <= ('z')); /*bool*/;
    if (_match_19655) {
      return true;
    }
    {
      bool _match_19656 = (c >= ('A')); /*bool*/;
      if (_match_19656) {
        return (c <= ('Z'));
      }
      {
        return false;
      }
    }
  }
  {
    bool _match_19654 = (c >= ('A')); /*bool*/;
    if (_match_19654) {
      return (c <= ('Z'));
    }
    {
      return false;
    }
  }
}
 
// Is the character an ASCII hexa-decimal digit ?

bool kk_std_core_is_hex_digit(kk_char_t c, kk_context_t* _ctx) { /* (c : char) -> bool */ 
  bool _match_19640 = (c >= ('0')); /*bool*/;
  if (_match_19640) {
    bool _match_19645 = (c <= ('9')); /*bool*/;
    if (_match_19645) {
      return true;
    }
    {
      bool _match_19646 = (c >= ('a')); /*bool*/;
      if (_match_19646) {
        bool _match_19648 = (c <= ('f')); /*bool*/;
        if (_match_19648) {
          return true;
        }
        {
          bool _match_19649 = (c >= ('A')); /*bool*/;
          if (_match_19649) {
            return (c <= ('F'));
          }
          {
            return false;
          }
        }
      }
      {
        bool _match_19647 = (c >= ('A')); /*bool*/;
        if (_match_19647) {
          return (c <= ('F'));
        }
        {
          return false;
        }
      }
    }
  }
  {
    bool _match_19641 = (c >= ('a')); /*bool*/;
    if (_match_19641) {
      bool _match_19643 = (c <= ('f')); /*bool*/;
      if (_match_19643) {
        return true;
      }
      {
        bool _match_19644 = (c >= ('A')); /*bool*/;
        if (_match_19644) {
          return (c <= ('F'));
        }
        {
          return false;
        }
      }
    }
    {
      bool _match_19642 = (c >= ('A')); /*bool*/;
      if (_match_19642) {
        return (c <= ('F'));
      }
      {
        return false;
      }
    }
  }
}
 
// Tests if a character is an element of `" \t\n\r"`

bool kk_std_core_is_white(kk_char_t c, kk_context_t* _ctx) { /* (c : char) -> bool */ 
  bool _match_19637 = (c == (' ')); /*bool*/;
  if (_match_19637) {
    return true;
  }
  {
    bool _match_19638 = (c == 0x0009); /*bool*/;
    if (_match_19638) {
      return true;
    }
    {
      bool _match_19639 = (c == 0x000A); /*bool*/;
      if (_match_19639) {
        return true;
      }
      {
        return (c == 0x000D);
      }
    }
  }
}
 
// Append `end` to each string in the list `xs` and join them all together.
// `join-end([],end) === ""`
// `join-end(["a","b"],"/") === "a/b/"`

kk_string_t kk_std_core_join_end(kk_std_core__list xs, kk_string_t end, kk_context_t* _ctx) { /* (xs : list<string>, end : string) -> string */ 
  if (kk_std_core__is_Nil(xs)) {
    kk_string_drop(end, _ctx);
    return kk_string_empty();
  }
  {
    kk_string_t _x21035;
    if (kk_std_core__is_Nil(xs)) {
      _x21035 = kk_string_empty(); /*string*/
    }
    else {
      struct kk_std_core_Cons* _con21037 = kk_std_core__as_Cons(xs);
      kk_box_t _box_x19138 = _con21037->head;
      kk_std_core__list xx = _con21037->tail;
      kk_string_t x = kk_string_unbox(_box_x19138);
      if (kk_likely(kk_std_core__list_is_unique(xs))) {
        kk_std_core__list_free(xs);
      }
      else {
        kk_string_dup(x);
        kk_std_core__list_dup(xx);
        kk_std_core__list_decref(xs, _ctx);
      }
      kk_string_t _x21039 = kk_string_dup(end); /*string*/
      _x21035 = kk_std_core__lift17267_joinsep(_x21039, xx, x, _ctx); /*string*/
    }
    return kk_std_core__lp__plus__plus__1_rp_(_x21035, end, _ctx);
  }
}
 
// Return the last element of a list (or `Nothing` for the empty list)

kk_std_core_types__maybe kk_std_core_last(kk_std_core__list xs, kk_context_t* _ctx) { /* forall<a> (xs : list<a>) -> maybe<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21040 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21040->head;
    kk_std_core__list _pat0 = _con21040->tail;
    if (kk_std_core__is_Nil(_pat0)) {
      if (kk_likely(kk_std_core__list_is_unique(xs))) {
        kk_std_core__list_free(xs);
      }
      else {
        kk_box_dup(x);
        kk_std_core__list_decref(xs, _ctx);
      }
      return kk_std_core_types__new_Just(x, _ctx);
    }
  }
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21041 = kk_std_core__as_Cons(xs);
    kk_box_t _pat2 = _con21041->head;
    kk_std_core__list xx = _con21041->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_box_drop(_pat2, _ctx);
      kk_std_core__list_free(xs);
    }
    else {
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    { // tailcall
      xs = xx;
      goto kk__tailcall;
    }
  }
  {
    return kk_std_core_types__new_Nothing(_ctx);
  }
}
 
// Return the last element of a list (or `default` for the empty list)

kk_box_t kk_std_core_last_1(kk_std_core__list xs, kk_box_t default0, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, default : a) -> a */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21042 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21042->head;
    kk_std_core__list _pat0 = _con21042->tail;
    if (kk_std_core__is_Nil(_pat0)) {
      if (kk_likely(kk_std_core__list_is_unique(xs))) {
        kk_std_core__list_free(xs);
      }
      else {
        kk_box_dup(x);
        kk_std_core__list_decref(xs, _ctx);
      }
      kk_box_drop(default0, _ctx);
      return x;
    }
  }
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21043 = kk_std_core__as_Cons(xs);
    kk_box_t _pat2 = _con21043->head;
    kk_std_core__list xx = _con21043->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_box_drop(_pat2, _ctx);
      kk_std_core__list_free(xs);
    }
    else {
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    { // tailcall
      xs = xx;
      goto kk__tailcall;
    }
  }
  {
    return default0;
  }
}
 
// O(`n`). The last `n` (default = `1`) characters in a string

kk_std_core__sslice kk_std_core_last_2(kk_string_t s, kk_std_core_types__optional n, kk_context_t* _ctx) { /* (s : string, n : optional<int>) -> sslice */ 
  kk_std_core__sslice slice0 = kk_std_core_last1(s, _ctx); /*sslice*/;
  bool _match_19628;
  kk_integer_t _brw_19635;
  if (kk_std_core_types__is_Optional(n)) {
    kk_box_t _box_x19139 = n._cons.Optional.value;
    kk_integer_t _n_13781 = kk_integer_unbox(_box_x19139);
    kk_integer_dup(_n_13781);
    _brw_19635 = _n_13781; /*int*/
    goto _match21044;
  }
  {
    _brw_19635 = kk_integer_from_small(1); /*int*/
  }
  _match21044: ;
  kk_integer_t _brw_19634 = kk_integer_from_small(1); /*int*/;
  bool _brw_19636 = kk_integer_eq_borrow(_brw_19635,_brw_19634,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19635, _ctx);
  kk_integer_drop(_brw_19634, _ctx);
  _match_19628 = _brw_19636; /*bool*/
  if (_match_19628) {
    kk_std_core_types__optional_drop(n, _ctx);
    return slice0;
  }
  {
    kk_std_core__sslice _own_19630;
    kk_integer_t _brw_19631;
    kk_integer_t _x21046;
    if (kk_std_core_types__is_Optional(n)) {
      kk_box_t _box_x19140 = n._cons.Optional.value;
      kk_integer_t _n_137810 = kk_integer_unbox(_box_x19140);
      kk_integer_dup(_n_137810);
      _x21046 = _n_137810; /*int*/
      goto _match21047;
    }
    {
      _x21046 = kk_integer_from_small(1); /*int*/
    }
    _match21047: ;
    _brw_19631 = kk_integer_sub((kk_integer_from_small(1)),_x21046,kk_context()); /*int*/
    kk_std_core__sslice _brw_19632 = kk_std_core_advance(slice0, _brw_19631, _ctx); /*sslice*/;
    kk_integer_drop(_brw_19631, _ctx);
    _own_19630 = _brw_19632; /*sslice*/
    kk_integer_t _brw_19629;
    kk_integer_t _x21049;
    if (kk_std_core_types__is_Optional(n)) {
      kk_box_t _box_x19141 = n._cons.Optional.value;
      kk_integer_t _n_137811 = kk_integer_unbox(_box_x19141);
      kk_integer_dup(_n_137811);
      kk_std_core_types__optional_drop(n, _ctx);
      _x21049 = _n_137811; /*int*/
      goto _match21050;
    }
    {
      _x21049 = kk_integer_from_small(1); /*int*/
    }
    _match21050: ;
    _brw_19629 = kk_integer_sub(_x21049,(kk_integer_from_small(1)),kk_context()); /*int*/
    kk_std_core__sslice _brw_19633 = kk_std_core_extend(_own_19630, _brw_19629, _ctx); /*sslice*/;
    kk_integer_drop(_brw_19629, _ctx);
    return _brw_19633;
  }
}
 
// Take the first `n` elements of a list (or fewer if the list is shorter than `n`)

kk_std_core__list kk_std_core__ctail_take(kk_std_core__list xs, kk_integer_t n, kk_std_core_types__ctail _acc, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, n : int, ctail<list<a>>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21052 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21052->head;
    kk_std_core__list xx = _con21052->tail;
    kk_integer_t _brw_19626 = kk_integer_from_small(0); /*int*/;
    bool _brw_19627 = kk_integer_gt_borrow(n,_brw_19626,kk_context()); /*bool*/;
    kk_integer_drop(_brw_19626, _ctx);
    if (_brw_19627) {
      kk_reuse_t _ru_20001 = kk_reuse_null; /*reuse*/;
      if (kk_likely(kk_std_core__list_is_unique(xs))) {
        _ru_20001 = (kk_std_core__list_reuse(xs));
      }
      else {
        kk_box_dup(x);
        kk_std_core__list_dup(xx);
        kk_std_core__list_decref(xs, _ctx);
      }
      kk_std_core__list _ctail_17367 = kk_std_core__list_hole(); /*list<13905>*/;
      kk_std_core__list _ctail_17368 = kk_std_core__new_Cons(_ru_20001, x, _ctail_17367, _ctx); /*list<13905>*/;
      { // tailcall
        kk_integer_t _x21053 = kk_integer_sub(n,(kk_integer_from_small(1)),kk_context()); /*int*/
        kk_std_core_types__ctail _x21054;
        kk_box_t* _b_19152_19147 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17368)->tail)); /*cfield<list<13905>>*/;
        _x21054 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17368, _ctx)),_b_19152_19147); /*ctail<0>*/
        xs = xx;
        n = _x21053;
        _acc = _x21054;
        goto kk__tailcall;
      }
    }
  }
  {
    kk_std_core__list_drop(xs, _ctx);
    kk_integer_drop(n, _ctx);
    kk_box_t _x21055 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x21055, _ctx);
  }
}
 
// Take the first `n` elements of a list (or fewer if the list is shorter than `n`)

kk_std_core__list kk_std_core_take(kk_std_core__list xs0, kk_integer_t n0, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, n : int) -> list<a> */ 
  kk_std_core_types__ctail _x21056 = kk_ctail_nil(); /*ctail<0>*/
  return kk_std_core__ctail_take(xs0, n0, _x21056, _ctx);
}
 
// split a list at position `n`

kk_std_core_types__tuple2_ kk_std_core_split(kk_std_core__list xs, kk_integer_t n, kk_context_t* _ctx) { /* forall<a> (xs : list<a>, n : int) -> (list<a>, list<a>) */ 
  kk_std_core__list _b_19160_19158;
  kk_std_core__list _x21057 = kk_std_core__list_dup(xs); /*list<13925>*/
  kk_integer_t _x21058 = kk_integer_dup(n); /*int*/
  _b_19160_19158 = kk_std_core_take(_x21057, _x21058, _ctx); /*list<13925>*/
  kk_std_core__list _b_19161_19159 = kk_std_core_drop(xs, n, _ctx); /*list<13925>*/;
  return kk_std_core_types__new_dash__lp__comma__rp_(kk_std_core__list_box(_b_19160_19158, _ctx), kk_std_core__list_box(_b_19161_19159, _ctx), _ctx);
}
 
// Lookup the first element satisfying some predicate


// lift anonymous function
struct kk_std_core_lookup_fun21063__t {
  struct kk_function_s _base;
  kk_function_t pred;
};
static kk_std_core_types__maybe kk_std_core_lookup_fun21063(kk_function_t _fself, kk_box_t _b_19164, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_lookup_fun21063(kk_function_t pred, kk_context_t* _ctx) {
  struct kk_std_core_lookup_fun21063__t* _self = kk_function_alloc_as(struct kk_std_core_lookup_fun21063__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_lookup_fun21063, kk_context());
  _self->pred = pred;
  return &_self->_base;
}

static kk_std_core_types__maybe kk_std_core_lookup_fun21063(kk_function_t _fself, kk_box_t _b_19164, kk_context_t* _ctx) {
  struct kk_std_core_lookup_fun21063__t* _self = kk_function_as(struct kk_std_core_lookup_fun21063__t*, _fself);
  kk_function_t pred = _self->pred; /* (14257) -> bool */
  kk_drop_match(_self, {kk_function_dup(pred);}, {}, _ctx)
  bool _match_19623;
  kk_box_t _x21064;
  kk_std_core_types__tuple2_ _match_19625;
  kk_box_t _x21065 = kk_box_dup(_b_19164); /*10698*/
  _match_19625 = kk_std_core_types__tuple2__unbox(_x21065, _ctx); /*(14257, 14258)*/
  {
    kk_box_t _x = _match_19625.fst;
    kk_box_dup(_x);
    kk_std_core_types__tuple2__drop(_match_19625, _ctx);
    _x21064 = _x; /*14257*/
  }
  _match_19623 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), pred, (pred, _x21064, _ctx)); /*bool*/
  if (_match_19623) {
    kk_box_t _x21066;
    kk_std_core_types__tuple2_ _match_19624 = kk_std_core_types__tuple2__unbox(_b_19164, _ctx); /*(14257, 14258)*/;
    {
      kk_box_t _x0 = _match_19624.snd;
      kk_box_dup(_x0);
      kk_std_core_types__tuple2__drop(_match_19624, _ctx);
      _x21066 = _x0; /*14258*/
    }
    return kk_std_core_types__new_Just(_x21066, _ctx);
  }
  {
    kk_box_drop(_b_19164, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
}

kk_std_core_types__maybe kk_std_core_lookup(kk_std_core__list xs, kk_function_t pred, kk_context_t* _ctx) { /* forall<a,b> (xs : list<(a, b)>, pred : (a) -> bool) -> maybe<b> */ 
  return kk_std_core_foreach_while(xs, kk_std_core_new_lookup_fun21063(pred, _ctx), _ctx);
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17746_op(kk_box_t _y_17596, kk_std_core__list _y_17597, kk_context_t* _ctx) { /* forall<a,e> (a, list<a>) -> e list<a> */ 
  return kk_std_core__new_Cons(kk_reuse_null, _y_17596, _y_17597, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17747_op_fun21068__t {
  struct kk_function_s _base;
  kk_box_t _y_175960;
};
static kk_box_t kk_std_core__mlift17747_op_fun21068(kk_function_t _fself, kk_box_t _b_19169, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17747_op_fun21068(kk_box_t _y_175960, kk_context_t* _ctx) {
  struct kk_std_core__mlift17747_op_fun21068__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17747_op_fun21068__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17747_op_fun21068, kk_context());
  _self->_y_175960 = _y_175960;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17747_op_fun21068(kk_function_t _fself, kk_box_t _b_19169, kk_context_t* _ctx) {
  struct kk_std_core__mlift17747_op_fun21068__t* _self = kk_function_as(struct kk_std_core__mlift17747_op_fun21068__t*, _fself);
  kk_box_t _y_175960 = _self->_y_175960; /* 14313 */
  kk_drop_match(_self, {kk_box_dup(_y_175960);}, {}, _ctx)
  kk_std_core__list _x21069;
  kk_std_core__list _x21070 = kk_std_core__list_unbox(_b_19169, _ctx); /*list<14313>*/
  _x21069 = kk_std_core__mlift17746_op(_y_175960, _x21070, _ctx); /*list<14313>*/
  return kk_std_core__list_box(_x21069, _ctx);
}

kk_std_core__list kk_std_core__mlift17747_op(kk_function_t f, kk_integer_t i, kk_std_core__list yy, kk_box_t _y_175960, kk_context_t* _ctx) { /* forall<a,b,e> (f : (idx : int, value : a) -> e b, i : int, yy : list<a>, b) -> e list<b> */ 
  kk_integer_t i0_17305 = kk_integer_add(i,(kk_integer_from_small(1)),kk_context()); /*int*/;
  kk_std_core__list x_18048 = kk_std_core__lift17277_map_indexed(f, yy, i0_17305, _ctx); /*list<14313>*/;
  if (kk_yielding(kk_context())) {
    kk_std_core__list_drop(x_18048, _ctx);
    kk_box_t _x21067 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17747_op_fun21068(_y_175960, _ctx), _ctx); /*3860*/
    return kk_std_core__list_unbox(_x21067, _ctx);
  }
  {
    return kk_std_core__mlift17746_op(_y_175960, x_18048, _ctx);
  }
}
 
// lifted


// lift anonymous function
struct kk_std_core__lift17277_map_indexed_fun21075__t {
  struct kk_function_s _base;
  kk_function_t f0;
  kk_integer_t i0;
  kk_std_core__list yy0;
};
static kk_box_t kk_std_core__lift17277_map_indexed_fun21075(kk_function_t _fself, kk_box_t _b_19173, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17277_map_indexed_fun21075(kk_function_t f0, kk_integer_t i0, kk_std_core__list yy0, kk_context_t* _ctx) {
  struct kk_std_core__lift17277_map_indexed_fun21075__t* _self = kk_function_alloc_as(struct kk_std_core__lift17277_map_indexed_fun21075__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17277_map_indexed_fun21075, kk_context());
  _self->f0 = f0;
  _self->i0 = i0;
  _self->yy0 = yy0;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17277_map_indexed_fun21075(kk_function_t _fself, kk_box_t _b_19173, kk_context_t* _ctx) {
  struct kk_std_core__lift17277_map_indexed_fun21075__t* _self = kk_function_as(struct kk_std_core__lift17277_map_indexed_fun21075__t*, _fself);
  kk_function_t f0 = _self->f0; /* (idx : int, value : 14312) -> 14314 14313 */
  kk_integer_t i0 = _self->i0; /* int */
  kk_std_core__list yy0 = _self->yy0; /* list<14312> */
  kk_drop_match(_self, {kk_function_dup(f0);kk_integer_dup(i0);kk_std_core__list_dup(yy0);}, {}, _ctx)
  kk_std_core__list _x21076 = kk_std_core__mlift17747_op(f0, i0, yy0, _b_19173, _ctx); /*list<14313>*/
  return kk_std_core__list_box(_x21076, _ctx);
}


// lift anonymous function
struct kk_std_core__lift17277_map_indexed_fun21078__t {
  struct kk_function_s _base;
  kk_box_t x0_18050;
};
static kk_box_t kk_std_core__lift17277_map_indexed_fun21078(kk_function_t _fself, kk_box_t _b_19175, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17277_map_indexed_fun21078(kk_box_t x0_18050, kk_context_t* _ctx) {
  struct kk_std_core__lift17277_map_indexed_fun21078__t* _self = kk_function_alloc_as(struct kk_std_core__lift17277_map_indexed_fun21078__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17277_map_indexed_fun21078, kk_context());
  _self->x0_18050 = x0_18050;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17277_map_indexed_fun21078(kk_function_t _fself, kk_box_t _b_19175, kk_context_t* _ctx) {
  struct kk_std_core__lift17277_map_indexed_fun21078__t* _self = kk_function_as(struct kk_std_core__lift17277_map_indexed_fun21078__t*, _fself);
  kk_box_t x0_18050 = _self->x0_18050; /* 14313 */
  kk_drop_match(_self, {kk_box_dup(x0_18050);}, {}, _ctx)
  kk_std_core__list _x21079;
  kk_std_core__list _x21080 = kk_std_core__list_unbox(_b_19175, _ctx); /*list<14313>*/
  _x21079 = kk_std_core__mlift17746_op(x0_18050, _x21080, _ctx); /*list<14313>*/
  return kk_std_core__list_box(_x21079, _ctx);
}

kk_std_core__list kk_std_core__lift17277_map_indexed(kk_function_t f0, kk_std_core__list ys, kk_integer_t i0, kk_context_t* _ctx) { /* forall<a,b,e> (f : (idx : int, value : a) -> e b, ys : list<a>, i : int) -> e list<b> */ 
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con21071 = kk_std_core__as_Cons(ys);
    kk_box_t y = _con21071->head;
    kk_std_core__list yy0 = _con21071->tail;
    kk_reuse_t _ru_20002 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      _ru_20002 = (kk_std_core__list_reuse(ys));
    }
    else {
      kk_box_dup(y);
      kk_std_core__list_dup(yy0);
      kk_std_core__list_decref(ys, _ctx);
    }
    kk_box_t x0_18050;
    kk_function_t _x21073 = kk_function_dup(f0); /*(idx : int, value : 14312) -> 14314 14313*/
    kk_integer_t _x21072 = kk_integer_dup(i0); /*int*/
    x0_18050 = kk_function_call(kk_box_t, (kk_function_t, kk_integer_t, kk_box_t, kk_context_t*), _x21073, (_x21073, _x21072, y, _ctx)); /*14313*/
    if (kk_yielding(kk_context())) {
      kk_reuse_drop(_ru_20002);
      kk_box_drop(x0_18050, _ctx);
      kk_box_t _x21074 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17277_map_indexed_fun21075(f0, i0, yy0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x21074, _ctx);
    }
    {
      kk_integer_t i0_173050 = kk_integer_add(i0,(kk_integer_from_small(1)),kk_context()); /*int*/;
      kk_std_core__list x1_18053 = kk_std_core__lift17277_map_indexed(f0, yy0, i0_173050, _ctx); /*list<14313>*/;
      if (kk_yielding(kk_context())) {
        kk_reuse_drop(_ru_20002);
        kk_std_core__list_drop(x1_18053, _ctx);
        kk_box_t _x21077 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17277_map_indexed_fun21078(x0_18050, _ctx), _ctx); /*3860*/
        return kk_std_core__list_unbox(_x21077, _ctx);
      }
      {
        return kk_std_core__new_Cons(_ru_20002, x0_18050, x1_18053, _ctx);
      }
    }
  }
  {
    kk_integer_drop(i0, _ctx);
    kk_function_drop(f0, _ctx);
    return kk_std_core__new_Nil(_ctx);
  }
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17748_op(kk_box_t _y_17600, kk_std_core__list _y_17601, kk_context_t* _ctx) { /* forall<a,e> (a, list<a>) -> e list<a> */ 
  return kk_std_core__new_Cons(kk_reuse_null, _y_17600, _y_17601, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17749_op_fun21082__t {
  struct kk_function_s _base;
  kk_box_t _y_176000;
};
static kk_box_t kk_std_core__mlift17749_op_fun21082(kk_function_t _fself, kk_box_t _b_19181, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17749_op_fun21082(kk_box_t _y_176000, kk_context_t* _ctx) {
  struct kk_std_core__mlift17749_op_fun21082__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17749_op_fun21082__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17749_op_fun21082, kk_context());
  _self->_y_176000 = _y_176000;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17749_op_fun21082(kk_function_t _fself, kk_box_t _b_19181, kk_context_t* _ctx) {
  struct kk_std_core__mlift17749_op_fun21082__t* _self = kk_function_as(struct kk_std_core__mlift17749_op_fun21082__t*, _fself);
  kk_box_t _y_176000 = _self->_y_176000; /* 14370 */
  kk_drop_match(_self, {kk_box_dup(_y_176000);}, {}, _ctx)
  kk_std_core__list _x21083;
  kk_std_core__list _x21084 = kk_std_core__list_unbox(_b_19181, _ctx); /*list<14370>*/
  _x21083 = kk_std_core__mlift17748_op(_y_176000, _x21084, _ctx); /*list<14370>*/
  return kk_std_core__list_box(_x21083, _ctx);
}

kk_std_core__list kk_std_core__mlift17749_op(kk_function_t f, kk_integer_t i, kk_std_core__list yy, kk_box_t _y_176000, kk_context_t* _ctx) { /* forall<a,b,e> (f : (idx : int, value : a, rest : list<a>) -> e b, i : int, yy : list<a>, b) -> e list<b> */ 
  kk_integer_t i0_17307 = kk_integer_add(i,(kk_integer_from_small(1)),kk_context()); /*int*/;
  kk_std_core__list x_18056 = kk_std_core__lift17278_map_indexed_peek(f, yy, i0_17307, _ctx); /*list<14370>*/;
  if (kk_yielding(kk_context())) {
    kk_std_core__list_drop(x_18056, _ctx);
    kk_box_t _x21081 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17749_op_fun21082(_y_176000, _ctx), _ctx); /*3860*/
    return kk_std_core__list_unbox(_x21081, _ctx);
  }
  {
    return kk_std_core__mlift17748_op(_y_176000, x_18056, _ctx);
  }
}
 
// lifted


// lift anonymous function
struct kk_std_core__lift17278_map_indexed_peek_fun21090__t {
  struct kk_function_s _base;
  kk_function_t f0;
  kk_integer_t i0;
  kk_std_core__list yy0;
};
static kk_box_t kk_std_core__lift17278_map_indexed_peek_fun21090(kk_function_t _fself, kk_box_t _b_19185, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17278_map_indexed_peek_fun21090(kk_function_t f0, kk_integer_t i0, kk_std_core__list yy0, kk_context_t* _ctx) {
  struct kk_std_core__lift17278_map_indexed_peek_fun21090__t* _self = kk_function_alloc_as(struct kk_std_core__lift17278_map_indexed_peek_fun21090__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17278_map_indexed_peek_fun21090, kk_context());
  _self->f0 = f0;
  _self->i0 = i0;
  _self->yy0 = yy0;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17278_map_indexed_peek_fun21090(kk_function_t _fself, kk_box_t _b_19185, kk_context_t* _ctx) {
  struct kk_std_core__lift17278_map_indexed_peek_fun21090__t* _self = kk_function_as(struct kk_std_core__lift17278_map_indexed_peek_fun21090__t*, _fself);
  kk_function_t f0 = _self->f0; /* (idx : int, value : 14369, rest : list<14369>) -> 14371 14370 */
  kk_integer_t i0 = _self->i0; /* int */
  kk_std_core__list yy0 = _self->yy0; /* list<14369> */
  kk_drop_match(_self, {kk_function_dup(f0);kk_integer_dup(i0);kk_std_core__list_dup(yy0);}, {}, _ctx)
  kk_std_core__list _x21091 = kk_std_core__mlift17749_op(f0, i0, yy0, _b_19185, _ctx); /*list<14370>*/
  return kk_std_core__list_box(_x21091, _ctx);
}


// lift anonymous function
struct kk_std_core__lift17278_map_indexed_peek_fun21093__t {
  struct kk_function_s _base;
  kk_box_t x0_18058;
};
static kk_box_t kk_std_core__lift17278_map_indexed_peek_fun21093(kk_function_t _fself, kk_box_t _b_19187, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17278_map_indexed_peek_fun21093(kk_box_t x0_18058, kk_context_t* _ctx) {
  struct kk_std_core__lift17278_map_indexed_peek_fun21093__t* _self = kk_function_alloc_as(struct kk_std_core__lift17278_map_indexed_peek_fun21093__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17278_map_indexed_peek_fun21093, kk_context());
  _self->x0_18058 = x0_18058;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17278_map_indexed_peek_fun21093(kk_function_t _fself, kk_box_t _b_19187, kk_context_t* _ctx) {
  struct kk_std_core__lift17278_map_indexed_peek_fun21093__t* _self = kk_function_as(struct kk_std_core__lift17278_map_indexed_peek_fun21093__t*, _fself);
  kk_box_t x0_18058 = _self->x0_18058; /* 14370 */
  kk_drop_match(_self, {kk_box_dup(x0_18058);}, {}, _ctx)
  kk_std_core__list _x21094;
  kk_std_core__list _x21095 = kk_std_core__list_unbox(_b_19187, _ctx); /*list<14370>*/
  _x21094 = kk_std_core__mlift17748_op(x0_18058, _x21095, _ctx); /*list<14370>*/
  return kk_std_core__list_box(_x21094, _ctx);
}

kk_std_core__list kk_std_core__lift17278_map_indexed_peek(kk_function_t f0, kk_std_core__list ys, kk_integer_t i0, kk_context_t* _ctx) { /* forall<a,b,e> (f : (idx : int, value : a, rest : list<a>) -> e b, ys : list<a>, i : int) -> e list<b> */ 
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con21085 = kk_std_core__as_Cons(ys);
    kk_box_t y = _con21085->head;
    kk_std_core__list yy0 = _con21085->tail;
    kk_reuse_t _ru_20003 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      _ru_20003 = (kk_std_core__list_reuse(ys));
    }
    else {
      kk_box_dup(y);
      kk_std_core__list_dup(yy0);
      kk_std_core__list_decref(ys, _ctx);
    }
    kk_box_t x0_18058;
    kk_function_t _x21088 = kk_function_dup(f0); /*(idx : int, value : 14369, rest : list<14369>) -> 14371 14370*/
    kk_integer_t _x21086 = kk_integer_dup(i0); /*int*/
    kk_std_core__list _x21087 = kk_std_core__list_dup(yy0); /*list<14369>*/
    x0_18058 = kk_function_call(kk_box_t, (kk_function_t, kk_integer_t, kk_box_t, kk_std_core__list, kk_context_t*), _x21088, (_x21088, _x21086, y, _x21087, _ctx)); /*14370*/
    if (kk_yielding(kk_context())) {
      kk_reuse_drop(_ru_20003);
      kk_box_drop(x0_18058, _ctx);
      kk_box_t _x21089 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17278_map_indexed_peek_fun21090(f0, i0, yy0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x21089, _ctx);
    }
    {
      kk_integer_t i0_173070 = kk_integer_add(i0,(kk_integer_from_small(1)),kk_context()); /*int*/;
      kk_std_core__list x1_18061 = kk_std_core__lift17278_map_indexed_peek(f0, yy0, i0_173070, _ctx); /*list<14370>*/;
      if (kk_yielding(kk_context())) {
        kk_reuse_drop(_ru_20003);
        kk_std_core__list_drop(x1_18061, _ctx);
        kk_box_t _x21092 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17278_map_indexed_peek_fun21093(x0_18058, _ctx), _ctx); /*3860*/
        return kk_std_core__list_unbox(_x21092, _ctx);
      }
      {
        return kk_std_core__new_Cons(_ru_20003, x0_18058, x1_18061, _ctx);
      }
    }
  }
  {
    kk_integer_drop(i0, _ctx);
    kk_function_drop(f0, _ctx);
    return kk_std_core__new_Nil(_ctx);
  }
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17750_op(kk_std_core_types__ctail _acc, kk_function_t action, kk_std_core__list xx, kk_std_core_types__maybe _y_17604, kk_context_t* _ctx) { /* forall<a,b,e> (ctail<list<b>>, action : (a) -> e maybe<b>, xx : list<a>, maybe<b>) -> e list<b> */ 
  if (kk_std_core_types__is_Just(_y_17604)) {
    kk_box_t y = _y_17604._cons.Just.value;
    kk_std_core__list _ctail_17369 = kk_std_core__list_hole(); /*list<14412>*/;
    kk_std_core__list _ctail_17370 = kk_std_core__new_Cons(kk_reuse_null, y, _ctail_17369, _ctx); /*list<14412>*/;
    kk_std_core_types__ctail _x21096;
    kk_box_t* _b_19202_19197 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17370)->tail)); /*cfield<list<14412>>*/;
    _x21096 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17370, _ctx)),_b_19202_19197); /*ctail<0>*/
    return kk_std_core__ctail_map_while(xx, action, _x21096, _ctx);
  }
  {
    kk_std_core__list_drop(xx, _ctx);
    kk_function_drop(action, _ctx);
    kk_box_t _x21097 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x21097, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17751_op_fun21098__t {
  struct kk_function_s _base;
  kk_function_t _accm;
  kk_box_t y0;
};
static kk_std_core__list kk_std_core__mlift17751_op_fun21098(kk_function_t _fself, kk_std_core__list _ctail_17372, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17751_op_fun21098(kk_function_t _accm, kk_box_t y0, kk_context_t* _ctx) {
  struct kk_std_core__mlift17751_op_fun21098__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17751_op_fun21098__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17751_op_fun21098, kk_context());
  _self->_accm = _accm;
  _self->y0 = y0;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__mlift17751_op_fun21098(kk_function_t _fself, kk_std_core__list _ctail_17372, kk_context_t* _ctx) {
  struct kk_std_core__mlift17751_op_fun21098__t* _self = kk_function_as(struct kk_std_core__mlift17751_op_fun21098__t*, _fself);
  kk_function_t _accm = _self->_accm; /* (list<14412>) -> list<14412> */
  kk_box_t y0 = _self->y0; /* 14412 */
  kk_drop_match(_self, {kk_function_dup(_accm);kk_box_dup(y0);}, {}, _ctx)
  kk_std_core__list _x21099 = kk_std_core__new_Cons(kk_reuse_null, y0, _ctail_17372, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, _x21099, _ctx));
}

kk_std_core__list kk_std_core__mlift17751_op(kk_function_t _accm, kk_function_t action0, kk_std_core__list xx0, kk_std_core_types__maybe _y_17608, kk_context_t* _ctx) { /* forall<a,b,e> ((list<b>) -> list<b>, action : (a) -> e maybe<b>, xx : list<a>, maybe<b>) -> e list<b> */ 
  if (kk_std_core_types__is_Just(_y_17608)) {
    kk_box_t y0 = _y_17608._cons.Just.value;
    return kk_std_core__ctailm_map_while(xx0, action0, kk_std_core__new_mlift17751_op_fun21098(_accm, y0, _ctx), _ctx);
  }
  {
    kk_std_core__list_drop(xx0, _ctx);
    kk_function_drop(action0, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// Invoke `action` on each element of a list while `action` returns `Just`


// lift anonymous function
struct kk_std_core__ctail_map_while_fun21104__t {
  struct kk_function_s _base;
  kk_function_t action1;
  kk_std_core__list xx1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_map_while_fun21104(kk_function_t _fself, kk_box_t _b_19211, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_map_while_fun21104(kk_function_t action1, kk_std_core__list xx1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_map_while_fun21104__t* _self = kk_function_alloc_as(struct kk_std_core__ctail_map_while_fun21104__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_map_while_fun21104, kk_context());
  _self->action1 = action1;
  _self->xx1 = xx1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_map_while_fun21104(kk_function_t _fself, kk_box_t _b_19211, kk_context_t* _ctx) {
  struct kk_std_core__ctail_map_while_fun21104__t* _self = kk_function_as(struct kk_std_core__ctail_map_while_fun21104__t*, _fself);
  kk_function_t action1 = _self->action1; /* (14411) -> 14413 maybe<14412> */
  kk_std_core__list xx1 = _self->xx1; /* list<14411> */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<14412>> */
  kk_drop_match(_self, {kk_function_dup(action1);kk_std_core__list_dup(xx1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x21105;
  kk_std_core_types__maybe _x21106 = kk_std_core_types__maybe_unbox(_b_19211, _ctx); /*maybe<14412>*/
  _x21105 = kk_std_core__mlift17750_op(_acc0, action1, xx1, _x21106, _ctx); /*list<14412>*/
  return kk_std_core__list_box(_x21105, _ctx);
}

kk_std_core__list kk_std_core__ctail_map_while(kk_std_core__list xs, kk_function_t action1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, action : (a) -> e maybe<b>, ctail<list<b>>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Nil(xs)) {
    kk_function_drop(action1, _ctx);
    kk_box_t _x21100 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x21100, _ctx);
  }
  {
    struct kk_std_core_Cons* _con21101 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21101->head;
    kk_std_core__list xx1 = _con21101->tail;
    kk_reuse_t _ru_20004 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      _ru_20004 = (kk_std_core__list_reuse(xs));
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx1);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_std_core_types__maybe x0_18064;
    kk_function_t _x21102 = kk_function_dup(action1); /*(14411) -> 14413 maybe<14412>*/
    x0_18064 = kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_box_t, kk_context_t*), _x21102, (_x21102, x, _ctx)); /*maybe<14412>*/
    if (kk_yielding(kk_context())) {
      kk_reuse_drop(_ru_20004);
      kk_std_core_types__maybe_drop(x0_18064, _ctx);
      kk_box_t _x21103 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_map_while_fun21104(action1, xx1, _acc0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x21103, _ctx);
    }
    if (kk_std_core_types__is_Just(x0_18064)) {
      kk_box_t y1 = x0_18064._cons.Just.value;
      kk_std_core__list _ctail_173690 = kk_std_core__list_hole(); /*list<14412>*/;
      kk_std_core__list _ctail_173700 = kk_std_core__new_Cons(_ru_20004, y1, _ctail_173690, _ctx); /*list<14412>*/;
      { // tailcall
        kk_std_core_types__ctail _x21107;
        kk_box_t* _b_19225_19217 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_173700)->tail)); /*cfield<list<14412>>*/;
        _x21107 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_173700, _ctx)),_b_19225_19217); /*ctail<0>*/
        xs = xx1;
        _acc0 = _x21107;
        goto kk__tailcall;
      }
    }
    {
      kk_reuse_drop(_ru_20004);
      kk_std_core__list_drop(xx1, _ctx);
      kk_function_drop(action1, _ctx);
      kk_box_t _x21108 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
      return kk_std_core__list_unbox(_x21108, _ctx);
    }
  }
}
 
// Invoke `action` on each element of a list while `action` returns `Just`


// lift anonymous function
struct kk_std_core__ctailm_map_while_fun21112__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t action2;
  kk_std_core__list xx2;
};
static kk_box_t kk_std_core__ctailm_map_while_fun21112(kk_function_t _fself, kk_box_t _b_19233, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_map_while_fun21112(kk_function_t _accm0, kk_function_t action2, kk_std_core__list xx2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_map_while_fun21112__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_map_while_fun21112__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_map_while_fun21112, kk_context());
  _self->_accm0 = _accm0;
  _self->action2 = action2;
  _self->xx2 = xx2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_map_while_fun21112(kk_function_t _fself, kk_box_t _b_19233, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_map_while_fun21112__t* _self = kk_function_as(struct kk_std_core__ctailm_map_while_fun21112__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<14412>) -> list<14412> */
  kk_function_t action2 = _self->action2; /* (14411) -> 14413 maybe<14412> */
  kk_std_core__list xx2 = _self->xx2; /* list<14411> */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(action2);kk_std_core__list_dup(xx2);}, {}, _ctx)
  kk_std_core__list _x21113;
  kk_std_core_types__maybe _x21114 = kk_std_core_types__maybe_unbox(_b_19233, _ctx); /*maybe<14412>*/
  _x21113 = kk_std_core__mlift17751_op(_accm0, action2, xx2, _x21114, _ctx); /*list<14412>*/
  return kk_std_core__list_box(_x21113, _ctx);
}


// lift anonymous function
struct kk_std_core__ctailm_map_while_fun21116__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t y2;
};
static kk_std_core__list kk_std_core__ctailm_map_while_fun21116(kk_function_t _fself, kk_std_core__list _ctail_173720, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_map_while_fun21116(kk_function_t _accm0, kk_box_t y2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_map_while_fun21116__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_map_while_fun21116__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_map_while_fun21116, kk_context());
  _self->_accm0 = _accm0;
  _self->y2 = y2;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_map_while_fun21116(kk_function_t _fself, kk_std_core__list _ctail_173720, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_map_while_fun21116__t* _self = kk_function_as(struct kk_std_core__ctailm_map_while_fun21116__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<14412>) -> list<14412> */
  kk_box_t y2 = _self->y2; /* 14412 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(y2);}, {}, _ctx)
  kk_std_core__list _x21117 = kk_std_core__new_Cons(kk_reuse_null, y2, _ctail_173720, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x21117, _ctx));
}

kk_std_core__list kk_std_core__ctailm_map_while(kk_std_core__list xs0, kk_function_t action2, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, action : (a) -> e maybe<b>, (list<b>) -> list<b>) -> e list<b> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Nil(xs0)) {
    kk_function_drop(action2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
  {
    struct kk_std_core_Cons* _con21109 = kk_std_core__as_Cons(xs0);
    kk_box_t x1 = _con21109->head;
    kk_std_core__list xx2 = _con21109->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs0))) {
      kk_std_core__list_free(xs0);
    }
    else {
      kk_box_dup(x1);
      kk_std_core__list_dup(xx2);
      kk_std_core__list_decref(xs0, _ctx);
    }
    kk_std_core_types__maybe x2_18067;
    kk_function_t _x21110 = kk_function_dup(action2); /*(14411) -> 14413 maybe<14412>*/
    x2_18067 = kk_function_call(kk_std_core_types__maybe, (kk_function_t, kk_box_t, kk_context_t*), _x21110, (_x21110, x1, _ctx)); /*maybe<14412>*/
    if (kk_yielding(kk_context())) {
      kk_std_core_types__maybe_drop(x2_18067, _ctx);
      kk_box_t _x21111 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_map_while_fun21112(_accm0, action2, xx2, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x21111, _ctx);
    }
    if (kk_std_core_types__is_Just(x2_18067)) {
      kk_box_t y2 = x2_18067._cons.Just.value;
      { // tailcall
        kk_function_t _x21115 = kk_std_core__new_ctailm_map_while_fun21116(_accm0, y2, _ctx); /*(list<14412>) -> list<14412>*/
        xs0 = xx2;
        _accm0 = _x21115;
        goto kk__tailcall;
      }
    }
    {
      kk_std_core__list_drop(xx2, _ctx);
      kk_function_drop(action2, _ctx);
      return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
    }
  }
}
 
// Invoke `action` on each element of a list while `action` returns `Just`


// lift anonymous function
struct kk_std_core_map_while_fun21119__t {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core_map_while_fun21119(kk_function_t _fself, kk_std_core__list _ctail_17371, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_map_while_fun21119(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_map_while_fun21119, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core_map_while_fun21119(kk_function_t _fself, kk_std_core__list _ctail_17371, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17371;
}

kk_std_core__list kk_std_core_map_while(kk_std_core__list xs1, kk_function_t action3, kk_context_t* _ctx) { /* forall<a,b,e> (xs : list<a>, action : (a) -> e maybe<b>) -> e list<b> */ 
  bool _match_19614 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19614) {
    kk_std_core_types__ctail _x21118 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_map_while(xs1, action3, _x21118, _ctx);
  }
  {
    return kk_std_core__ctailm_map_while(xs1, action3, kk_std_core_new_map_while_fun21119(_ctx), _ctx);
  }
}
 
// Returns the largest element of a list of integers (or `default` (=`0`) for the empty list)


// lift anonymous function
struct kk_std_core_maximum_fun21124__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_maximum_fun21124(kk_function_t _fself, kk_box_t _b_19241, kk_box_t _b_19242, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_maximum_fun21124(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_maximum_fun21124, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_maximum_fun21124(kk_function_t _fself, kk_box_t _b_19241, kk_box_t _b_19242, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_integer_t _x21125;
  kk_integer_t _x21126 = kk_integer_unbox(_b_19241); /*int*/
  kk_integer_t _x21127 = kk_integer_unbox(_b_19242); /*int*/
  _x21125 = kk_std_core_max(_x21126, _x21127, _ctx); /*int*/
  return kk_integer_box(_x21125);
}

kk_integer_t kk_std_core_maximum(kk_std_core__list xs, kk_std_core_types__optional default0, kk_context_t* _ctx) { /* (xs : list<int>, default : optional<int>) -> int */ 
  if (kk_std_core__is_Nil(xs)) {
    if (kk_std_core_types__is_Optional(default0)) {
      kk_box_t _box_x19236 = default0._cons.Optional.value;
      kk_integer_t _default_14461 = kk_integer_unbox(_box_x19236);
      kk_integer_dup(_default_14461);
      kk_std_core_types__optional_drop(default0, _ctx);
      return _default_14461;
    }
    {
      return kk_integer_from_small(0);
    }
  }
  {
    struct kk_std_core_Cons* _con21121 = kk_std_core__as_Cons(xs);
    kk_box_t _box_x19237 = _con21121->head;
    kk_std_core__list xx = _con21121->tail;
    kk_integer_t x = kk_integer_unbox(_box_x19237);
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_integer_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_std_core_types__optional_drop(default0, _ctx);
    kk_box_t _x21123 = kk_std_core_foldl(xx, kk_integer_box(x), kk_std_core_new_maximum_fun21124(_ctx), _ctx); /*12420*/
    return kk_integer_unbox(_x21123);
  }
}
 
// Returns the largest element of a list of doubles (or `0` for the empty list)


// lift anonymous function
struct kk_std_core_maximum_fun21131__t_1 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_maximum_fun21131_1(kk_function_t _fself, kk_box_t _b_19250, kk_box_t _b_19251, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_maximum_fun21131_1(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_maximum_fun21131_1, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_maximum_fun21131_1(kk_function_t _fself, kk_box_t _b_19250, kk_box_t _b_19251, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  double _x21132;
  double _x21133 = kk_double_unbox(_b_19250, _ctx); /*double*/
  double _x21134 = kk_double_unbox(_b_19251, _ctx); /*double*/
  _x21132 = kk_std_core_max_1(_x21133, _x21134, _ctx); /*double*/
  return kk_double_box(_x21132, _ctx);
}

double kk_std_core_maximum_1(kk_std_core__list xs, kk_context_t* _ctx) { /* (xs : list<double>) -> double */ 
  if (kk_std_core__is_Nil(xs)) {
    return 0.0;
  }
  {
    struct kk_std_core_Cons* _con21128 = kk_std_core__as_Cons(xs);
    kk_box_t _box_x19246 = _con21128->head;
    kk_std_core__list xx = _con21128->tail;
    double x = kk_double_unbox(_box_x19246, NULL);
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_box_drop(_box_x19246, _ctx);
      kk_std_core__list_free(xs);
    }
    else {
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_box_t _x21130 = kk_std_core_foldl(xx, kk_double_box(x, _ctx), kk_std_core_new_maximum_fun21131_1(_ctx), _ctx); /*12420*/
    return kk_double_unbox(_x21130, _ctx);
  }
}
 
// Convert a list to a `:maybe` type, using `Nothing` for an empty list, and otherwise `Just` on the head element.
// Note: this is just `head`.

kk_std_core_types__maybe kk_std_core_maybe_3(kk_std_core__list xs, kk_context_t* _ctx) { /* forall<a> (xs : list<a>) -> maybe<a> */ 
  if (kk_std_core__is_Nil(xs)) {
    return kk_std_core_types__new_Nothing(_ctx);
  }
  {
    struct kk_std_core_Cons* _con21135 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21135->head;
    kk_std_core__list _pat1 = _con21135->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_drop(_pat1, _ctx);
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_decref(xs, _ctx);
    }
    return kk_std_core_types__new_Just(x, _ctx);
  }
}
 
// Transform an integer to a maybe type, using `Nothing` for `0`

kk_std_core_types__maybe kk_std_core_maybe_5(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> maybe<int> */ 
  bool _match_19609;
  kk_integer_t _brw_19610 = kk_integer_from_small(0); /*int*/;
  bool _brw_19611 = kk_integer_eq_borrow(i,_brw_19610,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19610, _ctx);
  _match_19609 = _brw_19611; /*bool*/
  if (_match_19609) {
    kk_integer_drop(i, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
  {
    return kk_std_core_types__new_Just(kk_integer_box(i), _ctx);
  }
}
 
// Transform a string to a maybe type, using `Nothing` for an empty string

kk_std_core_types__maybe kk_std_core_maybe_6(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> maybe<string> */ 
  bool _match_19608;
  kk_string_t _x21136 = kk_string_dup(s); /*string*/
  kk_string_t _x21137 = kk_string_empty(); /*string*/
  _match_19608 = kk_string_is_eq(_x21136,_x21137,kk_context()); /*bool*/
  if (_match_19608) {
    kk_string_drop(s, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
  {
    return kk_std_core_types__new_Just(kk_string_box(s), _ctx);
  }
}
 
// Transform a `:null` type to a `:maybe` type. Note that it is not
// always the case that `id(x) == maybe(null(x))` (e.g. when `x = Just(Nothing)`).

kk_std_core_types__maybe kk_std_core_maybe_8(kk_std_core__null n, kk_context_t* _ctx) { /* forall<a> (n : null<a>) -> maybe<a> */ 
  return (kk_datatype_as_ptr(n) == NULL ? kk_std_core_types__new_Nothing(kk_context()) : kk_std_core_types__new_Just(kk_datatype_box(n),kk_context()));
}
 
// Returns the smallest element of a list of integers (or `default` (=`0`) for the empty list)


// lift anonymous function
struct kk_std_core_minimum_fun21143__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_minimum_fun21143(kk_function_t _fself, kk_box_t _b_19266, kk_box_t _b_19267, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_minimum_fun21143(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_minimum_fun21143, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_minimum_fun21143(kk_function_t _fself, kk_box_t _b_19266, kk_box_t _b_19267, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_integer_t _x21144;
  kk_integer_t _x21145 = kk_integer_unbox(_b_19266); /*int*/
  kk_integer_t _x21146 = kk_integer_unbox(_b_19267); /*int*/
  _x21144 = kk_std_core_min(_x21145, _x21146, _ctx); /*int*/
  return kk_integer_box(_x21144);
}

kk_integer_t kk_std_core_minimum(kk_std_core__list xs, kk_std_core_types__optional default0, kk_context_t* _ctx) { /* (xs : list<int>, default : optional<int>) -> int */ 
  if (kk_std_core__is_Nil(xs)) {
    if (kk_std_core_types__is_Optional(default0)) {
      kk_box_t _box_x19261 = default0._cons.Optional.value;
      kk_integer_t _default_14940 = kk_integer_unbox(_box_x19261);
      kk_integer_dup(_default_14940);
      kk_std_core_types__optional_drop(default0, _ctx);
      return _default_14940;
    }
    {
      return kk_integer_from_small(0);
    }
  }
  {
    struct kk_std_core_Cons* _con21140 = kk_std_core__as_Cons(xs);
    kk_box_t _box_x19262 = _con21140->head;
    kk_std_core__list xx = _con21140->tail;
    kk_integer_t x = kk_integer_unbox(_box_x19262);
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_integer_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_std_core_types__optional_drop(default0, _ctx);
    kk_box_t _x21142 = kk_std_core_foldl(xx, kk_integer_box(x), kk_std_core_new_minimum_fun21143(_ctx), _ctx); /*12420*/
    return kk_integer_unbox(_x21142);
  }
}
 
// Returns the smallest element of a list of doubles (or `0` for the empty list)


// lift anonymous function
struct kk_std_core_minimum_fun21150__t_1 {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_minimum_fun21150_1(kk_function_t _fself, kk_box_t _b_19275, kk_box_t _b_19276, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_minimum_fun21150_1(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_minimum_fun21150_1, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_minimum_fun21150_1(kk_function_t _fself, kk_box_t _b_19275, kk_box_t _b_19276, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  double _x21151;
  double _x21152 = kk_double_unbox(_b_19275, _ctx); /*double*/
  double _x21153 = kk_double_unbox(_b_19276, _ctx); /*double*/
  _x21151 = kk_std_core_min_1(_x21152, _x21153, _ctx); /*double*/
  return kk_double_box(_x21151, _ctx);
}

double kk_std_core_minimum_1(kk_std_core__list xs, kk_context_t* _ctx) { /* (xs : list<double>) -> double */ 
  if (kk_std_core__is_Nil(xs)) {
    return 0.0;
  }
  {
    struct kk_std_core_Cons* _con21147 = kk_std_core__as_Cons(xs);
    kk_box_t _box_x19271 = _con21147->head;
    kk_std_core__list xx = _con21147->tail;
    double x = kk_double_unbox(_box_x19271, NULL);
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_box_drop(_box_x19271, _ctx);
      kk_std_core__list_free(xs);
    }
    else {
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    kk_box_t _x21149 = kk_std_core_foldl(xx, kk_double_box(x, _ctx), kk_std_core_new_minimum_fun21150_1(_ctx), _ctx); /*12420*/
    return kk_double_unbox(_x21149, _ctx);
  }
}
 
// Disable tracing completely.

kk_unit_t kk_std_core_notrace(kk_context_t* _ctx) { /* () -> (st<global>) () */ 
  kk_ref_set_borrow(kk_std_core_trace_enabled,(kk_bool_box(false)),kk_context()); return kk_Unit;
}
 
// Transform a `:maybe` type to a `:null` type (using `null` for `Nothing`).

kk_std_core__null kk_std_core_null(kk_std_core_types__maybe x, kk_context_t* _ctx) { /* forall<a> (x : maybe<a>) -> null<a> */ 
  return (kk_std_core_types__is_Nothing(x) ? kk_datatype_from_ptr(NULL) : kk_datatype_unbox((x)._cons.Just.value));
}
 
// Cast a integer that is zero to a null

kk_std_core__null kk_std_core_null_1(kk_integer_t i, kk_context_t* _ctx) { /* (i : int) -> null<int> */ 
  kk_std_core_types__maybe _x21154;
  bool _match_19603;
  kk_integer_t _brw_19604 = kk_integer_from_small(0); /*int*/;
  bool _brw_19605 = kk_integer_eq_borrow(i,_brw_19604,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19604, _ctx);
  _match_19603 = _brw_19605; /*bool*/
  if (_match_19603) {
    kk_integer_drop(i, _ctx);
    _x21154 = kk_std_core_types__new_Nothing(_ctx); /*forall<a> maybe<a>*/
  }
  else {
    _x21154 = kk_std_core_types__new_Just(kk_integer_box(i), _ctx); /*forall<a> maybe<a>*/
  }
  return kk_std_core_null(_x21154, _ctx);
}
 
// Cast an empty string a null

kk_std_core__null kk_std_core_null_2(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> null<string> */ 
  kk_std_core_types__maybe _x21155;
  bool _match_19602;
  kk_string_t _x21156 = kk_string_dup(s); /*string*/
  kk_string_t _x21157 = kk_string_empty(); /*string*/
  _match_19602 = kk_string_is_eq(_x21156,_x21157,kk_context()); /*bool*/
  if (_match_19602) {
    kk_string_drop(s, _ctx);
    _x21155 = kk_std_core_types__new_Nothing(_ctx); /*forall<a> maybe<a>*/
  }
  else {
    _x21155 = kk_std_core_types__new_Just(kk_string_box(s), _ctx); /*forall<a> maybe<a>*/
  }
  return kk_std_core_null(_x21155, _ctx);
}
 
// Left-align a string to width `width`  using `fill`  (default is a space) to fill on the right.

kk_string_t kk_std_core_pad_right(kk_string_t s, kk_integer_t width, kk_std_core_types__optional fill, kk_context_t* _ctx) { /* (s : string, width : int, fill : optional<char>) -> string */ 
  kk_ssize_t w;
  kk_integer_t _x21160 = kk_integer_dup(width); /*int*/
  w = kk_std_core_ssize__t(_x21160, _ctx); /*ssize_t*/
  kk_ssize_t n;
  kk_string_t _x21161 = kk_string_dup(s); /*string*/
  n = kk_string_len(_x21161,kk_context()); /*ssize_t*/
  bool _match_19601 = (w <= n); /*bool*/;
  if (_match_19601) {
    kk_std_core_types__optional_drop(fill, _ctx);
    return s;
  }
  {
    kk_string_t _x21162;
    kk_string_t _x21163;
    kk_char_t _x21164;
    if (kk_std_core_types__is_Optional(fill)) {
      kk_box_t _box_x19290 = fill._cons.Optional.value;
      kk_char_t _fill_15342 = kk_char_unbox(_box_x19290, NULL);
      kk_std_core_types__optional_drop(fill, _ctx);
      _x21164 = _fill_15342; /*char*/
      goto _match21165;
    }
    {
      _x21164 = ' '; /*char*/
    }
    _match21165: ;
    _x21163 = kk_std_core_string(_x21164, _ctx); /*string*/
    kk_ssize_t _x21167 = (w - n); /*ssize_t*/
    _x21162 = kk_std_core_repeatz(_x21163, _x21167, _ctx); /*string*/
    return kk_std_core__lp__plus__plus__1_rp_(s, _x21162, _ctx);
  }
}
 
// Is `pre`  a prefix of `s`? If so, returns a slice
// of `s` following `pre` up to the end of `s`.

kk_std_core_types__maybe kk_std_core_starts_with(kk_string_t s, kk_string_t pre, kk_context_t* _ctx) { /* (s : string, pre : string) -> maybe<sslice> */ 
  bool _match_19600;
  kk_string_t _x21168 = kk_string_dup(s); /*string*/
  kk_string_t _x21169 = kk_string_dup(pre); /*string*/
  _match_19600 = kk_string_starts_with(_x21168,_x21169,kk_context()); /*bool*/
  if (_match_19600) {
    kk_std_core__sslice _b_19292_19291;
    kk_string_t _x21170 = kk_string_dup(s); /*string*/
    kk_ssize_t _x21171;
    kk_string_t _x21172 = kk_string_dup(pre); /*string*/
    _x21171 = kk_string_len(_x21172,kk_context()); /*ssize_t*/
    kk_ssize_t _x21173;
    kk_ssize_t _x21174 = kk_string_len(s,kk_context()); /*ssize_t*/
    kk_ssize_t _x21175 = kk_string_len(pre,kk_context()); /*ssize_t*/
    _x21173 = (_x21174 - _x21175); /*ssize_t*/
    _b_19292_19291 = kk_std_core__new_Sslice(_x21170, _x21171, _x21173, _ctx); /*sslice*/
    return kk_std_core_types__new_Just(kk_std_core__sslice_box(_b_19292_19291, _ctx), _ctx);
  }
  {
    kk_string_drop(s, _ctx);
    kk_string_drop(pre, _ctx);
    return kk_std_core_types__new_Nothing(_ctx);
  }
}
 
// Trim off a substring `sub` while `s` starts with that string.

kk_string_t kk_std_core_trim_left_1(kk_string_t s, kk_string_t sub, kk_context_t* _ctx) { /* (s : string, sub : string) -> string */ 
  kk__tailcall: ;
  bool _match_19598;
  kk_string_t _x21176 = kk_string_dup(sub); /*string*/
  kk_string_t _x21177 = kk_string_empty(); /*string*/
  _match_19598 = kk_string_is_eq(_x21176,_x21177,kk_context()); /*bool*/
  if (_match_19598) {
    kk_string_drop(sub, _ctx);
    return s;
  }
  {
    kk_std_core_types__maybe _match_19599;
    kk_string_t _x21179 = kk_string_dup(s); /*string*/
    kk_string_t _x21180 = kk_string_dup(sub); /*string*/
    _match_19599 = kk_std_core_starts_with(_x21179, _x21180, _ctx); /*maybe<sslice>*/
    if (kk_std_core_types__is_Just(_match_19599)) {
      kk_box_t _box_x19293 = _match_19599._cons.Just.value;
      kk_std_core__sslice slice0 = kk_std_core__sslice_unbox(_box_x19293, NULL);
      kk_string_drop(s, _ctx);
      kk_std_core__sslice_dup(slice0);
      kk_std_core_types__maybe_drop(_match_19599, _ctx);
      { // tailcall
        kk_string_t _x21182 = kk_std_core_string_3(slice0, _ctx); /*string*/
        s = _x21182;
        goto kk__tailcall;
      }
    }
    {
      kk_string_drop(sub, _ctx);
      return s;
    }
  }
}
 
// Trim off a substring `sub` while `s` ends with that string.

kk_string_t kk_std_core_trim_right_1(kk_string_t s, kk_string_t sub, kk_context_t* _ctx) { /* (s : string, sub : string) -> string */ 
  kk__tailcall: ;
  bool _match_19596;
  kk_string_t _x21183 = kk_string_dup(sub); /*string*/
  kk_string_t _x21184 = kk_string_empty(); /*string*/
  _match_19596 = kk_string_is_eq(_x21183,_x21184,kk_context()); /*bool*/
  if (_match_19596) {
    kk_string_drop(sub, _ctx);
    return s;
  }
  {
    kk_std_core_types__maybe _match_19597;
    kk_string_t _x21186 = kk_string_dup(s); /*string*/
    kk_string_t _x21187 = kk_string_dup(sub); /*string*/
    _match_19597 = kk_std_core_ends_with(_x21186, _x21187, _ctx); /*maybe<sslice>*/
    if (kk_std_core_types__is_Just(_match_19597)) {
      kk_box_t _box_x19294 = _match_19597._cons.Just.value;
      kk_std_core__sslice slice0 = kk_std_core__sslice_unbox(_box_x19294, NULL);
      kk_string_drop(s, _ctx);
      kk_std_core__sslice_dup(slice0);
      kk_std_core_types__maybe_drop(_match_19597, _ctx);
      { // tailcall
        kk_string_t _x21189 = kk_std_core_string_3(slice0, _ctx); /*string*/
        s = _x21189;
        goto kk__tailcall;
      }
    }
    {
      kk_string_drop(sub, _ctx);
      return s;
    }
  }
}
 
// Parse an integer using `parseInt`. If an illegal digit character is encountered the
// `default` value is returned. An empty string will also result in `default`.

kk_integer_t kk_std_core_parse_int_default(kk_string_t s, kk_std_core_types__optional default0, kk_std_core_types__optional hex, kk_context_t* _ctx) { /* (s : string, default : optional<int>, hex : optional<bool>) -> int */ 
  bool _match_19595;
  kk_string_t _x21196 = kk_string_dup(s); /*string*/
  kk_string_t _x21197 = kk_string_empty(); /*string*/
  _match_19595 = kk_string_is_eq(_x21196,_x21197,kk_context()); /*bool*/
  if (_match_19595) {
    kk_string_drop(s, _ctx);
    kk_std_core_types__optional_drop(hex, _ctx);
    if (kk_std_core_types__is_Optional(default0)) {
      kk_box_t _box_x19296 = default0._cons.Optional.value;
      kk_integer_t _default_15624 = kk_integer_unbox(_box_x19296);
      kk_integer_dup(_default_15624);
      kk_std_core_types__optional_drop(default0, _ctx);
      return _default_15624;
    }
    {
      return kk_integer_from_small(0);
    }
  }
  {
    kk_std_core_types__maybe m_17214;
    kk_string_t _x21200;
    kk_string_t _x21201 = kk_string_trim_left(s,kk_context()); /*string*/
    _x21200 = kk_string_trim_right(_x21201,kk_context()); /*string*/
    bool _x21202;
    if (kk_std_core_types__is_Optional(hex)) {
      kk_box_t _box_x19297 = hex._cons.Optional.value;
      bool _hex_15628 = kk_bool_unbox(_box_x19297);
      kk_std_core_types__optional_drop(hex, _ctx);
      _x21202 = _hex_15628; /*bool*/
      goto _match21203;
    }
    {
      _x21202 = false; /*bool*/
    }
    _match21203: ;
    m_17214 = kk_std_core_xparse_int(_x21200, _x21202, _ctx); /*maybe<int>*/
    if (kk_std_core_types__is_Nothing(m_17214)) {
      if (kk_std_core_types__is_Optional(default0)) {
        kk_box_t _box_x19298 = default0._cons.Optional.value;
        kk_integer_t _default_156240 = kk_integer_unbox(_box_x19298);
        kk_integer_dup(_default_156240);
        kk_std_core_types__optional_drop(default0, _ctx);
        return _default_156240;
      }
      {
        return kk_integer_from_small(0);
      }
    }
    {
      kk_box_t _box_x19299 = m_17214._cons.Just.value;
      kk_integer_t x = kk_integer_unbox(_box_x19299);
      kk_integer_dup(x);
      kk_std_core_types__maybe_drop(m_17214, _ctx);
      kk_std_core_types__optional_drop(default0, _ctx);
      return x;
    }
  }
}
 
// monadic lift

kk_std_core_types__tuple2_ kk_std_core__mlift17752_partition_acc(kk_std_core__list acc1, kk_std_core__list acc2, kk_function_t pred, kk_box_t x, kk_std_core__list xx, bool _y_17617, kk_context_t* _ctx) { /* forall<a,e> (acc1 : list<a>, acc2 : list<a>, pred : (a) -> e bool, x : a, xx : list<a>, bool) -> e (list<a>, list<a>) */ 
  if (_y_17617) {
    kk_std_core__list _x21207 = kk_std_core__new_Cons(kk_reuse_null, x, acc1, _ctx); /*list<61>*/
    return kk_std_core_partition_acc(xx, pred, _x21207, acc2, _ctx);
  }
  {
    kk_std_core__list _x21208 = kk_std_core__new_Cons(kk_reuse_null, x, acc2, _ctx); /*list<61>*/
    return kk_std_core_partition_acc(xx, pred, acc1, _x21208, _ctx);
  }
}


// lift anonymous function
struct kk_std_core_partition_acc_fun21213__t {
  struct kk_function_s _base;
  kk_std_core__list acc10;
  kk_std_core__list acc20;
  kk_function_t pred0;
  kk_box_t x0;
  kk_std_core__list xx0;
};
static kk_box_t kk_std_core_partition_acc_fun21213(kk_function_t _fself, kk_box_t _b_19303, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_partition_acc_fun21213(kk_std_core__list acc10, kk_std_core__list acc20, kk_function_t pred0, kk_box_t x0, kk_std_core__list xx0, kk_context_t* _ctx) {
  struct kk_std_core_partition_acc_fun21213__t* _self = kk_function_alloc_as(struct kk_std_core_partition_acc_fun21213__t, 6, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_partition_acc_fun21213, kk_context());
  _self->acc10 = acc10;
  _self->acc20 = acc20;
  _self->pred0 = pred0;
  _self->x0 = x0;
  _self->xx0 = xx0;
  return &_self->_base;
}

static kk_box_t kk_std_core_partition_acc_fun21213(kk_function_t _fself, kk_box_t _b_19303, kk_context_t* _ctx) {
  struct kk_std_core_partition_acc_fun21213__t* _self = kk_function_as(struct kk_std_core_partition_acc_fun21213__t*, _fself);
  kk_std_core__list acc10 = _self->acc10; /* list<15746> */
  kk_std_core__list acc20 = _self->acc20; /* list<15746> */
  kk_function_t pred0 = _self->pred0; /* (15746) -> 15747 bool */
  kk_box_t x0 = _self->x0; /* 15746 */
  kk_std_core__list xx0 = _self->xx0; /* list<15746> */
  kk_drop_match(_self, {kk_std_core__list_dup(acc10);kk_std_core__list_dup(acc20);kk_function_dup(pred0);kk_box_dup(x0);kk_std_core__list_dup(xx0);}, {}, _ctx)
  kk_std_core_types__tuple2_ _x21214;
  bool _x21215 = kk_bool_unbox(_b_19303); /*bool*/
  _x21214 = kk_std_core__mlift17752_partition_acc(acc10, acc20, pred0, x0, xx0, _x21215, _ctx); /*(list<15746>, list<15746>)*/
  return kk_std_core_types__tuple2__box(_x21214, _ctx);
}

kk_std_core_types__tuple2_ kk_std_core_partition_acc(kk_std_core__list xs, kk_function_t pred0, kk_std_core__list acc10, kk_std_core__list acc20, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, pred : (a) -> e bool, acc1 : list<a>, acc2 : list<a>) -> e (list<a>, list<a>) */ 
  kk__tailcall: ;
  if (kk_std_core__is_Nil(xs)) {
    kk_function_drop(pred0, _ctx);
    kk_std_core__list _b_19304_19300 = kk_std_core__lift17274_reverse(kk_std_core__new_Nil(_ctx), acc10, _ctx); /*list<15746>*/;
    kk_std_core__list _b_19305_19301 = kk_std_core__lift17274_reverse(kk_std_core__new_Nil(_ctx), acc20, _ctx); /*list<15746>*/;
    return kk_std_core_types__new_dash__lp__comma__rp_(kk_std_core__list_box(_b_19304_19300, _ctx), kk_std_core__list_box(_b_19305_19301, _ctx), _ctx);
  }
  {
    struct kk_std_core_Cons* _con21209 = kk_std_core__as_Cons(xs);
    kk_box_t x0 = _con21209->head;
    kk_std_core__list xx0 = _con21209->tail;
    kk_reuse_t _ru_20011 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      _ru_20011 = (kk_std_core__list_reuse(xs));
    }
    else {
      kk_box_dup(x0);
      kk_std_core__list_dup(xx0);
      kk_std_core__list_decref(xs, _ctx);
    }
    bool x1_18072;
    kk_function_t _x21211 = kk_function_dup(pred0); /*(15746) -> 15747 bool*/
    kk_box_t _x21210 = kk_box_dup(x0); /*15746*/
    x1_18072 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), _x21211, (_x21211, _x21210, _ctx)); /*bool*/
    if (kk_yielding(kk_context())) {
      kk_reuse_drop(_ru_20011);
      kk_box_t _x21212 = kk_std_core_hnd_yield_extend(kk_std_core_new_partition_acc_fun21213(acc10, acc20, pred0, x0, xx0, _ctx), _ctx); /*3860*/
      return kk_std_core_types__tuple2__unbox(_x21212, _ctx);
    }
    if (x1_18072) { // tailcall
                    kk_std_core__list _x21216 = kk_std_core__new_Cons(_ru_20011, x0, acc10, _ctx); /*list<61>*/
                    xs = xx0;
                    acc10 = _x21216;
                    goto kk__tailcall;
    }
    { // tailcall
      kk_std_core__list _x21217 = kk_std_core__new_Cons(_ru_20011, x0, acc20, _ctx); /*list<61>*/
      xs = xx0;
      acc20 = _x21217;
      goto kk__tailcall;
    }
  }
}
 
// redirect `print` and `println` calls to a specified function.


// lift anonymous function
struct kk_std_core_print_redirect_fun21220__t {
  struct kk_function_s _base;
  kk_function_t print0;
};
static kk_box_t kk_std_core_print_redirect_fun21220(kk_function_t _fself, kk_box_t _b_19311, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_print_redirect_fun21220(kk_function_t print0, kk_context_t* _ctx) {
  struct kk_std_core_print_redirect_fun21220__t* _self = kk_function_alloc_as(struct kk_std_core_print_redirect_fun21220__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_print_redirect_fun21220, kk_context());
  _self->print0 = print0;
  return &_self->_base;
}

static kk_box_t kk_std_core_print_redirect_fun21220(kk_function_t _fself, kk_box_t _b_19311, kk_context_t* _ctx) {
  struct kk_std_core_print_redirect_fun21220__t* _self = kk_function_as(struct kk_std_core_print_redirect_fun21220__t*, _fself);
  kk_function_t print0 = _self->print0; /* (msg : string) -> console () */
  kk_drop_match(_self, {kk_function_dup(print0);}, {}, _ctx)
  kk_unit_t _x21221 = kk_Unit;
  kk_string_t _x21222 = kk_string_unbox(_b_19311); /*string*/
  kk_function_call(kk_unit_t, (kk_function_t, kk_string_t, kk_context_t*), print0, (print0, _x21222, _ctx));
  return kk_unit_box(_x21221);
}

kk_unit_t kk_std_core_print_redirect(kk_function_t print0, kk_context_t* _ctx) { /* (print : (msg : string) -> console ()) -> io () */ 
  kk_box_t _x21218;
  kk_std_core_types__maybe _x21219 = kk_std_core_types__new_Just(kk_function_box(kk_std_core_new_print_redirect_fun21220(print0, _ctx)), _ctx); /*maybe<105>*/
  _x21218 = kk_std_core_types__maybe_box(_x21219, _ctx); /*170*/
  kk_ref_set_borrow(kk_std_core_redirect,_x21218,kk_context()); return kk_Unit;
}
extern bool kk_std_core_remove_fun21223(kk_function_t _fself, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_remove_fun21223__t* _self = kk_function_as(struct kk_std_core_remove_fun21223__t*, _fself);
  kk_function_t pred = _self->pred; /* (15815) -> bool */
  kk_drop_match(_self, {kk_function_dup(pred);}, {}, _ctx)
  bool b_17221 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), pred, (pred, x, _ctx)); /*bool*/;
  if (b_17221) {
    return false;
  }
  {
    return true;
  }
}
extern kk_unit_t kk_std_core_repeat_fun21226_1(kk_function_t _fself, kk_integer_t i, kk_context_t* _ctx) {
  struct kk_std_core_repeat_fun21226__t_1* _self = kk_function_as(struct kk_std_core_repeat_fun21226__t_1*, _fself);
  kk_function_t action = _self->action; /* () -> 15834 () */
  kk_drop_match(_self, {kk_function_dup(action);}, {}, _ctx)
  kk_integer_drop(i, _ctx);
  return kk_function_call(kk_unit_t, (kk_function_t, kk_context_t*), action, (action, _ctx));
}
 
// Create a list of `n`  repeated elementes `x`

kk_std_core__list kk_std_core__ctail_replicate(kk_box_t x, kk_integer_t n, kk_std_core_types__ctail _acc, kk_context_t* _ctx) { /* forall<a> (x : a, n : int, ctail<list<a>>) -> list<a> */ 
  kk__tailcall: ;
  bool _match_19591;
  kk_integer_t _brw_19592 = kk_integer_from_small(0); /*int*/;
  bool _brw_19593 = kk_integer_gt_borrow(n,_brw_19592,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19592, _ctx);
  _match_19591 = _brw_19593; /*bool*/
  if (_match_19591) {
    kk_std_core__list _ctail_17373 = kk_std_core__list_hole(); /*list<15878>*/;
    kk_std_core__list _ctail_17374;
    kk_box_t _x21228 = kk_box_dup(x); /*15878*/
    _ctail_17374 = kk_std_core__new_Cons(kk_reuse_null, _x21228, _ctail_17373, _ctx); /*list<15878>*/
    { // tailcall
      kk_integer_t _x21229 = kk_integer_sub(n,(kk_integer_from_small(1)),kk_context()); /*int*/
      kk_std_core_types__ctail _x21230;
      kk_box_t* _b_19327_19322 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17374)->tail)); /*cfield<list<15878>>*/;
      _x21230 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17374, _ctx)),_b_19327_19322); /*ctail<0>*/
      n = _x21229;
      _acc = _x21230;
      goto kk__tailcall;
    }
  }
  {
    kk_box_drop(x, _ctx);
    kk_integer_drop(n, _ctx);
    kk_box_t _x21231 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x21231, _ctx);
  }
}
 
// Create a list of `n`  repeated elementes `x`

kk_std_core__list kk_std_core_replicate(kk_box_t x0, kk_integer_t n0, kk_context_t* _ctx) { /* forall<a> (x : a, n : int) -> list<a> */ 
  kk_std_core_types__ctail _x21232 = kk_ctail_nil(); /*ctail<0>*/
  return kk_std_core__ctail_replicate(x0, n0, _x21232, _ctx);
}
 
// lifted

kk_std_core__list kk_std_core__lift17279_reverse_join(kk_std_core__list acc, kk_std_core__list ys, kk_context_t* _ctx) { /* forall<a> (acc : list<a>, ys : list<a>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con21233 = kk_std_core__as_Cons(ys);
    kk_box_t x = _con21233->head;
    kk_std_core__list xx = _con21233->tail;
    kk_reuse_t _ru_20012 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      _ru_20012 = (kk_std_core__list_reuse(ys));
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(ys, _ctx);
    }
    { // tailcall
      kk_std_core__list _x21234 = kk_std_core__new_Cons(_ru_20012, x, acc, _ctx); /*list<61>*/
      acc = _x21234;
      ys = xx;
      goto kk__tailcall;
    }
  }
  {
    return acc;
  }
}
 
// lifted

kk_string_t kk_std_core__lift17280_reverse_join(kk_std_core__list ys0, kk_string_t acc0, kk_context_t* _ctx) { /* (ys0 : list<string>, acc0 : string) -> string */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys0)) {
    struct kk_std_core_Cons* _con21235 = kk_std_core__as_Cons(ys0);
    kk_box_t _box_x19333 = _con21235->head;
    kk_std_core__list yy = _con21235->tail;
    kk_string_t y = kk_string_unbox(_box_x19333);
    if (kk_likely(kk_std_core__list_is_unique(ys0))) {
      kk_std_core__list_free(ys0);
    }
    else {
      kk_string_dup(y);
      kk_std_core__list_dup(yy);
      kk_std_core__list_decref(ys0, _ctx);
    }
    { // tailcall
      kk_string_t _x21237;
      kk_string_t _x21238;
      kk_string_t _x21239 = kk_string_empty(); /*string*/
      _x21238 = kk_std_core__lp__plus__plus__1_rp_(_x21239, y, _ctx); /*string*/
      _x21237 = kk_std_core__lp__plus__plus__1_rp_(acc0, _x21238, _ctx); /*string*/
      ys0 = yy;
      acc0 = _x21237;
      goto kk__tailcall;
    }
  }
  {
    return acc0;
  }
}
 
// Concatenate all strings in a list in reverse order

kk_string_t kk_std_core_reverse_join(kk_std_core__list xs, kk_context_t* _ctx) { /* (xs : list<string>) -> string */ 
  kk_std_core__list xs0_17223 = kk_std_core__lift17279_reverse_join(kk_std_core__new_Nil(_ctx), xs, _ctx); /*list<string>*/;
  if (kk_std_core__is_Nil(xs0_17223)) {
    return kk_string_empty();
  }
  {
    struct kk_std_core_Cons* _con21242 = kk_std_core__as_Cons(xs0_17223);
    kk_box_t _box_x19334 = _con21242->head;
    kk_std_core__list xx0 = _con21242->tail;
    kk_string_t x0 = kk_string_unbox(_box_x19334);
    if (kk_likely(kk_std_core__list_is_unique(xs0_17223))) {
      kk_std_core__list_free(xs0_17223);
    }
    else {
      kk_string_dup(x0);
      kk_std_core__list_dup(xx0);
      kk_std_core__list_decref(xs0_17223, _ctx);
    }
    return kk_std_core__lift17280_reverse_join(xx0, x0, _ctx);
  }
}

kk_string_t kk_std_core_show_tuple(kk_std_core_types__tuple2_ x, kk_function_t showfst, kk_function_t showsnd, kk_context_t* _ctx) { /* forall<a,b> (x : (a, b), showfst : (a) -> string, showsnd : (b) -> string) -> string */ 
  kk_string_t _x21244;
  kk_define_string_literal(, _s21245, 1, "(")
  _x21244 = kk_string_dup(_s21245); /*string*/
  kk_string_t _x21246;
  kk_string_t _x21247;
  kk_box_t _x21248;
  {
    kk_box_t _x = x.fst;
    kk_box_dup(_x);
    _x21248 = _x; /*16198*/
  }
  _x21247 = kk_function_call(kk_string_t, (kk_function_t, kk_box_t, kk_context_t*), showfst, (showfst, _x21248, _ctx)); /*string*/
  kk_string_t _x21249;
  kk_string_t _x21250;
  kk_define_string_literal(, _s21251, 1, ",")
  _x21250 = kk_string_dup(_s21251); /*string*/
  kk_string_t _x21252;
  kk_string_t _x21253;
  kk_box_t _x21254;
  {
    kk_box_t _x0 = x.snd;
    kk_box_dup(_x0);
    kk_std_core_types__tuple2__drop(x, _ctx);
    _x21254 = _x0; /*16199*/
  }
  _x21253 = kk_function_call(kk_string_t, (kk_function_t, kk_box_t, kk_context_t*), showsnd, (showsnd, _x21254, _ctx)); /*string*/
  kk_string_t _x21255;
  kk_define_string_literal(, _s21256, 1, ")")
  _x21255 = kk_string_dup(_s21256); /*string*/
  _x21252 = kk_std_core__lp__plus__plus__1_rp_(_x21253, _x21255, _ctx); /*string*/
  _x21249 = kk_std_core__lp__plus__plus__1_rp_(_x21250, _x21252, _ctx); /*string*/
  _x21246 = kk_std_core__lp__plus__plus__1_rp_(_x21247, _x21249, _ctx); /*string*/
  return kk_std_core__lp__plus__plus__1_rp_(_x21244, _x21246, _ctx);
}
 
// monadic lift

kk_std_core_types__tuple2_ kk_std_core__mlift17753_op(kk_std_core__list acc, kk_function_t predicate, kk_box_t y, kk_std_core__list ys, kk_std_core__list yy, bool _y_17625, kk_context_t* _ctx) { /* forall<a,e> (acc : list<a>, predicate : (a) -> e bool, y : a, ys : list<a>, yy : list<a>, bool) -> e (list<a>, list<a>) */ 
  if (_y_17625) {
    kk_std_core__list_drop(ys, _ctx);
    kk_std_core__list _x21257 = kk_std_core__new_Cons(kk_reuse_null, y, acc, _ctx); /*list<61>*/
    return kk_std_core__lift17281_span(predicate, yy, _x21257, _ctx);
  }
  {
    kk_std_core__list_drop(yy, _ctx);
    kk_box_drop(y, _ctx);
    kk_function_drop(predicate, _ctx);
    kk_std_core__list _b_19337_19335 = kk_std_core__lift17274_reverse(kk_std_core__new_Nil(_ctx), acc, _ctx); /*list<16257>*/;
    return kk_std_core_types__new_dash__lp__comma__rp_(kk_std_core__list_box(_b_19337_19335, _ctx), kk_std_core__list_box(ys, _ctx), _ctx);
  }
}
 
// lifted
// todo: implement TRMC with multiple results to avoid the reverse


// lift anonymous function
struct kk_std_core__lift17281_span_fun21262__t {
  struct kk_function_s _base;
  kk_std_core__list acc0;
  kk_function_t predicate0;
  kk_box_t y0;
  kk_std_core__list ys0;
  kk_std_core__list yy0;
};
static kk_box_t kk_std_core__lift17281_span_fun21262(kk_function_t _fself, kk_box_t _b_19340, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17281_span_fun21262(kk_std_core__list acc0, kk_function_t predicate0, kk_box_t y0, kk_std_core__list ys0, kk_std_core__list yy0, kk_context_t* _ctx) {
  struct kk_std_core__lift17281_span_fun21262__t* _self = kk_function_alloc_as(struct kk_std_core__lift17281_span_fun21262__t, 6, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17281_span_fun21262, kk_context());
  _self->acc0 = acc0;
  _self->predicate0 = predicate0;
  _self->y0 = y0;
  _self->ys0 = ys0;
  _self->yy0 = yy0;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17281_span_fun21262(kk_function_t _fself, kk_box_t _b_19340, kk_context_t* _ctx) {
  struct kk_std_core__lift17281_span_fun21262__t* _self = kk_function_as(struct kk_std_core__lift17281_span_fun21262__t*, _fself);
  kk_std_core__list acc0 = _self->acc0; /* list<16257> */
  kk_function_t predicate0 = _self->predicate0; /* (16257) -> 16258 bool */
  kk_box_t y0 = _self->y0; /* 16257 */
  kk_std_core__list ys0 = _self->ys0; /* list<16257> */
  kk_std_core__list yy0 = _self->yy0; /* list<16257> */
  kk_drop_match(_self, {kk_std_core__list_dup(acc0);kk_function_dup(predicate0);kk_box_dup(y0);kk_std_core__list_dup(ys0);kk_std_core__list_dup(yy0);}, {}, _ctx)
  kk_std_core_types__tuple2_ _x21263;
  bool _x21264 = kk_bool_unbox(_b_19340); /*bool*/
  _x21263 = kk_std_core__mlift17753_op(acc0, predicate0, y0, ys0, yy0, _x21264, _ctx); /*(list<16257>, list<16257>)*/
  return kk_std_core_types__tuple2__box(_x21263, _ctx);
}

kk_std_core_types__tuple2_ kk_std_core__lift17281_span(kk_function_t predicate0, kk_std_core__list ys0, kk_std_core__list acc0, kk_context_t* _ctx) { /* forall<a,e> (predicate : (a) -> e bool, ys : list<a>, acc : list<a>) -> e (list<a>, list<a>) */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys0)) {
    struct kk_std_core_Cons* _con21258 = kk_std_core__as_Cons(ys0);
    kk_box_t y0 = _con21258->head;
    kk_std_core__list yy0 = _con21258->tail;
    kk_box_dup(y0);
    kk_std_core__list_dup(yy0);
    bool x_18079;
    kk_function_t _x21260 = kk_function_dup(predicate0); /*(16257) -> 16258 bool*/
    kk_box_t _x21259 = kk_box_dup(y0); /*16257*/
    x_18079 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), _x21260, (_x21260, _x21259, _ctx)); /*bool*/
    if (kk_yielding(kk_context())) {
      kk_box_t _x21261 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17281_span_fun21262(acc0, predicate0, y0, ys0, yy0, _ctx), _ctx); /*3860*/
      return kk_std_core_types__tuple2__unbox(_x21261, _ctx);
    }
    if (x_18079) {
      kk_reuse_t _ru_20015 = kk_reuse_null; /*reuse*/;
      if (kk_likely(kk_std_core__list_is_unique(ys0))) {
        kk_std_core__list_drop(yy0, _ctx);
        kk_box_drop(y0, _ctx);
        _ru_20015 = (kk_std_core__list_reuse(ys0));
      }
      else {
        kk_std_core__list_decref(ys0, _ctx);
      }
      { // tailcall
        kk_std_core__list _x21265 = kk_std_core__new_Cons(_ru_20015, y0, acc0, _ctx); /*list<61>*/
        ys0 = yy0;
        acc0 = _x21265;
        goto kk__tailcall;
      }
    }
    {
      kk_std_core__list_drop(yy0, _ctx);
      kk_box_drop(y0, _ctx);
      kk_function_drop(predicate0, _ctx);
      kk_std_core__list _b_19346_19341 = kk_std_core__lift17274_reverse(kk_std_core__new_Nil(_ctx), acc0, _ctx); /*list<16257>*/;
      return kk_std_core_types__new_dash__lp__comma__rp_(kk_std_core__list_box(_b_19346_19341, _ctx), kk_std_core__list_box(ys0, _ctx), _ctx);
    }
  }
  {
    kk_function_drop(predicate0, _ctx);
    kk_std_core__list _b_19348_19343 = kk_std_core__lift17274_reverse(kk_std_core__new_Nil(_ctx), acc0, _ctx); /*list<16257>*/;
    return kk_std_core_types__new_dash__lp__comma__rp_(kk_std_core__list_box(_b_19348_19343, _ctx), kk_std_core__list_box(ys0, _ctx), _ctx);
  }
}
 
// Return the sum of a list of integers


// lift anonymous function
struct kk_std_core_sum_fun21267__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_sum_fun21267(kk_function_t _fself, kk_box_t _b_19354, kk_box_t _b_19355, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_sum_fun21267(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_sum_fun21267, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_sum_fun21267(kk_function_t _fself, kk_box_t _b_19354, kk_box_t _b_19355, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_integer_t _x21268;
  kk_integer_t _x21269 = kk_integer_unbox(_b_19354); /*int*/
  kk_integer_t _x21270 = kk_integer_unbox(_b_19355); /*int*/
  _x21268 = kk_integer_add(_x21269,_x21270,kk_context()); /*int*/
  return kk_integer_box(_x21268);
}

kk_integer_t kk_std_core_sum(kk_std_core__list xs, kk_context_t* _ctx) { /* (xs : list<int>) -> int */ 
  kk_box_t _x21266 = kk_std_core_foldl(xs, kk_integer_box(kk_integer_from_small(0)), kk_std_core_new_sum_fun21267(_ctx), _ctx); /*12420*/
  return kk_integer_unbox(_x21266);
}
 
// Return the tail of list. Returns the empty list if `xs` is empty.

kk_std_core__list kk_std_core_tail_1(kk_std_core__list xs, kk_context_t* _ctx) { /* forall<a> (xs : list<a>) -> list<a> */ 
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21271 = kk_std_core__as_Cons(xs);
    kk_box_t _pat0 = _con21271->head;
    kk_std_core__list xx = _con21271->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_box_drop(_pat0, _ctx);
      kk_std_core__list_free(xs);
    }
    else {
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    return xx;
  }
  {
    return kk_std_core__new_Nil(_ctx);
  }
}
 
// Return the tail of a string (or the empty string)

kk_string_t kk_std_core_tail_2(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> string */ 
  kk_std_core__sslice _x21272;
  kk_std_core__sslice slice1 = kk_std_core_first1(s, _ctx); /*sslice*/;
  kk_std_core__sslice slice0_17227;
  bool _match_19582;
  kk_integer_t _brw_19587;
  kk_std_core_types__optional _match_19588 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
  if (kk_std_core_types__is_Optional(_match_19588)) {
    kk_box_t _box_x19361 = _match_19588._cons.Optional.value;
    kk_integer_t _n_10165 = kk_integer_unbox(_box_x19361);
    kk_integer_dup(_n_10165);
    kk_std_core_types__optional_drop(_match_19588, _ctx);
    _brw_19587 = _n_10165; /*int*/
    goto _match21273;
  }
  {
    _brw_19587 = kk_integer_from_small(1); /*int*/
  }
  _match21273: ;
  kk_integer_t _brw_19586 = kk_integer_from_small(1); /*int*/;
  bool _brw_19589 = kk_integer_eq_borrow(_brw_19587,_brw_19586,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19587, _ctx);
  kk_integer_drop(_brw_19586, _ctx);
  _match_19582 = _brw_19589; /*bool*/
  if (_match_19582) {
    slice0_17227 = slice1; /*sslice*/
  }
  else {
    kk_integer_t _brw_19583;
    kk_integer_t _x21275;
    kk_std_core_types__optional _match_19584 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
    if (kk_std_core_types__is_Optional(_match_19584)) {
      kk_box_t _box_x19362 = _match_19584._cons.Optional.value;
      kk_integer_t _n_101650 = kk_integer_unbox(_box_x19362);
      kk_integer_dup(_n_101650);
      kk_std_core_types__optional_drop(_match_19584, _ctx);
      _x21275 = _n_101650; /*int*/
      goto _match21276;
    }
    {
      _x21275 = kk_integer_from_small(1); /*int*/
    }
    _match21276: ;
    _brw_19583 = kk_integer_sub(_x21275,(kk_integer_from_small(1)),kk_context()); /*int*/
    kk_std_core__sslice _brw_19585 = kk_std_core_extend(slice1, _brw_19583, _ctx); /*sslice*/;
    kk_integer_drop(_brw_19583, _ctx);
    slice0_17227 = _brw_19585; /*sslice*/
  }
  {
    kk_string_t s0 = slice0_17227.str;
    kk_ssize_t start0 = slice0_17227.start;
    kk_ssize_t len0 = slice0_17227.len;
    kk_string_dup(s0);
    kk_std_core__sslice_drop(slice0_17227, _ctx);
    kk_string_t _x21278 = kk_string_dup(s0); /*string*/
    kk_ssize_t _x21279 = (start0 + len0); /*ssize_t*/
    kk_ssize_t _x21280;
    kk_ssize_t _x21281 = kk_string_len(s0,kk_context()); /*ssize_t*/
    kk_ssize_t _x21282 = (start0 + len0); /*ssize_t*/
    _x21280 = (_x21281 - _x21282); /*ssize_t*/
    _x21272 = kk_std_core__new_Sslice(_x21278, _x21279, _x21280, _ctx); /*sslice*/
  }
  return kk_std_core_string_3(_x21272, _ctx);
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17754_op(kk_std_core_types__ctail _acc, kk_function_t predicate, kk_box_t x, kk_std_core__list xx, bool _y_17630, kk_context_t* _ctx) { /* forall<a,e> (ctail<list<a>>, predicate : (a) -> e bool, x : a, xx : list<a>, bool) -> e list<a> */ 
  if (_y_17630) {
    kk_std_core__list _ctail_17375 = kk_std_core__list_hole(); /*list<16368>*/;
    kk_std_core__list _ctail_17376 = kk_std_core__new_Cons(kk_reuse_null, x, _ctail_17375, _ctx); /*list<16368>*/;
    kk_std_core_types__ctail _x21283;
    kk_box_t* _b_19373_19368 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17376)->tail)); /*cfield<list<16368>>*/;
    _x21283 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17376, _ctx)),_b_19373_19368); /*ctail<0>*/
    return kk_std_core__ctail_take_while(xx, predicate, _x21283, _ctx);
  }
  {
    kk_std_core__list_drop(xx, _ctx);
    kk_box_drop(x, _ctx);
    kk_function_drop(predicate, _ctx);
    kk_box_t _x21284 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x21284, _ctx);
  }
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17755_op_fun21285__t {
  struct kk_function_s _base;
  kk_function_t _accm;
  kk_box_t x0;
};
static kk_std_core__list kk_std_core__mlift17755_op_fun21285(kk_function_t _fself, kk_std_core__list _ctail_17378, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17755_op_fun21285(kk_function_t _accm, kk_box_t x0, kk_context_t* _ctx) {
  struct kk_std_core__mlift17755_op_fun21285__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17755_op_fun21285__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17755_op_fun21285, kk_context());
  _self->_accm = _accm;
  _self->x0 = x0;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__mlift17755_op_fun21285(kk_function_t _fself, kk_std_core__list _ctail_17378, kk_context_t* _ctx) {
  struct kk_std_core__mlift17755_op_fun21285__t* _self = kk_function_as(struct kk_std_core__mlift17755_op_fun21285__t*, _fself);
  kk_function_t _accm = _self->_accm; /* (list<16368>) -> list<16368> */
  kk_box_t x0 = _self->x0; /* 16368 */
  kk_drop_match(_self, {kk_function_dup(_accm);kk_box_dup(x0);}, {}, _ctx)
  kk_std_core__list _x21286 = kk_std_core__new_Cons(kk_reuse_null, x0, _ctail_17378, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, _x21286, _ctx));
}

kk_std_core__list kk_std_core__mlift17755_op(kk_function_t _accm, kk_function_t predicate0, kk_box_t x0, kk_std_core__list xx0, bool _y_17634, kk_context_t* _ctx) { /* forall<a,e> ((list<a>) -> list<a>, predicate : (a) -> e bool, x : a, xx : list<a>, bool) -> e list<a> */ 
  if (_y_17634) {
    return kk_std_core__ctailm_take_while(xx0, predicate0, kk_std_core__new_mlift17755_op_fun21285(_accm, x0, _ctx), _ctx);
  }
  {
    kk_std_core__list_drop(xx0, _ctx);
    kk_box_drop(x0, _ctx);
    kk_function_drop(predicate0, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// Keep only those initial elements that satisfy `predicate`


// lift anonymous function
struct kk_std_core__ctail_take_while_fun21291__t {
  struct kk_function_s _base;
  kk_function_t predicate1;
  kk_box_t x1;
  kk_std_core__list xx1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_take_while_fun21291(kk_function_t _fself, kk_box_t _b_19380, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_take_while_fun21291(kk_function_t predicate1, kk_box_t x1, kk_std_core__list xx1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_take_while_fun21291__t* _self = kk_function_alloc_as(struct kk_std_core__ctail_take_while_fun21291__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_take_while_fun21291, kk_context());
  _self->predicate1 = predicate1;
  _self->x1 = x1;
  _self->xx1 = xx1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_take_while_fun21291(kk_function_t _fself, kk_box_t _b_19380, kk_context_t* _ctx) {
  struct kk_std_core__ctail_take_while_fun21291__t* _self = kk_function_as(struct kk_std_core__ctail_take_while_fun21291__t*, _fself);
  kk_function_t predicate1 = _self->predicate1; /* (16368) -> 16369 bool */
  kk_box_t x1 = _self->x1; /* 16368 */
  kk_std_core__list xx1 = _self->xx1; /* list<16368> */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<16368>> */
  kk_drop_match(_self, {kk_function_dup(predicate1);kk_box_dup(x1);kk_std_core__list_dup(xx1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x21292;
  bool _x21293 = kk_bool_unbox(_b_19380); /*bool*/
  _x21292 = kk_std_core__mlift17754_op(_acc0, predicate1, x1, xx1, _x21293, _ctx); /*list<16368>*/
  return kk_std_core__list_box(_x21292, _ctx);
}

kk_std_core__list kk_std_core__ctail_take_while(kk_std_core__list xs, kk_function_t predicate1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, predicate : (a) -> e bool, ctail<list<a>>) -> e list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21287 = kk_std_core__as_Cons(xs);
    kk_box_t x1 = _con21287->head;
    kk_std_core__list xx1 = _con21287->tail;
    kk_reuse_t _ru_20017 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      _ru_20017 = (kk_std_core__list_reuse(xs));
    }
    else {
      kk_box_dup(x1);
      kk_std_core__list_dup(xx1);
      kk_std_core__list_decref(xs, _ctx);
    }
    bool x2_18084;
    kk_function_t _x21289 = kk_function_dup(predicate1); /*(16368) -> 16369 bool*/
    kk_box_t _x21288 = kk_box_dup(x1); /*16368*/
    x2_18084 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), _x21289, (_x21289, _x21288, _ctx)); /*bool*/
    if (kk_yielding(kk_context())) {
      kk_reuse_drop(_ru_20017);
      kk_box_t _x21290 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_take_while_fun21291(predicate1, x1, xx1, _acc0, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x21290, _ctx);
    }
    if (x2_18084) {
      kk_std_core__list _ctail_173750 = kk_std_core__list_hole(); /*list<16368>*/;
      kk_std_core__list _ctail_173760 = kk_std_core__new_Cons(_ru_20017, x1, _ctail_173750, _ctx); /*list<16368>*/;
      { // tailcall
        kk_std_core_types__ctail _x21294;
        kk_box_t* _b_19394_19386 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_173760)->tail)); /*cfield<list<16368>>*/;
        _x21294 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_173760, _ctx)),_b_19394_19386); /*ctail<0>*/
        xs = xx1;
        _acc0 = _x21294;
        goto kk__tailcall;
      }
    }
    {
      kk_reuse_drop(_ru_20017);
      kk_std_core__list_drop(xx1, _ctx);
      kk_box_drop(x1, _ctx);
      kk_function_drop(predicate1, _ctx);
      kk_box_t _x21295 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
      return kk_std_core__list_unbox(_x21295, _ctx);
    }
  }
  {
    kk_function_drop(predicate1, _ctx);
    kk_box_t _x21296 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x21296, _ctx);
  }
}
 
// Keep only those initial elements that satisfy `predicate`


// lift anonymous function
struct kk_std_core__ctailm_take_while_fun21301__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t predicate2;
  kk_box_t x3;
  kk_std_core__list xx2;
};
static kk_box_t kk_std_core__ctailm_take_while_fun21301(kk_function_t _fself, kk_box_t _b_19404, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_take_while_fun21301(kk_function_t _accm0, kk_function_t predicate2, kk_box_t x3, kk_std_core__list xx2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_take_while_fun21301__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_take_while_fun21301__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_take_while_fun21301, kk_context());
  _self->_accm0 = _accm0;
  _self->predicate2 = predicate2;
  _self->x3 = x3;
  _self->xx2 = xx2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_take_while_fun21301(kk_function_t _fself, kk_box_t _b_19404, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_take_while_fun21301__t* _self = kk_function_as(struct kk_std_core__ctailm_take_while_fun21301__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<16368>) -> list<16368> */
  kk_function_t predicate2 = _self->predicate2; /* (16368) -> 16369 bool */
  kk_box_t x3 = _self->x3; /* 16368 */
  kk_std_core__list xx2 = _self->xx2; /* list<16368> */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(predicate2);kk_box_dup(x3);kk_std_core__list_dup(xx2);}, {}, _ctx)
  kk_std_core__list _x21302;
  bool _x21303 = kk_bool_unbox(_b_19404); /*bool*/
  _x21302 = kk_std_core__mlift17755_op(_accm0, predicate2, x3, xx2, _x21303, _ctx); /*list<16368>*/
  return kk_std_core__list_box(_x21302, _ctx);
}


// lift anonymous function
struct kk_std_core__ctailm_take_while_fun21305__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t x3;
};
static kk_std_core__list kk_std_core__ctailm_take_while_fun21305(kk_function_t _fself, kk_std_core__list _ctail_173780, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_take_while_fun21305(kk_function_t _accm0, kk_box_t x3, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_take_while_fun21305__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_take_while_fun21305__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_take_while_fun21305, kk_context());
  _self->_accm0 = _accm0;
  _self->x3 = x3;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_take_while_fun21305(kk_function_t _fself, kk_std_core__list _ctail_173780, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_take_while_fun21305__t* _self = kk_function_as(struct kk_std_core__ctailm_take_while_fun21305__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<16368>) -> list<16368> */
  kk_box_t x3 = _self->x3; /* 16368 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(x3);}, {}, _ctx)
  kk_std_core__list _x21306 = kk_std_core__new_Cons(kk_reuse_null, x3, _ctail_173780, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x21306, _ctx));
}

kk_std_core__list kk_std_core__ctailm_take_while(kk_std_core__list xs0, kk_function_t predicate2, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, predicate : (a) -> e bool, (list<a>) -> list<a>) -> e list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs0)) {
    struct kk_std_core_Cons* _con21297 = kk_std_core__as_Cons(xs0);
    kk_box_t x3 = _con21297->head;
    kk_std_core__list xx2 = _con21297->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs0))) {
      kk_std_core__list_free(xs0);
    }
    else {
      kk_box_dup(x3);
      kk_std_core__list_dup(xx2);
      kk_std_core__list_decref(xs0, _ctx);
    }
    bool x4_18087;
    kk_function_t _x21299 = kk_function_dup(predicate2); /*(16368) -> 16369 bool*/
    kk_box_t _x21298 = kk_box_dup(x3); /*16368*/
    x4_18087 = kk_function_call(bool, (kk_function_t, kk_box_t, kk_context_t*), _x21299, (_x21299, _x21298, _ctx)); /*bool*/
    if (kk_yielding(kk_context())) {
      kk_box_t _x21300 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_take_while_fun21301(_accm0, predicate2, x3, xx2, _ctx), _ctx); /*3860*/
      return kk_std_core__list_unbox(_x21300, _ctx);
    }
    if (x4_18087) { // tailcall
                    kk_function_t _x21304 = kk_std_core__new_ctailm_take_while_fun21305(_accm0, x3, _ctx); /*(list<16368>) -> list<16368>*/
                    xs0 = xx2;
                    _accm0 = _x21304;
                    goto kk__tailcall;
    }
    {
      kk_std_core__list_drop(xx2, _ctx);
      kk_box_drop(x3, _ctx);
      kk_function_drop(predicate2, _ctx);
      return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
    }
  }
  {
    kk_function_drop(predicate2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// Keep only those initial elements that satisfy `predicate`


// lift anonymous function
struct kk_std_core_take_while_fun21308__t {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core_take_while_fun21308(kk_function_t _fself, kk_std_core__list _ctail_17377, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_take_while_fun21308(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_take_while_fun21308, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core_take_while_fun21308(kk_function_t _fself, kk_std_core__list _ctail_17377, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17377;
}

kk_std_core__list kk_std_core_take_while(kk_std_core__list xs1, kk_function_t predicate3, kk_context_t* _ctx) { /* forall<a,e> (xs : list<a>, predicate : (a) -> e bool) -> e list<a> */ 
  bool _match_19579 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19579) {
    kk_std_core_types__ctail _x21307 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_take_while(xs1, predicate3, _x21307, _ctx);
  }
  {
    return kk_std_core__ctailm_take_while(xs1, predicate3, kk_std_core_new_take_while_fun21308(_ctx), _ctx);
  }
}
 
// Trace a message used for debug purposes.
// The behaviour is system dependent. On a browser and node it uses
// `console.log`  by default.
// Disabled if `notrace` is called.


// lift anonymous function
struct kk_std_core_trace_fun21312__t {
  struct kk_function_s _base;
  kk_string_t message0;
};
static kk_box_t kk_std_core_trace_fun21312(kk_function_t _fself, kk_box_t _b_19410, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_trace_fun21312(kk_string_t message0, kk_context_t* _ctx) {
  struct kk_std_core_trace_fun21312__t* _self = kk_function_alloc_as(struct kk_std_core_trace_fun21312__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_trace_fun21312, kk_context());
  _self->message0 = message0;
  return &_self->_base;
}

static kk_box_t kk_std_core_trace_fun21312(kk_function_t _fself, kk_box_t _b_19410, kk_context_t* _ctx) {
  struct kk_std_core_trace_fun21312__t* _self = kk_function_as(struct kk_std_core_trace_fun21312__t*, _fself);
  kk_string_t message0 = _self->message0; /* string */
  kk_drop_match(_self, {kk_string_dup(message0);}, {}, _ctx)
  kk_unit_t _x21313 = kk_Unit;
  bool _y_19412_17641 = kk_bool_unbox(_b_19410); /*bool*/;
  if (_y_19412_17641) {
    kk_std_core_xtrace(message0, _ctx);
  }
  else {
    kk_string_drop(message0, _ctx);
  }
  return kk_unit_box(_x21313);
}

kk_unit_t kk_std_core_trace(kk_string_t message0, kk_context_t* _ctx) { /* (message : string) -> () */ 
  bool x_18090;
  kk_box_t _x21309;
  kk_ref_t _x21310 = kk_ref_dup(kk_std_core_trace_enabled); /*ref<global,bool>*/
  _x21309 = kk_ref_get(_x21310,kk_context()); /*184*/
  x_18090 = kk_bool_unbox(_x21309); /*bool*/
  if (kk_yielding(kk_context())) {
    kk_box_t _x21311 = kk_std_core_hnd_yield_extend(kk_std_core_new_trace_fun21312(message0, _ctx), _ctx); /*3860*/
    kk_unit_unbox(_x21311); return kk_Unit;
  }
  if (x_18090) {
    kk_std_core_xtrace(message0, _ctx); return kk_Unit;
  }
  {
    kk_string_drop(message0, _ctx);
    kk_Unit; return kk_Unit;
  }
}


// lift anonymous function
struct kk_std_core_trace_any_fun21317__t {
  struct kk_function_s _base;
  kk_string_t message0;
  kk_box_t x;
};
static kk_box_t kk_std_core_trace_any_fun21317(kk_function_t _fself, kk_box_t _b_19417, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_trace_any_fun21317(kk_string_t message0, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_trace_any_fun21317__t* _self = kk_function_alloc_as(struct kk_std_core_trace_any_fun21317__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_trace_any_fun21317, kk_context());
  _self->message0 = message0;
  _self->x = x;
  return &_self->_base;
}

static kk_box_t kk_std_core_trace_any_fun21317(kk_function_t _fself, kk_box_t _b_19417, kk_context_t* _ctx) {
  struct kk_std_core_trace_any_fun21317__t* _self = kk_function_as(struct kk_std_core_trace_any_fun21317__t*, _fself);
  kk_string_t message0 = _self->message0; /* string */
  kk_box_t x = _self->x; /* 16477 */
  kk_drop_match(_self, {kk_string_dup(message0);kk_box_dup(x);}, {}, _ctx)
  kk_unit_t _x21318 = kk_Unit;
  bool _y_19419_17643 = kk_bool_unbox(_b_19417); /*bool*/;
  if (_y_19419_17643) {
    kk_std_core_xtrace_any(message0, x, _ctx);
  }
  else {
    kk_box_drop(x, _ctx);
    kk_string_drop(message0, _ctx);
  }
  return kk_unit_box(_x21318);
}

kk_unit_t kk_std_core_trace_any(kk_string_t message0, kk_box_t x, kk_context_t* _ctx) { /* forall<a> (message : string, x : a) -> () */ 
  bool x0_18094;
  kk_box_t _x21314;
  kk_ref_t _x21315 = kk_ref_dup(kk_std_core_trace_enabled); /*ref<global,bool>*/
  _x21314 = kk_ref_get(_x21315,kk_context()); /*184*/
  x0_18094 = kk_bool_unbox(_x21314); /*bool*/
  if (kk_yielding(kk_context())) {
    kk_box_t _x21316 = kk_std_core_hnd_yield_extend(kk_std_core_new_trace_any_fun21317(message0, x, _ctx), _ctx); /*3860*/
    kk_unit_unbox(_x21316); return kk_Unit;
  }
  if (x0_18094) {
    kk_std_core_xtrace_any(message0, x, _ctx); return kk_Unit;
  }
  {
    kk_box_drop(x, _ctx);
    kk_string_drop(message0, _ctx);
    kk_Unit; return kk_Unit;
  }
}
 
// Truncate a string to `count` characters.

kk_string_t kk_std_core_truncate(kk_string_t s, kk_integer_t count, kk_context_t* _ctx) { /* (s : string, count : int) -> string */ 
  kk_std_core__sslice _x21319;
  kk_std_core__sslice _own_19567;
  kk_std_core__sslice slice0 = kk_std_core_first1(s, _ctx); /*sslice*/;
  bool _match_19568;
  kk_integer_t _brw_19573;
  kk_std_core_types__optional _match_19574 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
  if (kk_std_core_types__is_Optional(_match_19574)) {
    kk_box_t _box_x19421 = _match_19574._cons.Optional.value;
    kk_integer_t _n_10165 = kk_integer_unbox(_box_x19421);
    kk_integer_dup(_n_10165);
    kk_std_core_types__optional_drop(_match_19574, _ctx);
    _brw_19573 = _n_10165; /*int*/
    goto _match21320;
  }
  {
    _brw_19573 = kk_integer_from_small(1); /*int*/
  }
  _match21320: ;
  kk_integer_t _brw_19572 = kk_integer_from_small(1); /*int*/;
  bool _brw_19575 = kk_integer_eq_borrow(_brw_19573,_brw_19572,kk_context()); /*bool*/;
  kk_integer_drop(_brw_19573, _ctx);
  kk_integer_drop(_brw_19572, _ctx);
  _match_19568 = _brw_19575; /*bool*/
  if (_match_19568) {
    _own_19567 = slice0; /*sslice*/
  }
  else {
    kk_integer_t _brw_19569;
    kk_integer_t _x21322;
    kk_std_core_types__optional _match_19570 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
    if (kk_std_core_types__is_Optional(_match_19570)) {
      kk_box_t _box_x19422 = _match_19570._cons.Optional.value;
      kk_integer_t _n_101650 = kk_integer_unbox(_box_x19422);
      kk_integer_dup(_n_101650);
      kk_std_core_types__optional_drop(_match_19570, _ctx);
      _x21322 = _n_101650; /*int*/
      goto _match21323;
    }
    {
      _x21322 = kk_integer_from_small(1); /*int*/
    }
    _match21323: ;
    _brw_19569 = kk_integer_sub(_x21322,(kk_integer_from_small(1)),kk_context()); /*int*/
    kk_std_core__sslice _brw_19571 = kk_std_core_extend(slice0, _brw_19569, _ctx); /*sslice*/;
    kk_integer_drop(_brw_19569, _ctx);
    _own_19567 = _brw_19571; /*sslice*/
  }
  kk_integer_t _brw_19566 = kk_integer_sub(count,(kk_integer_from_small(1)),kk_context()); /*int*/;
  kk_std_core__sslice _brw_19576 = kk_std_core_extend(_own_19567, _brw_19566, _ctx); /*sslice*/;
  kk_integer_drop(_brw_19566, _ctx);
  _x21319 = _brw_19576; /*sslice*/
  return kk_std_core_string_3(_x21319, _ctx);
}
 
// Return a default value when an exception is raised


// lift anonymous function
struct kk_std_core_try_default_fun21327__t {
  struct kk_function_s _base;
  kk_box_t value;
};
static kk_box_t kk_std_core_try_default_fun21327(kk_function_t _fself, kk_std_core_hnd__marker _b_19424, kk_std_core_hnd__ev _b_19425, kk_box_t _b_19426, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_default_fun21327(kk_box_t value, kk_context_t* _ctx) {
  struct kk_std_core_try_default_fun21327__t* _self = kk_function_alloc_as(struct kk_std_core_try_default_fun21327__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_try_default_fun21327, kk_context());
  _self->value = value;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_try_default_fun21328__t {
  struct kk_function_s _base;
  kk_box_t value;
};
static kk_box_t kk_std_core_try_default_fun21328(kk_function_t _fself, kk_function_t ___wildcard__585__45, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_default_fun21328(kk_box_t value, kk_context_t* _ctx) {
  struct kk_std_core_try_default_fun21328__t* _self = kk_function_alloc_as(struct kk_std_core_try_default_fun21328__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_try_default_fun21328, kk_context());
  _self->value = value;
  return &_self->_base;
}

static kk_box_t kk_std_core_try_default_fun21328(kk_function_t _fself, kk_function_t ___wildcard__585__45, kk_context_t* _ctx) {
  struct kk_std_core_try_default_fun21328__t* _self = kk_function_as(struct kk_std_core_try_default_fun21328__t*, _fself);
  kk_box_t value = _self->value; /* 16553 */
  kk_drop_match(_self, {kk_box_dup(value);}, {}, _ctx)
  kk_function_drop(___wildcard__585__45, _ctx);
  return value;
}
static kk_box_t kk_std_core_try_default_fun21327(kk_function_t _fself, kk_std_core_hnd__marker _b_19424, kk_std_core_hnd__ev _b_19425, kk_box_t _b_19426, kk_context_t* _ctx) {
  struct kk_std_core_try_default_fun21327__t* _self = kk_function_as(struct kk_std_core_try_default_fun21327__t*, _fself);
  kk_box_t value = _self->value; /* 16553 */
  kk_drop_match(_self, {kk_box_dup(value);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(_b_19425, ((int32_t)KI32(3)), _ctx);
  kk_box_drop(_b_19426, _ctx);
  return kk_std_core_hnd_yield_to_final(_b_19424, kk_std_core_new_try_default_fun21328(value, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_try_default_fun21329__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_try_default_fun21329(kk_function_t _fself, kk_box_t _x, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_try_default_fun21329(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_try_default_fun21329, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_try_default_fun21329(kk_function_t _fself, kk_box_t _x, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _x;
}

kk_box_t kk_std_core_try_default(kk_box_t value, kk_function_t action, kk_context_t* _ctx) { /* forall<a,e> (value : a, action : () -> <exn|e> a) -> e a */ 
  kk_std_core__hnd_exn _x21325;
  kk_std_core_hnd__clause1 _x21326 = kk_std_core_hnd__new_Clause1(kk_std_core_new_try_default_fun21327(value, _ctx), _ctx); /*std/core/hnd/clause1<51,52,53,54,55>*/
  _x21325 = kk_std_core__new_Hnd_exn(kk_reuse_null, _x21326, _ctx); /*.hnd-exn<11,12>*/
  return kk_std_core__handle_exn(((int32_t)KI32(0)), _x21325, kk_std_core_new_try_default_fun21329(_ctx), action, _ctx);
}
 
// Returns a unique integer (modulo 32-bits).


// lift anonymous function
struct kk_std_core_unique_fun21333__t {
  struct kk_function_s _base;
};
static kk_integer_t kk_std_core_unique_fun21333(kk_function_t _fself, kk_integer_t u, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_unique_fun21333(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_unique_fun21333, _ctx)
  return kk_function_dup(_fself);
}

static kk_integer_t kk_std_core_unique_fun21333(kk_function_t _fself, kk_integer_t u, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_integer_t _b_19440_19438;
  kk_integer_t _x21334 = kk_integer_dup(u); /*int*/
  _b_19440_19438 = kk_integer_add(_x21334,(kk_integer_from_small(1)),kk_context()); /*int*/
  kk_unit_t __ = kk_Unit;
  kk_ref_set_borrow(kk_std_core_unique_count,(kk_integer_box(_b_19440_19438)),kk_context());
  return u;
}


// lift anonymous function
struct kk_std_core_unique_fun21336__t {
  struct kk_function_s _base;
  kk_function_t next0_18101;
};
static kk_box_t kk_std_core_unique_fun21336(kk_function_t _fself, kk_box_t _b_19442, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_unique_fun21336(kk_function_t next0_18101, kk_context_t* _ctx) {
  struct kk_std_core_unique_fun21336__t* _self = kk_function_alloc_as(struct kk_std_core_unique_fun21336__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_unique_fun21336, kk_context());
  _self->next0_18101 = next0_18101;
  return &_self->_base;
}

static kk_box_t kk_std_core_unique_fun21336(kk_function_t _fself, kk_box_t _b_19442, kk_context_t* _ctx) {
  struct kk_std_core_unique_fun21336__t* _self = kk_function_as(struct kk_std_core_unique_fun21336__t*, _fself);
  kk_function_t next0_18101 = _self->next0_18101; /* (int) -> <read<global>,write<global>|_16614> int */
  kk_drop_match(_self, {kk_function_dup(next0_18101);}, {}, _ctx)
  kk_integer_t _x21337;
  kk_integer_t _x21338 = kk_integer_unbox(_b_19442); /*int*/
  _x21337 = kk_function_call(kk_integer_t, (kk_function_t, kk_integer_t, kk_context_t*), next0_18101, (next0_18101, _x21338, _ctx)); /*int*/
  return kk_integer_box(_x21337);
}

kk_integer_t kk_std_core_unique(kk_context_t* _ctx) { /* () -> ndet int */ 
  kk_integer_t x_18100;
  kk_box_t _x21331;
  kk_ref_t _x21332 = kk_ref_dup(kk_std_core_unique_count); /*ref<global,int>*/
  _x21331 = kk_ref_get(_x21332,kk_context()); /*184*/
  x_18100 = kk_integer_unbox(_x21331); /*int*/
  kk_function_t next0_18101 = kk_std_core_new_unique_fun21333(_ctx); /*(int) -> <read<global>,write<global>|_16614> int*/;
  if (kk_yielding(kk_context())) {
    kk_integer_drop(x_18100, _ctx);
    kk_box_t _x21335 = kk_std_core_hnd_yield_extend(kk_std_core_new_unique_fun21336(next0_18101, _ctx), _ctx); /*3860*/
    return kk_integer_unbox(_x21335);
  }
  {
    return kk_function_call(kk_integer_t, (kk_function_t, kk_integer_t, kk_context_t*), next0_18101, (next0_18101, x_18100, _ctx));
  }
}
 
// Get the value of the `Just` constructor or raise an exception

kk_box_t kk_std_core_unjust(kk_std_core_types__maybe m, kk_context_t* _ctx) { /* forall<a> (m : maybe<a>) -> exn a */ 
  if (kk_std_core_types__is_Just(m)) {
    kk_box_t x = m._cons.Just.value;
    return x;
  }
  {
    kk_std_core_hnd__ev ev_18103;
    kk_ssize_t _x21339 = ((kk_ssize_t)0); /*ssize_t*/
    ev_18103 = kk_evv_at(_x21339,kk_context()); /*std/core/hnd/ev<.hnd-exn>*/
    {
      struct kk_std_core_hnd_Ev* _con21340 = kk_std_core_hnd__as_Ev(ev_18103);
      kk_std_core_hnd__marker m0 = _con21340->marker;
      kk_box_t _box_x19444 = _con21340->hnd;
      kk_std_core__hnd_exn h = kk_std_core__hnd_exn_unbox(_box_x19444, NULL);
      kk_std_core__hnd_exn_dup(h);
      kk_std_core_hnd__clause1 _match_19562;
      kk_std_core_hnd__clause1 _brw_19564 = kk_std_core__select_throw_exn(h, _ctx); /*std/core/hnd/clause1<exception,1940,.hnd-exn,1941,1942>*/;
      kk_std_core__hnd_exn_drop(h, _ctx);
      _match_19562 = _brw_19564; /*std/core/hnd/clause1<exception,1940,.hnd-exn,1941,1942>*/
      {
        kk_function_t _fun_unbox_x19448 = _match_19562.clause;
        kk_box_t _x21342;
        kk_std_core__exception _x21343;
        kk_string_t _x21344;
        kk_define_string_literal(, _s21345, 37, "unexpected Nothing in std/core/unjust")
        _x21344 = kk_string_dup(_s21345); /*string*/
        kk_std_core__exception_info _x21346;
        kk_std_core_types__optional _match_19563 = kk_std_core_types__new_None(_ctx); /*forall<a> optional<a>*/;
        if (kk_std_core_types__is_Optional(_match_19563)) {
          kk_box_t _box_x19452 = _match_19563._cons.Optional.value;
          kk_std_core__exception_info _info_11739 = kk_std_core__exception_info_unbox(_box_x19452, NULL);
          kk_std_core__exception_info_dup(_info_11739);
          kk_std_core_types__optional_drop(_match_19563, _ctx);
          _x21346 = _info_11739; /*exception-info*/
          goto _match21347;
        }
        {
          _x21346 = kk_std_core__new_ExnError(_ctx); /*exception-info*/
        }
        _match21347: ;
        _x21343 = kk_std_core__new_Exception(_x21344, _x21346, _ctx); /*exception*/
        _x21342 = kk_std_core__exception_box(_x21343, _ctx); /*51*/
        return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__marker, kk_std_core_hnd__ev, kk_box_t, kk_context_t*), _fun_unbox_x19448, (_fun_unbox_x19448, m0, ev_18103, _x21342, _ctx));
      }
    }
  }
}
 
// lifted

kk_string_t kk_std_core__lift17282_unlines(kk_std_core__list ys, kk_string_t acc, kk_context_t* _ctx) { /* (ys : list<string>, acc : string) -> string */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con21349 = kk_std_core__as_Cons(ys);
    kk_box_t _box_x19453 = _con21349->head;
    kk_std_core__list yy = _con21349->tail;
    kk_string_t y = kk_string_unbox(_box_x19453);
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      kk_std_core__list_free(ys);
    }
    else {
      kk_string_dup(y);
      kk_std_core__list_dup(yy);
      kk_std_core__list_decref(ys, _ctx);
    }
    { // tailcall
      kk_string_t _x21351;
      kk_string_t _x21352;
      kk_string_t _x21353;
      kk_define_string_literal(, _s21354, 1, "\n")
      _x21353 = kk_string_dup(_s21354); /*string*/
      _x21352 = kk_std_core__lp__plus__plus__1_rp_(_x21353, y, _ctx); /*string*/
      _x21351 = kk_std_core__lp__plus__plus__1_rp_(acc, _x21352, _ctx); /*string*/
      ys = yy;
      acc = _x21351;
      goto kk__tailcall;
    }
  }
  {
    return acc;
  }
}
 
// Join a list of strings with newlines

kk_string_t kk_std_core_unlines(kk_std_core__list xs, kk_context_t* _ctx) { /* (xs : list<string>) -> string */ 
  if (kk_std_core__is_Nil(xs)) {
    return kk_string_empty();
  }
  {
    struct kk_std_core_Cons* _con21356 = kk_std_core__as_Cons(xs);
    kk_box_t _box_x19454 = _con21356->head;
    kk_std_core__list xx = _con21356->tail;
    kk_string_t x = kk_string_unbox(_box_x19454);
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_string_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    return kk_std_core__lift17282_unlines(xx, x, _ctx);
  }
}
 
// lifted

kk_std_core__list kk_std_core__lift17284_unzip(kk_std_core__list acc, kk_std_core__list ys0, kk_context_t* _ctx) { /* forall<a> (acc : list<a>, ys0 : list<a>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys0)) {
    struct kk_std_core_Cons* _con21358 = kk_std_core__as_Cons(ys0);
    kk_box_t x0 = _con21358->head;
    kk_std_core__list xx0 = _con21358->tail;
    kk_reuse_t _ru_20022 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys0))) {
      _ru_20022 = (kk_std_core__list_reuse(ys0));
    }
    else {
      kk_box_dup(x0);
      kk_std_core__list_dup(xx0);
      kk_std_core__list_decref(ys0, _ctx);
    }
    { // tailcall
      kk_std_core__list _x21359 = kk_std_core__new_Cons(_ru_20022, x0, acc, _ctx); /*list<61>*/
      acc = _x21359;
      ys0 = xx0;
      goto kk__tailcall;
    }
  }
  {
    return acc;
  }
}
 
// lifted

kk_std_core__list kk_std_core__lift17285_unzip(kk_std_core__list acc0, kk_std_core__list ys1, kk_context_t* _ctx) { /* forall<a> (acc0 : list<a>, ys1 : list<a>) -> list<a> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys1)) {
    struct kk_std_core_Cons* _con21360 = kk_std_core__as_Cons(ys1);
    kk_box_t x1 = _con21360->head;
    kk_std_core__list xx1 = _con21360->tail;
    kk_reuse_t _ru_20023 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys1))) {
      _ru_20023 = (kk_std_core__list_reuse(ys1));
    }
    else {
      kk_box_dup(x1);
      kk_std_core__list_dup(xx1);
      kk_std_core__list_decref(ys1, _ctx);
    }
    { // tailcall
      kk_std_core__list _x21361 = kk_std_core__new_Cons(_ru_20023, x1, acc0, _ctx); /*list<61>*/
      acc0 = _x21361;
      ys1 = xx1;
      goto kk__tailcall;
    }
  }
  {
    return acc0;
  }
}
 
// lifted
// todo: implement TRMC for multiple results

kk_std_core_types__tuple2_ kk_std_core__lift17283_unzip(kk_std_core__list ys, kk_std_core__list acc1, kk_std_core__list acc2, kk_context_t* _ctx) { /* forall<a,b> (ys : list<(a, b)>, acc1 : list<a>, acc2 : list<b>) -> (list<a>, list<b>) */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(ys)) {
    struct kk_std_core_Cons* _con21362 = kk_std_core__as_Cons(ys);
    kk_box_t _box_x19455 = _con21362->head;
    kk_std_core__list xx = _con21362->tail;
    kk_std_core_types__tuple2_ _pat0 = kk_std_core_types__tuple2__unbox(_box_x19455, NULL);
    kk_box_t x = _pat0.fst;
    kk_box_t y = _pat0.snd;
    kk_reuse_t _ru_20024 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_std_core__list_is_unique(ys))) {
      kk_box_dup(x);
      kk_box_dup(y);
      kk_box_drop(_box_x19455, _ctx);
      _ru_20024 = (kk_std_core__list_reuse(ys));
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_box_dup(y);
      kk_std_core__list_decref(ys, _ctx);
    }
    { // tailcall
      kk_std_core__list _x21364 = kk_std_core__new_Cons(_ru_20024, x, acc1, _ctx); /*list<61>*/
      kk_std_core__list _x21365 = kk_std_core__new_Cons(kk_reuse_null, y, acc2, _ctx); /*list<61>*/
      ys = xx;
      acc1 = _x21364;
      acc2 = _x21365;
      goto kk__tailcall;
    }
  }
  {
    kk_std_core__list _b_19458_19456 = kk_std_core__lift17284_unzip(kk_std_core__new_Nil(_ctx), acc1, _ctx); /*list<16682>*/;
    kk_std_core__list _b_19459_19457 = kk_std_core__lift17285_unzip(kk_std_core__new_Nil(_ctx), acc2, _ctx); /*list<16683>*/;
    return kk_std_core_types__new_dash__lp__comma__rp_(kk_std_core__list_box(_b_19458_19456, _ctx), kk_std_core__list_box(_b_19459_19457, _ctx), _ctx);
  }
}
 
// Convert a string to a vector of characters.

kk_vector_t kk_std_core_vector_1(kk_string_t s, kk_context_t* _ctx) { /* (s : string) -> vector<char> */ 
  return kk_string_to_chars(s,kk_context());
}
extern kk_box_t kk_std_core_vector_fun21368_2(kk_function_t _fself, kk_ssize_t ___wildcard__2118__30, kk_context_t* _ctx) {
  struct kk_std_core_vector_fun21368__t_2* _self = kk_function_as(struct kk_std_core_vector_fun21368__t_2*, _fself);
  kk_box_t default0 = _self->default0; /* 16759 */
  kk_drop_match(_self, {kk_box_dup(default0);}, {}, _ctx)
  return default0;
}
extern kk_box_t kk_std_core_vector_init_fun21371(kk_function_t _fself, kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_vector_init_fun21371__t* _self = kk_function_as(struct kk_std_core_vector_init_fun21371__t*, _fself);
  kk_function_t f = _self->f; /* (int) -> 16828 */
  kk_drop_match(_self, {kk_function_dup(f);}, {}, _ctx)
  kk_integer_t _x21372 = kk_integer_from_ssize_t(i,kk_context()); /*int*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_integer_t, kk_context_t*), f, (f, _x21372, _ctx));
}
 
// monadic lift

kk_unit_t kk_std_core__mlift17759_while(kk_function_t action, kk_function_t predicate, kk_unit_t wild__, kk_context_t* _ctx) { /* forall<e> (action : () -> <div|e> (), predicate : () -> <div|e> bool, wild_ : ()) -> <div|e> () */ 
  kk_std_core_while(predicate, action, _ctx); return kk_Unit;
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17760_while_fun21375__t {
  struct kk_function_s _base;
  kk_function_t action0;
  kk_function_t predicate0;
};
static kk_box_t kk_std_core__mlift17760_while_fun21375(kk_function_t _fself, kk_box_t _b_19461, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17760_while_fun21375(kk_function_t action0, kk_function_t predicate0, kk_context_t* _ctx) {
  struct kk_std_core__mlift17760_while_fun21375__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17760_while_fun21375__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17760_while_fun21375, kk_context());
  _self->action0 = action0;
  _self->predicate0 = predicate0;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17760_while_fun21375(kk_function_t _fself, kk_box_t _b_19461, kk_context_t* _ctx) {
  struct kk_std_core__mlift17760_while_fun21375__t* _self = kk_function_as(struct kk_std_core__mlift17760_while_fun21375__t*, _fself);
  kk_function_t action0 = _self->action0; /* () -> <div|16849> () */
  kk_function_t predicate0 = _self->predicate0; /* () -> <div|16849> bool */
  kk_drop_match(_self, {kk_function_dup(action0);kk_function_dup(predicate0);}, {}, _ctx)
  kk_unit_t _x21376 = kk_Unit;
  kk_unit_t _x21377 = kk_Unit;
  kk_unit_unbox(_b_19461);
  kk_std_core__mlift17759_while(action0, predicate0, _x21377, _ctx);
  return kk_unit_box(_x21376);
}

kk_unit_t kk_std_core__mlift17760_while(kk_function_t action0, kk_function_t predicate0, bool _y_17649, kk_context_t* _ctx) { /* forall<e> (action : () -> <div|e> (), predicate : () -> <div|e> bool, bool) -> <div|e> () */ 
  if (_y_17649) {
    kk_unit_t x_18106 = kk_Unit;
    kk_function_t _x21373 = kk_function_dup(action0); /*() -> <div|16849> ()*/
    kk_function_call(kk_unit_t, (kk_function_t, kk_context_t*), _x21373, (_x21373, _ctx));
    if (kk_yielding(kk_context())) {
      kk_box_t _x21374 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17760_while_fun21375(action0, predicate0, _ctx), _ctx); /*3860*/
      kk_unit_unbox(_x21374); return kk_Unit;
    }
    {
      kk_std_core__mlift17759_while(action0, predicate0, x_18106, _ctx); return kk_Unit;
    }
  }
  {
    kk_function_drop(predicate0, _ctx);
    kk_function_drop(action0, _ctx);
    kk_Unit; return kk_Unit;
  }
}
 
// The `while` fun executes `action`  as long as `pred`  is `true`.


// lift anonymous function
struct kk_std_core_while_fun21380__t {
  struct kk_function_s _base;
  kk_function_t action1;
  kk_function_t predicate1;
};
static kk_box_t kk_std_core_while_fun21380(kk_function_t _fself, kk_box_t _b_19465, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_while_fun21380(kk_function_t action1, kk_function_t predicate1, kk_context_t* _ctx) {
  struct kk_std_core_while_fun21380__t* _self = kk_function_alloc_as(struct kk_std_core_while_fun21380__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_while_fun21380, kk_context());
  _self->action1 = action1;
  _self->predicate1 = predicate1;
  return &_self->_base;
}

static kk_box_t kk_std_core_while_fun21380(kk_function_t _fself, kk_box_t _b_19465, kk_context_t* _ctx) {
  struct kk_std_core_while_fun21380__t* _self = kk_function_as(struct kk_std_core_while_fun21380__t*, _fself);
  kk_function_t action1 = _self->action1; /* () -> <div|16849> () */
  kk_function_t predicate1 = _self->predicate1; /* () -> <div|16849> bool */
  kk_drop_match(_self, {kk_function_dup(action1);kk_function_dup(predicate1);}, {}, _ctx)
  kk_unit_t _x21381 = kk_Unit;
  bool _x21382 = kk_bool_unbox(_b_19465); /*bool*/
  kk_std_core__mlift17760_while(action1, predicate1, _x21382, _ctx);
  return kk_unit_box(_x21381);
}


// lift anonymous function
struct kk_std_core_while_fun21385__t {
  struct kk_function_s _base;
  kk_function_t action1;
  kk_function_t predicate1;
};
static kk_box_t kk_std_core_while_fun21385(kk_function_t _fself, kk_box_t _b_19467, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_while_fun21385(kk_function_t action1, kk_function_t predicate1, kk_context_t* _ctx) {
  struct kk_std_core_while_fun21385__t* _self = kk_function_alloc_as(struct kk_std_core_while_fun21385__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_while_fun21385, kk_context());
  _self->action1 = action1;
  _self->predicate1 = predicate1;
  return &_self->_base;
}

static kk_box_t kk_std_core_while_fun21385(kk_function_t _fself, kk_box_t _b_19467, kk_context_t* _ctx) {
  struct kk_std_core_while_fun21385__t* _self = kk_function_as(struct kk_std_core_while_fun21385__t*, _fself);
  kk_function_t action1 = _self->action1; /* () -> <div|16849> () */
  kk_function_t predicate1 = _self->predicate1; /* () -> <div|16849> bool */
  kk_drop_match(_self, {kk_function_dup(action1);kk_function_dup(predicate1);}, {}, _ctx)
  kk_unit_t _x21386 = kk_Unit;
  kk_unit_t _x21387 = kk_Unit;
  kk_unit_unbox(_b_19467);
  kk_std_core__mlift17759_while(action1, predicate1, _x21387, _ctx);
  return kk_unit_box(_x21386);
}

kk_unit_t kk_std_core_while(kk_function_t predicate1, kk_function_t action1, kk_context_t* _ctx) { /* forall<e> (predicate : () -> <div|e> bool, action : () -> <div|e> ()) -> <div|e> () */ 
  kk__tailcall: ;
  bool x0_18108;
  kk_function_t _x21378 = kk_function_dup(predicate1); /*() -> <div|16849> bool*/
  x0_18108 = kk_function_call(bool, (kk_function_t, kk_context_t*), _x21378, (_x21378, _ctx)); /*bool*/
  if (kk_yielding(kk_context())) {
    kk_box_t _x21379 = kk_std_core_hnd_yield_extend(kk_std_core_new_while_fun21380(action1, predicate1, _ctx), _ctx); /*3860*/
    kk_unit_unbox(_x21379); return kk_Unit;
  }
  if (x0_18108) {
    kk_unit_t x1_18111 = kk_Unit;
    kk_function_t _x21383 = kk_function_dup(action1); /*() -> <div|16849> ()*/
    kk_function_call(kk_unit_t, (kk_function_t, kk_context_t*), _x21383, (_x21383, _ctx));
    if (kk_yielding(kk_context())) {
      kk_box_t _x21384 = kk_std_core_hnd_yield_extend(kk_std_core_new_while_fun21385(action1, predicate1, _ctx), _ctx); /*3860*/
      kk_unit_unbox(_x21384); return kk_Unit;
    }
    { // tailcall
      goto kk__tailcall;
    }
  }
  {
    kk_function_drop(predicate1, _ctx);
    kk_function_drop(action1, _ctx);
    kk_Unit; return kk_Unit;
  }
}
 
// Zip two lists together by pairing the corresponding elements.
// The returned list is only as long as the smallest input list.

kk_std_core__list kk_std_core__ctail_zip(kk_std_core__list xs, kk_std_core__list ys, kk_std_core_types__ctail _acc, kk_context_t* _ctx) { /* forall<a,b> (xs : list<a>, ys : list<b>, ctail<list<(a, b)>>) -> list<(a, b)> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21388 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21388->head;
    kk_std_core__list xx = _con21388->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx);
      kk_std_core__list_decref(xs, _ctx);
    }
    if (kk_std_core__is_Cons(ys)) {
      struct kk_std_core_Cons* _con21389 = kk_std_core__as_Cons(ys);
      kk_box_t y = _con21389->head;
      kk_std_core__list yy = _con21389->tail;
      kk_reuse_t _ru_20026 = kk_reuse_null; /*reuse*/;
      if (kk_likely(kk_std_core__list_is_unique(ys))) {
        _ru_20026 = (kk_std_core__list_reuse(ys));
      }
      else {
        kk_box_dup(y);
        kk_std_core__list_dup(yy);
        kk_std_core__list_decref(ys, _ctx);
      }
      kk_std_core__list _ctail_17379 = kk_std_core__list_hole(); /*list<(16892, 16893)>*/;
      kk_std_core__list _ctail_17380;
      kk_box_t _x21390;
      kk_std_core_types__tuple2_ _x21391 = kk_std_core_types__new_dash__lp__comma__rp_(x, y, _ctx); /*(6, 7)*/
      _x21390 = kk_std_core_types__tuple2__box(_x21391, _ctx); /*61*/
      _ctail_17380 = kk_std_core__new_Cons(_ru_20026, _x21390, _ctail_17379, _ctx); /*list<(16892, 16893)>*/
      { // tailcall
        kk_std_core_types__ctail _x21392;
        kk_box_t* _b_19488_19481 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17380)->tail)); /*cfield<list<(16892, 16893)>>*/;
        _x21392 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17380, _ctx)),_b_19488_19481); /*ctail<0>*/
        xs = xx;
        ys = yy;
        _acc = _x21392;
        goto kk__tailcall;
      }
    }
    {
      kk_std_core__list_drop(xx, _ctx);
      kk_box_drop(x, _ctx);
      kk_box_t _x21393 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
      return kk_std_core__list_unbox(_x21393, _ctx);
    }
  }
  {
    kk_std_core__list_drop(ys, _ctx);
    kk_box_t _x21394 = kk_ctail_resolve(_acc,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x21394, _ctx);
  }
}
 
// Zip two lists together by pairing the corresponding elements.
// The returned list is only as long as the smallest input list.

kk_std_core__list kk_std_core_zip(kk_std_core__list xs0, kk_std_core__list ys0, kk_context_t* _ctx) { /* forall<a,b> (xs : list<a>, ys : list<b>) -> list<(a, b)> */ 
  kk_std_core_types__ctail _x21395 = kk_ctail_nil(); /*ctail<0>*/
  return kk_std_core__ctail_zip(xs0, ys0, _x21395, _ctx);
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17761_op(kk_std_core_types__ctail _acc, kk_function_t f, kk_std_core__list xx, kk_std_core__list yy, kk_box_t _ctail_17381, kk_context_t* _ctx) { /* forall<a,b,c,e> (ctail<list<c>>, f : (a, b) -> e c, xx : list<a>, yy : list<b>, c) -> e list<c> */ 
  kk_std_core__list _ctail_17382 = kk_std_core__list_hole(); /*list<16940>*/;
  kk_std_core__list _ctail_17383 = kk_std_core__new_Cons(kk_reuse_null, _ctail_17381, _ctail_17382, _ctx); /*list<16940>*/;
  kk_std_core_types__ctail _x21396;
  kk_box_t* _b_19504_19501 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_17383)->tail)); /*cfield<list<16940>>*/;
  _x21396 = kk_ctail_link(_acc,(kk_std_core__list_box(_ctail_17383, _ctx)),_b_19504_19501); /*ctail<0>*/
  return kk_std_core__ctail_zipwith(xx, yy, f, _x21396, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17762_op_fun21397__t {
  struct kk_function_s _base;
  kk_function_t _accm;
  kk_box_t _ctail_17386;
};
static kk_std_core__list kk_std_core__mlift17762_op_fun21397(kk_function_t _fself, kk_std_core__list _ctail_17385, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17762_op_fun21397(kk_function_t _accm, kk_box_t _ctail_17386, kk_context_t* _ctx) {
  struct kk_std_core__mlift17762_op_fun21397__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17762_op_fun21397__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17762_op_fun21397, kk_context());
  _self->_accm = _accm;
  _self->_ctail_17386 = _ctail_17386;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__mlift17762_op_fun21397(kk_function_t _fself, kk_std_core__list _ctail_17385, kk_context_t* _ctx) {
  struct kk_std_core__mlift17762_op_fun21397__t* _self = kk_function_as(struct kk_std_core__mlift17762_op_fun21397__t*, _fself);
  kk_function_t _accm = _self->_accm; /* (list<16940>) -> list<16940> */
  kk_box_t _ctail_17386 = _self->_ctail_17386; /* 16940 */
  kk_drop_match(_self, {kk_function_dup(_accm);kk_box_dup(_ctail_17386);}, {}, _ctx)
  kk_std_core__list _x21398 = kk_std_core__new_Cons(kk_reuse_null, _ctail_17386, _ctail_17385, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm, (_accm, _x21398, _ctx));
}

kk_std_core__list kk_std_core__mlift17762_op(kk_function_t _accm, kk_function_t f0, kk_std_core__list xx0, kk_std_core__list yy0, kk_box_t _ctail_17386, kk_context_t* _ctx) { /* forall<a,b,c,e> ((list<c>) -> list<c>, f : (a, b) -> e c, xx : list<a>, yy : list<b>, c) -> e list<c> */ 
  return kk_std_core__ctailm_zipwith(xx0, yy0, f0, kk_std_core__new_mlift17762_op_fun21397(_accm, _ctail_17386, _ctx), _ctx);
}
 
// Zip two lists together by apply a function `f` to all corresponding elements.
// The returned list is only as long as the smallest input list.


// lift anonymous function
struct kk_std_core__ctail_zipwith_fun21403__t {
  struct kk_function_s _base;
  kk_function_t f1;
  kk_std_core__list xx1;
  kk_std_core__list yy1;
  kk_std_core_types__ctail _acc0;
};
static kk_box_t kk_std_core__ctail_zipwith_fun21403(kk_function_t _fself, kk_box_t _b_19509, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctail_zipwith_fun21403(kk_function_t f1, kk_std_core__list xx1, kk_std_core__list yy1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) {
  struct kk_std_core__ctail_zipwith_fun21403__t* _self = kk_function_alloc_as(struct kk_std_core__ctail_zipwith_fun21403__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctail_zipwith_fun21403, kk_context());
  _self->f1 = f1;
  _self->xx1 = xx1;
  _self->yy1 = yy1;
  _self->_acc0 = _acc0;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctail_zipwith_fun21403(kk_function_t _fself, kk_box_t _b_19509, kk_context_t* _ctx) {
  struct kk_std_core__ctail_zipwith_fun21403__t* _self = kk_function_as(struct kk_std_core__ctail_zipwith_fun21403__t*, _fself);
  kk_function_t f1 = _self->f1; /* (16938, 16939) -> 16941 16940 */
  kk_std_core__list xx1 = _self->xx1; /* list<16938> */
  kk_std_core__list yy1 = _self->yy1; /* list<16939> */
  kk_std_core_types__ctail _acc0 = _self->_acc0; /* ctail<list<16940>> */
  kk_drop_match(_self, {kk_function_dup(f1);kk_std_core__list_dup(xx1);kk_std_core__list_dup(yy1);kk_std_core_types__ctail_dup(_acc0);}, {}, _ctx)
  kk_std_core__list _x21404 = kk_std_core__mlift17761_op(_acc0, f1, xx1, yy1, _b_19509, _ctx); /*list<16940>*/
  return kk_std_core__list_box(_x21404, _ctx);
}

kk_std_core__list kk_std_core__ctail_zipwith(kk_std_core__list xs, kk_std_core__list ys, kk_function_t f1, kk_std_core_types__ctail _acc0, kk_context_t* _ctx) { /* forall<a,b,c,e> (xs : list<a>, ys : list<b>, f : (a, b) -> e c, ctail<list<c>>) -> e list<c> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21399 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21399->head;
    kk_std_core__list xx1 = _con21399->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx1);
      kk_std_core__list_decref(xs, _ctx);
    }
    if (kk_std_core__is_Cons(ys)) {
      struct kk_std_core_Cons* _con21400 = kk_std_core__as_Cons(ys);
      kk_box_t y = _con21400->head;
      kk_std_core__list yy1 = _con21400->tail;
      kk_reuse_t _ru_20028 = kk_reuse_null; /*reuse*/;
      if (kk_likely(kk_std_core__list_is_unique(ys))) {
        _ru_20028 = (kk_std_core__list_reuse(ys));
      }
      else {
        kk_box_dup(y);
        kk_std_core__list_dup(yy1);
        kk_std_core__list_decref(ys, _ctx);
      }
      kk_box_t x0_18114;
      kk_function_t _x21401 = kk_function_dup(f1); /*(16938, 16939) -> 16941 16940*/
      x0_18114 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), _x21401, (_x21401, x, y, _ctx)); /*16940*/
      if (kk_yielding(kk_context())) {
        kk_reuse_drop(_ru_20028);
        kk_box_drop(x0_18114, _ctx);
        kk_box_t _x21402 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctail_zipwith_fun21403(f1, xx1, yy1, _acc0, _ctx), _ctx); /*3860*/
        return kk_std_core__list_unbox(_x21402, _ctx);
      }
      {
        kk_std_core__list _ctail_173820 = kk_std_core__list_hole(); /*list<16940>*/;
        kk_std_core__list _ctail_173830 = kk_std_core__new_Cons(_ru_20028, x0_18114, _ctail_173820, _ctx); /*list<16940>*/;
        { // tailcall
          kk_std_core_types__ctail _x21405;
          kk_box_t* _b_19523_19515 = (kk_box_t*)((&kk_std_core__as_Cons(_ctail_173830)->tail)); /*cfield<list<16940>>*/;
          _x21405 = kk_ctail_link(_acc0,(kk_std_core__list_box(_ctail_173830, _ctx)),_b_19523_19515); /*ctail<0>*/
          xs = xx1;
          ys = yy1;
          _acc0 = _x21405;
          goto kk__tailcall;
        }
      }
    }
    {
      kk_std_core__list_drop(xx1, _ctx);
      kk_box_drop(x, _ctx);
      kk_function_drop(f1, _ctx);
      kk_box_t _x21406 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
      return kk_std_core__list_unbox(_x21406, _ctx);
    }
  }
  {
    kk_std_core__list_drop(ys, _ctx);
    kk_function_drop(f1, _ctx);
    kk_box_t _x21407 = kk_ctail_resolve(_acc0,(kk_std_core__list_box(kk_std_core__new_Nil(_ctx), _ctx))); /*-1*/
    return kk_std_core__list_unbox(_x21407, _ctx);
  }
}
 
// Zip two lists together by apply a function `f` to all corresponding elements.
// The returned list is only as long as the smallest input list.


// lift anonymous function
struct kk_std_core__ctailm_zipwith_fun21412__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_function_t f2;
  kk_std_core__list xx2;
  kk_std_core__list yy2;
};
static kk_box_t kk_std_core__ctailm_zipwith_fun21412(kk_function_t _fself, kk_box_t _b_19533, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_zipwith_fun21412(kk_function_t _accm0, kk_function_t f2, kk_std_core__list xx2, kk_std_core__list yy2, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_zipwith_fun21412__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_zipwith_fun21412__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_zipwith_fun21412, kk_context());
  _self->_accm0 = _accm0;
  _self->f2 = f2;
  _self->xx2 = xx2;
  _self->yy2 = yy2;
  return &_self->_base;
}

static kk_box_t kk_std_core__ctailm_zipwith_fun21412(kk_function_t _fself, kk_box_t _b_19533, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_zipwith_fun21412__t* _self = kk_function_as(struct kk_std_core__ctailm_zipwith_fun21412__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<16940>) -> list<16940> */
  kk_function_t f2 = _self->f2; /* (16938, 16939) -> 16941 16940 */
  kk_std_core__list xx2 = _self->xx2; /* list<16938> */
  kk_std_core__list yy2 = _self->yy2; /* list<16939> */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_function_dup(f2);kk_std_core__list_dup(xx2);kk_std_core__list_dup(yy2);}, {}, _ctx)
  kk_std_core__list _x21413 = kk_std_core__mlift17762_op(_accm0, f2, xx2, yy2, _b_19533, _ctx); /*list<16940>*/
  return kk_std_core__list_box(_x21413, _ctx);
}


// lift anonymous function
struct kk_std_core__ctailm_zipwith_fun21415__t {
  struct kk_function_s _base;
  kk_function_t _accm0;
  kk_box_t x2_18117;
};
static kk_std_core__list kk_std_core__ctailm_zipwith_fun21415(kk_function_t _fself, kk_std_core__list _ctail_173850, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_ctailm_zipwith_fun21415(kk_function_t _accm0, kk_box_t x2_18117, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_zipwith_fun21415__t* _self = kk_function_alloc_as(struct kk_std_core__ctailm_zipwith_fun21415__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__ctailm_zipwith_fun21415, kk_context());
  _self->_accm0 = _accm0;
  _self->x2_18117 = x2_18117;
  return &_self->_base;
}

static kk_std_core__list kk_std_core__ctailm_zipwith_fun21415(kk_function_t _fself, kk_std_core__list _ctail_173850, kk_context_t* _ctx) {
  struct kk_std_core__ctailm_zipwith_fun21415__t* _self = kk_function_as(struct kk_std_core__ctailm_zipwith_fun21415__t*, _fself);
  kk_function_t _accm0 = _self->_accm0; /* (list<16940>) -> list<16940> */
  kk_box_t x2_18117 = _self->x2_18117; /* 16940 */
  kk_drop_match(_self, {kk_function_dup(_accm0);kk_box_dup(x2_18117);}, {}, _ctx)
  kk_std_core__list _x21416 = kk_std_core__new_Cons(kk_reuse_null, x2_18117, _ctail_173850, _ctx); /*list<61>*/
  return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, _x21416, _ctx));
}

kk_std_core__list kk_std_core__ctailm_zipwith(kk_std_core__list xs0, kk_std_core__list ys0, kk_function_t f2, kk_function_t _accm0, kk_context_t* _ctx) { /* forall<a,b,c,e> (xs : list<a>, ys : list<b>, f : (a, b) -> e c, (list<c>) -> list<c>) -> e list<c> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Cons(xs0)) {
    struct kk_std_core_Cons* _con21408 = kk_std_core__as_Cons(xs0);
    kk_box_t x1 = _con21408->head;
    kk_std_core__list xx2 = _con21408->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs0))) {
      kk_std_core__list_free(xs0);
    }
    else {
      kk_box_dup(x1);
      kk_std_core__list_dup(xx2);
      kk_std_core__list_decref(xs0, _ctx);
    }
    if (kk_std_core__is_Cons(ys0)) {
      struct kk_std_core_Cons* _con21409 = kk_std_core__as_Cons(ys0);
      kk_box_t y0 = _con21409->head;
      kk_std_core__list yy2 = _con21409->tail;
      if (kk_likely(kk_std_core__list_is_unique(ys0))) {
        kk_std_core__list_free(ys0);
      }
      else {
        kk_box_dup(y0);
        kk_std_core__list_dup(yy2);
        kk_std_core__list_decref(ys0, _ctx);
      }
      kk_box_t x2_18117;
      kk_function_t _x21410 = kk_function_dup(f2); /*(16938, 16939) -> 16941 16940*/
      x2_18117 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), _x21410, (_x21410, x1, y0, _ctx)); /*16940*/
      if (kk_yielding(kk_context())) {
        kk_box_drop(x2_18117, _ctx);
        kk_box_t _x21411 = kk_std_core_hnd_yield_extend(kk_std_core__new_ctailm_zipwith_fun21412(_accm0, f2, xx2, yy2, _ctx), _ctx); /*3860*/
        return kk_std_core__list_unbox(_x21411, _ctx);
      }
      { // tailcall
        kk_function_t _x21414 = kk_std_core__new_ctailm_zipwith_fun21415(_accm0, x2_18117, _ctx); /*(list<16940>) -> list<16940>*/
        xs0 = xx2;
        ys0 = yy2;
        _accm0 = _x21414;
        goto kk__tailcall;
      }
    }
    {
      kk_std_core__list_drop(xx2, _ctx);
      kk_box_drop(x1, _ctx);
      kk_function_drop(f2, _ctx);
      return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
    }
  }
  {
    kk_std_core__list_drop(ys0, _ctx);
    kk_function_drop(f2, _ctx);
    return kk_function_call(kk_std_core__list, (kk_function_t, kk_std_core__list, kk_context_t*), _accm0, (_accm0, kk_std_core__new_Nil(_ctx), _ctx));
  }
}
 
// Zip two lists together by apply a function `f` to all corresponding elements.
// The returned list is only as long as the smallest input list.


// lift anonymous function
struct kk_std_core_zipwith_fun21418__t {
  struct kk_function_s _base;
};
static kk_std_core__list kk_std_core_zipwith_fun21418(kk_function_t _fself, kk_std_core__list _ctail_17384, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_zipwith_fun21418(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_zipwith_fun21418, _ctx)
  return kk_function_dup(_fself);
}

static kk_std_core__list kk_std_core_zipwith_fun21418(kk_function_t _fself, kk_std_core__list _ctail_17384, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return _ctail_17384;
}

kk_std_core__list kk_std_core_zipwith(kk_std_core__list xs1, kk_std_core__list ys1, kk_function_t f3, kk_context_t* _ctx) { /* forall<a,b,c,e> (xs : list<a>, ys : list<b>, f : (a, b) -> e c) -> e list<c> */ 
  bool _match_19556 = kk_std_core_hnd__evv_is_affine(_ctx); /*bool*/;
  if (_match_19556) {
    kk_std_core_types__ctail _x21417 = kk_ctail_nil(); /*ctail<0>*/
    return kk_std_core__ctail_zipwith(xs1, ys1, f3, _x21417, _ctx);
  }
  {
    return kk_std_core__ctailm_zipwith(xs1, ys1, f3, kk_std_core_new_zipwith_fun21418(_ctx), _ctx);
  }
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17763_zipwith_acc(kk_std_core__list acc, kk_function_t f, kk_integer_t i, kk_std_core__list xx, kk_std_core__list yy, kk_box_t _y_17664, kk_context_t* _ctx) { /* forall<e,a,b,c> (acc : list<b>, f : (int, a, c) -> e b, i : int, xx : list<a>, yy : list<c>, b) -> e list<b> */ 
  kk_integer_t _x21419 = kk_integer_add(i,(kk_integer_from_small(1)),kk_context()); /*int*/
  kk_std_core__list _x21420 = kk_std_core__new_Cons(kk_reuse_null, _y_17664, acc, _ctx); /*list<61>*/
  return kk_std_core_zipwith_acc(f, _x21419, _x21420, xx, yy, _ctx);
}


// lift anonymous function
struct kk_std_core_zipwith_acc_fun21426__t {
  struct kk_function_s _base;
  kk_std_core__list acc0;
  kk_function_t f0;
  kk_integer_t i0;
  kk_std_core__list xx0;
  kk_std_core__list yy0;
};
static kk_box_t kk_std_core_zipwith_acc_fun21426(kk_function_t _fself, kk_box_t _b_19537, kk_context_t* _ctx);
static kk_function_t kk_std_core_new_zipwith_acc_fun21426(kk_std_core__list acc0, kk_function_t f0, kk_integer_t i0, kk_std_core__list xx0, kk_std_core__list yy0, kk_context_t* _ctx) {
  struct kk_std_core_zipwith_acc_fun21426__t* _self = kk_function_alloc_as(struct kk_std_core_zipwith_acc_fun21426__t, 6, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_zipwith_acc_fun21426, kk_context());
  _self->acc0 = acc0;
  _self->f0 = f0;
  _self->i0 = i0;
  _self->xx0 = xx0;
  _self->yy0 = yy0;
  return &_self->_base;
}

static kk_box_t kk_std_core_zipwith_acc_fun21426(kk_function_t _fself, kk_box_t _b_19537, kk_context_t* _ctx) {
  struct kk_std_core_zipwith_acc_fun21426__t* _self = kk_function_as(struct kk_std_core_zipwith_acc_fun21426__t*, _fself);
  kk_std_core__list acc0 = _self->acc0; /* list<16953> */
  kk_function_t f0 = _self->f0; /* (int, 16951, 16961) -> 16948 16953 */
  kk_integer_t i0 = _self->i0; /* int */
  kk_std_core__list xx0 = _self->xx0; /* list<16951> */
  kk_std_core__list yy0 = _self->yy0; /* list<16961> */
  kk_drop_match(_self, {kk_std_core__list_dup(acc0);kk_function_dup(f0);kk_integer_dup(i0);kk_std_core__list_dup(xx0);kk_std_core__list_dup(yy0);}, {}, _ctx)
  kk_std_core__list _x21427 = kk_std_core__mlift17763_zipwith_acc(acc0, f0, i0, xx0, yy0, _b_19537, _ctx); /*list<16953>*/
  return kk_std_core__list_box(_x21427, _ctx);
}

kk_std_core__list kk_std_core_zipwith_acc(kk_function_t f0, kk_integer_t i0, kk_std_core__list acc0, kk_std_core__list xs, kk_std_core__list ys, kk_context_t* _ctx) { /* forall<a,b,c,e> ((int, a, b) -> e c, int, list<c>, list<a>, list<b>) -> e list<c> */ 
  kk__tailcall: ;
  if (kk_std_core__is_Nil(xs)) {
    kk_std_core__list_drop(ys, _ctx);
    kk_integer_drop(i0, _ctx);
    kk_function_drop(f0, _ctx);
    return kk_std_core__lift17274_reverse(kk_std_core__new_Nil(_ctx), acc0, _ctx);
  }
  {
    struct kk_std_core_Cons* _con21421 = kk_std_core__as_Cons(xs);
    kk_box_t x = _con21421->head;
    kk_std_core__list xx0 = _con21421->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x);
      kk_std_core__list_dup(xx0);
      kk_std_core__list_decref(xs, _ctx);
    }
    if (kk_std_core__is_Nil(ys)) {
      kk_std_core__list_drop(xx0, _ctx);
      kk_box_drop(x, _ctx);
      kk_integer_drop(i0, _ctx);
      kk_function_drop(f0, _ctx);
      return kk_std_core__lift17274_reverse(kk_std_core__new_Nil(_ctx), acc0, _ctx);
    }
    {
      struct kk_std_core_Cons* _con21422 = kk_std_core__as_Cons(ys);
      kk_box_t y = _con21422->head;
      kk_std_core__list yy0 = _con21422->tail;
      kk_reuse_t _ru_20032 = kk_reuse_null; /*reuse*/;
      if (kk_likely(kk_std_core__list_is_unique(ys))) {
        _ru_20032 = (kk_std_core__list_reuse(ys));
      }
      else {
        kk_box_dup(y);
        kk_std_core__list_dup(yy0);
        kk_std_core__list_decref(ys, _ctx);
      }
      kk_box_t x0_18122;
      kk_function_t _x21424 = kk_function_dup(f0); /*(int, 16951, 16961) -> 16948 16953*/
      kk_integer_t _x21423 = kk_integer_dup(i0); /*int*/
      x0_18122 = kk_function_call(kk_box_t, (kk_function_t, kk_integer_t, kk_box_t, kk_box_t, kk_context_t*), _x21424, (_x21424, _x21423, x, y, _ctx)); /*16953*/
      if (kk_yielding(kk_context())) {
        kk_reuse_drop(_ru_20032);
        kk_box_drop(x0_18122, _ctx);
        kk_box_t _x21425 = kk_std_core_hnd_yield_extend(kk_std_core_new_zipwith_acc_fun21426(acc0, f0, i0, xx0, yy0, _ctx), _ctx); /*3860*/
        return kk_std_core__list_unbox(_x21425, _ctx);
      }
      { // tailcall
        kk_integer_t _x21428 = kk_integer_add(i0,(kk_integer_from_small(1)),kk_context()); /*int*/
        kk_std_core__list _x21429 = kk_std_core__new_Cons(_ru_20032, x0_18122, acc0, _ctx); /*list<61>*/
        i0 = _x21428;
        acc0 = _x21429;
        xs = xx0;
        ys = yy0;
        goto kk__tailcall;
      }
    }
  }
}
 
// monadic lift

kk_std_core__list kk_std_core__mlift17764_op(kk_box_t _y_17668, kk_std_core__list _y_17669, kk_context_t* _ctx) { /* forall<a,e> (a, list<a>) -> e list<a> */ 
  return kk_std_core__new_Cons(kk_reuse_null, _y_17668, _y_17669, _ctx);
}
 
// monadic lift


// lift anonymous function
struct kk_std_core__mlift17765_op_fun21431__t {
  struct kk_function_s _base;
  kk_box_t _y_176680;
};
static kk_box_t kk_std_core__mlift17765_op_fun21431(kk_function_t _fself, kk_box_t _b_19541, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_mlift17765_op_fun21431(kk_box_t _y_176680, kk_context_t* _ctx) {
  struct kk_std_core__mlift17765_op_fun21431__t* _self = kk_function_alloc_as(struct kk_std_core__mlift17765_op_fun21431__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__mlift17765_op_fun21431, kk_context());
  _self->_y_176680 = _y_176680;
  return &_self->_base;
}

static kk_box_t kk_std_core__mlift17765_op_fun21431(kk_function_t _fself, kk_box_t _b_19541, kk_context_t* _ctx) {
  struct kk_std_core__mlift17765_op_fun21431__t* _self = kk_function_as(struct kk_std_core__mlift17765_op_fun21431__t*, _fself);
  kk_box_t _y_176680 = _self->_y_176680; /* 17082 */
  kk_drop_match(_self, {kk_box_dup(_y_176680);}, {}, _ctx)
  kk_std_core__list _x21432;
  kk_std_core__list _x21433 = kk_std_core__list_unbox(_b_19541, _ctx); /*list<17082>*/
  _x21432 = kk_std_core__mlift17764_op(_y_176680, _x21433, _ctx); /*list<17082>*/
  return kk_std_core__list_box(_x21432, _ctx);
}

kk_std_core__list kk_std_core__mlift17765_op(kk_function_t f, kk_integer_t i, kk_std_core__list xx, kk_std_core__list yy, kk_box_t _y_176680, kk_context_t* _ctx) { /* forall<a,b,c,e> (f : (int, a, b) -> e c, i : int, xx : list<a>, yy : list<b>, c) -> e list<c> */ 
  kk_integer_t i0_17310 = kk_integer_add(i,(kk_integer_from_small(1)),kk_context()); /*int*/;
  kk_std_core__list x_18125 = kk_std_core__lift17286_zipwith_indexed(f, i0_17310, xx, yy, _ctx); /*list<17082>*/;
  if (kk_yielding(kk_context())) {
    kk_std_core__list_drop(x_18125, _ctx);
    kk_box_t _x21430 = kk_std_core_hnd_yield_extend(kk_std_core__new_mlift17765_op_fun21431(_y_176680, _ctx), _ctx); /*3860*/
    return kk_std_core__list_unbox(_x21430, _ctx);
  }
  {
    return kk_std_core__mlift17764_op(_y_176680, x_18125, _ctx);
  }
}
 
// lifted


// lift anonymous function
struct kk_std_core__lift17286_zipwith_indexed_fun21439__t {
  struct kk_function_s _base;
  kk_function_t f0;
  kk_integer_t i0;
  kk_std_core__list xx0;
  kk_std_core__list yy0;
};
static kk_box_t kk_std_core__lift17286_zipwith_indexed_fun21439(kk_function_t _fself, kk_box_t _b_19545, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17286_zipwith_indexed_fun21439(kk_function_t f0, kk_integer_t i0, kk_std_core__list xx0, kk_std_core__list yy0, kk_context_t* _ctx) {
  struct kk_std_core__lift17286_zipwith_indexed_fun21439__t* _self = kk_function_alloc_as(struct kk_std_core__lift17286_zipwith_indexed_fun21439__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17286_zipwith_indexed_fun21439, kk_context());
  _self->f0 = f0;
  _self->i0 = i0;
  _self->xx0 = xx0;
  _self->yy0 = yy0;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17286_zipwith_indexed_fun21439(kk_function_t _fself, kk_box_t _b_19545, kk_context_t* _ctx) {
  struct kk_std_core__lift17286_zipwith_indexed_fun21439__t* _self = kk_function_as(struct kk_std_core__lift17286_zipwith_indexed_fun21439__t*, _fself);
  kk_function_t f0 = _self->f0; /* (int, 17080, 17081) -> 17083 17082 */
  kk_integer_t i0 = _self->i0; /* int */
  kk_std_core__list xx0 = _self->xx0; /* list<17080> */
  kk_std_core__list yy0 = _self->yy0; /* list<17081> */
  kk_drop_match(_self, {kk_function_dup(f0);kk_integer_dup(i0);kk_std_core__list_dup(xx0);kk_std_core__list_dup(yy0);}, {}, _ctx)
  kk_std_core__list _x21440 = kk_std_core__mlift17765_op(f0, i0, xx0, yy0, _b_19545, _ctx); /*list<17082>*/
  return kk_std_core__list_box(_x21440, _ctx);
}


// lift anonymous function
struct kk_std_core__lift17286_zipwith_indexed_fun21442__t {
  struct kk_function_s _base;
  kk_box_t x1_18127;
};
static kk_box_t kk_std_core__lift17286_zipwith_indexed_fun21442(kk_function_t _fself, kk_box_t _b_19547, kk_context_t* _ctx);
static kk_function_t kk_std_core__new_lift17286_zipwith_indexed_fun21442(kk_box_t x1_18127, kk_context_t* _ctx) {
  struct kk_std_core__lift17286_zipwith_indexed_fun21442__t* _self = kk_function_alloc_as(struct kk_std_core__lift17286_zipwith_indexed_fun21442__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core__lift17286_zipwith_indexed_fun21442, kk_context());
  _self->x1_18127 = x1_18127;
  return &_self->_base;
}

static kk_box_t kk_std_core__lift17286_zipwith_indexed_fun21442(kk_function_t _fself, kk_box_t _b_19547, kk_context_t* _ctx) {
  struct kk_std_core__lift17286_zipwith_indexed_fun21442__t* _self = kk_function_as(struct kk_std_core__lift17286_zipwith_indexed_fun21442__t*, _fself);
  kk_box_t x1_18127 = _self->x1_18127; /* 17082 */
  kk_drop_match(_self, {kk_box_dup(x1_18127);}, {}, _ctx)
  kk_std_core__list _x21443;
  kk_std_core__list _x21444 = kk_std_core__list_unbox(_b_19547, _ctx); /*list<17082>*/
  _x21443 = kk_std_core__mlift17764_op(x1_18127, _x21444, _ctx); /*list<17082>*/
  return kk_std_core__list_box(_x21443, _ctx);
}

kk_std_core__list kk_std_core__lift17286_zipwith_indexed(kk_function_t f0, kk_integer_t i0, kk_std_core__list xs, kk_std_core__list ys, kk_context_t* _ctx) { /* forall<a,b,c,e> (f : (int, a, b) -> e c, i : int, xs : list<a>, ys : list<b>) -> e list<c> */ 
  if (kk_std_core__is_Cons(xs)) {
    struct kk_std_core_Cons* _con21434 = kk_std_core__as_Cons(xs);
    kk_box_t x0 = _con21434->head;
    kk_std_core__list xx0 = _con21434->tail;
    if (kk_likely(kk_std_core__list_is_unique(xs))) {
      kk_std_core__list_free(xs);
    }
    else {
      kk_box_dup(x0);
      kk_std_core__list_dup(xx0);
      kk_std_core__list_decref(xs, _ctx);
    }
    if (kk_std_core__is_Cons(ys)) {
      struct kk_std_core_Cons* _con21435 = kk_std_core__as_Cons(ys);
      kk_box_t y = _con21435->head;
      kk_std_core__list yy0 = _con21435->tail;
      kk_reuse_t _ru_20034 = kk_reuse_null; /*reuse*/;
      if (kk_likely(kk_std_core__list_is_unique(ys))) {
        _ru_20034 = (kk_std_core__list_reuse(ys));
      }
      else {
        kk_box_dup(y);
        kk_std_core__list_dup(yy0);
        kk_std_core__list_decref(ys, _ctx);
      }
      kk_box_t x1_18127;
      kk_function_t _x21437 = kk_function_dup(f0); /*(int, 17080, 17081) -> 17083 17082*/
      kk_integer_t _x21436 = kk_integer_dup(i0); /*int*/
      x1_18127 = kk_function_call(kk_box_t, (kk_function_t, kk_integer_t, kk_box_t, kk_box_t, kk_context_t*), _x21437, (_x21437, _x21436, x0, y, _ctx)); /*17082*/
      if (kk_yielding(kk_context())) {
        kk_reuse_drop(_ru_20034);
        kk_box_drop(x1_18127, _ctx);
        kk_box_t _x21438 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17286_zipwith_indexed_fun21439(f0, i0, xx0, yy0, _ctx), _ctx); /*3860*/
        return kk_std_core__list_unbox(_x21438, _ctx);
      }
      {
        kk_integer_t i0_173100 = kk_integer_add(i0,(kk_integer_from_small(1)),kk_context()); /*int*/;
        kk_std_core__list x2_18130 = kk_std_core__lift17286_zipwith_indexed(f0, i0_173100, xx0, yy0, _ctx); /*list<17082>*/;
        if (kk_yielding(kk_context())) {
          kk_reuse_drop(_ru_20034);
          kk_std_core__list_drop(x2_18130, _ctx);
          kk_box_t _x21441 = kk_std_core_hnd_yield_extend(kk_std_core__new_lift17286_zipwith_indexed_fun21442(x1_18127, _ctx), _ctx); /*3860*/
          return kk_std_core__list_unbox(_x21441, _ctx);
        }
        {
          return kk_std_core__new_Cons(_ru_20034, x1_18127, x2_18130, _ctx);
        }
      }
    }
    {
      kk_std_core__list_drop(xx0, _ctx);
      kk_box_drop(x0, _ctx);
      kk_integer_drop(i0, _ctx);
      kk_function_drop(f0, _ctx);
      return kk_std_core__new_Nil(_ctx);
    }
  }
  {
    kk_std_core__list_drop(ys, _ctx);
    kk_integer_drop(i0, _ctx);
    kk_function_drop(f0, _ctx);
    return kk_std_core__new_Nil(_ctx);
  }
}

// initialization
void kk_std_core__init(kk_context_t* _ctx){
  static bool _kk_initialized = false;
  if (_kk_initialized) return;
  _kk_initialized = true;
  kk_std_core_types__init(_ctx);
  kk_std_core_hnd__init(_ctx);
  #if defined(KK_CUSTOM_INIT)
    KK_CUSTOM_INIT (_ctx);
  #endif
  {
    kk_string_t _x20070;
    kk_define_string_literal(, _s20071, 8, "exn.core")
    _x20070 = kk_string_dup(_s20071); /*string*/
    kk_std_core__tag_exn = kk_std_core_hnd__new_Htag(_x20070, _ctx); /*std/core/hnd/htag<.hnd-exn>*/
  }
  {
    kk_std_core_redirect = kk_ref_alloc((kk_std_core_types__maybe_box(kk_std_core_types__new_Nothing(_ctx), _ctx)),kk_context()); /*ref<global,maybe<(string) -> console ()>>*/
  }
  {
    kk_std_core_maxListStack = kk_integer_from_small(50); /*int*/
  }
  {
    kk_std_core_trace_enabled = kk_ref_alloc((kk_bool_box(true)),kk_context()); /*ref<global,bool>*/
  }
  {
    kk_std_core_unique_count = kk_ref_alloc((kk_integer_box(kk_integer_from_small(0))),kk_context()); /*ref<global,int>*/
  }
  {
    kk_string_t _x20802 = kk_string_empty(); /*string*/
    kk_ssize_t _x20804 = ((kk_ssize_t)0); /*ssize_t*/
    kk_ssize_t _x20805 = ((kk_ssize_t)0); /*ssize_t*/
    kk_std_core_empty = kk_std_core__new_Sslice(_x20802, _x20804, _x20805, _ctx); /*sslice*/
  }
  {
    kk_string_t _x21025 = kk_string_empty(); /*string*/
    kk_ssize_t _x21027 = ((kk_ssize_t)-1); /*ssize_t*/
    kk_ssize_t _x21028 = ((kk_ssize_t)0); /*ssize_t*/
    kk_std_core_invalid = kk_std_core__new_Sslice(_x21025, _x21027, _x21028, _ctx); /*sslice*/
  }
}

// termination
void kk_std_core__done(kk_context_t* _ctx){
  static bool _kk_done = false;
  if (_kk_done) return;
  _kk_done = true;
  #if defined(KK_CUSTOM_DONE)
    KK_CUSTOM_DONE (_ctx);
  #endif
  kk_std_core__sslice_drop(kk_std_core_invalid, _ctx);
  kk_std_core__sslice_drop(kk_std_core_empty, _ctx);
  kk_ref_drop(kk_std_core_unique_count, _ctx);
  kk_ref_drop(kk_std_core_trace_enabled, _ctx);
  kk_integer_drop(kk_std_core_maxListStack, _ctx);
  kk_ref_drop(kk_std_core_redirect, _ctx);
  kk_std_core_hnd__htag_drop(kk_std_core__tag_exn, _ctx);
  kk_std_core_hnd__done(_ctx);
  kk_std_core_types__done(_ctx);
}
