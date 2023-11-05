//
// Target Manager Window for Scrubby
//
#ifndef TARGETS_H_
#define TARGETS_H_

#include "imgui.h"
#include <string>
#include <vector>

#include "foenix.h"


class Target
{
public:

	const std::string& GetPortName() { return m_PortName; }
	const std::string& GetPortDescription() { return m_PortDescription; }
	const std::string& GetUserName() { return m_UserName; }

	std::string GetDisplayName();


private:

	std::string m_PortName;
	std::string m_PortDescription;
	std::string m_UserName;

	FoenixDebugPort m_DebugPort;

};


class TargetManager
{
public:
	void Render();

	Target* GetCurrentTarget();

	static TargetManager* GetInstance();

private:
	TargetManager();

	int m_RadioTargetNumber;

	Target* m_CurrentTarget;

	std::vector<Target*> m_targets;

};


#endif // TARGETS_H_

