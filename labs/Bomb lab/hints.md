- Phase 1: The program is comparing two strings. The answer string is already there. Use print instruction in gdb to show that.
- Phase 2: The program asks for 6 integers. After reading the assembly code, we can find the answer is a geometric progression with ratio 2 and starting number 1.
- Phase 3: 2 ints. The first should be in some range, and with each of them, we can find corresponding second number in the code. The original program may be just a switch statement.
- Phase 4: 2 unsigned ints. The second one is 0, while the first one is >= 0 and <= 0xe and satisfies that func4(first, 0, first) == 0. func4 is a bit complicated with recursion, so I just bruteforced all of the 15 choices to find the correct value.
- Phase 5: string S of length 6. OK if `''.join("maduiersnfotvbylSo"[ord(ch) & 0xf] for ch in S) == "flyers"`
- Phase 6: Input 6 integers, which should be a permutation of 1,2,3,4,5,6. The program firstly transforms it with `lambda x: 7-x`. Then a new array of pointers is generated. ptr[i] = (address of the node with ID i in the linked list). The node structure is presumably the following, and we need to sort the list in decreasing order of val.

```c
struct node1 {
    int val, id;
    node1 *nxt;
};
```


