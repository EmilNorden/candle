#ifndef WEAK_CACHE_H_
#define WEAK_CACHE_H_

#include <map>
#include <memory>
#include <string>

template <typename T>
class WeakCache
{
public:
	std::shared_ptr<T> get(const std::string &key);
	void insert(const std::string &key, const std::shared_ptr<T> &value);
private:
	std::map<std::string, std::weak_ptr<T>> m_cache;
};

template <typename T>
std::shared_ptr<T> WeakCache<T>::get(const std::string &key)
{
	auto it = m_cache.find(key);
	if(it == m_cache.end())
		return std::shared_ptr<T>();

	auto weak_ref = it->second;
	auto shared_ref = weak_ref.lock();
	if(!shared_ref)
		m_cache.erase(key);

	return shared_ref;
}

template <typename T>
void WeakCache<T>::insert(const std::string &key, const std::shared_ptr<T> &value)
{
	m_cache.emplace(key, std::weak_ptr<T>(value));
}


#endif