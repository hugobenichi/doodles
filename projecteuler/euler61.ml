(* util *)
let join f ls =
  ls |> List.map f |> List.fold_left (fun a b -> a ^ b ^ "\n") ""

let print_with f ls = ls |> join f |> print_string

let rec last ls = match ls with
  | [] -> raise Not_found
  | x :: [] -> x
  | _ :: t -> last t

let fmap f ls = ls |> List.map f |> List.concat


(* define poly number lists *)
let f_poly a b n = n*(a*n+b)/2

let f_tri = f_poly 1 1

let f_sqr = f_poly 2 0

let f_pen = f_poly 3 (-1)

let f_hex = f_poly 4 (-2)

let f_hep = f_poly 5 (-3)

let f_oct = f_poly 6 (-4)

let make_poly_nums start stop f =
  let rec first n = if (f n) < start then first (n+1) else n in
  let rec loop n = if (f n) > stop then [] else (f n) :: loop (n+1) in
    1 |> first |> loop


(* problem algo: generate all combinations *)
let hd x = x / 100 mod 100

let tl x = x mod 100

let valid_chain ls = (ls |> List.hd |> hd) = (ls |> last |> tl)

let find_next x = List.filter (fun y -> hd y == tl x)

let shift ls = if ls = [] then [] else (List.tl ls) @ [List.hd ls]

let perms (ls : 'a list) : 'a list list  =
  let apply loop n ps ks = loop n (ks :: ps) ks in
  let rec loop n ps qs =
    if n = 0
      then ps
      else qs |> shift |> apply loop (n-1) ps
  in
    loop (List.length ls) [] ls

let all_next (x : 'a) (llls : 'a list list list) : ('a * 'a list list list) list =
  let assoc lls = match lls with
    | [] -> []
    | h :: t -> h |> find_next x |> List.map (fun y -> (y, perms t))
  in fmap assoc llls

let rec combine ((x, llls) : ('a * 'a list list list)) : 'a list list =
  match all_next x llls with
  | [] -> [[x]]
  | ls -> ls |> fmap combine |> List.map (fun ls -> x :: ls)

let solve (polys: 'a list list) : 'a list list =
  let llls = polys |> List.tl |> perms in
  let assoc x = combine (x, llls) in
  polys |> List.hd |> fmap assoc
        |> List.filter valid_chain
        |> List.filter (fun ls -> List.length ls = List.length polys)


(* main *)

let () =
  let polys = [f_tri; f_sqr; f_pen; f_hex; f_hep; f_oct] in
  let sols = polys |> List.map (make_poly_nums 1000 10000) |> solve in
    sols |> List.map (join string_of_int) |> print_with (fun s -> s);
    sols |> List.map (List.fold_left (+) 0) |> print_with string_of_int

(*
let () =
  [1; 2; 3] |> perms
            |> List.map (join string_of_int)
            |> print_with (fun s -> s)
*)

(* 
let () =
  (make_poly_nums 1000 10000 f_tri) |> print_with string_of_int
*)
