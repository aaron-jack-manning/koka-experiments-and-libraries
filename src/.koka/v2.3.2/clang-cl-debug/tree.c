// Koka generated module: "tree", koka version: 2.3.2, platform: 64-bit
#include "tree.h"


// lift anonymous function
struct kk_tree_treemap_fun419__t {
  struct kk_function_s _base;
  kk_function_t f;
};
static kk_box_t kk_tree_treemap_fun419(kk_function_t _fself, kk_box_t _b_406, kk_context_t* _ctx);
static kk_function_t kk_tree_new_treemap_fun419(kk_function_t f, kk_context_t* _ctx) {
  struct kk_tree_treemap_fun419__t* _self = kk_function_alloc_as(struct kk_tree_treemap_fun419__t, 2, _ctx);
  _self->_base.fun = kk_cfun_ptr_box(&kk_tree_treemap_fun419, kk_context());
  _self->f = f;
  return &_self->_base;
}

static kk_box_t kk_tree_treemap_fun419(kk_function_t _fself, kk_box_t _b_406, kk_context_t* _ctx) {
  struct kk_tree_treemap_fun419__t* _self = kk_function_as(struct kk_tree_treemap_fun419__t*, _fself);
  kk_function_t f = _self->f; /* (391) -> 392 */
  kk_drop_match(_self, {kk_function_dup(f);}, {}, _ctx)
  kk_tree__tree _x420;
  kk_tree__tree _x421 = kk_tree__tree_unbox(_b_406, _ctx); /*tree/tree<391>*/
  _x420 = kk_tree_treemap(f, _x421, _ctx); /*tree/tree<19>*/
  return kk_tree__tree_box(_x420, _ctx);
}

kk_tree__tree kk_tree_treemap(kk_function_t f, kk_tree__tree tr, kk_context_t* _ctx) { /* forall<a,b> (f : (a) -> b, tr : tree<a>) -> div tree<b> */ 
  if (kk_tree__is_Leaf(tr)) {
    kk_function_drop(f, _ctx);
    return kk_tree__new_Leaf(_ctx);
  }
  {
    struct kk_tree_Branch* _con415 = kk_tree__as_Branch(tr);
    kk_box_t value = _con415->value;
    kk_std_core__list branches = _con415->branches;
    kk_reuse_t _ru_411 = kk_reuse_null; /*reuse*/;
    if (kk_likely(kk_tree__tree_is_unique(tr))) {
      _ru_411 = (kk_tree__tree_reuse(tr));
    }
    else {
      kk_std_core__list_dup(branches);
      kk_box_dup(value);
      kk_tree__tree_decref(tr, _ctx);
    }
    kk_box_t _x416;
    kk_function_t _x417 = kk_function_dup(f); /*(391) -> 392*/
    _x416 = kk_function_call(kk_box_t, (kk_function_t, kk_box_t, kk_context_t*), _x417, (_x417, value, _ctx)); /*392*/
    kk_std_core__list _x418 = kk_std_core_map_5(branches, kk_tree_new_treemap_fun419(f, _ctx), _ctx); /*list<1002>*/
    return kk_tree__new_Branch(_ru_411, _x416, _x418, _ctx);
  }
}

// initialization
void kk_tree__init(kk_context_t* _ctx){
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
void kk_tree__done(kk_context_t* _ctx){
  static bool _kk_done = false;
  if (_kk_done) return;
  _kk_done = true;
  #if defined(KK_CUSTOM_DONE)
    KK_CUSTOM_DONE (_ctx);
  #endif
  kk_std_core__done(_ctx);
  kk_std_core_types__done(_ctx);
}
