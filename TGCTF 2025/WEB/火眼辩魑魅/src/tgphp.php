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
    error_reporting(0);
    $code = $_GET['code'] ?? '';
    $white_list = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '$', '\\', ';'];
    for ($i = 0; $i < strlen($code); $i++) {
        $char = $code[$i];
        if (!in_array($char, $white_list)) {
            die("检测到非法字符: " . htmlspecialchars($char) . "！只允许使用数字0-9和小部分符号哦！");
        }
    }
    eval($code);
?>
CODE;
echo htmlspecialchars($source_code)."\n";
echo "你真的可以尝试打破这个数字牢笼！";
?></code></pre>
</div>
</body>
</html>
<?php
    error_reporting(0);
    $code = $_GET['code'] ?? '';
    $white_list = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '$', '\\', ';'];
    for ($i = 0; $i < strlen($code); $i++) {
        $char = $code[$i];
        if (!in_array($char, $white_list)) {
            die("检测到非法字符: " . htmlspecialchars($char) . "！只允许使用数字0-9和小部分符号哦！");
        }
    }
    eval($code);
?>