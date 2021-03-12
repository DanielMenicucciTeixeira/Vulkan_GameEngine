#ifndef SINGLETON_H
#define SINGLETON_H

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\\
//! MAKE SURE TO ONLY UNSE "ONE" OF THE MACROS BELLOW PER CLASS     !\\
//! DO NOTE DEFINE MORE THEN ONE OF THE #ifdef CONDITIONS PER CLASS !\\
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\\


//Include "memory (#include <memory>) before the classe's declaration.
//Then define singletonClass as the classe's name as the first thing in it's definition, right after the "{".
//Then include this flie (#include Auxiliary/Singleton.h).
//Then include the SingletonCpp.h file in the classes cpp (#include "Auxiliary/SingletonCPP.h")



#include <memory>

//Deletes all the move and copy constructors for the class.
//Creates a GetInstance static funtion that returns a unique_ptr to the only class instance that will be allowed to exist.
//If no class instance exists yet, GetInstance() will create one.
//Makes the Constructor() and Destructor() of the class private.

/*template <typename singletonClass>
class Singleton
{
	friend singletonClass;

private:
	Singleton() = default;
	virtual ~Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	friend std::default_delete<Singleton>;

protected:
	static std::unique_ptr<singletonClass> Instance;

public:
	static singletonClass* GetInstance();
	static std::unique_ptr<singletonClass> GetInternalInstance()
	{ 
		//if (Instance.get() == nullptr) Instance.reset(new singletonClass);
		return Instance; 
	}
};

template <typename singletonClass>
singletonClass* Singleton<singletonClass>::GetInstance()
{
	if (instance.get() == nullptr) instance.reset(new singletonClass);
	return instance.get();
}*/

//Deletes all the move and copy constructors for the class.
template<typename noCopyClass>
class NoCopyClass
{
	friend noCopyClass;
public:
	NoCopyClass() = default;
	virtual ~NoCopyClass() = default;

	NoCopyClass(const NoCopyClass&) = delete;
	NoCopyClass(NoCopyClass&&) = delete;
	NoCopyClass& operator=(const NoCopyClass&) = delete;
	NoCopyClass& operator=(NoCopyClass&&) = delete;
};

//Deletes all the move, copy constructors for the class and main constructor of the class.
template<typename staticClass>
class StaticClass
{
	friend staticClass;
public:
	virtual ~StaticClass() = default;

	StaticClass(const StaticClass&) = delete;
	StaticClass(StaticClass&&) = delete;
	StaticClass& operator=(const StaticClass&) = delete;
	StaticClass& operator=(StaticClass&&) = delete;
	StaticClass() = delete;
};


#endif
