#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "29";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.10";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 72;
	static const long BUILD  = 6162;
	static const long REVISION  = 33451;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 17008;
	#define RC_FILEVERSION 1,72,6162,33451
	#define RC_FILEVERSION_STRING "1, 72, 6162, 33451\0"
	static const char FULLVERSION_STRING [] = "1.72.6162.33451";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 62;
	

}
#endif //VERSION_H
