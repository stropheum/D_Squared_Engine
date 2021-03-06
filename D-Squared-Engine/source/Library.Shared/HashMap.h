#pragma once
#include <cstdint>
#include <utility>
#include "Vector.h"
#include "DefaultHash.h"


namespace Library
{
    template <typename TKey, typename TValue, class HashFunctor = DefaultHash<TKey>> class HashMap
    {
        // The key/value pairs being used by the HashMap
        typedef std::pair<const TKey, TValue> PairType;

        // The buckets being used in the Vector chain. This implementation uses Vectors of PairTypes
        typedef Library::Vector<PairType> BucketType;

        static const uint32_t defaultBucketCount = 13; // If no Size is specified, number of buckets defaults to 13

    public:

        class Iterator;

#pragma region Construction/Copy/Assignment

        explicit HashMap(std::uint32_t hashMapSize = defaultBucketCount);

        virtual ~HashMap() = default;

        HashMap(const HashMap<TKey, TValue, HashFunctor>& rhs);

        HashMap<TKey, TValue, HashFunctor>& operator=(const HashMap<TKey, TValue, HashFunctor>& rhs);

        HashMap(HashMap<TKey, TValue, HashFunctor>&& rhs) noexcept;

        HashMap<TKey, TValue, HashFunctor>& operator=(HashMap<TKey, TValue, HashFunctor>&& rhs);

#pragma endregion

        /**
         * Finds an instance of a key in the HashMap and returns its associated value
         * @Param rhs: constant reference to the key being searched for
         * @Return: An Iterator containing the found PairType, end otherwise
         * @Exception: Thrown if bucket chain is null. Should never be reached
         */
        Iterator Find(const TKey& key) const;

        /**
         * Inserts a Pairtype into the HashMap
         * @Param entry: The PairType being added to the HashMap
         * @Return: An Iterator pointing to the inserted PairType
         */
        Iterator Insert(const PairType& entry);

        /**
         * Inserts a Pairtype into the HashMap
         * @Param entry: The PairType being added to the HashMap
         * @Param found: Output parameter to determine if the entry was found
         * @Return: An Iterator pointing to the inserted PairType
         */
        Iterator Insert(const PairType& entry, bool& found);

        /**
         * Removes a PairType from the HashMap, if it exists. Otherwise does nothing
         * @Param key: The key being used to Search for the item in the HashMap
         */
        void Remove(const TKey& key);

        /**
         * Index-of operator
         * @Param key: Key being used as an index for the HashMap element
         * @Return: A reference to the data associated with the key
         */
        TValue& operator[](const TKey& key);

        /**
         * Const Index-of operator
         * @Param key: Key being used as an index for the HashMap element
         * @Return: A reference to the data associated with the key
         */
        const TValue& operator[](const TKey& key) const;

        /**
         * Points to the first data point in the HashMap
         * @Return: An iterator pointing to the first piece of data in the HashMap
         */
        Iterator begin() const;

        /**
         * Points to the end node in the last bucket in the HashMap
         * @Return: An iterator pointing to the end of the last bucket in the HashMap
         */
        Iterator end() const;

        /**
         * Clears all memory stored in the HashMap
         */
        void Clear();

        /**
         * The number of elements inserted into the HashMap
         * @Return: The number of elements that have been inserted into the HashMap
         */
        std::uint32_t Size() const;

        /**
         * Determines if a value has been inserted into the HashMap with the specified key
         * @Param key: The key being searched for
         * @Return: True if the key exists in the HashMap
         */
        bool ContainsKey(const TKey& key) const;

    private:

        std::uint32_t m_bucketCount; // Number of buckets in the hash map.
        HashFunctor m_hashFunctor; // The hash function being used by this HashMap
        Library::Vector<BucketType> m_buckets; // Collection of buckets in the hash map
        std::uint32_t m_size;

        /**
         * Creates one vector of Type TData for each bucket in the specified Size
         */
        void InitializeBuckets();

    public:

        class Iterator
        {
            friend class HashMap;

        public:

            /**
             * Iterator constructor. Creates an Iterator pointing to nothing. Used for lazy initialization
             */
            Iterator();

            /**
             * Iterator constructor. Creates an Iterator pointing to nothing. Used for lazy initialization
             */
            virtual ~Iterator() = default;

            /**
             * Copy Constructor. Creates an iterator equivalent to the current Iterator
             * @Param rhs: The Iterator being copied
             */
            Iterator(const Iterator& rhs);

            /**
             * Assignment Operator
             * @Param rhs: The Iterator being assigned to
             * @Return: A reference to the Iterator copy
             * @Exception: Thrown if right hand side owner is null. Should never be reached
             */
            Iterator& operator=(const Iterator& rhs);

            /**
             * Increment operator(prefix)
             * @Return: A reference to the next Iterator in the HashMap
             */
            Iterator& operator++();

            /**
             * Increment operator(postfix)
             * @Return: A copy of the Iterator before it has been incremented
             */
            Iterator operator++(int);

            /**
             * Dereference operator
             * @Return: A reference to a PairType that the Iterator's member iterator is pointing to
             * @Exception: Thrown if attempting to dereferencing the end of the HashMap
             */
            PairType& operator*();

            /**
             * Dereference Operator
             * @Return: A reference to a PairType that the Iterator's member iterator is pointing to
             * @Exception: Thrown if attempting to dereference the end of the Hashmap
             */
            const PairType& operator*() const;

            /**
             * Arrow operator
             * @Return: A pointer to a PairType that the Iterator's member iterator is pointing to
             * @Exception: Thrown if attempting to dereference the end of the HashMap
             */
            PairType* operator->();

            /**
             * Arrow operator
             * @Return: A pointer to a PairType that the Iterator's member iterator is pointing to
             * @Exception: Thrown if attempting to dereference the end of the HashMap
             */
            const PairType* operator->() const;

            /**
             * Equality operator
             * @Param rhs: The Iterator being compared against
             * @return: True if the two Iterators are equivalent
             * @Exception: Thrown if owner is null
             */
            bool operator==(const Iterator& rhs) const;

            /**
             * Not-Equal operator
             * @Param rhs: The Iterator being compared against
             * @Return: False if the two Iterators are equivalent
             * @Exception: Thrown if owner is null
             */
            bool operator!=(const Iterator& rhs) const;

        private:

            /**
             * Private constructor. Used for traversing the chained vectors
             * @Param owner: The HashMap that owns this iterator
             * @Param bucketIndex: The index into the Vector chain
             * @Param iter: The iterator that mIter is being assigned to
             */
            Iterator(const HashMap<TKey, TValue, HashFunctor>* owner, const std::uint32_t& bucketIndex, typename Library::Vector<PairType>::Iterator iter);

            const HashMap<TKey, TValue, HashFunctor>* m_owner;	// The HashMap that owns this Iterator
            std::uint32_t m_bucketIndex;							// The index into the Vector chain
            typename Library::Vector<PairType>::Iterator m_iter;	// The Iterator on the Vector of PairTypes that is currently being iterated on

        };
    };
}

#include "HashMap.inl"