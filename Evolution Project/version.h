#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 25;
	static const long BUILD  = 1454;
	static const long REVISION  = 7866;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2666;
	#define RC_FILEVERSION 1,25,1454,7866
	#define RC_FILEVERSION_STRING "1, 25, 1454, 7866\0"
	static const char FULLVERSION_STRING [] = "1.25.1454.7866";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 54;
	

}
#endif //VERSION_H
