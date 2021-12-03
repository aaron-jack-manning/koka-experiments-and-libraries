// Koka generated module: "std/core/hnd", koka version: 2.3.2, platform: 64-bit
#include "std_core_hnd.h"
/*---------------------------------------------------------------------------
  Copyright 2020-2021, Microsoft Research, Daan Leijen.

  This is free software; you can redistribute it and/or modify it under the
  terms of the Apache License, Version 2.0. A copy of the License can be
  found in the LICENSE file at the root of this distribution.
---------------------------------------------------------------------------*/
/*
typedef datatype_t kk_std_core_hnd__ev;
struct kk_std_core_hnd_Ev {
  kk_std_core_hnd__htag htag;
  kk_box_t hnd;
  int32_t cfc;  // control flow context
  kk_std_core_hnd__evv hevv;
  kk_std_core_hnd__marker marker;
};
*/

static kk_evv_vector_t kk_evv_vector_alloc(kk_ssize_t length, int32_t cfc, kk_context_t* ctx) {
  kk_assert_internal(length>=0);
  kk_evv_vector_t v = (kk_evv_vector_t)kk_block_alloc(kk_ssizeof(struct kk_evv_vector_s) + (length-1)*kk_ssizeof(void*), length + 1 /* cfc */, KK_TAG_EVV_VECTOR, ctx);
  v->cfc = kk_integer_from_int32(cfc,ctx);
  return v;
}

static kk_std_core_hnd__ev* kk_evv_vector_buf(kk_evv_vector_t vec, kk_ssize_t* len) {
  if (len != NULL) { *len = kk_block_scan_fsize(&vec->_block) - 1; }
  return &vec->vec[0];
}

static kk_std_core_hnd__ev* kk_evv_as_vec(kk_evv_t evv, kk_ssize_t* len, kk_std_core_hnd__ev* single) {
  if (kk_evv_is_vector(evv)) {
    kk_evv_vector_t vec = kk_evv_as_vector(evv);
    *len = kk_block_scan_fsize(&vec->_block) - 1;
    return &vec->vec[0];
  }
  else {
    // single evidence
    *single = kk_evv_as_ev(evv);
    *len = 1;
    return single;
  }
}

kk_std_core_hnd__ev kk_ev_none(kk_context_t* ctx) {
  static kk_std_core_hnd__ev ev_none_singleton;
  if (ev_none_singleton==NULL) {
    ev_none_singleton = kk_std_core_hnd__new_Ev(
      kk_reuse_null,
      kk_std_core_hnd__new_Htag(kk_string_empty(),ctx), // tag ""
      kk_std_core_hnd__new_Marker(0,ctx),               // marker 0
      kk_box_null,                                      // no handler
      -1,                                               // bot
      kk_evv_empty(ctx),
      ctx
    );
  }
  return kk_std_core_hnd__ev_dup(ev_none_singleton);
}


kk_ssize_t kk_evv_index( struct kk_std_core_hnd_Htag htag, kk_context_t* ctx ) {
  // todo: drop htag?
  kk_ssize_t len;
  kk_std_core_hnd__ev single;
  kk_std_core_hnd__ev* vec = kk_evv_as_vec(ctx->evv,&len,&single);
  for(kk_ssize_t i = 0; i < len; i++) {
    struct kk_std_core_hnd_Ev* ev = kk_std_core_hnd__as_Ev(vec[i]);
    if (kk_string_cmp_borrow(htag.tagname,ev->htag.tagname) <= 0) return i; // break on insertion point
  }
  //string_t evvs = kk_evv_show(dup_datatype_as(kk_evv_t,ctx->evv),ctx);
  //fatal_error(EFAULT,"cannot find tag '%s' in: %s", string_cbuf_borrow(htag.htag), string_cbuf_borrow(evvs));
  //drop_string_t(evvs,ctx);
  return len;
}

kk_std_core_hnd__ev kk_evv_lookup( struct kk_std_core_hnd_Htag htag, kk_context_t* ctx ) {
  // todo: drop htag
  kk_ssize_t idx = kk_evv_index(htag,ctx);
  return kk_evv_at(idx,ctx);
}

static inline int32_t kk_cfc_lub(int32_t cfc1, int32_t cfc2) {
  if (cfc1 < 0) return cfc2;
  else if (cfc1+cfc2 == 1) return 2;
  else if (cfc1>cfc2) return cfc1;
  else return cfc2;
}

static inline struct kk_std_core_hnd_Ev* kk_evv_as_Ev( kk_evv_t evv ) {
  return kk_std_core_hnd__as_Ev(kk_evv_as_ev(evv));
}


static int32_t kk_evv_cfc_of_borrow(kk_evv_t evv, kk_context_t* ctx) {
  if (kk_evv_is_vector(evv)) {
    kk_evv_vector_t vec = kk_evv_as_vector(evv);
    return kk_integer_clamp32_borrow(vec->cfc);
  }
  else {
    struct kk_std_core_hnd_Ev* ev = kk_evv_as_Ev(evv);
    return ev->cfc;
  }
}

int32_t kk_evv_cfc(kk_context_t* ctx) {
  return kk_evv_cfc_of_borrow(ctx->evv,ctx);
}

static void kk_evv_update_cfc_borrow(kk_evv_t evv, int32_t cfc, kk_context_t* ctx) {
  kk_assert_internal(!kk_evv_is_empty(evv)); // should never happen (as named handlers are always in some context)
  if (kk_evv_is_vector(evv)) {
    kk_evv_vector_t vec = kk_evv_as_vector(evv);
    vec->cfc = kk_integer_from_int32(kk_cfc_lub(kk_integer_clamp32_borrow(vec->cfc),cfc), ctx);
  }
  else {
    struct kk_std_core_hnd_Ev* ev = kk_evv_as_Ev(evv);
    ev->cfc = kk_cfc_lub(ev->cfc,cfc);
  }
}

kk_evv_t kk_evv_insert(kk_evv_t evvd, kk_std_core_hnd__ev evd, kk_context_t* ctx) {
  struct kk_std_core_hnd_Ev* ev = kk_std_core_hnd__as_Ev(evd);
  // update ev with parent evidence vector (either at init, or due to non-scoped resumptions)
  int32_t marker = ev->marker.m;
  if (marker==0) { kk_std_core_hnd__ev_drop(evd,ctx); return evvd; } // ev-none
  kk_evv_drop(ev->hevv,ctx);
  ev->hevv = kk_evv_dup(evvd);
  if (marker<0) { // negative marker is used for named evidence; this means this evidence should not be inserted into the evidence vector
    kk_evv_update_cfc_borrow(evvd,ev->cfc,ctx); // update cfc in-place for named evidence
    kk_std_core_hnd__ev_drop(evd,ctx);
    return evvd;
  }
  // for regular handler evidence, insert ev
  kk_ssize_t n;
  kk_std_core_hnd__ev single;
  kk_std_core_hnd__ev* const evv1 = kk_evv_as_vec(evvd, &n, &single);
  if (n == 0) {
    // use ev directly as the evidence vector
    kk_evv_drop(evvd, ctx);
    return &evd->_block;
  }
  else {
    // create evidence vector
    const int32_t cfc = kk_cfc_lub(kk_evv_cfc_of_borrow(evvd, ctx), ev->cfc);
    ev->cfc = cfc; // update in place
    kk_evv_vector_t vec2 = kk_evv_vector_alloc(n+1, cfc, ctx);
    kk_std_core_hnd__ev* const evv2 = kk_evv_vector_buf(vec2, NULL);
    kk_ssize_t i;
    for (i = 0; i < n; i++) {
      struct kk_std_core_hnd_Ev* ev1 = kk_std_core_hnd__as_Ev(evv1[i]);
      if (kk_string_cmp_borrow(ev->htag.tagname, ev1->htag.tagname) <= 0) break;
      evv2[i] = kk_std_core_hnd__ev_dup(&ev1->_base);
    }
    evv2[i] = evd;
    for (; i < n; i++) {
      evv2[i+1] = kk_std_core_hnd__ev_dup(evv1[i]);
    }
    kk_evv_drop(evvd, ctx);  // assigned to evidence already
    return &vec2->_block;
  }
}

kk_evv_t kk_evv_delete(kk_evv_t evvd, kk_ssize_t index, bool behind, kk_context_t* ctx) {
  kk_ssize_t n;
  kk_std_core_hnd__ev single;
  const kk_std_core_hnd__ev* evv1 = kk_evv_as_vec(evvd, &n, &single);
  if (n <= 1) {
    kk_evv_drop(evvd,ctx);
    return kk_evv_total(ctx);
  }
  if (behind) index++;
  kk_assert_internal(index < n);
  // todo: copy without dupping (and later dropping) when possible
  const int32_t cfc1 = kk_evv_cfc_of_borrow(evvd,ctx);
  kk_evv_vector_t const vec2 = kk_evv_vector_alloc(n-1,cfc1,ctx);
  kk_std_core_hnd__ev* const evv2 = kk_evv_vector_buf(vec2,NULL);
  kk_ssize_t i;
  for(i = 0; i < index; i++) {
    evv2[i] = kk_std_core_hnd__ev_dup(evv1[i]);
  }
  for(; i < n-1; i++) {
    evv2[i] = kk_std_core_hnd__ev_dup(evv1[i+1]);
  }
  struct kk_std_core_hnd_Ev* ev = kk_std_core_hnd__as_Ev(evv1[index]);
  if (ev->cfc >= cfc1) {
    int32_t cfc = kk_std_core_hnd__as_Ev(evv2[0])->cfc;
    for(i = 1; i < n-1; i++) {
      cfc = kk_cfc_lub(cfc,kk_std_core_hnd__as_Ev(evv2[i])->cfc);
    }
    vec2->cfc = kk_integer_from_int32(cfc,ctx);
  }
  kk_evv_drop(evvd,ctx);
  return &vec2->_block;
}

kk_evv_t kk_evv_create(kk_evv_t evv1, kk_vector_t indices, kk_context_t* ctx) {
  kk_ssize_t len;
  kk_box_t* elems = kk_vector_buf_borrow(indices,&len); // borrows
  kk_evv_vector_t evv2 = kk_evv_vector_alloc(len,kk_evv_cfc_of_borrow(evv1,ctx),ctx);
  kk_std_core_hnd__ev* buf2 = kk_evv_vector_buf(evv2,NULL);
  kk_assert_internal(kk_evv_is_vector(evv1));
  kk_ssize_t len1;
  kk_std_core_hnd__ev single;
  kk_std_core_hnd__ev* buf1 = kk_evv_as_vec(evv1,&len1,&single);
  for(kk_ssize_t i = 0; i < len; i++) {
    kk_ssize_t idx = kk_ssize_unbox(elems[i],ctx);
    kk_assert_internal(idx < len1);
    buf2[i] = kk_std_core_hnd__ev_dup( buf1[idx] );
  }
  kk_vector_drop(indices,ctx);
  kk_evv_drop(evv1,ctx);
  return &evv2->_block;
}

kk_evv_t kk_evv_swap_create( kk_vector_t indices, kk_context_t* ctx ) {
  kk_ssize_t len;
  kk_box_t* vec = kk_vector_buf_borrow(indices,&len);
  if (len==0) {
    kk_vector_drop(indices,ctx);
    return kk_evv_swap_create0(ctx);
  }
  if (len==1) {
    kk_ssize_t i = kk_ssize_unbox(vec[0],ctx);
    kk_vector_drop(indices,ctx);
    return kk_evv_swap_create1(i,ctx);
  }
  return kk_evv_swap( kk_evv_create(kk_evv_dup(ctx->evv),indices,ctx), ctx );
}


kk_string_t kk_evv_show(kk_evv_t evv, kk_context_t* ctx) {
  return kk_string_alloc_dup_valid_utf8("(not yet implemented: kk_evv_show)",ctx);
}


/*-----------------------------------------------------------------------
  Compose continuations
-----------------------------------------------------------------------*/

struct kcompose_fun_s {
  struct kk_function_s _base;
  kk_box_t      count;
  kk_function_t conts[1];
};

// kleisli composition of continuations
static kk_box_t kcompose( kk_function_t fself, kk_box_t x, kk_context_t* ctx) {
  struct kcompose_fun_s* self = kk_function_as(struct kcompose_fun_s*,fself);
  kk_intx_t count = kk_int_unbox(self->count);
  kk_function_t* conts = &self->conts[0];
  // call each continuation in order
  for(kk_intx_t i = 0; i < count; i++) {
    // todo: take uniqueness of fself into account to avoid dup_function
    kk_function_t f = kk_function_dup(conts[i]);
    x = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, x, ctx));
    if (kk_yielding(ctx)) {
      // if yielding, `yield_next` all continuations that still need to be done
      while(++i < count) {
        // todo: if fself is unique, we could copy without dup?
        kk_yield_extend(kk_function_dup(conts[i]),ctx);
      }
      kk_function_drop(fself,ctx);
      kk_box_drop(x,ctx);     // still drop even though we yield as it may release a boxed value type?
      return kk_box_any(ctx); // return yielding
    }
  }
  kk_function_drop(fself,ctx);
  return x;
}

static kk_function_t new_kcompose( kk_function_t* conts, kk_ssize_t count, kk_context_t* ctx ) {
  if (count==0) return kk_function_id(ctx);
  if (count==1) return conts[0];
  struct kcompose_fun_s* f = kk_block_as(struct kcompose_fun_s*,
                               kk_block_alloc(kk_ssizeof(struct kcompose_fun_s) - kk_ssizeof(kk_function_t) + (count*kk_ssizeof(kk_function_t)),
                                 2 + count /* scan size */, KK_TAG_FUNCTION, ctx));
  f->_base.fun = kk_cfun_ptr_box(&kcompose,ctx);
  f->count = kk_int_box(count);
  kk_memcpy(f->conts, conts, count * kk_ssizeof(kk_function_t));
  return (&f->_base);
}

/*-----------------------------------------------------------------------
  Yield extension
-----------------------------------------------------------------------*/

kk_box_t kk_yield_extend( kk_function_t next, kk_context_t* ctx ) {
  kk_yield_t* yield = &ctx->yield;
  kk_assert_internal(kk_yielding(ctx));  // cannot extend if not yielding
  if (kk_unlikely(kk_yielding_final(ctx))) {
    // todo: can we optimize this so `next` is never allocated in the first place?
    kk_function_drop(next,ctx); // ignore extension if never resuming
  }
  else {
    if (kk_unlikely(yield->conts_count >= KK_YIELD_CONT_MAX)) {
      // alloc a function to compose all continuations in the array
      kk_function_t comp = new_kcompose( yield->conts, yield->conts_count, ctx );
      yield->conts[0] = comp;
      yield->conts_count = 1;
    }
    yield->conts[yield->conts_count++] = next;
  }
  return kk_box_any(ctx);
}

// cont_apply: \x -> f(cont,x)
struct cont_apply_fun_s {
  struct kk_function_s _base;
  kk_function_t f;
  kk_function_t cont;
};

static kk_box_t cont_apply( kk_function_t fself, kk_box_t x, kk_context_t* ctx ) {
  struct cont_apply_fun_s* self = kk_function_as(struct cont_apply_fun_s*, fself);
  kk_function_t f = self->f;
  kk_function_t cont = self->cont;
  kk_drop_match(self,{kk_function_dup(f);kk_function_dup(cont);},{},ctx);
  return kk_function_call( kk_box_t, (kk_function_t, kk_function_t, kk_box_t, kk_context_t* ctx), f, (f, cont, x, ctx));
}

static kk_function_t kk_new_cont_apply( kk_function_t f, kk_function_t cont, kk_context_t* ctx ) {
  struct cont_apply_fun_s* self = kk_function_alloc_as(struct cont_apply_fun_s, 3, ctx);
  self->_base.fun = kk_cfun_ptr_box(&cont_apply,ctx);
  self->f = f;
  self->cont = cont;
  return (&self->_base);
}

// Unlike `yield_extend`, `yield_cont` gets access to the current continuation. This is used in `yield_prompt`.
kk_box_t kk_yield_cont( kk_function_t f, kk_context_t* ctx ) {
  kk_yield_t* yield = &ctx->yield;
  kk_assert_internal(kk_yielding(ctx)); // cannot extend if not yielding
  if (kk_unlikely(kk_yielding_final(ctx))) {
    kk_function_drop(f,ctx); // ignore extension if never resuming
  }
  else {
    kk_function_t cont = new_kcompose(yield->conts, yield->conts_count, ctx);
    yield->conts_count = 1;
    yield->conts[0] = kk_new_cont_apply(f, cont, ctx);
  }
  return kk_box_any(ctx);
}

kk_function_t kk_yield_to( struct kk_std_core_hnd_Marker m, kk_function_t clause, kk_context_t* ctx ) {
  kk_yield_t* yield = &ctx->yield;
  kk_assert_internal(!kk_yielding(ctx)); // already yielding
  ctx->yielding = KK_YIELD_NORMAL;
  yield->marker = m.m;
  yield->clause = clause;
  yield->conts_count = 0;
  return kk_basetype_unbox_as(kk_function_t,kk_box_any(ctx));
}

kk_box_t kk_yield_final( struct kk_std_core_hnd_Marker m, kk_function_t clause, kk_context_t* ctx ) {
  kk_yield_to(m,clause,ctx);
  ctx->yielding = KK_YIELD_FINAL;
  return kk_box_any(ctx);
}

kk_box_t kk_fatal_resume_final(kk_context_t* ctx) {
  kk_fatal_error(EFAULT,"trying to resume a finalized resumption");
  return kk_box_any(ctx);
}

static kk_box_t _fatal_resume_final(kk_function_t self, kk_context_t* ctx) {
  kk_function_drop(self,ctx);
  return kk_fatal_resume_final(ctx);
}
static kk_function_t fun_fatal_resume_final(kk_context_t* ctx) {
  kk_define_static_function(f,_fatal_resume_final,ctx);
  return kk_function_dup(f);
}


struct kk_std_core_hnd_yld_s kk_yield_prompt( struct kk_std_core_hnd_Marker m, kk_context_t* ctx ) {
  kk_yield_t* yield = &ctx->yield;
  if (ctx->yielding == KK_YIELD_NONE) {
    return kk_std_core_hnd__new_Pure(ctx);
  }
  else if (yield->marker != m.m) {
    return (ctx->yielding == KK_YIELD_FINAL ? kk_std_core_hnd__new_YieldingFinal(ctx) : kk_std_core_hnd__new_Yielding(ctx));
  }
  else {
    kk_function_t cont = (ctx->yielding == KK_YIELD_FINAL ? fun_fatal_resume_final(ctx) : new_kcompose(yield->conts, yield->conts_count, ctx));
    kk_function_t clause = yield->clause;
    ctx->yielding = KK_YIELD_NONE;
    #ifndef NDEBUG
    kk_memset(yield,0,kk_ssizeof(kk_yield_t));
    #endif
    return kk_std_core_hnd__new_Yield(clause, cont, ctx);
  }
}

kk_unit_t  kk_evv_guard(kk_evv_t evv, kk_context_t* ctx) {
  bool eq = (ctx->evv == evv);
  kk_evv_drop(evv,ctx);
  if (!eq) {
    // todo: improve error message with diagnostics
    kk_fatal_error(EFAULT,"trying to resume outside the (handler) scope of the original handler");
  }
  return kk_Unit;
}

typedef struct yield_info_s {
  struct kk_std_core_hnd__yield_info_s _base;
  kk_function_t clause;
  kk_function_t conts[KK_YIELD_CONT_MAX];
  kk_ssize_t    conts_count;
  int32_t       marker;
  int8_t        yielding;
}* yield_info_t;

kk_std_core_hnd__yield_info kk_yield_capture(kk_context_t* ctx) {
  kk_assert_internal(kk_yielding(ctx));
  yield_info_t yld = kk_block_alloc_as(struct yield_info_s, 1 + KK_YIELD_CONT_MAX, (kk_tag_t)1, ctx);
  yld->clause = ctx->yield.clause;
  kk_ssize_t i = 0;
  for( ; i < ctx->yield.conts_count; i++) {
    yld->conts[i] = ctx->yield.conts[i];
  }
  for( ; i < KK_YIELD_CONT_MAX; i++) {
    yld->conts[i] = kk_function_null(ctx);
  }
  yld->conts_count = ctx->yield.conts_count;
  yld->marker = ctx->yield.marker;
  yld->yielding = ctx->yielding;
  ctx->yielding = 0;
  ctx->yield.conts_count = 0;
  return kk_datatype_from_base(&yld->_base);
}

