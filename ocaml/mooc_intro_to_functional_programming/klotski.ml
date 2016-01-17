(* -- Prelude --------- *)

exception NotFound

type 'e rel = 'e -> 'e list
type 'e prop = 'e -> bool

type ('a, 'set) set_operations = {
  empty : 'set;              (* The empty set. *)
  mem : 'a -> 'set -> bool;  (* [mem x s = true] iff [x] is in [s]. *)
  add : 'a -> 'set -> 'set;  (* [add s x] is the set [s] union {x}. *)
}

type ('configuration, 'move) puzzle = {
  move : 'configuration -> 'move -> 'configuration;
  possible_moves : 'configuration -> 'move list;
  final : 'configuration -> bool
}

type piece_kind = S | H | V | C | X
type piece = piece_kind * int
type board = piece array array
type direction = { dcol : int; drow : int; }
type move = Move of piece * direction * board

let x = (X, 0) and s = (S, 0) and h = (H, 0)
let (c0, c1, c2, c3) = ((C, 0), (C, 1), (C, 2), (C, 3))
let (v0, v1, v2, v3) = ((V, 0), (V, 1), (V, 2), (V, 3))
let all_pieces : piece list = [
  (S, 0) ;
  (H, 0) ;
  (C, 0); (C, 1); (C, 2); (C, 3) ;
  (V, 0); (V, 1); (V, 2); (V, 3) ;
] ;;

let move _ (Move (_, _, b)) = b


let initial_board =
  [| [| v0 ; s  ; s  ; v1 |];
  [| v0 ; s  ; s  ; v1 |];
  [| v2 ; h  ; h  ; v3 |];
  [| v2 ; c0 ; c1 ; v3 |];
  [| c2 ; x  ; x  ; c3 |] |]

let initial_board_simpler =
  [| [| c2 ; s  ; s  ; c1 |] ;
  [| c0 ; s  ; s  ; c3 |] ;
  [| v1 ; v2 ; v3 ; v0 |] ;
  [| v1 ; v2 ; v3 ; v0 |] ;
  [| x  ; x  ; x  ; x  |] |]

let initial_board_trivial =
  [| [| x  ; s  ; s  ; x  |] ;
  [| x  ; s  ; s  ; x  |] ;
  [| x  ; x  ; x  ; x  |] ;
  [| x  ; x  ; x  ; x  |] ;
  [| x  ; x  ; x  ; x  |] |]


(* -- Utilz ------------------------------------------------ *)

let (>>) f g x = g ( f x ) ;;

let (<<) g f x = g ( f x ) ;;

let swap f x y = f y x ;;

let cons x xs = x :: xs ;;

let append xs x = x :: xs ;;

let pair x y = (x, y) ;;

let (<*>) xs ys = xs |> List.map (swap List.map ys << pair) |> List.flatten ;;


(* -- Warmup ------------------------------------------------ *)

let rec loop p f = function
  | x when p x -> x
  | x -> loop p f (f x)
;;

let rec exists p = function
  | [] -> false
  | x :: _ when p x -> true
  | _ :: t -> exists p t
;;

let rec find p = function
  | [] -> raise NotFound
  | x :: _ when p x -> x
  | _ :: t -> find p t
;;


(* --- Part A: A Generic Problem Solver --- *)

let near x = [ x - 2 ; x - 1 ; x ; x + 1 ; x + 2 ] ;;

