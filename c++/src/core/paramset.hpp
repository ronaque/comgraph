/*!
 * This class implements an heterogeneous hash table.
 * @author Selan
 * @data 2025
 */

#ifndef PARAMSET_HPP
#define PARAMSET_HPP

#include <iostream>
#include <map>
#include <memory>
#include <string>
namespace gc
{
  /// This is the **base** class that represents a generic value.
  /*!
   * This is a pure virtual basic type.
   * The data part of the map inside ParamSet stores a shared pointer to this base class.
   */
  class GenericType
  {
  public:
    GenericType() = default;
    virtual ~GenericType() = default;
  };

  /// This is a **derived** class that actually stores a template value.
  /*!
   * This class is derived from `GenericType`.
   * It works as a wrap around a template value.
   * We rely on polymorphism to cast a `GenericType` pointer into
   * a `ValueType<T>` object that holds the `T` value the client wants.
   * This way, we may have an heterogeneous map.
   */
  template <typename T>
  class ValueType : public GenericType
  {
  public:
    T m_value; // The stored data.
    ValueType(const T &value = T{}) : m_value{value} {}
    ~ValueType() override = default;
    // T value() { return m_value; }
  };

  /// This class represents a special type of map for key-value items.
  /*! The `key` in the ParamSet class  is always a string.
   *  The `value` may be of different data types.
   *  The ParamSet is just a heterogeneous hash table. All keys are strings.
   */
  class ParamSet
  {
    using Dictionary = std::map<std::string, std::shared_ptr<GenericType>>;
    Dictionary m_map; //!< This is where the key-value itens are stored.

  public:
    ParamSet() = default;
    ~ParamSet() = default;

    /*!
     * Tries to retrieve the value associated with `key`.
     * In case there is no such key/value pair in the internal map, the method returns
     * the default value `default_value`.
     * @param key The key stored in the ParamSet.
     * @param default_value The default value returned, in case key is not in the ParamSet.
     * @return The value associated with `key`, if such key is stored in the `ps`, or
     * the given default value otherwise.
     */
    template <typename T>
    T retrieve(const std::string &key, const T &default_value = T{}) const
    {
      // Try to retrieve key/data item from the map.
      auto result = m_map.find(key);
      if (result != m_map.end())
      {
      }
      else
      {
        return default_value;
      }
      // Get key-value pair from iterator.
      const auto &[the_key, sptr] = *result;
      // Convert generic pointer to the base value of type T.
      auto data = dynamic_cast<ValueType<T> &>(*sptr);
      return data.m_value; // Finally, return the stored value.
    }

    /*!
     * Assigns a pair key-value to the ParamSet.
     * If the key already exists, the stored value is overwritten by the new data value provided.
     * @param key The key associated with the `value`.
     * @param value The value the client wants to store.
     */
    template <typename T>
    void assign(const std::string &key, const T &value)
    {
      m_map[key] = std::make_shared<ValueType<T>>(value);
    }
  };

} // namespace gc

#endif
