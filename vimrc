set nocompatible
set nu 
set showtabline=1
setlocal noswapfile
set tabstop=4
set softtabstop=4
set shiftwidth=4
set cindent
set autoindent
set nobackup
set shortmess=atI
" 通过使用: commands命令，告诉我们文件的哪一行被改变过
set report=0
" 不让vim发出讨厌的滴滴声
set showmatch
" 匹配括号高亮的时间（单位是十分之一秒）
set matchtime=5
set backspace=indent,eol,start
" 在搜索的时候忽略大小写
set ignorecase smartcase
" 高亮搜索关键字
set hlsearch
" 在搜索时，输入的词句的逐字符高亮（类似firefox的搜索）
set incsearch
" :set list 显示内容
set listchars=tab:\|\ ,trail:.,extends:>,precedes:<,eol:$
" 光标移动到buffer的顶部和底部时保持3行距离
set scrolloff=5
" 不要闪烁
" set novisualbell
" 总是显示状态行
set laststatus=2
" 自动格式化
" set formatoptions=tcrqn
" 正确处理中文字符的折行和拼接
set formatoptions+=mM
" set formatoptions+=tcroqn2mBM1
" 继承前一行的缩进方式，特别适用于多行注释
set autoindent
" 为C程序提供自动缩进
set smartindent
" 使用C样式的缩进
set cindent
" 制表符为4
set tabstop=4
" 统一缩进为4
set softtabstop=4
set shiftwidth=4
" 不要用空格代替制表符
set noexpandtab
" 不要换行
" set nowrap
" 在行和段开始处使用制表符
set smarttab
"" 断行设置
" 不要在单词中间断行
set lbr

set nocompatible
set nocompatible
set nu 
colorscheme google
set encoding=utf-8
set termencoding=utf-8
set fileencoding=chinese

set imcmdline
"=================================
"close menu bar
set guioptions-=r
set guioptions-=m
set guioptions-=T
