(* mutable map *)

type ('a 'b) t = {
  mutable length: int;
  mutable keys: a' array;
  mutable vals: b' array;
}

let default_length = 10

let make () =
  { length = 0; keys = Array.create default_length ;

val set   : (a' b') t -> a' -> b' -> unit
val get   : (a' b') t -> a' -> b' option
