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