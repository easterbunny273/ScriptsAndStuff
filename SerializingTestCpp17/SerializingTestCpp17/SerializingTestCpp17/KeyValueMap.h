#pragma once

#include <unordered_map>
#include <variant>
#include <string>
#include <type_traits>
#include <exception>

class KeyValueMap;

class KeyValueMap
{
public:
	using value_t = std::variant<bool, double, float, long int, int, unsigned int, char, unsigned char, std::string, KeyValueMap>;
	using key_t = std::string;

	template <class T> void set(key_t key, T value);
	bool has(key_t key) { return m_map.find(key) != m_map.end(); };
	template <class T> bool has_type(key_t key);

	template <class T> const T& get(key_t key);
	template <class T> const T* const get_if(key_t key);

private:

	std::unordered_map<key_t, value_t> m_map;
};

template <class T> void KeyValueMap::set(KeyValueMap::key_t key, T value)
{
	m_map[key] = value;
};

template <class T> bool KeyValueMap::has_type(key_t key)
{
	bool keyKnown = has(key);

	if (!keyKnown)
		return false;

	auto iter = m_map.find(key);

	return std::holds_alternative<T>(iter->second);
}

template <class T> const T& KeyValueMap::get(key_t key)
{
	bool keyKnown = has(key);

	if (!keyKnown)
		throw std::exception("KeyValueMap: key does not exist");

	auto iter = m_map.find(key);

	return std::get<T>(iter->second);
}

template <class T> const T* const KeyValueMap::get_if(key_t key)
{
	bool keyKnown = has(key);

	if (!keyKnown)
		return nullptr;

	auto iter = m_map.find(key);

	return std::get_if<T>(&(iter->second));
}