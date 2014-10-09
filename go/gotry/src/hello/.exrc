if &cp | set nocp | endif
map  :w
nmap   zz
nmap \\ :q
let s:cpo_save=&cpo
set cpo&vim
nmap gx <Plug>NetrwBrowseX
map gvl :Scratch:r!git log #
map gvb :Scratch:r!git blame #
map gvD :Scratch:r!git diff:runtime syntax/diff.vim
map gvd :Scratch:r!git diff #:runtime syntax/diff.vim
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
map <ScrollWheelDown> 
map <ScrollWheelUp> 
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set background=dark
set backspace=2
set expandtab
set fileencodings=ucs-bom,utf-8,default,latin1
set hlsearch
set modelines=0
set mouse=a
set ruler
set runtimepath=~/.vim,/usr/share/vim/vimfiles,/usr/share/vim/vim73,/usr/share/vim/vimfiles/after,~/.vim/after,/ocamlmerlin/vim,/ocamlmerlin/vimbufsync
set shiftwidth=2
set showmatch
set softtabstop=2
set noswapfile
set tabstop=2
set tags=tags;/
set window=0
" vim: set ft=vim :
