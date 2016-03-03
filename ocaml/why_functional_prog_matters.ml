(* ---- utils --------------------------------------------------------------- *)

let (>>) f g x = g ( f x )

let lft (a, _) = a
let rgh (_, b) = b


(* ---- part 1: fold map ---------------------------------------------------- *)

let rec fold f a = function
  | [] -> a
  | h :: t -> fold f ( f a h ) t

let sum = fold (+) 0

let cons h t = h :: t

let swap f x y = f y x

let map f = fold ( f >> cons |> swap ) []


(* ---- iterators ----------------------------------------------------------- *)

type 'a gen = Gen of ( unit -> 'a * 'a gen )

let nx ( Gen fn ) = fn ()

let iter fn x0 =
  let rec loop x () = (x , Gen ( loop (fn x) ) )
  in Gen ( loop x0 )

let iter_step fn y0 =
  let rec loop y () =
    let (x, y') = fn y in (x, Gen ( loop y' ) )
  in Gen ( loop y0 )

let rec map_iter fn (Gen g) =
  let iter () =
    let (a, g') = g () in (fn a, map_iter fn g')
  in
  Gen iter

let inc n = n + 1

let nat = iter inc 0

let dbl n = 2 * n

let even = map_iter dbl nat

(* ---- lazy list ----------------------------------------------------------- *)

type 'a stream = 'a cell ref
  and 'a cell = Not of 'a gen | Has of ( 'a * 'a stream )

let gen_to_cell (a, g) = ( a, ref ( Not g ) )

let advance s g =
  let c = g |> nx |> gen_to_cell in (
    s := Has c ;
    c
  )

let ensure_next ( s : 'a stream ) : 'a * 'a stream = match !s with
  | Has tp -> tp
  | Not g -> advance s g

let hd s = s |> ensure_next |> lft
let tl s = s |> ensure_next |> rgh

(* let rec nth ( s : 'a stream ) : 'a = function *)
let rec nth s = function
  | 0 -> hd s
  | n -> nth (tl s) (n-1)

let iter_stream fn x0 = ref ( Not (iter fn x0) )

let nat_stream = iter_stream inc 0

let map_stream fn s =
  let step s' = ( s' |> hd |> fn, s' |> tl )
  in
  ref ( Not ( iter_step step s ) )

let map2_stream fn2 s =
  let step s' =
    let (a, b) = ( s' |> hd, s' |> tl |> hd ) in
    (fn2 a b, s' |> tl)
  in
  ref ( Not ( iter_step step s ) )


(* ---- part 4: num approx -------------------------------------------------- *)

let sqrt_next y x = x /. 2. +. 0.5 *. y /. x

let sqrt_stream y = iter_stream (sqrt_next y) (2. *. y)

let sqrt2 = sqrt_stream 2.

let sqrt100 = sqrt_stream 100.

let rec approx dist s =
  let (a, b) = ( s |> hd, s |> tl |> hd ) in
  if dist a b then b else approx dist (tl s)

let rel_dist x y = abs_float ( x /. y -. 1. )

let rel e = approx (fun x y -> rel_dist x y < e )

let diff fn x h = ( ( fn ( x +. h ) ) -. ( fn x ) ) /. h

let half h = h /. 2.

let differentiate h0 fn x = map_stream (diff fn x) (iter_stream half h0)

let fsq x = x *. x

let fsq_prime x : float stream = differentiate 1. fsq x

let log2 x = ( log x ) /. ( log 2. )

let order s =
  let (a, b, c) = ( s |> hd, s |> tl |> hd , s |> tl |> tl |> hd)
  in
  log2 ( (a -. b) /. (b -. c) )

let err_correct n s : float stream =
  let fn2 a b =
    let n2 = 2. ** n in ( b *. n2 -. a ) /. (n2 -. 1.)
  in
  map2_stream fn2 s

let improve s = err_correct (order s) s

let fsq_prime_better x = x |> fsq_prime |> improve


(* ---- main ---------------------------------------------------------------- *)

let () = (
  nth nat_stream 1 |> print_int ; print_newline () ;
  nth nat_stream 2 |> print_int ; print_newline () ;
  nth nat_stream 3 |> print_int ; print_newline () ;

  nth sqrt2 3 |> print_float ; print_newline () ;
  nth sqrt2 4 |> print_float ; print_newline () ;
  nth sqrt2 10 |> print_float ; print_newline () ;
)
