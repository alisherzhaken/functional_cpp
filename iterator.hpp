
#ifndef RANDOM_ACCESS_ITERATOR_HPP
#define RANDOM_ACCESS_ITERATOR_HPP

// VERSION 2.1

namespace functional_cpp {

    template<class T>
    class RandomAccessIterator {
        public:
            using value_type = T;
            using reference = value_type&;
            using pointer = value_type*;
            using const_reference = const value_type&;
            using const_pointer = const value_type*;
            using difference_type = std::ptrdiff_t;
        public:
            constexpr RandomAccessIterator(pointer data) : m_data{data} {}
            constexpr RandomAccessIterator(const RandomAccessIterator& iterator) : m_data{iterator.m_data} {}
            constexpr bool operator==(const RandomAccessIterator& iterator) const {
                return this->m_data == iterator.m_data;
            }
            constexpr bool operator!=(const RandomAccessIterator& iterator) const {
                return !(*this == iterator);
            }
            RandomAccessIterator& operator++() {
                ++m_data;
                return *this;
            }
            RandomAccessIterator operator++(int) {
                auto copy = *this;
                ++m_data;
                return copy;
            }
            reference operator*() {
                return *m_data;
            }
            pointer operator->() {
                return m_data;
            }
            const_reference operator*() const {
                return *m_data;
            }
            const_pointer operator->() const {
                return m_data;
            }
        private:
            pointer m_data;
    };

    template<class T>
    class ReverseRandomAccessIterator {
        public:
            using value_type = T;
            using reference = value_type&;
            using pointer = value_type*;
            using const_reference = const value_type&;
            using const_pointer = const value_type*;
        public:
            ReverseRandomAccessIterator(pointer data) : m_data{data} {}
            ReverseRandomAccessIterator(const ReverseRandomAccessIterator& iterator) : m_data{iterator.m_data} {}
            bool operator==(const ReverseRandomAccessIterator& iterator) const {
                return this->m_data == iterator.m_data;
            }
            bool operator!=(const ReverseRandomAccessIterator& iterator) const {
                return !(*this == iterator);
            }
            ReverseRandomAccessIterator& operator++() {
                --m_data;
                return *this;
            }
            ReverseRandomAccessIterator operator++(int) {
                auto copy = *this;
                --m_data;
                return copy;
            }
            reference operator*() {
                return *m_data;
            }
            pointer operator->() {
                return m_data;
            }
            const_reference operator*() const {
                return *m_data;
            }
            const_pointer operator->() const {
                return m_data;
            }
        private:
            pointer m_data;
    };

}

#endif