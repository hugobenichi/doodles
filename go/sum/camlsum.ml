open Str

let (>>) f g x = g(f(x));;

let add f = float_of_string >> (+.) f

let split = " " |> Str.regexp |> Str.split

let sum = function
    [] -> split >> (List.map float_of_string)
  | ac -> split >> (List.map2 add ac)

let rec foldch f acc chan =
  match input_line chan with
  | l -> foldch f (f acc l) chan
  | exception End_of_file -> acc

let sumch = foldch sum [] >> List.map string_of_float >> String.concat " "

let sum_and_print_file filename =
  let chan = open_in filename in
    chan |> sumch |> print_string;
    print_string "\n";
    close_in chan

let () =
  match Sys.argv with
  | [| _; filename |] -> sum_and_print_file filename
  | _ -> failwith "Exactly 1 filename must be given"
