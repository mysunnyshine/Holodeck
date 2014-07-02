#include "common.h"


BOOL My_DnsHostnameToComputerNameA()
{
	LPCSTR Hostname=NULL;
	LPSTR ComputerName=NULL;
	LPDWORD nSize=NULL;
	BOOL returnVal_Real = NULL;
	BOOL returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = DnsHostnameToComputerNameA (Hostname,ComputerName,nSize);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = DnsHostnameToComputerNameA (Hostname,ComputerName,nSize);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}