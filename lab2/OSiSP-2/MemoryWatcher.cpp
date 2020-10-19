#include "MemoryWatcher.h"


void ThreadWriteMemory(std::string, std::string);

MemoryWatcher::MemoryWatcher(std::string Path) {
	_path = Path;
}
void MemoryWatcher::WriteMemory(bool wait, std::string message = "") {
	std::thread thread(ThreadWriteMemory, this->_path, message);
	if (wait) {
		thread.join();
	}
	else {
		thread.detach();
	}
}

void ThreadWriteMemory(static std::string path, std::string message) {
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(PROCESS_MEMORY_COUNTERS));
	std::ofstream file(path,std::ios_base::app);
	try {
		if (file.is_open())
		{
			int pfc = pmc.PageFaultCount;
			int pfu = pmc.PagefileUsage;
			int wss = pmc.WorkingSetSize;
			int pwss = pmc.PeakWorkingSetSize;
			int qppu = pmc.QuotaPagedPoolUsage;
			int qpppu = pmc.QuotaPeakPagedPoolUsage;
			int qnppu = pmc.QuotaNonPagedPoolUsage;
			int qpnppu = pmc.QuotaPeakNonPagedPoolUsage;
			message += ("\n���������� ������ �������: " + std::to_string(pfc));
			message += "\n����� �� ��������(����� ����� ������� ������): " + std::to_string(pfu);
			message += "\n������� ������ �������� ������ � ������: " + std::to_string(wss);
			message += "\n������������ ������ �������� ������ � ������: " + std::to_string(pwss);
			message += "\n������� ������������� ������������ ���� � ������: " + std::to_string(qppu);
			message += "\n������� ������������� ������������ ���� � ������: " + std::to_string(qpppu);
			message += "\n������� ������������� �������������� ���� � ������: " + std::to_string(qnppu);
			message += "\n������� ������������� �������������� ���� � ������: " + std::to_string(qpnppu);

			file << message << std::endl;
			file.close();
		}
	}
	catch (...) {
		file.close();
	}
}
