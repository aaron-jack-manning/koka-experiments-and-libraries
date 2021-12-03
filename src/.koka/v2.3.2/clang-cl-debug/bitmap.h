#pragma once
#ifndef kk_bitmap_H
#define kk_bitmap_H
// Koka generated module: "bitmap", koka version: 2.3.2, platform: 64-bit
#include <kklib.h>
#include "std_core_types.h"
#include "std_core_hnd.h"
#include "std_core.h"

// type declarations

// value type bitmap/colour
struct kk_bitmap_Colour {
  kk_integer_t red;
  kk_integer_t green;
  kk_integer_t blue;
};
typedef struct kk_bitmap_Colour kk_bitmap__colour;
static inline kk_bitmap__colour kk_bitmap__new_Colour(kk_integer_t red, kk_integer_t green, kk_integer_t blue, kk_context_t* _ctx){
  kk_bitmap__colour _con;
  _con.red = red;
  _con.green = green;
  _con.blue = blue;
  return _con;
}
static inline bool kk_bitmap__is_Colour(kk_bitmap__colour x) {
  return (true);
}
static inline kk_bitmap__colour kk_bitmap__colour_dup(kk_bitmap__colour _x) {
  kk_integer_dup(_x.red);
  kk_integer_dup(_x.green);
  kk_integer_dup(_x.blue);
  return _x;
}
static inline void kk_bitmap__colour_drop(kk_bitmap__colour _x, kk_context_t* _ctx) {
  kk_integer_drop(_x.red, _ctx);
  kk_integer_drop(_x.green, _ctx);
  kk_integer_drop(_x.blue, _ctx);
}
static inline kk_box_t kk_bitmap__colour_box(kk_bitmap__colour _x, kk_context_t* _ctx) {
  kk_box_t _box;
  kk_valuetype_box(kk_bitmap__colour, _box, _x, 3 /* scan count */, _ctx);
  return _box;
}
static inline kk_bitmap__colour kk_bitmap__colour_unbox(kk_box_t _x, kk_context_t* _ctx) {
  kk_boxed_value_t _p;
  kk_bitmap__colour _unbox;
  kk_valuetype_unbox_(kk_bitmap__colour, _p, _unbox, _x, _ctx);
  if (_ctx!=NULL && _p!=NULL) {
    if (kk_basetype_is_unique(_p)) { kk_basetype_free(_p); } else {
      kk_bitmap__colour_dup(_unbox);
      kk_basetype_decref(_p, _ctx);
    }
  }
  return _unbox;
}

// value declarations
 
// Automatically generated. Retrieves the `red` constructor field of the `:colour` type.

static inline kk_integer_t kk_bitmap_red(kk_bitmap__colour colour, kk_context_t* _ctx) { /* (colour : colour) -> int */ 
  {
    kk_integer_t _x = colour.red;
    return kk_integer_dup(_x);
  }
}
 
// Automatically generated. Retrieves the `green` constructor field of the `:colour` type.

static inline kk_integer_t kk_bitmap_green(kk_bitmap__colour colour, kk_context_t* _ctx) { /* (colour : colour) -> int */ 
  {
    kk_integer_t _x = colour.green;
    return kk_integer_dup(_x);
  }
}
 
// Automatically generated. Retrieves the `blue` constructor field of the `:colour` type.

static inline kk_integer_t kk_bitmap_blue(kk_bitmap__colour colour, kk_context_t* _ctx) { /* (colour : colour) -> int */ 
  {
    kk_integer_t _x = colour.blue;
    return kk_integer_dup(_x);
  }
}

kk_bitmap__colour kk_bitmap__copy(kk_bitmap__colour _this, kk_std_core_types__optional red0, kk_std_core_types__optional green0, kk_std_core_types__optional blue0, kk_context_t* _ctx); /* (colour, red : optional<int>, green : optional<int>, blue : optional<int>) -> colour */ 

kk_std_core__list kk_bitmap__lift1237_bytes(kk_integer_t current, kk_integer_t base, kk_context_t* _ctx); /* (current : int, base : int) -> div list<int> */ 

kk_std_core__list kk_bitmap_bytes(kk_integer_t number, kk_context_t* _ctx); /* (number : int) -> div list<int> */ 

kk_vector_t kk_bitmap_colour_to_bytes(kk_bitmap__colour colour, kk_context_t* _ctx); /* (colour : colour) -> vector<int> */ 

kk_unit_t kk_bitmap__mlift1265_get_dib_header(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_unit_t wild__4, kk_context_t* _ctx); /* forall<h> (dib-header : local-var<h,vector<int>>, i : int, print-resolution-bytes : list<int>, wild_4 : ()) -> <local<h>,exn,div> () */ 

