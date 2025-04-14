<?php
define('UPLOAD_PATH', __DIR__ . '/uploads/');
$is_upload = false;
$msg = null;
$status_code = 200; // 默认状态码为 200
if (isset($_POST['submit'])) {
    if (file_exists(UPLOAD_PATH)) {
        $deny_ext = array("php", "php5", "php4", "php3", "php2", "html", "htm", "phtml", "pht", "jsp", "jspa", "jspx", "jsw", "jsv", "jspf", "jtml", "asp", "aspx", "asa", "asax", "ascx", "ashx", "asmx", "cer", "swf", "htaccess");

        if (isset($_GET['name'])) {
            $file_name = $_GET['name'];
        } else {
            $file_name = basename($_FILES['name']['name']);
        }
        $file_ext = pathinfo($file_name, PATHINFO_EXTENSION);

        if (!in_array($file_ext, $deny_ext)) {
            $temp_file = $_FILES['name']['tmp_name'];
            $file_content = file_get_contents($temp_file);

            if (preg_match('/.+?</s', $file_content)) {
                $msg = '文件内容包含非法字符，禁止上传！';
                $status_code = 403; // 403 表示禁止访问
            } else {
                $img_path = UPLOAD_PATH . $file_name;
                if (move_uploaded_file($temp_file, $img_path)) {
                    $is_upload = true;
                    $msg = '文件上传成功！';
                } else {
                    $msg = '上传出错！';
                    $status_code = 500; // 500 表示服务器内部错误
                }
            }
        } else {
            $msg = '禁止保存为该类型文件！';
            $status_code = 403; // 403 表示禁止访问
        }
    } else {
        $msg = UPLOAD_PATH . '文件夹不存在,请手工创建！';
        $status_code = 404; // 404 表示资源未找到
    }
}

// 设置 HTTP 状态码
http_response_code($status_code);

// 输出结果
echo json_encode([
    'status_code' => $status_code,
    'msg' => $msg,
]);