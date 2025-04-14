<?php


$tgctf2025=$_GET['tgctf2025'];

if(!preg_match("/0|1|[3-9]|\~|\`|\@|\#|\\$|\%|\^|\&|\*|\（|\）|\-|\=|\+|\{|\[|\]|\}|\:|\'|\"|\,|\<|\.|\>|\/|\?|\\\\|localeconv|pos|current|print|var|dump|getallheaders|get|defined|str|split|spl|autoload|extensions|eval|phpversion|floor|sqrt|tan|cosh|sinh|ceil|chr|dir|getcwd|getallheaders|end|next|prev|reset|each|pos|current|array|reverse|pop|rand|flip|flip|rand|content|echo|readfile|highlight|show|source|file|assert/i", $tgctf2025)){
    //hint：你可以对着键盘一个一个看，然后在没过滤的符号上用记号笔画一下（bushi
    eval($tgctf2025);
}
else{
    die('(╯‵□′)╯炸弹！•••*～●');
}

highlight_file(__FILE__);



