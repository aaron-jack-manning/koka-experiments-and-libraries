import operators

type tree<a>
    Leaf
    Branch (value : a, branches : list<tree<a>>)

fun combine (tr1 : tree<a>, tr2 : tree<a>, a : a) : tree<a>
    Branch (a, [tr1, tr2])

fun treemap (f : a -> b, tr : tree<a>) : div tree<b>
    match tr
        Leaf -> Leaf
        Branch (value, branches) ->
            Branch (f (value), branches.map(treemap ? f))