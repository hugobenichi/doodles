(* run with $ ocaml str.cma euler67.ml *)

(* utils *)
let (>>) f g x = g (f x)

let (++) l q = List.concat [l; q]

let max x y = if x > y then x else y


(* read file into lines of list of ints -_- *)
let rec foldch f acc chan =
  match input_line chan with
  | l -> foldch f (f acc l) chan
  | exception End_of_file -> acc

let source f acc =
  let process chan =
    let out = foldch f acc chan in
      close_in chan;
      out
  in  open_in >> process

let readlines = source (fun acc l -> acc ++ [l]) []

let split = " " |> Str.regexp |> Str.split

let to_ints l = l |> split |> List.map int_of_string

let join = List.fold_left (fun s x -> s ^ " " ^ (string_of_int x)) ""


(* actual problem *)
let sum l q = (List.combine l q) |> List.map (fun (x,y) -> x + y)

let rec find_path l =
  let rec loop ls =
    match ls with
    | [] -> [] (* should not happen *)
    | a :: [] -> []
    | a :: b :: t -> (max a b) :: (ls |> List.tl |> loop)
  in if List.length l < 2 then l else loop l

let rec aggreg ls =
  match ls with
  | [] -> []
  | a :: [] -> find_path a
  | a :: t -> t |> aggreg |> sum a |> find_path

let () =
  let lines = "p067_triangle.txt" |> readlines in
  lines |> (List.map to_ints) |> aggreg |> List.hd |> string_of_int |> print_string;
  print_string "\n"
