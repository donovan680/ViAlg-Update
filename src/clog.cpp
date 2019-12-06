/*-----------------------------------------------------------------------------+
| File name: clog.hpp							   						       |
| Date:		 5 October 2005													   |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Log system that is represented by CLog class.   							   |
| Based on the NeHe tutorials and code to the "Focus on 3D Terrain Programming"|
| by Trent Polack.															   |
+-----------------------------------------------------------------------------*/

/*
system log�w b�dzie bardzo prosty, oparty w�a�ciwie na jednej czy kilku funkcjach
inicjujemy obiekt podaj�c nazw� pliku do wpis�w, a p�xniej wywo�ujemy metod� Write
lub co� w tym stylu aby dokona� wpisu.
Bardzo ciekawa metoda polega na zapisie w HTMLu dzi�ki czemu mo�na stosowa� r�ne
kolory, podbieram ten pomys� z kodu do wcze�niej wspomnianej ksi��ki.

format pliku HTML
Log Started
Date and Time: ....

 - wiadomo�� 1	 (w danym kolorze)
 - wiadomo�� 2
 - ...
*/

#include "clog.h"
#include <string_view>

/*-----------------------------------------------------------------------------+
|                      Implementation of the CLog class                        |
+-----------------------------------------------------------------------------*/

// constructor:
CLog::CLog() noexcept :
	m_bEnabled(false)
{
	m_szFileName[0] = '\0';
}

// constructor 2:
CLog::CLog(const char *szFileName) noexcept
{
	Init(szFileName);
}

/*-----------------------------------------------------------------------------+
|                                    Init                                      |
+------------------------------------------------------------------------------+
| Description:																   |
|    Initializes the Log system, opens the specific file and writes basic info |
|    to it such as date, and time...			                               |
| Arguments:																   |
|  - const char *szFileName - file name                    					   |
| Return value:																   |
|    true if everything goes ok, unless false								   |
+-----------------------------------------------------------------------------*/
bool CLog::Init(const char *szFileName) noexcept {
	strcpy_s(m_szFileName, szFileName);

	// open clean file and write some basic info:
	FILE *f;
	fopen_s(&f, m_szFileName, "w+");

	if (f == NULL)
		return false;

	m_bEnabled = true;

	// date and time:
	char timestring[280];
	__time64_t ltime;

    _time64( &ltime );
	_ctime64_s(timestring, &ltime);
    //sprintf_s(timestring, "%s",  );

    // module name:
	char szModuleName[128];
	GetModuleFileName(NULL, szModuleName, 128);
	fprintf(f, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
			   "<HTML>\n"
			   "<HEAD>\n"
			   "<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-2\">"
			   "<TITLE>Log File</TITLE>\n"
			   "<style> p { font-family: Courier; margin:0;} .info { color: black; } .err {color: #aa6666;} .success {color:#66aa66;}</style>\n"
			   "</HEAD>"
			   "<BODY>\n");

	fprintf(f, "<p class=\"info\"> Log for the \"%s\" started!</p>\n", szModuleName);
	fprintf(f, "<p class=\"info\"> Date and time: %s </p>\n", timestring);
	fprintf(f, "</BODY>\n"
		       "</HTML>\n");

	fclose(f);
	return true;
}

/*-----------------------------------------------------------------------------+
|                                   AddMsg                                     |
+------------------------------------------------------------------------------+
| Description:																   |
|    Writes MSG into the log file.											   |
|    			                              							       |
| Arguments:																   |
|  - LOG_MODE lmMode - type of the message (Error, Normal, or Success)         |
|  - char *szMsg - message string with argumets ( ... )                        |
| Return value:																   |
|    none								                                       |
+-----------------------------------------------------------------------------*/
void CLog::AddMsg(LOG_MODE lmMode, const char *szMsg, ...) noexcept {
	if (m_bEnabled == false)
		return;
	
	FILE *f;
	fopen_s(&f, m_szFileName, "r+");
	if (f == NULL)
		return;

	// parse argument to the string:
	char buf[512];

	va_list arglist;

	if (!szMsg)
   		return;

	va_start(arglist, szMsg);
	vsprintf_s(buf, szMsg, arglist);
	va_end(arglist);

	// overwrite last 16 characters - that is "</BODY>\n"..."
	fseek(f, -18, SEEK_END);
	fprintf(f, "<p class=\"%s\"> %s  </p>\n", GetClassName(lmMode), buf);
	fprintf(f, "</BODY>\n"
		       "</HTML>\n");


	fclose(f);
}

/*-----------------------------------------------------------------------------+
|                                GetColorName                                  |
+------------------------------------------------------------------------------+
| Description:																   |
|    Decodes LOG_MODE into the color used in HTML file          			   |
|    			                              							       |
| Arguments:																   |
|  - LOG_MODE lmMode - type of the message (Error, Normal, or Success)         |
| Return value:																   |
|    array of char that represent color fe. "ffeeff"						   |
+-----------------------------------------------------------------------------*/
const char *CLog::GetClassName(LOG_MODE lmMode) const noexcept {
	static constexpr std::string_view LogClasses[] = { "info", 
												 		"err",  
														"success" };

	return LogClasses[(int)lmMode].data();
}

// end of file ----------------------------------------------------------------+

