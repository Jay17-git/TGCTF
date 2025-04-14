from flask import Flask, request, redirect, render_template, render_template_string
import pymysql.cursors
import os


def db():
    return pymysql.connect(
        host=os.environ["MYSQL_HOST"],
        user=os.environ["MYSQL_USER"],
        password=os.environ["MYSQL_PASSWORD"],
        database=os.environ["MYSQL_DATABASE"],
        charset="utf8mb4",
        cursorclass=pymysql.cursors.DictCursor,
    )


app = Flask(__name__)


@app.get("/")
def index():
    if "username" not in request.cookies:
        return redirect("/login")
    return render_template("index.html", username=request.cookies["username"])


@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")

        if username is None or password is None:
            return "要输入账号密码喔~", 400
        if len(username) > 64 or len(password) > 64:
            return "哈~太长了，受不了了~", 400
        if "'" in username or "'" in password:
            return "杂鱼，还想SQL注入？爬！", 400

        conn = None
        try:
            conn = db()
            with conn.cursor() as cursor:
                cursor.execute(
                    f"SELECT * FROM users WHERE username = '{username}' AND password = '{password}'"
                )
                user = cursor.fetchone()
        except Exception as e:
            return f"Error: {e}", 500
        finally:
            if conn is not None:
                conn.close()

        if user is None or "username" not in user:
            return "账号密码错误", 400

        response = redirect("/")
        response.set_cookie("username", user["username"])
        return response
    else:
        return render_template("login.html")
