#include "ValueUnit.h"

ValueUnit::ValueUnit(float from, float to, float duration, std::function<void(float)> update_func) :
    TweenUnit{ duration },
    from_{ from },
    to_{ to },
    update_func_{ update_func } {
}

void ValueUnit::on_update(float progress) {
    update_func_(LERP(progress, from_, to_));
}