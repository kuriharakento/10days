#include "CollisionAlgorithm.h"
#include <cmath>
#include <algorithm>
#include "application/GameObject/base/GameObject.h"

// --- 3D用判定 ---

bool CollisionAlgorithm::CheckAABBvsAABB3D(const AABBColliderComponent* a, const AABBColliderComponent* b)
{
	const AABB& aBox = a->GetAABB();
	const AABB& bBox = b->GetAABB();

	return (aBox.max_.x >= bBox.min_.x && aBox.min_.x <= bBox.max_.x) &&
		(aBox.max_.y >= bBox.min_.y && aBox.min_.y <= bBox.max_.y) &&
		(aBox.max_.z >= bBox.min_.z && aBox.min_.z <= bBox.max_.z);
}

bool CollisionAlgorithm::CheckOBBvsOBB3D(const OBBColliderComponent* a, const OBBColliderComponent* b)
{
	const OBB& obbA = a->GetOBB();
	const OBB& obbB = b->GetOBB();

	// OBBの回転行列（各軸ベクトル）
	Matrix4x4 rotA = obbA.rotate;
	Matrix4x4 rotB = obbB.rotate;

	Vector3 axesA[3] =
	{
		Vector3::Normalize(Vector3(rotA.m[0][0], rotA.m[0][1], rotA.m[0][2])),
		Vector3::Normalize(Vector3(rotA.m[1][0], rotA.m[1][1], rotA.m[1][2])),
		Vector3::Normalize(Vector3(rotA.m[2][0], rotA.m[2][1], rotA.m[2][2]))
	};

	Vector3 axesB[3] =
	{
		Vector3::Normalize(Vector3(rotB.m[0][0], rotB.m[0][1], rotB.m[0][2])),
		Vector3::Normalize(Vector3(rotB.m[1][0], rotB.m[1][1], rotB.m[1][2])),
		Vector3::Normalize(Vector3(rotB.m[2][0], rotB.m[2][1], rotB.m[2][2]))
	};

	// 15の分離軸
	Vector3 testAxes[15];
	int axisCount = 0;

	for (int i = 0; i < 3; ++i) testAxes[axisCount++] = axesA[i];
	for (int i = 0; i < 3; ++i) testAxes[axisCount++] = axesB[i];

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			testAxes[axisCount++] = Vector3::Normalize(Vector3::Cross(axesA[i], axesB[j]));
		}
	}

	Vector3 toCenter = obbB.center - obbA.center;

	for (int i = 0; i < 15; ++i)
	{
		const Vector3& axis = testAxes[i];
		if (axis.x == 0 && axis.y == 0 && axis.z == 0) continue;

		float aProj =
			std::abs(Vector3::Dot(axesA[0] * obbA.size.x, axis)) +
			std::abs(Vector3::Dot(axesA[1] * obbA.size.y, axis)) +
			std::abs(Vector3::Dot(axesA[2] * obbA.size.z, axis));

		float bProj =
			std::abs(Vector3::Dot(axesB[0] * obbB.size.x, axis)) +
			std::abs(Vector3::Dot(axesB[1] * obbB.size.y, axis)) +
			std::abs(Vector3::Dot(axesB[2] * obbB.size.z, axis));

		float distance = std::abs(Vector3::Dot(toCenter, axis));

		if (distance > aProj + bProj)
		{
			return false;
		}
	}
	ICollisionComponent* aNonConst = const_cast<OBBColliderComponent*>(a);
	ICollisionComponent* bNonConst = const_cast<OBBColliderComponent*>(b);
	aNonConst->SetCollisionPosition(obbA.center);
	bNonConst->SetCollisionPosition(obbB.center);

	return true;
}

