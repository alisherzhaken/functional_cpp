#include <memory>
#include <cassert>

#ifndef ARRAY_HPP
#define ARRAY_HPP

// VERSION 1.0

namespace functional_cpp {
    template<class T>
    class array {
        using value_type = T;
        using size_type = std::size_t;
        using pointer = std::unique_ptr<value_type[]>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = value_type*;
        public:
            array(std::size_t size) : m_data{std::make_unique<value_type[]>(size)}, m_size{size} {}
            array(std::initializer_list<T> collection) : m_data{std::make_unique<value_type[]>(collection.size())}, 
                                                         m_size{collection.size()} {
                for (auto it {collection.begin()}; it != collection.end(); ++it) {
                    m_data[it - collection.begin()] = *it;
                }
            }
            ~array() = default;
            reference operator[](size_type index) const {
                assert(index >= 0 && index < m_size);
                return m_data[index];
            }
            pointer data() const {
                return m_data.get();
            }
            iterator begin() {
                return m_data.get();
            }
            iterator end() {
                return m_data.get() + m_size;
            }
            std::size_t size() const {
                return m_size;
            }
        private:
            pointer m_data;
            std::size_t m_size;
    };
}

#endif