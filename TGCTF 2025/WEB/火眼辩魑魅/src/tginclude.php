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
        <div class="code-box">
            <form class="input-form" action="" method="post">
                <input type="text" class="file-input" name="file" placeholder="输入文件路径...">
                <button type="submit" class="submit-btn">🔍 包含文件</button>
            </form>
        </div>
</body>
</html>
<?php
if(isset($_POST['file']) && !empty($_POST['file'])) {
    $file = $_POST['file'];
    $content=file_get_contents($file);
    echo $content;
}
else{
    echo "应该包含什么文件呢？好难猜~";
}
?>