kk_box_t kk_yield_reyield( kk_std_core_hnd__yield_info yldinfo, kk_context_t* ctx) {
  kk_assert_internal(!kk_yielding(ctx));
  yield_info_t yld = kk_datatype_as_assert(yield_info_t, yldinfo, (kk_tag_t)1);
  ctx->yield.clause = kk_function_dup(yld->clause);
  ctx->yield.marker = yld->marker;
  ctx->yield.conts_count = yld->conts_count;
  ctx->yielding = yld->yielding;
  for(kk_ssize_t i = 0; i < yld->conts_count; i++) {
    ctx->yield.conts[i] = kk_function_dup(yld->conts[i]);
  }
  kk_constructor_drop(yld,ctx);
  return kk_box_any(ctx);
}


kk_std_core_hnd__ev kk_std_core_hnd__copy_2(kk_std_core_hnd__ev _this, kk_std_core_types__optional htag0, kk_std_core_hnd__marker marker, kk_box_t hnd, kk_std_core_types__optional cfc0, kk_evv_t hevv, kk_context_t* _ctx) { /* forall<a,e,b> (ev<a>, htag : optional<htag<a>>, marker : marker<e,b>, hnd : a<e,b>, cfc : optional<cfc>, hevv : evv<e>) -> ev<a> */ 
  kk_std_core_hnd__htag _x10627;
  if (kk_std_core_types__is_Optional(htag0)) {
    kk_box_t _box_x10325 = htag0._cons.Optional.value;
    kk_std_core_hnd__htag _htag_2053 = kk_std_core_hnd__htag_unbox(_box_x10325, NULL);
    kk_std_core_hnd__htag_dup(_htag_2053);
    kk_std_core_types__optional_drop(htag0, _ctx);
    _x10627 = _htag_2053; /*std/core/hnd/htag<2079>*/
    goto _match10628;
  }
  {
    struct kk_std_core_hnd_Ev* _con10630 = kk_std_core_hnd__as_Ev(_this);
    kk_std_core_hnd__htag _x = _con10630->htag;
    kk_std_core_hnd__htag_dup(_x);
    _x10627 = _x; /*std/core/hnd/htag<2079>*/
  }
  _match10628: ;
  int32_t _x10631;
  if (kk_std_core_types__is_Optional(cfc0)) {
    kk_box_t _box_x10326 = cfc0._cons.Optional.value;
    int32_t _cfc_2060 = kk_int32_unbox(_box_x10326, NULL);
    kk_std_core_types__optional_drop(cfc0, _ctx);
    kk_std_core_hnd__ev_dropn(_this, ((int32_t)KI32(3)), _ctx);
    _x10631 = _cfc_2060; /*std/core/hnd/cfc*/
    goto _match10632;
  }
  {
    struct kk_std_core_hnd_Ev* _con10634 = kk_std_core_hnd__as_Ev(_this);
    kk_std_core_hnd__htag _pat01 = _con10634->htag;
    kk_box_t _pat21 = _con10634->hnd;
    int32_t _x0 = _con10634->cfc;
    kk_evv_t _pat30 = _con10634->hevv;
    if (kk_likely(kk_std_core_hnd__ev_is_unique(_this))) {
      kk_evv_drop(_pat30, _ctx);
      kk_box_drop(_pat21, _ctx);
      kk_std_core_hnd__htag_drop(_pat01, _ctx);
      kk_std_core_hnd__ev_free(_this);
    }
    else {
      kk_std_core_hnd__ev_decref(_this, _ctx);
    }
    _x10631 = _x0; /*std/core/hnd/cfc*/
  }
  _match10632: ;
  return kk_std_core_hnd__new_Ev(kk_reuse_null, _x10627, marker, hnd, _x10631, hevv, _ctx);
}


