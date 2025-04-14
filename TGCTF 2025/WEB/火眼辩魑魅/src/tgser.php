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
class road{
    public function __get($arg1) {
        $blacklist = ['DirectoryIterator', 'Error', 'Exception', 'SoapClient', 'FilesystemIterator', 'SimpleXMLElement', 'GlobIterator', 'ReflectionClass', 'ReflectionObject'];
        array_walk($this, function ($day1, $day2) use ($blacklist) {
            if (in_array($day2, $blacklist)) {die('hacker');}
            $day3 = new $day2($day1);
            foreach ($day3 as $day4) {
                echo ($day4 . '<br>');
            }
        });
    }
}

class river{
    public $fish;
    public $shark;
    public function __invoke() {
        if (md5($this->fish) == $this->fish) {
            return $this->shark->upper;
        }
    }
}

class sky{
    public $bird;
    public $eagle;
    public function __construct($a) {
        $this->bird = $a;
    }
    function __destruct() {
        echo $this->bird;
    }
    public function __toString() {
        $newFunc = $this->eagle;
        return $newFunc();
    }
}
$way=$_POST['J'];
@unserialize(base64_decode($way));
?>
CODE;
echo htmlspecialchars($source_code);
?></code></pre>
</div>
</body>
</html>
<?php
error_reporting(0);
class road{
    public function __get($arg1) {
        $blacklist = ['DirectoryIterator', 'Error', 'Exception', 'SoapClient', 'FilesystemIterator', 'SimpleXMLElement', 'GlobIterator', 'ReflectionClass', 'ReflectionObject'];
        array_walk($this, function ($day1, $day2) use ($blacklist) {
            if (in_array($day2, $blacklist)) {die('hacker');}
            $day3 = new $day2($day1);
            foreach ($day3 as $day4) {
                echo ($day4 . '<br>');
            }
        });
    }
}

class river{
    public $fish;
    public $shark;
    public function __invoke() {
        if (md5($this->fish) == $this->fish) {
            return $this->shark->upper;
        }
    }
}

class sky{
    public $bird;
    public $eagle;
    public function __construct($a) {
        $this->bird = $a;
    }
    function __destruct() {
        echo $this->bird;
    }
    public function __toString() {
        $newFunc = $this->eagle;
        return $newFunc();
    }
}
$way=$_POST['J'];
@unserialize(base64_decode($way));
?>
