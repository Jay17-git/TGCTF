<!DOCTYPE html>
<html lang="zh">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>TGCTF-青春CTFer</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
<div class="container">
    <div class="decoration">(>ω<)</div>
    <div class="decoration">(◕‿◕✿)</div>
    <div class="message-box">
        <div class="blush left"></div>
        <div class="blush right"></div>
        <div class="code-box"><pre><code><?php
                    $source_code = <<<'CODE'
<?php
    $shell=$_POST["shell"];
    {
        eval($shell);
    }
?>
CODE;
echo htmlspecialchars($source_code);
?></code></pre>
        </div>
</body>
</html>
<?php
if ($_POST["shell"]){
    $shell=$_POST["shell"];
    if(!preg_match("/openlog|syslog|readlink|symlink|popepassthru|stream_socket_server|scandir|assert|pcntl_exec|fwrite|curl|system|eval|assert|flag|passthru|exec|chroot|chgrp|chown|shell_exec|proc_open|proc_get_status|popen|ini_alter|ini_restore/i", $shell))
    {
        eval($shell);
    }
    else{
        die("你明明知道你被waf了，为什么还在尝试？。？");
    }
}
else{
    echo "哇，贞德是你鸭！";
}
?>
