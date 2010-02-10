// Logger.h
// Adam Grzeszczak - 6/16/07
// Logs errors to a console window, or to file.

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include <vector>


using std::vector;
using std::ofstream;


class CLogger
{

private:
	
	HANDLE m_hConsole;					// Console handle - used for outputting to the console window
	static CLogger *m_pInstance;		//	The Instance of this class.

	char *m_szBuffer;					// Buffer for output

	vector<char *> m_FileDump;			// The list of Errors to dump to file
	ofstream ofl;						// output file stream

	SYSTEMTIME m_tTime;					// System time, used for logging time


public:

	// Constructor
	CLogger(void);

	// Destructor
	~CLogger(void);


	/////////////////////////////////////////////////////////////////////
	// Function: "Init"
	//
	// Purpose: Initializes the Logger, and sets up the Console window
	//
	// Out:		bool		- True if success, false if it failed	
	/////////////////////////////////////////////////////////////////////
	bool Init();

	/////////////////////////////////////////////////////////////////////
	// Function: "GetInstance"
	//
	// Purpose: Get the current instance of the logger
	//
	// Out:		CLogger *	-	The current instance of the logger
	/////////////////////////////////////////////////////////////////////
	static CLogger *GetInstance(void);

	/////////////////////////////////////////////////////////////////////
	// Function: "DeleteInstance"
	//
	// Purpose: Delete the instance of the logger and free the memory
	/////////////////////////////////////////////////////////////////////
	static void DeleteInstance(void);

	/////////////////////////////////////////////////////////////////////
	// Function: "DumpErrorsToFile"
	//
	// Purpose: Write the Errors to a file (from the vector)
	//
	// In:		szFileName	-	Path of the file to write out to
	//			append		-	True = append, false = truncate
	//
	// Out:		bool		-	True = success, false = fail		
	/////////////////////////////////////////////////////////////////////
	bool DumpErrorsToFile(char *szFileName, bool append = true);

	/////////////////////////////////////////////////////////////////////
	// Function: "OutputErrorConsole"
	//
	// Purpose: Output errors to the console window
	//
	// In:		err		-	The String or error to output
	//
	// Out:		bool	-	True = success, false = fail
	/////////////////////////////////////////////////////////////////////
	bool OutputErrorConsole(char *err);

	/////////////////////////////////////////////////////////////////////
	// Function: "OutputErrorLog"
	//
	// Purpose: Puts the error into a vector to be dumped to file later
	//			Call this instead of OutputErrorConsole, Then Dump at end.
	//
	// In:		err		-	The string or error to output
	//
	// Out:		bool	-	True = success, false = fail
	/////////////////////////////////////////////////////////////////////
	bool OutputErrorLog(char *err);

	/////////////////////////////////////////////////////////////////////
	// Function: "ClearConsole"
	//
	// Purpose: Clears the console window		
	/////////////////////////////////////////////////////////////////////
	void ClearConsole();

	/////////////////////////////////////////////////////////////////////
	// Function: "ClearLogStream"
	//
	// Purpose: Clears out the vector to be dumped to file
	/////////////////////////////////////////////////////////////////////
	void ClearLogStream();

	/////////////////////////////////////////////////////////////////////
	// Function: "Shutdown"
	//
	// Purpose: Shuts down the Logger and frees up memory
	/////////////////////////////////////////////////////////////////////
	void Shutdown();

};

#endif