bool CollisionAlgorithm::CheckAABBvsOBB3D(const AABBColliderComponent* a, const OBBColliderComponent* b)
{
	const AABB& aBox = a->GetAABB();
	const OBB& obb = b->GetOBB();

	Matrix4x4 rot = obb.rotate;

	Vector3 axes[3] =
	{
		Vector3::Normalize(Vector3(rot.m[0][0], rot.m[0][1], rot.m[0][2])),
		Vector3::Normalize(Vector3(rot.m[1][0], rot.m[1][1], rot.m[1][2])),
		Vector3::Normalize(Vector3(rot.m[2][0], rot.m[2][1], rot.m[2][2]))
	};

	Vector3 toCenter = aBox.GetCenter() - obb.center;
	Vector3 aHalfSize = aBox.GetHalfSize();

	Vector3 testAxes[6];
	int axisCount = 0;

	for (int i = 0; i < 3; ++i) testAxes[axisCount++] = axes[i];
	testAxes[axisCount++] = Vector3(1, 0, 0);
	testAxes[axisCount++] = Vector3(0, 1, 0);
	testAxes[axisCount++] = Vector3(0, 0, 1);

	for (int i = 0; i < axisCount; ++i)
	{
		const Vector3& axis = testAxes[i];

		float aProj = std::abs(Vector3::Dot(axis, Vector3(aHalfSize.x, 0.0f, 0.0f))) +
			std::abs(Vector3::Dot(axis, Vector3(0.0f, aHalfSize.y, 0.0f))) +
			std::abs(Vector3::Dot(axis, Vector3(0.0f, 0.0f, aHalfSize.z)));

		float bProj = std::abs(Vector3::Dot(axes[0] * obb.size.x, axis)) +
			std::abs(Vector3::Dot(axes[1] * obb.size.y, axis)) +
			std::abs(Vector3::Dot(axes[2] * obb.size.z, axis));

		float distance = std::abs(Vector3::Dot(toCenter, axis));

		if (distance > aProj + bProj)
		{
			return false;
		}
	}

	ICollisionComponent* aNonConst = const_cast<AABBColliderComponent*>(a);
	ICollisionComponent* bNonConst = const_cast<OBBColliderComponent*>(b);
	aNonConst->SetCollisionPosition(aBox.GetCenter());
	bNonConst->SetCollisionPosition(obb.center);

	return true;
}

// --- 3Dサブステップ判定 ---

bool CollisionAlgorithm::CheckAABBvsAABBSubstep3D(const AABBColliderComponent* a, const AABBColliderComponent* b)
{
	constexpr float MAX_STEP_DISTANCE = 1.0f;

	Vector3 startA = a->GetPreviousPosition();
	Vector3 endA = a->GetOwner()->GetPosition();
	Vector3 startB = b->GetPreviousPosition();
	Vector3 endB = b->GetOwner()->GetPosition();

	const AABB& aBox = a->GetAABB();
	const AABB& bBox = b->GetAABB();

	if (CheckAABBvsAABB3D(a, b)) return true;

	float distanceA = (endA - startA).Length();
	float distanceB = (endB - startB).Length();

	float maxDistance = (std::max)(distanceA, distanceB);
	int subStepCount = (std::max)(1, static_cast<int>(std::ceil(maxDistance / MAX_STEP_DISTANCE)));

	AABBColliderComponent* aNonConst = const_cast<AABBColliderComponent*>(a);
	AABBColliderComponent* bNonConst = const_cast<AABBColliderComponent*>(b);

	AABBColliderComponent tempA(nullptr);
	AABBColliderComponent tempB(nullptr);

	for (int step = 0; step <= subStepCount; ++step)
	{
		float t = static_cast<float>(step) / subStepCount;

		Vector3 subPosA = MathUtils::Lerp(startA, endA, t);
		Vector3 subPosB = MathUtils::Lerp(startB, endB, t);

		AABB movedAABB_A(subPosA - aBox.GetHalfSize(), subPosA + aBox.GetHalfSize());
		AABB movedAABB_B(subPosB - bBox.GetHalfSize(), subPosB + bBox.GetHalfSize());


		tempA.SetAABB(movedAABB_A);
		tempB.SetAABB(movedAABB_B);

		if (CheckAABBvsAABB3D(&tempA, &tempB))
		{
			aNonConst->SetCollisionPosition(subPosA);
			bNonConst->SetCollisionPosition(subPosB);
			return true;
		}
	}

	return false;
}

