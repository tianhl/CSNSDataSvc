#ifndef DYNAMIC_SINGLETON_H
#define DYNAMIC_SINGLETON_H

#include <cstdlib>
#include <cassert>
#include <stdexcept>
#include <typeinfo>
#include <iostream>
/// class to manage an instance of an object as a singleton
template <typename T>
class DynamicSingleton
{
  public:
    static T* ptr();
    static T& instance();

  private:
    static void DestroySingleton();
    DynamicSingleton();

    static T* pInstance;
};

template <typename T>
struct CreateUsingNew
{
  static T* Create(){return new T;}
  static void Destroy(T* p){delete p;}
};

template <typename T>
inline T* DynamicSingleton<T>::ptr()
{
  if (!pInstance) pInstance = CreateUsingNew<T>::Create();
  return pInstance;
};

template <typename T>
inline T& DynamicSingleton<T>::instance()
{
  if (!pInstance) pInstance = CreateUsingNew<T>::Create();
  return *pInstance;
};

template <typename T>
void DynamicSingleton<T>::DestroySingleton()
{
  CreateUsingNew<T>::Destroy(pInstance);
  pInstance = 0;
}

template <typename T>
T* DynamicSingleton<T>::pInstance = 0;

#endif
