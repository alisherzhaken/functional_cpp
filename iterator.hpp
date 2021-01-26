
#ifndef RANDOM_ACCESS_ITERATOR_HPP
#define RANDOM_ACCESS_ITERATOR_HPP

// VERSION 1.0

namespace functional_cpp {

    template<class T>
    class RandomAccessIterator {
        private:
            using value_type = T;
            using reference = value_type&;
            using pointer = value_type*;
        public:
            RandomAccessIterator(pointer data) : m_data{data} {}
            RandomAccessIterator(const RandomAccessIterator& iterator) : m_data{iterator.m_data} {}
            bool operator==(const RandomAccessIterator& iterator) const {
                return this->m_data == iterator.m_data;
            }
            bool operator!=(const RandomAccessIterator& iterator) const {
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
        private:
            pointer m_data;
    };

}

#endif