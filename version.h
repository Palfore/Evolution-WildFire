#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 72;
	static const long BUILD  = 6164;
	static const long REVISION  = 33464;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 17112;
	#define RC_FILEVERSION 1,72,6164,33464
	#define RC_FILEVERSION_STRING "1, 72, 6164, 33464\0"
	static const char FULLVERSION_STRING [] = "1.72.6164.33464";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 64;
	

}
#endif //VERSION_H