bool CollisionAlgorithm::CheckOBBvsOBBSubstep3D(const OBBColliderComponent* a, const OBBColliderComponent* b)
{
	constexpr float MAX_STEP_DISTANCE = 1.0f;

	Vector3 startA = a->GetPreviousPosition();
	Vector3 endA = a->GetOwner()->GetPosition();
	Vector3 startB = b->GetPreviousPosition();
	Vector3 endB = b->GetOwner()->GetPosition();

	OBB aObb = a->GetOBB();
	OBB bObb = b->GetOBB();

	float distanceA = (endA - startA).Length();
	float distanceB = (endB - startB).Length();

	float maxDistance = (std::max)(distanceA, distanceB);
	int subStepCount = (std::max)(1, static_cast<int>(std::ceil(maxDistance / MAX_STEP_DISTANCE)));

	OBBColliderComponent* aNonConst = const_cast<OBBColliderComponent*>(a);
	OBBColliderComponent* bNonConst = const_cast<OBBColliderComponent*>(b);

	OBBColliderComponent tempA(nullptr);
	OBBColliderComponent tempB(nullptr);

	for (int step = 0; step < subStepCount; ++step)
	{
		float t = static_cast<float>(step + 1) / subStepCount;

		Vector3 subPosA = startA + (endA - startA) * t;
		Vector3 subPosB = startB + (endB - startB) * t;

		OBB movedOBB_A = aObb;
		OBB movedOBB_B = bObb;
		movedOBB_A.center = subPosA;
		movedOBB_B.center = subPosB;


		tempA.SetOBB(movedOBB_A);
		tempB.SetOBB(movedOBB_B);

		if (CheckOBBvsOBB3D(&tempA, &tempB))
		{
			aNonConst->SetCollisionPosition(subPosA);
			bNonConst->SetCollisionPosition(subPosB);
			return true;
		}
	}

	return false;
}

bool CollisionAlgorithm::CheckAABBvsOBBSubstep3D(const AABBColliderComponent* a, const OBBColliderComponent* b)
{
	constexpr float MAX_STEP_DISTANCE = 1.0f;

	Vector3 startA = a->GetPreviousPosition();
	Vector3 endA = a->GetOwner()->GetPosition();
	Vector3 startB = b->GetPreviousPosition();
	Vector3 endB = b->GetOwner()->GetPosition();

	const AABB& aBox = a->GetAABB();
	OBB bObb = b->GetOBB();

	float distanceA = (endA - startA).Length();
	float distanceB = (endB - startB).Length();

	float maxDistance = (std::max)(distanceA, distanceB);
	int subStepCount = (std::max)(1, static_cast<int>(std::ceil(maxDistance / MAX_STEP_DISTANCE)));

	AABBColliderComponent* aNonConst = const_cast<AABBColliderComponent*>(a);
	OBBColliderComponent* bNonConst = const_cast<OBBColliderComponent*>(b);

	AABBColliderComponent tempA(nullptr);
	OBBColliderComponent tempB(nullptr);

	for (int step = 0; step < subStepCount; ++step)
	{
		float t = static_cast<float>(step + 1) / subStepCount;

		Vector3 subPosA = startA + (endA - startA) * t;
		Vector3 subPosB = startB + (endB - startB) * t;

		OBB movedOBB = bObb;
		movedOBB.center = subPosB;

		Vector3 aHalf = aBox.GetHalfSize();
		AABB movedAABB(subPosA - aHalf, subPosA + aHalf);


		tempA.SetAABB(movedAABB);
		tempB.SetOBB(movedOBB);

		if (CheckAABBvsOBB3D(&tempA, &tempB))
		{
			aNonConst->SetCollisionPosition(subPosA);
			bNonConst->SetCollisionPosition(subPosB);
			return true;
		}
	}

	return false;
}

// --- 2D用判定（平面指定） ---

// 平面の軸情報を取得
static void GetPlaneAxes(CollisionPlane plane, int& axis1, int& axis2)
{
	switch (plane)
	{
	case CollisionPlane::XY: axis1 = 0; axis2 = 1; break;
	case CollisionPlane::XZ: axis1 = 0; axis2 = 2; break;
	case CollisionPlane::YZ: axis1 = 1; axis2 = 2; break;
	}
}

// 軸インデックスから値取得
float GetSizeComponent(const Vector3& v, int axis)
{
	switch (axis)
	{
	case 0: return v.x;
	case 1: return v.y;
	case 2: return v.z;
	default: return 0.0f;
	}
}

