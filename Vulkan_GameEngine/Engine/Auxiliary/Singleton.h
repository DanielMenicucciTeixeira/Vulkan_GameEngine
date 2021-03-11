#ifndef SINGLETON_H
#define SINGLETON_H

	#ifdef singletonClass
	
	public:
		singletonClass(const singletonClass&) = delete;
		singletonClass(singletonClass&&) = delete;
		singletonClass& operator=(const singletonClass&) = delete;
		singletonClass& operator=(singletonClass&&) = delete;

		static singletonClass* GetInstance()
		{
			if (Instance.get() == nullptr) Instance.reset(new singletonClass);
			return Instance.get();
		}
	private:
		static std::unique_ptr<singletonClass> Instance;
		friend std::default_delete<singletonClass>;
	public:
	#endif

	#ifdef noCopyClass
		public:
			noCopyClass(const noCopyClass&) = delete;
			noCopyClass(noCopyClass&&) = delete;
			noCopyClass& operator=(const noCopyClass&) = delete;
			noCopyClass& operator=(noCopyClass&&) = delete;
	#endif

	#ifdef staticClass
			public:
				staticClass(const staticClass&) = delete;
				staticClass(staticClass&&) = delete;
				staticClass& operator=(const staticClass&) = delete;
				staticClass& operator=(staticClass&&) = delete;
				staticClass() = delete;
	#endif

#endif
