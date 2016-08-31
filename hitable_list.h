#include "hitable.h"
#pragma once


//Chp 5

/* 
* a list of hitable objects
*/

class hitable_list: public hitable {

  public:
    hitable_list() {}
    hitable_list(hitable **l, int n) {list = l; list_size = n;} //** declares a point to a pointer (array)
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    hitable **list;
    int list_size;

};


//Iterates through list of objects and check if provided ray has hit any of them
bool hitable_list::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {

  hit_record temp_rec;
  bool hit_anything = false;
  double closest_so_far = tmax;
  for (int i = 0; i < list_size; i++) {
    if(list[i]->hit(r, tmin, closest_so_far, temp_rec)){
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }
  
  return hit_anything;


}