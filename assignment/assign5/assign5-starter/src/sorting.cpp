#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */

// 将list链表按奇偶分成两个链表
void split(ListNode *list, ListNode *&first, ListNode *&second){
    //current为进入本次循环时的当前结点，next为进入循环时当前结点的下一结点
    ListNode* current, *next;
    first = second = nullptr;
    if(list){ //如果list不为空链表
        first = list;
    }
    if(list->next){//如果list有不止一个元素
        second = list->next;
    }
    //current指向第一个元素，next指向第二个元素
    current = first, next = second;
    //在当前结点和下一结点都存在时
    while(current && next){
       current->next = next->next; //当前结点的下一结点改为第三个结点（或nullptr）
       current = current->next;//current指向第三个结点（或nullptr）
       if(next->next){ //如果第三个结点不为nullptr
           next->next = next->next->next;//next结点的下一个结点指向第四个结点（或nullptr）
       }
       next = next->next;//下一结点指向第四个结点（或nullptr）
    }
    if(current){ //next为nullptr但是current不为nullptr时
        current->next = nullptr;
    }
}

// 将first链表和second链表的元素按不递减顺序连接成新的链表，list指向链表的首元素
void merge(ListNode *&list, ListNode *first, ListNode *second){
   //first和second都至少有一个元素
    ListNode *current;
    if(first->data < second->data){
        current = first;
        first = first->next;
    }else{
        current = second;
        second = second->next;
    }
    list = current;
    //current为当前已完成merge链表的尾元素，first和second分别为未merge的两个链表的头指针
    while(first && second){//两个链表都不为空时
        if(first->data < second->data){
            current->next = first;
            first = first->next;
        }else{
            current->next = second;
            second = second->next;
        }
        current = current->next;
    }
    //退出循环时，必有某个链表已经merge完成，current指向该链表的最后一个元素
    //另一个链表还有一个或多个元素未merge
    while(first){
        current->next = first;
        first = first->next;
        current = current->next;
    }
    while(second){
        current->next = second;
        second = second->next;
        current = current->next;
    }
    //退出上面两个循环的任何一个循环时，current都指向merge完成的链表的最后一个元素
    //该元素的next指针已经为空


}
void mergeSort(ListNode*& front) {
    if(!front || !front->next){ //如果链表为空链表或只有一个元素，直接返回
        return;
    }
    ListNode *first,  *second;
    split(front, first, second);
    mergeSort(first);
    mergeSort(second);
    merge(front, first, second);
}


//将front链表分为三个链表:less, equal, greater
void partition(ListNode *front, ListNode *&less, ListNode *&equal, ListNode *&greater){
    equal = front;
    less = greater = nullptr;
    front = front->next;
    equal->next = nullptr;
    while(front){
        //将下一元素的地址保存在next指针中
        ListNode* next = front->next;
        if(front->data < equal->data){
            //将front插入less链表的头部
            ListNode *node = less;
            less = front;
            front->next = node;
        }else if(front->data > equal->data){
            //将front插入greater链表的头部
            ListNode *node = greater;
            greater = front;
            front->next = node;
        }else{
            //将front插入equal链表的头部
            ListNode *node = equal;
            equal = front;
            front->next = node;
        }
        //front指向下一元素
        front = next;
    }

}

