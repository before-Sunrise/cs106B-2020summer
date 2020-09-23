#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

bool isLeaf(EncodingTreeNode* a){
    if(a == nullptr) return false;
    return a->one == nullptr && a->zero == nullptr;
}

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * bits queue contains a valid sequence of encoded bits.
 *
 * 只要messagebits不为空，每次取出一个bit，在编码树上走到对应的子树，判断当前子树结点是否为叶结点
 * 若为叶结点则将当前结点的char加入text并将当前结点重新设为根结点
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string text = "";
    EncodingTreeNode* currentNode = tree;
    Bit currentBit;
    while(!messageBits.isEmpty()){
        currentBit = messageBits.dequeue();
        if(currentBit == 0){
            currentNode = currentNode->zero;
        }else{
            currentNode = currentNode->one;
        }

        if(isLeaf(currentNode)){
            text += currentNode->ch;
            currentNode = tree;
        }
        // defensive code to avoid segment fault
        if(currentNode == nullptr){
            error("unvalid sequence of encoded bits");
        }

    }
    return text;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the input Queues are well-formed and represent
 * a valid encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeBits, Queue<char>& treeLeaves) {
    EncodingTreeNode* root = nullptr, *left, *right;
    char ch;
    if(treeBits.isEmpty()) return root;
    Bit frontTreeBits = treeBits.dequeue();
    if(frontTreeBits == 1){
        left = unflattenTree(treeBits, treeLeaves);
        right = unflattenTree(treeBits, treeLeaves);
        root = new EncodingTreeNode(left, right);
    }else{
        ch = treeLeaves.dequeue();
        root = new EncodingTreeNode(ch);
    }
    return root;

}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {
    EncodingTreeNode *tree = unflattenTree(data.treeBits, data.treeLeaves);
    string text = decodeText(tree, data.messageBits);
    deallocateTree(tree);
    return text;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    //construct the frequency table
    Map<char, double> frequency;
    for(char ch: text){
        if(frequency.containsKey(ch)){
            frequency[ch]++;
        }else{
            frequency[ch] = 1;
        }
    }
    //优先队列，优先级为double，元素类型为结点指针
    PriorityQueue<EncodingTreeNode*> queue;
    // initialize the priority queue
    for(char key: frequency){
        // 若优先队列中有两个元素优先级相同，那么它们出队列的顺序等于它们入队列的顺序
        //这里即在string text的先后顺序
        EncodingTreeNode *node = new EncodingTreeNode(key);
        queue.add(node, frequency[key]); //加入优先队列时要指定优先级，即词频
    }
    if(queue.size() < 2){
        error("ext does not contain at least two distinct characters.");
    }
    while(queue.size() >= 2){
        // 每次取出两个优先级最高（数值最小）的
        double leastPriority = queue.peekPriority();
        EncodingTreeNode *first = queue.dequeue();
        double secondePriority = queue.peekPriority();
        EncodingTreeNode *second = queue.dequeue();

        //组成新的结点，优先级为两者之和，进入优先队列
        EncodingTreeNode *newNode = new EncodingTreeNode(first, second);
        queue.add(newNode, leastPriority + secondePriority);
    }

    // 最后优先队列只剩下一个结点，即为构建好的huffman树的根节点
    return queue.dequeue();



}


// helper function, 给定一颗树tree， 在到达这棵树的叶节点前所经历的路径（即编码code，用来记录每一次遍历经历的路程），构建编码表table
void encodeTable(EncodingTreeNode *tree, Queue<Bit> code, Map<char, Queue<Bit>> &table){
    if(tree == nullptr){
        error("can't create encodedTable with an empty tree");
    }
    if(isLeaf(tree)){
        table.add(tree->ch, code);
        return;
    }else{
        Queue<Bit> leftCode = code;
        leftCode.enqueue(0);
        encodeTable(tree->zero, leftCode, table);
        code.enqueue(1);
        encodeTable(tree->one, code, table);
    }
}
/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * implement the help function encodeTable() to create the table
 */
