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
                <?php
                error_reporting(0);
                require './Smarty/libs/Smarty.class.php';
                $smarty = new Smarty();
                $ip = isset($_SERVER['HTTP_X_FORWARDED_FOR']) ? $_SERVER['HTTP_X_FORWARDED_FOR'] : $_SERVER['REMOTE_ADDR'];
                echo "OvO 你电脑的IP是：";
                $smarty->display("string:".$ip);
                ?>
            </form>
        </div>
</body>
</html>
