infixl 30 (?)
infixr 30 (::)

// Currying
fun (?) (h : a -> y z, a : a) : y z
    h (a)

fun (?) (h : (a, b) -> y z, a : a) : (f : b -> y z)
    fn (b) { h (a, b) }

fun (?) (h : (a, b, c) -> y z, a : a) : (f : (b, c) -> y z)
    fn (b, c) { h (a, b, c) }

fun (?) (h : (a, b, c, d) -> y z, a : a) : (f : (b, c, d) -> y z)
    fn (b, c, d) { h (a, b, c, d) }

fun (?) (h : (a, b, c, d, e) -> y z, a : a) : (f : (b, c, d, e) -> y z)
    fn (b, c, d, e) { h (a, b, c, d, e) }

fun (?) (h : (a, b, c, d, e, f) -> y z, a : a) : (f : (b, c, d, e, f) -> y z)
    fn (b, c, d, e, f) { h (a, b, c, d, e, f) }

fun (?) (h : (a, b, c, d, e, f, g) -> y z, a : a) : (f : (b, c, d, e, f, g) -> y z)
    fn (b, c, d, e, f, g) { h (a, b, c, d, e, f, g) }

// List construction (note that literals can be created with [,] syntax, but this is poorly documented)
fun (::) (a : a, ls : list<a>) : list<a>
    Cons(a, ls)

fun (::) (a : a, b : a) : list<a>
    Cons(a, Cons(b, Nil))