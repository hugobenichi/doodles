open Core.Std
open String
open List

let ( |> ) x f = f x;;

let x = [1;2;3]
let y = [1]
let z = []

let printls xs =
  let rec join = function
    | [] -> ""
    | x :: [] -> string_of_int x
    | x :: t ->  (string_of_int x) ^ ", " ^ (join t)
  in ("["  ^ (join xs) ^ "]\n") |> print_string

let () =
  print_string "Hellow world\n";
  printls x;
  printls y;
  printls z;
