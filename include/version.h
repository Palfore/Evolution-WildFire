#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "20";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 54;
	static const long BUILD  = 4303;
	static const long REVISION  = 23506;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 8583;
	#define RC_FILEVERSION 1,54,4303,23506
	#define RC_FILEVERSION_STRING "1, 54, 4303, 23506\0"
	static const char FULLVERSION_STRING [] = "1.54.4303.23506";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 3;
	

}
#endif //VERSION_H
