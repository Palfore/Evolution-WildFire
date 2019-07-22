#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "21";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 71;
	static const long BUILD  = 6097;
	static const long REVISION  = 33126;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 14815;
	#define RC_FILEVERSION 1,71,6097,33126
	#define RC_FILEVERSION_STRING "1, 71, 6097, 33126\0"
	static const char FULLVERSION_STRING [] = "1.71.6097.33126";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 97;
	

}
#endif //VERSION_H
