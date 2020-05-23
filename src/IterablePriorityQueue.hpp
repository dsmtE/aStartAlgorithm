//sources: https://stackoverflow.com/questions/4484767/how-to-iterate-over-a-priority-queue
#pragma once

#include <vector> // std::vector
#include <functional> // std::less

template< class T, class Container = std::vector<T>, class Compare = std::less<typename Container::value_type>> 
struct IterablePriorityQueue : std::priority_queue<T, Container, Compare> {
    typename Container::iterator begin() {
        return std::priority_queue<T, Container, Compare>::c.begin();
    }
    typename Container::iterator end() {
        return std::priority_queue<T, Container, Compare>::c.end();
    }
};