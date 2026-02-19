#include "Vector3Unit.h"

Vector3Unit::Vector3Unit(const GSvector3& from, const GSvector3& to, float duration,
    std::function<void(const GSvector3&)> update_func) :
    TweenUnit{ duration },
    from_{ from },
    to_{ to },
    update_func_{ update_func } {
}

void Vector3Unit::on_update(float progress) {
    update_func_(LERP(progress, from_, to_));
}