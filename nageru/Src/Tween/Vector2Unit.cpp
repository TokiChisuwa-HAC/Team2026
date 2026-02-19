#include "Vector2Unit.h"

Vector2Unit::Vector2Unit(const GSvector2& from, const GSvector2& to, float duration,
    std::function<void(const GSvector2&)> update_func) :
    TweenUnit{ duration },
    from_{ from },
    to_{ to },
    update_func_{ update_func } {
}

void Vector2Unit::on_update(float progress) {
    update_func_(LERP(progress, from_, to_));
}