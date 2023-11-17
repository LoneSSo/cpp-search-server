#pragma once

#include "document.h"

#include <iostream>
#include <iterator>
#include <vector>

template <typename Iterator>
class IteratorRange {
public:
    explicit IteratorRange(Iterator begin, Iterator end)
        : begin_(begin)
        , end_(end)
    {
    }

    explicit IteratorRange() = default;
    auto begin(){
        return begin_;
    }
    auto end(){
        return end_;
    }
    int size(){
        return distance(begin_, end_);
    }
private:
    Iterator begin_ = 0;
    Iterator end_ = 0;
};

template<typename It>
class Paginator {
public:

    Paginator(It left, It right, int page_size){
        using namespace std;
        int num_of_pages = distance(left, right) / page_size;
        for (int i = 0; i < num_of_pages; ++i){
            auto end = left + page_size;
            pages_.push_back(IteratorRange(left, end));
            left = end;
        }
        if(distance(left, right) != 0){
            pages_.push_back(IteratorRange(left, right));
        }
        
    }
        
    auto begin() const {
    using namespace std;
        return pages_.begin();
    }
    auto end() const {
    using namespace std;
        return pages_.end();
    }

private:
    std::vector<IteratorRange<It>> pages_;
};

template<typename It>
std::ostream& operator<<(std::ostream& out, IteratorRange<It> page){
using namespace std;
    for (It it = page.begin(); it < page.end(); ++it){
        out << *it;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, Document document){
    using namespace std;
    out << "{ "s;
    out << "document_id = "s << document.id << ", "s;
    out << "relevance = "s << document.relevance << ", "s;
    out << "rating = "s << document.rating;
    out << " }"s;
    return out;
}

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
using namespace std;
    return Paginator(begin(c), end(c), page_size);
}