// --- AABB vs AABB 2D判定 ---
bool CollisionAlgorithm::CheckAABBvsAABB2D(const AABBColliderComponent* a, const AABBColliderComponent* b, CollisionPlane plane)
{
	int axis1, axis2;
	GetPlaneAxes(plane, axis1, axis2);

	const AABB& aBox = a->GetAABB();
	const AABB& bBox = b->GetAABB();

	// 各軸のmin/max
	float aMin1 = GetSizeComponent(aBox.min_, axis1);
	float aMax1 = GetSizeComponent(aBox.max_, axis1);
	float aMin2 = GetSizeComponent(aBox.min_, axis2);
	float aMax2 = GetSizeComponent(aBox.max_, axis2);

	float bMin1 = GetSizeComponent(bBox.min_, axis1);
	float bMax1 = GetSizeComponent(bBox.max_, axis1);
	float bMin2 = GetSizeComponent(bBox.min_, axis2);
	float bMax2 = GetSizeComponent(bBox.max_, axis2);

	bool overlap =
		(aMax1 >= bMin1 && aMin1 <= bMax1) &&
		(aMax2 >= bMin2 && aMin2 <= bMax2);

	if (overlap)
	{
		ICollisionComponent* aNonConst = const_cast<AABBColliderComponent*>(a);
		ICollisionComponent* bNonConst = const_cast<AABBColliderComponent*>(b);
		aNonConst->SetCollisionPosition(aBox.GetCenter());
		bNonConst->SetCollisionPosition(bBox.GetCenter());
	}
	return overlap;
}

// --- OBB vs OBB 2D判定 ---
bool CollisionAlgorithm::CheckOBBvsOBB2D(const OBBColliderComponent* a, const OBBColliderComponent* b, CollisionPlane plane)
{
	const OBB& obbA = a->GetOBB();
	const OBB& obbB = b->GetOBB();

	bool isColliding = false;

	switch (plane)
	{
	case CollisionPlane::XY:
		isColliding = CheckOBBvsOBB_XY(obbA, obbB);
		break;
	case CollisionPlane::XZ:
		isColliding = CheckOBBvsOBB_XZ(obbA, obbB);
		break;
	case CollisionPlane::YZ:
		isColliding = CheckOBBvsOBB_YZ(obbA, obbB);
		break;
	}

	if (isColliding)
	{
		// 衝突位置の設定
		ICollisionComponent* aNonConst = const_cast<OBBColliderComponent*>(a);
		ICollisionComponent* bNonConst = const_cast<OBBColliderComponent*>(b);
		aNonConst->SetCollisionPosition(obbA.center);
		bNonConst->SetCollisionPosition(obbB.center);
	}

	return isColliding;
}

// XY平面専用の高速判定
bool CollisionAlgorithm::CheckOBBvsOBB_XY(const OBB& obbA, const OBB& obbB)
{
	// XY平面の中心座標（直接アクセス）
	Vector2 centerA(obbA.center.x, obbA.center.y);
	Vector2 centerB(obbB.center.x, obbB.center.y);
	Vector2 toCenter = centerB - centerA;

	// XY平面の軸ベクトル（回転行列から直接取得、正規化済み）
	Vector2 axesA[2] = {
		Vector2(obbA.rotate.m[0][0], obbA.rotate.m[0][1]),  // X軸
		Vector2(obbA.rotate.m[1][0], obbA.rotate.m[1][1])   // Y軸
	};

	Vector2 axesB[2] = {
		Vector2(obbB.rotate.m[0][0], obbB.rotate.m[0][1]),  // X軸
		Vector2(obbB.rotate.m[1][0], obbB.rotate.m[1][1])   // Y軸
	};

	// XY平面のサイズ
	Vector2 sizeA(obbA.size.x, obbA.size.y);
	Vector2 sizeB(obbB.size.x, obbB.size.y);

	// 4つの分離軸でテスト
	Vector2 testAxes[4] = { axesA[0], axesA[1], axesB[0], axesB[1] };

	for (int i = 0; i < 4; ++i)
	{
		const Vector2& axis = testAxes[i];

		// 投影幅計算
		float projA = std::abs(Vector2::Dot(axesA[0] * sizeA.x, axis)) +
			std::abs(Vector2::Dot(axesA[1] * sizeA.y, axis));

		float projB = std::abs(Vector2::Dot(axesB[0] * sizeB.x, axis)) +
			std::abs(Vector2::Dot(axesB[1] * sizeB.y, axis));

		float distance = std::abs(Vector2::Dot(toCenter, axis));

		if (distance > projA + projB)
		{
			return false; // 分離軸発見
		}
	}
	return true; // 衝突
}

