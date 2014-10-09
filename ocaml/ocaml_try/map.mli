(* map *)

type ('a 'b) t

val make  : unit -> t
val set   : (a' b') t -> a' -> b' -> unit
val get   : (a' b') t -> a' -> b' option
