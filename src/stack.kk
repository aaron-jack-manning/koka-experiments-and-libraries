type stack<a>
    con Empty
    con Top (head : a, tail : stack<a>)

fun pop (st : stack<a>) : (maybe<a>, stack<a>)
    match st
        Empty -> (Nothing, Empty)
        Top (x,  xs) -> (Just (x), xs)

fun peek (st : stack<a>) : maybe<a>
    match st
        Empty -> Nothing
        Top (x, _) -> Just (x)

fun push (st : stack<a>, a : a) : stack<a>
    Top (a, st)