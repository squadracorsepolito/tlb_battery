set path+=**
"let g:header='\
"/**\n \
" * @file      adc.c\n \
" * @prefix    uADC (userADC)\n \
" * @author    Simone Ruffini [simone.ruffini.work@gmail.com | simone.ruffini@squadracorse.com]\n \
" * @date      DATE\n \
" * \n \
" * @brief   This file provides code for the configuration\n \
" *          of the ADC instances.\n \
" *\n \
" * @license Licensed under \"THE BEER-WARE LICENSE\", Revision 69 (see LICENSE)\n \
" */ \
"'
nnoremap <F4> :!ptags --include-untracked --include-submodule<enter>
nnoremap <F5> :make -j build <bar> cwindow <enter>
nnoremap <F6> :!make flash<enter>
nnoremap <M-C-F> :w<enter>:!clang-format -style=file -i %<enter>
