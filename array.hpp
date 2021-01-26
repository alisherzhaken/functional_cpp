#include <memory>
#include <cassert>

#ifndef ARRAY_HPP
#define ARRAY_HPP

// VERSION 1.0

namespace functional_cpp {
    template<class T, class Allocator = std::allocator<T>>
    class array {
        using value_type = T;
        using size_type = std::size_t;
        using pointer = value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = pointer;
        public:
            array(std::size_t size) : m_allocator{}, m_data{m_allocator.allocate(size)}, m_size{size} {}
            array(std::initializer_list<T> collection) : m_allocator{}, 
                                                         m_data{m_allocator.allocate(collection.size())}, 
                                                         m_size{collection.size()} {
                for (auto it {collection.begin()}; it != collection.end(); ++it) {
                    m_data[it - collection.begin()] = *it;
                }
            }
            ~array() {
                if (m_data != nullptr) {
                    m_allocator.deallocate(m_data, m_size);
                }
            }
            reference operator[](size_type index) const {
                assert(index >= 0 && index < m_size);
                return m_data[index];
            }
            pointer data() const {
                return m_data;
            }
            iterator begin() {
                return m_data;
            }
            iterator end() {
                return m_data + m_size;
            }
            std::size_t size() const {
                return m_size;
            }
        private:
            Allocator m_allocator;
            pointer m_data;
            std::size_t m_size;
    };
}

#endif