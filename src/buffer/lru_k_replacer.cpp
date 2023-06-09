//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_k_replacer.cpp
//
// Identification: src/buffer/lru_k_replacer.cpp
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_k_replacer.h"
#include <iostream>
namespace bustub {

LruNode::LruNode(frame_id_t key):key(key)
{
}

LruNode::~LruNode()
{
}


LRUKReplacer::LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(num_frames), k_(k) {

}

auto LRUKReplacer::Evict(frame_id_t *frame_id) -> bool { 
    if(buf_mp_.find(*frame_id) != buf_mp_.end()){
        auto lnode = buf_mp_[*frame_id];
        DelNode(lnode);
        buf_mp_.erase(buf_mp_.find(*frame_id));
        return true;

    }
    return false; 
}

void LRUKReplacer::RecordAccess(frame_id_t frame_id) {
    // 节点存在buf 
    if(buf_mp_.find(frame_id) != buf_mp_.end()){
        LruNode* tmp = buf_mp_[frame_id];
        BufHeadNode(tmp);
    } 
    // 节点存在his
    else if(his_mp_.find(frame_id) != buf_mp_.end()){
        LruNode* tmp = his_mp_[frame_id];
        tmp->count++;
        if(tmp->count >= k_){
            tmp = DelNode(tmp);
            his_mp_.erase(his_mp_.find(frame_id));
            if(buf_mp_.size() >= replacer_size_){
                BufDelNode();
            }
            AddNode(buf_list_,tmp);
            buf_mp_[frame_id] = tmp;
        }
    }
    // 节点不在his中
    else{
        LruNode* tmp = new LruNode(frame_id);
        if(his_mp_.size() >= replacer_size_){
            HisDelNode();
        }
        std::cout << "his_list_ is " << his_list_ <<std::endl; 
        his_mp_[frame_id] = tmp;
        AddNode(his_list_,tmp);
    }


}

void LRUKReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable) {
    his_mp_[frame_id]->set_enable_evict(set_evictable);
}

void LRUKReplacer::Remove(frame_id_t frame_id) {
    if(his_mp_.find(frame_id) != his_mp_.end()){
        auto lnode = his_mp_[frame_id];
        DelNode(lnode);
        his_mp_.erase(his_mp_.find(frame_id));
    }
    return;
}

auto LRUKReplacer::Size() -> size_t { 
    int count=0;
    for(auto e : his_mp_){
        std::cout << "e.second is " << e.second << std::endl;
        if(e.second->enable_evict)  count++;
    }
    return count; 

}
auto LRUKReplacer::HisDelNode()->bool{
    LruNode* tmp =  DelNode(his_list_->prev) ;
    if(tmp) {
        delete tmp;
        return true;
    }
    return false;
}
auto LRUKReplacer::BufDelNode()->bool{
    LruNode* tmp =  DelNode(buf_list_->prev) ;
    if(tmp) {
        delete tmp;
        return true;
    }
    return false;
}

auto LRUKReplacer::BufHeadNode(LruNode* buf_node) -> bool{
    LruNode* tmp = DelNode(buf_node);
    AddNode(buf_list_, tmp);
    return true;
}


auto LRUKReplacer::DelNode(LruNode* node) -> LruNode*{
    LruNode* up = node->prev;
    up->next = node->next;
    node->next->prev = up;
    return node;
}

auto LRUKReplacer::AddNode(LruNode*& head, LruNode* node) -> bool{
    // std::cout << "head is " << head <<std::endl; 
    // std::cout << "node is " << node <<std::endl; 
    if(!head) {
        head = node;
        head->prev = head;
        head->next = head;
        return true;
    }
    if(head->next == head){
        head->prev = node;
        head->next = node;
        node->prev = head;
        node->next = head;
        head = node;
        return true;
    }

    (head->prev)->next = node;
    node->next = head;
    node->prev = head->prev;
    head->prev = node;
    head = node;
    return true;
}

}  // namespace bustub
