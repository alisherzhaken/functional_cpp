#include <memory>
#include <stdexcept>
#include <functional>

#include "iterator.hpp"

#ifndef ARRAY_HPP
#define ARRAY_HPP

// VERSION 2.4

namespace functional_cpp {
    template<class T>
    class array {
        public:
            using value_type = T;
            using size_type = std::size_t;
            using pointer = std::unique_ptr<value_type[]>;
            using raw_pointer = value_type*;
            using reference = value_type&;
            using const_reference = const value_type&;
            using iterator = RandomAccessIterator<value_type>;
            using const_iterator = RandomAccessIterator<const value_type>;
            using reverse_iterator = ReverseRandomAccessIterator<value_type>;
            using const_reverse_iterator = ReverseRandomAccessIterator<const value_type>;

        public:
            constexpr array(std::size_t size) : m_data{std::make_unique<value_type[]>(size)}, 
                                                m_size{size} {}
            
            constexpr array(std::initializer_list<T> collection) : m_data{std::make_unique<value_type[]>(collection.size())}, 
                                                                   m_size{collection.size()} {
                for (auto it {collection.begin()}; it != collection.end(); ++it) {
                    m_data[std::distance(collection.begin(), it)] = *it;
                }
            }
            
            constexpr array(const array& other) : array{other.cbegin(), other.cend()} {}
            
            constexpr array(array&& other) noexcept : m_data{std::move(other.m_data)}, 
                                                      m_size{std::exchange(other.m_size, 0)} {}
            
            template<class InputIterator>
            constexpr array(InputIterator first, InputIterator last) : m_data{std::make_unique<value_type[]>(static_cast<std::size_t>(std::distance(first, last)))},
                                                                       m_size{static_cast<std::size_t>(std::distance(first,last))} {
                for (auto it { m_data.get() }; first != last; ++first, ++it) {
                    *it = *first;
                }
            }

            ~array() = default;
            
            constexpr reference operator[](size_type index) const {
                return index < m_size ? m_data[index] : throw std::out_of_range("Index out of bounds");
            }
            
            constexpr raw_pointer data() const {
                return m_data.get();
            }
            
            constexpr iterator begin() {
                return iterator{m_data.get()};
            }
            constexpr iterator end() {
                return iterator{m_data.get() + m_size};
            }
            constexpr const_iterator cbegin() const {
                return const_iterator{m_data.get()};
            }
            constexpr const_iterator cend() const {
                return const_iterator{m_data.get() + m_size};
            }
            
            constexpr reverse_iterator rbegin() {
                return reverse_iterator{m_data.get() + m_size - 1};
            }

            constexpr reverse_iterator rend() {
                return reverse_iterator{m_data.get() - 1};
            }

            constexpr const_reverse_iterator crbegin() const {
                return const_reverse_iterator{m_data.get() + m_size - 1};
            }

            constexpr const_reverse_iterator crend() const {
                return const_reverse_iterator{m_data.get() - 1};
            }
            
            constexpr std::size_t size() const {
                return m_size;
            }
            
            constexpr reference front() const {
                return m_data[0];
            }
            constexpr reference back() const {
                return m_data[m_size-1];
            }
            
            constexpr void for_each(std::function<void(value_type)> function) {
                for (std::size_t i{0}; i < m_size; ++i) {
                    function(m_data[i]);
                }
            }
            constexpr array<value_type> map(std::function<value_type(value_type)> unary_op) {
                array<value_type> mapped_array(m_size);
                for (std::size_t i{0}; i < m_size; ++i) {
                    mapped_array[i] = unary_op(this->m_data[i]);
                }
                return mapped_array;
            }
            
            template<class OutputIterator>
            constexpr void map(OutputIterator outputIterator,  std::function<value_type(value_type)> unary_op) {
                for (auto it { cbegin() }; it != cend(); ++it) {
                    *outputIterator++ = unary_op(*it);
                }
            }

            template<class OutputIterator>
            constexpr void map(OutputIterator first, OutputIterator last, std::function<value_type(value_type)> unary_op) {
                auto distance { std::distance(cbegin(), cend()) < std::distance(first, last) 
                              ? std::distance(cbegin(), cend()) : std::distance(first, last) };
                for (auto it { cbegin() }; it != cbegin() + distance; ++it) {
                    *first++ = unary_op(*it);
                }
            }


            constexpr array<value_type> filter(std::function<bool(value_type)> predicate) {
                raw_pointer filtered_data { static_cast<raw_pointer>(std::malloc(m_size * sizeof(value_type))) };
                std::size_t new_size { 0 };
                for (auto it { begin() }; it != end(); ++it) {
                    if (predicate(*it)) {
                        filtered_data[new_size++] = *it;
                    }
                }
                array<value_type> filtered_array(filtered_data, filtered_data + new_size);
                std::free(filtered_data);
                return filtered_array;
            }

        private:
            pointer m_data;
            std::size_t m_size;
    };
}

#endif