// lift anonymous function
struct kk_std_core_hnd__copy_fun10636__t_3 {
  struct kk_function_s _base;
  kk_box_t _fun_unbox_x10332;
};
static kk_box_t kk_std_core_hnd__copy_fun10636_3(kk_function_t _fself, kk_std_core_hnd__marker _b_10336, kk_std_core_hnd__ev _b_10337, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_copy_fun10636_3(kk_box_t _fun_unbox_x10332, kk_context_t* _ctx) {
  struct kk_std_core_hnd__copy_fun10636__t_3* _self = kk_function_alloc_as(struct kk_std_core_hnd__copy_fun10636__t_3, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__copy_fun10636_3, kk_context());
  _self->_fun_unbox_x10332 = _fun_unbox_x10332;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__copy_fun10636_3(kk_function_t _fself, kk_std_core_hnd__marker _b_10336, kk_std_core_hnd__ev _b_10337, kk_context_t* _ctx) {
  struct kk_std_core_hnd__copy_fun10636__t_3* _self = kk_function_as(struct kk_std_core_hnd__copy_fun10636__t_3*, _fself);
  kk_box_t _fun_unbox_x10332 = _self->_fun_unbox_x10332; /* 110 */
  kk_drop_match(_self, {kk_box_dup(_fun_unbox_x10332);}, {}, _ctx)
  kk_function_t _x10637 = kk_function_unbox(_fun_unbox_x10332); /*(10333, 10334) -> 2172 10335*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), _x10637, (_x10637, kk_std_core_hnd__marker_box(_b_10336, _ctx), kk_std_core_hnd__ev_box(_b_10337, _ctx), _ctx));
}

kk_std_core_hnd__clause0 kk_std_core_hnd__copy_3(kk_std_core_hnd__clause0 _this, kk_std_core_types__optional clause0, kk_context_t* _ctx) { /* forall<a,b,e,c> (clause0<a,b,e,c>, clause : optional<(marker<e,c>, ev<b>) -> e a>) -> clause0<a,b,e,c> */ 
  kk_function_t _x10635;
  if (kk_std_core_types__is_Optional(clause0)) {
    kk_box_t _fun_unbox_x10332 = clause0._cons.Optional.value;
    kk_std_core_hnd__clause0_drop(_this, _ctx);
    _x10635 = kk_std_core_hnd__new_copy_fun10636_3(_fun_unbox_x10332, _ctx); /*(std/core/hnd/marker<2172,2173>, std/core/hnd/ev<2171>) -> 2172 10335*/
  }
  else {
    kk_function_t _x = _this.clause;
    _x10635 = _x; /*(std/core/hnd/marker<2172,2173>, std/core/hnd/ev<2171>) -> 2172 10335*/
  }
  return kk_std_core_hnd__new_Clause0(_x10635, _ctx);
}


// lift anonymous function
struct kk_std_core_hnd__copy_fun10639__t_4 {
  struct kk_function_s _base;
  kk_box_t _fun_unbox_x10345;
};
static kk_box_t kk_std_core_hnd__copy_fun10639_4(kk_function_t _fself, kk_std_core_hnd__marker _b_10350, kk_std_core_hnd__ev _b_10351, kk_box_t _b_10352, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_copy_fun10639_4(kk_box_t _fun_unbox_x10345, kk_context_t* _ctx) {
  struct kk_std_core_hnd__copy_fun10639__t_4* _self = kk_function_alloc_as(struct kk_std_core_hnd__copy_fun10639__t_4, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__copy_fun10639_4, kk_context());
  _self->_fun_unbox_x10345 = _fun_unbox_x10345;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__copy_fun10639_4(kk_function_t _fself, kk_std_core_hnd__marker _b_10350, kk_std_core_hnd__ev _b_10351, kk_box_t _b_10352, kk_context_t* _ctx) {
  struct kk_std_core_hnd__copy_fun10639__t_4* _self = kk_function_as(struct kk_std_core_hnd__copy_fun10639__t_4*, _fself);
  kk_box_t _fun_unbox_x10345 = _self->_fun_unbox_x10345; /* 110 */
  kk_drop_match(_self, {kk_box_dup(_fun_unbox_x10345);}, {}, _ctx)
  kk_function_t _x10640 = kk_function_unbox(_fun_unbox_x10345); /*(10346, 10347, 10348) -> 2378 10349*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), _x10640, (_x10640, kk_std_core_hnd__marker_box(_b_10350, _ctx), kk_std_core_hnd__ev_box(_b_10351, _ctx), _b_10352, _ctx));
}

kk_std_core_hnd__clause1 kk_std_core_hnd__copy_4(kk_std_core_hnd__clause1 _this, kk_std_core_types__optional clause0, kk_context_t* _ctx) { /* forall<a,b,c,e,d> (clause1<a,b,c,e,d>, clause : optional<(marker<e,d>, ev<c>, a) -> e b>) -> clause1<a,b,c,e,d> */ 
  kk_function_t _x10638;
  if (kk_std_core_types__is_Optional(clause0)) {
    kk_box_t _fun_unbox_x10345 = clause0._cons.Optional.value;
    kk_std_core_hnd__clause1_drop(_this, _ctx);
    _x10638 = kk_std_core_hnd__new_copy_fun10639_4(_fun_unbox_x10345, _ctx); /*(std/core/hnd/marker<2378,2379>, std/core/hnd/ev<2377>, 2375) -> 2378 10349*/
  }
  else {
    kk_function_t _x = _this.clause;
    _x10638 = _x; /*(std/core/hnd/marker<2378,2379>, std/core/hnd/ev<2377>, 2375) -> 2378 10349*/
  }
  return kk_std_core_hnd__new_Clause1(_x10638, _ctx);
}


// lift anonymous function
struct kk_std_core_hnd__copy_fun10642__t_5 {
  struct kk_function_s _base;
  kk_box_t _fun_unbox_x10362;
};
static kk_box_t kk_std_core_hnd__copy_fun10642_5(kk_function_t _fself, kk_std_core_hnd__marker _b_10368, kk_std_core_hnd__ev _b_10369, kk_box_t _b_10370, kk_box_t _b_10371, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_copy_fun10642_5(kk_box_t _fun_unbox_x10362, kk_context_t* _ctx) {
  struct kk_std_core_hnd__copy_fun10642__t_5* _self = kk_function_alloc_as(struct kk_std_core_hnd__copy_fun10642__t_5, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__copy_fun10642_5, kk_context());
  _self->_fun_unbox_x10362 = _fun_unbox_x10362;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__copy_fun10642_5(kk_function_t _fself, kk_std_core_hnd__marker _b_10368, kk_std_core_hnd__ev _b_10369, kk_box_t _b_10370, kk_box_t _b_10371, kk_context_t* _ctx) {
  struct kk_std_core_hnd__copy_fun10642__t_5* _self = kk_function_as(struct kk_std_core_hnd__copy_fun10642__t_5*, _fself);
  kk_box_t _fun_unbox_x10362 = _self->_fun_unbox_x10362; /* 110 */
  kk_drop_match(_self, {kk_box_dup(_fun_unbox_x10362);}, {}, _ctx)
  kk_function_t _x10643 = kk_function_unbox(_fun_unbox_x10362); /*(10363, 10364, 10365, 10366) -> 2679 10367*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), _x10643, (_x10643, kk_std_core_hnd__marker_box(_b_10368, _ctx), kk_std_core_hnd__ev_box(_b_10369, _ctx), _b_10370, _b_10371, _ctx));
}

kk_std_core_hnd__clause2 kk_std_core_hnd__copy_5(kk_std_core_hnd__clause2 _this, kk_std_core_types__optional clause0, kk_context_t* _ctx) { /* forall<a,b,c,d,e,a1> (clause2<a,b,c,d,e,a1>, clause : optional<(marker<e,a1>, ev<d>, a, b) -> e c>) -> clause2<a,b,c,d,e,a1> */ 
  kk_function_t _x10641;
  if (kk_std_core_types__is_Optional(clause0)) {
    kk_box_t _fun_unbox_x10362 = clause0._cons.Optional.value;
    kk_std_core_hnd__clause2_drop(_this, _ctx);
    _x10641 = kk_std_core_hnd__new_copy_fun10642_5(_fun_unbox_x10362, _ctx); /*(std/core/hnd/marker<2679,2680>, std/core/hnd/ev<2678>, 2675, 2676) -> 2679 10367*/
  }
  else {
    kk_function_t _x = _this.clause;
    _x10641 = _x; /*(std/core/hnd/marker<2679,2680>, std/core/hnd/ev<2678>, 2675, 2676) -> 2679 10367*/
  }
  return kk_std_core_hnd__new_Clause2(_x10641, _ctx);
}


// lift anonymous function
struct kk_std_core_hnd__copy_fun10645__t_6 {
  struct kk_function_s _base;
  kk_box_t _fun_unbox_x10375;
};
static kk_box_t kk_std_core_hnd__copy_fun10645_6(kk_function_t _fself, kk_std_core_hnd__resume_result _b_10378, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_copy_fun10645_6(kk_box_t _fun_unbox_x10375, kk_context_t* _ctx) {
  struct kk_std_core_hnd__copy_fun10645__t_6* _self = kk_function_alloc_as(struct kk_std_core_hnd__copy_fun10645__t_6, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__copy_fun10645_6, kk_context());
  _self->_fun_unbox_x10375 = _fun_unbox_x10375;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__copy_fun10645_6(kk_function_t _fself, kk_std_core_hnd__resume_result _b_10378, kk_context_t* _ctx) {
  struct kk_std_core_hnd__copy_fun10645__t_6* _self = kk_function_as(struct kk_std_core_hnd__copy_fun10645__t_6*, _fself);
  kk_box_t _fun_unbox_x10375 = _self->_fun_unbox_x10375; /* 110 */
  kk_drop_match(_self, {kk_box_dup(_fun_unbox_x10375);}, {}, _ctx)
  kk_function_t _x10646 = kk_function_unbox(_fun_unbox_x10375); /*(10376) -> 2893 10377*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x10646, (_x10646, kk_std_core_hnd__resume_result_box(_b_10378, _ctx), _ctx));
}

kk_std_core_hnd__resume_context kk_std_core_hnd__copy_6(kk_std_core_hnd__resume_context _this, kk_std_core_types__optional k0, kk_context_t* _ctx) { /* forall<a,e,e1,b> (resume-context<a,e,e1,b>, k : optional<(resume-result<a,b>) -> e b>) -> resume-context<a,e,e1,b> */ 
  kk_function_t _x10644;
  if (kk_std_core_types__is_Optional(k0)) {
    kk_box_t _fun_unbox_x10375 = k0._cons.Optional.value;
    kk_std_core_hnd__resume_context_drop(_this, _ctx);
    _x10644 = kk_std_core_hnd__new_copy_fun10645_6(_fun_unbox_x10375, _ctx); /*(std/core/hnd/resume-result<2892,2895>) -> 2893 10377*/
  }
  else {
    kk_function_t _x = _this.k;
    _x10644 = _x; /*(std/core/hnd/resume-result<2892,2895>) -> 2893 10377*/
  }
  return kk_std_core_hnd__new_Resume_context(_x10644, _ctx);
}
 
// (dynamically) find evidence insertion/deletion index in the evidence vector

kk_ssize_t kk_std_core_hnd__evv_index(kk_std_core_hnd__htag htag0, kk_context_t* _ctx) { /* forall<e,a> (htag : htag<a>) -> e ev-index */ 
  return kk_evv_index(htag0,kk_context());
}

bool kk_std_core_hnd__evv_is_affine(kk_context_t* _ctx) { /* () -> bool */ 
  return kk_evv_cfc(kk_context())<=2;
}

kk_std_core_hnd__ev kk_std_core_hnd__evv_lookup(kk_std_core_hnd__htag htag0, kk_context_t* _ctx) { /* forall<a> (htag : htag<a>) -> ev<a> */ 
  return kk_evv_lookup(htag0,kk_context());
}

kk_evv_t kk_std_core_hnd_evv_get(kk_context_t* _ctx) { /* forall<e> () -> e evv<e> */ 
  return kk_evv_get(kk_context());
}

kk_evv_t kk_std_core_hnd_evv_insert(kk_evv_t evv, kk_std_core_hnd__ev ev, kk_context_t* _ctx) { /* forall<e,e1,a> (evv : evv<e>, ev : ev<a>) -> e evv<e1> */ 
  return kk_evv_insert(evv,ev,kk_context());
}

int32_t kk_std_core_hnd_fresh_marker_int(kk_context_t* _ctx) { /* () -> int32 */ 
  return kk_marker_unique(kk_context());
}

bool kk_std_core_hnd_evv_eq(kk_evv_t evv0, kk_evv_t evv1, kk_context_t* _ctx) { /* forall<e> (evv0 : evv<e>, evv1 : evv<e>) -> bool */ 
  return kk_evv_eq(evv0,evv1,kk_context());
}

kk_unit_t kk_std_core_hnd_guard(kk_evv_t w, kk_context_t* _ctx) { /* forall<e> (w : evv<e>) -> e () */ 
  kk_evv_guard(w,kk_context()); return kk_Unit;
}

kk_box_t kk_std_core_hnd_yield_extend(kk_function_t next, kk_context_t* _ctx) { /* forall<a,b,e> (next : (a) -> e b) -> e b */ 
  return kk_yield_extend(next,kk_context());
}

kk_box_t kk_std_core_hnd_yield_cont(kk_function_t f, kk_context_t* _ctx) { /* forall<a,e,b> (f : forall<c> ((c) -> e a, c) -> e b) -> e b */ 
  return kk_yield_cont(f,kk_context());
}

kk_std_core_hnd__yld kk_std_core_hnd_yield_prompt(kk_std_core_hnd__marker m0, kk_context_t* _ctx) { /* forall<a,e,b> (m : marker<e,b>) -> yld<e,a,b> */ 
  return kk_yield_prompt(m0,kk_context());
}

kk_box_t kk_std_core_hnd_yield_to_final(kk_std_core_hnd__marker m0, kk_function_t clause0, kk_context_t* _ctx) { /* forall<a,e,e1,b> (m : marker<e1,b>, clause : ((resume-result<a,b>) -> e1 b) -> e1 b) -> e a */ 
  return kk_yield_final(m0,clause0,kk_context());
}

kk_evv_t kk_std_core_hnd_evv_swap_delete(kk_ssize_t i, bool behind, kk_context_t* _ctx) { /* forall<e,e1> (i : ev-index, behind : bool) -> e1 evv<e> */ 
  return kk_evv_swap_delete(i,behind,kk_context());
}

int32_t kk_std_core_hnd_fresh_marker_named_int(kk_context_t* _ctx) { /* () -> int32 */ 
  return -kk_marker_unique(kk_context());
}

kk_evv_t kk_std_core_hnd_evv_swap_create(kk_vector_t indices, kk_context_t* _ctx) { /* forall<e> (indices : vector<ev-index>) -> e evv<e> */ 
  return kk_evv_swap_create(indices,kk_context());
}

kk_function_t kk_std_core_hnd_yield_to_prim(kk_std_core_hnd__marker m0, kk_function_t clause0, kk_context_t* _ctx) { /* forall<a,e,e1,b> (m : marker<e1,b>, clause : ((resume-result<a,b>) -> e1 b) -> e1 b) -> e (() -> a) */ 
  return kk_yield_to(m0,clause0,kk_context());
}
extern kk_box_t kk_std_core_hnd_clause_tail_noyield0_fun10651(kk_function_t _fself, kk_std_core_hnd__marker ___wildcard__637__14, kk_std_core_hnd__ev ___wildcard__637__17, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail_noyield0_fun10651__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail_noyield0_fun10651__t*, _fself);
  kk_function_t op = _self->op; /* () -> 4106 4108 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__637__17, ((int32_t)KI32(3)), _ctx);
  return kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), op, (op, _ctx));
}
extern kk_box_t kk_std_core_hnd_clause_tail_noyield1_fun10652(kk_function_t _fself, kk_std_core_hnd__marker ___wildcard__581__14, kk_std_core_hnd__ev ___wildcard__581__17, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail_noyield1_fun10652__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail_noyield1_fun10652__t*, _fself);
  kk_function_t op = _self->op; /* (4139) -> 4137 4140 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__581__17, ((int32_t)KI32(3)), _ctx);
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), op, (op, x, _ctx));
}
extern kk_box_t kk_std_core_hnd_clause_tail_noyield2_fun10653(kk_function_t _fself, kk_std_core_hnd__marker ___wildcard__689__14, kk_std_core_hnd__ev ___wildcard__689__17, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail_noyield2_fun10653__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail_noyield2_fun10653__t*, _fself);
  kk_function_t op = _self->op; /* (4176, 4177) -> 4174 4178 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__689__17, ((int32_t)KI32(3)), _ctx);
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), op, (op, x1, x2, _ctx));
}

kk_evv_t kk_std_core_hnd_evv_swap_with(kk_std_core_hnd__ev ev, kk_context_t* _ctx) { /* forall<a,e> (ev : ev<a>) -> evv<e> */ 
  kk_evv_t _x10654;
  {
    struct kk_std_core_hnd_Ev* _con10655 = kk_std_core_hnd__as_Ev(ev);
    kk_std_core_hnd__htag _pat0 = _con10655->htag;
    kk_box_t _pat2 = _con10655->hnd;
    kk_evv_t w = _con10655->hevv;
    if (kk_likely(kk_std_core_hnd__ev_is_unique(ev))) {
      kk_box_drop(_pat2, _ctx);
      kk_std_core_hnd__htag_drop(_pat0, _ctx);
      kk_std_core_hnd__ev_free(ev);
    }
    else {
      kk_evv_dup(w);
      kk_std_core_hnd__ev_decref(ev, _ctx);
    }
    _x10654 = w; /*std/core/hnd/evv<4183>*/
  }
  return kk_evv_swap(_x10654,kk_context());
}
extern kk_box_t kk_std_core_hnd_clause_value_fun10656(kk_function_t _fself, kk_std_core_hnd__marker ___wildcard__641__14, kk_std_core_hnd__ev ___wildcard__641__17, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_value_fun10656__t* _self = kk_function_as(struct kk_std_core_hnd_clause_value_fun10656__t*, _fself);
  kk_box_t v = _self->v; /* 4226 */
  kk_drop_match(_self, {kk_box_dup(v);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__641__17, ((int32_t)KI32(3)), _ctx);
  return v;
}

kk_string_t kk_std_core_hnd_evv_show(kk_evv_t evv, kk_context_t* _ctx) { /* forall<e> (evv : evv<e>) -> string */ 
  return kk_evv_show(evv,kk_context());
}

kk_box_t kk_std_core_hnd_unsafe_reyield(kk_std_core_hnd__yield_info yld, kk_context_t* _ctx) { /* forall<a,e> (yld : yield-info) -> e a */ 
  return kk_yield_reyield(yld,kk_context());
}

kk_std_core_hnd__yield_info kk_std_core_hnd_yield_capture(kk_context_t* _ctx) { /* forall<e> () -> e yield-info */ 
  return kk_yield_capture(kk_context());
}

kk_box_t kk_std_core_hnd_resume_final(kk_context_t* _ctx) { /* forall<a> () -> a */ 
  return kk_fatal_resume_final(kk_context());
}


// lift anonymous function
struct kk_std_core_hnd_prompt_fun10664__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
  kk_function_t ret;
  kk_evv_t w0;
  kk_evv_t w1;
  kk_std_core_hnd__marker m0;
};
static kk_box_t kk_std_core_hnd_prompt_fun10664(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_prompt_fun10664(kk_std_core_hnd__ev ev, kk_function_t ret, kk_evv_t w0, kk_evv_t w1, kk_std_core_hnd__marker m0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10664__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_prompt_fun10664__t, 5, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_prompt_fun10664, kk_context());
  _self->ev = ev;
  _self->ret = ret;
  _self->w0 = w0;
  _self->w1 = w1;
  _self->m0 = m0;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_prompt_fun10664(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10664__t* _self = kk_function_as(struct kk_std_core_hnd_prompt_fun10664__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<4698> */
  kk_function_t ret = _self->ret; /* (4696) -> 4697 4699 */
  kk_evv_t w0 = _self->w0; /* std/core/hnd/evv<4697> */
  kk_evv_t w1 = _self->w1; /* std/core/hnd/evv<4697> */
  kk_std_core_hnd__marker m0 = _self->m0; /* std/core/hnd/marker<4697,4699> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);kk_function_dup(ret);kk_evv_dup(w0);kk_evv_dup(w1);kk_std_core_hnd__marker_dup(m0);}, {}, _ctx)
  kk_evv_t w0_sq_ = kk_std_core_hnd_evv_get(_ctx); /*std/core/hnd/evv<4697>*/;
  kk_evv_t w1_sq_;
  bool _match_10600;
  kk_evv_t _x10665 = kk_evv_dup(w0_sq_); /*std/core/hnd/evv<4697>*/
  _match_10600 = kk_std_core_hnd_evv_eq(w0, _x10665, _ctx); /*bool*/
  if (_match_10600) {
    w1_sq_ = w1; /*std/core/hnd/evv<4697>*/
  }
  else {
    kk_evv_drop(w1, _ctx);
    kk_evv_t _x10666 = kk_evv_dup(w0_sq_); /*std/core/hnd/evv<4697>*/
    kk_std_core_hnd__ev _x10667 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<4698>*/
    w1_sq_ = kk_std_core_hnd_evv_insert(_x10666, _x10667, _ctx); /*std/core/hnd/evv<4697>*/
  }
  kk_unit_t __1 = kk_Unit;
  kk_evv_t _x10668 = kk_evv_dup(w1_sq_); /*std/core/hnd/evv<4697>*/
  kk_evv_set(_x10668,kk_context());
  kk_box_t _x10669 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, res, _ctx)); /*4696*/
  return kk_std_core_hnd_prompt(w0_sq_, w1_sq_, ev, m0, ret, _x10669, _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_prompt_fun10670__t {
  struct kk_function_s _base;
  kk_function_t cont0;
  kk_std_core_hnd__ev ev;
  kk_function_t ret;
  kk_evv_t w0;
  kk_evv_t w1;
  kk_std_core_hnd__marker m0;
};
static kk_box_t kk_std_core_hnd_prompt_fun10670(kk_function_t _fself, kk_std_core_hnd__resume_result r, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_prompt_fun10670(kk_function_t cont0, kk_std_core_hnd__ev ev, kk_function_t ret, kk_evv_t w0, kk_evv_t w1, kk_std_core_hnd__marker m0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10670__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_prompt_fun10670__t, 6, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_prompt_fun10670, kk_context());
  _self->cont0 = cont0;
  _self->ev = ev;
  _self->ret = ret;
  _self->w0 = w0;
  _self->w1 = w1;
  _self->m0 = m0;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_prompt_fun10676__t {
  struct kk_function_s _base;
  kk_box_t x;
};
static kk_box_t kk_std_core_hnd_prompt_fun10676(kk_function_t _fself, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_prompt_fun10676(kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10676__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_prompt_fun10676__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_prompt_fun10676, kk_context());
  _self->x = x;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_prompt_fun10676(kk_function_t _fself, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10676__t* _self = kk_function_as(struct kk_std_core_hnd_prompt_fun10676__t*, _fself);
  kk_box_t x = _self->x; /* 4562 */
  kk_drop_match(_self, {kk_box_dup(x);}, {}, _ctx)
  return x;
}


// lift anonymous function
struct kk_std_core_hnd_prompt_fun10677__t {
  struct kk_function_s _base;
  kk_box_t x0;
};
static kk_box_t kk_std_core_hnd_prompt_fun10677(kk_function_t _fself, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_prompt_fun10677(kk_box_t x0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10677__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_prompt_fun10677__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_prompt_fun10677, kk_context());
  _self->x0 = x0;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_prompt_fun10677(kk_function_t _fself, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10677__t* _self = kk_function_as(struct kk_std_core_hnd_prompt_fun10677__t*, _fself);
  kk_box_t x0 = _self->x0; /* 4562 */
  kk_drop_match(_self, {kk_box_dup(x0);}, {}, _ctx)
  return x0;
}


// lift anonymous function
struct kk_std_core_hnd_prompt_fun10683__t {
  struct kk_function_s _base;
  kk_box_t x10;
  kk_std_core_hnd__marker m0;
};
static kk_box_t kk_std_core_hnd_prompt_fun10683(kk_function_t _fself, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_prompt_fun10683(kk_box_t x10, kk_std_core_hnd__marker m0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10683__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_prompt_fun10683__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_prompt_fun10683, kk_context());
  _self->x10 = x10;
  _self->m0 = m0;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_prompt_fun10684__t {
  struct kk_function_s _base;
  kk_box_t x10;
};
static kk_box_t kk_std_core_hnd_prompt_fun10684(kk_function_t _fself, kk_function_t ___wildcard__327__83, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_prompt_fun10684(kk_box_t x10, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10684__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_prompt_fun10684__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_prompt_fun10684, kk_context());
  _self->x10 = x10;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_prompt_fun10684(kk_function_t _fself, kk_function_t ___wildcard__327__83, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10684__t* _self = kk_function_as(struct kk_std_core_hnd_prompt_fun10684__t*, _fself);
  kk_box_t x10 = _self->x10; /* 4699 */
  kk_drop_match(_self, {kk_box_dup(x10);}, {}, _ctx)
  kk_function_drop(___wildcard__327__83, _ctx);
  return x10;
}
static kk_box_t kk_std_core_hnd_prompt_fun10683(kk_function_t _fself, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10683__t* _self = kk_function_as(struct kk_std_core_hnd_prompt_fun10683__t*, _fself);
  kk_box_t x10 = _self->x10; /* 4699 */
  kk_std_core_hnd__marker m0 = _self->m0; /* std/core/hnd/marker<4697,4699> */
  kk_drop_match(_self, {kk_box_dup(x10);kk_std_core_hnd__marker_dup(m0);}, {}, _ctx)
  return kk_std_core_hnd_yield_to_final(m0, kk_std_core_hnd_new_prompt_fun10684(x10, _ctx), _ctx);
}
static kk_box_t kk_std_core_hnd_prompt_fun10670(kk_function_t _fself, kk_std_core_hnd__resume_result r, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_fun10670__t* _self = kk_function_as(struct kk_std_core_hnd_prompt_fun10670__t*, _fself);
  kk_function_t cont0 = _self->cont0; /* (() -> 4562) -> 4697 4696 */
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<4698> */
  kk_function_t ret = _self->ret; /* (4696) -> 4697 4699 */
  kk_evv_t w0 = _self->w0; /* std/core/hnd/evv<4697> */
  kk_evv_t w1 = _self->w1; /* std/core/hnd/evv<4697> */
  kk_std_core_hnd__marker m0 = _self->m0; /* std/core/hnd/marker<4697,4699> */
  kk_drop_match(_self, {kk_function_dup(cont0);kk_std_core_hnd__ev_dup(ev);kk_function_dup(ret);kk_evv_dup(w0);kk_evv_dup(w1);kk_std_core_hnd__marker_dup(m0);}, {}, _ctx)
  if (kk_std_core_hnd__is_Deep(r)) {
    kk_box_t x = r._cons.Deep.result;
    kk_evv_t w00_sq_ = kk_std_core_hnd_evv_get(_ctx); /*std/core/hnd/evv<4697>*/;
    kk_evv_t w10_sq_;
    bool _match_10599;
    kk_evv_t _x10671 = kk_evv_dup(w00_sq_); /*std/core/hnd/evv<4697>*/
    _match_10599 = kk_std_core_hnd_evv_eq(w0, _x10671, _ctx); /*bool*/
    if (_match_10599) {
      w10_sq_ = w1; /*std/core/hnd/evv<4697>*/
    }
    else {
      kk_evv_drop(w1, _ctx);
      kk_evv_t _x10672 = kk_evv_dup(w00_sq_); /*std/core/hnd/evv<4697>*/
      kk_std_core_hnd__ev _x10673 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<4698>*/
      w10_sq_ = kk_std_core_hnd_evv_insert(_x10672, _x10673, _ctx); /*std/core/hnd/evv<4697>*/
    }
    kk_unit_t __2 = kk_Unit;
    kk_evv_t _x10674 = kk_evv_dup(w10_sq_); /*std/core/hnd/evv<4697>*/
    kk_evv_set(_x10674,kk_context());
    kk_box_t _x10675 = kk_function_call(kk_box_t, (kk_function_t, kk_function_t, kk_context_t*), cont0, (cont0, kk_std_core_hnd_new_prompt_fun10676(x, _ctx), _ctx)); /*4696*/
    return kk_std_core_hnd_prompt(w00_sq_, w10_sq_, ev, m0, ret, _x10675, _ctx);
  }
  if (kk_std_core_hnd__is_Shallow(r)) {
    kk_box_t x0 = r._cons.Shallow.result;
    kk_evv_drop(w1, _ctx);
    kk_evv_drop(w0, _ctx);
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    kk_box_t x1_10270 = kk_function_call(kk_box_t, (kk_function_t, kk_function_t, kk_context_t*), cont0, (cont0, kk_std_core_hnd_new_prompt_fun10677(x0, _ctx), _ctx)); /*4696*/;
    if (kk_yielding(kk_context())) {
      kk_box_drop(x1_10270, _ctx);
      return kk_std_core_hnd_yield_extend(ret, _ctx);
    }
    {
      return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), ret, (ret, x1_10270, _ctx));
    }
  }
  {
    kk_box_t x10 = r._cons.Finalize.result;
    kk_evv_t w01_sq_ = kk_std_core_hnd_evv_get(_ctx); /*std/core/hnd/evv<4697>*/;
    kk_evv_t w11_sq_;
    bool _match_10597;
    kk_evv_t _x10678 = kk_evv_dup(w01_sq_); /*std/core/hnd/evv<4697>*/
    _match_10597 = kk_std_core_hnd_evv_eq(w0, _x10678, _ctx); /*bool*/
    if (_match_10597) {
      w11_sq_ = w1; /*std/core/hnd/evv<4697>*/
    }
    else {
      kk_evv_drop(w1, _ctx);
      kk_evv_t _x10679 = kk_evv_dup(w01_sq_); /*std/core/hnd/evv<4697>*/
      kk_std_core_hnd__ev _x10680 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<4698>*/
      w11_sq_ = kk_std_core_hnd_evv_insert(_x10679, _x10680, _ctx); /*std/core/hnd/evv<4697>*/
    }
    kk_unit_t __3 = kk_Unit;
    kk_evv_t _x10681 = kk_evv_dup(w11_sq_); /*std/core/hnd/evv<4697>*/
    kk_evv_set(_x10681,kk_context());
    kk_box_t _x10682 = kk_function_call(kk_box_t, (kk_function_t, kk_function_t, kk_context_t*), cont0, (cont0, kk_std_core_hnd_new_prompt_fun10683(x10, m0, _ctx), _ctx)); /*4696*/
    return kk_std_core_hnd_prompt(w01_sq_, w11_sq_, ev, m0, ret, _x10682, _ctx);
  }
}

kk_box_t kk_std_core_hnd_prompt(kk_evv_t w0, kk_evv_t w1, kk_std_core_hnd__ev ev, kk_std_core_hnd__marker m0, kk_function_t ret, kk_box_t result, kk_context_t* _ctx) { /* forall<a,e,b,c> (w0 : evv<e>, w1 : evv<e>, ev : ev<b>, m : marker<e,c>, ret : (a) -> e c, result : a) -> e c */ 
  kk_unit_t __ = kk_Unit;
  kk_evv_t _x10662 = kk_evv_dup(w1); /*std/core/hnd/evv<4697>*/
  kk_std_core_hnd_guard(_x10662, _ctx);
  kk_unit_t __0 = kk_Unit;
  kk_evv_t _x10663 = kk_evv_dup(w0); /*std/core/hnd/evv<4697>*/
  kk_evv_set(_x10663,kk_context());
  kk_std_core_hnd__yld _match_10596 = kk_std_core_hnd_yield_prompt(m0, _ctx); /*std/core/hnd/yld<3902,3901,3903>*/;
  if (kk_std_core_hnd__is_Pure(_match_10596)) {
    kk_evv_drop(w1, _ctx);
    kk_evv_drop(w0, _ctx);
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), ret, (ret, result, _ctx));
  }
  if (kk_std_core_hnd__is_YieldingFinal(_match_10596)) {
    kk_evv_drop(w1, _ctx);
    kk_evv_drop(w0, _ctx);
    kk_function_drop(ret, _ctx);
    kk_box_drop(result, _ctx);
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    return kk_box_any(kk_context());
  }
  if (kk_std_core_hnd__is_Yielding(_match_10596)) {
    kk_box_drop(result, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_prompt_fun10664(ev, ret, w0, w1, m0, _ctx), _ctx);
  }
  {
    kk_function_t clause0 = _match_10596._cons.Yield.clause;
    kk_function_t cont0 = _match_10596._cons.Yield.cont;
    kk_box_drop(result, _ctx);
    return kk_function_call(kk_box_t, (kk_function_t, kk_function_t, kk_context_t*), clause0, (clause0, kk_std_core_hnd_new_prompt_fun10670(cont0, ev, ret, w0, w1, m0, _ctx), _ctx));
  }
}

kk_box_t kk_std_core_hnd__hhandle(kk_std_core_hnd__htag tag, int32_t cfc0, kk_box_t h, kk_function_t ret, kk_function_t action, kk_context_t* _ctx) { /* forall<a,e,e1,b,c> (tag : htag<b>, cfc : cfc, h : b<e,c>, ret : (a) -> e c, action : () -> e1 a) -> e c */ 
  kk_evv_t w0 = kk_std_core_hnd_evv_get(_ctx); /*std/core/hnd/evv<4795>*/;
  kk_std_core_hnd__marker m0;
  int32_t _x10685 = kk_std_core_hnd_fresh_marker_int(_ctx); /*int32*/
  m0 = kk_std_core_hnd__new_Marker(_x10685, _ctx); /*std/core/hnd/marker<4795,4798>*/
  kk_std_core_hnd__ev ev;
  kk_evv_t _x10686 = kk_evv_dup(w0); /*std/core/hnd/evv<4795>*/
  ev = kk_std_core_hnd__new_Ev(kk_reuse_null, tag, m0, h, cfc0, _x10686, _ctx); /*std/core/hnd/ev<4797>*/
  kk_evv_t w1;
  kk_evv_t _x10687 = kk_evv_dup(w0); /*std/core/hnd/evv<4795>*/
  kk_std_core_hnd__ev _x10688 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<4797>*/
  w1 = kk_std_core_hnd_evv_insert(_x10687, _x10688, _ctx); /*std/core/hnd/evv<4795>*/
  kk_unit_t __ = kk_Unit;
  kk_evv_t _x10689 = kk_evv_dup(w1); /*std/core/hnd/evv<4795>*/
  kk_evv_set(_x10689,kk_context());
  kk_box_t _x10690 = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), action, (action, _ctx)); /*4794*/
  return kk_std_core_hnd_prompt(w0, w1, ev, m0, ret, _x10690, _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_mask_at1_fun10691__t {
  struct kk_function_s _base;
  kk_ssize_t i;
  bool behind;
};
static kk_box_t kk_std_core_hnd_mask_at1_fun10691(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_mask_at1_fun10691(kk_ssize_t i, bool behind, kk_context_t* _ctx) {
  struct kk_std_core_hnd_mask_at1_fun10691__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_mask_at1_fun10691__t, 1, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_mask_at1_fun10691, kk_context());
  _self->i = i;
  _self->behind = behind;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_mask_at1_fun10691(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_mask_at1_fun10691__t* _self = kk_function_as(struct kk_std_core_hnd_mask_at1_fun10691__t*, _fself);
  kk_ssize_t i = _self->i; /* std/core/hnd/ev-index */
  bool behind = _self->behind; /* bool */
  kk_drop_match(_self, {;;}, {}, _ctx)
  return kk_std_core_hnd_mask_at1(i, behind, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd_mask_at1(kk_ssize_t i, bool behind, kk_function_t action, kk_box_t x, kk_context_t* _ctx) { /* forall<a,b,e,e1> (i : ev-index, behind : bool, action : (a) -> e b, x : a) -> e1 b */ 
  kk_evv_t w0 = kk_std_core_hnd_evv_swap_delete(i, behind, _ctx); /*std/core/hnd/evv<_4803>*/;
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), action, (action, x, _ctx)); /*4909*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w0,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_mask_at1_fun10691(i, behind, _ctx), _ctx);
  }
  {
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__mask_at_fun10692__t {
  struct kk_function_s _base;
  kk_ssize_t i;
  bool behind;
};
static kk_box_t kk_std_core_hnd__mask_at_fun10692(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_mask_at_fun10692(kk_ssize_t i, bool behind, kk_context_t* _ctx) {
  struct kk_std_core_hnd__mask_at_fun10692__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__mask_at_fun10692__t, 1, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__mask_at_fun10692, kk_context());
  _self->i = i;
  _self->behind = behind;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__mask_at_fun10692(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__mask_at_fun10692__t* _self = kk_function_as(struct kk_std_core_hnd__mask_at_fun10692__t*, _fself);
  kk_ssize_t i = _self->i; /* std/core/hnd/ev-index */
  bool behind = _self->behind; /* bool */
  kk_drop_match(_self, {;;}, {}, _ctx)
  return kk_std_core_hnd_mask_at1(i, behind, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__mask_at(kk_ssize_t i, bool behind, kk_function_t action, kk_context_t* _ctx) { /* forall<a,e,e1> (i : ev-index, behind : bool, action : () -> e a) -> e1 a */ 
  kk_evv_t w0 = kk_std_core_hnd_evv_swap_delete(i, behind, _ctx); /*std/core/hnd/evv<_4915>*/;
  kk_box_t x = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), action, (action, _ctx)); /*5004*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w0,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(x, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_mask_at_fun10692(i, behind, _ctx), _ctx);
  }
  {
    return x;
  }
}

kk_box_t kk_std_core_hnd__named_handle(kk_std_core_hnd__htag tag, int32_t cfc0, kk_box_t h, kk_function_t ret, kk_function_t action, kk_context_t* _ctx) { /* forall<a,e,e1,b,c> (tag : htag<b>, cfc : cfc, h : b<e,c>, ret : (a) -> e c, action : (ev<b>) -> e1 a) -> e c */ 
  kk_std_core_hnd__marker m0;
  int32_t _x10694 = kk_std_core_hnd_fresh_marker_named_int(_ctx); /*int32*/
  m0 = kk_std_core_hnd__new_Marker(_x10694, _ctx); /*std/core/hnd/marker<5119,5122>*/
  kk_evv_t w0 = kk_std_core_hnd_evv_get(_ctx); /*std/core/hnd/evv<5119>*/;
  kk_std_core_hnd__ev ev;
  kk_evv_t _x10695 = kk_evv_dup(w0); /*std/core/hnd/evv<5119>*/
  ev = kk_std_core_hnd__new_Ev(kk_reuse_null, tag, m0, h, cfc0, _x10695, _ctx); /*std/core/hnd/ev<5121>*/
  kk_evv_t _x10696 = kk_evv_dup(w0); /*std/core/hnd/evv<5119>*/
  kk_std_core_hnd__ev _x10697 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<5121>*/
  kk_box_t _x10698 = kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__ev, kk_context_t*), action, (action, ev, _ctx)); /*5118*/
  return kk_std_core_hnd_prompt(_x10696, w0, _x10697, m0, ret, _x10698, _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_open_at1_fun10699__t {
  struct kk_function_s _base;
  kk_ssize_t i;
};
static kk_box_t kk_std_core_hnd_open_at1_fun10699(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_open_at1_fun10699(kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_hnd_open_at1_fun10699__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_open_at1_fun10699__t, 1, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_open_at1_fun10699, kk_context());
  _self->i = i;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_open_at1_fun10699(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_open_at1_fun10699__t* _self = kk_function_as(struct kk_std_core_hnd_open_at1_fun10699__t*, _fself);
  kk_ssize_t i = _self->i; /* std/core/hnd/ev-index */
  kk_drop_match(_self, {;}, {}, _ctx)
  return kk_std_core_hnd_open_at1(i, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd_open_at1(kk_ssize_t i, kk_function_t f, kk_box_t x, kk_context_t* _ctx) { /* forall<a,b,e,e1> (i : ev-index, f : (a) -> e b, x : a) -> e1 b */ 
  kk_evv_t w = kk_evv_swap_create1(i,kk_context()); /*std/core/hnd/evv<5232>*/;
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, x, _ctx)); /*5230*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_open_at1_fun10699(i, _ctx), _ctx);
  }
  {
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__open_at0_fun10700__t {
  struct kk_function_s _base;
  kk_ssize_t i;
};
static kk_box_t kk_std_core_hnd__open_at0_fun10700(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_open_at0_fun10700(kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open_at0_fun10700__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__open_at0_fun10700__t, 1, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__open_at0_fun10700, kk_context());
  _self->i = i;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__open_at0_fun10700(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open_at0_fun10700__t* _self = kk_function_as(struct kk_std_core_hnd__open_at0_fun10700__t*, _fself);
  kk_ssize_t i = _self->i; /* std/core/hnd/ev-index */
  kk_drop_match(_self, {;}, {}, _ctx)
  return kk_std_core_hnd_open_at1(i, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__open_at0(kk_ssize_t i, kk_function_t f, kk_context_t* _ctx) { /* forall<a,e,e1> (i : ev-index, f : () -> e a) -> e1 a */ 
  kk_evv_t w = kk_evv_swap_create1(i,kk_context()); /*std/core/hnd/evv<5324>*/;
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), f, (f, _ctx)); /*5322*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_open_at0_fun10700(i, _ctx), _ctx);
  }
  {
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__open_at1_fun10701__t {
  struct kk_function_s _base;
  kk_ssize_t i;
};
static kk_box_t kk_std_core_hnd__open_at1_fun10701(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_open_at1_fun10701(kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open_at1_fun10701__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__open_at1_fun10701__t, 1, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__open_at1_fun10701, kk_context());
  _self->i = i;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__open_at1_fun10701(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open_at1_fun10701__t* _self = kk_function_as(struct kk_std_core_hnd__open_at1_fun10701__t*, _fself);
  kk_ssize_t i = _self->i; /* std/core/hnd/ev-index */
  kk_drop_match(_self, {;}, {}, _ctx)
  return kk_std_core_hnd_open_at1(i, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__open_at1(kk_ssize_t i, kk_function_t f, kk_box_t x, kk_context_t* _ctx) { /* forall<a,b,e,e1> (i : ev-index, f : (a) -> e b, x : a) -> e1 b */ 
  kk_evv_t w = kk_evv_swap_create1(i,kk_context()); /*std/core/hnd/evv<5431>*/;
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, x, _ctx)); /*5429*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_open_at1_fun10701(i, _ctx), _ctx);
  }
  {
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__open_at2_fun10702__t {
  struct kk_function_s _base;
  kk_ssize_t i;
};
static kk_box_t kk_std_core_hnd__open_at2_fun10702(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_open_at2_fun10702(kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open_at2_fun10702__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__open_at2_fun10702__t, 1, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__open_at2_fun10702, kk_context());
  _self->i = i;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__open_at2_fun10702(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open_at2_fun10702__t* _self = kk_function_as(struct kk_std_core_hnd__open_at2_fun10702__t*, _fself);
  kk_ssize_t i = _self->i; /* std/core/hnd/ev-index */
  kk_drop_match(_self, {;}, {}, _ctx)
  return kk_std_core_hnd_open_at1(i, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__open_at2(kk_ssize_t i, kk_function_t f, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx) { /* forall<a,b,c,e,e1> (i : ev-index, f : (a, b) -> e c, x1 : a, x2 : b) -> e1 c */ 
  kk_evv_t w = kk_evv_swap_create1(i,kk_context()); /*std/core/hnd/evv<5553>*/;
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), f, (f, x1, x2, _ctx)); /*5551*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_open_at2_fun10702(i, _ctx), _ctx);
  }
  {
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__open_at3_fun10703__t {
  struct kk_function_s _base;
  kk_ssize_t i;
};
static kk_box_t kk_std_core_hnd__open_at3_fun10703(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_open_at3_fun10703(kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open_at3_fun10703__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__open_at3_fun10703__t, 1, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__open_at3_fun10703, kk_context());
  _self->i = i;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__open_at3_fun10703(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open_at3_fun10703__t* _self = kk_function_as(struct kk_std_core_hnd__open_at3_fun10703__t*, _fself);
  kk_ssize_t i = _self->i; /* std/core/hnd/ev-index */
  kk_drop_match(_self, {;}, {}, _ctx)
  return kk_std_core_hnd_open_at1(i, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__open_at3(kk_ssize_t i, kk_function_t f, kk_box_t x1, kk_box_t x2, kk_box_t x3, kk_context_t* _ctx) { /* forall<a,b,c,d,e,e1> (i : ev-index, f : (a, b, c) -> e d, x1 : a, x2 : b, x3 : c) -> e1 d */ 
  kk_evv_t w = kk_evv_swap_create1(i,kk_context()); /*std/core/hnd/evv<5627>*/;
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), f, (f, x1, x2, x3, _ctx)); /*5625*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_open_at3_fun10703(i, _ctx), _ctx);
  }
  {
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__open_at4_fun10704__t {
  struct kk_function_s _base;
  kk_ssize_t i;
};
static kk_box_t kk_std_core_hnd__open_at4_fun10704(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_open_at4_fun10704(kk_ssize_t i, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open_at4_fun10704__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__open_at4_fun10704__t, 1, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__open_at4_fun10704, kk_context());
  _self->i = i;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__open_at4_fun10704(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open_at4_fun10704__t* _self = kk_function_as(struct kk_std_core_hnd__open_at4_fun10704__t*, _fself);
  kk_ssize_t i = _self->i; /* std/core/hnd/ev-index */
  kk_drop_match(_self, {;}, {}, _ctx)
  return kk_std_core_hnd_open_at1(i, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__open_at4(kk_ssize_t i, kk_function_t f, kk_box_t x1, kk_box_t x2, kk_box_t x3, kk_box_t x4, kk_context_t* _ctx) { /* forall<a,b,c,d,a1,e,e1> (i : ev-index, f : (a, b, c, d) -> e a1, x1 : a, x2 : b, x3 : c, x4 : d) -> e1 a1 */ 
  kk_evv_t w = kk_evv_swap_create1(i,kk_context()); /*std/core/hnd/evv<5707>*/;
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), f, (f, x1, x2, x3, x4, _ctx)); /*5705*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_open_at4_fun10704(i, _ctx), _ctx);
  }
  {
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd_open1_fun10706__t {
  struct kk_function_s _base;
  kk_vector_t indices;
};
static kk_box_t kk_std_core_hnd_open1_fun10706(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_open1_fun10706(kk_vector_t indices, kk_context_t* _ctx) {
  struct kk_std_core_hnd_open1_fun10706__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_open1_fun10706__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_open1_fun10706, kk_context());
  _self->indices = indices;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_open1_fun10706(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_open1_fun10706__t* _self = kk_function_as(struct kk_std_core_hnd_open1_fun10706__t*, _fself);
  kk_vector_t indices = _self->indices; /* vector<std/core/hnd/ev-index> */
  kk_drop_match(_self, {kk_vector_dup(indices);}, {}, _ctx)
  return kk_std_core_hnd_open1(indices, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd_open1(kk_vector_t indices, kk_function_t f, kk_box_t x, kk_context_t* _ctx) { /* forall<a,b,e,e1> (indices : vector<ev-index>, f : (a) -> e b, x : a) -> e1 b */ 
  kk_evv_t w;
  kk_vector_t _x10705 = kk_vector_dup(indices); /*vector<std/core/hnd/ev-index>*/
  w = kk_std_core_hnd_evv_swap_create(_x10705, _ctx); /*std/core/hnd/evv<5817>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, x, _ctx)); /*5815*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_open1_fun10706(indices, _ctx), _ctx);
  }
  {
    kk_vector_drop(indices, _ctx);
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__open0_fun10708__t {
  struct kk_function_s _base;
  kk_vector_t indices;
};
static kk_box_t kk_std_core_hnd__open0_fun10708(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_open0_fun10708(kk_vector_t indices, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open0_fun10708__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__open0_fun10708__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__open0_fun10708, kk_context());
  _self->indices = indices;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__open0_fun10708(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open0_fun10708__t* _self = kk_function_as(struct kk_std_core_hnd__open0_fun10708__t*, _fself);
  kk_vector_t indices = _self->indices; /* vector<std/core/hnd/ev-index> */
  kk_drop_match(_self, {kk_vector_dup(indices);}, {}, _ctx)
  return kk_std_core_hnd_open1(indices, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__open0(kk_vector_t indices, kk_function_t f, kk_context_t* _ctx) { /* forall<a,e,e1> (indices : vector<ev-index>, f : () -> e a) -> e1 a */ 
  kk_evv_t w;
  kk_vector_t _x10707 = kk_vector_dup(indices); /*vector<std/core/hnd/ev-index>*/
  w = kk_std_core_hnd_evv_swap_create(_x10707, _ctx); /*std/core/hnd/evv<5909>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), f, (f, _ctx)); /*5907*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_open0_fun10708(indices, _ctx), _ctx);
  }
  {
    kk_vector_drop(indices, _ctx);
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__open1_fun10710__t {
  struct kk_function_s _base;
  kk_vector_t indices;
};
static kk_box_t kk_std_core_hnd__open1_fun10710(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_open1_fun10710(kk_vector_t indices, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open1_fun10710__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__open1_fun10710__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__open1_fun10710, kk_context());
  _self->indices = indices;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__open1_fun10710(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open1_fun10710__t* _self = kk_function_as(struct kk_std_core_hnd__open1_fun10710__t*, _fself);
  kk_vector_t indices = _self->indices; /* vector<std/core/hnd/ev-index> */
  kk_drop_match(_self, {kk_vector_dup(indices);}, {}, _ctx)
  return kk_std_core_hnd_open1(indices, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__open1(kk_vector_t indices, kk_function_t f, kk_box_t x, kk_context_t* _ctx) { /* forall<a,b,e,e1> (indices : vector<ev-index>, f : (a) -> e b, x : a) -> e1 b */ 
  kk_evv_t w;
  kk_vector_t _x10709 = kk_vector_dup(indices); /*vector<std/core/hnd/ev-index>*/
  w = kk_std_core_hnd_evv_swap_create(_x10709, _ctx); /*std/core/hnd/evv<6016>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), f, (f, x, _ctx)); /*6014*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_open1_fun10710(indices, _ctx), _ctx);
  }
  {
    kk_vector_drop(indices, _ctx);
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__open2_fun10712__t {
  struct kk_function_s _base;
  kk_vector_t indices;
};
static kk_box_t kk_std_core_hnd__open2_fun10712(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_open2_fun10712(kk_vector_t indices, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open2_fun10712__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__open2_fun10712__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__open2_fun10712, kk_context());
  _self->indices = indices;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__open2_fun10712(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open2_fun10712__t* _self = kk_function_as(struct kk_std_core_hnd__open2_fun10712__t*, _fself);
  kk_vector_t indices = _self->indices; /* vector<std/core/hnd/ev-index> */
  kk_drop_match(_self, {kk_vector_dup(indices);}, {}, _ctx)
  return kk_std_core_hnd_open1(indices, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__open2(kk_vector_t indices, kk_function_t f, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx) { /* forall<a,b,c,e,e1> (indices : vector<ev-index>, f : (a, b) -> e c, x1 : a, x2 : b) -> e1 c */ 
  kk_evv_t w;
  kk_vector_t _x10711 = kk_vector_dup(indices); /*vector<std/core/hnd/ev-index>*/
  w = kk_std_core_hnd_evv_swap_create(_x10711, _ctx); /*std/core/hnd/evv<6138>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), f, (f, x1, x2, _ctx)); /*6136*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_open2_fun10712(indices, _ctx), _ctx);
  }
  {
    kk_vector_drop(indices, _ctx);
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__open3_fun10714__t {
  struct kk_function_s _base;
  kk_vector_t indices;
};
static kk_box_t kk_std_core_hnd__open3_fun10714(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_open3_fun10714(kk_vector_t indices, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open3_fun10714__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__open3_fun10714__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__open3_fun10714, kk_context());
  _self->indices = indices;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__open3_fun10714(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open3_fun10714__t* _self = kk_function_as(struct kk_std_core_hnd__open3_fun10714__t*, _fself);
  kk_vector_t indices = _self->indices; /* vector<std/core/hnd/ev-index> */
  kk_drop_match(_self, {kk_vector_dup(indices);}, {}, _ctx)
  return kk_std_core_hnd_open1(indices, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__open3(kk_vector_t indices, kk_function_t f, kk_box_t x1, kk_box_t x2, kk_box_t x3, kk_context_t* _ctx) { /* forall<a,b,c,d,e,e1> (indices : vector<ev-index>, f : (a, b, c) -> e d, x1 : a, x2 : b, x3 : c) -> e1 d */ 
  kk_evv_t w;
  kk_vector_t _x10713 = kk_vector_dup(indices); /*vector<std/core/hnd/ev-index>*/
  w = kk_std_core_hnd_evv_swap_create(_x10713, _ctx); /*std/core/hnd/evv<6212>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), f, (f, x1, x2, x3, _ctx)); /*6210*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_open3_fun10714(indices, _ctx), _ctx);
  }
  {
    kk_vector_drop(indices, _ctx);
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd__open4_fun10716__t {
  struct kk_function_s _base;
  kk_vector_t indices;
};
static kk_box_t kk_std_core_hnd__open4_fun10716(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd__new_open4_fun10716(kk_vector_t indices, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open4_fun10716__t* _self = kk_function_alloc_as(struct kk_std_core_hnd__open4_fun10716__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd__open4_fun10716, kk_context());
  _self->indices = indices;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd__open4_fun10716(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd__open4_fun10716__t* _self = kk_function_as(struct kk_std_core_hnd__open4_fun10716__t*, _fself);
  kk_vector_t indices = _self->indices; /* vector<std/core/hnd/ev-index> */
  kk_drop_match(_self, {kk_vector_dup(indices);}, {}, _ctx)
  return kk_std_core_hnd_open1(indices, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd__open4(kk_vector_t indices, kk_function_t f, kk_box_t x1, kk_box_t x2, kk_box_t x3, kk_box_t x4, kk_context_t* _ctx) { /* forall<a,b,c,d,a1,e,e1> (indices : vector<ev-index>, f : (a, b, c, d) -> e a1, x1 : a, x2 : b, x3 : c, x4 : d) -> e1 a1 */ 
  kk_evv_t w;
  kk_vector_t _x10715 = kk_vector_dup(indices); /*vector<std/core/hnd/ev-index>*/
  w = kk_std_core_hnd_evv_swap_create(_x10715, _ctx); /*std/core/hnd/evv<6292>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), f, (f, x1, x2, x3, x4, _ctx)); /*6290*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd__new_open4_fun10716(indices, _ctx), _ctx);
  }
  {
    kk_vector_drop(indices, _ctx);
    return y;
  }
}


// lift anonymous function
struct kk_std_core_hnd_yield_to_fun10723__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_hnd_yield_to_fun10723(kk_function_t _fself, kk_box_t _b_10394, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_yield_to_fun10723(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_hnd_yield_to_fun10723, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_hnd_yield_to_fun10723(kk_function_t _fself, kk_box_t _b_10394, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_function_t _x10724 = kk_function_unbox(_b_10394); /*() -> 6424 10395*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), _x10724, (_x10724, _ctx));
}

kk_box_t kk_std_core_hnd_yield_to(kk_std_core_hnd__marker m0, kk_function_t clause0, kk_context_t* _ctx) { /* forall<a,e,b> (m : marker<e,b>, clause : ((resume-result<a,b>) -> e b) -> e b) -> e a */ 
  kk_function_t g = kk_std_core_hnd_yield_to_prim(m0, clause0, _ctx); /*() -> 6423*/;
  kk_function_drop(g, _ctx);
  return kk_std_core_hnd_yield_extend(kk_std_core_hnd_new_yield_to_fun10723(_ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_control_raw0_fun10725__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control_raw0_fun10725(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__618__16, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control_raw0_fun10725(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw0_fun10725__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control_raw0_fun10725__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control_raw0_fun10725, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_control_raw0_fun10726__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control_raw0_fun10726(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control_raw0_fun10726(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw0_fun10726__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control_raw0_fun10726__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control_raw0_fun10726, kk_context());
  _self->op = op;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_control_raw0_fun10726(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw0_fun10726__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control_raw0_fun10726__t*, _fself);
  kk_function_t op = _self->op; /* (std/core/hnd/resume-context<6464,6465,6466,6468>) -> 6465 6468 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__resume_context _x10727 = kk_std_core_hnd__new_Resume_context(k0, _ctx); /*std/core/hnd/resume-context<89,90,91,92>*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__resume_context, kk_context_t*), op, (op, _x10727, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_control_raw0_fun10725(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__618__16, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw0_fun10725__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control_raw0_fun10725__t*, _fself);
  kk_function_t op = _self->op; /* (std/core/hnd/resume-context<6464,6465,6466,6468>) -> 6465 6468 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__618__16, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to(m0, kk_std_core_hnd_new_clause_control_raw0_fun10726(op, _ctx), _ctx);
}

kk_std_core_hnd__clause0 kk_std_core_hnd_clause_control_raw0(kk_function_t op, kk_context_t* _ctx) { /* forall<a,e,e1,b,c> (op : (resume-context<a,e,e1,c>) -> e c) -> clause0<a,b,e,c> */ 
  return kk_std_core_hnd__new_Clause0(kk_std_core_hnd_new_clause_control_raw0_fun10725(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_control_raw1_fun10728__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control_raw1_fun10728(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__539__16, kk_box_t x, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control_raw1_fun10728(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw1_fun10728__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control_raw1_fun10728__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control_raw1_fun10728, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_control_raw1_fun10729__t {
  struct kk_function_s _base;
  kk_function_t op;
  kk_box_t x;
};
static kk_box_t kk_std_core_hnd_clause_control_raw1_fun10729(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control_raw1_fun10729(kk_function_t op, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw1_fun10729__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control_raw1_fun10729__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control_raw1_fun10729, kk_context());
  _self->op = op;
  _self->x = x;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_control_raw1_fun10729(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw1_fun10729__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control_raw1_fun10729__t*, _fself);
  kk_function_t op = _self->op; /* (x : 6512, r : std/core/hnd/resume-context<6513,6514,6515,6517>) -> 6514 6517 */
  kk_box_t x = _self->x; /* 6512 */
  kk_drop_match(_self, {kk_function_dup(op);kk_box_dup(x);}, {}, _ctx)
  kk_std_core_hnd__resume_context _x10730 = kk_std_core_hnd__new_Resume_context(k0, _ctx); /*std/core/hnd/resume-context<89,90,91,92>*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_std_core_hnd__resume_context, kk_context_t*), op, (op, x, _x10730, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_control_raw1_fun10728(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__539__16, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw1_fun10728__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control_raw1_fun10728__t*, _fself);
  kk_function_t op = _self->op; /* (x : 6512, r : std/core/hnd/resume-context<6513,6514,6515,6517>) -> 6514 6517 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__539__16, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to(m0, kk_std_core_hnd_new_clause_control_raw1_fun10729(op, x, _ctx), _ctx);
}

kk_std_core_hnd__clause1 kk_std_core_hnd_clause_control_raw1(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,e,e1,c,d> (op : (x : a, r : resume-context<b,e,e1,d>) -> e d) -> clause1<a,b,c,e,d> */ 
  return kk_std_core_hnd__new_Clause1(kk_std_core_hnd_new_clause_control_raw1_fun10728(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_control_raw2_fun10731__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control_raw2_fun10731(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__681__16, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control_raw2_fun10731(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw2_fun10731__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control_raw2_fun10731__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control_raw2_fun10731, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_control_raw2_fun10732__t {
  struct kk_function_s _base;
  kk_function_t op;
  kk_box_t x1;
  kk_box_t x2;
};
static kk_box_t kk_std_core_hnd_clause_control_raw2_fun10732(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control_raw2_fun10732(kk_function_t op, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw2_fun10732__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control_raw2_fun10732__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control_raw2_fun10732, kk_context());
  _self->op = op;
  _self->x1 = x1;
  _self->x2 = x2;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_control_raw2_fun10732(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw2_fun10732__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control_raw2_fun10732__t*, _fself);
  kk_function_t op = _self->op; /* (x1 : 6566, x2 : 6567, r : std/core/hnd/resume-context<6568,6569,6570,6572>) -> 6569 6572 */
  kk_box_t x1 = _self->x1; /* 6566 */
  kk_box_t x2 = _self->x2; /* 6567 */
  kk_drop_match(_self, {kk_function_dup(op);kk_box_dup(x1);kk_box_dup(x2);}, {}, _ctx)
  kk_std_core_hnd__resume_context _x10733 = kk_std_core_hnd__new_Resume_context(k0, _ctx); /*std/core/hnd/resume-context<89,90,91,92>*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_std_core_hnd__resume_context, kk_context_t*), op, (op, x1, x2, _x10733, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_control_raw2_fun10731(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__681__16, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw2_fun10731__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control_raw2_fun10731__t*, _fself);
  kk_function_t op = _self->op; /* (x1 : 6566, x2 : 6567, r : std/core/hnd/resume-context<6568,6569,6570,6572>) -> 6569 6572 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__681__16, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to(m0, kk_std_core_hnd_new_clause_control_raw2_fun10732(op, x1, x2, _ctx), _ctx);
}

kk_std_core_hnd__clause2 kk_std_core_hnd_clause_control_raw2(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,c,e,e1,d,a1> (op : (x1 : a, x2 : b, r : resume-context<c,e,e1,a1>) -> e a1) -> clause2<a,b,c,d,e,a1> */ 
  return kk_std_core_hnd__new_Clause2(kk_std_core_hnd_new_clause_control_raw2_fun10731(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_control_raw3_fun10734__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control_raw3_fun10734(kk_function_t _fself, kk_std_core_hnd__marker _b_10399, kk_std_core_hnd__ev _b_10400, kk_box_t _b_10401, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control_raw3_fun10734(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw3_fun10734__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control_raw3_fun10734__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control_raw3_fun10734, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_control_raw3_fun10735__t {
  struct kk_function_s _base;
  kk_box_t _b_10401;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control_raw3_fun10735(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control_raw3_fun10735(kk_box_t _b_10401, kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw3_fun10735__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control_raw3_fun10735__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control_raw3_fun10735, kk_context());
  _self->_b_10401 = _b_10401;
  _self->op = op;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_control_raw3_fun10735(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw3_fun10735__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control_raw3_fun10735__t*, _fself);
  kk_box_t _b_10401 = _self->_b_10401; /* 51 */
  kk_function_t op = _self->op; /* (x1 : 6807, x2 : 6808, x3 : 6809, r : std/core/hnd/resume-context<6810,6811,6812,6814>) -> 6811 6814 */
  kk_drop_match(_self, {kk_box_dup(_b_10401);kk_function_dup(op);}, {}, _ctx)
  kk_box_t _x10736;
  kk_std_core_types__tuple3_ _brw_10578;
  kk_box_t _x10737 = kk_box_dup(_b_10401); /*51*/
  _brw_10578 = kk_std_core_types__tuple3__unbox(_x10737, _ctx); /*(6807, 6808, 6809)*/
  kk_box_t _brw_10579 = kk_std_core_types_fst_1(_brw_10578, _ctx); /*461*/;
  kk_std_core_types__tuple3__drop(_brw_10578, _ctx);
  _x10736 = _brw_10579; /*461*/
  kk_box_t _x10738;
  kk_std_core_types__tuple3_ _brw_10576;
  kk_box_t _x10739 = kk_box_dup(_b_10401); /*51*/
  _brw_10576 = kk_std_core_types__tuple3__unbox(_x10739, _ctx); /*(6807, 6808, 6809)*/
  kk_box_t _brw_10577 = kk_std_core_types_snd_1(_brw_10576, _ctx); /*505*/;
  kk_std_core_types__tuple3__drop(_brw_10576, _ctx);
  _x10738 = _brw_10577; /*505*/
  kk_box_t _x10740;
  kk_std_core_types__tuple3_ _brw_10574 = kk_std_core_types__tuple3__unbox(_b_10401, _ctx); /*(6807, 6808, 6809)*/;
  kk_box_t _brw_10575 = kk_std_core_types_thd(_brw_10574, _ctx); /*549*/;
  kk_std_core_types__tuple3__drop(_brw_10574, _ctx);
  _x10740 = _brw_10575; /*549*/
  kk_std_core_hnd__resume_context _x10741 = kk_std_core_hnd__new_Resume_context(k0, _ctx); /*std/core/hnd/resume-context<89,90,91,92>*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_std_core_hnd__resume_context, kk_context_t*), op, (op, _x10736, _x10738, _x10740, _x10741, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_control_raw3_fun10734(kk_function_t _fself, kk_std_core_hnd__marker _b_10399, kk_std_core_hnd__ev _b_10400, kk_box_t _b_10401, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control_raw3_fun10734__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control_raw3_fun10734__t*, _fself);
  kk_function_t op = _self->op; /* (x1 : 6807, x2 : 6808, x3 : 6809, r : std/core/hnd/resume-context<6810,6811,6812,6814>) -> 6811 6814 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(_b_10400, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to(_b_10399, kk_std_core_hnd_new_clause_control_raw3_fun10735(_b_10401, op, _ctx), _ctx);
}

kk_std_core_hnd__clause1 kk_std_core_hnd_clause_control_raw3(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,c,d,e,e1,a1,b1> (op : (x1 : a, x2 : b, x3 : c, r : resume-context<d,e,e1,b1>) -> e b1) -> clause1<(a, b, c),d,a1,e,b1> */ 
  return kk_std_core_hnd__new_Clause1(kk_std_core_hnd_new_clause_control_raw3_fun10734(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_finalize_fun10746__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_hnd_finalize_fun10746(kk_function_t _fself, kk_box_t _x110745, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_finalize_fun10746(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_hnd_finalize_fun10746, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_hnd_finalize_fun10746(kk_function_t _fself, kk_box_t _x110745, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  return kk_std_core_types_id(_x110745, _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_finalize_fun10749__t {
  struct kk_function_s _base;
  kk_box_t res;
  kk_std_core_hnd__marker m0;
};
static kk_box_t kk_std_core_hnd_finalize_fun10749(kk_function_t _fself, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_finalize_fun10749(kk_box_t res, kk_std_core_hnd__marker m0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_finalize_fun10749__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_finalize_fun10749__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_finalize_fun10749, kk_context());
  _self->res = res;
  _self->m0 = m0;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_finalize_fun10750__t {
  struct kk_function_s _base;
  kk_box_t res;
};
static kk_box_t kk_std_core_hnd_finalize_fun10750(kk_function_t _fself, kk_function_t ___wildcard__426__70, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_finalize_fun10750(kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_finalize_fun10750__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_finalize_fun10750__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_finalize_fun10750, kk_context());
  _self->res = res;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_finalize_fun10750(kk_function_t _fself, kk_function_t ___wildcard__426__70, kk_context_t* _ctx) {
  struct kk_std_core_hnd_finalize_fun10750__t* _self = kk_function_as(struct kk_std_core_hnd_finalize_fun10750__t*, _fself);
  kk_box_t res = _self->res; /* 6873 */
  kk_drop_match(_self, {kk_box_dup(res);}, {}, _ctx)
  kk_function_drop(___wildcard__426__70, _ctx);
  return res;
}
static kk_box_t kk_std_core_hnd_finalize_fun10749(kk_function_t _fself, kk_context_t* _ctx) {
  struct kk_std_core_hnd_finalize_fun10749__t* _self = kk_function_as(struct kk_std_core_hnd_finalize_fun10749__t*, _fself);
  kk_box_t res = _self->res; /* 6873 */
  kk_std_core_hnd__marker m0 = _self->m0; /* std/core/hnd/marker<6875,6873> */
  kk_drop_match(_self, {kk_box_dup(res);kk_std_core_hnd__marker_dup(m0);}, {}, _ctx)
  return kk_std_core_hnd_yield_to_final(m0, kk_std_core_hnd_new_finalize_fun10750(res, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_finalize_fun10751__t {
  struct kk_function_s _base;
  kk_box_t res;
};
static kk_box_t kk_std_core_hnd_finalize_fun10751(kk_function_t _fself, kk_box_t ___wildcard__426__89, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_finalize_fun10751(kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_finalize_fun10751__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_finalize_fun10751__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_finalize_fun10751, kk_context());
  _self->res = res;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_finalize_fun10751(kk_function_t _fself, kk_box_t ___wildcard__426__89, kk_context_t* _ctx) {
  struct kk_std_core_hnd_finalize_fun10751__t* _self = kk_function_as(struct kk_std_core_hnd_finalize_fun10751__t*, _fself);
  kk_box_t res = _self->res; /* 6873 */
  kk_drop_match(_self, {kk_box_dup(res);}, {}, _ctx)
  kk_box_drop(___wildcard__426__89, _ctx);
  return res;
}

kk_box_t kk_std_core_hnd_finalize(kk_function_t cont, kk_box_t res, kk_context_t* _ctx) { /* forall<a,b,e,c> (cont : (() -> b) -> e c, res : a) -> e a */ 
  kk_std_core_hnd__marker m0;
  int32_t _x10742 = kk_std_core_hnd_fresh_marker_int(_ctx); /*int32*/
  m0 = kk_std_core_hnd__new_Marker(_x10742, _ctx); /*std/core/hnd/marker<6875,6873>*/
  kk_evv_t w = kk_std_core_hnd_evv_get(_ctx); /*std/core/hnd/evv<6875>*/;
  kk_evv_t _x10743 = kk_evv_dup(w); /*std/core/hnd/evv<6875>*/
  kk_std_core_hnd__ev _x10744 = kk_ev_none(kk_context()); /*std/core/hnd/ev<197>*/
  kk_box_t _x10747;
  kk_box_t x_10279;
  kk_function_t _x10748;
  kk_box_dup(res);
  _x10748 = kk_std_core_hnd_new_finalize_fun10749(res, m0, _ctx); /*() -> 3924*/
  x_10279 = kk_function_call(kk_box_t, (kk_function_t, kk_function_t, kk_context_t*), cont, (cont, _x10748, _ctx)); /*6876*/
  kk_box_drop(x_10279, _ctx);
  if (kk_yielding(kk_context())) {
    _x10747 = kk_std_core_hnd_yield_extend(kk_std_core_hnd_new_finalize_fun10751(res, _ctx), _ctx); /*3860*/
  }
  else {
    _x10747 = res; /*3860*/
  }
  return kk_std_core_hnd_prompt(_x10743, w, _x10744, m0, kk_std_core_hnd_new_finalize_fun10746(_ctx), _x10747, _ctx);
}

kk_box_t kk_std_core_hnd_protect_check(kk_ref_t resumed, kk_function_t k0, kk_box_t res, kk_context_t* _ctx) { /* forall<a,e,b> (resumed : ref<global,bool>, k : (resume-result<a,b>) -> e b, res : b) -> e b */ 
  bool did_resume;
  kk_box_t _x10754 = kk_ref_get(resumed,kk_context()); /*184*/
  did_resume = kk_bool_unbox(_x10754); /*bool*/
  if (did_resume) {
    kk_function_drop(k0, _ctx);
    return res;
  }
  {
    kk_std_core_hnd__resume_result _x10755 = kk_std_core_hnd__new_Finalize(res, _ctx); /*std/core/hnd/resume-result<80,81>*/
    return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__resume_result, kk_context_t*), k0, (k0, _x10755, _ctx));
  }
}


// lift anonymous function
struct kk_std_core_hnd_protect_fun10757__t {
  struct kk_function_s _base;
  kk_function_t k0;
  kk_ref_t resumed;
};
static kk_box_t kk_std_core_hnd_protect_fun10757(kk_function_t _fself, kk_box_t ret, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_protect_fun10757(kk_function_t k0, kk_ref_t resumed, kk_context_t* _ctx) {
  struct kk_std_core_hnd_protect_fun10757__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_protect_fun10757__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_protect_fun10757, kk_context());
  _self->k0 = k0;
  _self->resumed = resumed;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_protect_fun10757(kk_function_t _fself, kk_box_t ret, kk_context_t* _ctx) {
  struct kk_std_core_hnd_protect_fun10757__t* _self = kk_function_as(struct kk_std_core_hnd_protect_fun10757__t*, _fself);
  kk_function_t k0 = _self->k0; /* (std/core/hnd/resume-result<7140,7142>) -> 7141 7142 */
  kk_ref_t resumed = _self->resumed; /* ref<global,bool> */
  kk_drop_match(_self, {kk_function_dup(k0);kk_ref_dup(resumed);}, {}, _ctx)
  kk_unit_t __ = kk_Unit;
  kk_unit_t _brw_10571 = kk_Unit;
  kk_ref_set_borrow(resumed,(kk_bool_box(true)),kk_context());
  kk_ref_drop(resumed, _ctx);
  _brw_10571;
  kk_std_core_hnd__resume_result _x10758 = kk_std_core_hnd__new_Deep(ret, _ctx); /*std/core/hnd/resume-result<80,81>*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__resume_result, kk_context_t*), k0, (k0, _x10758, _ctx));
}


// lift anonymous function
struct kk_std_core_hnd_protect_fun10759__t {
  struct kk_function_s _base;
  kk_function_t k0;
  kk_ref_t resumed;
};
static kk_box_t kk_std_core_hnd_protect_fun10759(kk_function_t _fself, kk_box_t xres, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_protect_fun10759(kk_function_t k0, kk_ref_t resumed, kk_context_t* _ctx) {
  struct kk_std_core_hnd_protect_fun10759__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_protect_fun10759__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_protect_fun10759, kk_context());
  _self->k0 = k0;
  _self->resumed = resumed;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_protect_fun10759(kk_function_t _fself, kk_box_t xres, kk_context_t* _ctx) {
  struct kk_std_core_hnd_protect_fun10759__t* _self = kk_function_as(struct kk_std_core_hnd_protect_fun10759__t*, _fself);
  kk_function_t k0 = _self->k0; /* (std/core/hnd/resume-result<7140,7142>) -> 7141 7142 */
  kk_ref_t resumed = _self->resumed; /* ref<global,bool> */
  kk_drop_match(_self, {kk_function_dup(k0);kk_ref_dup(resumed);}, {}, _ctx)
  return kk_std_core_hnd_protect_check(resumed, k0, xres, _ctx);
}

kk_box_t kk_std_core_hnd_protect(kk_box_t x, kk_function_t clause0, kk_function_t k0, kk_context_t* _ctx) { /* forall<a,b,e,c> (x : a, clause : (x : a, k : (b) -> e c) -> e c, k : (resume-result<b,c>) -> e c) -> e c */ 
  kk_ref_t resumed = kk_ref_alloc((kk_bool_box(false)),kk_context()); /*ref<global,bool>*/;
  kk_box_t res;
  kk_function_t _x10756;
  kk_function_dup(k0);
  kk_ref_dup(resumed);
  _x10756 = kk_std_core_hnd_new_protect_fun10757(k0, resumed, _ctx); /*(ret : 7140) -> 7141 7142*/
  res = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_function_t, kk_context_t*), clause0, (clause0, x, _x10756, _ctx)); /*7142*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(res, _ctx);
    return kk_std_core_hnd_yield_extend(kk_std_core_hnd_new_protect_fun10759(k0, resumed, _ctx), _ctx);
  }
  {
    return kk_std_core_hnd_protect_check(resumed, k0, res, _ctx);
  }
}


// lift anonymous function
struct kk_std_core_hnd_protect_fun10761__t_1 {
  struct kk_function_s _base;
  kk_function_t k0;
  kk_ref_t resumed;
};
static kk_box_t kk_std_core_hnd_protect_fun10761_1(kk_function_t _fself, kk_box_t ret, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_protect_fun10761_1(kk_function_t k0, kk_ref_t resumed, kk_context_t* _ctx) {
  struct kk_std_core_hnd_protect_fun10761__t_1* _self = kk_function_alloc_as(struct kk_std_core_hnd_protect_fun10761__t_1, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_protect_fun10761_1, kk_context());
  _self->k0 = k0;
  _self->resumed = resumed;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_protect_fun10761_1(kk_function_t _fself, kk_box_t ret, kk_context_t* _ctx) {
  struct kk_std_core_hnd_protect_fun10761__t_1* _self = kk_function_as(struct kk_std_core_hnd_protect_fun10761__t_1*, _fself);
  kk_function_t k0 = _self->k0; /* (std/core/hnd/resume-result<7233,7235>) -> 7234 7235 */
  kk_ref_t resumed = _self->resumed; /* ref<global,bool> */
  kk_drop_match(_self, {kk_function_dup(k0);kk_ref_dup(resumed);}, {}, _ctx)
  kk_unit_t __ = kk_Unit;
  kk_unit_t _brw_10569 = kk_Unit;
  kk_ref_set_borrow(resumed,(kk_bool_box(true)),kk_context());
  kk_ref_drop(resumed, _ctx);
  _brw_10569;
  kk_std_core_hnd__resume_result _x10762 = kk_std_core_hnd__new_Deep(ret, _ctx); /*std/core/hnd/resume-result<80,81>*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_std_core_hnd__resume_result, kk_context_t*), k0, (k0, _x10762, _ctx));
}


// lift anonymous function
struct kk_std_core_hnd_protect_fun10763__t_1 {
  struct kk_function_s _base;
  kk_function_t k0;
  kk_ref_t resumed;
};
static kk_box_t kk_std_core_hnd_protect_fun10763_1(kk_function_t _fself, kk_box_t xres, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_protect_fun10763_1(kk_function_t k0, kk_ref_t resumed, kk_context_t* _ctx) {
  struct kk_std_core_hnd_protect_fun10763__t_1* _self = kk_function_alloc_as(struct kk_std_core_hnd_protect_fun10763__t_1, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_protect_fun10763_1, kk_context());
  _self->k0 = k0;
  _self->resumed = resumed;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_protect_fun10763_1(kk_function_t _fself, kk_box_t xres, kk_context_t* _ctx) {
  struct kk_std_core_hnd_protect_fun10763__t_1* _self = kk_function_as(struct kk_std_core_hnd_protect_fun10763__t_1*, _fself);
  kk_function_t k0 = _self->k0; /* (std/core/hnd/resume-result<7233,7235>) -> 7234 7235 */
  kk_ref_t resumed = _self->resumed; /* ref<global,bool> */
  kk_drop_match(_self, {kk_function_dup(k0);kk_ref_dup(resumed);}, {}, _ctx)
  return kk_std_core_hnd_protect_check(resumed, k0, xres, _ctx);
}

kk_box_t kk_std_core_hnd_protect_1(kk_box_t x1, kk_box_t x2, kk_function_t clause0, kk_function_t k0, kk_context_t* _ctx) { /* forall<a,b,c,e,d> (x1 : a, x2 : b, clause : (x : a, x : b, k : (c) -> e d) -> e d, k : (resume-result<c,d>) -> e d) -> e d */ 
  kk_ref_t resumed = kk_ref_alloc((kk_bool_box(false)),kk_context()); /*ref<global,bool>*/;
  kk_box_t res;
  kk_function_t _x10760;
  kk_function_dup(k0);
  kk_ref_dup(resumed);
  _x10760 = kk_std_core_hnd_new_protect_fun10761_1(k0, resumed, _ctx); /*(ret : 7233) -> 7234 7235*/
  res = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_function_t, kk_context_t*), clause0, (clause0, x1, x2, _x10760, _ctx)); /*7235*/
  if (kk_yielding(kk_context())) {
    kk_box_drop(res, _ctx);
    return kk_std_core_hnd_yield_extend(kk_std_core_hnd_new_protect_fun10763_1(k0, resumed, _ctx), _ctx);
  }
  {
    return kk_std_core_hnd_protect_check(resumed, k0, res, _ctx);
  }
}


// lift anonymous function
struct kk_std_core_hnd_clause_control0_fun10764__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control0_fun10764(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__628__16, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control0_fun10764(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control0_fun10764__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control0_fun10764__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control0_fun10764, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_control0_fun10765__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control0_fun10765(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control0_fun10765(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control0_fun10765__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control0_fun10765__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control0_fun10765, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_control0_fun10766__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control0_fun10766(kk_function_t _fself, kk_box_t _b_10423, kk_function_t _b_10424, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control0_fun10766(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control0_fun10766__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control0_fun10766__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control0_fun10766, kk_context());
  _self->op = op;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_control0_fun10766(kk_function_t _fself, kk_box_t _b_10423, kk_function_t _b_10424, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control0_fun10766__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control0_fun10766__t*, _fself);
  kk_function_t op = _self->op; /* ((7336) -> 7337 7339) -> 7337 7339 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_box_drop(_b_10423, _ctx);
  return kk_function_call(kk_box_t, (kk_function_t, kk_function_t, kk_context_t*), op, (op, _b_10424, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_control0_fun10765(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control0_fun10765__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control0_fun10765__t*, _fself);
  kk_function_t op = _self->op; /* ((7336) -> 7337 7339) -> 7337 7339 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  return kk_std_core_hnd_protect(kk_unit_box(kk_Unit), kk_std_core_hnd_new_clause_control0_fun10766(op, _ctx), k0, _ctx);
}
static kk_box_t kk_std_core_hnd_clause_control0_fun10764(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__628__16, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control0_fun10764__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control0_fun10764__t*, _fself);
  kk_function_t op = _self->op; /* ((7336) -> 7337 7339) -> 7337 7339 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__628__16, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to(m0, kk_std_core_hnd_new_clause_control0_fun10765(op, _ctx), _ctx);
}

kk_std_core_hnd__clause0 kk_std_core_hnd_clause_control0(kk_function_t op, kk_context_t* _ctx) { /* forall<a,e,b,c> (op : ((a) -> e c) -> e c) -> clause0<a,b,e,c> */ 
  return kk_std_core_hnd__new_Clause0(kk_std_core_hnd_new_clause_control0_fun10764(op, _ctx), _ctx);
}
extern kk_box_t kk_std_core_hnd_clause_control1_fun10768(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control1_fun10768__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control1_fun10768__t*, _fself);
  kk_function_t clause0 = _self->clause0; /* (x : 7409, k : (7410) -> 7411 7413) -> 7411 7413 */
  kk_box_t x = _self->x; /* 7409 */
  kk_drop_match(_self, {kk_function_dup(clause0);kk_box_dup(x);}, {}, _ctx)
  return kk_std_core_hnd_protect(x, clause0, k0, _ctx);
}
extern kk_box_t kk_std_core_hnd_clause_control1_fun10767(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__573__16, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control1_fun10767__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control1_fun10767__t*, _fself);
  kk_function_t clause0 = _self->clause0; /* (x : 7409, k : (7410) -> 7411 7413) -> 7411 7413 */
  kk_drop_match(_self, {kk_function_dup(clause0);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__573__16, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to(m0, kk_std_core_hnd_new_clause_control1_fun10768(clause0, x, _ctx), _ctx);
}
extern kk_box_t kk_std_core_hnd_clause_control2_fun10770(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control2_fun10770__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control2_fun10770__t*, _fself);
  kk_function_t clause0 = _self->clause0; /* (x1 : 7492, x2 : 7493, k : (7494) -> 7495 7497) -> 7495 7497 */
  kk_box_t x1 = _self->x1; /* 7492 */
  kk_box_t x2 = _self->x2; /* 7493 */
  kk_drop_match(_self, {kk_function_dup(clause0);kk_box_dup(x1);kk_box_dup(x2);}, {}, _ctx)
  return kk_std_core_hnd_protect_1(x1, x2, clause0, k0, _ctx);
}
extern kk_box_t kk_std_core_hnd_clause_control2_fun10769(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__676__16, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control2_fun10769__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control2_fun10769__t*, _fself);
  kk_function_t clause0 = _self->clause0; /* (x1 : 7492, x2 : 7493, k : (7494) -> 7495 7497) -> 7495 7497 */
  kk_drop_match(_self, {kk_function_dup(clause0);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__676__16, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to(m0, kk_std_core_hnd_new_clause_control2_fun10770(clause0, x1, x2, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_control3_fun10771__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control3_fun10771(kk_function_t _fself, kk_std_core_hnd__marker _b_10436, kk_std_core_hnd__ev _b_10437, kk_box_t _b_10438, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control3_fun10771(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control3_fun10771__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control3_fun10771__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control3_fun10771, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_control3_fun10772__t {
  struct kk_function_s _base;
  kk_box_t _b_10438;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control3_fun10772(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control3_fun10772(kk_box_t _b_10438, kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control3_fun10772__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control3_fun10772__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control3_fun10772, kk_context());
  _self->_b_10438 = _b_10438;
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_control3_fun10773__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control3_fun10773(kk_function_t _fself, kk_box_t _b_10433, kk_function_t _b_10434, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control3_fun10773(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control3_fun10773__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control3_fun10773__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control3_fun10773, kk_context());
  _self->op = op;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_control3_fun10773(kk_function_t _fself, kk_box_t _b_10433, kk_function_t _b_10434, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control3_fun10773__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control3_fun10773__t*, _fself);
  kk_function_t op = _self->op; /* (x1 : 7728, x2 : 7729, x3 : 7730, k : (7731) -> 7732 7734) -> 7732 7734 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_box_t _x10774;
  kk_std_core_types__tuple3_ _brw_10566;
  kk_box_t _x10775 = kk_box_dup(_b_10433); /*7139*/
  _brw_10566 = kk_std_core_types__tuple3__unbox(_x10775, _ctx); /*(7728, 7729, 7730)*/
  kk_box_t _brw_10567 = kk_std_core_types_fst_1(_brw_10566, _ctx); /*461*/;
  kk_std_core_types__tuple3__drop(_brw_10566, _ctx);
  _x10774 = _brw_10567; /*461*/
  kk_box_t _x10776;
  kk_std_core_types__tuple3_ _brw_10564;
  kk_box_t _x10777 = kk_box_dup(_b_10433); /*7139*/
  _brw_10564 = kk_std_core_types__tuple3__unbox(_x10777, _ctx); /*(7728, 7729, 7730)*/
  kk_box_t _brw_10565 = kk_std_core_types_snd_1(_brw_10564, _ctx); /*505*/;
  kk_std_core_types__tuple3__drop(_brw_10564, _ctx);
  _x10776 = _brw_10565; /*505*/
  kk_box_t _x10778;
  kk_std_core_types__tuple3_ _brw_10562 = kk_std_core_types__tuple3__unbox(_b_10433, _ctx); /*(7728, 7729, 7730)*/;
  kk_box_t _brw_10563 = kk_std_core_types_thd(_brw_10562, _ctx); /*549*/;
  kk_std_core_types__tuple3__drop(_brw_10562, _ctx);
  _x10778 = _brw_10563; /*549*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_function_t, kk_context_t*), op, (op, _x10774, _x10776, _x10778, _b_10434, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_control3_fun10772(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control3_fun10772__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control3_fun10772__t*, _fself);
  kk_box_t _b_10438 = _self->_b_10438; /* 51 */
  kk_function_t op = _self->op; /* (x1 : 7728, x2 : 7729, x3 : 7730, k : (7731) -> 7732 7734) -> 7732 7734 */
  kk_drop_match(_self, {kk_box_dup(_b_10438);kk_function_dup(op);}, {}, _ctx)
  return kk_std_core_hnd_protect(_b_10438, kk_std_core_hnd_new_clause_control3_fun10773(op, _ctx), k0, _ctx);
}
static kk_box_t kk_std_core_hnd_clause_control3_fun10771(kk_function_t _fself, kk_std_core_hnd__marker _b_10436, kk_std_core_hnd__ev _b_10437, kk_box_t _b_10438, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control3_fun10771__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control3_fun10771__t*, _fself);
  kk_function_t op = _self->op; /* (x1 : 7728, x2 : 7729, x3 : 7730, k : (7731) -> 7732 7734) -> 7732 7734 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(_b_10437, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to(_b_10436, kk_std_core_hnd_new_clause_control3_fun10772(_b_10438, op, _ctx), _ctx);
}

kk_std_core_hnd__clause1 kk_std_core_hnd_clause_control3(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,c,d,e,a1,b1> (op : (x1 : a, x2 : b, x3 : c, k : (d) -> e b1) -> e b1) -> clause1<(a, b, c),d,a1,e,b1> */ 
  return kk_std_core_hnd__new_Clause1(kk_std_core_hnd_new_clause_control3_fun10771(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_control4_fun10779__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control4_fun10779(kk_function_t _fself, kk_std_core_hnd__marker _b_10454, kk_std_core_hnd__ev _b_10455, kk_box_t _b_10456, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control4_fun10779(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control4_fun10779__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control4_fun10779__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control4_fun10779, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_control4_fun10780__t {
  struct kk_function_s _base;
  kk_box_t _b_10456;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control4_fun10780(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control4_fun10780(kk_box_t _b_10456, kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control4_fun10780__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control4_fun10780__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control4_fun10780, kk_context());
  _self->_b_10456 = _b_10456;
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_control4_fun10781__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_control4_fun10781(kk_function_t _fself, kk_box_t _b_10451, kk_function_t _b_10452, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_control4_fun10781(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control4_fun10781__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_control4_fun10781__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_control4_fun10781, kk_context());
  _self->op = op;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_control4_fun10781(kk_function_t _fself, kk_box_t _b_10451, kk_function_t _b_10452, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control4_fun10781__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control4_fun10781__t*, _fself);
  kk_function_t op = _self->op; /* (x1 : 8035, x2 : 8036, x3 : 8037, x4 : 8038, k : (8039) -> 8040 8042) -> 8040 8042 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_types__tuple4_ x0_10464 = kk_std_core_types__tuple4__unbox(_b_10451, _ctx); /*(8035, 8036, 8037, 8038)*/;
  kk_box_t _x10782 = kk_std_core_types_fst_2(x0_10464, _ctx); /*706*/
  kk_box_t _x10783 = kk_std_core_types_snd_2(x0_10464, _ctx); /*775*/
  kk_box_t _x10784 = kk_std_core_types_thd_1(x0_10464, _ctx); /*844*/
  kk_box_t _x10785;
  kk_box_t _brw_10561 = kk_std_core_types_field4(x0_10464, _ctx); /*900*/;
  kk_std_core_types__tuple4__dropn(x0_10464, ((int32_t)KI32(4)), _ctx);
  _x10785 = _brw_10561; /*900*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_function_t, kk_context_t*), op, (op, _x10782, _x10783, _x10784, _x10785, _b_10452, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_control4_fun10780(kk_function_t _fself, kk_function_t k0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control4_fun10780__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control4_fun10780__t*, _fself);
  kk_box_t _b_10456 = _self->_b_10456; /* 51 */
  kk_function_t op = _self->op; /* (x1 : 8035, x2 : 8036, x3 : 8037, x4 : 8038, k : (8039) -> 8040 8042) -> 8040 8042 */
  kk_drop_match(_self, {kk_box_dup(_b_10456);kk_function_dup(op);}, {}, _ctx)
  return kk_std_core_hnd_protect(_b_10456, kk_std_core_hnd_new_clause_control4_fun10781(op, _ctx), k0, _ctx);
}
static kk_box_t kk_std_core_hnd_clause_control4_fun10779(kk_function_t _fself, kk_std_core_hnd__marker _b_10454, kk_std_core_hnd__ev _b_10455, kk_box_t _b_10456, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_control4_fun10779__t* _self = kk_function_as(struct kk_std_core_hnd_clause_control4_fun10779__t*, _fself);
  kk_function_t op = _self->op; /* (x1 : 8035, x2 : 8036, x3 : 8037, x4 : 8038, k : (8039) -> 8040 8042) -> 8040 8042 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(_b_10455, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to(_b_10454, kk_std_core_hnd_new_clause_control4_fun10780(_b_10456, op, _ctx), _ctx);
}

kk_std_core_hnd__clause1 kk_std_core_hnd_clause_control4(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,c,d,a1,e,b1,c1> (op : (x1 : a, x2 : b, x3 : c, x4 : d, k : (a1) -> e c1) -> e c1) -> clause1<(a, b, c, d),a1,b1,e,c1> */ 
  return kk_std_core_hnd__new_Clause1(kk_std_core_hnd_new_clause_control4_fun10779(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_never0_fun10786__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_never0_fun10786(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__645__16, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_never0_fun10786(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never0_fun10786__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_never0_fun10786__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_never0_fun10786, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_never0_fun10787__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_never0_fun10787(kk_function_t _fself, kk_function_t ___wildcard__645__43, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_never0_fun10787(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never0_fun10787__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_never0_fun10787__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_never0_fun10787, kk_context());
  _self->op = op;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_never0_fun10787(kk_function_t _fself, kk_function_t ___wildcard__645__43, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never0_fun10787__t* _self = kk_function_as(struct kk_std_core_hnd_clause_never0_fun10787__t*, _fself);
  kk_function_t op = _self->op; /* () -> 8073 8075 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_function_drop(___wildcard__645__43, _ctx);
  return kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), op, (op, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_never0_fun10786(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__645__16, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never0_fun10786__t* _self = kk_function_as(struct kk_std_core_hnd_clause_never0_fun10786__t*, _fself);
  kk_function_t op = _self->op; /* () -> 8073 8075 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__645__16, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to_final(m0, kk_std_core_hnd_new_clause_never0_fun10787(op, _ctx), _ctx);
}

kk_std_core_hnd__clause0 kk_std_core_hnd_clause_never0(kk_function_t op, kk_context_t* _ctx) { /* forall<a,e,b,c> (op : () -> e c) -> clause0<a,b,e,c> */ 
  return kk_std_core_hnd__new_Clause0(kk_std_core_hnd_new_clause_never0_fun10786(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_never1_fun10788__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_never1_fun10788(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__585__16, kk_box_t x, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_never1_fun10788(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never1_fun10788__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_never1_fun10788__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_never1_fun10788, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_never1_fun10789__t {
  struct kk_function_s _base;
  kk_function_t op;
  kk_box_t x;
};
static kk_box_t kk_std_core_hnd_clause_never1_fun10789(kk_function_t _fself, kk_function_t ___wildcard__585__45, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_never1_fun10789(kk_function_t op, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never1_fun10789__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_never1_fun10789__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_never1_fun10789, kk_context());
  _self->op = op;
  _self->x = x;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_never1_fun10789(kk_function_t _fself, kk_function_t ___wildcard__585__45, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never1_fun10789__t* _self = kk_function_as(struct kk_std_core_hnd_clause_never1_fun10789__t*, _fself);
  kk_function_t op = _self->op; /* (8110) -> 8112 8114 */
  kk_box_t x = _self->x; /* 8110 */
  kk_drop_match(_self, {kk_function_dup(op);kk_box_dup(x);}, {}, _ctx)
  kk_function_drop(___wildcard__585__45, _ctx);
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), op, (op, x, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_never1_fun10788(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__585__16, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never1_fun10788__t* _self = kk_function_as(struct kk_std_core_hnd_clause_never1_fun10788__t*, _fself);
  kk_function_t op = _self->op; /* (8110) -> 8112 8114 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__585__16, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to_final(m0, kk_std_core_hnd_new_clause_never1_fun10789(op, x, _ctx), _ctx);
}

kk_std_core_hnd__clause1 kk_std_core_hnd_clause_never1(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,e,c,d> (op : (a) -> e d) -> clause1<a,b,c,e,d> */ 
  return kk_std_core_hnd__new_Clause1(kk_std_core_hnd_new_clause_never1_fun10788(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_never2_fun10790__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_never2_fun10790(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__701__16, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_never2_fun10790(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never2_fun10790__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_never2_fun10790__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_never2_fun10790, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_never2_fun10791__t {
  struct kk_function_s _base;
  kk_function_t op;
  kk_box_t x1;
  kk_box_t x2;
};
static kk_box_t kk_std_core_hnd_clause_never2_fun10791(kk_function_t _fself, kk_function_t ___wildcard__701__49, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_never2_fun10791(kk_function_t op, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never2_fun10791__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_never2_fun10791__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_never2_fun10791, kk_context());
  _self->op = op;
  _self->x1 = x1;
  _self->x2 = x2;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_never2_fun10791(kk_function_t _fself, kk_function_t ___wildcard__701__49, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never2_fun10791__t* _self = kk_function_as(struct kk_std_core_hnd_clause_never2_fun10791__t*, _fself);
  kk_function_t op = _self->op; /* (8154, 8155) -> 8157 8159 */
  kk_box_t x1 = _self->x1; /* 8154 */
  kk_box_t x2 = _self->x2; /* 8155 */
  kk_drop_match(_self, {kk_function_dup(op);kk_box_dup(x1);kk_box_dup(x2);}, {}, _ctx)
  kk_function_drop(___wildcard__701__49, _ctx);
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), op, (op, x1, x2, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_never2_fun10790(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ___wildcard__701__16, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never2_fun10790__t* _self = kk_function_as(struct kk_std_core_hnd_clause_never2_fun10790__t*, _fself);
  kk_function_t op = _self->op; /* (8154, 8155) -> 8157 8159 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(___wildcard__701__16, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to_final(m0, kk_std_core_hnd_new_clause_never2_fun10791(op, x1, x2, _ctx), _ctx);
}

kk_std_core_hnd__clause2 kk_std_core_hnd_clause_never2(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,c,e,d,a1> (op : (a, b) -> e a1) -> clause2<a,b,c,d,e,a1> */ 
  return kk_std_core_hnd__new_Clause2(kk_std_core_hnd_new_clause_never2_fun10790(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_never3_fun10792__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_never3_fun10792(kk_function_t _fself, kk_std_core_hnd__marker _b_10467, kk_std_core_hnd__ev _b_10468, kk_box_t _b_10469, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_never3_fun10792(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never3_fun10792__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_never3_fun10792__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_never3_fun10792, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_never3_fun10793__t {
  struct kk_function_s _base;
  kk_box_t _b_10469;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_never3_fun10793(kk_function_t _fself, kk_function_t ___wildcard__585__45, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_never3_fun10793(kk_box_t _b_10469, kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never3_fun10793__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_never3_fun10793__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_never3_fun10793, kk_context());
  _self->_b_10469 = _b_10469;
  _self->op = op;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_never3_fun10793(kk_function_t _fself, kk_function_t ___wildcard__585__45, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never3_fun10793__t* _self = kk_function_as(struct kk_std_core_hnd_clause_never3_fun10793__t*, _fself);
  kk_box_t _b_10469 = _self->_b_10469; /* 51 */
  kk_function_t op = _self->op; /* (8389, 8390, 8391) -> 8393 8395 */
  kk_drop_match(_self, {kk_box_dup(_b_10469);kk_function_dup(op);}, {}, _ctx)
  kk_function_drop(___wildcard__585__45, _ctx);
  kk_box_t _x10794;
  kk_std_core_types__tuple3_ _brw_10559;
  kk_box_t _x10795 = kk_box_dup(_b_10469); /*51*/
  _brw_10559 = kk_std_core_types__tuple3__unbox(_x10795, _ctx); /*(8389, 8390, 8391)*/
  kk_box_t _brw_10560 = kk_std_core_types_fst_1(_brw_10559, _ctx); /*461*/;
  kk_std_core_types__tuple3__drop(_brw_10559, _ctx);
  _x10794 = _brw_10560; /*461*/
  kk_box_t _x10796;
  kk_std_core_types__tuple3_ _brw_10557;
  kk_box_t _x10797 = kk_box_dup(_b_10469); /*51*/
  _brw_10557 = kk_std_core_types__tuple3__unbox(_x10797, _ctx); /*(8389, 8390, 8391)*/
  kk_box_t _brw_10558 = kk_std_core_types_snd_1(_brw_10557, _ctx); /*505*/;
  kk_std_core_types__tuple3__drop(_brw_10557, _ctx);
  _x10796 = _brw_10558; /*505*/
  kk_box_t _x10798;
  kk_std_core_types__tuple3_ _brw_10555 = kk_std_core_types__tuple3__unbox(_b_10469, _ctx); /*(8389, 8390, 8391)*/;
  kk_box_t _brw_10556 = kk_std_core_types_thd(_brw_10555, _ctx); /*549*/;
  kk_std_core_types__tuple3__drop(_brw_10555, _ctx);
  _x10798 = _brw_10556; /*549*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), op, (op, _x10794, _x10796, _x10798, _ctx));
}
static kk_box_t kk_std_core_hnd_clause_never3_fun10792(kk_function_t _fself, kk_std_core_hnd__marker _b_10467, kk_std_core_hnd__ev _b_10468, kk_box_t _b_10469, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_never3_fun10792__t* _self = kk_function_as(struct kk_std_core_hnd_clause_never3_fun10792__t*, _fself);
  kk_function_t op = _self->op; /* (8389, 8390, 8391) -> 8393 8395 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(_b_10468, ((int32_t)KI32(3)), _ctx);
  return kk_std_core_hnd_yield_to_final(_b_10467, kk_std_core_hnd_new_clause_never3_fun10793(_b_10469, op, _ctx), _ctx);
}

kk_std_core_hnd__clause1 kk_std_core_hnd_clause_never3(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,c,d,e,a1,b1> (op : (a, b, c) -> e b1) -> clause1<(a, b, c),d,a1,e,b1> */ 
  return kk_std_core_hnd__new_Clause1(kk_std_core_hnd_new_clause_never3_fun10792(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_tail_noyield3_fun10799__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_tail_noyield3_fun10799(kk_function_t _fself, kk_std_core_hnd__marker _b_10475, kk_std_core_hnd__ev _b_10476, kk_box_t _b_10477, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_tail_noyield3_fun10799(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail_noyield3_fun10799__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_tail_noyield3_fun10799__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_tail_noyield3_fun10799, kk_context());
  _self->op = op;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_tail_noyield3_fun10799(kk_function_t _fself, kk_std_core_hnd__marker _b_10475, kk_std_core_hnd__ev _b_10476, kk_box_t _b_10477, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail_noyield3_fun10799__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail_noyield3_fun10799__t*, _fself);
  kk_function_t op = _self->op; /* (8625, 8626, 8627) -> 8629 8628 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(_b_10476, ((int32_t)KI32(3)), _ctx);
  kk_box_t _x10800;
  kk_std_core_types__tuple3_ _brw_10553;
  kk_box_t _x10801 = kk_box_dup(_b_10477); /*51*/
  _brw_10553 = kk_std_core_types__tuple3__unbox(_x10801, _ctx); /*(8625, 8626, 8627)*/
  kk_box_t _brw_10554 = kk_std_core_types_fst_1(_brw_10553, _ctx); /*461*/;
  kk_std_core_types__tuple3__drop(_brw_10553, _ctx);
  _x10800 = _brw_10554; /*461*/
  kk_box_t _x10802;
  kk_std_core_types__tuple3_ _brw_10551;
  kk_box_t _x10803 = kk_box_dup(_b_10477); /*51*/
  _brw_10551 = kk_std_core_types__tuple3__unbox(_x10803, _ctx); /*(8625, 8626, 8627)*/
  kk_box_t _brw_10552 = kk_std_core_types_snd_1(_brw_10551, _ctx); /*505*/;
  kk_std_core_types__tuple3__drop(_brw_10551, _ctx);
  _x10802 = _brw_10552; /*505*/
  kk_box_t _x10804;
  kk_std_core_types__tuple3_ _brw_10549 = kk_std_core_types__tuple3__unbox(_b_10477, _ctx); /*(8625, 8626, 8627)*/;
  kk_box_t _brw_10550 = kk_std_core_types_thd(_brw_10549, _ctx); /*549*/;
  kk_std_core_types__tuple3__drop(_brw_10549, _ctx);
  _x10804 = _brw_10550; /*549*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), op, (op, _x10800, _x10802, _x10804, _ctx));
}

kk_std_core_hnd__clause1 kk_std_core_hnd_clause_tail_noyield3(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,c,d,e,a1,b1> (op : (a, b, c) -> e d) -> clause1<(a, b, c),d,a1,e,b1> */ 
  return kk_std_core_hnd__new_Clause1(kk_std_core_hnd_new_clause_tail_noyield3_fun10799(op, _ctx), _ctx);
}
 
// extra under1x to make under1 inlineable


// lift anonymous function
struct kk_std_core_hnd_under1x_fun10806__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
};
static kk_box_t kk_std_core_hnd_under1x_fun10806(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_under1x_fun10806(kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under1x_fun10806__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_under1x_fun10806__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_under1x_fun10806, kk_context());
  _self->ev = ev;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_under1x_fun10806(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under1x_fun10806__t* _self = kk_function_as(struct kk_std_core_hnd_under1x_fun10806__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<8692> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  return kk_std_core_hnd_under1x(ev, cont, res, _ctx);
}

kk_box_t kk_std_core_hnd_under1x(kk_std_core_hnd__ev ev, kk_function_t op, kk_box_t x, kk_context_t* _ctx) { /* forall<a,b,e,c> (ev : ev<c>, op : (a) -> e b, x : a) -> e b */ 
  kk_evv_t w0;
  kk_std_core_hnd__ev _x10805 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<8692>*/
  w0 = kk_std_core_hnd_evv_swap_with(_x10805, _ctx); /*std/core/hnd/evv<_8637>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), op, (op, x, _ctx)); /*8690*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    kk_evv_drop(w0, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_under1x_fun10806(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_evv_set(w0,kk_context());
    return y;
  }
}
extern kk_box_t kk_std_core_hnd_under1_fun10808(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under1_fun10808__t* _self = kk_function_as(struct kk_std_core_hnd_under1_fun10808__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<8753> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  return kk_std_core_hnd_under1x(ev, cont, res, _ctx);
}
extern kk_box_t kk_std_core_hnd_under0_fun10812(kk_function_t _fself, kk_function_t cont0, kk_box_t res0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under0_fun10812__t* _self = kk_function_as(struct kk_std_core_hnd_under0_fun10812__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<8806> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  return kk_std_core_hnd_under1x(ev, cont0, res0, _ctx);
}
extern kk_box_t kk_std_core_hnd_under0_fun10810(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under0_fun10810__t* _self = kk_function_as(struct kk_std_core_hnd_under0_fun10810__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<8806> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  kk_evv_t w00;
  kk_std_core_hnd__ev _x10811 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<8806>*/
  w00 = kk_std_core_hnd_evv_swap_with(_x10811, _ctx); /*std/core/hnd/evv<_8698>*/
  kk_box_t y0 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, res, _ctx)); /*8804*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(y0, _ctx);
    kk_evv_drop(w00, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_under0_fun10812(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_evv_set(w00,kk_context());
    return y0;
  }
}


// lift anonymous function
struct kk_std_core_hnd_clause_tail0_fun10813__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_tail0_fun10813(kk_function_t _fself, kk_std_core_hnd__marker ___wildcard__633__14, kk_std_core_hnd__ev ev, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_tail0_fun10813(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail0_fun10813__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_tail0_fun10813__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_tail0_fun10813, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_tail0_fun10815__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
};
static kk_box_t kk_std_core_hnd_clause_tail0_fun10815(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_tail0_fun10815(kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail0_fun10815__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_tail0_fun10815__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_tail0_fun10815, kk_context());
  _self->ev = ev;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_tail0_fun10817__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
};
static kk_box_t kk_std_core_hnd_clause_tail0_fun10817(kk_function_t _fself, kk_function_t cont0, kk_box_t res0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_tail0_fun10817(kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail0_fun10817__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_tail0_fun10817__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_tail0_fun10817, kk_context());
  _self->ev = ev;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_tail0_fun10817(kk_function_t _fself, kk_function_t cont0, kk_box_t res0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail0_fun10817__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail0_fun10817__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<8837> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  return kk_std_core_hnd_under1x(ev, cont0, res0, _ctx);
}
static kk_box_t kk_std_core_hnd_clause_tail0_fun10815(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail0_fun10815__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail0_fun10815__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<8837> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  kk_evv_t w00;
  kk_std_core_hnd__ev _x10816 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<8837>*/
  w00 = kk_std_core_hnd_evv_swap_with(_x10816, _ctx); /*std/core/hnd/evv<_8698>*/
  kk_box_t y0 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, res, _ctx)); /*8836*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(y0, _ctx);
    kk_evv_drop(w00, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_clause_tail0_fun10817(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_evv_set(w00,kk_context());
    return y0;
  }
}
static kk_box_t kk_std_core_hnd_clause_tail0_fun10813(kk_function_t _fself, kk_std_core_hnd__marker ___wildcard__633__14, kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail0_fun10813__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail0_fun10813__t*, _fself);
  kk_function_t op = _self->op; /* () -> 8834 8836 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_evv_t w0;
  kk_std_core_hnd__ev _x10814 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<8837>*/
  w0 = kk_std_core_hnd_evv_swap_with(_x10814, _ctx); /*std/core/hnd/evv<_8758>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), op, (op, _ctx)); /*8836*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w0,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_clause_tail0_fun10815(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    return y;
  }
}

kk_std_core_hnd__clause0 kk_std_core_hnd_clause_tail0(kk_function_t op, kk_context_t* _ctx) { /* forall<e,a,b,c> (op : () -> e b) -> clause0<b,c,e,a> */ 
  return kk_std_core_hnd__new_Clause0(kk_std_core_hnd_new_clause_tail0_fun10813(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_tail1_fun10818__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_tail1_fun10818(kk_function_t _fself, kk_std_core_hnd__marker ___wildcard__577__14, kk_std_core_hnd__ev ev, kk_box_t x, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_tail1_fun10818(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail1_fun10818__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_tail1_fun10818__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_tail1_fun10818, kk_context());
  _self->op = op;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_clause_tail1_fun10820__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
};
static kk_box_t kk_std_core_hnd_clause_tail1_fun10820(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_tail1_fun10820(kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail1_fun10820__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_tail1_fun10820__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_tail1_fun10820, kk_context());
  _self->ev = ev;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_tail1_fun10820(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail1_fun10820__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail1_fun10820__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<8875> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  return kk_std_core_hnd_under1x(ev, cont, res, _ctx);
}
static kk_box_t kk_std_core_hnd_clause_tail1_fun10818(kk_function_t _fself, kk_std_core_hnd__marker ___wildcard__577__14, kk_std_core_hnd__ev ev, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail1_fun10818__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail1_fun10818__t*, _fself);
  kk_function_t op = _self->op; /* (8873) -> 8871 8874 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_evv_t w0;
  kk_std_core_hnd__ev _x10819 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<8875>*/
  w0 = kk_std_core_hnd_evv_swap_with(_x10819, _ctx); /*std/core/hnd/evv<_8698>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), op, (op, x, _ctx)); /*8874*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    kk_evv_drop(w0, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_clause_tail1_fun10820(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_evv_set(w0,kk_context());
    return y;
  }
}

kk_std_core_hnd__clause1 kk_std_core_hnd_clause_tail1(kk_function_t op, kk_context_t* _ctx) { /* forall<e,a,b,c,d> (op : (b) -> e c) -> clause1<b,c,d,e,a> */ 
  return kk_std_core_hnd__new_Clause1(kk_std_core_hnd_new_clause_tail1_fun10818(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_under2_fun10822__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
};
static kk_box_t kk_std_core_hnd_under2_fun10822(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_under2_fun10822(kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under2_fun10822__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_under2_fun10822__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_under2_fun10822, kk_context());
  _self->ev = ev;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_under2_fun10824__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
};
static kk_box_t kk_std_core_hnd_under2_fun10824(kk_function_t _fself, kk_function_t cont0, kk_box_t res0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_under2_fun10824(kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under2_fun10824__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_under2_fun10824__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_under2_fun10824, kk_context());
  _self->ev = ev;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_under2_fun10824(kk_function_t _fself, kk_function_t cont0, kk_box_t res0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under2_fun10824__t* _self = kk_function_as(struct kk_std_core_hnd_under2_fun10824__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<8938> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  return kk_std_core_hnd_under1x(ev, cont0, res0, _ctx);
}
static kk_box_t kk_std_core_hnd_under2_fun10822(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under2_fun10822__t* _self = kk_function_as(struct kk_std_core_hnd_under2_fun10822__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<8938> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  kk_evv_t w00;
  kk_std_core_hnd__ev _x10823 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<8938>*/
  w00 = kk_std_core_hnd_evv_swap_with(_x10823, _ctx); /*std/core/hnd/evv<_8698>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, res, _ctx)); /*8936*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    kk_evv_drop(w00, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_under2_fun10824(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_evv_set(w00,kk_context());
    return y;
  }
}

kk_box_t kk_std_core_hnd_under2(kk_std_core_hnd__ev ev, kk_function_t op, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx) { /* forall<a,b,c,e,d> (ev : ev<d>, op : (a, b) -> e c, x1 : a, x2 : b) -> e c */ 
  kk_evv_t w0;
  kk_std_core_hnd__ev _x10821 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<8938>*/
  w0 = kk_std_core_hnd_evv_swap_with(_x10821, _ctx); /*std/core/hnd/evv<_8882>*/
  kk_box_t z = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_context_t*), op, (op, x1, x2, _ctx)); /*8936*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w0,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(z, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_under2_fun10822(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    return z;
  }
}
extern kk_box_t kk_std_core_hnd_clause_tail2_fun10825(kk_function_t _fself, kk_std_core_hnd__marker m0, kk_std_core_hnd__ev ev, kk_box_t x1, kk_box_t x2, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail2_fun10825__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail2_fun10825__t*, _fself);
  kk_function_t op = _self->op; /* (8980, 8981) -> 8978 8982 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  return kk_std_core_hnd_under2(ev, op, x1, x2, _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_tail3_fun10826__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_tail3_fun10826(kk_function_t _fself, kk_std_core_hnd__marker _b_10483, kk_std_core_hnd__ev _b_10484, kk_box_t _b_10485, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_tail3_fun10826(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail3_fun10826__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_tail3_fun10826__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_tail3_fun10826, kk_context());
  _self->op = op;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_tail3_fun10826(kk_function_t _fself, kk_std_core_hnd__marker _b_10483, kk_std_core_hnd__ev _b_10484, kk_box_t _b_10485, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail3_fun10826__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail3_fun10826__t*, _fself);
  kk_function_t op = _self->op; /* (9213, 9214, 9215) -> 9217 9216 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(_b_10484, ((int32_t)KI32(3)), _ctx);
  kk_box_t _x10827;
  kk_std_core_types__tuple3_ _brw_10538;
  kk_box_t _x10828 = kk_box_dup(_b_10485); /*51*/
  _brw_10538 = kk_std_core_types__tuple3__unbox(_x10828, _ctx); /*(9213, 9214, 9215)*/
  kk_box_t _brw_10539 = kk_std_core_types_fst_1(_brw_10538, _ctx); /*461*/;
  kk_std_core_types__tuple3__drop(_brw_10538, _ctx);
  _x10827 = _brw_10539; /*461*/
  kk_box_t _x10829;
  kk_std_core_types__tuple3_ _brw_10536;
  kk_box_t _x10830 = kk_box_dup(_b_10485); /*51*/
  _brw_10536 = kk_std_core_types__tuple3__unbox(_x10830, _ctx); /*(9213, 9214, 9215)*/
  kk_box_t _brw_10537 = kk_std_core_types_snd_1(_brw_10536, _ctx); /*505*/;
  kk_std_core_types__tuple3__drop(_brw_10536, _ctx);
  _x10829 = _brw_10537; /*505*/
  kk_box_t _x10831;
  kk_std_core_types__tuple3_ _brw_10534 = kk_std_core_types__tuple3__unbox(_b_10485, _ctx); /*(9213, 9214, 9215)*/;
  kk_box_t _brw_10535 = kk_std_core_types_thd(_brw_10534, _ctx); /*549*/;
  kk_std_core_types__tuple3__drop(_brw_10534, _ctx);
  _x10831 = _brw_10535; /*549*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), op, (op, _x10827, _x10829, _x10831, _ctx));
}

kk_std_core_hnd__clause1 kk_std_core_hnd_clause_tail3(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,c,d,e,a1,b1> (op : (a, b, c) -> e d) -> clause1<(a, b, c),d,a1,e,b1> */ 
  return kk_std_core_hnd__new_Clause1(kk_std_core_hnd_new_clause_tail3_fun10826(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_clause_tail4_fun10832__t {
  struct kk_function_s _base;
  kk_function_t op;
};
static kk_box_t kk_std_core_hnd_clause_tail4_fun10832(kk_function_t _fself, kk_std_core_hnd__marker _b_10491, kk_std_core_hnd__ev _b_10492, kk_box_t _b_10493, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_clause_tail4_fun10832(kk_function_t op, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail4_fun10832__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_clause_tail4_fun10832__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_clause_tail4_fun10832, kk_context());
  _self->op = op;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_clause_tail4_fun10832(kk_function_t _fself, kk_std_core_hnd__marker _b_10491, kk_std_core_hnd__ev _b_10492, kk_box_t _b_10493, kk_context_t* _ctx) {
  struct kk_std_core_hnd_clause_tail4_fun10832__t* _self = kk_function_as(struct kk_std_core_hnd_clause_tail4_fun10832__t*, _fself);
  kk_function_t op = _self->op; /* (9519, 9520, 9521, 9522) -> 9524 9523 */
  kk_drop_match(_self, {kk_function_dup(op);}, {}, _ctx)
  kk_std_core_hnd__ev_dropn(_b_10492, ((int32_t)KI32(3)), _ctx);
  kk_std_core_types__tuple4_ x_10497 = kk_std_core_types__tuple4__unbox(_b_10493, _ctx); /*(9519, 9520, 9521, 9522)*/;
  kk_box_t _x10833 = kk_std_core_types_fst_2(x_10497, _ctx); /*706*/
  kk_box_t _x10834 = kk_std_core_types_snd_2(x_10497, _ctx); /*775*/
  kk_box_t _x10835 = kk_std_core_types_thd_1(x_10497, _ctx); /*844*/
  kk_box_t _x10836;
  kk_box_t _brw_10533 = kk_std_core_types_field4(x_10497, _ctx); /*900*/;
  kk_std_core_types__tuple4__dropn(x_10497, ((int32_t)KI32(4)), _ctx);
  _x10836 = _brw_10533; /*900*/
  return kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), op, (op, _x10833, _x10834, _x10835, _x10836, _ctx));
}

kk_std_core_hnd__clause1 kk_std_core_hnd_clause_tail4(kk_function_t op, kk_context_t* _ctx) { /* forall<a,b,c,d,a1,e,b1,c1> (op : (a, b, c, d) -> e a1) -> clause1<(a, b, c, d),a1,b1,e,c1> */ 
  return kk_std_core_hnd__new_Clause1(kk_std_core_hnd_new_clause_tail4_fun10832(op, _ctx), _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_finally_prompt_fun10837__t {
  struct kk_function_s _base;
  kk_function_t fin;
};
static kk_box_t kk_std_core_hnd_finally_prompt_fun10837(kk_function_t _fself, kk_function_t cont, kk_box_t x, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_finally_prompt_fun10837(kk_function_t fin, kk_context_t* _ctx) {
  struct kk_std_core_hnd_finally_prompt_fun10837__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_finally_prompt_fun10837__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_finally_prompt_fun10837, kk_context());
  _self->fin = fin;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_finally_prompt_fun10837(kk_function_t _fself, kk_function_t cont, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_finally_prompt_fun10837__t* _self = kk_function_as(struct kk_std_core_hnd_finally_prompt_fun10837__t*, _fself);
  kk_function_t fin = _self->fin; /* () -> 9616 () */
  kk_drop_match(_self, {kk_function_dup(fin);}, {}, _ctx)
  kk_box_t _x10838 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, x, _ctx)); /*9615*/
  return kk_std_core_hnd_finally_prompt(fin, _x10838, _ctx);
}


// lift anonymous function
struct kk_std_core_hnd_finally_prompt_fun10839__t {
  struct kk_function_s _base;
  kk_std_core_hnd__yield_info yld;
};
static kk_box_t kk_std_core_hnd_finally_prompt_fun10839(kk_function_t _fself, kk_box_t ___wildcard__420__43, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_finally_prompt_fun10839(kk_std_core_hnd__yield_info yld, kk_context_t* _ctx) {
  struct kk_std_core_hnd_finally_prompt_fun10839__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_finally_prompt_fun10839__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_finally_prompt_fun10839, kk_context());
  _self->yld = yld;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_finally_prompt_fun10839(kk_function_t _fself, kk_box_t ___wildcard__420__43, kk_context_t* _ctx) {
  struct kk_std_core_hnd_finally_prompt_fun10839__t* _self = kk_function_as(struct kk_std_core_hnd_finally_prompt_fun10839__t*, _fself);
  kk_std_core_hnd__yield_info yld = _self->yld; /* std/core/hnd/yield-info */
  kk_drop_match(_self, {kk_std_core_hnd__yield_info_dup(yld);}, {}, _ctx)
  kk_box_drop(___wildcard__420__43, _ctx);
  return kk_std_core_hnd_unsafe_reyield(yld, _ctx);
}

kk_box_t kk_std_core_hnd_finally_prompt(kk_function_t fin, kk_box_t res, kk_context_t* _ctx) { /* forall<a,e> (fin : () -> e (), res : a) -> e a */ 
  if (kk_yielding(kk_context())) {
    kk_box_drop(res, _ctx);
    bool _match_10531 = kk_yielding_non_final(kk_context()); /*bool*/;
    if (_match_10531) {
      return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_finally_prompt_fun10837(fin, _ctx), _ctx);
    }
    {
      kk_std_core_hnd__yield_info yld = kk_std_core_hnd_yield_capture(_ctx); /*std/core/hnd/yield-info*/;
      kk_unit_t __0 = kk_Unit;
      kk_function_call(kk_unit_t, (kk_function_t, kk_context_t*), fin, (fin, _ctx));
      if (kk_yielding(kk_context())) {
        return kk_std_core_hnd_yield_extend(kk_std_core_hnd_new_finally_prompt_fun10839(yld, _ctx), _ctx);
      }
      {
        return kk_std_core_hnd_unsafe_reyield(yld, _ctx);
      }
    }
  }
  {
    kk_unit_t __ = kk_Unit;
    kk_function_call(kk_unit_t, (kk_function_t, kk_context_t*), fin, (fin, _ctx));
    return res;
  }
}


// lift anonymous function
struct kk_std_core_hnd_initially_prompt_fun10841__t {
  struct kk_function_s _base;
  kk_ref_t count;
  kk_function_t init;
};
static kk_box_t kk_std_core_hnd_initially_prompt_fun10841(kk_function_t _fself, kk_function_t cont, kk_box_t x, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_initially_prompt_fun10841(kk_ref_t count, kk_function_t init, kk_context_t* _ctx) {
  struct kk_std_core_hnd_initially_prompt_fun10841__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_initially_prompt_fun10841__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_initially_prompt_fun10841, kk_context());
  _self->count = count;
  _self->init = init;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_initially_prompt_fun10847__t {
  struct kk_function_s _base;
  kk_function_t cont;
  kk_function_t init;
  kk_box_t x;
};
static kk_box_t kk_std_core_hnd_initially_prompt_fun10847(kk_function_t _fself, kk_box_t ___wildcard__462__49, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_initially_prompt_fun10847(kk_function_t cont, kk_function_t init, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_initially_prompt_fun10847__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_initially_prompt_fun10847__t, 4, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_initially_prompt_fun10847, kk_context());
  _self->cont = cont;
  _self->init = init;
  _self->x = x;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_initially_prompt_fun10847(kk_function_t _fself, kk_box_t ___wildcard__462__49, kk_context_t* _ctx) {
  struct kk_std_core_hnd_initially_prompt_fun10847__t* _self = kk_function_as(struct kk_std_core_hnd_initially_prompt_fun10847__t*, _fself);
  kk_function_t cont = _self->cont; /* (9760) -> 9770 9769 */
  kk_function_t init = _self->init; /* (int) -> 9770 () */
  kk_box_t x = _self->x; /* 9760 */
  kk_drop_match(_self, {kk_function_dup(cont);kk_function_dup(init);kk_box_dup(x);}, {}, _ctx)
  kk_box_drop(___wildcard__462__49, _ctx);
  kk_box_t _x10848 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, x, _ctx)); /*9769*/
  return kk_std_core_hnd_initially_prompt(init, _x10848, _ctx);
}
static kk_box_t kk_std_core_hnd_initially_prompt_fun10841(kk_function_t _fself, kk_function_t cont, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_initially_prompt_fun10841__t* _self = kk_function_as(struct kk_std_core_hnd_initially_prompt_fun10841__t*, _fself);
  kk_ref_t count = _self->count; /* ref<global,int> */
  kk_function_t init = _self->init; /* (int) -> 9770 () */
  kk_drop_match(_self, {kk_ref_dup(count);kk_function_dup(init);}, {}, _ctx)
  kk_integer_t cnt;
  kk_box_t _x10842;
  kk_ref_t _x10843 = kk_ref_dup(count); /*ref<global,int>*/
  _x10842 = kk_ref_get(_x10843,kk_context()); /*184*/
  cnt = kk_integer_unbox(_x10842); /*int*/
  kk_integer_t _b_10505_10503;
  kk_integer_t _x10844 = kk_integer_dup(cnt); /*int*/
  _b_10505_10503 = kk_integer_add(_x10844,(kk_integer_from_small(1)),kk_context()); /*int*/
  kk_unit_t __ = kk_Unit;
  kk_unit_t _brw_10530 = kk_Unit;
  kk_ref_set_borrow(count,(kk_integer_box(_b_10505_10503)),kk_context());
  kk_ref_drop(count, _ctx);
  _brw_10530;
  bool _match_10526;
  kk_integer_t _brw_10528 = kk_integer_from_small(0); /*int*/;
  bool _brw_10529 = kk_integer_eq_borrow(cnt,_brw_10528,kk_context()); /*bool*/;
  kk_integer_drop(_brw_10528, _ctx);
  _match_10526 = _brw_10529; /*bool*/
  if (_match_10526) {
    kk_integer_drop(cnt, _ctx);
    kk_box_t _x10845 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, x, _ctx)); /*9769*/
    return kk_std_core_hnd_initially_prompt(init, _x10845, _ctx);
  }
  {
    kk_unit_t r = kk_Unit;
    kk_function_t _x10846 = kk_function_dup(init); /*(int) -> 9770 ()*/
    kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), _x10846, (_x10846, cnt, _ctx));
    if (kk_yielding(kk_context())) {
      return kk_std_core_hnd_yield_extend(kk_std_core_hnd_new_initially_prompt_fun10847(cont, init, x, _ctx), _ctx);
    }
    {
      kk_box_t _x10849 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, x, _ctx)); /*9769*/
      return kk_std_core_hnd_initially_prompt(init, _x10849, _ctx);
    }
  }
}

kk_box_t kk_std_core_hnd_initially_prompt(kk_function_t init, kk_box_t res, kk_context_t* _ctx) { /* forall<a,e> (init : (int) -> e (), res : a) -> e a */ 
  bool _match_10525 = kk_yielding_non_final(kk_context()); /*bool*/;
  if (_match_10525) {
    kk_box_drop(res, _ctx);
    kk_ref_t count = kk_ref_alloc((kk_integer_box(kk_integer_from_small(0))),kk_context()); /*ref<global,int>*/;
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_initially_prompt_fun10841(count, init, _ctx), _ctx);
  }
  {
    kk_function_drop(init, _ctx);
    return res;
  }
}


// lift anonymous function
struct kk_std_core_hnd_initially_fun10851__t {
  struct kk_function_s _base;
  kk_function_t action;
  kk_function_t init;
};
static kk_box_t kk_std_core_hnd_initially_fun10851(kk_function_t _fself, kk_box_t _b_10507, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_initially_fun10851(kk_function_t action, kk_function_t init, kk_context_t* _ctx) {
  struct kk_std_core_hnd_initially_fun10851__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_initially_fun10851__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_initially_fun10851, kk_context());
  _self->action = action;
  _self->init = init;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_initially_fun10851(kk_function_t _fself, kk_box_t _b_10507, kk_context_t* _ctx) {
  struct kk_std_core_hnd_initially_fun10851__t* _self = kk_function_as(struct kk_std_core_hnd_initially_fun10851__t*, _fself);
  kk_function_t action = _self->action; /* () -> 9806 9805 */
  kk_function_t init = _self->init; /* (int) -> 9806 () */
  kk_drop_match(_self, {kk_function_dup(action);kk_function_dup(init);}, {}, _ctx)
  kk_box_drop(_b_10507, _ctx);
  kk_box_t _x10852 = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), action, (action, _ctx)); /*9805*/
  return kk_std_core_hnd_initially_prompt(init, _x10852, _ctx);
}

kk_box_t kk_std_core_hnd_initially(kk_function_t init, kk_function_t action, kk_context_t* _ctx) { /* forall<a,e> (init : (int) -> e (), action : () -> e a) -> e a */ 
  kk_unit_t __ = kk_Unit;
  kk_function_t _x10850 = kk_function_dup(init); /*(int) -> 9806 ()*/
  kk_function_call(kk_unit_t, (kk_function_t, kk_integer_t, kk_context_t*), _x10850, (_x10850, kk_integer_from_small(0), _ctx));
  if (kk_yielding(kk_context())) {
    return kk_std_core_hnd_yield_extend(kk_std_core_hnd_new_initially_fun10851(action, init, _ctx), _ctx);
  }
  {
    kk_box_t _x10853 = kk_function_call(kk_box_t, (kk_function_t, kk_context_t*), action, (action, _ctx)); /*9805*/
    return kk_std_core_hnd_initially_prompt(init, _x10853, _ctx);
  }
}


// lift anonymous function
struct kk_std_core_hnd_prompt_local_var_fun10855__t {
  struct kk_function_s _base;
  kk_ref_t loc;
  kk_box_t v;
};
static kk_box_t kk_std_core_hnd_prompt_local_var_fun10855(kk_function_t _fself, kk_function_t cont, kk_box_t x, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_prompt_local_var_fun10855(kk_ref_t loc, kk_box_t v, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_local_var_fun10855__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_prompt_local_var_fun10855__t, 3, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_prompt_local_var_fun10855, kk_context());
  _self->loc = loc;
  _self->v = v;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_prompt_local_var_fun10855(kk_function_t _fself, kk_function_t cont, kk_box_t x, kk_context_t* _ctx) {
  struct kk_std_core_hnd_prompt_local_var_fun10855__t* _self = kk_function_as(struct kk_std_core_hnd_prompt_local_var_fun10855__t*, _fself);
  kk_ref_t loc = _self->loc; /* local-var<9899,9896> */
  kk_box_t v = _self->v; /* 9896 */
  kk_drop_match(_self, {kk_ref_dup(loc);kk_box_dup(v);}, {}, _ctx)
  kk_unit_t __0 = kk_Unit;
  (kk_ref_set_borrow(loc,v,kk_context()));
  kk_box_t _x10856 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, x, _ctx)); /*9897*/
  return kk_std_core_hnd_prompt_local_var(loc, _x10856, _ctx);
}

kk_box_t kk_std_core_hnd_prompt_local_var(kk_ref_t loc, kk_box_t res, kk_context_t* _ctx) { /* forall<a,b,h> (loc : local-var<h,a>, res : b) -> <div,local<h>> b */ 
  if (kk_yielding(kk_context())) {
    kk_box_drop(res, _ctx);
    kk_box_t v;
    kk_ref_t _x10854 = kk_ref_dup(loc); /*local-var<9899,9896>*/
    v = (kk_ref_get(_x10854,kk_context())); /*9896*/
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_prompt_local_var_fun10855(loc, v, _ctx), _ctx);
  }
  {
    kk_ref_drop(loc, _ctx);
    return res;
  }
}


// lift anonymous function
struct kk_std_core_hnd_try_finalize_prompt_fun10859__t {
  struct kk_function_s _base;
};
static kk_box_t kk_std_core_hnd_try_finalize_prompt_fun10859(kk_function_t _fself, kk_function_t _b_10511, kk_box_t _b_10512, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_try_finalize_prompt_fun10859(kk_context_t* _ctx) {
  kk_define_static_function(_fself, kk_std_core_hnd_try_finalize_prompt_fun10859, _ctx)
  return kk_function_dup(_fself);
}

static kk_box_t kk_std_core_hnd_try_finalize_prompt_fun10859(kk_function_t _fself, kk_function_t _b_10511, kk_box_t _b_10512, kk_context_t* _ctx) {
  KK_UNUSED(_fself);
  kk_std_core_types__either _x10860;
  kk_box_t _x10861 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _b_10511, (_b_10511, _b_10512, _ctx)); /*3881*/
  _x10860 = kk_std_core_hnd_try_finalize_prompt(_x10861, _ctx); /*either<std/core/hnd/yield-info,1860>*/
  return kk_std_core_types__either_box(_x10860, _ctx);
}

kk_std_core_types__either kk_std_core_hnd_try_finalize_prompt(kk_box_t res, kk_context_t* _ctx) { /* forall<a,e> (res : a) -> e either<yield-info,a> */ 
  bool _match_10523 = kk_yielding_non_final(kk_context()); /*bool*/;
  if (_match_10523) {
    kk_box_drop(res, _ctx);
    kk_box_t _x10858 = kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_try_finalize_prompt_fun10859(_ctx), _ctx); /*3883*/
    return kk_std_core_types__either_unbox(_x10858, _ctx);
  }
  if (kk_yielding(kk_context())) {
    kk_box_drop(res, _ctx);
    kk_std_core_hnd__yield_info _b_10515_10513 = kk_std_core_hnd_yield_capture(_ctx); /*std/core/hnd/yield-info*/;
    return kk_std_core_types__new_Left(kk_std_core_hnd__yield_info_box(_b_10515_10513, _ctx), _ctx);
  }
  {
    return kk_std_core_types__new_Right(res, _ctx);
  }
}


// lift anonymous function
struct kk_std_core_hnd_under3_fun10863__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
};
static kk_box_t kk_std_core_hnd_under3_fun10863(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_under3_fun10863(kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under3_fun10863__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_under3_fun10863__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_under3_fun10863, kk_context());
  _self->ev = ev;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_under3_fun10865__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
};
static kk_box_t kk_std_core_hnd_under3_fun10865(kk_function_t _fself, kk_function_t cont0, kk_box_t res0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_under3_fun10865(kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under3_fun10865__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_under3_fun10865__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_under3_fun10865, kk_context());
  _self->ev = ev;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_under3_fun10865(kk_function_t _fself, kk_function_t cont0, kk_box_t res0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under3_fun10865__t* _self = kk_function_as(struct kk_std_core_hnd_under3_fun10865__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<10141> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  return kk_std_core_hnd_under1x(ev, cont0, res0, _ctx);
}
static kk_box_t kk_std_core_hnd_under3_fun10863(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under3_fun10863__t* _self = kk_function_as(struct kk_std_core_hnd_under3_fun10863__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<10141> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  kk_evv_t w00;
  kk_std_core_hnd__ev _x10864 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<10141>*/
  w00 = kk_std_core_hnd_evv_swap_with(_x10864, _ctx); /*std/core/hnd/evv<_8698>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, res, _ctx)); /*10139*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    kk_evv_drop(w00, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_under3_fun10865(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_evv_set(w00,kk_context());
    return y;
  }
}

kk_box_t kk_std_core_hnd_under3(kk_std_core_hnd__ev ev, kk_function_t op, kk_box_t x1, kk_box_t x2, kk_box_t x3, kk_context_t* _ctx) { /* forall<a,b,c,d,e,a1> (ev : ev<a1>, op : (a, b, c) -> e d, x1 : a, x2 : b, x3 : c) -> e d */ 
  kk_evv_t w0;
  kk_std_core_hnd__ev _x10862 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<10141>*/
  w0 = kk_std_core_hnd_evv_swap_with(_x10862, _ctx); /*std/core/hnd/evv<_10081>*/
  kk_box_t z = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), op, (op, x1, x2, x3, _ctx)); /*10139*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w0,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(z, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_under3_fun10863(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    return z;
  }
}


// lift anonymous function
struct kk_std_core_hnd_under4_fun10867__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
};
static kk_box_t kk_std_core_hnd_under4_fun10867(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_under4_fun10867(kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under4_fun10867__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_under4_fun10867__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_under4_fun10867, kk_context());
  _self->ev = ev;
  return &_self->_base;
}



// lift anonymous function
struct kk_std_core_hnd_under4_fun10869__t {
  struct kk_function_s _base;
  kk_std_core_hnd__ev ev;
};
static kk_box_t kk_std_core_hnd_under4_fun10869(kk_function_t _fself, kk_function_t cont0, kk_box_t res0, kk_context_t* _ctx);
static kk_function_t kk_std_core_hnd_new_under4_fun10869(kk_std_core_hnd__ev ev, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under4_fun10869__t* _self = kk_function_alloc_as(struct kk_std_core_hnd_under4_fun10869__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_std_core_hnd_under4_fun10869, kk_context());
  _self->ev = ev;
  return &_self->_base;
}

static kk_box_t kk_std_core_hnd_under4_fun10869(kk_function_t _fself, kk_function_t cont0, kk_box_t res0, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under4_fun10869__t* _self = kk_function_as(struct kk_std_core_hnd_under4_fun10869__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<10214> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  return kk_std_core_hnd_under1x(ev, cont0, res0, _ctx);
}
static kk_box_t kk_std_core_hnd_under4_fun10867(kk_function_t _fself, kk_function_t cont, kk_box_t res, kk_context_t* _ctx) {
  struct kk_std_core_hnd_under4_fun10867__t* _self = kk_function_as(struct kk_std_core_hnd_under4_fun10867__t*, _fself);
  kk_std_core_hnd__ev ev = _self->ev; /* std/core/hnd/ev<10214> */
  kk_drop_match(_self, {kk_std_core_hnd__ev_dup(ev);}, {}, _ctx)
  kk_evv_t w00;
  kk_std_core_hnd__ev _x10868 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<10214>*/
  w00 = kk_std_core_hnd_evv_swap_with(_x10868, _ctx); /*std/core/hnd/evv<_8698>*/
  kk_box_t y = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), cont, (cont, res, _ctx)); /*10212*/;
  if (kk_yielding(kk_context())) {
    kk_box_drop(y, _ctx);
    kk_evv_drop(w00, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_under4_fun10869(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    kk_unit_t __0 = kk_Unit;
    kk_evv_set(w00,kk_context());
    return y;
  }
}

kk_box_t kk_std_core_hnd_under4(kk_std_core_hnd__ev ev, kk_function_t op, kk_box_t x1, kk_box_t x2, kk_box_t x3, kk_box_t x4, kk_context_t* _ctx) { /* forall<a,b,c,d,a1,e,b1> (ev : ev<b1>, op : (a, b, c, d) -> e a1, x1 : a, x2 : b, x3 : c, x4 : d) -> e a1 */ 
  kk_evv_t w0;
  kk_std_core_hnd__ev _x10866 = kk_std_core_hnd__ev_dup(ev); /*std/core/hnd/ev<10214>*/
  w0 = kk_std_core_hnd_evv_swap_with(_x10866, _ctx); /*std/core/hnd/evv<_10150>*/
  kk_box_t z = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_box_t, kk_box_t, kk_box_t, kk_context_t*), op, (op, x1, x2, x3, x4, _ctx)); /*10212*/;
  kk_unit_t __ = kk_Unit;
  kk_evv_set(w0,kk_context());
  if (kk_yielding(kk_context())) {
    kk_box_drop(z, _ctx);
    return kk_std_core_hnd_yield_cont(kk_std_core_hnd_new_under4_fun10867(ev, _ctx), _ctx);
  }
  {
    kk_std_core_hnd__ev_dropn(ev, ((int32_t)KI32(3)), _ctx);
    return z;
  }
}

// initialization
void kk_std_core_hnd__init(kk_context_t* _ctx){
  static bool _kk_initialized = false;
  if (_kk_initialized) return;
  _kk_initialized = true;
  kk_std_core_types__init(_ctx);
  #if defined(KK_CUSTOM_INIT)
    KK_CUSTOM_INIT (_ctx);
  #endif
}

// termination
void kk_std_core_hnd__done(kk_context_t* _ctx){
  static bool _kk_done = false;
  if (_kk_done) return;
  _kk_done = true;
  #if defined(KK_CUSTOM_DONE)
    KK_CUSTOM_DONE (_ctx);
  #endif
  kk_std_core_types__done(_ctx);
}
