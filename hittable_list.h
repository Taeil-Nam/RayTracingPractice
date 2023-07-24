#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;	// shard_ptr 사용
using std::make_shared;	// make_shared로 shared_ptr 객체 생성

class hittable_list : public hittable {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); } // 객체 추가

        void clear() { objects.clear(); } // 객체 리스트 삭제
        void add(shared_ptr<hittable> object) { objects.push_back(object); } // 객체 추가

		// 추상 클래스 hittable로 부터 받은 가상 함수
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        std::vector<shared_ptr<hittable>> objects;
		// shared_ptr로 관리되는 객체들을 vector로 가지고 있음
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec; // hit_record 객체 생성
    bool hit_anything = false; // hit한 객체가 있는지
    auto closest_so_far = t_max; // ??

	// 각 객체의 hit 유무 확인
    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif