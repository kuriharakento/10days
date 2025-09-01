#pragma once
#include "BlackBoard.h"

// ノードのステータスを表す
enum class NodeStatus { Success, Failure, Running };

/**
 * \brief すべてのノードの共通インターフェース
 */
class BTNode
{
public:
    virtual ~BTNode() = default;
	virtual NodeStatus Tick(Blackboard& blackboard) = 0; // ノードの実行
    virtual void Reset() {} // 状態のリセットなどに使用
	virtual const  std::string& GetNodeName() const { return nodeName_; } // ノード名の取得
	NodeStatus GetLastStatus() const { return lastStatus_; } // 最後の実行結果の取得

protected:
	std::string nodeName_ = "BTNode"; // ノードの名前
	NodeStatus lastStatus_ = NodeStatus::Failure; // 最後の実行結果のステータス
};