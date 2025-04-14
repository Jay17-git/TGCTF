import os
import string
from flask import Flask, request, render_template_string, jsonify, send_from_directory
from a.b.c.d.secret import secret_key

app = Flask(__name__)

black_list=['lipsum','|','%','{','}','map','chr', 'value', 'get', "url", 'pop','include','popen','os','import','eval','_','system','read','base','globals','_.','set','application','getitem','request', '+', 'init', 'arg', 'config', 'app', 'self']
def waf(name):
    for x in black_list:
        if x in name.lower():
            return True
    return False
def is_typable(char):
    # 定义可通过标准 QWERTY 键盘输入的字符集
    typable_chars = string.ascii_letters + string.digits + string.punctuation + string.whitespace
    return char in typable_chars

@app.route('/')
def home():
    return send_from_directory('static', 'index.html')

@app.route('/jingu', methods=['POST'])
def greet():
    template1=""
    template2=""
    name = request.form.get('name')
    template = f'{name}'
    if waf(name):
        template = '想干坏事了是吧hacker？哼，还天命人，可笑，可悲，可叹<br><img src="{{  url_for("static", filename="3.jpeg") }}" alt="Image">'
    else:
        k=0
        for i in name:
            if is_typable(i):
                continue
            k=1
            break
        if k==1:
            if not (secret_key[:2] in name and secret_key[2:]):
                template = '连“六根”都凑不齐，谈什么天命不天命的，还是戴上这金箍吧<br><br>再去西行历练历练<br><br><img src="{{  url_for("static", filename="4.jpeg") }}" alt="Image">'
                return render_template_string(template)
            template1 = "“六根”也凑齐了，你已经可以直面天命了！我帮你把“secret_key”替换为了“{{}}”<br>最后，如果你用了cat，就可以见到齐天大圣了<br>"
            template= template.replace("天命","{{").replace("难违","}}")
            template = template
    if "cat" in template:
        template2 = '<br>或许你这只叫天命人的猴子，真的能做到？<br><br><img src="{{  url_for("static", filename="2.jpeg") }}" alt="Image">'
    try:
        return template1+render_template_string(template)+render_template_string(template2)
    except Exception as e:
        error_message = f"500报错了，查询语句如下：<br>{template}"
        return error_message, 400

@app.route('/hint', methods=['GET'])
def hinter():
    template="hint：<br>有一个aazz路由，去那里看看吧，天命人!"
    return render_template_string(template)

@app.route('/aazz', methods=['GET'])
def finder():
    with open(__file__, 'r') as f:
        source_code = f.read()
    return f"<pre>{source_code}</pre>", 200, {'Content-Type': 'text/html; charset=utf-8'}

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)