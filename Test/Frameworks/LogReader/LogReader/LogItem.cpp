//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 � All Rights Reserved.
//
// FILE:		LogItem.cpp
//
// DESCRIPTION: Contains implementation for the log item class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#include "logitem.h"
#include "defines.h"

namespace HoloScript
{
	//*************************************************************************
	// Method:		LogItem
	// Description: Constructor for the LogItem class
	//
	// Parameters:
	//	v - the Holodeck version of this log item
	//
	// Return Value: None
	//*************************************************************************
	LogItem::LogItem()
	{
		Initialize();
	}

	//*************************************************************************
	// Method:		~LogItem
	// Description: Destructor for the LogItem class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogItem::~LogItem()
	{
		timeStamp = NULL;
		threadID = NULL;
		category = NULL;
		dllName = NULL;
		function = NULL;
		errorCode = NULL;
		exception = NULL;
		returnValue = NULL;
		parameters = NULL;
	}

	void LogItem::Initialize()
	{		
		timeStamp = NULL;
		threadID = NULL;
		category = NULL;
		dllName = NULL;
		function = NULL;
		errorCode = NULL;
		exception = NULL;
		returnValue = NULL;
		parameters = NULL;
	}

	//*************************************************************************
	// Method:		ConvertFromByteSequence
	// Description: DeSerializes the data into a log entry structure.  All values
	//	before this call will get overwritten.  The logical opposite of
	//	ConvertToByteSequence.
	//
	// Parameters:
	//	buffer - the buffer that holds the byte sequence to convert.
	//		Assumptions: Was generated by ConvertToByteSequence and not tampered with
	//		since.
	//
	// Return Value: None
	//*************************************************************************
	void LogItem::ConvertFromByteSequence(unsigned char *buffer)
	{
		if (buffer == NULL)
			return;

		unsigned int offset = 0;
		unsigned int i = 0;

		// get the structure size
		unsigned int logEntrySize = *((unsigned int*)(buffer + offset));
		offset += sizeof(unsigned int);

		// get the call level		
		callDepth = *((unsigned int *)(buffer + offset));
		offset += sizeof(unsigned int);		

		// get the number of parameters
		unsigned int numParameters = *((unsigned int*)(buffer + offset));
		offset += sizeof(unsigned int);

		if (numParameters > MAX_PARAMETER_COLUMNS)
			numParameters = MAX_PARAMETER_COLUMNS;

		// get the number of events
		unsigned int numEvents;
		
		numEvents = *((unsigned int*)(buffer + offset));
		offset += sizeof(unsigned int);
			
		// get the timeStamp length
		unsigned int timeStampLength = *((unsigned int*)(buffer + offset));;
		offset += sizeof(unsigned int);

		// get the threadID length
		unsigned int threadIDLength = *((unsigned int*)(buffer + offset));;
		offset += sizeof(unsigned int);

		// get the category length
		unsigned int categoryLength = *((unsigned int*)(buffer + offset));;
		offset += sizeof(unsigned int);

		// get the dllName length
		unsigned int dllNameLength = *((unsigned int*)(buffer + offset));;
		offset += sizeof(unsigned int);

		// get the function length
		unsigned int functionLength = *((unsigned int*)(buffer + offset));;
		offset += sizeof(unsigned int);

		// get the returnValue length
		unsigned int returnValueLength = *((unsigned int*)(buffer + offset));;
		offset += sizeof(unsigned int);

		// get the errorCode length
		unsigned int errorCodeLength = *((unsigned int*)(buffer + offset));;
		offset += sizeof(unsigned int);

		// get the exception length
		unsigned int exceptionLength = *((unsigned int*)(buffer + offset));;
		offset += sizeof(unsigned int);
	
		// get the parameter lengths
		unsigned int parameterLengths[MAX_PARAMETER_COLUMNS];
		for (i = 0; i < numParameters; i++)
		{
			parameterLengths[i] = *((unsigned int*)(buffer + offset));;
			offset += sizeof(unsigned int);
		}
	
		// get the event lengths (numEvents is zero for old logs)
		unsigned int eventLengths __gc[] = new unsigned int __gc[numEvents];
		for (i = 0; i < numEvents; i++)
		{
			eventLengths[i] = *((unsigned int*)(buffer + offset));;
			offset += sizeof(unsigned int);
		}

		// get the time stamp
		SYSTEMTIME systemTime;
		FILETIME fileTime;
		ULARGE_INTEGER uLargeInt;
		memcpy(&systemTime, (buffer + offset), timeStampLength);
		offset += timeStampLength;
		timeStamp = ConvertTimeToString(systemTime);
		SystemTimeToFileTime(&systemTime, &fileTime);
		uLargeInt.LowPart = fileTime.dwLowDateTime;
		uLargeInt.HighPart = fileTime.dwHighDateTime;
		uint64TimeStamp = uLargeInt.QuadPart;

		// get the threadID
		threadID = new String((char*)(buffer + offset), 0, threadIDLength);
		offset += threadIDLength;

		// get the category
		category = new String((char*)(buffer + offset), 0, categoryLength);
		offset += categoryLength;

		// get the dllName
		dllName = new String((char*)(buffer + offset), 0, dllNameLength);
		offset += dllNameLength;

		// get the function
		function = new String((char*)(buffer + offset), 0, functionLength);
		offset += functionLength;

		// get the returnValue
		returnValue = new String((char*)(buffer + offset), 0, returnValueLength);
		offset += returnValueLength;

		// get the errorCode
		errorCode = new String((char*)(buffer + offset), 0, errorCodeLength);
		offset += errorCodeLength;

		// get the exception
		exception = new String((char*)(buffer + offset), 0, exceptionLength);
		offset += exceptionLength;

		// get the parameters
		parameters = new ArrayList();
		for (i = 0; i < numParameters; i++)
		{
			if (parameterLengths[i] > 0)
			{
				parameters->Add(new String((char*)(buffer + offset), 0, parameterLengths[i]));
				offset += parameterLengths[i];
			}
			else
				parameters->Add(new String("0"));
		}

		// get the events (numEvents is zero for old logs)
		events = new ArrayList();
		for (i = 0; i < numEvents; i++)
		{
			if (eventLengths[i] > 0)
			{
				LogEvent* event = new LogEvent();

				// First byte is the type
				event->Type = (LogEventType)(*(char*)(buffer + offset));
				offset++;

				// Rest of length is the data
				event->Bytes = new unsigned char __gc[eventLengths[i] - 1];
				for (unsigned j = 0; j < eventLengths[i] - 1; j++)
					event->Bytes[j] = (*(unsigned char*)(buffer + offset + j));
				event->StringValue = new String((char*)(buffer + offset));
				offset += eventLengths[i] - 1;

				events->Add(event);
			}
		}
	}

	//*************************************************************************
	// Method:		ConvertTimeToString
	// Description: converts a system time struct into a string
	//
	// Parameters:
	//	systemTime - time structure to convert
	//
	// Return Value: string that represents this system time
	//*************************************************************************
	String *LogItem::ConvertTimeToString(SYSTEMTIME systemTime)
	{
		const int bufferSize = 63; // big enough to hold the timestamp in its current format
		char buffer[bufferSize + 1];
		String *timeString;
		
		ZeroMemory(buffer, bufferSize + 1);
		wsprintf(buffer, "%2d/%2d/%2d  %2d:%2d:%2d:%3d", 
			systemTime.wMonth, systemTime.wDay, systemTime.wYear, systemTime.wHour,
			systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
		
		timeString = new String(buffer);

		return timeString;
	}
}