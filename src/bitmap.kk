struct colour
    red : int
    green : int
    blue : int

fun bytes (number : int) : div list<int>
    fun base-b-algorithm (current, base)
        val (quotient, remainder) = divmod (current, base)

        match quotient
            0 -> Cons(remainder, Nil) ++ Nil
            _ -> Cons(remainder, Nil) ++ (base-b-algorithm (quotient, base))

    val digits = base-b-algorithm (number, 256)

    val padding = 4 - length(digits) % 4

    digits
    .append (list (1, padding, fn (x) { 0 }))

fun get-header (file-size : int)

    var header := vector (14, 0)

    header[0] := 66 // B
    header[1] := 77 // M

    val file-size-bytes = bytes (file-size)

    for (0, 11,
        fn (i) {
            header[i + 2] := maybe(file-size-bytes[i], 0)
        })

    header[10] := 54 // Header Size

    header

fun get-dib-header (horizontal-resolution : int, vertical-resolution : int, dpcm : int)

    var dib-header := vector (50, 0)

    dib-header[0] := 40 // Size of DIB Header
    dib-header[12] := 1 // Number of colour planes
    dib-header[14] := 24 // Bits per pixel

    val horizontal-resolution-bytes = bytes (horizontal-resolution)
    val vertical-resolution-bytes = bytes (vertical-resolution)
    val print-resolution-bytes = bytes (dpcm * 100)

    for (0, 3,
        fn (i) {
            dib-header[4 + i] := maybe(horizontal-resolution-bytes[i], 0)
            dib-header[8 + i] := maybe(vertical-resolution-bytes[i], 0)
            dib-header[24 + i] := maybe(print-resolution-bytes[i], 0)
            dib-header[28 + i] := maybe(print-resolution-bytes[i], 0)
        })

    dib-header

fun colour-to-bytes (colour : colour) : vector<int>
    vector-init(3, fn (i) {
        val values = match i
            0 -> colour.blue
            1 -> colour.green
            2 -> colour.red
            _ -> 0

        return values
        })

fun round-up-to-four (number : int) : int
    if number % 4 == 0 then
        number
    else
        number + 4 - (number % 4)

fun get-bitmap-bytes (screen : vector<vector<colour>>) : <div,exn> vector<int> // this will ultimately return vector<int>

    val horizontal-resolution = screen.length
    val vertical-resolution = screen[0].length

    val filesize = 54 + (vertical-resolution * round-up-to-four (horizontal-resolution))


    val header = get-header (filesize)
    val dib-header = get-dib-header (horizontal-resolution, vertical-resolution, 40)

    val padding = horizontal-resolution % 4


    val pixels-length = 3 * (horizontal-resolution * vertical-resolution) + (padding * vertical-resolution)

    var pixels := vector (pixels-length, 0)


    var pixels-index := 0

    for (0, vertical-resolution - 1,
        fn(i) {
            for (0, horizontal-resolution - 1,
                fn(j) {
                    val colour = colour-to-bytes (screen[i][j])

                    foreach (colour,
                        fn(colour-value) {
                            pixels[pixels-index] := colour-value
                            pixels-index := pixels-index + 1
                        })

                })
        
            for (1, padding,
                fn (padding-index) {
                    pixels[pixels-index] := 0
                    pixels-index := pixels-index + 1
                })
        
        })


    var final-bitmap := vector (header.length + dib-header.length + pixels.length, 0)


    for (0, header.length - 1,
        fn(i) {
            final-bitmap[i] := header[i]
        })

    for (0, dib-header.length - 1,
        fn(i) {
            final-bitmap[header.length + i] := dib-header[i]
        })

    for (0, pixels.length - 1,
        fn(i) {
            final-bitmap[header.length + dib-header.length + i] := pixels[i]
        })


    final-bitmap