//将已排好序的三个链表重新链接到front链表上
void concatenate(ListNode *&front, ListNode *less, ListNode *equal, ListNode *greater){
    if(less){
        front = less;
        while(less->next){
            less = less->next;
        }
        //now less->next is nullptr;
        // linklist equal can't be empty
        less->next = equal;
    }else{
        front = equal;
    }
    while(equal->next){
        equal = equal->next;
    }
    // now eqaul->next is nullptr
    equal->next = greater;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void quickSort(ListNode*& front) {
    if(!front || !front->next){//若链表为空或只有一个元素
        return;
    }
    ListNode *less, *equal, *greater;
    //链表至少有两个元素
    partition(front, less, equal, greater);
    quickSort(less);
    quickSort(greater);
    concatenate(front, less, equal, greater);
}


/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 *
 * As part of this process, you are also strongly recommedned to fill in
 * the function prototypes we have defined below.
 */

/*
 * We have provided this helper function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   while (front != nullptr) {
       cout << front->data;
       if (front->next != nullptr){
           cout << ", ";
       }
       front = front->next;
   }
   cout << "}" << endl;
}

/*
 * This helper function is provided a pointer to the front of the list
 * and is responsible for deallocating all of the memory being used
 * to store nodes in that list.
 */
void deallocateList(ListNode* front) {
    ListNode* next;
    while(front != nullptr){
        next = front->next;
        delete front;
        front = next;
    }
}

/*
 * This helper function is provided a vector of integer values and
 * returns a pointer to the beginning of a linked list containing
 * those values in the specified order.
 */
ListNode* createList(Vector<int> values){
    ListNode* first = nullptr;
    ListNode* previous = nullptr;
    ListNode* node = nullptr;
    if(!values.isEmpty()){
        first = new ListNode(values.pop_front(), nullptr);
        previous = first;
    }
    while(!values.isEmpty()){
        node = new ListNode(values.pop_front(), nullptr);
        if(previous){
            previous->next = node;
        }
        previous = node;
    }
    return first;
}

/*
 * This helper function is provided a vector of integer values
 * and a pointer to the beginning of a linked list. If these two structures
 * contain the same values in the same order, the function returns
 * true, otherwise it returns false.
 */
bool checkListVectorEquality(ListNode* front, Vector<int> v){
    if(!front && !v.isEmpty() || front && v.isEmpty()){
        return false;
    }
    //现在两者都不为空
    while(front || !v.isEmpty()){ //任何一个不为空时
         //如果另一个为空或哪一对不相等时
        if(!front && !v.isEmpty() || front && v.isEmpty()
                || front->data != v.pop_front()){
            return false;
        }
        front = front->next;
    }
    return true;
}

STUDENT_TEST("This is a skeleton for what your overall student tests should look like."){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1, 2, 3, 4};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    mergeSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(checkListVectorEquality(testList, {1, 2, 3}));
    EXPECT(!checkListVectorEquality(testList, {1, 2}));
    EXPECT(!checkListVectorEquality(testList, {1, 2, 3, 4}));
    EXPECT(!checkListVectorEquality(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v;
        ListNode* list = nullptr;

        /* Insert the same random values into linked list and vector. */
        for (int i = 0; i < n; i++) {
            int val = randomInteger(-10000, 10000);
            v.add(val);
            list = new ListNode(val, list);
        }

        /* The vector values are currently in the reverse order than the
         * linked list values – let's fix that so we have a fair comparison. */
        v.reverse();

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to generate a comparison. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

STUDENT_TEST("test on split:{1,2,3,4,5}"){
    Vector<int> values = {1, 2, 3, 4,5};
    ListNode *list = createList(values);
    ListNode *first, *second;
    split(list, first, second);
    EXPECT(checkListVectorEquality(first, {1,3,5}));
    EXPECT(checkListVectorEquality(second, {2, 4}));
    deallocateList(first);
    deallocateList(second);

}

STUDENT_TEST("test on split:{1,2}"){
    Vector<int> values = {1, 2};
    ListNode *list = createList(values);
    ListNode *first, *second;
    split(list, first, second);
    EXPECT(checkListVectorEquality(first, {1}));
    EXPECT(checkListVectorEquality(second, {2}));
    deallocateList(first);
    deallocateList(second);

}
STUDENT_TEST("test on partition:{4,1,5,2,4,7,4,9,-3}"){
    Vector<int> values = {4, 1, 5, 2, 4, 7, 4, 9, -3};
    ListNode *list = createList(values);
    ListNode *less, *equal, *greater;
    partition(list, less, equal, greater);
    printList(less);
    printList(equal);
    printList(greater);
    deallocateList(less);
    deallocateList(equal);
    deallocateList(greater);
}

STUDENT_TEST("test concatenate:{4,1,5,2,4,7,4,9,-3}"){
    Vector<int> lessValues = {1, 2, -3};
    Vector<int> equalValues = {4, 4, 4};
    Vector<int> greaterValues = {5, 7, 9};
    Vector<int> values = {1, 2, -3, 4, 4, 4, 5, 7, 9};

    ListNode *less, *equal, *greater, *newList;
    less = createList(lessValues);
    equal = createList(equalValues);
    greater = createList(greaterValues);
    concatenate(newList, less, equal, greater);

    EXPECT(checkListVectorEquality(newList, values));
    deallocateList(newList);
}



STUDENT_TEST("Sorting an empty list using mergesort"){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    mergeSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}
STUDENT_TEST("Sorting an single-element list using mergesort"){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    mergeSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}

STUDENT_TEST("Sorting an already  sorted list using mergesort"){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1, 2 ,3, 4, 5, 7};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    mergeSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}
STUDENT_TEST("Sorting a list that contain some duplicate elements using mergesort"){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1, 3, 3, 6, 2, 5, 5};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    mergeSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}
STUDENT_TEST("Sorting a list that contains positive and negative numbers using mergesort"){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1, -3, 3, 6, -2, 5, 5};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    mergeSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}
STUDENT_TEST("Sorting a long list of random values, randomly organized"){
    Vector<int> values = {};
    for(int i = 0; i < 5000; i++){
        int val = randomInteger(-10000, 10000);
        values.add(val);
    }
    ListNode* list = createList(values);

    /* Sort the linked list. */
    mergeSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}

STUDENT_TEST("Sorting an empty list using quickSort"){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    quickSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}
STUDENT_TEST("Sorting an single-element list using quickSort"){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    quickSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}

STUDENT_TEST("Sorting an already  sorted list using quickSort"){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1, 2 ,3, 4, 5, 7};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    quickSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}
STUDENT_TEST("Sorting a list that contain some duplicate elements using quickSort"){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1, 3, 3, 6, 2, 5, 5};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    quickSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}
STUDENT_TEST("Sorting a list that contains positive and negative numbers using quickSort"){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1, -3, 3, 6, -2, 5, 5};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    quickSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}
STUDENT_TEST("Sorting a long list of random values, randomly organized using quickSort"){
    Vector<int> values = {};
    for(int i = 0; i < 5000; i++){
        int val = randomInteger(-10000, 10000);
        values.add(val);
    }
    ListNode* list = createList(values);

    /* Sort the linked list. */
    quickSort(list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}

