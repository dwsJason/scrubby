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

	void Render();

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

private:

	Target* m_CurrentTarget;

	std::vector<Target*> m_targets;

};


#endif // TARGETS_H_

