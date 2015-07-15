let (>>) f g x = g (f x)

(* char list <-> string util *)
let explode s =
  let rec loop l i =
    if i < 0
      then l
      else loop (s.[i] :: l) (i - 1)
  in loop [] (String.length s - 1)

let join charls =
  let rec loop s ls = match ls with
    | [] -> s
    | h :: t -> loop ((Char.escaped h) ^ s) t
  in loop "" charls

let joinln = List.fold_left (fun a b -> a ^ b ^ "\n") ""

let print_strings = joinln >> print_string


(* keying function *)
let cmp a b = (Char.code a) - (Char.code b)

let cube n = n * n * n

let ckey n = n |> cube |> string_of_int |> explode |> List.sort cmp |> join


(* algo: loop on ints, index int by ckeys, exit when 5 ints with same key *)
let populate max =
  let rec loop hasht n =
    if n < max then (
      Hashtbl.add hasht (ckey n) n;
      loop hasht (n+1)
    ) else hasht
  in loop (Hashtbl.create 1000) 1

let check_sol n ls = if List.length ls = n then ls else []

let find max m hasht =
  let rec loop chk n =
    if n < max
      then match n |> ckey |> Hashtbl.find_all hasht |> chk with
        | [] -> loop chk (n+1)
        | solution -> solution
      else []
  in loop (check_sol m) 1

let () =
  populate 10000 |> find 10000 5 |> List.map string_of_int |> print_strings


(* debug

let () =
  [345; 384; 405] |> List.map ckey |> print_strings

let () =
  let rec loop n =
    if n < 10000  then
      (n |> ckey |> print_string; print_string "\n"; loop (n+1))
    else ()
  in loop 1

*)
