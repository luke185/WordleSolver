# WordleSolver

A simple program written in c++ for assistance in solving the NYT [Wordle](https://www.nytimes.com/games/wordle)  
(uses the [2023-03-27](https://gist.github.com/cfreshman/d97dbe7004522f7bc52ed2a6e22e2c04) list of possible guesses)

## Features

- Generate random starter word
- Input guessed words to show remaining possible words

## How it works

1. User inputs letters from guess, based on colours
2. Words that don't fit these conditions, based on the rules of Wordle, are then removed
3. The user is then displayed a count and list of all possible remaining words

## How to use WordleSolver

1. Compile `wordleSolver.cpp` with c++ compiler of choice (gcc, clang, etc.)
2. Run complied executable in terminal of choice

## Example

- User entered word `BARKS`
- Wordle answer is `READY`

```
Welcome to the wordle solver!
w -> add a word
r -> reset words
s -> generate starter word
q -> quit
w

Enter new GREY letters
(DO NOT include letters that are green/yellow somewhere else in the word)
(If there are no new grey letters, just press ENTER)
bks
loading...
Words left: 6428

Enter new GREEN letters:
Type letters out in order, using spaces for non-green letters
example: word is 'hello' where 'e' & 'o' are green = ' e  o'
(If there are no new green letters, just press ENTER)

Enter new YELLOW letters:
Type letters out in order, using spaces for non-yellow letters
example: word is 'hello' where 'e' & 'o' are yellow = ' e  o'
(If there are no new green letters, just press ENTER)
 ar
loading...
Words left: 494

Remaining possible words:
acara,
acari,
accra,
achar,
...

Words left: 494

Welcome to the wordle solver!
w -> add a word
r -> reset words
s -> generate starter word
q -> quit

```



