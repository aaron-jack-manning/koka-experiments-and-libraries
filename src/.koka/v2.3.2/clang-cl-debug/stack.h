#pragma once
#ifndef kk_stack_H
#define kk_stack_H
// Koka generated module: "stack", koka version: 2.3.2, platform: 64-bit
#include <kklib.h>
#include "std_core_types.h"
#include "std_core.h"

// type declarations

// type stack/stack
struct kk_stack__stack_s {
  kk_block_t _block;
};
typedef kk_datatype_t kk_stack__stack;
struct kk_stack_Top {
  struct kk_stack__stack_s _base;
  kk_box_t head;
  kk_stack__stack tail;
};
static inline kk_stack__stack kk_stack__new_Empty(kk_context_t* _ctx){
  return kk_datatype_from_tag((kk_tag_t)(1));
}
static inline kk_stack__stack kk_stack__base_Top(struct kk_stack_Top* _x){
  return kk_datatype_from_base(&_x->_base);
}
static inline kk_stack__stack kk_stack__new_Top(kk_reuse_t _at, kk_box_t head, kk_stack__stack tail, kk_context_t* _ctx){
  struct kk_stack_Top* _con = kk_block_alloc_at_as(struct kk_stack_Top, _at, 2 /* scan count */, (kk_tag_t)(2), _ctx);
  _con->head = head;
  _con->tail = tail;
  return kk_stack__base_Top(_con);
}
static inline struct kk_stack_Top* kk_stack__as_Top(kk_stack__stack x) {
  return kk_datatype_as_assert(struct kk_stack_Top*, x, (kk_tag_t)(2) /* _tag */);
}
static inline bool kk_stack__is_Empty(kk_stack__stack x) {
  return (kk_datatype_eq(x,kk_stack__new_Empty(NULL)));
}
static inline bool kk_stack__is_Top(kk_stack__stack x) {
  return (!kk_datatype_eq(x,kk_stack__new_Empty(NULL)));
}
static inline kk_stack__stack kk_stack__stack_dup(kk_stack__stack _x) {
  return kk_datatype_dup(_x);
}
static inline void kk_stack__stack_drop(kk_stack__stack _x, kk_context_t* _ctx) {
  kk_datatype_drop(_x, _ctx);
}
static inline bool kk_stack__stack_is_unique(kk_stack__stack _x) {
  return kk_datatype_is_unique(_x);
}
static inline void kk_stack__stack_free(kk_stack__stack _x) {
  kk_datatype_free(_x);
}
static inline void kk_stack__stack_decref(kk_stack__stack _x, kk_context_t* _ctx) {
  kk_datatype_decref(_x, _ctx);
}
static inline kk_reuse_t kk_stack__stack_dropn_reuse(kk_stack__stack _x, kk_ssize_t _scan_fsize, kk_context_t* _ctx) {
  return kk_datatype_dropn_reuse(_x, _scan_fsize, _ctx);
}
static inline void kk_stack__stack_dropn(kk_stack__stack _x, kk_ssize_t _scan_fsize, kk_context_t* _ctx) {
  kk_datatype_dropn(_x, _scan_fsize, _ctx);
}
static inline kk_reuse_t kk_stack__stack_reuse(kk_stack__stack _x) {
  return kk_datatype_reuse(_x);
}
static inline kk_stack__stack kk_stack__stack_hole() {
  return kk_datatype_from_tag((kk_tag_t)0);
}
static inline kk_box_t kk_stack__stack_box(kk_stack__stack _x, kk_context_t* _ctx) {
  return kk_datatype_box(_x);
}
static inline kk_stack__stack kk_stack__stack_unbox(kk_box_t _x, kk_context_t* _ctx) {
  return kk_datatype_unbox(_x);
}

// value declarations
 
// Automatically generated. Tests for the `Empty` constructor of the `:stack` type.

static inline bool kk_stack_is_empty(kk_stack__stack stack, kk_context_t* _ctx) { /* forall<a> (stack : stack<a>) -> bool */ 
  if (kk_stack__is_Empty(stack)) {
    return true;
  }
  {
    return false;
  }
}
 
// Automatically generated. Tests for the `Top` constructor of the `:stack` type.

static inline bool kk_stack_is_top(kk_stack__stack stack, kk_context_t* _ctx) { /* forall<a> (stack : stack<a>) -> bool */ 
  if (kk_stack__is_Top(stack)) {
    struct kk_stack_Top* _con129 = kk_stack__as_Top(stack);
    return true;
  }
  {
    return false;
  }
}

kk_std_core_types__maybe kk_stack_peek(kk_stack__stack st, kk_context_t* _ctx); /* forall<a> (st : stack<a>) -> maybe<a> */ 

kk_std_core_types__tuple2_ kk_stack_pop(kk_stack__stack st, kk_context_t* _ctx); /* forall<a> (st : stack<a>) -> (maybe<a>, stack<a>) */ 

static inline kk_stack__stack kk_stack_push(kk_stack__stack st, kk_box_t a, kk_context_t* _ctx) { /* forall<a> (st : stack<a>, a : a) -> stack<a> */ 
  return kk_stack__new_Top(kk_reuse_null, a, st, _ctx);
}

void kk_stack__init(kk_context_t* _ctx);


void kk_stack__done(kk_context_t* _ctx);

#endif // header
