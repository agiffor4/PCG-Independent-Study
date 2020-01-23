#include "DeltaTime.h"

DeltaTime::DeltaTime()
{

}


DeltaTime::~DeltaTime()
{

}
float DeltaTime::GetCurrentDeltaTime()
{
	m_newTime = a_clock::now();
	std::chrono::duration<float> fs = m_newTime - m_currentTime;
	m_currentTime = a_clock::now();
	//std::chrono::milliseconds d = std::chrono::duration_cast<std::chrono::milliseconds>(fs);
	return fs.count() > 1 ? 1 : fs.count() < 0 ? 0 : fs.count();
}
