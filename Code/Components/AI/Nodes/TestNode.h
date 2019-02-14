#pragma once

#include <CryAISystem/BehaviorTree/Node.h>
#include <CryAISystem/BehaviorTree/IBehaviorTree.h>
#include <CryAISystem/BehaviorTree/NodeFactory.h>
#include <CryAISystem/BehaviorTree/XmlLoader.h>

using namespace BehaviorTree;

class MyNode : public BehaviorTree::Node
{
	typedef BehaviorTree::Node BaseClass;
public:
	// Every instance of a node in a tree for an agent will have a
	// runtime data object.
	//
	// This data is persistent from the point when a node is visited
	// until the point when it's left.
	//
	// If this struct is left out the code won't compile.
	//
// This would be variables like 'bestPostureID', 'shotsFired' etc.
	struct RuntimeData
	{
	};
	MyNode() : m_speed(0.0f)
	{}
	// This is where you'll load the configuration data from the XML file
	// into members of the node. They can only be written to during the loading phase
	// and are conceptually immutable (read-only) later while the game is running.
	virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context)
	{
		if (BaseClass::LoadFromXml(xml, context) == LoadFailure)
			return LoadFailure;
		xml->getAttr("speed", m_speed);
		return LoadSuccess;
	}
protected:
	// Called right before the first update
	virtual void OnInitialize(const UpdateContext& context)
	{
		BaseClass::OnInitialize(context);
		// Optional: access runtime data like this
		auto& runtimeData = GetRuntimeData<RuntimeData>(context);
	}
	// Called when the node is terminated
	virtual void OnTerminate(const UpdateContext& context)
	{
		BaseClass::OnTerminate(context);
		// Optional: access runtime data like this
		auto& runtimeData = GetRuntimeData<RuntimeData>(context);
	}
	virtual Status Update(const UpdateContext& context)
	{
		// Perform your update code and report back whether your
		// node succeeded, failed or is running and needs more
		// time to carry out its task.
		// Optional: access runtime data like this
		auto& runtimeData = GetRuntimeData<RuntimeData>(context);
		return Success;
	}
	// Handle any incoming events sent to this node
	virtual void HandleEvent(const EventContext& context, const Event& event)
	{
		// Optional: access runtime data like this
		auto& runtimeData = GetRuntimeData<RuntimeData>(context);
	}
private:
	// Store any configuration data for the node right here.
	// This would be immutable things like 'maxSpeed', 'duration',
	// 'threshold', 'impulsePower', 'soundName', etc.
	float m_speed;
	
};
// Generate an object specialized to create a node of your type upon
// request by the node factory. The macro drops a global variable here.

//GenerateBehaviorTreeNodeCreator(MyNode);