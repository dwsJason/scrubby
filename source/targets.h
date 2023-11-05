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
	Target(std::string portName, std::string portDescription)
		: m_PortName(portName)
		, m_PortDescription(portDescription)
	{
	}

	bool IsConnected() { return m_DebugPort.IsConnected(); }

	bool Connect()
	{
		if (!m_DebugPort.IsConnected())
		{
			Disconnect();
			m_DebugPort.OpenSerialPort(m_PortName.c_str());
		}

		return IsConnected();
	}

	void Disconnect() { m_DebugPort.CloseSerialPort(); }

	u16 Target::Send(ECommand Command, u32 TargetAddress = 0, std::vector<u8>* pPayLoad = nullptr)
	{
		if (IsConnected())
		{
			return m_DebugPort.Send(Command, TargetAddress, pPayLoad);
		}

		return 0;
	}

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

	Target* GetCurrentTarget() { return m_CurrentTarget; }

	static TargetManager* GetInstance();

private:
	TargetManager();

	int m_RadioTargetNumber;

	Target* m_CurrentTarget;

	// Cached List of strings for the ports
	bool m_PortsCacheDirty;
	std::vector<std::string> m_PortDesc;
	std::vector<std::string> m_PortName;

	FoenixDebugPort m_DebugPort;


	std::vector<Target*> m_targets;

};


#endif // TARGETS_H_