// XZ平面専用の高速判定
bool CollisionAlgorithm::CheckOBBvsOBB_XZ(const OBB& obbA, const OBB& obbB)
{
	Vector2 centerA(obbA.center.x, obbA.center.z);
	Vector2 centerB(obbB.center.x, obbB.center.z);
	Vector2 toCenter = centerB - centerA;

	Vector2 axesA[2] = {
		Vector2(obbA.rotate.m[0][0], obbA.rotate.m[0][2]),  // X軸のXZ成分
		Vector2(obbA.rotate.m[2][0], obbA.rotate.m[2][2])   // Z軸のXZ成分
	};

	Vector2 axesB[2] = {
		Vector2(obbB.rotate.m[0][0], obbB.rotate.m[0][2]),
		Vector2(obbB.rotate.m[2][0], obbB.rotate.m[2][2])
	};

	Vector2 sizeA(obbA.size.x, obbA.size.z);  // XZなのでZ成分
	Vector2 sizeB(obbB.size.x, obbB.size.z);  // XZなのでZ成分

	Vector2 testAxes[4] = { axesA[0], axesA[1], axesB[0], axesB[1] };

	for (int i = 0; i < 4; ++i)
	{
		const Vector2& axis = testAxes[i];

		// 修正: sizeA.x と sizeA.y ではなく、sizeA.x と sizeA.z
		float projA = std::abs(Vector2::Dot(axesA[0] * sizeA.x, axis)) +
			std::abs(Vector2::Dot(axesA[1] * sizeA.y, axis));  // ← ここをsizeA.yに修正

		float projB = std::abs(Vector2::Dot(axesB[0] * sizeB.x, axis)) +
			std::abs(Vector2::Dot(axesB[1] * sizeB.y, axis));  // ← ここをsizeB.yに修正

		float distance = std::abs(Vector2::Dot(toCenter, axis));

		if (distance > projA + projB)
		{
			return false;
		}
	}
	return true;
}

// YZ平面専用の高速判定
bool CollisionAlgorithm::CheckOBBvsOBB_YZ(const OBB& obbA, const OBB& obbB)
{
	Vector2 centerA(obbA.center.y, obbA.center.z);
	Vector2 centerB(obbB.center.y, obbB.center.z);
	Vector2 toCenter = centerB - centerA;

	Vector2 axesA[2] = {
		Vector2(obbA.rotate.m[1][1], obbA.rotate.m[1][2]),  // Y軸のYZ成分
		Vector2(obbA.rotate.m[2][1], obbA.rotate.m[2][2])   // Z軸のYZ成分
	};

	Vector2 axesB[2] = {
		Vector2(obbB.rotate.m[1][1], obbB.rotate.m[1][2]),
		Vector2(obbB.rotate.m[2][1], obbB.rotate.m[2][2])
	};

	Vector2 sizeA(obbA.size.y, obbA.size.z);  // YZなのでY,Z成分
	Vector2 sizeB(obbB.size.y, obbB.size.z);  // YZなのでY,Z成分

	Vector2 testAxes[4] = { axesA[0], axesA[1], axesB[0], axesB[1] };

	for (int i = 0; i < 4; ++i)
	{
		const Vector2& axis = testAxes[i];

		// 修正: YZ平面なので sizeA.x と sizeA.y を使用
		float projA = std::abs(Vector2::Dot(axesA[0] * sizeA.x, axis)) +  // sizeA.x = obbA.size.y
			std::abs(Vector2::Dot(axesA[1] * sizeA.y, axis));   // sizeA.y = obbA.size.z

		float projB = std::abs(Vector2::Dot(axesB[0] * sizeB.x, axis)) +
			std::abs(Vector2::Dot(axesB[1] * sizeB.y, axis));

		float distance = std::abs(Vector2::Dot(toCenter, axis));

		if (distance > projA + projB)
		{
			return false;
		}
	}
	return true;
}

