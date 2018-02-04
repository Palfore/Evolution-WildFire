#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "03";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 5;
<<<<<<< HEAD
	static const long BUILD  = 552;
	static const long REVISION  = 2942;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1022;
	#define RC_FILEVERSION 1,5,552,2942
	#define RC_FILEVERSION_STRING "1, 5, 552, 2942\0"
	static const char FULLVERSION_STRING [] = "1.5.552.2942";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 52;
=======
	static const long BUILD  = 525;
	static const long REVISION  = 2792;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 969;
	#define RC_FILEVERSION 1,5,525,2792
	#define RC_FILEVERSION_STRING "1, 5, 525, 2792\0"
	static const char FULLVERSION_STRING [] = "1.5.525.2792";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 25;
>>>>>>> master
	

}
#endif //VERSION_H
