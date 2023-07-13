#pragma once
#include "stdafx.h"

template <typename T>
class ObjectPool
{
protected:
	std::list<T*> pool; //동적 할당을 위해서 포인터형으로 타입지정
	std::list<T*> useList;

	void CreateObject(int count = 100);

public:
	ObjectPool() = default;
	~ObjectPool();

	void Init(int cacheSize = 100);
	void Release();
	void AllReturn();
	T* Get();
	void Return(T* object);
	const std::list<T*>& GetPool() const;
	const std::list<T*>& GetUseList() const;

	std::function<void(T*)> OnCreate;
};

template<typename T>
inline void ObjectPool<T>::CreateObject(int count)
{
	for (int i = 0; i < count; i++)
	{
		T* object = new T();

		if (OnCreate != nullptr)
		{
			OnCreate(object);
		}
		object->Init();
		pool.push_back(object);
	}
}

template<typename T>
inline ObjectPool<T>::~ObjectPool()
{
	Release();
}

template<typename T>
inline void ObjectPool<T>::Init(int cacheSize)
{
	Release();
	CreateObject(cacheSize);
}

template<typename T>
inline void ObjectPool<T>::Release()
{
	AllReturn();

	for (auto object : pool)
	{
		delete object;
	}
	pool.clear();
}

template<typename T>
inline void ObjectPool<T>::AllReturn()
{
	for (auto object : useList)
	{
		object->SetActive(false);
		pool.push_back(object);
	}
	useList.clear();
}

template<typename T>
inline T* ObjectPool<T>::Get()
{
	if (pool.empty())
	{
		CreateObject(); // 기본값 100
	}

	T* object = pool.front();
	pool.pop_front();
	object->SetActive(true);
	object->Reset();

	useList.push_back(object);

	return object;
}

template<typename T>
inline void ObjectPool<T>::Return(T* object)
{
	if (std::find(useList.begin(), useList.end(), object) == useList.end())
	{
		std::cout << "ObjectPool : Return Error" << std::endl;
		return;
	}
	useList.remove(object);
	object->SetActive(false);
	pool.push_back(object);
}

template<typename T>
inline const std::list<T*>& ObjectPool<T>::GetPool() const
{
	return pool;
}

template<typename T>
inline const std::list<T*>& ObjectPool<T>::GetUseList() const
{
	return useList;
}
