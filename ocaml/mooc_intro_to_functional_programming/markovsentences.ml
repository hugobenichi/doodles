(* -- Types --------------------------------------------------------------- *)

type ltable = (string * string list) list

type distribution = { total : int ; amounts : (string * int) list }

type htable = (string, distribution) Hashtbl.t

type ptable = { prefix_length : int ; table : (string list, distribution) Hashtbl.t }


(* -- Text ---------------------------------------------------------------- *)

let simple_0 =
  "I am a man and my dog is a good dog and a good dog makes a good man" ;;

let simple_1 =
  "a good dad is proud of his son and a good son is proud of his dad" ;;

let simple_2 =
  "a good woman is proud of her daughter and a good daughter is proud of her
  mom" ;;

let simple_3 =
  "there is a beer in a fridge in a kitchen in a house in a land where \
  there is a man who has a house where there is no beer in the kitchen" ;;

let multi_1 =
  "A good dad is proud of his son. \
  A good son is proud of his dad." ;;

let multi_2 =
  "A good woman is proud of her daughter. \
  A good daughter is proud of her mom." ;;

let multi_3 =
  "In a land of myths, and a time of magic, \
   the destiny of a great kingdom rests \
   on the shoulders of a young man." ;;

let grimms_travelling_musicians =
  "An honest farmer had once an ass that had been a faithful servant ..." ;;

let grimms_cat_and_mouse_in_partnership =
  "A certain cat had made the acquaintance of a mouse, and ..." ;;

let the_war_of_the_worlds_chapter_one =
  "No one would have believed in the last years ..." ;;

let some_cookbook_sauce_chapter =
  "Wine Chaudeau: Into a lined saucepan put ½ bottle Rhine ..." ;;

let history_of_ocaml =
  "“Caml” was originally an acronym for Categorical ..." ;;


(* -- utilz --------------------------------------------------------------- *)

let (>>) f g x = g ( f x )

let (<<) g f x = g ( f x )

let cons h t = h :: t

let append t h = h :: t

let swap f x y = f y x


(* -- Part A -------------------------------------------------------------- *)

module Split = struct
  let at = swap String.get

  let empty s = String.length s = 0

  let is_digit c = '0' <= c && c <= '9'

  let is_letter c = ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')

  let is_alpha c = is_digit c || is_letter c

  let sub s i j = String.sub s i (j - i)

  let words str =
    let stop = String.length str in
    let rec loop (acc, i) = function
      | j when j = stop -> (sub str i j) :: acc
      | j ->
          let next =
            if str |> at j |> ( not << is_alpha )
            then (sub str i j :: acc, j+1)
            else (acc, i)
          in loop next (j+1)
    in loop ( [], 0 ) 1 |> List.filter (not << empty) |> List.rev

end

let words : string -> string list = Split.words ;;

module LTable = struct

  let get x ltbl = match List.assoc x ltbl with
    | ys -> ys
    | exception Not_found -> []

  let add ltbl (x, y) =
    let ys = ltbl |> get x |> cons y in
    ltbl |> List.remove_assoc x |> cons (x, ys)

  let build words =
    List.combine ( "START" :: words ) ( words @ [ "STOP" ] )
      |> List.fold_left add []

  let sample ls = ls |> List.length |> Random.int |> List.nth ls

  let next table word = table |> get word |> sample

  let walk table =
    let rec loop acc w =
      match next table w with
      | "STOP" -> acc
      | w' -> loop ( w' :: acc ) w'
    in loop [] "START" |> List.rev

end

let build_ltable : string list -> ltable = LTable.build

let next_in_ltable : ltable -> string -> string = LTable.next

let walk_ltable : ltable -> string list = LTable.walk


(* -- Part B -------------------------------------------------------------- *)

module HTable = struct

  let get h x = match Hashtbl.find h x with
    | ys -> ys
    | exception Not_found -> []

  (* TODO: verifier ca *)
  let sample { total ; amounts } : string =
    let rec loop left = function
      | [] -> raise Not_found
      | ( w , _ ) :: [] -> w
      | ( w , n ) :: _ when left < n -> w
      | ( _ , n ) :: t -> loop (left - n) t
    in loop (Random.int total) amounts

  let count l =
    let rec loop = function
      | ( acc, [] ) -> acc
      | ( [], w :: t ) ->
          loop ( [ ( w, 1 ) ], t )
      | ( ( w, n ) :: r, w' :: t ) when w = w' -> loop ( ( w, n + 1 ) :: r, t )
      | ( l, w' :: t ) -> loop ( ( w', 1 ) :: l, t )
    in
    loop ( [], List.sort compare l )

  let distribution l = {
    total = List.length l ;
    amounts = count l ;
  }

  let group words =
    let add h (x, y) = Hashtbl.replace h x ( y :: get h x ) ; h
    in
    List.combine ( "START" :: words ) ( words @ [ "STOP" ] )
      |> List.fold_left add (Hashtbl.create 10)

  let from_groups ws_tbl =
    let add w ws t = Hashtbl.replace t w ( distribution ws ) ; t
    in
    Hashtbl.fold add ws_tbl ( Hashtbl.create 10 )

  let build = group >> from_groups

  let next t = ( Hashtbl.find t ) >> sample

  let walk table =
    let rec loop acc w =
      match next table w with
      | "STOP" -> acc
      | w' -> loop ( w' :: acc ) w'
    in loop [] "START" |> List.rev

