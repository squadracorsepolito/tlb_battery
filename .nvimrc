set path+=**
nnoremap <F4> :!ptags --include-untracked --include-submodule<enter>
nnoremap <F5> :make build <bar> cwindow <enter>
nnoremap <F6> :!make flash<enter>
nnoremap <M-C-F> :w<enter>:!clang-format -style=file -i %<enter>
