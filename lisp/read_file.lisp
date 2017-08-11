(defun my-with-file (path fn)
  (let (
        (f (open path)))
    (fn f)
    (close f)))
