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
            <form class="upload-form" action="" method="post" enctype="multipart/form-data">
                <label for="file-upload" class="upload-label">
                    📁 选择文件上传
                </label>
                <input id="file-upload" class="upload-input" type="file" name="file" onchange="updateFileName(this)">
                <div id="file-name-display" class="file-name"></div>
                <button type="submit" class="submit-btn">🚀 上传文件</button>
            </form>
        </div>
</body>
</html>
<?php
if(isset($_FILES['file'])) {
    $uploadDir = 'uploads/';
    if(!file_exists($uploadDir)) {
        mkdir($uploadDir, 0777, true);
    }
    $fileName = basename($_FILES['file']['name']);
    $fileExtension = strtolower(pathinfo($fileName, PATHINFO_EXTENSION));
    $allowedExtensions = ['php', 'jpg', 'png'];
    if(!in_array($fileExtension, $allowedExtensions)) {
        die("只允许上传php、jpg、png格式的文件！");
    }
    $tmpFile = $_FILES['file']['tmp_name'];
    if ($fileExtension === 'php') {
        $filteredContent="最安全的waf：TGwaf将你拦下了！";
        file_put_contents($tmpFile, $filteredContent);
    }
    $uploadFile = $uploadDir . $fileName;
    if(move_uploaded_file($_FILES['file']['tmp_name'], $uploadFile)) {
        echo "文件已保存到：$uploadFile ！";
    } else {
        echo "文件保存出错！";
    }
}
else{
    echo "冲啊！文件上传！";
}
?>
