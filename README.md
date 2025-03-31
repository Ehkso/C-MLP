# C-MLP
I'm only making this public to own my mistakes and so people can point fingers and laugh at my precautionary tale.

## Basically, what happened was this:
- "MLPs and other Neural Networks are generally written in Python."
- "But C is faster and more fun than Python"
- "Let's make a CNN in C!" 
- - This is fine
- "And we'll make its usage as streamlined as Python code!" 
- - This is where I went wrong.

## What I did
To provide a clean UX, I wanted something like:
```
model = makeModel(params)
examples = getExamplesFromFile(file)
trainModel(model, examples, epochs)
```
However, this is basically OOP, isn't it? C doesn't have OOP - should I give up?

NO! Let's mimic an object by writing and reading from a .txt file to create "permanence" between epochs and stuff!

This is such a good idea because \[other justifications I came up with that don't make up for the fact that sequential i/o is TERRIBLE].

Don't do this - for real.
