#ifndef __TIMELINEQUEUE_H__
#define __TIMELINEQUEUE_H__

typedef unsigned long priority_t;
typedef unsigned long entry_t;

#include <Arduino.h>

/*
TimelineQueue is a priority queue that works in reverse order (lowest number is highest priority).
To avoid memory allocations, it stores a linked-list in a non-sorted array, linked according to order (lowest to highest.)
While the array is used to keep it memory-constant, the linked-list aspect is used to make it easy to always
find the lowest priority item. This is ideal for situations where we only care about the lowest priority item
and not the rest of the list.
*/
namespace PropMechanic {
    template <class T, int LENGTH> class TimelineQueue
    {
    private:
        const int NO_NEXT = -1;


        struct Entry {
            priority_t priority;
            entry_t id;
            T entry;
            int nextIndex;
        };

        int headIndex;
        int count;
        unsigned long entryIdCounter;
        Entry contents[LENGTH];
    
        int findEmptyIndex() {
            for (int i = 0; i < LENGTH; i++)
            {
                if (contents[i].id == 0 && contents[i].entry == NULL)
                {
                    return i;
                }
            }
            return -1;
        }

    public:
        TimelineQueue() {
            headIndex = 0;
            count = 0;
            entryIdCounter = 1UL;
        
            for (int i = 0; i < LENGTH; i++)
            {
                contents[i].nextIndex = NO_NEXT;
            }
        }
    
        entry_t push(T entry, priority_t itemPriority) {
            if (count >= LENGTH) {
                // ERROR
                return 0UL;
            }

            // Store the data in an empty slot
            int newIndex = findEmptyIndex();
            contents[newIndex].entry = entry;
            contents[newIndex].priority = itemPriority;
            contents[newIndex].id = entryIdCounter;
            entryIdCounter++;
            count++;
        
            // Figure out where we need to insert the new record
            if (contents[headIndex].priority > itemPriority)
            {
                contents[newIndex].nextIndex = headIndex;
                headIndex = newIndex;
            }
            else
            {
                int prevIndex = headIndex;
                int currIndex = contents[headIndex].nextIndex;
                for (int i = 0; i < LENGTH; i++) {
                    if (currIndex == NO_NEXT || contents[currIndex].priority > itemPriority)
                    {
                        contents[prevIndex].nextIndex = newIndex;
                        contents[newIndex].nextIndex = currIndex;
                        break;
                    }
                    prevIndex = currIndex;
                    currIndex = contents[currIndex].nextIndex;
                }
            }
        
            return entryIdCounter;
        }
    
    
        priority_t peakPriority() {
            return contents[headIndex].priority;
        }

    
        T pop() {
            if (count == 0)
            {
                return NULL;
            }
        
            T returnEntry = contents[headIndex].entry;
            contents[headIndex].entry = NULL;
            contents[headIndex].id = 0;
        
            headIndex = contents[headIndex].nextIndex;
            count--;

            return returnEntry;
        }


        int size() {
            return count;
        }
    
        boolean isEmpty() {
            return count == 0;
        }
    
        boolean isNotEmpty() {
            return !isEmpty();
        }
    };
}
#endif //__TIMELINEQUEUE_H__
