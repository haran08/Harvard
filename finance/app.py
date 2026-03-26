import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
from enum import Enum

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


# Invalid characters for sql queries
invalid_chars = [";", ":", "`", "\'", "\"", "{", "}",
                    "[", "]", "(", ")", "~", "<", ">", "\\",
                    "/", "-", "+", "=", "*", "&", "%", "$", "!",
                    ",", "^", "|"]

class E_transaction(Enum):
    SELL = 1
    BUY  = 2

# Functions
def add_to_history(id, symbol, count, price, transaction):
    now = datetime.now().strftime("%Y-%m-%d || %H:%M:%S")

    db.execute(
        "INSERT INTO users_historic (user_id, share_symbol, share_count, price, "
        "transaction_type, datetime) VALUES(?, ?, ?, ?, ?, ?)", id, symbol, count,
        price, transaction, now
        )


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user = session["user_id"]

    stock = db.execute("SELECT * FROM users_shares WHERE user_id = ?", user)
    money = db.execute("SELECT cash FROM users WHERE id = ?", user)[0]["cash"]

    stks = list()
    i = 0
    for stk in stock:
        stks.append(dict())

        price = lookup(stk["share_symbol"])["price"]
        stks[i]["symbol"] = stk["share_symbol"]
        stks[i]["shares"] = stk["share_count"]
        stks[i]["price"]  = usd(price)
        stks[i]["total"]  = usd( float(stk["share_count"]) * float(price) )

        i += 1

    total_money = money
    for stk in stks:
        total_money += float(stk["total"].replace("$", "").replace(",", ""))

    return render_template("/index.html", stocks = stks, cash = usd(money),
                           total = usd(total_money))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        sym   = request.form.get("symbol")
        count = request.form.get("shares")

        stk = lookup(sym)

        if not stk:
            return apology("Stock invalid.", 400)


        if not count.isdecimal():
            return apology("Invalid shares count", 400)

        shares_count = int(count)

        if shares_count < 1:
            return apology("Invalid shares count", 400)

        user  = session["user_id"]
        _cash = db.execute("SELECT cash FROM users WHERE id = ?", user)
        cash = _cash[0]["cash"]

        price = float(stk["price"])
        if cash < price * shares_count:
            return apology("You dont have enough cash", 400)

        else:
            up_cash = cash - price * shares_count

            db.execute("UPDATE users SET cash = ? WHERE id = ?", up_cash, user)

            user_shares = db.execute("SELECT share_count FROM users_shares WHERE " +
                                        "user_id = ? AND share_symbol = ?",
                                        user, stk["symbol"])

            if len(user_shares) < 1:
                db.execute("INSERT INTO users_shares (user_id, share_symbol, share_count) "
                            "VALUES (?, ?, ?)", user, stk["symbol"], shares_count)
            else:
                db.execute("UPDATE users_shares SET share_count = ? "
                            "WHERE user_id = ? and share_symbol = ?",
                            shares_count + user_shares[0]["share_count"],
                            user, stk["symbol"])

            add_to_history(user, stk["symbol"], shares_count, stk["price"],
                            E_transaction.BUY.name)

        return redirect("/")

    else:
        return render_template("/buy.html", symbol="true")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user = session["user_id"]

    history = db.execute(
        "SELECT * FROM users_historic WHERE user_id = ?", user
    )
    history.reverse()

    for s in history:
        s["usd_price"] = usd(s["price"])

    return render_template("/history.html", stocks = history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        username = request.form.get("username")

        if any(c in username for c in invalid_chars):
            return apology("Username invalid", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", username
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        stock_symbol = request.form.get("symbol")

        if not stock_symbol:
            return apology("No stock symbol passed", 400)

        #search for stock symbol
        stk = lookup(stock_symbol)

        if not stk:
            return apology("Invalid stock symbol", 400)

        return render_template("/quoted.html", symbol = stk["symbol"], price = usd(stk["price"]))

    else:
        return render_template("/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User got to this route by submit a form
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirm  = request.form.get("confirmation")

        if not username:
            return apology("Must provide username", 400)

        if not password:
            return apology("Must provide password", 400)

        if password != confirm:
            return apology("Must provide password", 400)

        if any(c in username for c in invalid_chars):
            return apology("Username invalid", 400)

        row = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(row) != 0:
            return apology("Username alreadly exist", 400)

        passhash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?)", [username, passhash])
        id = db.execute("SELECT id FROM users WHERE username = ?", username)

        session["user_id"] = id[0]["id"]

        return redirect("/")

    # User got to this route by a link ou redirect
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user = session["user_id"]

    if request.method == "POST":
        symbol = request.form.get("symbol")
        count  = request.form.get("shares")

        if not symbol or not lookup(symbol):
            stks = db.execute("SELECT * FROM users_shares WHERE user_id = ?", user)
            return render_template("/sell.html", stocks = stks, invalid_stock = True)

        share_count = db.execute("SELECT share_count FROM users_shares WHERE "
                                  "user_id = ? AND share_symbol = ?",
                                  user, symbol)
        if not share_count:
            stks = db.execute("SELECT * FROM users_shares WHERE user_id = ?", user)
            return render_template("/sell.html", stocks = stks, invalid_user_stock = True)

        if not count or not count.isdecimal() or int(count) < 1:
            return apology("Invalid number of shares", 400)

        elif int(share_count[0]["share_count"]) < int(count):
            return apology("Invalid number of shares", 400)

        i_share_count   = int(share_count[0]["share_count"])
        i_count         = int(count)
        stock           = lookup(symbol)
        new_share_count = i_share_count - i_count
        money           = float(stock["price"]) * i_count
        cash            = float(db.execute("SELECT cash FROM users WHERE id = ?",
                                           user)[0]["cash"])
        new_cash        = cash + money

        if new_share_count == 0:
            db.execute("DELETE FROM users_shares WHERE user_id = ? AND "
                   "share_symbol = ?", user, symbol)
        else:
            db.execute("UPDATE users_shares SET share_count = ? WHERE user_id = ? AND "
                   "share_symbol = ?", new_share_count, user, symbol)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user)

        add_to_history(user, stock["symbol"], i_count, stock["price"],
                E_transaction.SELL.name)

        return redirect("/")

    else:
        stks = db.execute("SELECT * FROM users_shares WHERE user_id = ?", user)
        return render_template("/sell.html", stocks = stks)


@app.route("/config", methods=["GET"])
@login_required
def config():
    """User configurations"""

    return render_template("/config.html")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """User password Change"""

    if request.method == "POST":
        old_password     = request.form.get("old_password")
        new_password     = request.form.get("new_password")
        confirm_password = request.form.get("confirm_password")
        user_p           = db.execute("SELECT * FROM users WHERE id = ?",
                                        session["user_id"])[0]["hash"]

        if not check_password_hash(user_p, old_password):
            return render_template("/change_password.html", wrong_pass = True)

        elif new_password != confirm_password:
            return render_template("/change_password.html", no_macth_pass = True)

        elif check_password_hash(user_p, new_password):
            return render_template("/change_password.html", same_pass = True)


        hash_p = generate_password_hash(new_password)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hash_p, session["user_id"])

        return render_template("/config.html", pass_changed = True)

    else:
        return render_template("/change_password.html")


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """User deposit money"""

    if request.method == "POST":
        money  = request.form.get("value")
        user_m = db.execute("SELECT * FROM users WHERE id = ?",
                            session["user_id"])[0]["cash"]

        new_money = float(user_m) + float(money)
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   new_money, session["user_id"])


        return render_template("/config.html", up_cash = usd(new_money))

    else:
        return render_template("/deposit.html")
