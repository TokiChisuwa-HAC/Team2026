#ifndef BOUNDING_SPHERE_H_
#define BOUNDING_SPHERE_H_

#include <gslib.h>

class BoundingSphere {
public:
	//コンストラクタ
	BoundingSphere(float radius = 0.0f, const GSvector3& center = GSvector3{ 0.0f, 0.0f, 0.0f });
	//平行移動
	BoundingSphere translate(const GSvector3& position) const;
	//座標変換
	BoundingSphere transform(const GSmatrix4& matrix) const;
	//重なっているか
	bool intersects(const BoundingSphere& other) const;
	//デバッグ表示
	void draw() const;

public:
	float radius{ 0.0f };
	GSvector3 center{ 0.0f, 0.0f, 0.0f };
};

#endif