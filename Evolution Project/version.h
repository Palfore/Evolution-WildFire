#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 9;
	static const long BUILD  = 958;
	static const long REVISION  = 5196;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1741;
	#define RC_FILEVERSION 1,9,958,5196
	#define RC_FILEVERSION_STRING "1, 9, 958, 5196\0"
	static const char FULLVERSION_STRING [] = "1.9.958.5196";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 58;
	

}
#endif //VERSION_H
