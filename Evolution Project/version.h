#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "15";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 44;
	static const long BUILD  = 3316;
	static const long REVISION  = 18014;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 6447;
	#define RC_FILEVERSION 1,44,3316,18014
	#define RC_FILEVERSION_STRING "1, 44, 3316, 18014\0"
	static const char FULLVERSION_STRING [] = "1.44.3316.18014";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 16;
	

}
#endif //VERSION_H
