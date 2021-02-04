#include <memory>
#include <stdexcept>
#include <functional>

#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

// VERSION 1.1


namespace functional_cpp {

    template<class T, class Allocator = std::allocator<T>>
    class array_list {
        public:
            using value_type = T;
            using reference = value_type&;
            using pointer = value_type*;
            using const_reference = const value_type&;
            using iterator = RandomAccessIterator<value_type>;
            using const_iterator = RandomAccessIterator<const value_type>;
            using reverse_iterator = ReverseRandomAccessIterator<value_type>;
            using const_reverse_iterator = ReverseRandomAccessIterator<const value_type>;
            using allocator = Allocator;
        public:

            constexpr array_list() : m_data{m_allocator.allocate(5)}, m_size{0}, m_capacity{5} {}
            
            constexpr array_list(std::size_t capacity) : m_data{m_allocator.allocate(capacity)}, 
                                                        m_size{0}, 
                                                        m_capacity{capacity} {}
            
            constexpr array_list(std::initializer_list<value_type> collection) : m_data{m_allocator.allocate(collection.size())},
                                                                                m_size{collection.size()},
                                                                                m_capacity{collection.size()} {
                for (auto it {collection.begin()}; it != collection.end(); ++it ) {
                    m_data[std::distance(collection.begin(), it)] = std::move(*it);
                }
            }
            
            constexpr array_list(const array_list& other) : m_data{m_allocator.allocate(other.size())},
                                                            m_size{other.size()},
                                                            m_capacity{other.size()} {
                for (std::size_t i{0}; i < m_size; ++i) {
                    m_data[i] = other.m_data[i];
                }
            }
            
            constexpr array_list(array_list&& other) : m_data{std::exchange(other.m_data, nullptr)},
                                                    m_size{std::exchange(other.m_size, 0)},
                                                    m_capacity{std::exchange(other.m_capacity, 0)} {}
            
            template<class InputIterator>
            constexpr array_list(InputIterator first, InputIterator last) : m_data{m_allocator.allocate(std::distance(first, last))},
                                                                            m_size{std::distance(first, last)},
                                                                            m_capacity{std::distance(first, last)} {
                for (std::size_t i{0}; first != last; ++first, ++i) {
                    m_data[i] = *first;
                }
            }
            ~array_list() {
                if (m_data != nullptr) {
                    if (std::is_destructible_v<value_type>) {
                        for (std::size_t i{0}; i < m_size; ++i) {
                            m_data[i].~value_type();
                        }
                    }
                    m_allocator.deallocate(m_data, m_capacity);
                }
            }
            constexpr reference operator[](std::size_t index) {
                return index < m_size ? m_data[index] : throw std::out_of_range("Index out of bounds");
            }

            constexpr pointer data() const {
                return m_data;
            }

            constexpr iterator begin() {
                return iterator{m_data};
            }
            constexpr iterator end() {
                return iterator{m_data + m_size};
            }
            constexpr const_iterator cbegin() const {
                return const_iterator{m_data};
            }
            constexpr const_iterator cend() const {
                return const_iterator{m_data + m_size};
            }
            
            constexpr reverse_iterator rbegin() {
                return reverse_iterator{m_data + m_size - 1};
            }

            constexpr reverse_iterator rend() {
                return reverse_iterator{m_data - 1};
            }

            constexpr const_reverse_iterator crbegin() const {
                return const_reverse_iterator{m_data + m_size - 1};
            }

            constexpr const_reverse_iterator crend() const {
                return const_reverse_iterator{m_data - 1};
            }
            
            constexpr std::size_t size() const {
                return m_size;
            }
            
            constexpr std::size_t capacity() const {
                return m_capacity;
            }
            
            constexpr void add(value_type value) {
                if (m_size < m_capacity) {
                    m_data[m_size++] = value;
                }
                else {
                    pointer new_data { m_allocator.allocate(m_capacity * 2) };

                    for (std::size_t i {0}; i < m_size; ++i) {
                        new_data[i] = m_data[i];
                    }
                    new_data[m_size++] = value;
                    
                    if (std::is_destructible_v<value_type>) {
                        for (std::size_t i {0} ; i < m_size; ++i) {
                            m_data[i].~value_type();
                        }
                    }
                    
                    m_allocator.deallocate(m_data, m_capacity);
                    m_data = new_data;
                    m_capacity = m_capacity * 2;
                }
            }


            constexpr void for_each(std::function<void(value_type)> function) {
                for (std::size_t i{0}; i < m_size; ++i) {
                    function(m_data[i]);
                }
            }

            constexpr array_list<value_type> map(std::function<value_type(value_type)> unary_op) {
                array_list<value_type> mapped_list(m_size);
                for (std::size_t i{0}; i < m_size; ++i) {
                    mapped_list.add(unary_op(this->m_data[i]));
                }
                return mapped_list;
            }

            template<class OutputIterator>
            constexpr void map(OutputIterator outputIterator,  std::function<value_type(value_type)> unary_op) {
                for (auto it { cbegin() }; it != cend(); ++it) {
                    *outputIterator++ = unary_op(*it);
                }
            }

            constexpr void shrink() {
                if (m_size < m_capacity) {
                    pointer new_data { m_allocator.allocate(m_size) };
                    for (std::size_t i{0}; i < m_size; ++i) {
                        new_data[i] = m_data[i];
                    }
                    m_allocator.deallocate(m_data, m_capacity);
                    m_data = new_data;
                    m_capacity = m_size;
                }
            }

        private:
            allocator m_allocator{};
            pointer m_data;
            std::size_t m_size;
            std::size_t m_capacity;
    };

}

#endif