// --- AABB vs OBB 2D判定 ---
bool CollisionAlgorithm::CheckAABBvsOBB2D(const AABBColliderComponent* a, const OBBColliderComponent* b, CollisionPlane plane)
{
	int axis1, axis2;
	GetPlaneAxes(plane, axis1, axis2);

	const AABB& aBox = a->GetAABB();
	const OBB& obb = b->GetOBB();

	Vector2 aCenter, obbCenter;
	switch (plane)
	{
	case CollisionPlane::XY:
		aCenter = Vector2(aBox.GetCenter().x, aBox.GetCenter().y);
		obbCenter = Vector2(obb.center.x, obb.center.y);
		break;
	case CollisionPlane::XZ:
		aCenter = Vector2(aBox.GetCenter().x, aBox.GetCenter().z);
		obbCenter = Vector2(obb.center.x, obb.center.z);
		break;
	case CollisionPlane::YZ:
		aCenter = Vector2(aBox.GetCenter().y, aBox.GetCenter().z);
		obbCenter = Vector2(obb.center.y, obb.center.z);
		break;
	}

	Matrix4x4 rot = obb.rotate;
	Vector2 axes[2];
	axes[0] = Vector2(GetSizeComponent(Vector3(rot.m[axis1][0], rot.m[axis2][0], 0), 0),
					  GetSizeComponent(Vector3(rot.m[axis1][0], rot.m[axis2][0], 0), 1));
	axes[1] = Vector2(GetSizeComponent(Vector3(rot.m[axis1][1], rot.m[axis2][1], 0), 0),
					  GetSizeComponent(Vector3(rot.m[axis1][1], rot.m[axis2][1], 0), 1));

	Vector2 aabbAxes[2] = { Vector2(1,0), Vector2(0,1) };
	Vector2 testAxes[4] = {
		Vector2::Normalize(axes[0]),
		Vector2::Normalize(axes[1]),
		aabbAxes[0],
		aabbAxes[1]
	};

	Vector2 toCenter = aCenter - obbCenter;
	Vector2 aHalf;
	switch (plane)
	{
	case CollisionPlane::XY:
		aHalf = Vector2(aBox.GetHalfSize().x, aBox.GetHalfSize().y);
		break;
	case CollisionPlane::XZ:
		aHalf = Vector2(aBox.GetHalfSize().x, aBox.GetHalfSize().z);
		break;
	case CollisionPlane::YZ:
		aHalf = Vector2(aBox.GetHalfSize().y, aBox.GetHalfSize().z);
		break;
	}

	for (int i = 0; i < 4; ++i)
	{
		const Vector2& axis = testAxes[i];

		float aProj = std::abs(Vector2::Dot(axis, Vector2(aHalf.x, 0))) +
			std::abs(Vector2::Dot(axis, Vector2(0, aHalf.y)));

		float bProj = std::abs(Vector2::Dot(axes[0] * GetSizeComponent(obb.size, axis1), axis)) +
			std::abs(Vector2::Dot(axes[1] * GetSizeComponent(obb.size, axis2), axis));

		float distance = std::abs(Vector2::Dot(toCenter, axis));

		if (distance > aProj + bProj)
			return false;
	}

	ICollisionComponent* aNonConst = const_cast<AABBColliderComponent*>(a);
	ICollisionComponent* bNonConst = const_cast<OBBColliderComponent*>(b);
	aNonConst->SetCollisionPosition(aBox.GetCenter());
	bNonConst->SetCollisionPosition(obb.center);
	return true;
}

// --- サブステップ 2D判定 ---

bool CollisionAlgorithm::CheckAABBvsAABBSubstep2D(const AABBColliderComponent* a, const AABBColliderComponent* b, CollisionPlane plane)
{
	constexpr float MAX_STEP_DISTANCE = 1.0f;
	Vector3 startA = a->GetPreviousPosition();
	Vector3 endA = a->GetOwner()->GetPosition();
	Vector3 startB = b->GetPreviousPosition();
	Vector3 endB = b->GetOwner()->GetPosition();

	const AABB& aBox = a->GetAABB();
	const AABB& bBox = b->GetAABB();

	if (CheckAABBvsAABB2D(a, b, plane)) return true;

	float distanceA = (endA - startA).Length();
	float distanceB = (endB - startB).Length();

	float maxDistance = (std::max)(distanceA, distanceB);
	int subStepCount = (std::max)(1, static_cast<int>(std::ceil(maxDistance / MAX_STEP_DISTANCE)));

	AABBColliderComponent* aNonConst = const_cast<AABBColliderComponent*>(a);
	AABBColliderComponent* bNonConst = const_cast<AABBColliderComponent*>(b);

	AABBColliderComponent tempA(nullptr);
	AABBColliderComponent tempB(nullptr);

	for (int step = 0; step <= subStepCount; ++step)
	{
		float t = static_cast<float>(step) / subStepCount;
		Vector3 subPosA = MathUtils::Lerp(startA, endA, t);
		Vector3 subPosB = MathUtils::Lerp(startB, endB, t);

		AABB movedAABB_A(subPosA - aBox.GetHalfSize(), subPosA + aBox.GetHalfSize());
		AABB movedAABB_B(subPosB - bBox.GetHalfSize(), subPosB + bBox.GetHalfSize());


		tempA.SetAABB(movedAABB_A);
		tempB.SetAABB(movedAABB_B);

		if (CheckAABBvsAABB2D(&tempA, &tempB, plane))
		{
			aNonConst->SetCollisionPosition(subPosA);
			bNonConst->SetCollisionPosition(subPosB);
			return true;
		}
	}
	return false;
}

