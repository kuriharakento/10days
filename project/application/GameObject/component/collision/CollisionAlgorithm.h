#pragma once
#include "AABBColliderComponent.h"
#include "OBBColliderComponent.h"

enum class CollisionPlane
{
	XY, // XY平面
	XZ, // XZ平面
	YZ, // YZ平面
};

namespace CollisionAlgorithm
{
	// --- 3D用判定 ---
		// 基本判定（3D）
	bool CheckAABBvsAABB3D(const AABBColliderComponent* a, const AABBColliderComponent* b);    // AABB同士の3D衝突判定
	bool CheckOBBvsOBB3D(const OBBColliderComponent* a, const OBBColliderComponent* b);        // OBB同士の3D衝突判定
	bool CheckAABBvsOBB3D(const AABBColliderComponent* a, const OBBColliderComponent* b);      // AABBとOBBの3D衝突判定

	// サブステップ判定（3D）
	bool CheckAABBvsAABBSubstep3D(const AABBColliderComponent* a, const AABBColliderComponent* b);
	bool CheckOBBvsOBBSubstep3D(const OBBColliderComponent* a, const OBBColliderComponent* b);
	bool CheckAABBvsOBBSubstep3D(const AABBColliderComponent* a, const OBBColliderComponent* b);

	// --- 2D用判定（平面指定） ---
	// 基本判定（2D）
	bool CheckAABBvsAABB2D(const AABBColliderComponent* a, const AABBColliderComponent* b, CollisionPlane plane);    // AABB同士の2D衝突判定
	bool CheckOBBvsOBB2D(const OBBColliderComponent* a, const OBBColliderComponent* b, CollisionPlane plane);        // OBB同士の2D衝突判定
	bool CheckOBBvsOBB_XY(const OBB& obbA, const OBB& obbB);
	bool CheckOBBvsOBB_XZ(const OBB& obbA, const OBB& obbB);
	bool CheckOBBvsOBB_YZ(const OBB& obbA, const OBB& obbB);
	bool CheckAABBvsOBB2D(const AABBColliderComponent* a, const OBBColliderComponent* b, CollisionPlane plane);      // AABBとOBBの2D衝突判定

	// サブステップ判定（2D）
	bool CheckAABBvsAABBSubstep2D(const AABBColliderComponent* a, const AABBColliderComponent* b, CollisionPlane plane);
	bool CheckOBBvsOBBSubstep2D(const OBBColliderComponent* a, const OBBColliderComponent* b, CollisionPlane plane);
	bool CheckAABBvsOBBSubstep2D(const AABBColliderComponent* a, const OBBColliderComponent* b, CollisionPlane plane);
}