end

let compute_distribution : string list -> distribution = HTable.distribution

let build_htable : string list -> htable = HTable.build

let next_in_htable : htable -> string -> string = HTable.next

let walk_htable : htable -> string list = HTable.walk


(* -- Part C -------------------------------------------------------------- *)

module Sentence = struct

  type cr = Wrd | Pct | Trm | Sep

  let ponctuation = [ ';' ; ',' ; ':' ; '-' ; '"' ; '\'' ; '?' ; '!' ; '.' ]

  let terminal = [ '?' ; '!' ; '.' ]

  let rec contains c = function
    | [] -> false
    | c' :: _ when c = c' -> true
    | _ :: t -> contains c t

  let is_word c = ( Split.is_alpha c ) || ( '\128' <= c )

  let kind = function
    | c when is_word c              -> Wrd
    | c when contains c terminal    -> Trm
    | c when contains c ponctuation -> Pct
    | _                             -> Sep

  let at = Split.at

  let sub = Split.sub

  let rec advance_word s i =
    let rec loop j =
      if ( j = String.length s ) || ( s |> at j |> ( not << is_word ) )
      then j
      else loop ( j + 1 )
    in
    let j = loop ( i + 1 ) in ( sub s i j, j )

  let add lss = function
    | [] -> lss
    | ls -> ls |> List.rev |> append lss

  let rec next s (ls, lss, i) =
    if i = String.length s
    then ls |> add lss |> List.rev
    else
      let acc = match s |> at i |> kind with
        | Wrd -> let ( w, j ) = advance_word s i in ( w :: ls, lss, j )
        | Pct -> ( sub s i (i + 1) :: ls, lss, i + 1 )
        | Trm -> ( [], sub s i (i + 1) |> append ls |> add lss, i + 1 )
        | Sep -> ( ls, lss, i + 1 )
      in
      next s acc

  let split s = next s ( [], [], 0 )
end

let sentences : string -> string list list = Sentence.split

module PTable = struct

  let get = HTable.get
  let next = HTable.next
  let from_groups = HTable.from_groups

  let rec start : int -> string list = function
    | 0 -> []
    | n -> "START" :: ( start (n-1) )

  let shift l x = ( List.tl l ) @ [ x ]

  let group ws lp =
    let add p xs y = Hashtbl.replace p xs ( y :: HTable.get p xs )
    in
    let rec loop p chain = function
      | [] -> add p chain "STOP" ; p
      | w :: t -> add p chain w ; loop p (shift chain w) t
    in
    loop (Hashtbl.create 10) (start lp) ws

  let build ws lp = {
    prefix_length = lp ;
    table = group ws lp |> from_groups ;
  }

  let walk { table ; prefix_length } =
    let rec loop acc ws =
      match next table ws with
      | "STOP" -> acc
      | w -> loop ( w :: acc ) (shift ws w)
    in loop [] ( start prefix_length ) |> List.rev

end

let start : int -> string list = PTable.start

let shift : string list -> string -> string list = PTable.shift

let build_ptable : string list -> int -> ptable = PTable.build

let walk_ptable : ptable -> string list = PTable.walk

module Merge = struct
  let pt pl t = {
    table = t ;
    prefix_length = pl ;
  }

  let zero_dist = { total = 0 ; amounts = [] }

  let get h x = match Hashtbl.find h x with
    | ys -> ys
    | exception Not_found -> zero_dist

  let rec merge_ls ls = function
    | [] -> ls
    | x :: t -> merge_ls ( x :: ls ) t

  let merge_dist { total = t1 ; amounts = ws1 } { total = t2 ; amounts = ws2 } = {
    total = t1 + t2 ;
    amounts = merge_ls ws1 ws2 ;
  }

  let add xs d t = Hashtbl.replace t xs ( get t xs |> merge_dist d ) ; t

  let merge_two { table = t1 ; prefix_length = l1 } { table = t2 ; prefix_length = l2 } =
    if l1 <> l2
    then raise Not_found
    else
      ( Hashtbl.create 10 ) |> Hashtbl.fold add t1 |> Hashtbl.fold add t2 |> pt l1

  let merge ps = List.fold_left merge_two ( List.hd ps ) ( List.tl ps )
end

let merge_ptables : ptable list -> ptable = Merge.merge
