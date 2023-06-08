//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// extendible_hash_table.cpp
//
// Identification: src/container/hash/extendible_hash_table.cpp
//
// Copyright (c) 2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "container/hash/extendible_hash_table.h"
#include <algorithm>  // std::max
#include <cassert>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <list>
#include <utility>
#include "storage/page/page.h"

namespace bustub {

template <typename K, typename V>
ExtendibleHashTable<K, V>::ExtendibleHashTable(size_t bucket_size) : bucket_size_(bucket_size) {
  dir_.resize(num_buckets_);
  for (int i = 0; i < num_buckets_; i++) {
    std::shared_ptr<Bucket> init_bucket(new Bucket(bucket_size_, global_depth_));
    dir_[i] = init_bucket;
  }
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::IndexOf(const K &key) -> size_t {
  int mask = (1 << global_depth_) - 1;
  return std::hash<K>()(key) & mask;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetGlobalDepth() const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetGlobalDepthInternal();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetGlobalDepthInternal() const -> int {
  return global_depth_;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetLocalDepth(int dir_index) const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetLocalDepthInternal(dir_index);
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetLocalDepthInternal(int dir_index) const -> int {
  return dir_[dir_index]->GetDepth();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetNumBuckets() const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetNumBucketsInternal();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetNumBucketsInternal() const -> int {
  return num_buckets_;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Find(const K &key, V &value) -> bool {
  // std::scoped_lock<std::mutex> lock(latch_);
  std::scoped_lock<std::mutex> lock(latch_);
  auto hidx = IndexOf(key);
  auto bk_node = dir_[hidx];
  return bk_node->Find(key, value);
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Remove(const K &key) -> bool {
  // std::scoped_lock<std::mutex> lock(latch_);
  std::scoped_lock<std::mutex> lock(latch_);
  auto hidx = IndexOf(key);
  auto bk_node = dir_[hidx];
  return bk_node->Remove(key);
}

template <typename K, typename V>
void ExtendibleHashTable<K, V>::Insert(const K &key, const V &value) {
  std::scoped_lock<std::mutex> lock(latch_);
  // std::scoped_lock<std::mutex> lock(latch_);
  // std::scoped_lock<std::mutex> lock(latch_);
  // GetGlobalDepth();
  auto hidx = IndexOf(key);
  auto bk_node = dir_[hidx];

  if (bk_node->Insert(key, value)) {
    // int len = dir_.size();
    // for(int i =0 ;i < len;i++){
    //     auto bt = dir_[i]->GetItems();
    //     for(std::pair<K,V>& kv :bt){

    //         // std::cout << "bt is " << i << ", k is " << kv.first
    //         // << ", v is " << kv.second << std::endl;
    //         std::cout << "bt is  " << i << " k is " << kv.first << ", v is "<< kv.second << std::endl;

    //     }
    // }
    // std::cout << "====================" << std::endl;
    // lock.unlock();
    return;
  }
  // latch_.unlock();
  // latch_.lock();
  if (global_depth_ == bk_node->GetDepth()) {
    int oldlen = dir_.size();
    dir_.resize(oldlen * 2);
    for (int i = 0; i < oldlen; i++) {
      dir_[i + oldlen] = dir_[i];
    }
    global_depth_++;
  }

  if (global_depth_ > bk_node->GetDepth()) {
    auto local_dep = bk_node->GetDepth();
    auto rhidx = hidx ^ 1 << local_dep;
    auto hhidx = std::max(hidx, rhidx);
    std::shared_ptr<Bucket> new_bucket(new Bucket(bucket_size_, local_dep + 1));
    auto olditem = bk_node->GetItems();
    for (auto it = olditem.begin(); it != olditem.end(); it++) {
      if (IndexOf(it->first) & 1 << local_dep) {
        new_bucket->Insert(it->first, it->second);
        bk_node->Remove(it->first);
      }
    }
    dir_[hhidx] = new_bucket;
    num_buckets_++;
    bk_node->IncrementDepth();
    auto hidx = IndexOf(key);
    auto bk_node = dir_[hidx];
    if (bk_node->Insert(key, value)) {
      return;
    }
  }
  // lock.unlock();
}

//===--------------------------------------------------------------------===//
// Bucket
//===--------------------------------------------------------------------===//
template <typename K, typename V>
ExtendibleHashTable<K, V>::Bucket::Bucket(size_t array_size, int depth) : size_(array_size), depth_(depth) {}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Find(const K &key, V &value) -> bool {
  for (auto &e : list_) {
    if (e.first == key) {
      value = e.second;
      return true;
    }
  }
  return false;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Remove(const K &key) -> bool {
  if (!list_.empty()) {
    for (auto it = list_.begin(); it != list_.end(); it++) {
      if (it->first == key) {
        it = list_.erase(it);
        return true;
      }
    }
  }

  return false;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Insert(const K &key, const V &value) -> bool {
  for (auto &e : list_) {
    if (e.first == key) {
      e.second = value;
      return true;
    }
  }

  if (!IsFull()) {
    list_.insert(list_.end(), std::pair<K, V>(key, value));
    return true;
  }
  return false;
}

template class ExtendibleHashTable<page_id_t, Page *>;
// template class ExtendibleHashTable<Page *, std::list<Page *>::iterator>;
template class ExtendibleHashTable<int, int>;
// test purpose
template class ExtendibleHashTable<int, std::string>;
// template class ExtendibleHashTable<int, std::list<int>::iterator>;

}  // namespace bustub
