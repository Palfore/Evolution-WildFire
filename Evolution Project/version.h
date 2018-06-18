#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 51;
	static const long BUILD  = 4012;
	static const long REVISION  = 21815;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 7856;
	#define RC_FILEVERSION 1,51,4012,21815
	#define RC_FILEVERSION_STRING "1, 51, 4012, 21815\0"
	static const char FULLVERSION_STRING [] = "1.51.4012.21815";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 12;
	

}
#endif //VERSION_H
