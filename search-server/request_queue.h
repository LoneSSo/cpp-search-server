#pragma once

#include "search_server.h"

#include <deque>
#include <string>
#include <vector>

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;
    
private:
    struct QueryResult {
        int current_empty_requests = 0;
        bool is_this_empty = 0;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& server;
};

//====================================================//

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    using namespace std;
    vector<Document> documents = server.FindTopDocuments(raw_query, document_predicate);
    bool is_empty = documents.empty();
    if (requests_.empty()){
         requests_.push_back({0 + is_empty, is_empty});
    } else
    if (requests_.size() < min_in_day_){
         int prev_emptys = requests_.back().current_empty_requests;
         requests_.push_back({prev_emptys + is_empty, is_empty});
    } else {
        int prev_emptys = requests_.back().current_empty_requests;
        requests_.push_back({prev_emptys 
                            - requests_.front().is_this_empty
                            + is_empty, is_empty});
        requests_.pop_front();
    }

        return documents;

    }