bool CollisionAlgorithm::CheckOBBvsOBBSubstep2D(const OBBColliderComponent* a, const OBBColliderComponent* b, CollisionPlane plane)
{
	constexpr float MAX_STEP_DISTANCE = 1.0f;
	Vector3 startA = a->GetPreviousPosition();
	Vector3 endA = a->GetOwner()->GetPosition();
	Vector3 startB = b->GetPreviousPosition();
	Vector3 endB = b->GetOwner()->GetPosition();

	OBB aObb = a->GetOBB();
	OBB bObb = b->GetOBB();

	float distanceA = (endA - startA).Length();
	float distanceB = (endB - startB).Length();

	float maxDistance = (std::max)(distanceA, distanceB);
	int subStepCount = (std::max)(1, static_cast<int>(std::ceil(maxDistance / MAX_STEP_DISTANCE)));

	OBBColliderComponent* aNonConst = const_cast<OBBColliderComponent*>(a);
	OBBColliderComponent* bNonConst = const_cast<OBBColliderComponent*>(b);

	OBBColliderComponent tempA(nullptr);
	OBBColliderComponent tempB(nullptr);

	for (int step = 0; step < subStepCount; ++step)
	{
		float t = static_cast<float>(step + 1) / subStepCount;
		Vector3 subPosA = startA + (endA - startA) * t;
		Vector3 subPosB = startB + (endB - startB) * t;

		OBB movedOBB_A = aObb;
		OBB movedOBB_B = bObb;
		movedOBB_A.center = subPosA;
		movedOBB_B.center = subPosB;


		tempA.SetOBB(movedOBB_A);
		tempB.SetOBB(movedOBB_B);

		if (CheckOBBvsOBB2D(&tempA, &tempB, plane))
		{
			aNonConst->SetCollisionPosition(subPosA);
			bNonConst->SetCollisionPosition(subPosB);
			return true;
		}
	}
	return false;
}

bool CollisionAlgorithm::CheckAABBvsOBBSubstep2D(const AABBColliderComponent* a, const OBBColliderComponent* b, CollisionPlane plane)
{
	constexpr float MAX_STEP_DISTANCE = 1.0f;
	Vector3 startA = a->GetPreviousPosition();
	Vector3 endA = a->GetOwner()->GetPosition();
	Vector3 startB = b->GetPreviousPosition();
	Vector3 endB = b->GetOwner()->GetPosition();

	const AABB& aBox = a->GetAABB();
	OBB bObb = b->GetOBB();

	float distanceA = (endA - startA).Length();
	float distanceB = (endB - startB).Length();

	float maxDistance = (std::max)(distanceA, distanceB);
	int subStepCount = (std::max)(1, static_cast<int>(std::ceil(maxDistance / MAX_STEP_DISTANCE)));

	AABBColliderComponent* aNonConst = const_cast<AABBColliderComponent*>(a);
	OBBColliderComponent* bNonConst = const_cast<OBBColliderComponent*>(b);

	AABBColliderComponent tempA(nullptr);
	OBBColliderComponent tempB(nullptr);

	for (int step = 0; step < subStepCount; ++step)
	{
		float t = static_cast<float>(step + 1) / subStepCount;
		Vector3 subPosA = startA + (endA - startA) * t;
		Vector3 subPosB = startB + (endB - startB) * t;

		OBB movedOBB = bObb;
		movedOBB.center = subPosB;

		Vector3 aHalf = aBox.GetHalfSize();
		AABB movedAABB(subPosA - aHalf, subPosA + aHalf);


		tempA.SetAABB(movedAABB);
		tempB.SetOBB(movedOBB);

		if (CheckAABBvsOBB2D(&tempA, &tempB, plane))
		{
			aNonConst->SetCollisionPosition(subPosA);
			bNonConst->SetCollisionPosition(subPosB);
			return true;
		}
	}
	return false;
}