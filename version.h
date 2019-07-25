#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 72;
	static const long BUILD  = 6130;
	static const long REVISION  = 33277;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 16633;
	#define RC_FILEVERSION 1,72,6130,33277
	#define RC_FILEVERSION_STRING "1, 72, 6130, 33277\0"
	static const char FULLVERSION_STRING [] = "1.72.6130.33277";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 30;
	

}
#endif //VERSION_H
