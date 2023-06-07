// #include "stdlib.h"   /* abort() */
// #include "xxhash.h"
// #include<cstring>
// #include<string>
// #include<iostream>

// #define INIT_ARR_N 2^4
// #define INIT_BIT_L 4
// #define ELEMENTS_N 4
// #define HKEY_LEN 64


// size_t get_more_data(void* buffer, size_t size, char* fh){

//     memcpy(buffer, fh, size);

//     return size;
// }

// XXH64_hash_t calcul_hash_streaming(char* fh)
// {
    
//     /* create a hash state */
//     XXH64_state_t* const state = XXH64_createState();
//     if (state==NULL) abort();

//     size_t const bufferSize = 128;
//     void* const buffer = malloc(bufferSize);
//     if (buffer==NULL) abort();

//     /* Initialize state with selected seed */
//     XXH64_hash_t const seed = 0;   /* or any other value */
//     if (XXH64_reset(state, seed) == XXH_ERROR) abort();

//     /* Feed the state with input data, any size, any number of times */
//     size_t i=0;
//     size_t slen = strlen(fh);
//     size_t tmplen = 0;
//     while (i < slen) {
//         if(i + bufferSize < slen)  
//             tmplen = bufferSize;
//         else 
//             tmplen = slen - i;
//         size_t const length = get_more_data(buffer, tmplen, fh + i );
//         if (XXH64_update(state, buffer, length) == XXH_ERROR) abort();
//         i = i + length;
//     }

//     /* Produce the final hash value */
//     XXH64_hash_t const hash = XXH64_digest(state);

//     /* State could be re-used; but in this example, it is simply freed  */
//     free(buffer);
//     XXH64_freeState(state);

//     return hash;
// }


// template<class valueType>
// class kvEle{
//     size_t key;
//     valueType value;
// };

// template<class valueType>
// class bucketNode{
//     std::vector<kvEle<valueType>> elements(ELEMENTS_N);
//     bool existFlag[ELEMENTS_N] = {0};
//     int localFlag = INIT_BIT_L;
// public:
//     bucketNode(){

//     };
//     bool put(size_t key,valueType value){

//         for (int i=0;i<ELEMENTS_N;i++){
//             if(!existFlag[i]){
//                 elements[i].key = key;
//                 elements[i].value = value;
//                 return true;
//             }
//         }
//         return false;
        
//     }

//     bool del(size_t key,valueType value){

//         for (int i=0;i<ELEMENTS_N;i++){
//             if(!existFlag[i] && elements[i].key == key){
//                 existFlag[i] = 0;
//                 return true;
//             }
//         }
//         return false;
        
//     }

//     kvEle* get(keyType key){
//         for (int i=0;i<ELEMENTS_N;i++){
//             if(existFlag[i] && elements[i].key == key){
//                 return elements[i];
//             }
//         }
//         return nullptr;
//     }

//     ~bucketNode(){

        
//     };
// };






// // template<class valueType>
// // class hashNode {
// //     size_t arrN;
// //     bucketNode<valueType>* bucketPointor;
// // };


// template<class keyType,class valueType>
// class ExtendibleHashTable {
// private:
    
//     std::vector<bucketNode<valueType>*> hashArr(INIT_ARR_N);
//     int globalFlag = INIT_BIT_L;
//     int arrN = INIT_ARR_N;
//     int bucketN = INIT_ARR_N;

// public:
//     ExtendibleHashTable(){

//         for (int i=0;i<INIT_ARR_N;i++){
//             hashArr[i] = new bucketNode<valueType>;
//         }

//     }

//     bool expand(size_t hidx){

//         bucketNode* oldBucket = hashArr[hidx];
//         if(oldBucket->localFlag = globalFlag){
//             hashArr.resize(arrN*2);
//             for(int i=arrN;i<arrN*2;i++){
//                 hashArr[i] = hashArr[i-arrN]; 
//             }
//             arrN = arrN*2;
//             globalFlag++;
//         }


//         if(oldBucket->localFlag < globalFlag){
//             bucketNode<valueType>* bkNode = oldBucket;

//             size_t localIdx = hidx ^ (1<<oldBucket->localFlag);
//             size_t lowBit = hidx > localIdx ?localIdx:hidx;
//             size_t highBit = hidx < localIdx ?localIdx:hidx;


//             bucketNode<valueType>* newBucket = new bucketNode<valueType>;
//             for(int i=0;i<ELEMENTS_N;i++){
//                 kvEle* kv = oldBucket->elements[i];
//                 if(kv->key & 1<< oldBucket->localFlag  ){
                    
//                     newBucket->put(kv->key, kv->value);
//                     oldBucket->del(kv->key, kv->value);


//                 }

//             }
//             oldBucket->localFlag++;
//             bucketN++;
//             return true;
//         }



//         return false;

//     }


//     bool Find(keyType key, valueType& value){
//         size_t hkey = calcul_hash_streaming(&key);
//         size_t hidx = hkey & (1 << (globalFlag+1)) -1 ;
//         bucketNode<valueType>* bkNode = hashArr[hidx];
//         kvEle* kv = bkNode.get(hkey);
//         if(kv){
//             value = kv->value;
//             return true;
//         }
//         return false;
//     }

//     bool Insert(keyType key, valueType value){
//         kvEle* kv = Find(key,value);
//         if(kv){
//             kv->value = value;
//             return true;
//         }
//         size_t hkey = calcul_hash_streaming(&key);
//         size_t hidx = hkey & (1 << (globalFlag+1)) -1 ;
//         bucketNode<valueType>* bkNode = hashArr[hidx];
//         if(bkNode.put(hkey)){
//             return true;
//         }

//         if(expand(hidx)){
//             return Insert(key,value);
//         }

//         return false;
//     }


//     bool Remove(keyType key){
//         size_t hkey = calcul_hash_streaming(&key);
//         size_t hidx = hkey & (1 << (globalFlag+1)) -1 ;
//         bucketNode<valueType>* bkNode = hashArr[hidx];
//         if(bkNode.del(hkey)){
//             return true;
//         }
//         return false;
//     }

//     int GetGlobalDepth(){
//         return globalFlag;
//     }

//     int GetLocalDepth(size_t dir_index){
//         return hashArr[dir_index]->localFlag;
//     }

//     int GetNumBuckets(){
//         return bucketN;
//     }

//     size_t IndexOf(keyType key){
//         return calcul_hash_streaming(&key);
//     }



// };


