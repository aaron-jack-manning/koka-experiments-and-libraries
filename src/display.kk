// The collection display functions exist as a series of functions for each primitive as I am not sure if it is possible to put constraints on generic types.

// Display lists of primitives
fun println (ls : list<int>) : console ()
    match ls
        Nil -> ()
        Cons(x, Nil) -> 
            println (x)
        Cons(x, xs) ->
            print (x)
            print (", ")
            println (xs)
        
    ()

fun println (ls : list<char>) : console ()
    match ls
        Nil -> ()
        Cons(x, Nil) -> 
            println (x)
        Cons(x, xs) ->
            print (x)
            print (", ")
            println (xs)
        
    ()

fun println (ls : list<bool>) : console ()
    match ls
        Nil -> ()
        Cons(x, Nil) -> 
            println (x)
        Cons(x, xs) ->
            print (x)
            print (", ")
            println (xs)
        
    ()

fun println (ls : list<double>) : console ()
    match ls
        Nil -> ()
        Cons(x, Nil) -> 
            println (x)
        Cons(x, xs) ->
            print (x)
            print (", ")
            println (xs)
        
    ()

fun println (ls : list<string>) : console ()
    match ls
        Nil -> ()
        Cons(x, Nil) -> 
            println (x)
        Cons(x, xs) ->
            print (x)
            print (", ")
            println (xs)
        
    ()


// Display vectors of primitives
fun println (vec : vector<int>) : console ()
    foreach-indexed (vec, fn (element, index) ->
        
        print (element)

        if index == vec.length - 1 then
            println ("")
        else
            print (", ")
    )

    ()

fun println (vec : vector<char>) : console ()
    foreach-indexed (vec, fn (element, index) ->
        
        print (element)

        if index == vec.length - 1 then
            println ("")
        else
            print (", ")
    )

    ()

fun println (vec : vector<bool>) : console ()
    foreach-indexed (vec, fn (element, index) ->
        
        print (element)

        if index == vec.length - 1 then
            println ("")
        else
            print (", ")
    )

    ()

fun println (vec : vector<double>) : console ()
    foreach-indexed (vec, fn (element, index) ->
        
        print (element)

        if index == vec.length - 1 then
            println ("")
        else
            print (", ")
    )

    ()

fun println (vec : vector<string>) : console ()
    foreach-indexed (vec, fn (element, index) ->
        
        print (element)

        if index == vec.length - 1 then
            println ("")
        else
            print (", ")
    )

    ()
