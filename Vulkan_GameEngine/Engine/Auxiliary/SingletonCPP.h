#ifndef SINGLETONCPP_H
#define SINGLETONCPP_H

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\\
//! SEE INSTRUCTIONS IN "Auxiliary/Singleton.h" !\\
//! DO NOT USE WITHOUT "Auxiliary/Singleton.h"  !\\
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\\

	#ifdef singletonClass
			std::unique_ptr<singletonClass> singletonClass::Instance = nullptr;
	#endif

#endif