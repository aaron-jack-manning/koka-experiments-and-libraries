#pragma once
#ifndef kk_tree_H
#define kk_tree_H
// Koka generated module: "tree", koka version: 2.3.2, platform: 64-bit
#include <kklib.h>
#include "std_core_types.h"
#include "std_core.h"

// type declarations

// type tree/tree
struct kk_tree__tree_s {
  kk_block_t _block;
};
typedef kk_datatype_t kk_tree__tree;
struct kk_tree_Branch {
  struct kk_tree__tree_s _base;
  kk_box_t value;
  kk_std_core__list branches;
};
static inline kk_tree__tree kk_tree__new_Leaf(kk_context_t* _ctx){
  return kk_datatype_from_tag((kk_tag_t)(1));
}
static inline kk_tree__tree kk_tree__base_Branch(struct kk_tree_Branch* _x){
  return kk_datatype_from_base(&_x->_base);
}
static inline kk_tree__tree kk_tree__new_Branch(kk_reuse_t _at, kk_box_t value, kk_std_core__list branches, kk_context_t* _ctx){
  struct kk_tree_Branch* _con = kk_block_alloc_at_as(struct kk_tree_Branch, _at, 2 /* scan count */, (kk_tag_t)(2), _ctx);
  _con->value = value;
  _con->branches = branches;
  return kk_tree__base_Branch(_con);
}
static inline struct kk_tree_Branch* kk_tree__as_Branch(kk_tree__tree x) {
  return kk_datatype_as_assert(struct kk_tree_Branch*, x, (kk_tag_t)(2) /* _tag */);
}
static inline bool kk_tree__is_Leaf(kk_tree__tree x) {
  return (kk_datatype_eq(x,kk_tree__new_Leaf(NULL)));
}
static inline bool kk_tree__is_Branch(kk_tree__tree x) {
  return (!kk_datatype_eq(x,kk_tree__new_Leaf(NULL)));
}
static inline kk_tree__tree kk_tree__tree_dup(kk_tree__tree _x) {
  return kk_datatype_dup(_x);
}
static inline void kk_tree__tree_drop(kk_tree__tree _x, kk_context_t* _ctx) {
  kk_datatype_drop(_x, _ctx);
}
static inline bool kk_tree__tree_is_unique(kk_tree__tree _x) {
  return kk_datatype_is_unique(_x);
}
static inline void kk_tree__tree_free(kk_tree__tree _x) {
  kk_datatype_free(_x);
}
static inline void kk_tree__tree_decref(kk_tree__tree _x, kk_context_t* _ctx) {
  kk_datatype_decref(_x, _ctx);
}
static inline kk_reuse_t kk_tree__tree_dropn_reuse(kk_tree__tree _x, kk_ssize_t _scan_fsize, kk_context_t* _ctx) {
  return kk_datatype_dropn_reuse(_x, _scan_fsize, _ctx);
}
static inline void kk_tree__tree_dropn(kk_tree__tree _x, kk_ssize_t _scan_fsize, kk_context_t* _ctx) {
  kk_datatype_dropn(_x, _scan_fsize, _ctx);
}
static inline kk_reuse_t kk_tree__tree_reuse(kk_tree__tree _x) {
  return kk_datatype_reuse(_x);
}
static inline kk_tree__tree kk_tree__tree_hole() {
  return kk_datatype_from_tag((kk_tag_t)0);
}
static inline kk_box_t kk_tree__tree_box(kk_tree__tree _x, kk_context_t* _ctx) {
  return kk_datatype_box(_x);
}
static inline kk_tree__tree kk_tree__tree_unbox(kk_box_t _x, kk_context_t* _ctx) {
  return kk_datatype_unbox(_x);
}

// value declarations
 
// Automatically generated. Tests for the `Leaf` constructor of the `:tree` type.

static inline bool kk_tree_is_leaf(kk_tree__tree tree, kk_context_t* _ctx) { /* forall<a> (tree : tree<a>) -> bool */ 
  if (kk_tree__is_Leaf(tree)) {
    return true;
  }
  {
    return false;
  }
}
 
// Automatically generated. Tests for the `Branch` constructor of the `:tree` type.

static inline bool kk_tree_is_branch(kk_tree__tree tree, kk_context_t* _ctx) { /* forall<a> (tree : tree<a>) -> bool */ 
  if (kk_tree__is_Branch(tree)) {
    struct kk_tree_Branch* _con412 = kk_tree__as_Branch(tree);
    return true;
  }
  {
    return false;
  }
}

static inline kk_tree__tree kk_tree_combine(kk_tree__tree tr1, kk_tree__tree tr2, kk_box_t a, kk_context_t* _ctx) { /* forall<a> (tr1 : tree<a>, tr2 : tree<a>, a : a) -> tree<a> */ 
  kk_std_core__list _x413;
  kk_std_core__list _x414 = kk_std_core__new_Cons(kk_reuse_null, kk_tree__tree_box(tr2, _ctx), kk_std_core__new_Nil(_ctx), _ctx); /*list<1009>*/
  _x413 = kk_std_core__new_Cons(kk_reuse_null, kk_tree__tree_box(tr1, _ctx), _x414, _ctx); /*list<1009>*/
  return kk_tree__new_Branch(kk_reuse_null, a, _x413, _ctx);
}

kk_tree__tree kk_tree_treemap(kk_function_t f, kk_tree__tree tr, kk_context_t* _ctx); /* forall<a,b> (f : (a) -> b, tr : tree<a>) -> div tree<b> */ 

void kk_tree__init(kk_context_t* _ctx);


void kk_tree__done(kk_context_t* _ctx);

#endif // header
