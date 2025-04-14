from pyramid.config import Configurator
from pyramid.request import Request
from pyramid.response import Response
from pyramid.view import view_config
from wsgiref.simple_server import make_server
from pyramid.events import NewResponse
import re
from jinja2 import Environment, BaseLoader

eval_globals = { #防止eval执行恶意代码
    '__builtins__': {},      # 禁用所有内置函数
    '__import__': None       # 禁止动态导入
}


def checkExpr(expr_input):
    expr = re.split(r"[-+*/]", expr_input)
    print(exec(expr_input))

    if len(expr) != 2:
        return 0
    try:
        int(expr[0])
        int(expr[1])
    except:
        return 0

    return 1


def home_view(request):
    expr_input = ""
    result = ""

    if request.method == 'POST':
        expr_input = request.POST['expr']
        if checkExpr(expr_input):
            try:
                result = eval(expr_input, eval_globals)
            except Exception as e:
                result = e
        else:
            result = "爬！"


    template_str = '''
    <!doctype html>
    <html>
    <head>
        <meta charset="utf-8">
        <title>Calculator with Code Highlighting</title>
        <link rel="stylesheet"
              href="//cdnjs.cloudflare.com/ajax/libs/highlight.js/11.7.0/styles/default.min.css">
        <script src="//cdnjs.cloudflare.com/ajax/libs/highlight.js/11.7.0/highlight.min.js"></script>
        <script>hljs.highlightAll();</script>
        <style>
            body { font-family: Arial, sans-serif; margin: 20px; }
            pre { background: #f5f5f5; padding: 10px; }
        </style>
    </head>
    <body>
        <h1>Calculator</h1>
        <form method="post">
            <label for="expr">输入表达式:</label>
            <input type="text" name="expr" id="expr" value="{{ expr_input }}" placeholder="输入二元表达式，例如: 1+2 或 3-4 或 5*6 或 7/8" style="width: 300px;">
            <button type="submit">计算</button>
        </form>
        {% if result != "" %}
        <h2>结果: {{ result }}</h2>
        {% endif %}

        <h2>代码:</h2>
        <pre><code class="python">
{% raw %}
from pyramid.config import Configurator
from pyramid.request import Request
from pyramid.response import Response
from pyramid.view import view_config
from wsgiref.simple_server import make_server
from pyramid.events import NewResponse
import re
from jinja2 import Environment, BaseLoader

eval_globals = { #防止eval执行恶意代码
    '__builtins__': {},      # 禁用所有内置函数
    '__import__': None       # 禁止动态导入
}


def checkExpr(expr_input):
    expr = re.split(r"[-+*/]", expr_input)
    print(exec(expr_input))

    if len(expr) != 2:
        return 0
    try:
        int(expr[0])
        int(expr[1])
    except:
        return 0

    return 1


def home_view(request):
    expr_input = ""
    result = ""

    if request.method == 'POST':
        expr_input = request.POST['expr']
        if checkExpr(expr_input):
            try:
                result = eval(expr_input, eval_globals)
            except Exception as e:
                result = e
        else:
            result = "爬！"


    template_str = 【xxx】

    env = Environment(loader=BaseLoader())
    template = env.from_string(template_str)
    rendered = template.render(expr_input=expr_input, result=result)
    return Response(rendered)


if __name__ == '__main__':
    with Configurator() as config:
        config.add_route('home_view', '/')
        config.add_view(home_view, route_name='home_view')
        app = config.make_wsgi_app()

    server = make_server('0.0.0.0', 9040, app)
    server.serve_forever()
{% endraw %}
        </code></pre>
    </body>
    </html>
    '''

    env = Environment(loader=BaseLoader())
    template = env.from_string(template_str)
    rendered = template.render(expr_input=expr_input, result=result)
    return Response(rendered)


if __name__ == '__main__':
    with Configurator() as config:
        config.add_route('home_view', '/')
        config.add_view(home_view, route_name='home_view')
        app = config.make_wsgi_app()

    server = make_server('0.0.0.0', 9040, app)
    server.serve_forever()

