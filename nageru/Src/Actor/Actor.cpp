#include "Actor.h"
#include <GSeffect.h>

void Actor::update(float) {}

void Actor::late_update(float) {}

void Actor::draw() const {}

void Actor::draw_transparent() const {}

void Actor::draw_gui() const {}

void Actor::react(Actor&) {}

void Actor::handle_message(const std::string& message, void* param) {}

void Actor::collide(Actor& other) {
	if (enable_collider_ && other.enable_collider_) {
		if (is_collide(other)) {
			react(other);
			other.react(*this);
		}
	}
}

void Actor::die() {
	dead_ = true;
}

bool Actor::is_collide(const Actor& other) const {
	return collider().intersects(other.collider());
}

bool Actor::is_dead() const {
	return dead_;
}

const std::string& Actor::name() const {
	return name_;
}

const std::string& Actor::tag() const {
	return tag_;
}

const GStransform& Actor::transform() const {
	return transform_;
}

GStransform& Actor::transform() {
	return transform_;
}

GSvector3 Actor::velocity() const {
	return velocity_;
}

BoundingSphere Actor::collider() const {
	return collider_.transform(transform_.localToWorldMatrix());
}

void Actor::play_effect(GSuint id, const GSvector3& local_position, const GSvector3&
	local_rotation, const GSvector3& local_scale) {
	GSmatrix4 local_matrix = GSmatrix4::TRS(local_position, GSquaternion::euler(local_rotation),
		local_scale);
	// ワールド空間に変換する
	GSmatrix4 world_matrix = local_matrix * transform_.localToWorldMatrix();
	// エフェクトを再生する
	gsPlayEffectEx(id, &world_matrix);
}

TweenUnit& Actor::move_to(const GSvector3& to, float duration) {
	// 現在の場所から指定された場所まで、Tweenで移動する
	return Tween::vector3(transform_.position(), to, duration,
		[=](GSvector3 pos) { transform_.position(pos); });
}

TweenUnit& Actor::scale_to(const GSvector3& to, float duration) {
	// 現在のサイズから指定されたサイズまで、Tweenでリサイズする
	return Tween::vector3(transform_.localScale(), to, duration,
		[=](GSvector3 scale) { transform_.localScale(scale); });
}