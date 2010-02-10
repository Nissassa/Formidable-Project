// Logger.cpp
#include "logger.h"


// Init the instance
CLogger *CLogger::m_pInstance = 0;

// Constrcutor
CLogger::CLogger(void)
{
	m_hConsole = 0;
	m_pInstance = 0;
	m_szBuffer = 0;
	ZeroMemory(&m_tTime, sizeof(SYSTEMTIME));
}

// Destructor
CLogger::~CLogger(void)
{
}

// Initialize the Logger
bool CLogger::Init()
{

#ifdef _DEBUG

	// Allocate and create a console
	if(!AllocConsole())
		return false;

	// Get the output handle
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	return true;

#endif
	return true;
}

// GetInstance - Return the instance
CLogger *CLogger::GetInstance(void)
{
	//	Check if we have an instance
	if (!m_pInstance)
		m_pInstance = new CLogger;

	// Return the new/current instance
	return m_pInstance;
}

// DeleteInstance : Delete the instance of the class.
void CLogger::DeleteInstance(void)
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


bool CLogger::DumpErrorsToFile(char *szFileName, bool append)
{
#ifdef _DEBUG
	// Check to append, or overwrite
	if(append)
		ofl.open(szFileName, std::ios_base::app);
	else
		ofl.open(szFileName);

	// Check for open file
	if(!ofl.is_open())
		return false;


	// output the time Header
	GetLocalTime(&m_tTime);

	// Fuck military time
	if(m_tTime.wHour > 12)
	{
		m_tTime.wHour -= 12;
	}

	ofl << m_tTime.wHour << ":" << m_tTime.wMinute << ":" << m_tTime.wSecond;
	// Write the Date
	ofl << "\t\t\t\t\t";
	ofl << m_tTime.wMonth << "-" << m_tTime.wDay << "-" << m_tTime.wYear << '\n';
	ofl << "---------------------------------------------------------\n";


	// Write the errors to file
	for(unsigned int i = 0; i < m_FileDump.size(); i++)
	{
		ofl << m_FileDump[i];
		ofl << '\n';
	}
	ofl << "\n\n";


	// close and clear dirty bit
	ofl.close();
	ofl.clear();


	return true;
#endif
}

// Output the error to console
bool CLogger::OutputErrorConsole(char *err)
{
#ifdef _DEBUG
	// Set the time for output
	GetLocalTime(&m_tTime);
	char szBuffer[64];

	// Fuck military time
	if(m_tTime.wHour > 12)
	{
		m_tTime.wHour -= 12;
	}
	sprintf_s(szBuffer, 64, "[%d:%d:%d] ", m_tTime.wHour, m_tTime.wMinute, m_tTime.wSecond);


	// write out the time, then the error
	WriteFile(m_hConsole, szBuffer, DWORD(strlen(szBuffer)), 0, 0);
	WriteFile(m_hConsole, err, DWORD(strlen(err)), 0, 0);

	// append a new line
	WriteFile(m_hConsole, "\n", 1, 0, 0);
	
	// Success!
	return true;
#endif
	return true;
}

// Output the error to the log
bool CLogger::OutputErrorLog(char *err)
{
#ifdef _DEBUG
	// put into the
	m_FileDump.push_back(err);
	return true;
#endif
}

// Clears the console window
void CLogger::ClearConsole()
{
#ifdef _DEBUG
	// Clear the console window
	system("cls");
#endif
}

// Clear out the log stream / vector
void CLogger::ClearLogStream()
{
#ifdef _DEBUG
	// clear it out
	m_FileDump.clear();
#endif
}

void CLogger::Shutdown()
{
#ifdef _DEBUG
	// shutdown
	FreeConsole();
#endif
}


