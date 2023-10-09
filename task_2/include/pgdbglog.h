
#ifndef __PGDBGLOGGER_H__ //{
#define __PGDBGLOGGER_H__

#define pgdebug(...) if (glob_bTraceEnabled == 1){ PgDbgLogger(0, __LINE__, __FILE__, __VA_ARGS__);}

#define pgerror(...) PgDbgLogger(1, __LINE__, __FILE__, __VA_ARGS__)

#define AMOUNT_OF_XML_PROPS 2		// Amount of properties in the config.xml file.
#define LENGTH_OF_PROP_VALUE 80	// Length of the string holding the property value.

int glob_bTraceEnabled;

void PgDbgLogger(unsigned long ulErrorType, int iLine, const char *szFile, const char *pszFormat, ...);

void PgDbgInitialize();

void ParsingXmlFile( char xmlProp[][LENGTH_OF_PROP_VALUE]);

// }
#endif // __PGDBGLOGGER_H__