let flat_map (r : 'e rel) : 'e list -> 'e list =
  (* ( List.map r ) >> List.flatten ;; *)
  let add_all es e = List.fold_left append es (r e)
  in List.fold_left add_all []
  ;;

(* used for fusing flat_map and set filtering in archive_map *)
let flat_map_if (r : 'e rel) (p : 'e prop) : 'e list -> 'e list =
  let append_if es e = if p e then e :: es else es in
  let add_all es e = List.fold_left append_if es (r e)
  in List.fold_left add_all []
  ;;

let rec iter_rel rel = function
  | 0 -> rel
  | 1 -> rel
  | n -> iter_rel rel (n-1) >> flat_map rel
;;

let solve (r : 'e rel) (p : 'e prop) (x : 'e) : 'e =
  let rec loop xs =
    match find p xs with
    | y -> y
    | exception NotFound -> loop (flat_map r xs)
  in loop [x]
;;

let all_next r = function
  | [] -> raise NotFound
  | x :: _ as xs -> List.map (append xs) (r x)
;;

let solve_path (r : 'e rel) (p : 'e prop) (x : 'e) : 'e list =
  solve (all_next r) (List.hd >> p) [x] |> List.rev ;;

let archive_map (ops : ('e, 's) set_operations) (r : 'e rel) ((s, l) : ('s * 'e list)) : ('s * 'e list) =
  let l' = flat_map_if r (not << swap ops.mem s) l in
  let s' = List.fold_left (swap ops.add) s l'
  in (s', l')
;;

let solve' (ops : ('e, 's) set_operations) (r : 'e rel) (p : 'e prop) (x : 'e) : 'e =
  let rec loop (s, l) = match find p l with
    | y -> y
    | exception NotFound -> loop (archive_map ops r (s, l))
  in loop (ops.empty, [x])
;;

let solve_path' (ops : ('e list, 's) set_operations) (r : 'e rel) (p : 'e prop) (x : 'e) : 'e list =
  solve' ops (all_next r) (List.hd >> p) [x] |> List.rev ;;

let solve_puzzle (game: ('c, 'm) puzzle) (ops : ('c list, 's) set_operations)
: 'c -> 'c list =
  let rel c = c |> game.possible_moves |> List.map (game.move c)
  in solve_path' ops rel game.final
;;


(* --- Part B: A Solver for Klotski --- *)

let final (b : board) : bool =
  let want (x,y) = b.(x).(y) = ( S, 0 ) in
  List.for_all want [(3,1) ; (3,2) ; (4,1) ; (4,2)]
;;

module MovePiece = struct (* helpers for move_piece *)
  let ps_of b p =
    let ps : (int * int) list ref = ref [] in
    for i = 0 to 4 do
      for j = 0 to 3 do
        if b.(i).(j) = p then ps := (i,j) :: !ps else ()
      done
    done ;
    !ps
  ;;

  let put b p = List.iter (fun (i,j) -> b.(i).(j) <- p) ;;

  let add drow dcol (i, j) = (i + drow, j + dcol) ;;

  let inside (a, b) y = a <= y && y <= b ;;

  let in_board (i, j) = inside (0, 4) i && inside (0, 3) j ;;

  let empty_or b p (i, j) = let y = b.(i).(j) in y = x || y = p ;;

  let all_of f g x = (f x) && (g x) ;;

  let copy b =
    let b' = Array.copy b in (
      for i = 0 to 4 do
        b.(i) |> Array.copy |> Array.set b' i
      done ;
      b'
    ) ;;

  let move_piece b p { drow ; dcol } =
    let ps = ps_of b p in
    let ps' = List.map (add drow dcol) ps in
    let check = all_of in_board (empty_or b p) in
    if List.for_all check ps'
    then
      let b' = copy b
      in (
        put b' x ps ;
        put b' p ps' ;
        Some b'
      )
    else None
  ;;
end

let move_piece : board -> piece -> direction -> board option =
  MovePiece.move_piece ;;

let all_directions = [
  { drow = +1; dcol = 0 } ;
  { drow = -1; dcol = 0 } ;
  { drow = 0; dcol = +1 } ;
  { drow = 0; dcol = -1 } ;
] ;;

let all_moves = all_pieces <*> all_directions ;;

let possible_moves (b : board) : move list =
  let try_move ms (p,d) = match move_piece b p d with
    | Some b' -> Move (p, d, b') :: ms
    | None -> ms
  in all_moves |> List.fold_left try_move []
;;

module BoardOrdering = struct
  type t = board

  let kind_rank = function
    | S -> 5
    | H -> 4
    | C -> 3
    | V -> 2
    | X -> 1

  let rank (k,n) = n + 10 * kind_rank k

  exception Not_equal of int ;;

  let check = function | 0 -> () | c -> raise ( Not_equal c )

  let iter b1 b2 =
    for i = 0 to 4 do
      let (r1, r2) = ( b1.(i), b2.(i) ) in
      for j = 0 to 3 do
        rank r1.(j) - rank r2.(j) |> check
      done
    done

  let compare b1 b2 = try iter b1 b2 ; 0 with Not_equal c -> c
end

module BoardSet = Set.Make(BoardOrdering)

let board_opset : (board list, BoardSet.t) set_operations = {
  empty = BoardSet.empty ;
  mem = List.hd >> BoardSet.mem ;
  add = List.hd >> BoardSet.add ;
} ;;

module BoardListOrdering = struct
  type t = board list

  let check = BoardOrdering.check

  let iter = BoardOrdering.iter

  let rec iter_ls bs1 bs2 = match (bs1, bs2) with
    | ( [], [] ) -> 0
    | ( [], _ ) -> -1
    | ( _, [] ) -> +1
    | ( b1 :: t1, b2 :: t2 ) -> ( iter b1 b2 ; iter_ls t1 t2 ) ;;

  let rec compare bs1 bs2 = match (bs1, bs2) with
    | ( [], [] ) -> 0
    | ( [], _ ) -> -1
    | ( _, [] ) -> +1
    | ( b1 :: t1, b2 :: t2 ) ->
        try iter b1 b2 ; compare t1 t2 with BoardOrdering.Not_equal c -> c

  let rec compare2 bs1 bs2 =
    try (
      List.length bs1 - List.length bs2 |> check ;
      iter_ls bs1 bs2
    ) with BoardOrdering.Not_equal c -> c

end

module BoardListSet = Set.Make(BoardListOrdering)

let boardlist_opset : (board list, BoardListSet.t) set_operations = {
  empty = BoardListSet.empty ;
  mem = BoardListSet.mem ;
  add = BoardListSet.add ;
} ;;

let klotski_opset : (board, move) puzzle = {
  move = move ;
  possible_moves = possible_moves ;
  final = final ;
} ;;

let solve_klotski = solve_puzzle klotski_opset board_opset ;;
let solve_klotski = solve_puzzle klotski_opset boardlist_opset ;;
