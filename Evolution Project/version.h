#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "03";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 35;
	static const long BUILD  = 2448;
	static const long REVISION  = 13193;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4643;
	#define RC_FILEVERSION 1,35,2448,13193
	#define RC_FILEVERSION_STRING "1, 35, 2448, 13193\0"
	static const char FULLVERSION_STRING [] = "1.35.2448.13193";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 48;
	

}
#endif //VERSION_H