kk_unit_t kk_bitmap__mlift1266_get_dib_header(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_unit_t wild__3, kk_context_t* _ctx); /* forall<h> (dib-header : local-var<h,vector<int>>, i : int, print-resolution-bytes : list<int>, wild_3 : ()) -> <local<h>,exn,div> () */ 

kk_unit_t kk_bitmap__mlift1267_get_dib_header(kk_ref_t dib_header, kk_integer_t i, kk_std_core__list print_resolution_bytes, kk_std_core__list vertical_resolution_bytes, kk_unit_t wild__2, kk_context_t* _ctx); /* forall<h> (dib-header : local-var<h,vector<int>>, i : int, print-resolution-bytes : list<int>, vertical-resolution-bytes : list<int>, wild_2 : ()) -> <local<h>,exn,div> () */ 
 
// monadic lift

static inline kk_vector_t kk_bitmap__mlift1268_get_dib_header(kk_ref_t dib_header, kk_unit_t wild__5, kk_context_t* _ctx) { /* forall<h> (dib-header : local-var<h,vector<int>>, wild_5 : ()) -> <exn,local<h>,div> vector<int> */ 
  kk_box_t _x1614 = (kk_ref_get(dib_header,kk_context())); /*1000*/
  return kk_vector_unbox(_x1614, _ctx);
}

kk_vector_t kk_bitmap__mlift1269_get_dib_header(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_unit_t wild__1, kk_context_t* _ctx); /* forall<h> (dib-header : local-var<h,vector<int>>, dpcm : int, horizontal-resolution : int, vertical-resolution : int, wild_1 : ()) -> <local<h>,exn,div> vector<int> */ 

kk_vector_t kk_bitmap__mlift1270_get_dib_header(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_unit_t wild__0, kk_context_t* _ctx); /* forall<h> (dib-header : local-var<h,vector<int>>, dpcm : int, horizontal-resolution : int, vertical-resolution : int, wild_0 : ()) -> <local<h>,exn,div> vector<int> */ 

kk_vector_t kk_bitmap__mlift1271_get_dib_header(kk_ref_t dib_header, kk_integer_t dpcm, kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_unit_t wild__, kk_context_t* _ctx); /* forall<h> (dib-header : local-var<h,vector<int>>, dpcm : int, horizontal-resolution : int, vertical-resolution : int, wild_ : ()) -> <local<h>,exn,div> vector<int> */ 

kk_vector_t kk_bitmap_get_dib_header(kk_integer_t horizontal_resolution, kk_integer_t vertical_resolution, kk_integer_t dpcm, kk_context_t* _ctx); /* (horizontal-resolution : int, vertical-resolution : int, dpcm : int) -> pure vector<int> */ 
 
// monadic lift

static inline kk_vector_t kk_bitmap__mlift1272_get_header(kk_ref_t header, kk_unit_t wild__2, kk_context_t* _ctx) { /* forall<h> (header : local-var<h,vector<int>>, wild_2 : ()) -> <exn,local<h>,div> vector<int> */ 
  kk_box_t _x1653 = (kk_ref_get(header,kk_context())); /*1000*/
  return kk_vector_unbox(_x1653, _ctx);
}

kk_vector_t kk_bitmap__mlift1273_get_header(kk_integer_t file_size, kk_ref_t header, kk_unit_t wild__1, kk_context_t* _ctx); /* forall<h> (file-size : int, header : local-var<h,vector<int>>, wild_1 : ()) -> <local<h>,exn,div> vector<int> */ 

kk_vector_t kk_bitmap__mlift1274_get_header(kk_integer_t file_size, kk_ref_t header, kk_unit_t wild__0, kk_context_t* _ctx); /* forall<h> (file-size : int, header : local-var<h,vector<int>>, wild_0 : ()) -> <local<h>,exn,div> vector<int> */ 

kk_vector_t kk_bitmap__mlift1275_get_header(kk_integer_t file_size, kk_ref_t header, kk_unit_t wild__, kk_context_t* _ctx); /* forall<h> (file-size : int, header : local-var<h,vector<int>>, wild_ : ()) -> <local<h>,exn,div> vector<int> */ 

kk_vector_t kk_bitmap_get_header(kk_integer_t file_size, kk_context_t* _ctx); /* (file-size : int) -> pure vector<int> */ 

void kk_bitmap__init(kk_context_t* _ctx);


void kk_bitmap__done(kk_context_t* _ctx);

#endif // header
