# Scanner for Simplified C

## Code Structure
In this project, I implemented a simplified C scanner with the help of a Non-deterministic Finite Automaton (NFA). My project consists of two main components: the Scanner class and the NFA class, which are declared in the scanner.hpp and nfa.hpp header files, respectively.

### Scanner class:

I designed the Scanner class to handle the high-level tokenization of the input string.
I created a public constructor that takes a std::string as input.
I used the matchedTokens vector to store the matched tokens from the input string.
I implemented the compile() function as a public method that initiates the scanning process.
I also included private methods like test(), buildReservedWords(), buildINT_NUM(), buildID(), and buildSpecialSymbols() to help in constructing the various NFAs for different tokens and testing the input string against them.
I stored the reserved words and special symbols, along with their corresponding token types, in the reservedWords and specialSymbols arrays.
I used the reservedNfas and nfas vectors to store the NFAs for reserved words and other tokens, respectively.

### NFA class:
I defined the NFAState struct to represent a state in the NFA, with its transitions (a unordered map) and an associated token.
I created the Token enum to represent the different token types, as provided in the initial question.
I designed the NFA class to represent a Non-deterministic Finite Automaton, with methods for constructing an NFA from a reserved word, anonymous characters, or concatenating, repeating, or applying the star operation on other NFAs.
I also included methods for creating a new NFA state, adding transitions between states, and checking if a token is reserved.
I used the startState, currState, and endState pointers to represent the start, current, and end states of the NFA, respectively.

## How do I design the NFA?

The NFAState class I first implemented consists of a Token identifier (for non-accepted state, it is UNKNOWN) and an unordered_map (hash map) to store the transitions info.

The NFA class keeps the start, end, and current state (current state is not used in my scanner implementation).

I implemented the basic building block for regex in NFA, namely alternative, concatenation, repetition (+/*), as methods shown below
```C++
    NFA selfRepeat();
    NFA concatWith(const NFA&);
    NFA alternativeWith(const NFA&);
    NFA starOp();
```
Then I just these building block to construct the NFA as per the syntax specifications (see `buildReservedWords()`, `buildINT_NUM()`, `buildID()`, and `buildSpecialSymbols()` in scanner.cpp).


## How do I design the DFA?

The NFA can be converted into NFA with the powerset methods. For a certain state in NFA, all states reachable using empty transition (epsilon) can be collapsed into one state and only preserve the the transitions among the collapsed states. A union-find algorithm could be used to quicly merge and sets.

However, I think converting NFA to DFA is not a must in this scanner as it is bascially a computation-memory trade-off. A lot of real-world regex engines implement a NFA ones as breaking the pure NFA definition can enable exntensive functionalities.


## How do I design the Scanner?
Based upon (serveral) NFA, I adopt the backtracking method with a stack to enforce the longest-match rule. It will be awfully slow if we make each NFA as seperate engine and test all regexes one by one. Instead, I combine all NFAs into a big one and the backtracking stacks contains all states at the fringe from all NFAs. As long as there is an accepting state, it will keep it in the record (if it is longer than existing match) and keep consuming the input stream to look for a longer match. When all NFAs comes to a dead state, it will evict the longest match into the result records and reset the fringe stack to contain all start state of NFAs.
Note that in the fringe (frontier) stack, it stores a tuple of `<StatePtr, index, std::unordered_set<StatePtr>>`. The index is used for backtrakcing and when resting the fringe stack, it contains all starts state with index pointing to the first unmatched charater in the stream (/string).
The existence of the `unordered_set` is to resolve a technical issues in backtrakcing the NFAs, **epsilon loops**. To prevent infinite loops caused by epsilon transitions, a visited set will also be stored in the stack to store the visited state from a epsilon transition and will be cleared to empty when a non-epsilon is encountered.

To correctly handle the reserved words, two adaptations are needed:
* for a component seperated by white space, it should first be tested against the reserved word NFAs.
* after that, in case of a tie on length in the enforcement of longest match policy, the reservedWord should be prioritized.

Actually, the second one automatically solves the problem and the first one is no longer needed.