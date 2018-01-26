#include "Log.h"


CLog *CLog::m_pInstance = nullptr;


CLog * CLog::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CLog();
	}
	return m_pInstance;
}

const CLog & CLog::GetReference()
{
	// TODO: 在此处插入 return 语句
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CLog();
	}
	return *m_pInstance;
}

int CLog::Write(CLog::LogType a, char* func, int line, char *fmt, ...)
{
	//Write(LogType::INFO, func, line, fmt, ...);
	CLog *pLog = GetInstance();
	if (pLog->m_pThread.native_handle() == nullptr)
	{
		pLog->m_pThread = std::thread(&CLog::run, pLog);
		//pLog->m_pThread.detach();
	}
	if (pLog == nullptr)
	{
		return 0;
	}
	std::unique_lock<std::mutex> lck(pLog->m_pMutex);// , std::defer_lock);
	int cnt = 0;
	//if (lck.try_lock())
	{
		size_t tmp = pLog->m_nCurBlank;
		LogMessage *pMesg = &pLog->m_pMessage[tmp];
		memset(pMesg, 0, sizeof(LogMessage));
		pMesg->type = a;
		memcpy(pMesg->pFunc, func, strnlen_s(func, sizeof(pMesg->pFunc)));
		pMesg->mLine = line;
		va_list argptr;
		
		__crt_va_start(argptr, fmt);
		cnt = vsprintf_s(pMesg->data, fmt, argptr);
		__crt_va_end(argptr);
		pLog->m_nCurBlank = (tmp + 1) % MAX_LOG_NUM;
		pLog->SetFlag(true);
		pLog->m_pCond.notify_one();
	}
	
	
	return cnt;
}


void CLog::run()
{
	
	LogMessage buffer;
	while (1)
	{
		CLog *pLog = CLog::GetInstance();
		std::unique_lock<std::mutex> lck(m_pMutex);
		/*if (!lck.try_lock())
		{
			continue;
		}*/
		m_pCond.wait(lck, [&pLog] {return pLog->GetFlag(); });
		//m_pMessage[m_nCurValid];
		memcpy(&buffer, &m_pMessage[m_nCurValid], sizeof(LogMessage));
		memset(&m_pMessage[m_nCurValid], 0, sizeof(LogMessage));
		m_nCurValid = (m_nCurValid + 1) % MAX_LOG_NUM;
		if (m_nCurBlank == m_nCurValid)
		{
			pLog->SetFlag(false);
		}
		//lck.unlock();
		if (m_pLog.good())
		{
			if (buffer.type == LogType::FATAL_ERROR)
			{
				m_pLog << "Error\t";
			}
			else if (buffer.type == LogType::INFO)
			{
				m_pLog << "Info\t";
			}
			else if (buffer.type == LogType::WARNING)
			{
				m_pLog << "Warning\t";
			}
			else if (buffer.type == LogType::EXIT)
			{
				break;
			}
			m_pLog << "[" << buffer.pFunc << "," << buffer.mLine << "]" << buffer.data << "\n";
		}
	}
	m_pLog.close();
}

CLog::CLog()
{
	init();
	
}

void CLog::init()
{
	m_pMessage = new LogMessage[MAX_LOG_NUM];
	memset(m_pMessage, 0, sizeof(LogMessage) * MAX_LOG_NUM);
	m_nCurBlank = 0;
	m_nCurValid = 0;
	m_bFlag = false;

	m_pLog.open("log.txt", std::ios::trunc | std::ios::in | std::ios::out);
}

void CLog::SetFlag(bool flag)
{
	m_bFlag = flag;
}

bool CLog::GetFlag()
{
	return m_bFlag;
}


CLog::~CLog()
{
	if (m_pThread.joinable())
	{
		WriteExit("End");
		m_pThread.join();
	}
	std::chrono::duration<int, std::milli> a = std::chrono::milliseconds(100);
	std::this_thread::sleep_for(a);
	//m_pLog.close();
	if (m_pMessage != nullptr)
	{
		delete[]m_pMessage;
		m_pMessage = nullptr;
	}
}