Queue<Bit> encodeText(EncodingTreeNode *tree, string text) {
    Map<char, Queue<Bit>> table;//编码表，记录任一charactor和对应编码的映射
    encodeTable(tree, {}, table);
    Queue<Bit> result;
    for(char ch: text){
        if(table.containsKey(ch)){
            Queue<Bit> chCode = table[ch];
            while(!chCode.isEmpty()){
                result.enqueue(chCode.dequeue());
            }
        }else{
            error(string("encodeTable don't have the charactor ") + ch);
        }
    }
    return result;

}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input Queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeBits, Queue<char>& treeLeaves) {
    if(tree == nullptr){
        error("tree can't be error");
    }
    if(isLeaf(tree)){
        treeBits.enqueue(0);
        treeLeaves.enqueue(tree->ch);
        return;
    }else{
        treeBits.enqueue(1);
        flattenTree(tree->zero, treeBits, treeLeaves);
        flattenTree(tree->one, treeBits, treeLeaves);
    }
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    EncodedData data;
    EncodingTreeNode *tree = buildHuffmanTree(messageText);
    Queue<Bit> treeBits;
    Queue<char> treeLeaves;
    flattenTree(tree, treeBits, treeLeaves);
    Queue<Bit> messageBits = encodeText(tree, messageText);

    deallocateTree(tree);

    data.treeBits = treeBits;
    data.treeLeaves = treeLeaves;
    data.messageBits = messageBits;

    return data;


}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this helper function needed for testing. */

    EncodingTreeNode* r= new EncodingTreeNode('R');
    EncodingTreeNode* s = new EncodingTreeNode('S');
    EncodingTreeNode* rs = new EncodingTreeNode(r, s);
    EncodingTreeNode* e = new EncodingTreeNode('E');
    EncodingTreeNode* rse = new EncodingTreeNode(rs, e);
    EncodingTreeNode* t = new EncodingTreeNode('T');
    EncodingTreeNode* root = new EncodingTreeNode(t, rse);
    return root;
}

void deallocateTree(EncodingTreeNode* root) {
    if(root->zero != nullptr){
        deallocateTree(root->zero);
    }
    if(root->one != nullptr){
        deallocateTree((root->one));
    }
    delete  root;
}



bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
   if(a == b){ // a,b is nullptr or point to the same tree
       return true;
   }
   if(isLeaf(a) && isLeaf(b)){
       return a->ch == b->ch;
   }
   if(isLeaf(a) || isLeaf(b)){
       return false;
   }
   // now a and  b neither is leaf
   return areEqual(a->zero, b->zero) && areEqual(a->one, b->one);
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */


STUDENT_TEST("Test exampleTree and deallocateTree"){
    EncodingTreeNode* exampleTree = createExampleTree();
    deallocateTree(exampleTree);
}


STUDENT_TEST("Test areEqual"){
    EncodingTreeNode *exampleTreeOne = createExampleTree();
    EncodingTreeNode *exampleTreeTwo = createExampleTree();
    EXPECT(areEqual(exampleTreeOne, exampleTreeTwo));
    EXPECT(areEqual(exampleTreeOne->one, exampleTreeOne->one));
    EXPECT(!areEqual(exampleTreeOne->zero, exampleTreeOne->one));
    deallocateTree(exampleTreeOne);
    deallocateTree(exampleTreeTwo);
}






/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeBits   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeBits, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeBits
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedBits   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeBits;
    Queue<char> treeLeaves;
    flattenTree(reference, treeBits, treeLeaves);

    EXPECT_EQUAL(treeBits,   expectedBits);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeBits    = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeBits, treeBits);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "The job requires extra pluck and zeal from every young wage earner.",
        ":-) :-D XD <(^_^)>",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(output.size(), input.size());

        /* Don't clobber the output with a huge string if there's a mismatch. */
        EXPECT(input == output);
    }
}
